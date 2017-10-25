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

#include <stdarg.h>
#include "jsrtutils.h"
#include <string>
#if !defined(_WIN32) && !defined(__APPLE__)
#include <limits.h>  // UINT_MAX
#endif
#include "pal/pal.h"

namespace jsrt {

JsErrorCode UintToValue(uint32_t value, JsValueRef* result) {
  if (static_cast<int>(value) >= 0) {
    return JsIntToNumber(static_cast<int>(value), result);
  }

  // Otherwise doesn't fit int, use double
  return JsDoubleToNumber(value, result);
}

JsErrorCode GetProperty(JsValueRef ref,
                        JsValueRef propName,
                        JsValueRef *result) {
  JsPropertyIdRef idRef = JS_INVALID_REFERENCE;
  IfJsErrorRet(GetPropertyIdFromName(propName, &idRef));
  IfJsErrorRet(JsGetProperty(ref, idRef, result));

  return JsNoError;
}

JsErrorCode GetProperty(JsValueRef ref,
                        const char *propertyName,
                        JsValueRef *result) {
  JsPropertyIdRef idRef = JS_INVALID_REFERENCE;
  IfJsErrorRet(CreatePropertyId(propertyName, &idRef));
  IfJsErrorRet(JsGetProperty(ref, idRef, result));

  return JsNoError;
}

JsErrorCode GetProperty(JsValueRef ref,
                        CachedPropertyIdRef cachedIdRef,
                        JsValueRef *result) {
  JsPropertyIdRef idRef =
    IsolateShim::GetCurrent()->GetCachedPropertyIdRef(cachedIdRef);

  return JsGetProperty(ref, idRef, result);
}

JsErrorCode GetProperty(JsValueRef ref,
                        JsPropertyIdRef propId,
                        bool *boolValue) {
  JsValueRef value = JS_INVALID_REFERENCE;
  IfJsErrorRet(JsGetProperty(ref, propId, &value));

  return ValueToBoolLikely(value, boolValue);
}

JsErrorCode GetProperty(JsValueRef ref,
                        CachedPropertyIdRef cachedIdRef,
                        bool *boolValue) {
  JsPropertyIdRef idRef =
      IsolateShim::GetCurrent()->GetCachedPropertyIdRef(cachedIdRef);

  return GetProperty(ref, idRef, boolValue);
}

JsErrorCode GetProperty(JsValueRef ref,
                        JsPropertyIdRef propId,
                        int *intValue) {
  JsValueRef value = JS_INVALID_REFERENCE;
  IfJsErrorRet(JsGetProperty(ref, propId, &value));

  return ValueToIntLikely(value, intValue);
}

JsErrorCode GetProperty(JsValueRef ref,
                        CachedPropertyIdRef cachedIdRef,
                        int *intValue) {
  JsPropertyIdRef idRef =
      IsolateShim::GetCurrent()->GetCachedPropertyIdRef(cachedIdRef);

  return GetProperty(ref, idRef, intValue);
}

JsErrorCode SetProperty(JsValueRef ref,
                        CachedPropertyIdRef cachedIdRef,
                        JsValueRef propValue) {
  JsPropertyIdRef idRef =
    IsolateShim::GetCurrent()->GetCachedPropertyIdRef(cachedIdRef);
  return JsSetProperty(ref, idRef, propValue, false);
}

JsErrorCode SetProperty(JsValueRef ref,
                        JsValueRef propName,
                        JsValueRef propValue) {
  JsPropertyIdRef idRef = JS_INVALID_REFERENCE;
  IfJsErrorRet(GetPropertyIdFromName(propName, &idRef));
  IfJsErrorRet(JsSetProperty(ref, idRef, propValue, false));

  return JsNoError;
}

JsErrorCode SetProperty(JsValueRef ref,
                        const char *propertyName,
                        JsValueRef propValue) {
  JsPropertyIdRef idRef = JS_INVALID_REFERENCE;
  IfJsErrorRet(CreatePropertyId(propertyName, &idRef));
  IfJsErrorRet(JsSetProperty(ref, idRef, propValue, false));

  return JsNoError;
}

JsErrorCode SetProperty(JsValueRef ref,
                        JsPropertyIdRef propId,
                        bool boolValue) {
  JsValueRef boolVal = JS_INVALID_REFERENCE;
  IfJsErrorRet(JsBoolToBoolean(boolValue, &boolVal));
  IfJsErrorRet(JsSetProperty(ref, propId, boolVal, false));

  return JsNoError;
}

JsErrorCode SetProperty(JsValueRef ref,
                        CachedPropertyIdRef cachedIdRef,
                        bool boolValue) {
  JsPropertyIdRef idRef =
    IsolateShim::GetCurrent()->GetCachedPropertyIdRef(cachedIdRef);

  return SetProperty(ref, idRef, boolValue);
}

JsErrorCode SetProperty(JsValueRef ref,
                        JsPropertyIdRef propId,
                        const char *stringValue) {
  JsValueRef stringVal = JS_INVALID_REFERENCE;
  IfJsErrorRet(JsCreateString(stringValue, strlen(stringValue), &stringVal));
  IfJsErrorRet(JsSetProperty(ref, propId, stringVal, false));

  return JsNoError;
}

JsErrorCode SetProperty(JsValueRef ref,
                        CachedPropertyIdRef cachedIdRef,
                        const char *stringValue) {
  JsPropertyIdRef idRef =
    IsolateShim::GetCurrent()->GetCachedPropertyIdRef(cachedIdRef);

  return SetProperty(ref, idRef, stringValue);
}

JsErrorCode DeleteProperty(JsValueRef ref,
                           JsValueRef propName,
                           JsValueRef* result) {
  JsPropertyIdRef idRef = JS_INVALID_REFERENCE;
  IfJsErrorRet(GetPropertyIdFromName(propName, &idRef));
  IfJsErrorRet(JsDeleteProperty(ref, idRef, false, result));

  return JsNoError;
}

JsErrorCode CallProperty(JsValueRef ref,
                         CachedPropertyIdRef cachedIdRef,
                         JsValueRef *arguments,
                         unsigned short argumentCount,  // NOLINT(runtime/int)
                         JsValueRef *result) {
  JsValueRef propertyRef = JS_INVALID_REFERENCE;
  IfJsErrorRet(JsGetProperty(
      ref,
      IsolateShim::GetCurrent()->GetCachedPropertyIdRef(cachedIdRef),
      &propertyRef));

  IfJsErrorRet(JsCallFunction(propertyRef, arguments, argumentCount, result));

  return JsNoError;
}

JsErrorCode CallGetter(JsValueRef ref,
                       CachedPropertyIdRef cachedIdRef,
                       JsValueRef* result) {
  JsValueRef args[] = { ref };
  return CallProperty(ref, cachedIdRef, args, _countof(args), result);
}

JsErrorCode CallGetter(JsValueRef ref,
                       CachedPropertyIdRef cachedIdRef,
                       int* result) {
  JsValueRef value = JS_INVALID_REFERENCE;
  IfJsErrorRet(CallGetter(ref, cachedIdRef, &value));

  return ValueToIntLikely(value, result);
}

JsErrorCode GetPropertyOfGlobal(const char *propertyName,
                                JsValueRef *ref) {
  JsPropertyIdRef propertyIdRef, globalRef;
  IfJsErrorRet(CreatePropertyId(propertyName, &propertyIdRef));
  IfJsErrorRet(JsGetGlobalObject(&globalRef));

  return JsGetProperty(globalRef, propertyIdRef, ref);
}

JsErrorCode SetPropertyOfGlobal(const char *propertyName,
                                JsValueRef ref) {
  JsPropertyIdRef propertyIdRef, globalRef;
  IfJsErrorRet(CreatePropertyId(propertyName, &propertyIdRef));
  IfJsErrorRet(JsGetGlobalObject(&globalRef));

  return JsSetProperty(globalRef, propertyIdRef, ref, false);
}

JsValueRef GetTrue() {
  return ContextShim::GetCurrent()->GetTrue();
}

JsValueRef GetFalse() {
  return ContextShim::GetCurrent()->GetFalse();
}

JsValueRef GetUndefined() {
  return ContextShim::GetCurrent()->GetUndefined();
}

JsValueRef GetNull() {
  return ContextShim::GetCurrent()->GetNull();
}

JsErrorCode GetArrayLength(JsValueRef arrayRef,
                           unsigned int *arraySize) {
  JsPropertyIdRef arrayLengthPropertyIdRef =
    IsolateShim::GetCurrent()->GetCachedPropertyIdRef(
      CachedPropertyIdRef::length);
  JsValueRef lengthRef = JS_INVALID_REFERENCE;
  IfJsErrorRet(JsGetProperty(arrayRef, arrayLengthPropertyIdRef, &lengthRef));

  double sizeInDouble = 0;
  IfJsErrorRet(JsNumberToDouble(lengthRef, &sizeInDouble));
  *arraySize = static_cast<unsigned int>(sizeInDouble);

  return JsNoError;
}

bool InstanceOf(JsValueRef first,
                JsValueRef second) {
  bool result = false;
  return JsInstanceOf(first, second, &result) == JsNoError && result;
}

JsErrorCode CloneObject(JsValueRef source,
                        JsValueRef target,
                        bool clonePrototype) {
  JsValueRef cloneObjectFunction =
    ContextShim::GetCurrent()->GetcloneObjectFunction();

  JsValueRef resultRef = JS_INVALID_REFERENCE;
  IfJsErrorRet(CallFunction(cloneObjectFunction, source, target, &resultRef));

  if (clonePrototype) {
    JsValueRef prototypeRef = JS_INVALID_REFERENCE;
    IfJsErrorRet(JsGetPrototype(source, &prototypeRef));
    IfJsErrorRet(JsSetPrototype(target, prototypeRef));
  }

  return JsNoError;
}

JsErrorCode HasOwnProperty(JsValueRef object,
                           JsValueRef prop,
                           JsValueRef *result) {
  if (result == nullptr) {
    return JsErrorInvalidArgument;
  }

  *result = JS_INVALID_REFERENCE;

  JsPropertyIdRef propId = JS_INVALID_REFERENCE;
  IfJsErrorRet(GetPropertyIdFromValue(prop, &propId));

  bool hasOwnProperty = false;
  IfJsErrorRet(JsHasOwnProperty(object, propId, &hasOwnProperty));
  IfJsErrorRet(JsBoolToBoolean(hasOwnProperty, result));

  return JsNoError;
}

JsErrorCode GetOwnPropertyDescriptor(JsValueRef ref,
                                     JsValueRef prop,
                                     JsValueRef* result) {
  JsPropertyIdRef idRef = JS_INVALID_REFERENCE;
  IfJsErrorRet(GetPropertyIdFromName(prop, &idRef));

  return JsGetOwnPropertyDescriptor(ref, idRef, result);
}

JsErrorCode IsZero(JsValueRef value,
                   bool *result) {
  return JsEquals(value, ContextShim::GetCurrent()->GetZero(), result);
}

JsErrorCode IsUndefined(JsValueRef value,
                        bool *result) {
  return JsEquals(value, GetUndefined(), result);
}

JsErrorCode GetEnumerableNamedProperties(JsValueRef object,
                                         JsValueRef *result) {
  return CallFunction(
    ContextShim::GetCurrent()->GetgetEnumerableNamedPropertiesFunction(),
    object, result);
}

JsErrorCode GetEnumerableIndexedProperties(JsValueRef object,
                                           JsValueRef *result) {
  return CallFunction(
    ContextShim::GetCurrent()->GetgetEnumerableIndexedPropertiesFunction(),
    object, result);
}

JsErrorCode GetIndexedOwnKeys(JsValueRef object,
                              JsValueRef *result) {
  return CallFunction(
    ContextShim::GetCurrent()->GetgetIndexedOwnKeysFunction(),
    object, result);
}

JsErrorCode GetNamedOwnKeys(JsValueRef object,
                            JsValueRef *result) {
  return CallFunction(
    ContextShim::GetCurrent()->GetgetNamedOwnKeysFunction(),
    object, result);
}

JsErrorCode ConcatArray(JsValueRef first,
                        JsValueRef second,
                        JsValueRef *result) {
  JsValueRef args[] = { first, second };

  return CallProperty(first,
                      CachedPropertyIdRef::concat,
                      args, _countof(args), result);
}

JsErrorCode PushArray(JsValueRef array,
                      JsValueRef item,
                      JsValueRef *result) {
  JsValueRef args[] = { array, item };

  return CallProperty(array,
                      CachedPropertyIdRef::push,
                      args, _countof(args), result);
}

JsErrorCode CreateEnumerationIterator(JsValueRef enumeration,
                                      JsValueRef *result) {
  return CallFunction(
    ContextShim::GetCurrent()->GetcreateEnumerationIteratorFunction(),
    enumeration, result);
}

JsErrorCode CreatePropertyDescriptorsEnumerationIterator(JsValueRef enumeration,
                                                         JsValueRef *result) {
  return CallFunction(
    ContextShim::GetCurrent()
      ->GetcreatePropertyDescriptorsEnumerationIteratorFunction(),
    enumeration, result);
}

JsErrorCode GetPropertyNames(JsValueRef object,
                             JsValueRef *result) {
  return CallFunction(
    ContextShim::GetCurrent()->GetgetPropertyNamesFunction(),
    object, result);
}

JsErrorCode AddExternalData(JsValueRef ref,
                            JsPropertyIdRef externalDataPropertyId,
                            void *data,
                            JsFinalizeCallback onObjectFinalize) {
  JsValueRef externalObjectRef = JS_INVALID_REFERENCE;
  IfJsErrorRet(JsCreateExternalObject(data, onObjectFinalize,
                                      &externalObjectRef));

  IfJsErrorRet(DefineProperty(ref,
                              externalDataPropertyId,
                              PropertyDescriptorOptionValues::False,
                              PropertyDescriptorOptionValues::False,
                              PropertyDescriptorOptionValues::False,
                              externalObjectRef,
                              JS_INVALID_REFERENCE, JS_INVALID_REFERENCE));

  return JsNoError;
}

JsErrorCode AddExternalData(JsValueRef ref,
                            void *data,
                            JsFinalizeCallback onObjectFinalize) {
  IsolateShim* iso = IsolateShim::GetCurrent();
  JsPropertyIdRef propId = iso->GetCachedSymbolPropertyIdRef(
    CachedSymbolPropertyIdRef::__external__);

  return AddExternalData(ref, propId, data, onObjectFinalize);
}

JsErrorCode GetExternalData(JsValueRef ref,
                            JsPropertyIdRef idRef,
                            void **data) {
  JsValueRef externalObject = JS_INVALID_REFERENCE;
  IfJsErrorRet(JsGetProperty(ref, idRef, &externalObject));

  JsErrorCode error = JsGetExternalData(externalObject, data);
  if (error == JsErrorInvalidArgument) {
    *data = nullptr;
    error = JsNoError;
  }

  return error;
}

JsErrorCode GetExternalData(JsValueRef ref,
                            void **data) {
  IsolateShim* iso = IsolateShim::GetCurrent();
  JsPropertyIdRef propId = iso->GetCachedSymbolPropertyIdRef(
    CachedSymbolPropertyIdRef::__external__);

  return GetExternalData(ref, propId, data);
}

JsErrorCode CreateFunctionWithExternalData(
    JsNativeFunction nativeFunction,
    void* data,
    JsFinalizeCallback onObjectFinalize,
    JsValueRef *function) {
  IfJsErrorRet(JsCreateFunction(nativeFunction, nullptr, function));
  IfJsErrorRet(AddExternalData(*function, data, onObjectFinalize));

  return JsNoError;
}

JsErrorCode ToString(JsValueRef ref,
                     JsValueRef* strRef, StringUtf8* stringUtf8) {
  IfJsErrorRet(JsConvertValueToString(ref, strRef));
  return stringUtf8->From(*strRef);
}

JsErrorCode CreateString(const char *string,
                         JsValueRef *ref) {
  CHAKRA_VERIFY_NOERROR(JsCreateString(string, strlen(string), ref));

  return JsNoError;
}

JsErrorCode CreatePropertyId(const char *name,
                             JsValueRef *propertyIdRef) {
  CHAKRA_VERIFY_NOERROR(JsCreatePropertyId(name, strlen(name), propertyIdRef));

  return JsNoError;
}


#define DEF_IS_TYPE(F) \
JsErrorCode Call##F(JsValueRef value, JsValueRef *resultRef) { \
  return CallFunction( \
    ContextShim::GetCurrent()->Get##F##Function(), \
    value, resultRef); \
} \

