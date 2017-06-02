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

#include <string>

#include "v8chakra.h"
#include "jsrtinspectorhelpers.h"
#include "jsrtutils.h"

namespace jsrt {
  static const int JsrtDebugPropertyAttributeReadOnly = 4;
  static const int MaxPropertyCount = 5000;

  typedef JsErrorCode(*ConvertFunc)(JsValueRef, JsValueRef*);

  template<class ValueConvertFunc>
  static JsErrorCode CopyPropertyImpl(
      JsValueRef sourceObj,
      JsPropertyIdRef sourceIdRef,
      JsValueRef destObj,
      JsPropertyIdRef destIdRef,
      ValueConvertFunc convertFunc,
      bool *wasCopied) {
    if (sourceObj == JS_INVALID_REFERENCE ||
        sourceIdRef == JS_INVALID_REFERENCE ||
        destObj == JS_INVALID_REFERENCE) {
      return JsErrorInvalidArgument;
    }

    if (wasCopied != nullptr) {
      *wasCopied = false;
    }

    bool hasProp = false;
    IfJsErrorRet(JsHasProperty(sourceObj, sourceIdRef, &hasProp));

    if (!hasProp) {
      return JsNoError;
    }

    JsValueRef sourceVal = JS_INVALID_REFERENCE;
    IfJsErrorRet(JsGetProperty(sourceObj, sourceIdRef, &sourceVal));

    bool isUndefined = false;
    IfJsErrorRet(jsrt::IsUndefined(sourceVal, &isUndefined));

    if (isUndefined) {
      return JsNoError;
    }

    JsValueRef destVal = JS_INVALID_REFERENCE;
    if (convertFunc != nullptr) {
      IfJsErrorRet(convertFunc(sourceVal, &destVal));
    } else {
      destVal = sourceVal;
    }

    if (destIdRef == JS_INVALID_REFERENCE) {
      destIdRef = sourceIdRef;
    }

    IfJsErrorRet(JsSetProperty(destObj, destIdRef, destVal, true));

    if (wasCopied != nullptr) {
      *wasCopied = true;
    }

    return JsNoError;
  }

  static bool CopyPropertyIfPresent(
      JsValueRef sourceObj,
      CachedPropertyIdRef cachedSourceIdRef,
      JsValueRef destObj) {
    JsPropertyIdRef sourceIdRef =
        IsolateShim::GetCurrent()->GetCachedPropertyIdRef(cachedSourceIdRef);

    bool wasCopied = false;
    CHAKRA_VERIFY_NOERROR(InspectorHelpers::TryCopyProperty(
        sourceObj, sourceIdRef, destObj, JS_INVALID_REFERENCE, &wasCopied));

    return wasCopied;
  }

  static bool CopyPropertyIfPresent(
      JsValueRef sourceObj,
      CachedPropertyIdRef cachedSourceIdRef,
      JsValueRef destObj,
      CachedPropertyIdRef cachedDestIdRef) {
    JsPropertyIdRef sourceIdRef =
        IsolateShim::GetCurrent()->GetCachedPropertyIdRef(cachedSourceIdRef);
    JsPropertyIdRef destIdRef =
        IsolateShim::GetCurrent()->GetCachedPropertyIdRef(cachedDestIdRef);

    bool wasCopied = false;
    CHAKRA_VERIFY_NOERROR(InspectorHelpers::TryCopyProperty(
        sourceObj, sourceIdRef, destObj, destIdRef, &wasCopied));

    return wasCopied;
  }

  static bool CopyPropertyStringIfPresent(
      JsValueRef sourceObj,
      CachedPropertyIdRef cachedSourceIdRef,
      JsValueRef destObj) {
    JsPropertyIdRef sourceIdRef =
        IsolateShim::GetCurrent()->GetCachedPropertyIdRef(cachedSourceIdRef);

    bool wasCopied = false;
    CHAKRA_VERIFY_NOERROR(InspectorHelpers::TryCopyPropertyString(
        sourceObj, sourceIdRef, destObj, JS_INVALID_REFERENCE, &wasCopied));

    return wasCopied;
  }

