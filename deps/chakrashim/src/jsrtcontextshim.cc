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

#include "v8.h"
#include "jsrtutils.h"
#include "chakra_natives.h"
#include <memory>

namespace jsrt
{
  ContextShim::Scope::Scope(ContextShim * contextShim)
  {
    contextShim->GetIsolateShim()->PushScope(this, contextShim);
  }

  ContextShim::Scope::~Scope()
  {
    this->contextShim->GetIsolateShim()->PopScope(this);
  }

  ContextShim * ContextShim::New(IsolateShim * isolateShim, bool exposeGC)
  {
    JsContextRef context;
    if (JsCreateContext(isolateShim->GetRuntimeHandle(), &context) != JsNoError)
    {
      return false;
    }

    return new ContextShim(isolateShim, context, exposeGC);
  }

  ContextShim::ContextShim(IsolateShim * isolateShim, JsContextRef context, bool exposeGC) :
    isolateShim(isolateShim),
    context(context),
    initialized(false),
    exposeGC(exposeGC),
    promiseContinuationFunction(JS_INVALID_REFERENCE),
    instanceOfFunction(JS_INVALID_REFERENCE),
    cloneObjectFunction(JS_INVALID_REFERENCE),
    isUintFunction(JS_INVALID_REFERENCE),
    getPropertyNamesFunction(JS_INVALID_REFERENCE),
    getOwnPropertyDescriptorFunction(JS_INVALID_REFERENCE),
    getEnumerableNamedPropertiesFunction(JS_INVALID_REFERENCE),
    getEnumerableIndexedPropertiesFunction(JS_INVALID_REFERENCE),
    createEnumerationIteratorFunction(JS_INVALID_REFERENCE),
    createPropertyDescriptorsEnumerationIteratorFunction(JS_INVALID_REFERENCE),
    getNamedOwnKeysFunction(JS_INVALID_REFERENCE),
    getIndexedOwnKeysFunction(JS_INVALID_REFERENCE),
    createEmptyLambdaFunction(JS_INVALID_REFERENCE)
  {
  }

  IsolateShim * ContextShim::GetIsolateShim()
  {
    return isolateShim;
  }

  JsContextRef ContextShim::GetContextRef()
  {
    return context;
  }

  ContextShim * ContextShim::GetCurrent()
  {
    return IsolateShim::GetCurrent()->GetCurrentContextShim();
  }

  void* ContextShim::GetAlignedPointerFromEmbedderData(int index)
  {
    if (index >= 0 && static_cast<std::vector<void*>::size_type>(index) < embedderData.size())
    {
      return embedderData.at(index);
    }
    return nullptr;
  }

  bool ContextShim::KeepAlive(JsValueRef value)
  {
    // use an object set as a proeprty of the global object to keep object alive for the duration of the context
    JsValueRef builtInIndex;
    if (JsIntToNumber(builtInCount, &builtInIndex) != JsNoError)
    {
      return false;
    }
    if (JsSetIndexedProperty(keepAliveObject, builtInIndex, value) != JsNoError)
    {
      return false;
    }
    builtInCount++;
    return true;
  }

  template <typename Fn>
  bool ContextShim::InitializeBuiltIn(JsValueRef * builtInValue, Fn getBuiltIn)
  {
    JsValueRef value;
    if (getBuiltIn(&value) != JsNoError)
    {
      return false;
    }
    if (!KeepAlive(value))
    {
      return false;
    }
    *builtInValue = value;
    return true;
  }