#include "jsrtcachedpropertyidref.inc"
#undef DEF_IS_TYPE

PropertyDescriptorOptionValues GetPropertyDescriptorOptionValue(bool b) {
  return b ?
    PropertyDescriptorOptionValues::True :
    PropertyDescriptorOptionValues::False;
}

// CHAKRA-TODO: Convert this function to javascript
JsErrorCode CreatePropertyDescriptor(
    PropertyDescriptorOptionValues writable,
    PropertyDescriptorOptionValues enumerable,
    PropertyDescriptorOptionValues configurable,
    JsValueRef value,
    JsValueRef getter,
    JsValueRef setter,
    JsValueRef *descriptor) {
  IfJsErrorRet(JsCreateObject(descriptor));

  IsolateShim * isolateShim = IsolateShim::GetCurrent();
  ContextShim * contextShim = isolateShim->GetCurrentContextShim();
  JsValueRef trueRef = contextShim->GetTrue();
  JsValueRef falseRef = contextShim->GetFalse();

  // set writable
  if (writable != PropertyDescriptorOptionValues::None) {
    JsPropertyIdRef writablePropertyIdRef =
      isolateShim->GetCachedPropertyIdRef(CachedPropertyIdRef::writable);
    JsValueRef writableRef =
      (writable == PropertyDescriptorOptionValues::True) ? trueRef : falseRef;
    IfJsErrorRet(JsSetProperty(*descriptor, writablePropertyIdRef, writableRef,
                               false));
  }

  // set enumerable
  if (enumerable != PropertyDescriptorOptionValues::None) {
    JsPropertyIdRef enumerablePropertyIdRef =
      isolateShim->GetCachedPropertyIdRef(CachedPropertyIdRef::enumerable);
    JsValueRef enumerableRef =
      (enumerable == PropertyDescriptorOptionValues::True) ? trueRef : falseRef;
    IfJsErrorRet(JsSetProperty(*descriptor, enumerablePropertyIdRef,
                               enumerableRef, false));
  }

  // set configurable
  if (configurable != PropertyDescriptorOptionValues::None) {
    JsPropertyIdRef configurablePropertyIdRef =
      isolateShim->GetCachedPropertyIdRef(CachedPropertyIdRef::configurable);
    JsValueRef configurableRef =
      (configurable == PropertyDescriptorOptionValues::True) ?
        trueRef : falseRef;
    IfJsErrorRet(JsSetProperty(*descriptor, configurablePropertyIdRef,
                               configurableRef, false));
  }

  // set value
  if (value != JS_INVALID_REFERENCE) {
    JsPropertyIdRef valuePropertyIdRef =
      isolateShim->GetCachedPropertyIdRef(CachedPropertyIdRef::value);
    IfJsErrorRet(JsSetProperty(*descriptor, valuePropertyIdRef, value, false));
  }

  // set getter if needed
  if (getter != JS_INVALID_REFERENCE) {
    JsPropertyIdRef getterPropertyIdRef =
      isolateShim->GetCachedPropertyIdRef(CachedPropertyIdRef::get);
    IfJsErrorRet(JsSetProperty(*descriptor, getterPropertyIdRef, getter,
                               false));
  }

  // set setter if needed
  if (setter != JS_INVALID_REFERENCE) {
    JsPropertyIdRef setterPropertyIdRef =
      isolateShim->GetCachedPropertyIdRef(CachedPropertyIdRef::set);
    IfJsErrorRet(JsSetProperty(*descriptor, setterPropertyIdRef, setter,
                               false));
  }

  return JsNoError;
}

