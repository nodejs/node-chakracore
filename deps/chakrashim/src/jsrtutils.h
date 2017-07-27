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

#ifndef DEPS_CHAKRASHIM_SRC_JSRTUTILS_H_
#define DEPS_CHAKRASHIM_SRC_JSRTUTILS_H_

#include <assert.h>
#include <functional>
#include <stdint.h>
#include <string.h>
#include "v8.h"
#include "uv.h"

#ifndef _countof
template <typename T, size_t N>
inline size_t _countof(T (&)[N]) {
  return N;
}
#endif

#ifndef BYTE
#define BYTE unsigned char
#endif

#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif

#ifndef _WIN32
#define strnicmp strncasecmp
#define __debugbreak __builtin_trap
#endif

#ifndef __APPLE__
  #if defined(_MSC_VER) && _MSC_VER <= 1800  // VS2013?
    #define THREAD_LOCAL __declspec(thread)
  #else  // VS2015+, linux Clang etc.
    #define THREAD_LOCAL thread_local
  #endif  // VS2013?
#else  // __APPLE__
  #define THREAD_LOCAL _Thread_local
#endif

#include "jsrtproxyutils.h"
#include "jsrtcontextshim.h"
#include "jsrtisolateshim.h"

#define IfComFailError(v) \
  { \
    hr = (v) ; \
    if (FAILED(hr)) { \
      goto error; \
    } \
  }

#define IfJsErrorRet(expr, ...) \
  { \
    JsErrorCode _error = (expr); \
    if (_error != JsNoError) { \
      return _error, ##__VA_ARGS__; \
    } \
  }

#define CHAKRA_UNIMPLEMENTED() jsrt::Unimplemented(__FUNCTION__)
#define CHAKRA_UNIMPLEMENTED_(message) jsrt::Unimplemented(message)

#define CHAKRA_VERIFY(expr) if (!(expr)) { \
  jsrt::Fatal("internal error %s(%d): %s", __FILE__, __LINE__, #expr); }

#define CHAKRA_VERIFY_NOERROR(errorCode) if (errorCode != JsNoError) { \
  jsrt::Fatal("internal error %s(%d): %x", __FILE__, __LINE__, errorCode); }

#ifdef DEBUG
#define CHAKRA_ASSERT(expr) assert(expr)
#else
#define CHAKRA_ASSERT(expr)
#endif

namespace jsrt {

// Similar to v8::String::Utf8Value but only operates on JS String.
class StringUtf8 {
 public:
  StringUtf8();
  ~StringUtf8();
  char *operator*() { return _str; }
  operator const char *() const { return _str; }
  int length() const { return static_cast<int>(_length); }
  JsErrorCode From(JsValueRef strRef);
  // This just initializes length field. _str will remain uninitialized.
  // Use `From()` to initialize _str and _length
  JsErrorCode LengthFrom(JsValueRef strRef);
  // Detach the underlying `_str`. The caller is responsible to free this
  // buffer
  char* Detach();

 private:
  // Disallow copying and assigning
  StringUtf8(const StringUtf8&);
  void operator=(const StringUtf8&);

