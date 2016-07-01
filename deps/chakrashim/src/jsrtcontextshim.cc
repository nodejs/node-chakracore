// Copyright Microsoft. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and / or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.

#include "v8chakra.h"
#include "chakra_natives.h"
#include <algorithm>
#include <memory>

namespace jsrt {

using jsrt::chakra_shim_native;

ContextShim::Scope::Scope(ContextShim * contextShim) {
  contextShim->GetIsolateShim()->PushScope(this, contextShim);
}

ContextShim::Scope::~Scope() {
  this->contextShim->GetIsolateShim()->PopScope(this);
}

ContextShim * ContextShim::New(IsolateShim * isolateShim, bool exposeGC,
                               JsValueRef globalObjectTemplateInstance) {
  JsContextRef context;
  if (JsCreateContext(isolateShim->GetRuntimeHandle(), &context) != JsNoError) {
    return nullptr;
  }

  // AddRef on globalObjectTemplateInstance if specified. Save and use later.
  if (globalObjectTemplateInstance != JS_INVALID_REFERENCE &&
      JsAddRef(globalObjectTemplateInstance, nullptr) != JsNoError) {
    return nullptr;
  }

  return new ContextShim(isolateShim, context, exposeGC,
                         globalObjectTemplateInstance);
}

#define DEF_IS_TYPE(F) , F##Function(JS_INVALID_REFERENCE)

ContextShim::ContextShim(IsolateShim * isolateShim,
                         JsContextRef context,
                         bool exposeGC,
                         JsValueRef globalObjectTemplateInstance)
    : isolateShim(isolateShim),
      context(context),
      initialized(false),
      exposeGC(exposeGC),
      globalObjectTemplateInstance(globalObjectTemplateInstance),
      getOwnPropertyDescriptorFunction(JS_INVALID_REFERENCE),
      promiseContinuationFunction(JS_INVALID_REFERENCE),
      cloneObjectFunction(JS_INVALID_REFERENCE),
      getPropertyNamesFunction(JS_INVALID_REFERENCE),
      getEnumerableNamedPropertiesFunction(JS_INVALID_REFERENCE),
      getEnumerableIndexedPropertiesFunction(JS_INVALID_REFERENCE),
      createEnumerationIteratorFunction(JS_INVALID_REFERENCE),
      createPropertyDescriptorsEnumerationIteratorFunction
        (JS_INVALID_REFERENCE),
      getNamedOwnKeysFunction(JS_INVALID_REFERENCE),
      getIndexedOwnKeysFunction(JS_INVALID_REFERENCE),
      getStackTraceFunction(JS_INVALID_REFERENCE),
      getSymbolKeyForFunction(JS_INVALID_REFERENCE),
      getSymbolForFunction(JS_INVALID_REFERENCE),
      ensureDebugFunction(JS_INVALID_REFERENCE),
      enqueueMicrotaskFunction(JS_INVALID_REFERENCE),
      dequeueMicrotaskFunction(JS_INVALID_REFERENCE)
#include "jsrtcachedpropertyidref.inc"
      {
  memset(globalConstructor, 0, sizeof(globalConstructor));
  memset(globalPrototypeFunction, 0, sizeof(globalPrototypeFunction));
}

ContextShim::~ContextShim() {
  if (globalObjectTemplateInstance != JS_INVALID_REFERENCE) {
    JsRelease(globalObjectTemplateInstance, nullptr);
  }
}

bool ContextShim::CheckConfigGlobalObjectTemplate() {
  if (globalObjectTemplateInstance != JS_INVALID_REFERENCE) {
    // Only need to config once. Discard globalObjectTemplateInstance
    JsValueRef newProto = globalObjectTemplateInstance;
    JsRelease(globalObjectTemplateInstance, nullptr);
    globalObjectTemplateInstance = JS_INVALID_REFERENCE;

    JsValueRef glob, oldProto;
    return newProto != JS_INVALID_REFERENCE &&
      JsGetGlobalObject(&glob) == JsNoError &&
      JsGetPrototype(glob, &oldProto) == JsNoError &&
      JsSetPrototype(newProto, oldProto) == JsNoError &&
      JsSetPrototype(glob, newProto) == JsNoError;
  }

  return true;
}

IsolateShim * ContextShim::GetIsolateShim() {
  return isolateShim;
}

JsContextRef ContextShim::GetContextRef() {
  return context;
}

ContextShim * ContextShim::GetCurrent() {
  return IsolateShim::GetCurrent()->GetCurrentContextShim();
}

void* ContextShim::GetAlignedPointerFromEmbedderData(int index) {
  if (index >= 0 &&
      static_cast<std::vector<void*>::size_type>(index) < embedderData.size()) {
    return embedderData.at(index);
  }
  return nullptr;
}

bool ContextShim::KeepAlive(JsValueRef value) {
  // use an object set as a proeprty of the global object to keep object alive
  // for the duration of the context
  JsValueRef builtInIndex;
  if (JsIntToNumber(builtInCount, &builtInIndex) != JsNoError) {
    return false;
  }
  if (JsSetIndexedProperty(keepAliveObject, builtInIndex, value) != JsNoError) {
    return false;
  }
  builtInCount++;
  return true;
}

template <typename Fn>
bool ContextShim::InitializeBuiltIn(JsValueRef * builtInValue, Fn getBuiltIn) {
  JsValueRef value;
  if (getBuiltIn(&value) != JsNoError) {
    return false;
  }
  if (!KeepAlive(value)) {
    return false;
  }
  *builtInValue = value;
  return true;
}

bool ContextShim::InitializeBuiltIn(JsValueRef * builtInValue,
                                    const char* globalName) {
  return InitializeBuiltIn(builtInValue,
                           [=](JsValueRef * value) {
    return jsrt::GetProperty(globalObject, globalName, value);
  });
}

static const char* s_globalTypeNames[] = {
#define DEFTYPE(T) #T,
#include "jsrtcontextcachedobj.inc"
  ""
};

bool ContextShim::InitializeGlobalTypes() {
  for (int i = 0; i < GlobalType::_TypeCount; i++) {
    if (!InitializeBuiltIn(&globalConstructor[i], s_globalTypeNames[i])) {
      return false;
    }
  }
  return true;
}

bool ContextShim::InitializeGlobalPrototypeFunctions() {
  IsolateShim* iso = GetIsolateShim();
  JsPropertyIdRef prototypeIdRef =
    iso->GetCachedPropertyIdRef(CachedPropertyIdRef::prototype);

  auto init = [=](GlobalPrototypeFunction index,
                  GlobalType type, JsPropertyIdRef functionIdRef) {
    // Cache the builtin function on the type's prototype
    JsValueRef prototype;
    return InitializeBuiltIn(&globalPrototypeFunction[index],
                           [=, &prototype](JsValueRef * value) {
      JsErrorCode error = JsGetProperty(
        globalConstructor[type], prototypeIdRef, &prototype);
      if (error != JsNoError) {
        return error;
      }

      return JsGetProperty(prototype, functionIdRef, value);
    });
  };

  struct TypeMethodPair {
    GlobalType type;
    JsPropertyIdRef functionIdRef;
  };

  const TypeMethodPair pairs[GlobalPrototypeFunction::_FunctionCount] = {
#define DEFMETHOD(T, M) { \
    GlobalType::T, \
    iso->GetCachedPropertyIdRef(CachedPropertyIdRef::M) },
#include "jsrtcontextcachedobj.inc"
  };