JsErrorCode CreatePropertyDescriptor(v8::PropertyAttribute attributes,
                                     JsValueRef value,
                                     JsValueRef getter,
                                     JsValueRef setter,
                                     JsValueRef *descriptor) {
  return CreatePropertyDescriptor(
    GetPropertyDescriptorOptionValue(!(attributes & v8::ReadOnly)),
    GetPropertyDescriptorOptionValue(!(attributes & v8::DontEnum)),
    GetPropertyDescriptorOptionValue(!(attributes & v8::DontDelete)),
    value,
    JS_INVALID_REFERENCE,
    JS_INVALID_REFERENCE,
    descriptor);
}

JsErrorCode DefineProperty(JsValueRef object,
                           JsPropertyIdRef propertyIdRef,
                           PropertyDescriptorOptionValues writable,
                           PropertyDescriptorOptionValues enumerable,
                           PropertyDescriptorOptionValues configurable,
                           JsValueRef value,
                           JsValueRef getter,
                           JsValueRef setter) {
  JsValueRef descriptor = JS_INVALID_REFERENCE;
  IfJsErrorRet(CreatePropertyDescriptor(writable, enumerable, configurable,
                                        value, getter, setter, &descriptor));

  bool result = false;
  IfJsErrorRet(JsDefineProperty(object, propertyIdRef, descriptor, &result));

  if (!result) {
    return JsErrorInvalidArgument;
  }

  return JsNoError;
}