  static bool CopyPropertyStringIfPresent(
      JsValueRef sourceObj,
      CachedPropertyIdRef cachedSourceIdRef,
      JsValueRef destObj,
      CachedPropertyIdRef cachedDestIdRef) {
    JsPropertyIdRef sourceIdRef =
        IsolateShim::GetCurrent()->GetCachedPropertyIdRef(cachedSourceIdRef);
    JsPropertyIdRef destIdRef =
        IsolateShim::GetCurrent()->GetCachedPropertyIdRef(cachedDestIdRef);

    bool wasCopied = false;
    CHAKRA_VERIFY_NOERROR(InspectorHelpers::TryCopyPropertyString(
        sourceObj, sourceIdRef, destObj, destIdRef, &wasCopied));

    return wasCopied;
  }

  static std::string GetObjectIdForFrameProp(unsigned int frameIndex,
                                             const char *propName) {
    std::string objectId("{\"ordinal\":");
    objectId.append(std::to_string(frameIndex));
    objectId.append(",\"name\":\"");
    objectId.append(propName);
    objectId.append("\"}");

    return objectId;
  }

  static std::string GetObjectIdForHandle(unsigned int handle) {
    std::string objectId("{\"handle\":");
    objectId.append(std::to_string(handle));
    objectId.append("}");

    return objectId;
  }

  static void AddFunctionInfo(JsValueRef wrappedCallFrame,
                              JsValueRef callFrame) {
    int functionHandle = 0;
    CHAKRA_VERIFY_NOERROR(jsrt::GetProperty(
        callFrame, CachedPropertyIdRef::functionHandle, &functionHandle));

    JsValueRef funcObj = JS_INVALID_REFERENCE;
    CHAKRA_VERIFY_NOERROR(JsDiagGetObjectFromHandle(functionHandle,
                                                    &funcObj));

    CHAKRA_VERIFY(CopyPropertyIfPresent(funcObj, CachedPropertyIdRef::name,
                                        wrappedCallFrame,
                                        CachedPropertyIdRef::functionName));

    JsValueRef functionLocation = JS_INVALID_REFERENCE;
    CHAKRA_VERIFY_NOERROR(JsCreateObject(&functionLocation));
    CHAKRA_VERIFY_NOERROR(jsrt::SetProperty(
        wrappedCallFrame, CachedPropertyIdRef::functionLocation,
        functionLocation));

    CHAKRA_VERIFY(CopyPropertyStringIfPresent(funcObj,
                                              CachedPropertyIdRef::scriptId,
                                              functionLocation));
    CHAKRA_VERIFY(CopyPropertyIfPresent(funcObj, CachedPropertyIdRef::line,
                                        functionLocation,
                                        CachedPropertyIdRef::lineNumber));
    CHAKRA_VERIFY(CopyPropertyIfPresent(funcObj, CachedPropertyIdRef::column,
                                        functionLocation,
                                        CachedPropertyIdRef::columnNumber));
  }

  static void AddLocationInfo(JsValueRef wrappedCallFrame,
                              JsValueRef callFrame) {
    JsValueRef location = JS_INVALID_REFERENCE;
    CHAKRA_VERIFY_NOERROR(JsCreateObject(&location));
    CHAKRA_VERIFY_NOERROR(jsrt::SetProperty(wrappedCallFrame,
                                            CachedPropertyIdRef::location,
                                            location));

    CHAKRA_VERIFY(CopyPropertyStringIfPresent(callFrame,
                                              CachedPropertyIdRef::scriptId,
                                              location));
    CHAKRA_VERIFY(CopyPropertyIfPresent(callFrame, CachedPropertyIdRef::line,
                                        location,
                                        CachedPropertyIdRef::lineNumber));
    CHAKRA_VERIFY(CopyPropertyIfPresent(callFrame, CachedPropertyIdRef::column,
                                        location,
                                        CachedPropertyIdRef::columnNumber));
  }