  for (size_t i = 0; i < _countof(pairs); i++) {
    if (!init(static_cast<GlobalPrototypeFunction>(i),
              pairs[i].type, pairs[i].functionIdRef)) {
      return false;
    }
  }

  return true;
}

// Replace (cached) Object.prototype.toString with a shim to support
// ObjectTemplate class name. Called after InitializeGlobalPrototypeFunctions().
bool ContextShim::InitializeObjectPrototypeToStringShim() {
  IsolateShim* iso = GetIsolateShim();

  JsValueRef objectPrototype;
  if (JsGetProperty(GetObjectConstructor(),
                    iso->GetCachedPropertyIdRef(CachedPropertyIdRef::prototype),
                    &objectPrototype) != JsNoError) {
    return false;
  }

  JsValueRef function;
  if (JsCreateFunction(v8::Utils::ObjectPrototypeToStringShim,
                       GetGlobalPrototypeFunction(
                         GlobalPrototypeFunction::Object_toString),
                       &function) != JsNoError) {
    return false;
  }

  if (JsSetProperty(objectPrototype,
                    iso->GetCachedPropertyIdRef(CachedPropertyIdRef::toString),
                    function, false) != JsNoError) {
    return false;
  }

  globalPrototypeFunction[GlobalPrototypeFunction::Object_toString] = function;
  return true;
}