JsErrorCode GetPropertyIdFromName(JsValueRef nameRef,
                                  JsPropertyIdRef *idRef) {
  StringUtf8 str;

  // Expect the name be either a String or a Symbol.
  JsErrorCode error = str.From(nameRef);
  if (error != JsNoError) {
    if (error == JsErrorInvalidArgument) {
      error = JsGetPropertyIdFromSymbol(nameRef, idRef);
      if (error == JsErrorPropertyNotSymbol) {
        error = JsErrorInvalidArgument;  // Neither String nor Symbol
      }
    }
  } else {
    error = JsCreatePropertyId(str, str.length(), idRef);
  }

  return error;
}

JsErrorCode GetPropertyIdFromValue(JsValueRef valueRef,
                                   JsPropertyIdRef *idRef) {
  JsErrorCode error = GetPropertyIdFromName(valueRef, idRef);
  if (error == JsErrorInvalidArgument) {
    IfJsErrorRet(JsConvertValueToString(valueRef, &valueRef));
    IfJsErrorRet(GetPropertyIdFromName(valueRef, idRef));

    return JsNoError;
  }

  return error;
}

JsErrorCode GetObjectConstructor(JsValueRef objectRef,
                                 JsValueRef *constructorRef) {
  IsolateShim* iso = IsolateShim::GetCurrent();
  JsPropertyIdRef constructorPropertyIdRef = iso->GetCachedPropertyIdRef(
    CachedPropertyIdRef::constructor);

  return JsGetProperty(objectRef, constructorPropertyIdRef, constructorRef);
}