  static void AddLocalScopeInfo(JsValueRef scopesArray,
                                int callFrameIndex,
                                JsValueRef stackProperties) {
    bool hasProperty = false;
    CHAKRA_VERIFY_NOERROR(jsrt::HasProperty(stackProperties,
                                            CachedPropertyIdRef::locals,
                                            &hasProperty));

    if (hasProperty) {
      JsValueRef localObj = JS_INVALID_REFERENCE;
      CHAKRA_VERIFY_NOERROR(JsCreateObject(&localObj));

      JsValueRef newLength = JS_INVALID_REFERENCE;
      CHAKRA_VERIFY_NOERROR(jsrt::PushArray(scopesArray, localObj, &newLength));

      CHAKRA_VERIFY_NOERROR(jsrt::SetProperty(localObj,
                                              CachedPropertyIdRef::type,
                                              "local"));

      JsValueRef object = JS_INVALID_REFERENCE;
      CHAKRA_VERIFY_NOERROR(JsCreateObject(&object));
      CHAKRA_VERIFY_NOERROR(jsrt::SetProperty(localObj,
                                              CachedPropertyIdRef::object,
                                              object));

      CHAKRA_VERIFY_NOERROR(jsrt::SetProperty(object,
                                              CachedPropertyIdRef::type,
                                              "object"));
      CHAKRA_VERIFY_NOERROR(jsrt::SetProperty(object,
                                              CachedPropertyIdRef::className,
                                              "Object"));
      CHAKRA_VERIFY_NOERROR(jsrt::SetProperty(object,
                                              CachedPropertyIdRef::description,
                                              "Object"));
      CHAKRA_VERIFY_NOERROR(jsrt::SetProperty(
          object, CachedPropertyIdRef::objectId,
          GetObjectIdForFrameProp(callFrameIndex, "locals").c_str()));
    }
  }

  static void AddClosureScopeInfo(JsValueRef scopesArray, JsValueRef scope) {
    JsValueRef scopeObj = JS_INVALID_REFERENCE;
    CHAKRA_VERIFY_NOERROR(JsCreateObject(&scopeObj));

    JsValueRef newLength = JS_INVALID_REFERENCE;
    CHAKRA_VERIFY_NOERROR(jsrt::PushArray(scopesArray, scopeObj, &newLength));

    CHAKRA_VERIFY_NOERROR(jsrt::SetProperty(scopeObj, CachedPropertyIdRef::type,
                                            "closure"));

    JsValueRef object = JS_INVALID_REFERENCE;
    CHAKRA_VERIFY_NOERROR(JsCreateObject(&object));
    CHAKRA_VERIFY_NOERROR(jsrt::SetProperty(scopeObj,
                                            CachedPropertyIdRef::object,
                                            object));

    CHAKRA_VERIFY_NOERROR(jsrt::SetProperty(object, CachedPropertyIdRef::type,
                                            "object"));
    CHAKRA_VERIFY_NOERROR(jsrt::SetProperty(object,
                                            CachedPropertyIdRef::className,
                                            "Object"));
    CHAKRA_VERIFY_NOERROR(jsrt::SetProperty(object,
                                            CachedPropertyIdRef::description,
                                            "Object"));

    int handle = 0;
    CHAKRA_VERIFY_NOERROR(jsrt::GetProperty(scope, CachedPropertyIdRef::handle,
                                            &handle));
    CHAKRA_VERIFY_NOERROR(jsrt::SetProperty(
        object, CachedPropertyIdRef::objectId,
        GetObjectIdForHandle(handle).c_str()));
  }