bool ContextShim::InitializeBuiltIns() {
  // No need to keep the global object alive, the context will implicitly
  if (JsGetGlobalObject(&globalObject) != JsNoError) {
    return false;
  }

  JsValueRef newKeepAliveObject;
  // Create an object to keep these built in alive along with the global object
  builtInCount = 0;
  if (JsCreateObject(&newKeepAliveObject) != JsNoError) {
    return false;
  }
  keepAliveObject = newKeepAliveObject;
  // true and false is needed by DefineProperty to create the property
  // descriptor
  if (!InitializeBuiltIn(&trueRef, JsGetTrueValue)) {
    return false;
  }
  if (!InitializeBuiltIn(&falseRef, JsGetFalseValue)) {
    return false;
  }
  if (!InitializeBuiltIn(&undefinedRef, JsGetUndefinedValue)) {
    return false;
  }
  if (!InitializeBuiltIn(&nullRef, JsGetNullValue)) {
    return false;
  }
  if (!InitializeBuiltIn(&zero,
                         [](JsValueRef * value) {
                           return JsIntToNumber(0, value);
                         })) {
    return false;
  }

  if (!InitializeGlobalTypes()) {
    return false;
  }
  if (!InitializeGlobalPrototypeFunctions()) {
    return false;
  }
  if (!InitializeObjectPrototypeToStringShim()) {
    return false;
  }

  if (!InitializeBuiltIn(&getOwnPropertyDescriptorFunction,
                         [this](JsValueRef * value) {
                           return GetProperty(
                             GetObjectConstructor(),
                             CachedPropertyIdRef::getOwnPropertyDescriptor,
                             value);
                         })) {
    return false;
  }

  if (!InitializeReflect()) {
    return false;
  }

  if (DefineProperty(globalObject,
                     GetIsolateShim()->GetKeepAliveObjectSymbolPropertyIdRef(),
                     PropertyDescriptorOptionValues::False,
                     PropertyDescriptorOptionValues::False,
                     PropertyDescriptorOptionValues::False,
                     newKeepAliveObject,
                     JS_INVALID_REFERENCE, JS_INVALID_REFERENCE) != JsNoError) {
    return false;
  }

  if (!InitializeProxyOfGlobal()) {
    return false;
  }

  return true;
}

static JsValueRef CHAKRA_CALLBACK ProxyOfGlobalGetPrototypeOfCallback(
    JsValueRef callee,
    bool isConstructCall,
    JsValueRef *arguments,
    unsigned short argumentCount,
    void *callbackState) {
  // Return the target (which is the global object)
  return arguments[1];
}

bool ContextShim::InitializeReflect() {
  if (!InitializeBuiltIn(&reflectObject,
                         [](JsValueRef * value) {
                           return GetPropertyOfGlobal("Reflect", value);
                         })) {
    return false;
  }

  for (unsigned int i = 0; i < ProxyTraps::TrapCount; i++) {
    if (!InitializeBuiltIn(&reflectFunctions[i],
        [this, i](JsValueRef * value) {
          return JsGetProperty(reflectObject,
              this->GetIsolateShim()->GetProxyTrapPropertyIdRef((ProxyTraps)i),
              value);
        })) {
      return false;
    }
  }
  return true;
}

bool ContextShim::InitializeProxyOfGlobal() {
  return InitializeBuiltIn(&proxyOfGlobal,
                           [this](JsValueRef * value) {
    // V8 Global is actually proxy where the actual global is it's prototype.
    // We will fake it here.
    JsNativeFunction proxyTraps[ProxyTraps::TrapCount] = {};
    proxyTraps[GetPrototypeOfTrap] = ProxyOfGlobalGetPrototypeOfCallback;
    return CreateProxy(globalObject, proxyTraps, value);
  });
}