  char* _str;
  int _length;
};

JsErrorCode InitializePromise();

JsErrorCode UintToValue(uint32_t value, JsValueRef* result);

JsErrorCode GetProperty(JsValueRef ref,
                        JsValueRef propName,
                        JsValueRef *result);

JsErrorCode GetProperty(JsValueRef ref,
                        const char *propertyName,
                        JsValueRef *result);

JsErrorCode GetProperty(JsValueRef ref,
                        CachedPropertyIdRef cachedIdRef,
                        JsValueRef *result);

JsErrorCode GetProperty(JsValueRef ref,
                        JsPropertyIdRef propId,
                        bool *boolValue);

JsErrorCode GetProperty(JsValueRef ref,
                        CachedPropertyIdRef cachedIdRef,
                        bool *boolValue);

JsErrorCode GetProperty(JsValueRef ref,
                        JsPropertyIdRef propId,
                        int *intValue);

JsErrorCode GetProperty(JsValueRef ref,
                        CachedPropertyIdRef cachedIdRef,
                        int *intValue);

JsErrorCode SetProperty(JsValueRef ref,
                        JsValueRef propName,
                        JsValueRef propValue);

JsErrorCode SetProperty(JsValueRef ref,
                        CachedPropertyIdRef cachedIdRef,
                        JsValueRef propValue);

JsErrorCode SetProperty(JsValueRef ref,
                        const char *propertyName,
                        JsValueRef propValue);

JsErrorCode SetProperty(JsValueRef ref,
                        JsPropertyIdRef propId,
                        bool boolValue);

JsErrorCode SetProperty(JsValueRef ref,
                        CachedPropertyIdRef cachedIdRef,
                        bool boolValue);

JsErrorCode SetProperty(JsValueRef ref,
                        JsPropertyIdRef propId,
                        const char *stringValue);

JsErrorCode SetProperty(JsValueRef ref,
                        CachedPropertyIdRef cachedIdRef,
                        const char *stringValue);

JsErrorCode DeleteIndexedProperty(JsValueRef object,
                                  unsigned int index);

JsErrorCode DeleteProperty(JsValueRef ref,
                           JsValueRef propName,
                           JsValueRef* result);

JsErrorCode GetOwnPropertyDescriptor(JsValueRef ref,
                                     JsValueRef prop,
                                     JsValueRef* result);

JsErrorCode IsZero(JsValueRef value,
                   bool *result);

JsErrorCode IsUndefined(JsValueRef value,
                        bool *result);

JsErrorCode HasOwnProperty(JsValueRef object,
                           JsValueRef prop,
                           JsValueRef *result);

JsErrorCode HasProperty(JsValueRef object,
                        JsValueRef prop,
                        bool *result);

JsErrorCode HasProperty(JsValueRef object,
                        CachedPropertyIdRef cachedIdRef,
                        bool *result);

JsErrorCode HasIndexedProperty(JsValueRef object,
                               unsigned int index,
                               bool *result);

JsErrorCode GetEnumerableNamedProperties(JsValueRef object,
                                         JsValueRef *result);

JsErrorCode GetEnumerableIndexedProperties(JsValueRef object,
                                           JsValueRef *result);

JsErrorCode GetIndexedOwnKeys(JsValueRef object,
                              JsValueRef *result);

JsErrorCode GetNamedOwnKeys(JsValueRef object,
                            JsValueRef *result);

JsErrorCode CreateEnumerationIterator(JsValueRef enumeration,
                                      JsValueRef *result);

JsErrorCode CreatePropertyDescriptorsEnumerationIterator(JsValueRef enumeration,
                                                         JsValueRef *result);

JsErrorCode ConcatArray(JsValueRef first,
                        JsValueRef second,
                        JsValueRef *result);

JsErrorCode PushArray(JsValueRef array,
                      JsValueRef item,
                      JsValueRef *result);

JsErrorCode CallProperty(JsValueRef ref,
                         CachedPropertyIdRef cachedIdRef,
                         JsValueRef *arguments,
                         unsigned short argumentCount,  // NOLINT(runtime/int)
                         JsValueRef *result);

JsErrorCode CallGetter(JsValueRef ref,
                       CachedPropertyIdRef cachedIdRef,
                       JsValueRef* result);

JsErrorCode CallGetter(JsValueRef ref,
                       CachedPropertyIdRef cachedIdRef,
                       int* result);

JsErrorCode GetPropertyOfGlobal(const char *propertyName,
                                JsValueRef *ref);

JsErrorCode SetPropertyOfGlobal(const char *propertyName,
                                JsValueRef ref);

JsValueRef GetNull();

JsValueRef GetUndefined();

JsValueRef GetTrue();

JsValueRef GetFalse();

JsErrorCode GetArrayLength(JsValueRef arrayRef,
                           unsigned int *arraySize);

bool InstanceOf(JsValueRef first,
                JsValueRef second);

JsErrorCode CloneObject(JsValueRef source,
                        JsValueRef target,
                        bool cloneProtoype = false);

JsErrorCode GetPropertyNames(JsValueRef object,
                             JsValueRef *namesArray);

JsErrorCode AddExternalData(JsValueRef ref,
                            JsPropertyIdRef externalDataPropertyId,
                            void *data,
                            JsFinalizeCallback onObjectFinalize);

JsErrorCode AddExternalData(JsValueRef ref,
                            void *data,
                            JsFinalizeCallback onObjectFinalize);

JsErrorCode GetExternalData(JsValueRef ref,
                            JsPropertyIdRef idRef,
                            void **data);

JsErrorCode GetExternalData(JsValueRef ref,
                            void **data);

JsErrorCode CreateFunctionWithExternalData(JsNativeFunction,
                                           void* data,
                                           JsFinalizeCallback onObjectFinalize,
                                           JsValueRef *function);

JsErrorCode ToString(JsValueRef ref,
                     JsValueRef* strRef, StringUtf8* stringUtf8);

JsErrorCode CreateString(const char *string,
                         JsValueRef *ref);

JsErrorCode CreatePropertyId(const char *name,
                             JsValueRef *propertyIdRef);

#define DEF_IS_TYPE(F) \
JsErrorCode Call##F(JsValueRef value,  \
JsValueRef *resultRef); \