  bool ContextShim::InitializeBuiltIns()
  {
    // No need to keep the global object alive, the context will implicitly
    if (JsGetGlobalObject(&globalObject) != JsNoError)
    {
      return false;
    }

    JsValueRef newKeepAliveObject;
    // Create an object to keep these built in alive along with the global object
    builtInCount = 0;
    if (JsCreateObject(&newKeepAliveObject) != JsNoError)
    {
      return false;
    }
    keepAliveObject = newKeepAliveObject;
    // true and false is needed by DefineProperty to create the property descriptor
    if (!InitializeBuiltIn(&trueRef,
      [](JsValueRef * value) { return JsGetTrueValue(value); }))
    {
      return false;
    }
    if (!InitializeBuiltIn(&falseRef,
      [](JsValueRef * value) { return JsGetFalseValue(value); }))
    {
      return false;
    }

    if (!InitializeBuiltIn(&undefinedRef,
      [](JsValueRef * value) { return JsGetUndefinedValue(value); }))
    {
      return false;
    }
    if (!InitializeBuiltIn(&nullRef,
      [](JsValueRef * value) { return JsGetNullValue(value); }))
    {
      return false;
    }
    if (!InitializeBuiltIn(&zero,
      [](JsValueRef * value) { return JsIntToNumber(0, value); }))
    {
      return false;
    }
    if (!InitializeBuiltIn(&booleanObjectConstructor,
      [](JsValueRef * value) { return GetPropertyOfGlobal(L"Boolean", value); }))
    {
      return false;
    }
    if (!InitializeBuiltIn(&numberObjectConstructor,
      [](JsValueRef * value) { return GetPropertyOfGlobal(L"Number", value); }))
    {
      return false;
    }
    if (!InitializeBuiltIn(&stringObjectConstructor,
      [](JsValueRef * value) { return GetPropertyOfGlobal(L"String", value); }))
    {
      return false;
    }
    if (!InitializeBuiltIn(&objectConstructor,
      [](JsValueRef * value) { return GetPropertyOfGlobal(L"Object", value); }))
    {
      return false;
    }
    if (!InitializeBuiltIn(&dateConstructor,
      [](JsValueRef * value) { return GetPropertyOfGlobal(L"Date", value); }))
    {
      return false;
    }
    if (!InitializeBuiltIn(&proxyConstructor,
      [](JsValueRef * value) { return GetPropertyOfGlobal(L"Proxy", value); }))
    {
      return false;
    }

    if (!InitializeBuiltIn(&getOwnPropertyDescriptorFunction,
      [this](JsValueRef * value) { return GetProperty(objectConstructor, L"getOwnPropertyDescriptor", value); }))
    {
      return false;
    }

    if (!InitializeBuiltIn(&stringConcatFunction,
      [this](JsValueRef * value)
      {
        JsValueRef stringPrototype;
        JsErrorCode error = GetProperty(stringObjectConstructor, L"prototype", &stringPrototype);
        if (error != JsNoError)
        {
          return error;
        }

        return GetProperty(stringPrototype, L"concat", value);
      }))
    {
      return false;
    }

    if (!InitializeReflect())
    {
      return false;
    }

    if (DefineProperty(globalObject, GetIsolateShim()->GetKeepAliveObjectSymbolPropertyIdRef(),
      PropertyDescriptorOptionValues::False,
      PropertyDescriptorOptionValues::False,
      PropertyDescriptorOptionValues::False,
      newKeepAliveObject, JS_INVALID_REFERENCE, JS_INVALID_REFERENCE) != JsNoError)
    {
      return false;
    }

    return InitializeProxyOfGlobal();
  }

  static JsValueRef CALLBACK ProxyOfGlobalGetPrototypeOfCallback(_In_ JsValueRef callee, _In_ bool isConstructCall, _In_ JsValueRef *arguments, _In_ unsigned short argumentCount, _In_opt_ void *callbackState)
  {
    // Return the target (which is the global object)
    return arguments[1];
  }

  bool ContextShim::InitializeReflect()
  {
    if (!InitializeBuiltIn(&reflectObject,
      [](JsValueRef * value) { return GetPropertyOfGlobal(L"Reflect", value); }))
    {
      return false;
    }

    for (unsigned int i = 0; i < ProxyTraps::TrapCount; i++)
    {
      if (!InitializeBuiltIn(&reflectFunctions[i],
        [this, i](JsValueRef * value) { return JsGetProperty(reflectObject, this->GetIsolateShim()->GetProxyTrapPropertyIdRef((ProxyTraps)i), value); }))
      {
        return false;
      }
    }
    return true;
  }
  bool ContextShim::InitializeProxyOfGlobal()
  {
    return InitializeBuiltIn(&proxyOfGlobal,
      [this](JsValueRef * value)
    {
      // V8 Global is actually proxy where the actual global is it's prototype.
      // We will fake it here.
      std::map<jsrt::ProxyTraps, JsNativeFunction> proxyTraps;
      proxyTraps[GetPrototypeOfTrap] = ProxyOfGlobalGetPrototypeOfCallback;
      return CreateProxy(globalObject, proxyTraps, value);
    });
  }
  bool ContextShim::EnsureInitialized()
  {
    if (initialized)
    {
      return true;
    }

    if (jsrt::InitializePromise() != JsNoError)
    {
      return false;
    }

    if (exposeGC && !ExposeGc())
    {
      return false;
    }

    if (!InitializeBuiltIns())
    {
      return false;
    }

    if (!ExecuteChakraPatch())
    {
      return false;
    }

    initialized = true;
    return true;
  }

  bool ContextShim::ExposeGc()
  {
    JsValueRef collectGarbageRef;
    if (jsrt::GetPropertyOfGlobal(L"CollectGarbage", &collectGarbageRef) != JsNoError)
    {
      return false;
    }

    if (jsrt::SetPropertyOfGlobal(L"gc", collectGarbageRef) != JsNoError)
    {
      return false;
    }

    return true;
  }

