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

namespace v8 {

class ObjectTemplateData : public TemplateData {
 public:
  static const ExternalDataTypes ExternalDataType =
      ExternalDataTypes::ObjectTemplateData;

  SetterGetterInterceptor * setterGetterInterceptor;
  FunctionCallback functionCallDelegate;
  Persistent<Value> functionCallDelegateInterceptorData;
  Persistent<FunctionTemplate> constructor;
  int internalFieldCount;

  explicit ObjectTemplateData(Handle<FunctionTemplate> constructor)
      : TemplateData(ExternalDataType),
        setterGetterInterceptor(nullptr),
        functionCallDelegate(nullptr),
        functionCallDelegateInterceptorData(nullptr),
        constructor(*constructor),
        internalFieldCount(0) {
  }

  ~ObjectTemplateData() {
      constructor.Reset();

      if (setterGetterInterceptor != nullptr) {
        setterGetterInterceptor->namedPropertyInterceptorData.Reset();
        setterGetterInterceptor->indexedPropertyInterceptorData.Reset();
        delete setterGetterInterceptor;
      }
      functionCallDelegateInterceptorData.Reset();
  }

  static void CHAKRA_CALLBACK FinalizeCallback(void* data) {
    if (data != nullptr) {
      ObjectTemplateData* objectTemplateData =
        reinterpret_cast<ObjectTemplateData*>(data);
      delete objectTemplateData;
    }
  }

  bool AreInterceptorsRequired() {
    return setterGetterInterceptor != nullptr &&
     (setterGetterInterceptor->namedPropertyDeleter != nullptr ||
      setterGetterInterceptor->namedPropertyEnumerator != nullptr ||
      setterGetterInterceptor->namedPropertyGetter != nullptr ||
      setterGetterInterceptor->namedPropertyQuery != nullptr ||
      setterGetterInterceptor->namedPropertySetter != nullptr ||
      setterGetterInterceptor->namedPropertyDefiner != nullptr ||
      setterGetterInterceptor->namedPropertyDescriptor != nullptr ||
      setterGetterInterceptor->indexedPropertyDeleter != nullptr ||
      setterGetterInterceptor->indexedPropertyEnumerator != nullptr ||
      setterGetterInterceptor->indexedPropertyGetter != nullptr ||
      setterGetterInterceptor->indexedPropertyQuery != nullptr ||
      setterGetterInterceptor->indexedPropertySetter != nullptr ||
      setterGetterInterceptor->indexedPropertyDefiner != nullptr ||
      setterGetterInterceptor->indexedPropertyDescriptor != nullptr);
    /*
    CHAKRA: functionCallDelegate is intentionaly not added as interceptors because it can be invoked
    through Object::CallAsFunction or Object::CallAsConstructor
    */
  }