  static void AddGlobalScopeInfo(JsValueRef scopesArray,
                                 int callFrameIndex,
                                 JsValueRef stackProperties) {
    bool hasProperty = false;
    CHAKRA_VERIFY_NOERROR(jsrt::HasProperty(stackProperties,
                                            CachedPropertyIdRef::globals,
                                            &hasProperty));

    if (hasProperty) {
      JsValueRef globalObj = JS_INVALID_REFERENCE;
      CHAKRA_VERIFY_NOERROR(JsCreateObject(&globalObj));

      JsValueRef newLength = JS_INVALID_REFERENCE;
      CHAKRA_VERIFY_NOERROR(jsrt::PushArray(scopesArray, globalObj,
                            &newLength));

      CHAKRA_VERIFY_NOERROR(jsrt::SetProperty(globalObj,
                            CachedPropertyIdRef::type, "global"));

      JsValueRef object = JS_INVALID_REFERENCE;
      CHAKRA_VERIFY_NOERROR(JsCreateObject(&object));
      CHAKRA_VERIFY_NOERROR(jsrt::SetProperty(globalObj,
                                              CachedPropertyIdRef::object,
                                              object));

      CHAKRA_VERIFY_NOERROR(jsrt::SetProperty(object, CachedPropertyIdRef::type,
                                              "object"));
      CHAKRA_VERIFY_NOERROR(jsrt::SetProperty(object,
                                              CachedPropertyIdRef::className,
                                              "global"));
      CHAKRA_VERIFY_NOERROR(jsrt::SetProperty(object,
                                              CachedPropertyIdRef::description,
                                              "global"));
      CHAKRA_VERIFY_NOERROR(jsrt::SetProperty(
          object, CachedPropertyIdRef::objectId,
          GetObjectIdForFrameProp(callFrameIndex, "globals").c_str()));
    }
  }

  static void AddScopeInfo(JsValueRef wrappedCallFrame, int callFrameIndex,
                           JsValueRef stackProperties) {
    JsValueRef scopeChain = JS_INVALID_REFERENCE;
    CHAKRA_VERIFY_NOERROR(JsCreateArray(0, &scopeChain));
    CHAKRA_VERIFY_NOERROR(jsrt::SetProperty(wrappedCallFrame,
                                            CachedPropertyIdRef::scopeChain,
                                            scopeChain));

    AddLocalScopeInfo(scopeChain, callFrameIndex, stackProperties);

    bool hasProperty = false;
    CHAKRA_VERIFY_NOERROR(jsrt::HasProperty(stackProperties,
                                            CachedPropertyIdRef::scopes,
                                            &hasProperty));

    if (hasProperty) {
      JsValueRef scopes = JS_INVALID_REFERENCE;
      CHAKRA_VERIFY_NOERROR(jsrt::GetProperty(stackProperties,
                                              CachedPropertyIdRef::scopes,
                                              &scopes));

      unsigned int length = 0;
      CHAKRA_VERIFY_NOERROR(jsrt::GetArrayLength(scopes, &length));

      for (unsigned int i = 0; i < length; i++) {
        JsValueRef scope = JS_INVALID_REFERENCE;
        CHAKRA_VERIFY_NOERROR(jsrt::GetIndexedProperty(scopes, i, &scope));

        AddClosureScopeInfo(scopeChain, scope);
      }
    }

    AddGlobalScopeInfo(scopeChain, callFrameIndex, stackProperties);
  }

  JsErrorCode InspectorHelpers::TryCopyProperty(
      JsValueRef sourceObj,
      JsPropertyIdRef sourceIdRef,
      JsValueRef destObj,
      JsPropertyIdRef destIdRef,
      bool *wasCopied) {
    return CopyPropertyImpl(sourceObj, sourceIdRef, destObj, destIdRef,
                            static_cast<ConvertFunc>(nullptr), wasCopied);
  }

  JsErrorCode InspectorHelpers::TryCopyPropertyString(
      JsValueRef sourceObj,
      JsPropertyIdRef sourceIdRef,
      JsValueRef destObj,
      JsPropertyIdRef destIdRef,
      bool *wasCopied) {
    return CopyPropertyImpl(sourceObj, sourceIdRef, destObj, destIdRef,
                            &JsConvertValueToString, wasCopied);
  }