#include "jsrtcachedpropertyidref.inc"
#undef DEF_IS_TYPE

JsValueRef CHAKRA_CALLBACK CollectGarbage(
    JsValueRef callee,
    bool isConstructCall,
    JsValueRef *arguments,
    unsigned short argumentCount,  // NOLINT(runtime/int)
    void *callbackState);

// the possible values for the property descriptor options
enum PropertyDescriptorOptionValues {
  True,
  False,
  None
};

PropertyDescriptorOptionValues GetPropertyDescriptorOptionValue(bool b);

JsErrorCode CreatePropertyDescriptor(
    PropertyDescriptorOptionValues writable,
    PropertyDescriptorOptionValues enumerable,
    PropertyDescriptorOptionValues configurable,
    JsValueRef value,
    JsValueRef getter,
    JsValueRef setter,
    JsValueRef *descriptor);

JsErrorCode CreatePropertyDescriptor(v8::PropertyAttribute attributes,
                                     JsValueRef value,
                                     JsValueRef getter,
                                     JsValueRef setter,
                                     JsValueRef *descriptor);

JsErrorCode DefineProperty(JsValueRef object,
                           const char * propertyName,
                           PropertyDescriptorOptionValues writable,
                           PropertyDescriptorOptionValues enumerable,
                           PropertyDescriptorOptionValues configurable,
                           JsValueRef value,
                           JsValueRef getter,
                           JsValueRef setter);

JsErrorCode DefineProperty(JsValueRef object,
                           JsPropertyIdRef propertyIdRef,
                           PropertyDescriptorOptionValues writable,
                           PropertyDescriptorOptionValues enumerable,
                           PropertyDescriptorOptionValues configurable,
                           JsValueRef value,
                           JsValueRef getter,
                           JsValueRef setter);

JsErrorCode GetPropertyIdFromName(JsValueRef nameRef,
                                  JsPropertyIdRef *idRef);

JsErrorCode GetPropertyIdFromValue(JsValueRef valueRef,
                                   JsPropertyIdRef *idRef);

JsErrorCode GetObjectConstructor(JsValueRef objectRef,
                                 JsValueRef *constructorRef);

JsErrorCode SetIndexedProperty(JsValueRef object,
                               unsigned int index,
                               JsValueRef value);

JsErrorCode GetIndexedProperty(JsValueRef object,
                               unsigned int index,
                               JsValueRef *value);

// CHAKRA-TODO : Currently Chakra's ParseScript doesn't support strictMode
// flag. As a workaround, prepend the script text with 'use strict'.
JsErrorCode ParseScript(StringUtf8 *script,
                        JsSourceContext sourceContext,
                        JsValueRef sourceUrl,
                        bool isStrictMode,
                        JsValueRef *result);

JsErrorCode GetHiddenValuesTable(JsValueRef object,
                                JsPropertyIdRef* hiddenValueIdRef,
                                JsValueRef* hiddenValuesTable,
                                bool* isUndefined);

JsErrorCode GetPrivate(JsValueRef object, JsValueRef key,
                           JsValueRef *result);

JsErrorCode SetPrivate(JsValueRef object, JsValueRef key,
                           JsValueRef value);

bool HasPrivate(JsValueRef object, JsValueRef key);

bool DeletePrivate(JsValueRef object, JsValueRef key);

void Unimplemented(const char * message);

void Fatal(const char * format, ...);

void ScheduleIdleGcTask(uint64_t timeoutInMilliSeconds = 1000);

void PrepareIdleGC(uv_prepare_t* prepareHandler);

void IdleGC(uv_timer_t *timerHandler);

// Arguments buffer for JsCallFunction
template <int STATIC_COUNT = 4>
class JsArguments {
 private:
  JsValueRef _local[STATIC_COUNT];
  JsValueRef* _args;

 public:
  explicit JsArguments(int count) {
    _args = count <= STATIC_COUNT ? _local : new JsValueRef[count];
  }

  ~JsArguments() {
    if (_args != _local) {
      delete [] _args;
    }
  }

  operator JsValueRef*() {
    return _args;
  }
};


// Helpers for JsCallFunction/JsConstructObject with undefined as arg0

template <class T>
JsErrorCode CallFunction(const T& api,
                         JsValueRef func,
                         JsValueRef* result) {
  JsValueRef args[] = { GetUndefined() };
  return api(func, args, _countof(args), result);
}