JsErrorCode SetIndexedProperty(JsValueRef object,
                               unsigned int index,
                               JsValueRef value) {
  JsValueRef indexRef = JS_INVALID_REFERENCE;
  IfJsErrorRet(UintToValue(index, &indexRef));
  IfJsErrorRet(JsSetIndexedProperty(object, indexRef, value));

  return JsNoError;
}

JsErrorCode GetIndexedProperty(JsValueRef object,
                               unsigned int index,
                               JsValueRef *value) {
  JsValueRef indexRef = JS_INVALID_REFERENCE;
  IfJsErrorRet(UintToValue(index, &indexRef));
  IfJsErrorRet(JsGetIndexedProperty(object, indexRef, value));

  return JsNoError;
}

JsErrorCode DeleteIndexedProperty(JsValueRef object,
                                  unsigned int index) {
  JsValueRef indexRef = JS_INVALID_REFERENCE;
  IfJsErrorRet(UintToValue(index, &indexRef));
  IfJsErrorRet(JsDeleteIndexedProperty(object, indexRef));

  return JsNoError;
}

JsErrorCode HasProperty(JsValueRef object,
                        JsValueRef propName,
                        bool *result) {
  JsPropertyIdRef idRef = JS_INVALID_REFERENCE;
  IfJsErrorRet(GetPropertyIdFromName(propName, &idRef));
  IfJsErrorRet(JsHasProperty(object, idRef, result));

  return JsNoError;
}