  v8::Local<v8::Value> InspectorHelpers::WrapEvaluateObject(
      JsValueRef sourceObject) {
    JsValueRef resultObj = JS_INVALID_REFERENCE;
    WrapObject(sourceObject, &resultObj);

    return v8::Utils::ToLocal(static_cast<v8::Value*>(resultObj));
  }

  v8::Local<v8::Object> InspectorHelpers::WrapCallFrameDetails(
      JsValueRef callFrame) {
    JsValueRef wrappedObj = JS_INVALID_REFERENCE;
    CHAKRA_VERIFY_NOERROR(JsCreateObject(&wrappedObj));

    AddFunctionInfo(wrappedObj, callFrame);
    AddLocationInfo(wrappedObj, callFrame);

    int callFrameIndex = 0;
    CHAKRA_VERIFY_NOERROR(jsrt::GetProperty(callFrame,
                                            CachedPropertyIdRef::index,
                                            &callFrameIndex));

    JsValueRef stackProperties = JS_INVALID_REFERENCE;
    CHAKRA_VERIFY_NOERROR(JsDiagGetStackProperties(callFrameIndex,
                                                   &stackProperties));

    AddScopeInfo(wrappedObj, callFrameIndex, stackProperties);

    bool hasProperty = false;
    CHAKRA_VERIFY_NOERROR(jsrt::HasProperty(stackProperties,
                                            CachedPropertyIdRef::thisObject,
                                            &hasProperty));

    JsValueRef thisObj = JS_INVALID_REFERENCE;

    if (hasProperty) {
      JsValueRef thisObject = JS_INVALID_REFERENCE;
      CHAKRA_VERIFY_NOERROR(jsrt::GetProperty(stackProperties,
                                              CachedPropertyIdRef::thisObject,
                                              &thisObject));
      WrapObject(thisObject, &thisObj);
    } else {
      // The protocol requires a "this" member, so create an undefined object
      // to return.
      CHAKRA_VERIFY_NOERROR(JsCreateObject(&thisObj));
      CHAKRA_VERIFY_NOERROR(jsrt::SetProperty(thisObj,
                                              CachedPropertyIdRef::type,
                                              "undefined"));
    }

    CHAKRA_VERIFY_NOERROR(jsrt::SetProperty(wrappedObj, "this", thisObj));

    CHAKRA_VERIFY_NOERROR(jsrt::HasProperty(stackProperties,
                                            CachedPropertyIdRef::returnValue,
                                            &hasProperty));

    if (hasProperty) {
      JsValueRef returnObj = JS_INVALID_REFERENCE;
      CHAKRA_VERIFY_NOERROR(jsrt::GetProperty(stackProperties,
                                              CachedPropertyIdRef::returnValue,
                                              &returnObj));

      JsValueRef returnValue = JS_INVALID_REFERENCE;
      WrapObject(returnObj, &returnValue);
      CHAKRA_VERIFY_NOERROR(jsrt::SetProperty(wrappedObj,
                                              CachedPropertyIdRef::returnValue,
                                              returnValue));
    }

    return v8::Utils::ToLocal(static_cast<v8::Object*>(wrappedObj));
  }

  v8::Local<v8::Value> InspectorHelpers::WrapRuntimeException(
      JsValueRef eventData) {
    bool hasProperty = false;
    CHAKRA_VERIFY_NOERROR(jsrt::HasProperty(eventData,
                                            CachedPropertyIdRef::exception,
                                            &hasProperty));

    if (!hasProperty) {
      return v8::Local<v8::Value>();
    }

    JsValueRef exceptionProperty = JS_INVALID_REFERENCE;
    CHAKRA_VERIFY_NOERROR(jsrt::GetProperty(eventData,
                                            CachedPropertyIdRef::exception,
                                            &exceptionProperty));

    return WrapException(exceptionProperty);
  }