  bool ContextShim::ExecuteChakraPatch()
  {
    wchar_t buffer[_countof(chakra_shim_native)];

    if (StringConvert::CopyRaw<char, wchar_t>(chakra_shim_native, _countof(chakra_shim_native), buffer, _countof(chakra_shim_native)) != JsNoError)
    {
      return false;
    }
    JsValueRef getInitFunction;
    if (JsParseScript(buffer, JS_SOURCE_CONTEXT_NONE, L"chakra_shim.js", &getInitFunction) != JsNoError)
    {
      return false;
    }
    JsValueRef initFunction;
    if (JsCallFunction(getInitFunction, nullptr, 0, &initFunction) != JsNoError)
    {
      return false;
    }
    JsValueRef result;
    JsValueRef arguments[] = { this->keepAliveObject };
    return JsCallFunction(initFunction, arguments, _countof(arguments), &result) == JsNoError;
  }

  void ContextShim::SetAlignedPointerInEmbedderData(int index, void * value)
  {
    try
    {
      auto minSize = static_cast<std::vector<void*>::size_type>(index)+1;
      if (index >= 0 && embedderData.size() < minSize)
      {
        embedderData.resize(minSize);
        embedderData[index] = value;
      }
    }
    catch (const std::exception&)
    {
    }
  }

  JsValueRef ContextShim::GetPromiseContinuationFunction()
  {
    if (promiseContinuationFunction == JS_INVALID_REFERENCE)
    {
      JsValueRef process;
      if (jsrt::GetPropertyOfGlobal(L"process", &process) != JsNoError)
      {
        return JS_INVALID_REFERENCE;
      }

      JsValueRef nextTick;
      if (jsrt::GetProperty(process, L"nextTick", &nextTick) != JsNoError)
      {
        return JS_INVALID_REFERENCE;
      }

      // CHAKRA-REVIEW: Do we need to root this?
      promiseContinuationFunction = nextTick; // save in context data
    }
    return promiseContinuationFunction;
  }

  JsValueRef ContextShim::GetUndefined()
  {
    return undefinedRef;
  }

  JsValueRef ContextShim::GetTrue()
  {
    return trueRef;
  }

  JsValueRef ContextShim::GetFalse()
  {
    return falseRef;
  }

  JsValueRef ContextShim::GetNull()
  {
    return nullRef;
  }

  JsValueRef ContextShim::GetZero()
  {
    return zero;
  }

  JsValueRef ContextShim::GetNumberObjectConstructor()
  {
    return numberObjectConstructor;
  }

  JsValueRef ContextShim::GetBooleanObjectConstructor()
  {
    return booleanObjectConstructor;
  }

  JsValueRef ContextShim::GetStringObjectConstructor()
  {
    return stringObjectConstructor;
  }

  JsValueRef ContextShim::GetDateConstructor()
  {
    return dateConstructor;
  }

  JsValueRef ContextShim::GetProxyConstructor()
  {
    return proxyConstructor;
  }

  JsValueRef ContextShim::GetGetOwnPropertyDescriptorFunction()
  {
    return getOwnPropertyDescriptorFunction;
  }

  JsValueRef ContextShim::GetStringConcatFunction()
  {
    return stringConcatFunction;
  }

  JsValueRef ContextShim::GetProxyOfGlobal()
  {
    return proxyOfGlobal;
  }

  JsValueRef ContextShim::GetReflectObject()
  {
    return reflectObject;
  }

  JsValueRef ContextShim::GetReflectFunctionForTrap(ProxyTraps trap)
  {
    return reflectFunctions[trap];
  }

  JsValueRef ContextShim::GetInstanceOfFunction()
  {
    if (instanceOfFunction == JS_INVALID_REFERENCE)
    {
      // _chakra_patch.js has initialize the function an put it in keepAliveObject.
      JsErrorCode error = JsGetProperty(keepAliveObject,
        GetIsolateShim()->GetCachedPropertyIdRef(CachedPropertyIdRef::isInstanceOf), &instanceOfFunction);
      // CHAKRA-TODO: Handle Error?
    }

    return instanceOfFunction;
  }

  JsValueRef ContextShim::GetCloneObjectFunction()
  {
    if (cloneObjectFunction == JS_INVALID_REFERENCE)
    {
      // _chakra_patch.js has initialize the function an put it in keepAliveObject.
        JsErrorCode error = JsGetProperty(keepAliveObject,
          GetIsolateShim()->GetCachedPropertyIdRef(CachedPropertyIdRef::cloneObject), &cloneObjectFunction);
      // CHAKRA-TODO: Handle Error?
    }

    return cloneObjectFunction;
  }