JsErrorCode HasProperty(JsValueRef object,
                        CachedPropertyIdRef cachedIdRef,
                        bool *result) {
  JsPropertyIdRef idRef =
    IsolateShim::GetCurrent()->GetCachedPropertyIdRef(cachedIdRef);
  IfJsErrorRet(JsHasProperty(object, idRef, result));

  return JsNoError;
}

JsErrorCode HasIndexedProperty(JsValueRef object,
                               unsigned int index,
                               bool *result) {
  JsValueRef indexRef = JS_INVALID_REFERENCE;
  IfJsErrorRet(UintToValue(index, &indexRef));
  IfJsErrorRet(JsHasIndexedProperty(object, indexRef, result));

  return JsNoError;
}

JsErrorCode ParseScript(StringUtf8 *script,
                        JsSourceContext sourceContext,
                        JsValueRef sourceUrl,
                        bool isStrictMode,
                        JsValueRef *result) {
  if (isStrictMode) {
    // do not append new line so the line numbers on error stack are correct
    std::string useStrictTag("'use strict'; ");
    useStrictTag.append(*script);
    JsValueRef scriptToParse = JS_INVALID_REFERENCE;
    CHAKRA_VERIFY(JsCreateString(useStrictTag.c_str(),
                                 useStrictTag.length(),
                                 &scriptToParse) == JsNoError);
    return JsParse(scriptToParse, sourceContext, sourceUrl,
                   JsParseScriptAttributeNone, result);
  } else {
    JsValueRef scriptToParse = JS_INVALID_REFERENCE;
    CHAKRA_VERIFY(JsCreateString(script->operator*(),
                                 script->length(),
                                 &scriptToParse) == JsNoError);
    return JsParse(scriptToParse, sourceContext, sourceUrl,
                   JsParseScriptAttributeNone, result);
  }
}

JsErrorCode GetHiddenValuesTable(JsValueRef object,
                                JsPropertyIdRef* hiddenValueIdRef,
                                JsValueRef* hiddenValuesTable,
                                bool* isUndefined) {
  *isUndefined = true;
  IsolateShim* iso = IsolateShim::GetCurrent();
  *hiddenValueIdRef = iso->GetCachedSymbolPropertyIdRef(
      CachedSymbolPropertyIdRef::__hiddenvalues__);

  IfJsErrorRet(JsGetProperty(object, *hiddenValueIdRef, hiddenValuesTable));
  IfJsErrorRet(IsUndefined(*hiddenValuesTable, isUndefined));

  return JsNoError;
}

bool HasPrivate(JsValueRef object, JsValueRef key) {
  JsPropertyIdRef hiddenValuesIdRef = JS_INVALID_REFERENCE;
  JsValueRef hiddenValuesTable = JS_INVALID_REFERENCE;
  JsErrorCode errorCode = JsNoError;
  bool isUndefined = false;

  errorCode = GetHiddenValuesTable(object, &hiddenValuesIdRef,
                                   &hiddenValuesTable, &isUndefined);
  IfJsErrorRet(errorCode, false);

  if (isUndefined) {
    return false;
  }

  JsValueRef hasPropertyRef = JS_INVALID_REFERENCE;
  errorCode = HasOwnProperty(hiddenValuesTable, key, &hasPropertyRef);
  IfJsErrorRet(errorCode, false);

  bool hasKey = false;
  errorCode = JsBooleanToBool(hasPropertyRef, &hasKey);
  IfJsErrorRet(errorCode, false);

  return hasKey;
}