template <class T>
JsErrorCode CallFunction(const T& api,
                         JsValueRef func, JsValueRef arg1,
                         JsValueRef* result) {
  JsValueRef args[] = { GetUndefined(), arg1 };
  return api(func, args, _countof(args), result);
}

template <class T>
JsErrorCode CallFunction(const T& api,
                         JsValueRef func, JsValueRef arg1, JsValueRef arg2,
                         JsValueRef* result) {
  JsValueRef args[] = { GetUndefined(), arg1, arg2 };
  return api(func, args, _countof(args), result);
}

template <class T>
JsErrorCode CallFunction(const T& api,
                         JsValueRef func, JsValueRef arg1, JsValueRef arg2,
                         JsValueRef arg3, JsValueRef* result) {
  JsValueRef args[] = { GetUndefined(), arg1, arg2, arg3 };
  return api(func, args, _countof(args), result);
}

inline JsErrorCode CallFunction(JsValueRef func,
                                JsValueRef* result) {
  return CallFunction(JsCallFunction, func, result);
}

inline JsErrorCode CallFunction(JsValueRef func, JsValueRef arg1,
                                JsValueRef* result) {
  return CallFunction(JsCallFunction, func, arg1, result);
}

inline JsErrorCode CallFunction(JsValueRef func,
                                JsValueRef arg1, JsValueRef arg2,
                                JsValueRef* result) {
  return CallFunction(JsCallFunction, func, arg1, arg2, result);
}

inline JsErrorCode CallFunction(JsValueRef func,
                                JsValueRef arg1, JsValueRef arg2,
                                JsValueRef arg3, JsValueRef* result) {
  return CallFunction(JsCallFunction, func, arg1, arg2, arg3, result);
}

inline JsErrorCode ConstructObject(JsValueRef func,
                                   JsValueRef* result) {
  return CallFunction(JsConstructObject, func, result);
}

inline JsErrorCode ConstructObject(JsValueRef func, JsValueRef arg1,
                                   JsValueRef* result) {
  return CallFunction(JsConstructObject, func, arg1, result);
}

inline JsErrorCode ConstructObject(JsValueRef func,
                                   JsValueRef arg1, JsValueRef arg2,
                                   JsValueRef* result) {
  return CallFunction(JsConstructObject, func, arg1, arg2, result);
}


template <bool LIKELY,
          class JsConvertToValueFunc,
          class JsValueToNativeFunc,
          class T>
JsErrorCode ValueToNative(const JsConvertToValueFunc& JsConvertToValue,
                          const JsValueToNativeFunc& JsValueToNative,
                          JsValueRef value, T* nativeValue) {
  JsErrorCode error;

  // If LIKELY, try JsValueToNative first. Likely to succeed.
  if (LIKELY) {
    error = JsValueToNative(value, nativeValue);
    if (error != JsErrorInvalidArgument) {
      return error;
    }
  }

  // Perform JS conversion first, then to native.
  error = JsConvertToValue(value, &value);
  if (error != JsNoError) {
    return error;
  }
  return JsValueToNative(value, nativeValue);
}

inline JsErrorCode ValueToBool(JsValueRef value, bool* boolValue) {
  return ValueToNative</*LIKELY*/false>(
    JsConvertValueToBoolean, JsBooleanToBool, value, boolValue);
}

inline JsErrorCode ValueToBoolLikely(JsValueRef value, bool* boolValue) {
  return ValueToNative</*LIKELY*/true>(
    JsConvertValueToBoolean, JsBooleanToBool, value, boolValue);
}

inline JsErrorCode ValueToInt(JsValueRef value, int* intValue) {
  return ValueToNative</*LIKELY*/false>(
    JsConvertValueToNumber, JsNumberToInt, value, intValue);
}

inline JsErrorCode ValueToIntLikely(JsValueRef value, int* intValue) {
  return ValueToNative</*LIKELY*/true>(
    JsConvertValueToNumber, JsNumberToInt, value, intValue);
}

inline JsErrorCode ValueToDouble(JsValueRef value, double* dblValue) {
  return ValueToNative</*LIKELY*/false>(
    JsConvertValueToNumber, JsNumberToDouble, value, dblValue);
}

inline JsErrorCode ValueToDoubleLikely(JsValueRef value, double* dblValue) {
  return ValueToNative</*LIKELY*/true>(
    JsConvertValueToNumber, JsNumberToDouble, value, dblValue);
}

}  // namespace jsrt

#endif  // DEPS_CHAKRASHIM_SRC_JSRTUTILS_H_