  JsValueRef ContextShim::GetGetPropertyNamesFunction()
  {
    if (getPropertyNamesFunction == JS_INVALID_REFERENCE)
    {
      JsErrorCode error = JsGetProperty(keepAliveObject,
        GetIsolateShim()->GetCachedPropertyIdRef(CachedPropertyIdRef::getPropertyNames),
        &getPropertyNamesFunction);
      // CHAKRA-TODO: Handle Error?
    }
    return getPropertyNamesFunction;
  }

  JsValueRef ContextShim::GetGetEnumerableNamedPropertiesFunction()
  {
    if (getEnumerableNamedPropertiesFunction == JS_INVALID_REFERENCE)
    {
      JsErrorCode error = JsGetProperty(keepAliveObject,
        GetIsolateShim()->GetCachedPropertyIdRef(CachedPropertyIdRef::getEnumerableNamedProperties),
        &getEnumerableNamedPropertiesFunction);
      // CHAKRA-TODO: Handle Error?
    }
    return getEnumerableNamedPropertiesFunction;
  }

  JsValueRef ContextShim::GetGetEnumerableIndexedPropertiesFunction()
  {
    if (getEnumerableIndexedPropertiesFunction == JS_INVALID_REFERENCE)
    {
      JsErrorCode error = JsGetProperty(keepAliveObject,
        GetIsolateShim()->GetCachedPropertyIdRef(CachedPropertyIdRef::getEnumerableIndexedProperties),
        &getEnumerableIndexedPropertiesFunction);
      // CHAKRA-TODO: Handle Error?
    }
    return getEnumerableIndexedPropertiesFunction;
  }

  JsValueRef ContextShim::GetCreateEnumerationIteratorFunction()
  {
    if (createEnumerationIteratorFunction == JS_INVALID_REFERENCE)
    {
      JsErrorCode error = JsGetProperty(keepAliveObject,
        GetIsolateShim()->GetCachedPropertyIdRef(CachedPropertyIdRef::createEnumerationIterator),
        &createEnumerationIteratorFunction);
      // CHAKRA-TODO: Handle Error?
    }
    return createEnumerationIteratorFunction;
  }

  JsValueRef ContextShim::GetCreatePropertyDescriptorsEnumerationIteratorFunction()
  {
    if (createPropertyDescriptorsEnumerationIteratorFunction == JS_INVALID_REFERENCE)
    {
      JsErrorCode error = JsGetProperty(keepAliveObject,
        GetIsolateShim()->GetCachedPropertyIdRef(CachedPropertyIdRef::createPropertyDescriptorsEnumerationIterator),
        &createPropertyDescriptorsEnumerationIteratorFunction);
      // CHAKRA-TODO: Handle Error?
    }
    return createPropertyDescriptorsEnumerationIteratorFunction;
  }

  JsValueRef ContextShim::GetGetNamedOwnKeysFunction()
  {
    if (getNamedOwnKeysFunction == JS_INVALID_REFERENCE)
    {
      JsErrorCode error = JsGetProperty(keepAliveObject,
        GetIsolateShim()->GetCachedPropertyIdRef(CachedPropertyIdRef::getNamedOwnKeys), &getNamedOwnKeysFunction);
      // CHAKRA-TODO: Handle Error?
    }
    return getNamedOwnKeysFunction;
  }

  JsValueRef ContextShim::GetGetIndexedOwnKeysFunction()
  {
    if (getIndexedOwnKeysFunction == JS_INVALID_REFERENCE)
    {
      // _chakra_patch.js has initialize the function an put it in keepAliveObject.
      JsErrorCode error = JsGetProperty(keepAliveObject,
        GetIsolateShim()->GetCachedPropertyIdRef(CachedPropertyIdRef::getIndexedOwnKeys), &getIndexedOwnKeysFunction);
      // CHAKRA-TODO: Handle Error?
    }
    return getIndexedOwnKeysFunction;
  }

  JsValueRef ContextShim::GetCreateEmptyLambdaFunction()
  {
    if (createEmptyLambdaFunction == JS_INVALID_REFERENCE)
    {
      // _chakra_patch.js has initialize the function an put it in keepAliveObject.
      JsErrorCode error = JsGetProperty(keepAliveObject,
        GetIsolateShim()->GetCachedPropertyIdRef(CachedPropertyIdRef::createEmptyLambdaFunction), &createEmptyLambdaFunction);
      // CHAKRA-TODO: Handle Error?
    }
    return createEmptyLambdaFunction;
  }
};