bool DeletePrivate(JsValueRef object, JsValueRef key) {
  JsPropertyIdRef hiddenValuesIdRef = JS_INVALID_REFERENCE;
  JsValueRef hiddenValuesTable = JS_INVALID_REFERENCE;
  JsErrorCode errorCode = JsNoError;
  bool isUndefined = false;

  errorCode = GetHiddenValuesTable(object, &hiddenValuesIdRef,
                                   &hiddenValuesTable, &isUndefined);
  IfJsErrorRet(errorCode, false);

  if (isUndefined) {
    return false;
  }

  JsValueRef deleteResultRef = JS_INVALID_REFERENCE;
  errorCode = DeleteProperty(hiddenValuesTable, key, &deleteResultRef);
  IfJsErrorRet(errorCode, false);

  bool hasDeleted = false;
  errorCode = JsBooleanToBool(deleteResultRef, &hasDeleted);
  IfJsErrorRet(errorCode, false);

  return hasDeleted;
}

// CHAKRA-TODO: Convert this function to javascript
JsErrorCode GetPrivate(JsValueRef object, JsValueRef key,
                       JsValueRef *result) {
  JsPropertyIdRef hiddenValuesIdRef = JS_INVALID_REFERENCE;
  JsValueRef hiddenValuesTable = JS_INVALID_REFERENCE;
  JsValueRef undefinedValueRef = GetUndefined();
  bool isUndefined = false;

  IfJsErrorRet(GetHiddenValuesTable(object, &hiddenValuesIdRef,
                                    &hiddenValuesTable, &isUndefined));

  if (isUndefined) {
      *result = undefinedValueRef;
      return JsNoError;
  }

  JsPropertyIdRef keyIdRef = JS_INVALID_REFERENCE;
  IfJsErrorRet(GetPropertyIdFromName(key, &keyIdRef));

  // Is 'key' present in hiddenValuesTable? If not, return undefined
  JsValueRef hasPropertyRef = JS_INVALID_REFERENCE;
  IfJsErrorRet(HasOwnProperty(hiddenValuesTable, key, &hasPropertyRef));

  bool hasKey = false;
  IfJsErrorRet(JsBooleanToBool(hasPropertyRef, &hasKey));

  if (!hasKey) {
    *result = undefinedValueRef;
    return JsNoError;
  }

  IfJsErrorRet(JsGetProperty(hiddenValuesTable, keyIdRef, result));

  return JsNoError;
}

JsErrorCode SetPrivate(JsValueRef object, JsValueRef key,
                           JsValueRef value) {
  JsPropertyIdRef hiddenValuesIdRef = JS_INVALID_REFERENCE;
  JsValueRef hiddenValuesTable = JS_INVALID_REFERENCE;
  bool isUndefined = false;

  IfJsErrorRet(GetHiddenValuesTable(object, &hiddenValuesIdRef,
                                    &hiddenValuesTable, &isUndefined));

  // if '__hiddenvalues__' is not defined on object, define it
  if (isUndefined) {
    IfJsErrorRet(JsCreateObject(&hiddenValuesTable));

    IfJsErrorRet(DefineProperty(object, hiddenValuesIdRef,
                                PropertyDescriptorOptionValues::False,
                                PropertyDescriptorOptionValues::False,
                                PropertyDescriptorOptionValues::False,
                                hiddenValuesTable,
                                JS_INVALID_REFERENCE,
                                JS_INVALID_REFERENCE));
  }

  IfJsErrorRet(SetProperty(hiddenValuesTable, key, value));

  return JsNoError;
}

void Unimplemented(const char * message) {
  fprintf(stderr, "FATAL ERROR: '%s' unimplemented", message);
  __debugbreak();
  abort();
}