  v8::Local<v8::Value> InspectorHelpers::WrapPropertiesArray(
      JsValueRef properties) {
    JsValueRef propsArray = JS_INVALID_REFERENCE;
    CHAKRA_VERIFY_NOERROR(JsCreateArray(0, &propsArray));
    int nextIndex = 0;

    CHAKRA_VERIFY(properties != nullptr);

    unsigned int length = 0;
    CHAKRA_VERIFY_NOERROR(jsrt::GetArrayLength(properties, &length));

    for (unsigned int i = 0; i < length; i++) {
      JsValueRef propValue = JS_INVALID_REFERENCE;
      CHAKRA_VERIFY_NOERROR(jsrt::GetIndexedProperty(properties, i,
                                                     &propValue));

      JsValueRef wrappedProp = JS_INVALID_REFERENCE;
      InspectorHelpers::WrapProperty(propValue, &wrappedProp);
      jsrt::SetIndexedProperty(propsArray, nextIndex++, wrappedProp);
    }

    return v8::Utils::ToLocal(static_cast<v8::Value*>(propsArray));
  }

  v8::Local<v8::Value> InspectorHelpers::GetWrappedProperties(int handle) {
    JsValueRef diagProperties = JS_INVALID_REFERENCE;
    JsErrorCode err = JsDiagGetProperties(handle, 0, MaxPropertyCount,
                                          &diagProperties);
    if (err == JsErrorDiagInvalidHandle) {
      // The handle is no longer valid, this is likely due to a pending
      // request that wasn't serviced before the last continuation.
      return v8::Local<v8::Value>();
    }
    CHAKRA_VERIFY_NOERROR(err);

    JsValueRef allPropsArray = JS_INVALID_REFERENCE;
    CHAKRA_VERIFY_NOERROR(JsCreateArray(0, &allPropsArray));

    JsValueRef propertiesArray = JS_INVALID_REFERENCE;
    CHAKRA_VERIFY_NOERROR(jsrt::GetProperty(diagProperties,
                                            CachedPropertyIdRef::properties,
                                            &propertiesArray));
    CHAKRA_VERIFY_NOERROR(jsrt::ConcatArray(allPropsArray, propertiesArray,
                                            &allPropsArray));

    JsValueRef debuggerOnlyPropertiesArray = JS_INVALID_REFERENCE;
    CHAKRA_VERIFY_NOERROR(jsrt::GetProperty(
        diagProperties, CachedPropertyIdRef::debuggerOnlyProperties,
        &debuggerOnlyPropertiesArray));
    CHAKRA_VERIFY_NOERROR(jsrt::ConcatArray(allPropsArray,
                                            debuggerOnlyPropertiesArray,
                                            &allPropsArray));

    return InspectorHelpers::WrapPropertiesArray(allPropsArray);
  }

  v8::Local<v8::Value> InspectorHelpers::GetWrappedStackLocals(
      JsValueRef stackProperties) {
    JsValueRef localsArray = JS_INVALID_REFERENCE;
    CHAKRA_VERIFY_NOERROR(JsCreateArray(0, &localsArray));

    bool hasProp = false;
    CHAKRA_VERIFY_NOERROR(jsrt::HasProperty(stackProperties,
                                            CachedPropertyIdRef::exception,
                                            &hasProp));

    if (hasProp) {
      JsValueRef exception = JS_INVALID_REFERENCE;
      CHAKRA_VERIFY_NOERROR(jsrt::GetProperty(stackProperties,
                                              CachedPropertyIdRef::exception,
                                              &exception));
      CHAKRA_VERIFY_NOERROR(jsrt::ConcatArray(localsArray,
                                                          exception,
                                                          &localsArray));
    }

    CHAKRA_VERIFY_NOERROR(jsrt::HasProperty(stackProperties,
                                            CachedPropertyIdRef::arguments,
                                            &hasProp));

    if (hasProp) {
      JsValueRef arguments = JS_INVALID_REFERENCE;
      CHAKRA_VERIFY_NOERROR(jsrt::GetProperty(stackProperties,
                                              CachedPropertyIdRef::arguments,
                                              &arguments));
      CHAKRA_VERIFY_NOERROR(jsrt::ConcatArray(localsArray, arguments,
                                              &localsArray));
    }

    CHAKRA_VERIFY_NOERROR(jsrt::HasProperty(
        stackProperties, CachedPropertyIdRef::functionCallsReturn, &hasProp));

    if (hasProp) {
      JsValueRef functionCallsReturn = JS_INVALID_REFERENCE;
      CHAKRA_VERIFY_NOERROR(jsrt::GetProperty(
          stackProperties, CachedPropertyIdRef::functionCallsReturn,
          &functionCallsReturn));
      CHAKRA_VERIFY_NOERROR(jsrt::ConcatArray(localsArray, functionCallsReturn,
                                              &localsArray));
    }

    CHAKRA_VERIFY_NOERROR(jsrt::HasProperty(stackProperties,
                                            CachedPropertyIdRef::locals,
                                            &hasProp));

    if (hasProp) {
      JsValueRef locals = JS_INVALID_REFERENCE;
      CHAKRA_VERIFY_NOERROR(jsrt::GetProperty(stackProperties,
                                              CachedPropertyIdRef::locals,
                                              &locals));
      CHAKRA_VERIFY_NOERROR(jsrt::ConcatArray(localsArray, locals,
                                              &localsArray));
    }

    return InspectorHelpers::WrapPropertiesArray(localsArray);
  }

