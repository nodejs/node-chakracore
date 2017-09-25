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

#ifndef DEPS_CHAKRASHIM_SRC_V8CHAKRA_H_
#define DEPS_CHAKRASHIM_SRC_V8CHAKRA_H_

#include "v8.h"
#include "jsrtutils.h"

namespace v8 {

using jsrt::ContextShim;
class ObjectTemplateData;

// External object data types
enum class ExternalDataTypes {
  Unknown,
  ObjectTemplateData,
  ObjectData,
  FunctionTemplateData,
  FunctionCallbackData,
};

// Base class for external object data
class ExternalData {
 private:
  const ExternalDataTypes type;

 public:
  explicit ExternalData(ExternalDataTypes type) : type(type) {}
  virtual ~ExternalData() {}

  ExternalDataTypes GetType() const { return type; }

  template <class T>
  static bool Is(T* data) {
    return data->GetType() == T::ExternalDataType;
  }

  static JsErrorCode GetExternalData(JsValueRef ref, ExternalData** data) {
    return JsGetExternalData(ref, reinterpret_cast<void**>(data));
  }

  template <class T>
  static JsErrorCode GetExternalData(JsValueRef ref, T** data) {
    ExternalData* p = nullptr;
    JsErrorCode error = GetExternalData(ref, &p);
    *data = (error == JsNoError
            && p != nullptr
            && p->GetType() == T::ExternalDataType) ?
              static_cast<T*>(p) : nullptr;

    CHAKRA_ASSERT(*data == nullptr || (error == JsNoError && Is(*data)));
    return error;
  }

  template <class T>
  static bool TryGet(JsValueRef ref, T** data) {
    return GetExternalData(ref, data) == JsNoError && *data != nullptr;
  }
};

class ObjectData: public ExternalData {
 public:
  static const ExternalDataTypes ExternalDataType =
    ExternalDataTypes::ObjectData;

  struct FieldValue {
   public:
    FieldValue() : value(nullptr), isRefValue(false) {}
    ~FieldValue() { Reset(); }

    void SetRef(JsValueRef ref);
    JsValueRef GetRef() const;
    bool IsRef() const;

    void SetPointer(void* ptr);
    void* GetPointer() const;

   private:
    void Reset();

    void* value;
    bool isRefValue;
  };

  JsValueRef objectInstance;
  Persistent<ObjectTemplate> objectTemplate;  // Original ObjectTemplate
  NamedPropertyGetterCallback namedPropertyGetter;
  NamedPropertySetterCallback namedPropertySetter;
  NamedPropertyQueryCallback namedPropertyQuery;
  NamedPropertyDeleterCallback namedPropertyDeleter;
  NamedPropertyEnumeratorCallback namedPropertyEnumerator;
  Persistent<Value> namedPropertyInterceptorData;
  IndexedPropertyGetterCallback indexedPropertyGetter;
  IndexedPropertySetterCallback indexedPropertySetter;
  IndexedPropertyQueryCallback indexedPropertyQuery;
  IndexedPropertyDeleterCallback indexedPropertyDeleter;
  IndexedPropertyEnumeratorCallback indexedPropertyEnumerator;
  Persistent<Value> indexedPropertyInterceptorData;
  int internalFieldCount;
  FieldValue* internalFields;

  ObjectData(ObjectTemplate* objectTemplate, ObjectTemplateData *templateData);
  ~ObjectData();
  static void CHAKRA_CALLBACK FinalizeCallback(void *data);

  static FieldValue* GetInternalField(Object* object, int index);
};

class TemplateData : public ExternalData {
 private:
  Persistent<Object> properties;

 public:
  explicit TemplateData(ExternalDataTypes type) : ExternalData(type) {}
  virtual ~TemplateData() {
    properties.Reset();
  }

  static bool Is(ExternalData* data);
  virtual JsValueRef NewInstance(JsValueRef templateRef) = 0;

  Object* EnsureProperties();
  JsErrorCode CopyPropertiesTo(JsValueRef newInstance);
};

class Utils {
 public:
  static JsValueRef CHAKRA_CALLBACK AccessorHandler(
      JsValueRef callee,
      bool isConstructCall,
      JsValueRef *arguments,
      unsigned short argumentCount,  // NOLINT(runtime/int)
      void *callbackState);