void Fatal(const char * format, ...) {
  bool hasException = false;
  JsErrorCode errorCode = JsNoError;
  JsValueRef exceptionRef = JS_INVALID_REFERENCE;
  JsValueRef stackRef = JS_INVALID_REFERENCE;
  JsValueRef strErrorRef = JS_INVALID_REFERENCE;
  StringUtf8 strError;

  va_list args;
  va_start(args, format);
  fprintf(stderr, "FATAL ERROR: ");
  vfprintf(stderr, format, args);
  va_end(args);

  errorCode = JsHasException(&hasException);
  if (!hasException || errorCode != JsNoError) {
    if (errorCode != JsNoError) {
      fprintf(stderr, "\nImportant: While trying to check Javascript "
         "exception, JsHasException has also failed.\n");
    } else {
      fprintf(stderr, "\nImportant: This didn't happen because of an "
         "uncaught Javascript exception.\n");
    }
  } else if (JsGetAndClearException(&exceptionRef) == JsNoError &&
             GetProperty(exceptionRef, "stack", &stackRef) == JsNoError &&
             JsConvertValueToString(stackRef, &strErrorRef) == JsNoError  &&
             strError.From(strErrorRef) == JsNoError) {
    fprintf(stderr, "\n%s\n", (const char*)strError);
  }

#ifdef DEBUG
  __debugbreak();
#endif

  abort();
}


JsValueRef CHAKRA_CALLBACK CollectGarbage(
    JsValueRef callee,
    bool isConstructCall,
    JsValueRef *arguments,
    unsigned short argumentCount,  // NOLINT(runtime/int)
    void *callbackState) {
  JsCollectGarbage(IsolateShim::GetCurrent()->GetRuntimeHandle());
  return GetUndefined();
}

void IdleGC(uv_timer_t *timerHandler) {
  static unsigned int prevIdleTicks = 0;
  static DWORD prevTicks = 0;

  unsigned int currentIdleTicks = 0;
  unsigned int diffIdleTicks = 0, diffTicks = 0;

  CHAKRA_VERIFY(JsIdle(&currentIdleTicks) == JsNoError);
  DWORD currentTicks = GetTickCount();

  diffIdleTicks = currentIdleTicks - prevIdleTicks;
  prevIdleTicks = currentIdleTicks;

  diffTicks = currentTicks - prevTicks;
  prevTicks = currentTicks;

  // If idleGc completed, we don't need to schedule anything.
  // simply reset the script execution flag so that idleGC
  // is retriggered only when scripts are executed.
  if (currentIdleTicks == UINT_MAX) {
    IsolateShim::GetCurrent()->ResetScriptExecuted();
    IsolateShim::GetCurrent()->ResetIsIdleGcScheduled();
    return;
  }

  // If IdleGC didn't complete, retry doing it after diff.
  if (diffIdleTicks > diffTicks) {
    unsigned int diff = diffIdleTicks - diffTicks;
    if (diff > 2000) diff = 2000;  // limit the difference to 2s
    ScheduleIdleGcTask(diff);
  } else {
    IsolateShim::GetCurrent()->ResetIsIdleGcScheduled();
  }
}

void PrepareIdleGC(uv_prepare_t* prepareHandler) {
  // If there were no scripts executed, return
  if (!IsolateShim::GetCurrent()->IsJsScriptExecuted()) {
    return;
  }

  // If idleGC task already scheduled, return
  if (IsolateShim::GetCurrent()->IsIdleGcScheduled()) {
    return;
  }

  ScheduleIdleGcTask();
}

void ScheduleIdleGcTask(uint64_t timeoutInMilliSeconds) {
  uv_timer_start(IsolateShim::GetCurrent()->idleGc_timer_handle(),
                 IdleGC, timeoutInMilliSeconds, 0);
  IsolateShim::GetCurrent()->SetIsIdleGcScheduled();
}

StringUtf8::StringUtf8() : _str(nullptr), _length(0) {
}

StringUtf8::~StringUtf8() {
  if (_str != nullptr) {
    free(_str);
    _str = nullptr;
    _length = 0;
  }
}

char* StringUtf8::Detach() {
  char* str = _str;
  _str = nullptr;
  _length = 0;
  return str;
}

JsErrorCode StringUtf8::LengthFrom(JsValueRef strRef) {
  CHAKRA_ASSERT(_length == 0);

  size_t len = 0;
  IfJsErrorRet(JsCopyString(strRef, nullptr, 0, &len));

  _length = len;
  return JsNoError;
}

JsErrorCode StringUtf8::From(JsValueRef strRef) {
  CHAKRA_ASSERT(!_str);

  size_t length = 0;
  IfJsErrorRet(JsCopyString(strRef, nullptr, 0, &length));

  _str = reinterpret_cast<char*>(malloc(length + 1));
  if (_str == nullptr) {
    return JsErrorOutOfMemory;
  }

  JsErrorCode err = JsCopyString(strRef, _str, length, nullptr);
  if (err != JsNoError) {
    free(_str);
    _str = nullptr;
    return err;
  }

  _str[length] = '\0';
  _length = length;
  return JsNoError;
}

}  // namespace jsrt