  v8::Local<v8::Value> InspectorHelpers::EvaluateOnCallFrame(
      int ordinal, JsValueRef expression, bool returnByValue, bool* isError) {
    CHAKRA_VERIFY(ordinal >= 0);

    if (isError != nullptr) {
      *isError = false;
    }

    JsValueRef evalResult = JS_INVALID_REFERENCE;
    JsErrorCode err = JsDiagEvaluate(expression, ordinal,
                                     JsParseScriptAttributeNone,
                                     returnByValue, &evalResult);

    if (err == JsErrorScriptException) {
      if (isError != nullptr) {
        *isError = true;
      }

      return WrapException(evalResult);
    } else if (err == JsErrorDiagNotAtBreak) {
      return v8::Local<v8::Value>();
    }

    CHAKRA_VERIFY_NOERROR(err);
    return WrapEvaluateObject(evalResult);
  }

  v8::Local<v8::Value> InspectorHelpers::EvaluateOnCallFrame(
      JsValueRef callFrame, JsValueRef expression, bool returnByValue,
      bool* isError) {
    CHAKRA_VERIFY(callFrame != nullptr);

    int ordinal = 0;
    CHAKRA_VERIFY_NOERROR(jsrt::GetProperty(callFrame,
                                            CachedPropertyIdRef::index,
                                            &ordinal));

    return EvaluateOnCallFrame(ordinal, expression, returnByValue, isError);
  }

  v8::Local<v8::Value> InspectorHelpers::GetScriptSource(
      unsigned int scriptId) {
    JsValueRef scriptSource = JS_INVALID_REFERENCE;
    CHAKRA_VERIFY_NOERROR(JsDiagGetSource(scriptId, &scriptSource));

    JsValueRef sourceStr = JS_INVALID_REFERENCE;
    CHAKRA_VERIFY_NOERROR(jsrt::GetProperty(scriptSource,
                                            CachedPropertyIdRef::source,
                                            &sourceStr));

    return v8::Utils::ToLocal(static_cast<v8::Value*>(sourceStr));
  }

  JsRuntimeHandle InspectorHelpers::GetRuntimeFromIsolate(
      v8::Isolate *isolate) {
    return IsolateShim::FromIsolate(isolate)->GetRuntimeHandle();
  }