bool ContextShim::EnsureInitialized() {
  if (initialized) {
    return true;
  }

  if (jsrt::InitializePromise() != JsNoError) {
    return false;
  }

  if (exposeGC && !ExposeGc()) {
    return false;
  }

  if (!InitializeBuiltIns()) {
    return false;
  }

  if (!ExecuteChakraShimJS()) {
    return false;
  }

  initialized = true;

  // Following is a special one-time initialization that needs to marshal
  // objects to this context. Do it after marking initialized = true.
  if (!CheckConfigGlobalObjectTemplate()) {
    return false;
  }

  // add idleGC callback into prepareQueue
  if (IsolateShim::IsIdleGcEnabled()) {
    uv_prepare_start(IsolateShim::GetCurrent()->idleGc_prepare_handle(), PrepareIdleGC);
  }

  return true;
}

bool ContextShim::ExposeGc() {
  JsValueRef collectGarbageRef;

  if (JsCreateFunction(jsrt::CollectGarbage, nullptr,
                       &collectGarbageRef) != JsNoError) {
    return false;
  }

  if (jsrt::SetPropertyOfGlobal("gc", collectGarbageRef) != JsNoError) {
    return false;
  }

  return true;
}

bool ContextShim::ExecuteChakraShimJS() {
  // xplat-todo: Currently chakra_shim_native is not null-terminated, but JSRT
  // API requires null-terminator. Make a copy and null-terminate it.
  // Remove this when new API signature available.
  char buffer[_countof(jsrt::chakra_shim_native) + 1];
  memmove(buffer, chakra_shim_native, _countof(chakra_shim_native));
  buffer[_countof(chakra_shim_native)] = '\0';

  JsValueRef getInitFunction;
  if (JsParseScriptUtf8(buffer,
                    JS_SOURCE_CONTEXT_NONE,
                    "chakra_shim.js",
                    &getInitFunction) != JsNoError) {
    return false;
  }
  JsValueRef initFunction;
  if (CallFunction(getInitFunction, &initFunction) != JsNoError) {
    return false;
  }
  JsValueRef result;
  JsValueRef arguments[] = { this->globalObject, this->keepAliveObject };
  return JsCallFunction(initFunction, arguments, _countof(arguments),
                        &result) == JsNoError;
}

void ContextShim::SetAlignedPointerInEmbedderData(int index, void * value) {
  if (index < 0) {
    return;
  }

  try {
    auto minSize = static_cast<std::vector<void*>::size_type>(index) + 1;
    if (embedderData.size() < minSize) {
      embedderData.resize(minSize);
    }
    embedderData[index] = value;
  } catch(const std::exception&) {
  }
}

void ContextShim::RunMicrotasks() {
  JsValueRef dequeueMicrotaskFunction = GetdequeueMicrotaskFunction();

  for (;;) {
    JsValueRef task;
    if (jsrt::CallFunction(dequeueMicrotaskFunction, &task) != JsNoError ||
      reinterpret_cast<v8::Value*>(task)->IsUndefined()) {
      break;
    }

    JsValueRef notUsed;
    if (jsrt::CallFunction(task, &notUsed) != JsNoError) {
      JsGetAndClearException(&notUsed); // swallow any exception from task
    }
  }
}

JsValueRef ContextShim::GetUndefined() {
  return undefinedRef;
}

JsValueRef ContextShim::GetTrue() {
  return trueRef;
}

JsValueRef ContextShim::GetFalse() {
  return falseRef;
}

JsValueRef ContextShim::GetNull() {
  return nullRef;
}

JsValueRef ContextShim::GetZero() {
  return zero;
}

JsValueRef ContextShim::GetObjectConstructor() {
  return globalConstructor[GlobalType::Object];
}

JsValueRef ContextShim::GetBooleanObjectConstructor() {
  return globalConstructor[GlobalType::Boolean];
}

JsValueRef ContextShim::GetNumberObjectConstructor() {
  return globalConstructor[GlobalType::Number];
}

JsValueRef ContextShim::GetStringObjectConstructor() {
  return globalConstructor[GlobalType::String];
}

JsValueRef ContextShim::GetDateConstructor() {
  return globalConstructor[GlobalType::Date];
}

JsValueRef ContextShim::GetProxyConstructor() {
  return globalConstructor[GlobalType::Proxy];
}

