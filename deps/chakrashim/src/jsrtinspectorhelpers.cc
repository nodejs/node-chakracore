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
      const char *sourceName,
      JsValueRef destObj,
      const char *destName,
      ValueConvertFunc convertFunc,
      bool *wasCopied) {
    if (sourceObj == nullptr || sourceName == nullptr || destObj == nullptr) {
      return JsErrorInvalidArgument;
    }

    if (wasCopied != nullptr)
    {
      *wasCopied = false;
    }

    JsPropertyIdRef propId = nullptr;
    IfJsErrorRet(JsCreatePropertyId(sourceName, strlen(sourceName), &propId));

    bool hasProp;
    IfJsErrorRet(JsHasProperty(sourceObj, propId, &hasProp));

    if (!hasProp) {
      return JsNoError;
    }

    JsValueRef sourceVal = nullptr;
    IfJsErrorRet(JsGetProperty(sourceObj, propId, &sourceVal));

    bool isUndefined;
    IfJsErrorRet(jsrt::IsUndefined(sourceVal, &isUndefined));

    if (isUndefined) {
      return JsNoError;
    }

    JsValueRef destVal = nullptr;
    if (convertFunc != nullptr) {
      IfJsErrorRet(convertFunc(sourceVal, &destVal));
    }
    else {
      destVal = sourceVal;
    }

    if (destName != nullptr) {
      IfJsErrorRet(JsCreatePropertyId(destName, strlen(destName), &propId));
    }

    IfJsErrorRet(JsSetProperty(destObj, propId, destVal, true));

    if (wasCopied != nullptr)
    {
      *wasCopied = true;
    }

    return JsNoError;
  }

  static bool CopyPropertyIfPresent(JsValueRef sourceObj,
                                    const char *sourceName,JsValueRef destObj,
                                    const char *destName = nullptr) {
    bool wasCopied;
    CHAKRA_VERIFY_NOERROR(InspectorHelpers::TryCopyProperty(
        sourceObj, sourceName, destObj, destName, &wasCopied));

    return wasCopied;
  }

  static bool CopyPropertyStringIfPresent(JsValueRef sourceObj,
                                          const char *sourceName,
                                          JsValueRef destObj,
                                          const char *destName = nullptr) {
    bool wasCopied;
    CHAKRA_VERIFY_NOERROR(InspectorHelpers::TryCopyPropertyString(
        sourceObj, sourceName, destObj, destName, &wasCopied));

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
      CHAKRA_VERIFY_NOERROR(InspectorHelpers::GetIntProperty(callFrame,
                                                             "functionHandle",
                                                             &functionHandle));

      JsValueRef funcObj = nullptr;
      CHAKRA_VERIFY_NOERROR(JsDiagGetObjectFromHandle(functionHandle,
                                                      &funcObj));

      CHAKRA_VERIFY(CopyPropertyIfPresent(funcObj, "name", wrappedCallFrame,
                                          "functionName"));

      JsValueRef functionLocation = nullptr;
      CHAKRA_VERIFY_NOERROR(JsCreateObject(&functionLocation));
      CHAKRA_VERIFY_NOERROR(InspectorHelpers::SetProperty(
          wrappedCallFrame,
          "functionLocation",
          functionLocation));

      CHAKRA_VERIFY(CopyPropertyStringIfPresent(funcObj, "scriptId",
                                                functionLocation));
      CHAKRA_VERIFY(CopyPropertyIfPresent(funcObj, "line", functionLocation,
                                          "lineNumber"));
      CHAKRA_VERIFY(CopyPropertyIfPresent(funcObj, "column", functionLocation,
                                          "columnNumber"));
  }

  static void AddLocationInfo(JsValueRef wrappedCallFrame,
                              JsValueRef callFrame) {
    JsValueRef location = nullptr;
    CHAKRA_VERIFY_NOERROR(JsCreateObject(&location));
    CHAKRA_VERIFY_NOERROR(InspectorHelpers::SetProperty(wrappedCallFrame,
                                                        "location",
                                                        location));

    CHAKRA_VERIFY(CopyPropertyStringIfPresent(callFrame, "scriptId",
                                              location));
    CHAKRA_VERIFY(CopyPropertyIfPresent(callFrame, "line", location,
                                        "lineNumber"));
    CHAKRA_VERIFY(CopyPropertyIfPresent(callFrame, "column", location,
                                        "columnNumber"));
  }

  static void AddLocalScopeInfo(JsValueRef scopesArray,
                                int callFrameIndex,
                                JsValueRef stackProperties) {
    bool hasProperty = false;
    CHAKRA_VERIFY_NOERROR(InspectorHelpers::HasProperty(stackProperties,
                                                        "locals",
                                                        &hasProperty));

    if (hasProperty) {
      JsValueRef localObj = nullptr;
      CHAKRA_VERIFY_NOERROR(JsCreateObject(&localObj));
      CHAKRA_VERIFY_NOERROR(InspectorHelpers::ArrayPush(scopesArray,
                                                          localObj));

      CHAKRA_VERIFY_NOERROR(InspectorHelpers::SetStringProperty(
          localObj, "type", "local"));

      JsValueRef object = nullptr;
      CHAKRA_VERIFY_NOERROR(JsCreateObject(&object));
      CHAKRA_VERIFY_NOERROR(InspectorHelpers::SetProperty(localObj,
                                                          "object",
                                                          object));

      CHAKRA_VERIFY_NOERROR(InspectorHelpers::SetStringProperty(
          object, "type", "object"));
      CHAKRA_VERIFY_NOERROR(InspectorHelpers::SetStringProperty(
          object, "className", "Object"));
      CHAKRA_VERIFY_NOERROR(InspectorHelpers::SetStringProperty(
          object, "description", "Object"));            
      CHAKRA_VERIFY_NOERROR(InspectorHelpers::SetStringProperty(
          object, "objectId",
          GetObjectIdForFrameProp(callFrameIndex, "locals").c_str()));
    }
  }

  static void AddClosureScopeInfo(JsValueRef scopesArray, JsValueRef scope) {
    JsValueRef scopeObj = nullptr;
    CHAKRA_VERIFY_NOERROR(JsCreateObject(&scopeObj));
    CHAKRA_VERIFY_NOERROR(InspectorHelpers::ArrayPush(scopesArray,
                                                      scopeObj));

    CHAKRA_VERIFY_NOERROR(InspectorHelpers::SetStringProperty(
      scopeObj, "type", "closure"));

    JsValueRef object = nullptr;
    CHAKRA_VERIFY_NOERROR(JsCreateObject(&object));
    CHAKRA_VERIFY_NOERROR(InspectorHelpers::SetProperty(scopeObj,
                                                        "object",
                                                        object));

    CHAKRA_VERIFY_NOERROR(InspectorHelpers::SetStringProperty(
      object, "type", "object"));
    CHAKRA_VERIFY_NOERROR(InspectorHelpers::SetStringProperty(
      object, "className", "Object"));
    CHAKRA_VERIFY_NOERROR(InspectorHelpers::SetStringProperty(
      object, "description", "Object"));

    int handle = 0;
    CHAKRA_VERIFY_NOERROR(InspectorHelpers::GetIntProperty(scope,
                                                           "handle",
                                                           &handle));
    CHAKRA_VERIFY_NOERROR(InspectorHelpers::SetStringProperty(
        object, "objectId", GetObjectIdForHandle(handle).c_str()));
  }

  static void AddGlobalScopeInfo(JsValueRef scopesArray,
                                 int callFrameIndex,
                                 JsValueRef stackProperties) {
    bool hasProperty = false;
    CHAKRA_VERIFY_NOERROR(InspectorHelpers::HasProperty(stackProperties,
                                                        "globals",
                                                        &hasProperty));

    if (hasProperty) {
      JsValueRef globalObj = nullptr;
      CHAKRA_VERIFY_NOERROR(JsCreateObject(&globalObj));
      CHAKRA_VERIFY_NOERROR(InspectorHelpers::ArrayPush(scopesArray,
                                                        globalObj));

      CHAKRA_VERIFY_NOERROR(InspectorHelpers::SetStringProperty(
          globalObj, "type", "global"));

      JsValueRef object = nullptr;
      CHAKRA_VERIFY_NOERROR(JsCreateObject(&object));
      CHAKRA_VERIFY_NOERROR(InspectorHelpers::SetProperty(globalObj,
                                                          "object",
                                                          object));

      CHAKRA_VERIFY_NOERROR(InspectorHelpers::SetStringProperty(
          object, "type", "object"));
      CHAKRA_VERIFY_NOERROR(InspectorHelpers::SetStringProperty(
          object, "className", "global"));
      CHAKRA_VERIFY_NOERROR(InspectorHelpers::SetStringProperty(
          object, "description", "global"));
      CHAKRA_VERIFY_NOERROR(InspectorHelpers::SetStringProperty(
          object, "objectId",
          GetObjectIdForFrameProp(callFrameIndex, "globals").c_str()));
    }
  }

  static void AddScopeInfo(JsValueRef wrappedCallFrame, int callFrameIndex,
                           JsValueRef stackProperties) {
    JsValueRef scopeChain = nullptr;
    CHAKRA_VERIFY_NOERROR(JsCreateArray(0, &scopeChain));
    CHAKRA_VERIFY_NOERROR(InspectorHelpers::SetProperty(wrappedCallFrame,
                                                        "scopeChain",
                                                        scopeChain));

    AddLocalScopeInfo(scopeChain, callFrameIndex, stackProperties);

    bool hasProperty = false;
    CHAKRA_VERIFY_NOERROR(InspectorHelpers::HasProperty(stackProperties,
                                                        "scopes",
                                                        &hasProperty));

    if (hasProperty) {
      JsValueRef scopes = nullptr;
      CHAKRA_VERIFY_NOERROR(InspectorHelpers::GetProperty(stackProperties,
                                                          "scopes",
                                                          &scopes));

      int length = 0;
      CHAKRA_VERIFY_NOERROR(InspectorHelpers::GetIntProperty(scopes,
                                                             "length",
                                                             &length));

      for (int i = 0; i < length; i++) {
        JsValueRef scope = nullptr;
        CHAKRA_VERIFY_NOERROR(InspectorHelpers::GetIndexedProperty(scopes, i,
                                                                   &scope));

        AddClosureScopeInfo(scopeChain, scope);
      }
    }

    AddGlobalScopeInfo(scopeChain, callFrameIndex, stackProperties);
  }

  JsErrorCode InspectorHelpers::HasProperty(JsValueRef obj, const char *name,
                                            bool *hasProperty) {
    JsPropertyIdRef propId = nullptr;
    IfJsErrorRet(JsCreatePropertyId(name, strlen(name), &propId));
    IfJsErrorRet(JsHasProperty(obj, propId, hasProperty));

    return JsNoError;
  }

  JsErrorCode InspectorHelpers::GetProperty(JsValueRef obj, const char *name,
                                            JsValueRef *value) {
    JsPropertyIdRef propId = nullptr;
    IfJsErrorRet(JsCreatePropertyId(name, strlen(name), &propId));
    IfJsErrorRet(JsGetProperty(obj, propId, value));

    return JsNoError;
  }

  JsErrorCode InspectorHelpers::GetBoolProperty(JsValueRef obj,
                                                const char *name,
                                                bool *value) {
    JsValueRef propVal = nullptr;
    IfJsErrorRet(GetProperty(obj, name, &propVal));
    IfJsErrorRet(ValueToNative</*LIKELY*/true>(JsConvertValueToBoolean,
                                               JsBooleanToBool, propVal,
                                               value));

    return JsNoError;
  }

  JsErrorCode InspectorHelpers::GetIntProperty(JsValueRef obj,
                                               const char *name,
                                               int *value) {
    JsValueRef propVal = nullptr;
    IfJsErrorRet(GetProperty(obj, name, &propVal));
    IfJsErrorRet(ValueToIntLikely(propVal, value));

    return JsNoError;
  }

  JsErrorCode InspectorHelpers::GetIndexedProperty(JsValueRef obj, int index,
                                                   JsValueRef *value) {
    JsValueRef indexVal = nullptr;
    IfJsErrorRet(JsIntToNumber(index, &indexVal));
    IfJsErrorRet(JsGetIndexedProperty(obj, indexVal, value));

    return JsNoError;
  }

  JsErrorCode InspectorHelpers::SetProperty(JsValueRef obj, const char *name,
                                            JsValueRef value) {
    JsPropertyIdRef propId = nullptr;
    IfJsErrorRet(JsCreatePropertyId(name, strlen(name), &propId));
    IfJsErrorRet(JsSetProperty(obj, propId, value, true));

    return JsNoError;
  }

  JsErrorCode InspectorHelpers::SetBoolProperty(JsValueRef obj,
                                                const char *name,
                                                bool value) {
    JsValueRef boolVal = nullptr;
    IfJsErrorRet(JsBoolToBoolean(value, &boolVal));

    JsPropertyIdRef propId = nullptr;
    IfJsErrorRet(JsCreatePropertyId(name, strlen(name), &propId));
    IfJsErrorRet(JsSetProperty(obj, propId, boolVal, true));

    return JsNoError;
  }

  JsErrorCode InspectorHelpers::SetStringProperty(JsValueRef obj,
                                                  const char *name,
                                                  const char *value) {
    JsValueRef stringVal = nullptr;
    IfJsErrorRet(JsCreateString(value, strlen(value), &stringVal));

    JsPropertyIdRef propId = nullptr;
    IfJsErrorRet(JsCreatePropertyId(name, strlen(name), &propId));
    IfJsErrorRet(JsSetProperty(obj, propId, stringVal, true));

    return JsNoError;
  }

  JsErrorCode InspectorHelpers::SetIndexedProperty(JsValueRef obj, int index,
                                                   JsValueRef value) {
    JsValueRef indexVal = nullptr;
    IfJsErrorRet(JsIntToNumber(index, &indexVal));
    IfJsErrorRet(JsSetIndexedProperty(obj, indexVal, value));

    return JsNoError;
  }

  JsErrorCode InspectorHelpers::TryCopyProperty(
      JsValueRef sourceObj,
      const char *sourceName,
      JsValueRef destObj,
      const char *destName,
      bool *wasCopied) {
    return CopyPropertyImpl(sourceObj, sourceName, destObj, destName,
                            static_cast<ConvertFunc>(nullptr), wasCopied);
  }

  JsErrorCode InspectorHelpers::TryCopyPropertyString(
      JsValueRef sourceObj,
      const char *sourceName,
      JsValueRef destObj,
      const char *destName,
      bool *wasCopied) {
    return CopyPropertyImpl(sourceObj, sourceName, destObj, destName,
                            &JsConvertValueToString, wasCopied);
  }

  JsErrorCode InspectorHelpers::ArrayConcat(JsValueRef array,
                                            JsValueRef value,
                                            JsValueRef* result) {
    if (array == nullptr || value == nullptr || result == nullptr) {
      return JsErrorInvalidArgument;
    }

    JsValueRef concatFunc = nullptr;
    IfJsErrorRet(InspectorHelpers::GetProperty(array, "concat", &concatFunc),
                 JsErrorInvalidArgument);

    JsValueRef args[] = { array, value };
    IfJsErrorRet(JsCallFunction(concatFunc, args, _countof(args), result));

    return JsNoError;
  }

  JsErrorCode InspectorHelpers::ArrayPush(JsValueRef array, JsValueRef value) {
    if (array == nullptr || value == nullptr) {
      return JsErrorInvalidArgument;
    }

    JsValueRef pushFunc = nullptr;
    IfJsErrorRet(InspectorHelpers::GetProperty(array, "push", &pushFunc),
                 JsErrorInvalidArgument);

    JsValueRef args[] = { array, value };
    IfJsErrorRet(JsCallFunction(pushFunc, args, _countof(args), nullptr));

    return JsNoError;
  }

  v8::Local<v8::Value> InspectorHelpers::WrapEvaluateObject(
      JsValueRef sourceObject) {
    JsValueRef resultObj = nullptr;
    WrapObject(sourceObject, &resultObj);

    return v8::Utils::ToLocal(static_cast<v8::Value*>(resultObj));
  }

  v8::Local<v8::Object> InspectorHelpers::WrapCallFrameDetails(
      JsValueRef callFrame) {
    JsValueRef wrappedObj = nullptr;
    CHAKRA_VERIFY_NOERROR(JsCreateObject(&wrappedObj));

    AddFunctionInfo(wrappedObj, callFrame);
    AddLocationInfo(wrappedObj, callFrame);

    int callFrameIndex = 0;
    CHAKRA_VERIFY_NOERROR(InspectorHelpers::GetIntProperty(callFrame, "index",
                                                           &callFrameIndex));

    JsValueRef stackProperties = nullptr;
    CHAKRA_VERIFY_NOERROR(JsDiagGetStackProperties(callFrameIndex,
                                                   &stackProperties));

    AddScopeInfo(wrappedObj, callFrameIndex, stackProperties);

    bool hasProperty = false;
    CHAKRA_VERIFY_NOERROR(InspectorHelpers::HasProperty(stackProperties,
                                                        "thisObject",
                                                        &hasProperty));

    JsValueRef thisObj = nullptr;

    if (hasProperty) {
      JsValueRef thisObject = nullptr;
      CHAKRA_VERIFY_NOERROR(InspectorHelpers::GetProperty(stackProperties,
                                                          "thisObject",
                                                          &thisObject));
      WrapObject(thisObject, &thisObj);
    } else {
      // The protocol requires a "this" member, so create an undefined object
      // to return.
      CHAKRA_VERIFY_NOERROR(JsCreateObject(&thisObj));
      CHAKRA_VERIFY_NOERROR(InspectorHelpers::SetStringProperty(
          thisObj, "type", "undefined"));
    }
    
    CHAKRA_VERIFY_NOERROR(InspectorHelpers::SetProperty(wrappedObj,
                                                        "this",
                                                        thisObj));

    CHAKRA_VERIFY_NOERROR(InspectorHelpers::HasProperty(stackProperties,
                                                        "returnValue",
                                                        &hasProperty));

    if (hasProperty) {
      JsValueRef returnObj = nullptr;
      CHAKRA_VERIFY_NOERROR(InspectorHelpers::GetProperty(stackProperties,
                                                          "returnValue",
                                                          &returnObj));

      JsValueRef returnValue = nullptr;
      WrapObject(returnObj, &returnValue);
      CHAKRA_VERIFY_NOERROR(InspectorHelpers::SetProperty(wrappedObj,
                                                          "returnValue",
                                                          returnValue));
    }

    return v8::Utils::ToLocal(static_cast<v8::Object*>(wrappedObj));
  }

  v8::Local<v8::Value> InspectorHelpers::WrapRuntimeException(
      JsValueRef eventData) {
    bool hasProperty = false;
    CHAKRA_VERIFY_NOERROR(InspectorHelpers::HasProperty(eventData,
                                                        "exception",
                                                        &hasProperty));

    if (!hasProperty) {
      return v8::Local<v8::Value>();
    }

    JsValueRef exceptionProperty = nullptr;
    CHAKRA_VERIFY_NOERROR(InspectorHelpers::GetProperty(eventData, "exception",
      &exceptionProperty));

    return WrapException(exceptionProperty);
  }

  v8::Local<v8::Value> InspectorHelpers::WrapPropertiesArray(
      JsValueRef properties) {
    JsValueRef propsArray = nullptr;
    CHAKRA_VERIFY_NOERROR(JsCreateArray(0, &propsArray));
    int nextIndex = 0;

    CHAKRA_VERIFY(properties != nullptr);

    int length = 0;
    CHAKRA_VERIFY_NOERROR(InspectorHelpers::GetIntProperty(properties,
                                                           "length", &length));

    for (int i = 0; i < length; i++) {
      JsValueRef index = nullptr;
      CHAKRA_VERIFY_NOERROR(JsIntToNumber(i, &index));

      JsValueRef propValue = nullptr;
      CHAKRA_VERIFY_NOERROR(JsGetIndexedProperty(properties, index,
                                                 &propValue));

      JsValueRef wrappedProp = nullptr;
      InspectorHelpers::WrapProperty(propValue, &wrappedProp);
      InspectorHelpers::SetIndexedProperty(propsArray, nextIndex++,
                                           wrappedProp);
    }

    return v8::Utils::ToLocal(static_cast<v8::Value*>(propsArray));
  }

  v8::Local<v8::Value> InspectorHelpers::GetWrappedProperties(int handle) {
    JsValueRef diagProperties = nullptr;
    JsErrorCode err = JsDiagGetProperties(handle, 0, MaxPropertyCount,
                                          &diagProperties);
    if (err == JsErrorDiagInvalidHandle) {
      // The handle is no longer valid, this is likely due to a pending
      // request that wasn't serviced before the last continuation.
      return v8::Local<v8::Value>();
    }
    CHAKRA_VERIFY_NOERROR(err);

    JsValueRef allPropsArray = nullptr;
    CHAKRA_VERIFY_NOERROR(JsCreateArray(0, &allPropsArray));

    JsValueRef propertiesArray = nullptr;
    CHAKRA_VERIFY_NOERROR(InspectorHelpers::GetProperty(
        diagProperties, "properties", &propertiesArray));
    CHAKRA_VERIFY_NOERROR(InspectorHelpers::ArrayConcat(allPropsArray,
                                                        propertiesArray,
                                                        &allPropsArray));

    JsValueRef debuggerOnlyPropertiesArray = nullptr;
    CHAKRA_VERIFY_NOERROR(InspectorHelpers::GetProperty(
        diagProperties, "debuggerOnlyProperties",
        &debuggerOnlyPropertiesArray));
    CHAKRA_VERIFY_NOERROR(InspectorHelpers::ArrayConcat(
        allPropsArray, debuggerOnlyPropertiesArray, &allPropsArray));

    return InspectorHelpers::WrapPropertiesArray(allPropsArray);
  }

  v8::Local<v8::Value> InspectorHelpers::GetWrappedStackLocals(
      JsValueRef stackProperties) {
    JsValueRef localsArray = nullptr;
    CHAKRA_VERIFY_NOERROR(JsCreateArray(0, &localsArray));

    bool hasProp = false;
    CHAKRA_VERIFY_NOERROR(InspectorHelpers::HasProperty(stackProperties,
                                                        "exception",
                                                        &hasProp));

    if (hasProp) {
      JsValueRef exception = nullptr;
      CHAKRA_VERIFY_NOERROR(InspectorHelpers::GetProperty(stackProperties,
                                                          "exception",
                                                          &exception));
      CHAKRA_VERIFY_NOERROR(InspectorHelpers::ArrayConcat(localsArray,
                                                          exception,
                                                          &localsArray));
    }

    CHAKRA_VERIFY_NOERROR(InspectorHelpers::HasProperty(stackProperties,
                                                        "arguments",
                                                        &hasProp));

    if (hasProp) {
      JsValueRef arguments = nullptr;
      CHAKRA_VERIFY_NOERROR(InspectorHelpers::GetProperty(stackProperties,
                                                          "arguments",
                                                          &arguments));
      CHAKRA_VERIFY_NOERROR(InspectorHelpers::ArrayConcat(localsArray,
                                                          arguments,
                                                          &localsArray));
    }

    CHAKRA_VERIFY_NOERROR(InspectorHelpers::HasProperty(stackProperties,
                                                        "functionCallsReturn",
                                                        &hasProp));

    if (hasProp) {
      JsValueRef functionCallsReturn = nullptr;
      CHAKRA_VERIFY_NOERROR(InspectorHelpers::GetProperty(
          stackProperties, "functionCallsReturn", &functionCallsReturn));
      CHAKRA_VERIFY_NOERROR(InspectorHelpers::ArrayConcat(localsArray,
                                                          functionCallsReturn,
                                                          &localsArray));
    }

    CHAKRA_VERIFY_NOERROR(InspectorHelpers::HasProperty(stackProperties,
                                                        "locals", &hasProp));

    if (hasProp) {
      JsValueRef locals = nullptr;
      CHAKRA_VERIFY_NOERROR(InspectorHelpers::GetProperty(stackProperties,
                                                          "locals", &locals));
      CHAKRA_VERIFY_NOERROR(InspectorHelpers::ArrayConcat(localsArray, locals,
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

    JsValueRef evalResult = nullptr;
    JsErrorCode err = JsDiagEvaluate(expression, ordinal,
                                     JsParseScriptAttributeNone,
                                     returnByValue, &evalResult);

    if (err == JsErrorScriptException) {
      if (isError != nullptr) {
        *isError = true;
      }

      return WrapException(evalResult);
    }
    else if (err == JsErrorDiagNotAtBreak) {
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
    CHAKRA_VERIFY_NOERROR(InspectorHelpers::GetIntProperty(callFrame, "index",
                                                           &ordinal));

    return EvaluateOnCallFrame(ordinal, expression, returnByValue, isError);
  }

  v8::Local<v8::Value> InspectorHelpers::GetScriptSource(
      unsigned int scriptId) {
    JsValueRef scriptSource = nullptr;
    CHAKRA_VERIFY_NOERROR(JsDiagGetSource(scriptId, &scriptSource));

    JsValueRef sourceStr = nullptr;
    CHAKRA_VERIFY_NOERROR(InspectorHelpers::GetProperty(scriptSource, "source",
                                                        &sourceStr));

    return v8::Utils::ToLocal(static_cast<v8::Value*>(sourceStr));
  }

  JsRuntimeHandle InspectorHelpers::GetRuntimeFromIsolate(
      v8::Isolate *isolate) {
    return IsolateShim::FromIsolate(isolate)->GetRuntimeHandle();
  }

  void InspectorHelpers::WrapObject(JsValueRef obj, JsValueRef* wrappedObj) {
    JsValueRef value = nullptr;
    CHAKRA_VERIFY_NOERROR(JsCreateObject(&value));

    CHAKRA_VERIFY(CopyPropertyIfPresent(obj, "type", value));

    CopyPropertyIfPresent(obj, "className", value);

    bool hasValue = CopyPropertyIfPresent(obj, "value", value);
    bool hasDisplay = CopyPropertyIfPresent(obj, "display", value,
                                            "description");

    // A description is required for values to be shown in the debugger.
    if (hasValue && !hasDisplay) {
      hasDisplay = CopyPropertyStringIfPresent(obj, "value", value,
                                               "description");
    }

    CHAKRA_VERIFY(hasDisplay);

    int handle = 0;
    CHAKRA_VERIFY_NOERROR(InspectorHelpers::GetIntProperty(obj, "handle",
                                                           &handle));
    CHAKRA_VERIFY_NOERROR(InspectorHelpers::SetStringProperty(
        value, "objectId", GetObjectIdForHandle(handle).c_str()));

    *wrappedObj = value;
  }

  void InspectorHelpers::WrapProperty(JsValueRef propValue,
                                      JsValueRef* wrappedProperty) {
    JsValueRef wrappedObj = nullptr;
    CHAKRA_VERIFY_NOERROR(JsCreateObject(&wrappedObj));

    CHAKRA_VERIFY(CopyPropertyIfPresent(propValue, "name", wrappedObj));

    JsValueRef value = nullptr;
    WrapObject(propValue, &value);
    CHAKRA_VERIFY_NOERROR(InspectorHelpers::SetProperty(wrappedObj, "value",
                                                        value));

    int propertyAttributes = 0;
    CHAKRA_VERIFY_NOERROR(InspectorHelpers::GetIntProperty(
        propValue,"propertyAttributes", &propertyAttributes));
    CHAKRA_VERIFY_NOERROR(InspectorHelpers::SetBoolProperty(
        wrappedObj, "writable",
        (propertyAttributes & JsrtDebugPropertyAttributeReadOnly) == 0));
    CHAKRA_VERIFY_NOERROR(InspectorHelpers::SetBoolProperty(wrappedObj,
                                                            "configurable",
                                                            false));
    CHAKRA_VERIFY_NOERROR(InspectorHelpers::SetBoolProperty(wrappedObj,
                                                            "enumerable",
                                                            false));

    *wrappedProperty = wrappedObj;
  }

  v8::Local<v8::Value> InspectorHelpers::WrapException(JsValueRef exception) {
    JsValueRef exceptionVal = nullptr;
    CHAKRA_VERIFY_NOERROR(JsCreateObject(&exceptionVal));

    CHAKRA_VERIFY(CopyPropertyIfPresent(exception, "type", exceptionVal));
    CHAKRA_VERIFY_NOERROR(InspectorHelpers::SetStringProperty(exceptionVal,
                                                              "subtype",
                                                              "error"));
    CHAKRA_VERIFY(CopyPropertyIfPresent(exception, "className",
                                        exceptionVal));
    CHAKRA_VERIFY(CopyPropertyIfPresent(exception, "display",
                                        exceptionVal, "description"));

    int handle = 0;
    CHAKRA_VERIFY_NOERROR(InspectorHelpers::GetIntProperty(exception,
                                                           "handle",
                                                           &handle));
    CHAKRA_VERIFY_NOERROR(InspectorHelpers::SetStringProperty(
        exceptionVal, "objectId", GetObjectIdForHandle(handle).c_str()));

    return v8::Utils::ToLocal(static_cast<v8::Value*>(exceptionVal));
  }

}  // namespace jsrt