  void InspectorHelpers::WrapObject(JsValueRef obj, JsValueRef* wrappedObj) {
    JsValueRef value = JS_INVALID_REFERENCE;
    CHAKRA_VERIFY_NOERROR(JsCreateObject(&value));

    CHAKRA_VERIFY(CopyPropertyIfPresent(obj, CachedPropertyIdRef::type, value));

    CopyPropertyIfPresent(obj, CachedPropertyIdRef::className, value);

    bool hasValue = CopyPropertyIfPresent(obj, CachedPropertyIdRef::value,
                                          value);
    bool hasDisplay = CopyPropertyIfPresent(obj, CachedPropertyIdRef::display,
                                            value,
                                            CachedPropertyIdRef::description);

    // A description is required for values to be shown in the debugger.
    if (hasValue && !hasDisplay) {
      hasDisplay = CopyPropertyStringIfPresent(
          obj, CachedPropertyIdRef::value, value,
          CachedPropertyIdRef::description);
    }

    CHAKRA_VERIFY(hasDisplay);

    int handle = 0;
    CHAKRA_VERIFY_NOERROR(jsrt::GetProperty(obj, CachedPropertyIdRef::handle,
                                            &handle));
    CHAKRA_VERIFY_NOERROR(jsrt::SetProperty(
        value, CachedPropertyIdRef::objectId,
        GetObjectIdForHandle(handle).c_str()));

    *wrappedObj = value;
  }

  void InspectorHelpers::WrapProperty(JsValueRef propValue,
                                      JsValueRef* wrappedProperty) {
    JsValueRef wrappedObj = JS_INVALID_REFERENCE;
    CHAKRA_VERIFY_NOERROR(JsCreateObject(&wrappedObj));

    CHAKRA_VERIFY(CopyPropertyIfPresent(propValue, CachedPropertyIdRef::name,
                                        wrappedObj));

    JsValueRef value = JS_INVALID_REFERENCE;
    WrapObject(propValue, &value);
    CHAKRA_VERIFY_NOERROR(jsrt::SetProperty(wrappedObj,
                                            CachedPropertyIdRef::value, value));

    int propertyAttributes = 0;
    CHAKRA_VERIFY_NOERROR(jsrt::GetProperty(
        propValue, CachedPropertyIdRef::propertyAttributes,
        &propertyAttributes));
    CHAKRA_VERIFY_NOERROR(jsrt::SetProperty(
        wrappedObj, CachedPropertyIdRef::writable,
        (propertyAttributes & JsrtDebugPropertyAttributeReadOnly) == 0));
    CHAKRA_VERIFY_NOERROR(jsrt::SetProperty(wrappedObj,
                                            CachedPropertyIdRef::configurable,
                                            false));
    CHAKRA_VERIFY_NOERROR(jsrt::SetProperty(wrappedObj,
                                            CachedPropertyIdRef::enumerable,
                                            false));

    *wrappedProperty = wrappedObj;
  }

  v8::Local<v8::Value> InspectorHelpers::WrapException(JsValueRef exception) {
    JsValueRef exceptionVal = JS_INVALID_REFERENCE;
    CHAKRA_VERIFY_NOERROR(JsCreateObject(&exceptionVal));

    CHAKRA_VERIFY(CopyPropertyIfPresent(exception, CachedPropertyIdRef::type,
                                        exceptionVal));
    CHAKRA_VERIFY_NOERROR(jsrt::SetProperty(exceptionVal,
                                            CachedPropertyIdRef::subtype,
                                            "error"));
    CHAKRA_VERIFY(CopyPropertyIfPresent(exception,
                                        CachedPropertyIdRef::className,
                                        exceptionVal));
    CHAKRA_VERIFY(CopyPropertyIfPresent(exception, CachedPropertyIdRef::display,
                                        exceptionVal,
                                        CachedPropertyIdRef::description));

    int handle = 0;
    CHAKRA_VERIFY_NOERROR(jsrt::GetProperty(exception,
                                            CachedPropertyIdRef::handle,
                                            &handle));
    CHAKRA_VERIFY_NOERROR(jsrt::SetProperty(
        exceptionVal, CachedPropertyIdRef::objectId,
        GetObjectIdForHandle(handle).c_str()));

    return v8::Utils::ToLocal(static_cast<v8::Value*>(exceptionVal));
  }

}  // namespace jsrt