  static JsValueRef CHAKRA_CALLBACK GetCallback(
      JsValueRef callee,
      bool isConstructCall,
      JsValueRef *arguments,
      unsigned short argumentCount,  // NOLINT(runtime/int)
      void *callbackState);
  static JsValueRef CHAKRA_CALLBACK SetCallback(
      JsValueRef callee,
      bool isConstructCall,
      JsValueRef *arguments,
      unsigned short argumentCount,  // NOLINT(runtime/int)
      void *callbackState);
  static JsValueRef CHAKRA_CALLBACK DeletePropertyCallback(
      JsValueRef callee,
      bool isConstructCall,
      JsValueRef *arguments,
      unsigned short argumentCount,  // NOLINT(runtime/int)
      void *callbackState);

  static JsValueRef HasPropertyHandler(
      JsValueRef *arguments,
      unsigned short argumentCount);  // NOLINT(runtime/int)
  static JsValueRef CHAKRA_CALLBACK HasCallback(
      JsValueRef callee,
      bool isConstructCall,
      JsValueRef *arguments,
      unsigned short argumentCount,  // NOLINT(runtime/int)
      void *callbackState);

  static JsValueRef GetPropertiesEnumeratorHandler(
      JsValueRef* arguments,
      unsigned int argumentsCount);
  static JsValueRef CHAKRA_CALLBACK EnumerateCallback(
      JsValueRef callee,
      bool isConstructCall,
      JsValueRef *arguments,
      unsigned short argumentCount,  // NOLINT(runtime/int)
      void *callbackState);

  static JsValueRef GetPropertiesHandler(
      JsValueRef* arguments,
      unsigned int argumentsCount,
      bool getFromPrototype);
  static JsValueRef CHAKRA_CALLBACK OwnKeysCallback(
      JsValueRef callee,
      bool isConstructCall,
      JsValueRef *arguments,
      unsigned short argumentCount,  // NOLINT(runtime/int)
      void *callbackState);
  static JsValueRef CHAKRA_CALLBACK GetOwnPropertyDescriptorCallback(
      JsValueRef callee,
      bool isConstructCall,
      JsValueRef *arguments,
      unsigned short argumentCount,  // NOLINT(runtime/int)
      void *callbackState);

  static void CHAKRA_CALLBACK WeakReferenceCallbackWrapperCallback(
      JsRef ref, void *data);

  // Create a Local<T> internally (use private constructor)
  template <class T>
  static Local<T> ToLocal(T* that) {
    return Local<T>(that);
  }

  static JsErrorCode GetObjectData(Object* object, ObjectData** objectData);

  static bool IsInstanceOf(Object* obj, ObjectTemplate* objectTemplate) {
    return obj->GetObjectTemplate() == objectTemplate;
  }

  static bool CheckSignature(Local<FunctionTemplate> receiver,
                             Local<Object> thisPointer,
                             Local<Object>* holder);

  template <class Func>
  static Local<Value> NewError(Handle<String> message, const Func& f);

  static JsErrorCode NewTypedArray(ContextShim::GlobalType constructorIndex,
                                   Handle<ArrayBuffer> array_buffer,
                                   size_t byte_offset, size_t length,
                                   JsValueRef* result);
  template <class T>
  static Local<T> NewTypedArray(ContextShim::GlobalType constructorIndex,
                                Handle<ArrayBuffer> array_buffer,
                                size_t byte_offset, size_t length);

  static ObjectTemplate* EnsureObjectTemplate(
      Persistent<ObjectTemplate>* objectTemplate) {
    if (objectTemplate->IsEmpty()) {
      // Create ObjectTemplate lazily
      *objectTemplate = ObjectTemplate::New(nullptr);
    }
    return **objectTemplate;
  }

  static MaybeLocal<String> NewString(const char *data, int length = -1);
};


template <class T>
T FromMaybe(Maybe<T> maybe, const T& def = T()) {
  return maybe.FromMaybe(def);
}

template <class T>
Local<T> FromMaybe(MaybeLocal<T> maybe, const Local<T>& def = Local<T>()) {
  return maybe.FromMaybe(def);
}

}  // namespace v8

#endif  // DEPS_CHAKRASHIM_SRC_V8CHAKRA_H_