JsValueRef ContextShim::GetGlobalType(GlobalType index) {
  return globalConstructor[index];
}

JsValueRef ContextShim::GetGetOwnPropertyDescriptorFunction() {
  return getOwnPropertyDescriptorFunction;
}

JsValueRef ContextShim::GetStringConcatFunction() {
  return globalPrototypeFunction[GlobalPrototypeFunction::String_concat];
}

JsValueRef ContextShim::GetGlobalPrototypeFunction(
    GlobalPrototypeFunction index) {
  return globalPrototypeFunction[index];
}

JsValueRef ContextShim::GetProxyOfGlobal() {
  return proxyOfGlobal;
}

JsValueRef ContextShim::GetReflectObject() {
  return reflectObject;
}

JsValueRef ContextShim::GetReflectFunctionForTrap(ProxyTraps trap) {
  return reflectFunctions[trap];
}

JsValueRef ContextShim::GetCachedShimFunction(CachedPropertyIdRef id,
                                              JsValueRef* func) {
  if (*func == JS_INVALID_REFERENCE) {
    // chakra_shim.js has initialize the function an put it in keepAliveObject.
    JsErrorCode error = JsGetProperty(keepAliveObject,
                  GetIsolateShim()->GetCachedPropertyIdRef(id),
                  func);
    CHAKRA_VERIFY(error == JsNoError);
  }

  return *func;
}

#define CHAKRASHIM_FUNCTION_GETTER(F) \
JsValueRef ContextShim::Get##F##Function() { \
return GetCachedShimFunction(CachedPropertyIdRef::F, \
                             &F##Function); \
} \

CHAKRASHIM_FUNCTION_GETTER(cloneObject)
CHAKRASHIM_FUNCTION_GETTER(getPropertyNames)
CHAKRASHIM_FUNCTION_GETTER(getEnumerableNamedProperties)
CHAKRASHIM_FUNCTION_GETTER(getEnumerableIndexedProperties)
CHAKRASHIM_FUNCTION_GETTER(createEnumerationIterator)
CHAKRASHIM_FUNCTION_GETTER(createPropertyDescriptorsEnumerationIterator)
CHAKRASHIM_FUNCTION_GETTER(getNamedOwnKeys)
CHAKRASHIM_FUNCTION_GETTER(getIndexedOwnKeys)
CHAKRASHIM_FUNCTION_GETTER(getStackTrace)
CHAKRASHIM_FUNCTION_GETTER(getSymbolKeyFor)
CHAKRASHIM_FUNCTION_GETTER(getSymbolFor)
CHAKRASHIM_FUNCTION_GETTER(ensureDebug)
CHAKRASHIM_FUNCTION_GETTER(enqueueMicrotask);
CHAKRASHIM_FUNCTION_GETTER(dequeueMicrotask);

#define DEF_IS_TYPE(F) CHAKRASHIM_FUNCTION_GETTER(F)
#include "jsrtcachedpropertyidref.inc"
#undef DEF_IS_TYPE

}  // namespace jsrt

namespace v8 {

// This shim wraps Object.prototype.toString to supports ObjectTemplate class
// name.
JsValueRef CHAKRA_CALLBACK Utils::ObjectPrototypeToStringShim(
    JsValueRef callee,
    bool isConstructCall,
    JsValueRef *arguments,
    unsigned short argumentCount,
    void *callbackState) {
  if (argumentCount >= 1) {
    Isolate* iso = Isolate::GetCurrent();
    HandleScope scope(iso);

    Object* obj = static_cast<Object*>(arguments[0]);
    ObjectTemplate* objTemplate = obj->GetObjectTemplate();
    if (objTemplate) {
      Local<String> str = objTemplate->GetClassName();
      if (!str.IsEmpty()) {
        str = String::Concat(String::NewFromUtf8(iso, "[object "), str);
        str = String::Concat(str, String::NewFromUtf8(iso, "]"));
        return *str;
      }
    }
  }

  JsValueRef function = callbackState;
  JsValueRef result;
  if (JsCallFunction(function, arguments, argumentCount,
                     &result) != JsNoError) {
    return JS_INVALID_REFERENCE;
  }
  return result;
}

}  // namespace v8