  virtual JsValueRef NewInstance(JsValueRef templateRef) {
#ifdef DEBUG
    ObjectTemplateData* data;
    CHAKRA_ASSERT(ExternalData::TryGet(templateRef, &data) && data == this);
#endif
    ObjectTemplate* objectTemplate = static_cast<ObjectTemplate*>(templateRef);
    return *objectTemplate->NewInstance();
  }
};

void ObjectData::FieldValue::SetRef(JsValueRef ref) {
  Reset();

  if (JsAddRef(ref, nullptr) != JsNoError) {
    return;  // fail
  }

  value = reinterpret_cast<void*>(ref);
  isRefValue = true;
  CHAKRA_ASSERT(GetRef() == ref);
}

JsValueRef ObjectData::FieldValue::GetRef() const {
  CHAKRA_ASSERT(IsRef() || !value);
  return reinterpret_cast<JsValueRef>(value);
}

bool ObjectData::FieldValue::IsRef() const {
  return isRefValue;
}

void ObjectData::FieldValue::SetPointer(void* ptr) {
  Reset();
  value = ptr;
  isRefValue = false;
  CHAKRA_ASSERT(GetPointer() == ptr);
}

void* ObjectData::FieldValue::GetPointer() const {
  CHAKRA_ASSERT(!IsRef());
  return value;
}

void ObjectData::FieldValue::Reset() {
  if (IsRef()) {
    JsRelease(GetRef(), nullptr);
  }
  isRefValue = false;
  value = nullptr;
}

ObjectData::ObjectData(ObjectTemplate* objectTemplate,
                       ObjectTemplateData* templateData)
    : ExternalData(ExternalDataType),
      objectTemplate(nullptr, Utils::ToLocal(objectTemplate)),
      internalFieldCount(templateData->internalFieldCount) {
  if (internalFieldCount > 0) {
    internalFields = new FieldValue[internalFieldCount];
  }

  if (templateData->setterGetterInterceptor != nullptr) {
    setterGetterInterceptor = new
        SetterGetterInterceptor(templateData->setterGetterInterceptor);
  } else {
    setterGetterInterceptor = nullptr;
  }
}

ObjectData::~ObjectData() {
  if (internalFieldCount > 0) {
    delete[] internalFields;
  }

  objectTemplate.Reset();
  if (setterGetterInterceptor != nullptr) {
    setterGetterInterceptor->namedPropertyInterceptorData.Reset();
    setterGetterInterceptor->indexedPropertyInterceptorData.Reset();
    delete setterGetterInterceptor;
  }
}

void CHAKRA_CALLBACK ObjectData::FinalizeCallback(void* data) {
  if (data != nullptr) {
    ObjectData* objectData = reinterpret_cast<ObjectData*>(data);
    delete objectData;
  }
}

ObjectData::FieldValue* ObjectData::GetInternalField(Object* object,
                                                     int index) {
  ObjectData* objectData;
  if (Utils::GetObjectData(object, &objectData) != JsNoError ||
      !objectData ||
      index < 0 ||
      index >= objectData->internalFieldCount) {
    return nullptr;
  }

  return &objectData->internalFields[index];
}

// Callbacks used with proxies:
JsValueRef CHAKRA_CALLBACK Utils::GetCallback(
    JsValueRef callee,
    bool isConstructCall,
    JsValueRef* arguments,
    unsigned short argumentCount,  // NOLINT(runtime/int)
    void* callbackState) {
  CHAKRA_VERIFY(argumentCount >= 3);

  JsValueRef result;

  JsValueRef object = arguments[1];
  JsValueRef prop = arguments[2];

  // intercept "__getSelf__" call from Object::InternalFieldHelper
  JsValueType propValueType;
  if (JsGetValueType(prop, &propValueType) != JsNoError) {
    return jsrt::GetUndefined();
  }
  if (propValueType == JsValueType::JsSymbol) {
    JsPropertyIdRef selfSymbolPropertyIdRef =
      jsrt::IsolateShim::GetCurrent()->GetSelfSymbolPropertyIdRef();
    JsPropertyIdRef idRef;
    if (JsGetPropertyIdFromSymbol(prop, &idRef) == JsNoError &&
        selfSymbolPropertyIdRef == idRef) {
      return object;  // return proxy target
    }
  }

  ObjectData* objectData = nullptr;
  if (!ExternalData::TryGet(object, &objectData)) {
    return jsrt::GetUndefined();
  }

  bool isPropIntType = false;
  unsigned int index;
  if (propValueType == JsValueType::JsString &&
      jsrt::TryParseUInt32(prop, &isPropIntType, &index) != JsNoError) {
    return jsrt::GetUndefined();
  }

  if (isPropIntType) {
    if (objectData->setterGetterInterceptor != nullptr &&
        objectData->setterGetterInterceptor->indexedPropertyGetter != nullptr) {
      // indexed array properties were set
      PropertyCallbackInfo<Value> info(
        *(objectData->setterGetterInterceptor->indexedPropertyInterceptorData),
        reinterpret_cast<Object*>(object),
        /*holder*/reinterpret_cast<Object*>(object));
      objectData->setterGetterInterceptor->indexedPropertyGetter(index, info);
      result = reinterpret_cast<JsValueRef>(info.GetReturnValue().Get());
      if (result != JS_INVALID_REFERENCE) {
        return result;
      }
    }

    if (jsrt::GetIndexedProperty(object, index, &result) != JsNoError) {
      return jsrt::GetUndefined();
    }
    return result;
  } else {
    if (objectData->setterGetterInterceptor != nullptr &&
        objectData->setterGetterInterceptor->namedPropertyGetter != nullptr) {
      PropertyCallbackInfo<Value> info(
        *(objectData->setterGetterInterceptor->namedPropertyInterceptorData),
        reinterpret_cast<Object*>(object),
        /*holder*/reinterpret_cast<Object*>(object));
      objectData->setterGetterInterceptor->
        namedPropertyGetter(reinterpret_cast<String*>(prop), info);
      result = reinterpret_cast<JsValueRef>(info.GetReturnValue().Get());
      if (result != JS_INVALID_REFERENCE) {
        return result;
      }
    }

    // use default JS behavior, on the prototype..
    if (jsrt::GetProperty(object, prop, &result) != JsNoError) {
      return jsrt::GetUndefined();
    }
    return result;
  }
}

JsValueRef CHAKRA_CALLBACK Utils::SetCallback(
    JsValueRef callee,
    bool isConstructCall,
    JsValueRef* arguments,
    unsigned short argumentCount,  // NOLINT(runtime/int)
    void* callbackState) {
  CHAKRA_VERIFY(argumentCount >= 4);

  JsValueRef object = arguments[1];
  JsValueRef prop = arguments[2];
  JsValueRef value = arguments[3];

  ObjectData* objectData = nullptr;
  if (!ExternalData::TryGet(object, &objectData)) {
    return jsrt::GetFalse();
  }

  bool isPropIntType;
  unsigned int index;
  if (jsrt::TryParseUInt32(prop, &isPropIntType, &index) != JsNoError) {
    return jsrt::GetFalse();
  }

  Local<Value> setCallbackResult;
  if (isPropIntType) {
    if (objectData->setterGetterInterceptor != nullptr &&
        objectData->setterGetterInterceptor->indexedPropertySetter != nullptr) {
      PropertyCallbackInfo<Value> info(
        *(objectData->setterGetterInterceptor->indexedPropertyInterceptorData),
        reinterpret_cast<Object*>(object),
        /*holder*/reinterpret_cast<Object*>(object));
      objectData->setterGetterInterceptor->indexedPropertySetter(
        index, reinterpret_cast<Value*>(value), info);
      if (info.GetReturnValue().Get() != JS_INVALID_REFERENCE) {
        return jsrt::GetTrue();  // intercepted
      }
    }
    // use default JS behavior
    if (jsrt::SetIndexedProperty(object, index, value) != JsNoError) {
      return jsrt::GetFalse();
    }
  } else {
    if (objectData->setterGetterInterceptor != nullptr &&
        objectData->setterGetterInterceptor->namedPropertySetter != nullptr) {
      PropertyCallbackInfo<Value> info(
        *(objectData->setterGetterInterceptor->namedPropertyInterceptorData),
        reinterpret_cast<Object*>(object),
        /*holder*/reinterpret_cast<Object*>(object));
      objectData->setterGetterInterceptor->namedPropertySetter(
        reinterpret_cast<String*>(prop), reinterpret_cast<Value*>(value), info);
      if (info.GetReturnValue().Get() != JS_INVALID_REFERENCE) {
        return jsrt::GetTrue();  // intercepted
      }
    }
    // use default JS behavior
    if (jsrt::SetProperty(object, prop, value) != JsNoError) {
      return jsrt::GetFalse();
    }
  }

  return jsrt::GetTrue();
}

JsValueRef CHAKRA_CALLBACK Utils::DeletePropertyCallback(
    JsValueRef callee,
    bool isConstructCall,
    JsValueRef* arguments,
    unsigned short argumentCount,  // NOLINT(runtime/int)
    void* callbackState) {
  CHAKRA_VERIFY(argumentCount >= 3);

  JsValueRef object = arguments[1];
  JsValueRef prop = arguments[2];
  JsValueRef result;

  ObjectData* objectData = nullptr;
  if (!ExternalData::TryGet(object, &objectData)) {
    return jsrt::GetFalse();
  }

  bool isPropIntType;
  unsigned int index;
  if (jsrt::TryParseUInt32(prop, &isPropIntType, &index) != JsNoError) {
    return jsrt::GetFalse();
  }

  Local<Value> deleteCallbackResult;
  if (isPropIntType) {
    if (objectData->setterGetterInterceptor != nullptr &&
        objectData->setterGetterInterceptor->
                    indexedPropertyDeleter != nullptr) {
      PropertyCallbackInfo<Boolean> info(
        *(objectData->setterGetterInterceptor->indexedPropertyInterceptorData),
        reinterpret_cast<Object*>(object),
        /*holder*/reinterpret_cast<Object*>(object));
      objectData->setterGetterInterceptor->indexedPropertyDeleter(index, info);
      result = info.GetReturnValue().Get();
      if (result != JS_INVALID_REFERENCE) {
        return result;  // intercepted
      }
    }
    // use default JS behavior
    if (jsrt::DeleteIndexedProperty(object, index) != JsNoError) {
      return jsrt::GetFalse();
    }
    return jsrt::GetTrue();  // no result from JsDeleteIndexedProperty
  } else {
    if (objectData->setterGetterInterceptor != nullptr &&
        objectData->setterGetterInterceptor->namedPropertyDeleter != nullptr) {
      PropertyCallbackInfo<Boolean> info(
        *(objectData->setterGetterInterceptor->namedPropertyInterceptorData),
        reinterpret_cast<Object*>(object),
        /*holder*/reinterpret_cast<Object*>(object));
      objectData->setterGetterInterceptor->
        namedPropertyDeleter(reinterpret_cast<String*>(prop), info);
      result = info.GetReturnValue().Get();
      if (result != JS_INVALID_REFERENCE) {
        return result;  // intercepted
      }
    }
    // use default JS behavior
    if (jsrt::DeleteProperty(object, prop, &result) != JsNoError) {
      return jsrt::GetFalse();
    }
    return result;
  }
}

JsValueRef Utils::HasPropertyHandler(
    JsValueRef* arguments,
    unsigned short argumentCount) {  // NOLINT(runtime/int)
  CHAKRA_VERIFY(argumentCount >= 3);

  JsValueRef object = arguments[1];
  JsValueRef prop = arguments[2];

  ObjectData* objectData = nullptr;
  if (!ExternalData::TryGet(object, &objectData)) {
    return jsrt::GetFalse();
  }

  bool isPropIntType;
  unsigned int index;
  if (jsrt::TryParseUInt32(prop, &isPropIntType, &index) != JsNoError) {
    return jsrt::GetFalse();
  }

  if (isPropIntType) {
    if (objectData->setterGetterInterceptor != nullptr) {
      if (objectData->setterGetterInterceptor->
                      indexedPropertyQuery != nullptr) {
        HandleScope scope(Isolate::GetCurrent());
        PropertyCallbackInfo<Integer> info(
          *(objectData->setterGetterInterceptor->
                        indexedPropertyInterceptorData),
          reinterpret_cast<Object*>(object),
          /*holder*/reinterpret_cast<Object*>(object));
        objectData->setterGetterInterceptor->indexedPropertyQuery(index, info);
        if (info.GetReturnValue().Get() != JS_INVALID_REFERENCE) {
          return jsrt::GetTrue();  // intercepted
        }
      }

      if (objectData->setterGetterInterceptor->
                      indexedPropertyGetter != nullptr) {
        // indexed array properties were set
        PropertyCallbackInfo<Value> info(
          *(objectData->setterGetterInterceptor->
                        indexedPropertyInterceptorData),
          reinterpret_cast<Object*>(object),
          /*holder*/reinterpret_cast<Object*>(object));
        objectData->setterGetterInterceptor->indexedPropertyGetter(index, info);
        if (info.GetReturnValue().Get() != JS_INVALID_REFERENCE) {
          return jsrt::GetTrue();  // intercepted
        }
      }
    }

    // fallback to default JS behavior
    bool hasProperty;
    if (jsrt::HasIndexedProperty(object, index, &hasProperty) != JsNoError) {
      return jsrt::GetFalse();
    }
    return hasProperty ? jsrt::GetTrue() : jsrt::GetFalse();
  } else {  // named property...
    if (objectData->setterGetterInterceptor != nullptr) {
      if (objectData->setterGetterInterceptor->namedPropertyQuery != nullptr) {
        HandleScope scope(Isolate::GetCurrent());
        PropertyCallbackInfo<Integer> info(
          *(objectData->setterGetterInterceptor->namedPropertyInterceptorData),
          reinterpret_cast<Object*>(object),
          /*holder*/reinterpret_cast<Object*>(object));
        objectData->setterGetterInterceptor->
          namedPropertyQuery(reinterpret_cast<String*>(prop), info);
        if (info.GetReturnValue().Get() != JS_INVALID_REFERENCE) {
          return jsrt::GetTrue();  // intercepted
        }
      }

      if (objectData->setterGetterInterceptor->namedPropertyGetter != nullptr) {
        PropertyCallbackInfo<Value> info(
          *(objectData->setterGetterInterceptor->namedPropertyInterceptorData),
          reinterpret_cast<Object*>(object),
          /*holder*/reinterpret_cast<Object*>(object));
        objectData->setterGetterInterceptor->
          namedPropertyGetter(reinterpret_cast<String*>(prop), info);
        if (info.GetReturnValue().Get() != JS_INVALID_REFERENCE) {
          return jsrt::GetTrue();  // intercepted
        }
      }
    }

    // fallback to default JS behavior
    bool hasProperty;
    if (jsrt::HasProperty(object, prop, &hasProperty) != JsNoError) {
      return jsrt::GetFalse();
    }
    return hasProperty ? jsrt::GetTrue() : jsrt::GetFalse();
  }
}

JsValueRef CHAKRA_CALLBACK Utils::HasCallback(
    JsValueRef callee,
    bool isConstructCall,
    JsValueRef* arguments,
    unsigned short argumentCount,  // NOLINT(runtime/int)
    void* callbackState) {
  return HasPropertyHandler(arguments, argumentCount);
}

JsValueRef Utils::GetPropertiesHandler(
    JsValueRef* arguments,
    unsigned int argumentsCount,  // NOLINT(runtime/int)
    bool getFromPrototype) {
  CHAKRA_VERIFY(argumentsCount >= 2);

  HandleScope scope(Isolate::GetCurrent());
  JsValueRef object = arguments[1];

  ObjectData* objectData = nullptr;
  if (!ExternalData::TryGet(object, &objectData)) {
    return jsrt::GetUndefined();
  }

  JsValueRef indexedProperties;
  if (objectData->setterGetterInterceptor != nullptr &&
      objectData->setterGetterInterceptor->
                  indexedPropertyEnumerator != nullptr) {
    PropertyCallbackInfo<Array> info(
      *(objectData->setterGetterInterceptor->indexedPropertyInterceptorData),
      reinterpret_cast<Object*>(object),
      /*holder*/reinterpret_cast<Object*>(object));
    objectData->setterGetterInterceptor->indexedPropertyEnumerator(info);
    indexedProperties =
      reinterpret_cast<JsValueRef>(info.GetReturnValue().Get());
  } else if (getFromPrototype) {  // get indexed properties from object
    if (jsrt::GetEnumerableIndexedProperties(
            object, &indexedProperties) != JsNoError) {
      return jsrt::GetUndefined();
    }
  } else {
    if (jsrt::GetIndexedOwnKeys(object, &indexedProperties) != JsNoError) {
      return jsrt::GetUndefined();
    }
  }

  JsValueRef namedProperties;
  if (objectData->setterGetterInterceptor != nullptr &&
      objectData->setterGetterInterceptor->namedPropertyEnumerator != nullptr) {
    PropertyCallbackInfo<Array> info(
      *(objectData->setterGetterInterceptor->namedPropertyInterceptorData),
      reinterpret_cast<Object*>(object),
      /*holder*/reinterpret_cast<Object*>(object));
    objectData->setterGetterInterceptor->namedPropertyEnumerator(info);
    namedProperties = reinterpret_cast<JsValueRef>(info.GetReturnValue().Get());
  } else if (getFromPrototype) {  // get named properties from object
    if (jsrt::GetEnumerableNamedProperties(object,
                                           &namedProperties) != JsNoError) {
      return jsrt::GetUndefined();
    }
  } else {
    if (jsrt::GetNamedOwnKeys(object, &namedProperties) != JsNoError) {
      return jsrt::GetUndefined();
    }
  }

  JsValueRef concatenatedArray;
  if (jsrt::ConcatArray(indexedProperties, namedProperties,
                        &concatenatedArray) != JsNoError) {
    return jsrt::GetUndefined();
  }

  return concatenatedArray;
}

JsValueRef Utils::GetPropertiesEnumeratorHandler(
    JsValueRef* arguments,
    unsigned int argumentsCount) {  // NOLINT(runtime/int)
  JsValueRef properties =
      GetPropertiesHandler(arguments, argumentsCount, /*getFromPrototype*/true);

  JsValueRef result;
  if (jsrt::CreateEnumerationIterator(properties, &result) != JsNoError) {
    return jsrt::GetUndefined();
  }

  return result;
}

JsValueRef CHAKRA_CALLBACK Utils::EnumerateCallback(
    JsValueRef callee,
    bool isConstructCall,
    JsValueRef* arguments,
    unsigned short argumentCount,  // NOLINT(runtime/int)
    void* callbackState) {
  return GetPropertiesEnumeratorHandler(arguments, argumentCount);
}

JsValueRef CHAKRA_CALLBACK Utils::OwnKeysCallback(
    JsValueRef callee,
    bool isConstructCall,
    JsValueRef* arguments,
    unsigned short argumentCount,  // NOLINT(runtime/int)
    void* callbackState) {
  return GetPropertiesHandler(arguments, argumentCount, false);
}

JsValueRef CHAKRA_CALLBACK Utils::GetOwnPropertyDescriptorCallback(
    JsValueRef callee,
    bool isConstructCall,
    JsValueRef* arguments,
    unsigned short argumentCount,  // NOLINT(runtime/int)
    void* callbackState) {
  CHAKRA_VERIFY(argumentCount >= 3);

  JsValueRef object = arguments[1];
  JsValueRef prop = arguments[2];

  ObjectData* objectData = nullptr;
  if (!ExternalData::TryGet(object, &objectData)) {
    return jsrt::GetUndefined();
  }

  bool isPropIntType;
  unsigned int index;
  if (jsrt::TryParseUInt32(prop, &isPropIntType, &index) != JsNoError) {
    return jsrt::GetUndefined();
  }

  JsValueRef queryResult = JS_INVALID_REFERENCE;
  JsValueRef value = JS_INVALID_REFERENCE;
  JsValueRef descriptor = JS_INVALID_REFERENCE;

  if (isPropIntType) {
    if (objectData->setterGetterInterceptor != nullptr) {
      if (objectData->setterGetterInterceptor->
          indexedPropertyDescriptor != nullptr) {
        PropertyCallbackInfo<Value> info(
            *(objectData->setterGetterInterceptor->
                indexedPropertyInterceptorData),
            reinterpret_cast<Object*>(object),
            /*holder*/reinterpret_cast<Object*>(object));
        objectData->setterGetterInterceptor->indexedPropertyDescriptor(
            index, info);
        descriptor = reinterpret_cast<JsValueRef>(info.GetReturnValue().Get());

        if (descriptor != JS_INVALID_REFERENCE) {
          return descriptor;
        }
      }

      if (objectData->setterGetterInterceptor->
          indexedPropertyQuery != nullptr) {
        HandleScope scope(Isolate::GetCurrent());
        PropertyCallbackInfo<Integer> info(
            *(objectData->setterGetterInterceptor->
                indexedPropertyInterceptorData),
            reinterpret_cast<Object*>(object),
            /*holder*/reinterpret_cast<Object*>(object));
        objectData->setterGetterInterceptor->indexedPropertyQuery(index, info);
        queryResult = reinterpret_cast<JsValueRef>(info.GetReturnValue().Get());
      }

      if (objectData->setterGetterInterceptor->
          indexedPropertyGetter != nullptr) {
        PropertyCallbackInfo<Value> info(
            *(objectData->setterGetterInterceptor->
                indexedPropertyInterceptorData),
            reinterpret_cast<Object*>(object),
            /*holder*/reinterpret_cast<Object*>(object));
        objectData->setterGetterInterceptor->indexedPropertyGetter(index, info);
        value = reinterpret_cast<JsValueRef>(info.GetReturnValue().Get());
      }

      // if queryResult valid, ensure value available
      if (queryResult != JS_INVALID_REFERENCE && value ==
          JS_INVALID_REFERENCE) {
        if (jsrt::GetIndexedProperty(object, index, &value) != JsNoError) {
          return jsrt::GetUndefined();
        }
      }
    }
  } else {  // named property...
    if (objectData->setterGetterInterceptor != nullptr) {
      if (objectData->setterGetterInterceptor->
          namedPropertyDescriptor != nullptr) {
        PropertyCallbackInfo<Value> info(
            *(objectData->setterGetterInterceptor->
                namedPropertyInterceptorData),
            reinterpret_cast<Object*>(object),
            /*holder*/reinterpret_cast<Object*>(object));
        objectData->setterGetterInterceptor->namedPropertyDescriptor(
            reinterpret_cast<String*>(prop), info);
        descriptor = reinterpret_cast<JsValueRef>(info.GetReturnValue().Get());

        if (descriptor != JS_INVALID_REFERENCE) {
          return descriptor;
        }
      }

      // query the property descriptor if there is such, and then get the value
      // from the proxy in order to go through the interceptor
      if (objectData->setterGetterInterceptor->namedPropertyQuery != nullptr) {
        HandleScope scope(Isolate::GetCurrent());
        PropertyCallbackInfo<Integer> info(
            *(objectData->setterGetterInterceptor->
                namedPropertyInterceptorData),
            reinterpret_cast<Object*>(object),
            /*holder*/reinterpret_cast<Object*>(object));
        objectData->setterGetterInterceptor->
            namedPropertyQuery(reinterpret_cast<String*>(prop), info);
        queryResult = reinterpret_cast<JsValueRef>(info.GetReturnValue().Get());
      }

      if (objectData->setterGetterInterceptor->namedPropertyGetter != nullptr) {
        PropertyCallbackInfo<Value> info(
            *(objectData->setterGetterInterceptor->
                namedPropertyInterceptorData),
            reinterpret_cast<Object*>(object),
            /*holder*/reinterpret_cast<Object*>(object));
        objectData->setterGetterInterceptor->
            namedPropertyGetter(reinterpret_cast<String*>(prop), info);
        value = reinterpret_cast<JsValueRef>(info.GetReturnValue().Get());
      }

      // if queryResult valid, ensure value available
      if (queryResult != JS_INVALID_REFERENCE && value ==
          JS_INVALID_REFERENCE) {
        if (jsrt::GetProperty(object, prop, &value) != JsNoError) {
          return jsrt::GetUndefined();
        }
      }
    }
  }

  // if neither is intercepted, fallback to default
  if (queryResult == JS_INVALID_REFERENCE && value == JS_INVALID_REFERENCE) {
    if (jsrt::GetOwnPropertyDescriptor(object, prop,
                                       &descriptor) != JsNoError) {
      return jsrt::GetUndefined();
    }

    return descriptor;
  }

  int queryResultInt = v8::PropertyAttribute::DontEnum;
  if (queryResult != JS_INVALID_REFERENCE) {
    if (jsrt::ValueToIntLikely(queryResult, &queryResultInt) != JsNoError) {
      return jsrt::GetUndefined();
    }
  }

  v8::PropertyAttribute attributes =
      static_cast<v8::PropertyAttribute>(queryResultInt);
  if (jsrt::CreatePropertyDescriptor(attributes, value, JS_INVALID_REFERENCE,
                                     JS_INVALID_REFERENCE,
                                     &descriptor) != JsNoError) {
    return jsrt::GetUndefined();
  }

  return descriptor;
}

JsValueRef CHAKRA_CALLBACK Utils::DefinePropertyCallback(
  JsValueRef callee,
  bool isConstructCall,
  JsValueRef* arguments,
  unsigned short argumentCount,  // NOLINT(runtime/int)
  void* callbackState) {
  CHAKRA_VERIFY(argumentCount >= 4);

  JsValueRef object = arguments[1];
  JsValueRef prop = arguments[2];
  JsValueRef descriptor = arguments[3];

  PropertyDescriptor v8desc;
  jsrt::CreateV8PropertyDescriptor(descriptor, &v8desc);

  ObjectData* objectData = nullptr;
  if (!ExternalData::TryGet(object, &objectData)) {
    return jsrt::GetUndefined();
  }

  bool isPropIntType;
  unsigned int index;
  if (jsrt::TryParseUInt32(prop, &isPropIntType, &index) != JsNoError) {
    return jsrt::GetUndefined();
  }

  JsValueRef result = JS_INVALID_REFERENCE;

  if (objectData->setterGetterInterceptor != nullptr) {
    if (isPropIntType) {
      if (objectData->setterGetterInterceptor->
          indexedPropertyDefiner != nullptr) {
        PropertyCallbackInfo<Value> info(
          *(objectData->setterGetterInterceptor->
                        indexedPropertyInterceptorData),
          reinterpret_cast<Object*>(object),
          /*holder*/reinterpret_cast<Object*>(object));
        objectData->setterGetterInterceptor->
          indexedPropertyDefiner(index, v8desc, info);
        result = reinterpret_cast<JsValueRef>(info.GetReturnValue().Get());
      }
    } else {
      if (objectData->setterGetterInterceptor->
                      namedPropertyDefiner != nullptr) {
        PropertyCallbackInfo<Value> info(
          *(objectData->setterGetterInterceptor->namedPropertyInterceptorData),
          reinterpret_cast<Object*>(object),
          /*holder*/reinterpret_cast<Object*>(object));
        objectData->setterGetterInterceptor->namedPropertyDefiner(
          reinterpret_cast<String*>(prop), v8desc, info);
        result = reinterpret_cast<JsValueRef>(info.GetReturnValue().Get());
      }
    }
  }

  if (result == JS_INVALID_REFERENCE) {
    // No interception took place; fall back to default behavior
    JsPropertyIdRef propertyIdRef;
    if (jsrt::GetPropertyIdFromName(prop, &propertyIdRef) != JsNoError) {
      return jsrt::GetFalse();
    }

    bool result = false;
    if (JsDefineProperty(object, propertyIdRef, descriptor, &result)
        != JsNoError || !result) {
      return jsrt::GetFalse();
    }
  }

  return jsrt::GetTrue();
}

Local<ObjectTemplate> ObjectTemplate::New(Isolate* isolate,
                                          Local<FunctionTemplate> constructor) {
  JsValueRef objectTemplateRef;
  ObjectTemplateData* templateData = new ObjectTemplateData(constructor);

  if (JsCreateExternalObject(templateData,
                             ObjectTemplateData::FinalizeCallback,
                             &objectTemplateRef) != JsNoError) {
    delete templateData;
    return Local<ObjectTemplate>();
  }

  return Local<ObjectTemplate>::New(objectTemplateRef);
}

JsValueRef CHAKRA_CALLBACK GetSelf(
    JsValueRef callee,
    bool isConstructCall,
    JsValueRef* arguments,
    unsigned short argumentCount,  // NOLINT(runtime/int)
    void* callbackState) {
  return reinterpret_cast<JsValueRef>(callbackState);
}

MaybeLocal<Object> ObjectTemplate::NewInstance(Local<Context> context) {
  return NewInstance(Local<Function>());
}

Local<Object> ObjectTemplate::NewInstance() {
  return FromMaybe(NewInstance(Local<Context>()));
}

Local<Object> ObjectTemplate::NewInstance(Handle<Function> constructor) {
  ObjectTemplateData* objectTemplateData = nullptr;
  if (!ExternalData::TryGet(this, &objectTemplateData)) {
    return Local<Object>();
  }

  if (constructor.IsEmpty()) {
    if (!objectTemplateData->constructor.IsEmpty()) {
      constructor = objectTemplateData->constructor->GetFunction();
    }
  }

  JsValueRef prototype = nullptr;

  if (!constructor.IsEmpty()) {
    jsrt::IsolateShim* iso = jsrt::IsolateShim::GetCurrent();

    if (JsGetProperty(*constructor,
                      iso->GetCachedPropertyIdRef(
                          jsrt::CachedPropertyIdRef::prototype),
                      &prototype) != JsNoError) {
      prototype = nullptr;
    }
  }

  ObjectData* objectData = new ObjectData(this, objectTemplateData);
  JsValueRef newInstanceRef = JS_INVALID_REFERENCE;
  if (JsCreateExternalObjectWithPrototype(objectData,
                                          ObjectData::FinalizeCallback,
                                          prototype,
                                          &newInstanceRef) != JsNoError) {
    delete objectData;
    return Local<Object>();
  }

  // In case the object should support index or named properties interceptors,
  // we will use Proxies We will also support in case there is an overrdien
  // toString method on the intercepted object (like Buffer), by returning an
  // object which has the proxy as it prorotype - This is needed due to the fact
  // that proxy implements comparison to other by reference in case the proxy is
  // in the left side of the comparison, and does not call a toString method
  // (like different objects) when compared to string For Node, such comparision
  // is used for Buffers.

  if (objectTemplateData->AreInterceptorsRequired()) {
    JsValueRef newInstanceTargetRef = newInstanceRef;

    JsNativeFunction proxyConf[jsrt::ProxyTraps::TrapCount] = {};

    SetterGetterInterceptor * sgi = objectTemplateData->setterGetterInterceptor;
    CHAKRA_ASSERT(sgi != nullptr);

    if (sgi->indexedPropertyGetter != nullptr ||
        sgi->namedPropertyGetter != nullptr)
      proxyConf[jsrt::ProxyTraps::GetTrap] = Utils::GetCallback;

    if (sgi->indexedPropertySetter != nullptr ||
        sgi->namedPropertySetter != nullptr)
      proxyConf[jsrt::ProxyTraps::SetTrap] = Utils::SetCallback;

    if (sgi->indexedPropertyDeleter != nullptr ||
        sgi->namedPropertyDeleter != nullptr)
      proxyConf[jsrt::ProxyTraps::DeletePropertyTrap] =
          Utils::DeletePropertyCallback;

    if (sgi->indexedPropertyEnumerator != nullptr ||
        sgi->namedPropertyEnumerator != nullptr) {
      proxyConf[jsrt::ProxyTraps::EnumerateTrap] = Utils::EnumerateCallback;
      proxyConf[jsrt::ProxyTraps::OwnKeysTrap] = Utils::OwnKeysCallback;
    }

    if (sgi->indexedPropertyQuery != nullptr ||
        sgi->namedPropertyQuery != nullptr ||
        sgi->indexedPropertyGetter != nullptr ||
        sgi->namedPropertyGetter != nullptr) {
      proxyConf[jsrt::ProxyTraps::HasTrap] = Utils::HasCallback;
    }

    if (sgi->indexedPropertyQuery != nullptr ||
        sgi->namedPropertyQuery != nullptr ||
        sgi->indexedPropertyGetter != nullptr ||
        sgi->namedPropertyGetter != nullptr ||
        sgi->indexedPropertyDescriptor != nullptr ||
        sgi->namedPropertyDescriptor != nullptr) {
      proxyConf[jsrt::ProxyTraps::GetOwnPropertyDescriptorTrap] =
          Utils::GetOwnPropertyDescriptorCallback;
    }

    if (sgi->indexedPropertyDefiner != nullptr ||
        sgi->namedPropertyDefiner != nullptr) {
      proxyConf[jsrt::ProxyTraps::DefinePropertyTrap] =
          Utils::DefinePropertyCallback;
    }

    JsErrorCode error =
        jsrt::CreateProxy(newInstanceTargetRef, proxyConf, &newInstanceRef);

    if (error != JsNoError) {
      return Local<Object>();
    }
  }

  // clone the object template into the new instance
  if (objectTemplateData->CopyPropertiesTo(newInstanceRef) != JsNoError) {
    return Local<Object>();
  }

  return Local<Object>::New(static_cast<Object*>(newInstanceRef));
}

void ObjectTemplate::SetAccessor(Handle<String> name,
                                 AccessorGetterCallback getter,
                                 AccessorSetterCallback setter,
                                 Handle<Value> data,
                                 AccessControl settings,
                                 PropertyAttribute attribute,
                                 Handle<AccessorSignature> signature) {
  return SetAccessor(name,
                     reinterpret_cast<AccessorNameGetterCallback>(getter),
                     reinterpret_cast<AccessorNameSetterCallback>(setter),
                     data, settings, attribute, signature);
}

void ObjectTemplate::SetAccessor(Handle<Name> name,
                                 AccessorNameGetterCallback getter,
                                 AccessorNameSetterCallback setter,
                                 Handle<Value> data,
                                 AccessControl settings,
                                 PropertyAttribute attribute,
                                 Handle<AccessorSignature> signature) {
  ObjectTemplateData* objectTemplateData = nullptr;
  if (!ExternalData::TryGet(this, &objectTemplateData)) {
    return;
  }

  Object* properties = objectTemplateData->EnsureProperties();
  if (properties != nullptr) {
    properties->SetAccessor(
      name, getter, setter, data, settings, attribute, signature);
  }
}

void ObjectTemplate::SetNamedPropertyHandler(
    NamedPropertyGetterCallback getter,
    NamedPropertySetterCallback setter,
    NamedPropertyQueryCallback query,
    NamedPropertyDeleterCallback remover,
    NamedPropertyEnumeratorCallback enumerator,
    NamedPropertyDefinerCallback definer,
    NamedPropertyDescriptorCallback descriptor,
    Handle<Value> data) {
  ObjectTemplateData* objectTemplateData = nullptr;
  if (!ExternalData::TryGet(this, &objectTemplateData)) {
    return;
  }

  SetterGetterInterceptor * sgi = objectTemplateData->setterGetterInterceptor;
  if (sgi == nullptr) {
    sgi = new SetterGetterInterceptor();
    objectTemplateData->setterGetterInterceptor = sgi;
  }
  sgi->namedPropertyGetter = getter;
  sgi->namedPropertySetter = setter;
  sgi->namedPropertyQuery = query;
  sgi->namedPropertyDeleter = remover;
  sgi->namedPropertyEnumerator = enumerator;
  sgi->namedPropertyDefiner = definer;
  sgi->namedPropertyDescriptor = descriptor;
  sgi->namedPropertyInterceptorData = data;
}

void ObjectTemplate::SetHandler(
    const NamedPropertyHandlerConfiguration& config) {
  SetNamedPropertyHandler(
    reinterpret_cast<NamedPropertyGetterCallback>(config.getter),
    reinterpret_cast<NamedPropertySetterCallback>(config.setter),
    reinterpret_cast<NamedPropertyQueryCallback>(config.query),
    reinterpret_cast<NamedPropertyDeleterCallback>(config.deleter),
    reinterpret_cast<NamedPropertyEnumeratorCallback>(config.enumerator),
    reinterpret_cast<NamedPropertyDefinerCallback>(config.definer),
    reinterpret_cast<NamedPropertyDescriptorCallback>(config.descriptor),
    config.data);
}

void ObjectTemplate::SetIndexedPropertyHandler(
    IndexedPropertyGetterCallback getter,
    IndexedPropertySetterCallback setter,
    IndexedPropertyQueryCallback query,
    IndexedPropertyDeleterCallback remover,
    IndexedPropertyEnumeratorCallback enumerator,
    IndexedPropertyDefinerCallback definer,
    IndexedPropertyDescriptorCallback descriptor,
    Handle<Value> data) {
  ObjectTemplateData* objectTemplateData = nullptr;
  if (!ExternalData::TryGet(this, &objectTemplateData)) {
    return;
  }

  SetterGetterInterceptor * sgi = objectTemplateData->setterGetterInterceptor;
  if (sgi == nullptr) {
    sgi = new SetterGetterInterceptor();
    objectTemplateData->setterGetterInterceptor = sgi;
  }
  sgi->indexedPropertyGetter = getter;
  sgi->indexedPropertySetter = setter;
  sgi->indexedPropertyQuery = query;
  sgi->indexedPropertyDeleter = remover;
  sgi->indexedPropertyEnumerator = enumerator;
  sgi->indexedPropertyDefiner = definer;
  sgi->indexedPropertyDescriptor = descriptor;
  sgi->indexedPropertyInterceptorData = data;
}

void ObjectTemplate::SetHandler(
    const IndexedPropertyHandlerConfiguration& config) {
  SetIndexedPropertyHandler(
    config.getter,
    config.setter,
    config.query,
    config.deleter,
    config.enumerator,
    config.definer,
    config.descriptor,
    config.data);
}

void ObjectTemplate::SetAccessCheckCallbacks(
    NamedSecurityCallback named_callback,
    IndexedSecurityCallback indexed_callback,
    Handle<Value> data,
    bool turned_on_by_default) {
  // CHAKRA-TODO
}

void ObjectTemplate::SetCallAsFunctionHandler(FunctionCallback callback,
                                              Handle<Value> data) {
  ObjectTemplateData* objectTemplateData = nullptr;
  if (!ExternalData::TryGet(this, &objectTemplateData)) {
    CHAKRA_ASSERT(false);  // This should never happen
    return;
  }

  objectTemplateData->functionCallDelegate = callback;
  objectTemplateData->functionCallDelegateInterceptorData = data;
}

void ObjectTemplate::SetInternalFieldCount(int value) {
  ObjectTemplateData* objectTemplateData = nullptr;
  if (!ExternalData::TryGet(this, &objectTemplateData)) {
    CHAKRA_ASSERT(false);  // This should never happen
    return;
  }

  objectTemplateData->internalFieldCount = value;
}

void ObjectTemplate::SetConstructor(Handle<FunctionTemplate> constructor) {
  ObjectTemplateData* objectTemplateData = nullptr;
  if (!ExternalData::TryGet(this, &objectTemplateData)) {
    CHAKRA_ASSERT(false);  // This should never happen
    return;
  }

  objectTemplateData->constructor = constructor;
}

}  // namespace v8
