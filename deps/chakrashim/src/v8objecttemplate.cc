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

  Persistent<String> className;
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
  FunctionCallback functionCallDelegate;
  Persistent<Value> functionCallDelegateInterceptorData;
  int internalFieldCount;

  ObjectTemplateData()
      : TemplateData(ExternalDataType),
        namedPropertyGetter(nullptr),
        namedPropertySetter(nullptr),
        namedPropertyQuery(nullptr),
        namedPropertyDeleter(nullptr),
        namedPropertyEnumerator(nullptr),
        indexedPropertyGetter(nullptr),
        indexedPropertySetter(nullptr),
        indexedPropertyQuery(nullptr),
        indexedPropertyDeleter(nullptr),
        indexedPropertyEnumerator(nullptr),
        functionCallDelegate(nullptr),
        functionCallDelegateInterceptorData(nullptr),
        internalFieldCount(0) {
  }

  ~ObjectTemplateData() {
      className.Reset();
      namedPropertyInterceptorData.Reset();
      indexedPropertyInterceptorData.Reset();
      functionCallDelegateInterceptorData.Reset();
  }

  static void CHAKRA_CALLBACK FinalizeCallback(void *data) {
    if (data != nullptr) {
      ObjectTemplateData* objectTemplateData =
        reinterpret_cast<ObjectTemplateData*>(data);
      delete objectTemplateData;
    }
  }

  bool AreInterceptorsRequired() {
    return namedPropertyDeleter != nullptr ||
      namedPropertyEnumerator != nullptr ||
      namedPropertyGetter != nullptr ||
      namedPropertyQuery != nullptr ||
      namedPropertySetter != nullptr ||
      indexedPropertyDeleter != nullptr ||
      indexedPropertyEnumerator != nullptr ||
      indexedPropertyGetter != nullptr ||
      indexedPropertyQuery != nullptr ||
      indexedPropertySetter != nullptr;
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
                       ObjectTemplateData *templateData)
    : ExternalData(ExternalDataType),
      objectTemplate(nullptr, Utils::ToLocal(objectTemplate)),
      namedPropertyGetter(templateData->namedPropertyGetter),
      namedPropertySetter(templateData->namedPropertySetter),
      namedPropertyQuery(templateData->namedPropertyQuery),
      namedPropertyDeleter(templateData->namedPropertyDeleter),
      namedPropertyEnumerator(templateData->namedPropertyEnumerator),
      namedPropertyInterceptorData(
        nullptr, templateData->namedPropertyInterceptorData),
      indexedPropertyGetter(templateData->indexedPropertyGetter),
      indexedPropertySetter(templateData->indexedPropertySetter),
      indexedPropertyQuery(templateData->indexedPropertyQuery),
      indexedPropertyDeleter(templateData->indexedPropertyDeleter),
      indexedPropertyEnumerator(templateData->indexedPropertyEnumerator),
      indexedPropertyInterceptorData(
        nullptr, templateData->indexedPropertyInterceptorData),
      internalFieldCount(templateData->internalFieldCount) {
  if (internalFieldCount > 0) {
    internalFields = new FieldValue[internalFieldCount];
  }
}

ObjectData::~ObjectData() {
  if (internalFieldCount > 0) {
    delete[] internalFields;
  }

  objectTemplate.Reset();
  namedPropertyInterceptorData.Reset();
  indexedPropertyInterceptorData.Reset();
}

void CHAKRA_CALLBACK ObjectData::FinalizeCallback(void *data) {
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
    JsValueRef *arguments,
    unsigned short argumentCount,  // NOLINT(runtime/int)
    void *callbackState) {
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
    if (objectData->indexedPropertyGetter) {
      // indexed array properties were set
      PropertyCallbackInfo<Value> info(
        *objectData->indexedPropertyInterceptorData,
        reinterpret_cast<Object*>(object),
        /*holder*/reinterpret_cast<Object*>(object));
      objectData->indexedPropertyGetter(index, info);
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
    if (objectData->namedPropertyGetter) {
      PropertyCallbackInfo<Value> info(
        *objectData->namedPropertyInterceptorData,
        reinterpret_cast<Object*>(object),
        /*holder*/reinterpret_cast<Object*>(object));
      objectData->namedPropertyGetter(reinterpret_cast<String*>(prop), info);
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
    JsValueRef *arguments,
    unsigned short argumentCount,  // NOLINT(runtime/int)
    void *callbackState) {
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
    if (objectData->indexedPropertySetter) {
      PropertyCallbackInfo<Value> info(
        *objectData->indexedPropertyInterceptorData,
        reinterpret_cast<Object*>(object),
        /*holder*/reinterpret_cast<Object*>(object));
      objectData->indexedPropertySetter(
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
    if (objectData->namedPropertySetter) {
      PropertyCallbackInfo<Value> info(
        *objectData->namedPropertyInterceptorData,
        reinterpret_cast<Object*>(object),
        /*holder*/reinterpret_cast<Object*>(object));
      objectData->namedPropertySetter(
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
    JsValueRef *arguments,
    unsigned short argumentCount,  // NOLINT(runtime/int)
    void *callbackState) {
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
    if (objectData->indexedPropertyDeleter != nullptr) {
      PropertyCallbackInfo<Boolean> info(
        *objectData->indexedPropertyInterceptorData,
        reinterpret_cast<Object*>(object),
        /*holder*/reinterpret_cast<Object*>(object));
      objectData->indexedPropertyDeleter(index, info);
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
    if (objectData->namedPropertyDeleter != nullptr) {
      PropertyCallbackInfo<Boolean> info(
        *objectData->namedPropertyInterceptorData,
        reinterpret_cast<Object*>(object),
        /*holder*/reinterpret_cast<Object*>(object));
      objectData->namedPropertyDeleter(reinterpret_cast<String*>(prop), info);
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
    JsValueRef *arguments,
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
    if (objectData->indexedPropertyQuery != nullptr) {
      HandleScope scope(nullptr);
      PropertyCallbackInfo<Integer> info(
        *objectData->indexedPropertyInterceptorData,
        reinterpret_cast<Object*>(object),
        /*holder*/reinterpret_cast<Object*>(object));
      objectData->indexedPropertyQuery(index, info);
      if (info.GetReturnValue().Get() != JS_INVALID_REFERENCE) {
        return jsrt::GetTrue();  // intercepted
      }
    }

    if (objectData->indexedPropertyGetter) {
      // indexed array properties were set
      PropertyCallbackInfo<Value> info(
        *objectData->indexedPropertyInterceptorData,
        reinterpret_cast<Object*>(object),
        /*holder*/reinterpret_cast<Object*>(object));
      objectData->indexedPropertyGetter(index, info);
      if (info.GetReturnValue().Get() != JS_INVALID_REFERENCE) {
        return jsrt::GetTrue();  // intercepted
      }
    }

    // fallback to default JS behavior
    bool hasProperty;
    if (jsrt::HasIndexedProperty(object, index, &hasProperty) != JsNoError) {
      return jsrt::GetFalse();
    }
    return hasProperty ? jsrt::GetTrue() : jsrt::GetFalse();
  } else {  // named property...
    if (objectData->namedPropertyQuery != nullptr) {
      HandleScope scope(nullptr);
      PropertyCallbackInfo<Integer> info(
        *objectData->namedPropertyInterceptorData,
        reinterpret_cast<Object*>(object),
        /*holder*/reinterpret_cast<Object*>(object));
      objectData->namedPropertyQuery(reinterpret_cast<String*>(prop), info);
      if (info.GetReturnValue().Get() != JS_INVALID_REFERENCE) {
        return jsrt::GetTrue();  // intercepted
      }
    }

    if (objectData->namedPropertyGetter) {
      PropertyCallbackInfo<Value> info(
        *objectData->namedPropertyInterceptorData,
        reinterpret_cast<Object*>(object),
        /*holder*/reinterpret_cast<Object*>(object));
      objectData->namedPropertyGetter(reinterpret_cast<String*>(prop), info);
      if (info.GetReturnValue().Get() != JS_INVALID_REFERENCE) {
        return jsrt::GetTrue();  // intercepted
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
    JsValueRef *arguments,
    unsigned short argumentCount,  // NOLINT(runtime/int)
    void *callbackState) {
  return HasPropertyHandler(arguments, argumentCount);
}

JsValueRef Utils::GetPropertiesHandler(
    JsValueRef* arguments,
    unsigned int argumentsCount,  // NOLINT(runtime/int)
    bool getFromPrototype) {
  CHAKRA_VERIFY(argumentsCount >= 2);

  HandleScope scope(nullptr);
  JsValueRef object = arguments[1];

  ObjectData* objectData = nullptr;
  if (!ExternalData::TryGet(object, &objectData)) {
    return jsrt::GetUndefined();
  }

  JsValueRef indexedProperties;
  if (objectData->indexedPropertyEnumerator != nullptr) {
    PropertyCallbackInfo<Array> info(
      *objectData->indexedPropertyInterceptorData,
      reinterpret_cast<Object*>(object),
      /*holder*/reinterpret_cast<Object*>(object));
    objectData->indexedPropertyEnumerator(info);
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
  if (objectData->namedPropertyEnumerator != nullptr) {
    PropertyCallbackInfo<Array> info(
      *objectData->namedPropertyInterceptorData,
      reinterpret_cast<Object*>(object),
      /*holder*/reinterpret_cast<Object*>(object));
    objectData->namedPropertyEnumerator(info);
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
    JsValueRef *arguments,
    unsigned short argumentCount,  // NOLINT(runtime/int)
    void *callbackState) {
  return GetPropertiesEnumeratorHandler(arguments, argumentCount);
}

JsValueRef CHAKRA_CALLBACK Utils::OwnKeysCallback(
    JsValueRef callee,
    bool isConstructCall,
    JsValueRef *arguments,
    unsigned short argumentCount,  // NOLINT(runtime/int)
    void *callbackState) {
  return GetPropertiesHandler(arguments, argumentCount, false);
}

JsValueRef CHAKRA_CALLBACK Utils::GetOwnPropertyDescriptorCallback(
    JsValueRef callee,
    bool isConstructCall,
    JsValueRef *arguments,
    unsigned short argumentCount,  // NOLINT(runtime/int)
    void *callbackState) {
  CHAKRA_VERIFY(argumentCount >= 3);

  JsValueRef object = arguments[1];
  JsValueRef prop = arguments[2];
  JsValueRef descriptor;

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

  if (isPropIntType) {
    if (objectData->indexedPropertyQuery != nullptr) {
      HandleScope scope(nullptr);
      PropertyCallbackInfo<Integer> info(
        *objectData->indexedPropertyInterceptorData,
        reinterpret_cast<Object*>(object),
        /*holder*/reinterpret_cast<Object*>(object));
      objectData->indexedPropertyQuery(index, info);
      queryResult = reinterpret_cast<JsValueRef>(info.GetReturnValue().Get());
    }

    if (objectData->indexedPropertyGetter != nullptr) {
      PropertyCallbackInfo<Value> info(
        *objectData->indexedPropertyInterceptorData,
        reinterpret_cast<Object*>(object),
        /*holder*/reinterpret_cast<Object*>(object));
      objectData->indexedPropertyGetter(index, info);
      value = reinterpret_cast<JsValueRef>(info.GetReturnValue().Get());
    }

    // if queryResult valid, ensure value available
    if (queryResult != JS_INVALID_REFERENCE && value == JS_INVALID_REFERENCE) {
      if (jsrt::GetIndexedProperty(object, index, &value) != JsNoError) {
        return jsrt::GetUndefined();
      }
    }
  } else {  // named property...
    // query the property descriptor if there is such, and then get the value
    // from the proxy in order to go through the interceptor
    if (objectData->namedPropertyQuery != nullptr) {
      HandleScope scope(nullptr);
      PropertyCallbackInfo<Integer> info(
        *objectData->namedPropertyInterceptorData,
        reinterpret_cast<Object*>(object),
        /*holder*/reinterpret_cast<Object*>(object));
      objectData->namedPropertyQuery(reinterpret_cast<String*>(prop), info);
      queryResult = reinterpret_cast<JsValueRef>(info.GetReturnValue().Get());
    }

    if (objectData->namedPropertyGetter != nullptr) {
      PropertyCallbackInfo<Value> info(
        *objectData->namedPropertyInterceptorData,
        reinterpret_cast<Object*>(object),
        /*holder*/reinterpret_cast<Object*>(object));
      objectData->namedPropertyGetter(reinterpret_cast<String*>(prop), info);
      value = reinterpret_cast<JsValueRef>(info.GetReturnValue().Get());
    }

    // if queryResult valid, ensure value available
    if (queryResult != JS_INVALID_REFERENCE && value == JS_INVALID_REFERENCE) {
      if (jsrt::GetProperty(object, prop, &value) != JsNoError) {
        return jsrt::GetUndefined();
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

Local<ObjectTemplate> ObjectTemplate::New(Isolate* isolate) {
  JsValueRef objectTemplateRef;
  ObjectTemplateData* templateData = new ObjectTemplateData();

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
    JsValueRef *arguments,
    unsigned short argumentCount,  // NOLINT(runtime/int)
    void *callbackState) {
  return reinterpret_cast<JsValueRef>(callbackState);
}

MaybeLocal<Object> ObjectTemplate::NewInstance(Local<Context> context) {
  return NewInstance(Local<Object>());
}

Local<Object> ObjectTemplate::NewInstance() {
  return FromMaybe(NewInstance(Local<Context>()));
}

Local<Object> ObjectTemplate::NewInstance(Handle<Object> prototype) {
  ObjectTemplateData* objectTemplateData = nullptr;
  if (!ExternalData::TryGet(this, &objectTemplateData)) {
    return Local<Object>();
  }

  ObjectData *objectData = new ObjectData(this, objectTemplateData);
  JsValueRef newInstanceRef = JS_INVALID_REFERENCE;
  if (JsCreateExternalObject(objectData,
                             ObjectData::FinalizeCallback,
                             &newInstanceRef) != JsNoError) {
    delete objectData;
    return Local<Object>();
  }

  if (!prototype.IsEmpty()) {
    if (JsSetPrototype(newInstanceRef,
                       reinterpret_cast<JsValueRef>(*prototype)) != JsNoError) {
      return Local<Object>();
    }
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

    if (objectTemplateData->indexedPropertyGetter != nullptr ||
        objectTemplateData->namedPropertyGetter != nullptr)
      proxyConf[jsrt::ProxyTraps::GetTrap] = Utils::GetCallback;

    if (objectTemplateData->indexedPropertySetter != nullptr ||
        objectTemplateData->namedPropertySetter != nullptr)
      proxyConf[jsrt::ProxyTraps::SetTrap] = Utils::SetCallback;

    if (objectTemplateData->indexedPropertyDeleter != nullptr ||
        objectTemplateData->namedPropertyDeleter != nullptr)
      proxyConf[jsrt::ProxyTraps::DeletePropertyTrap] =
          Utils::DeletePropertyCallback;

    if (objectTemplateData->indexedPropertyEnumerator != nullptr ||
        objectTemplateData->namedPropertyEnumerator != nullptr) {
      proxyConf[jsrt::ProxyTraps::EnumerateTrap] = Utils::EnumerateCallback;
      proxyConf[jsrt::ProxyTraps::OwnKeysTrap] = Utils::OwnKeysCallback;
    }

    if (objectTemplateData->indexedPropertyQuery != nullptr ||
        objectTemplateData->namedPropertyQuery != nullptr ||
        objectTemplateData->indexedPropertyGetter != nullptr ||
        objectTemplateData->namedPropertyGetter != nullptr) {
      proxyConf[jsrt::ProxyTraps::HasTrap] = Utils::HasCallback;
    }

    if (objectTemplateData->indexedPropertyQuery != nullptr ||
        objectTemplateData->namedPropertyQuery != nullptr ||
        objectTemplateData->indexedPropertyGetter != nullptr ||
        objectTemplateData->namedPropertyGetter != nullptr) {
      proxyConf[jsrt::ProxyTraps::GetOwnPropertyDescriptorTrap] =
          Utils::GetOwnPropertyDescriptorCallback;
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

  objectData->objectInstance = newInstanceRef;
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
  ObjectTemplateData *objectTemplateData = nullptr;
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
    Handle<Value> data) {
  ObjectTemplateData *objectTemplateData = nullptr;
  if (!ExternalData::TryGet(this, &objectTemplateData)) {
    return;
  }
  objectTemplateData->namedPropertyGetter = getter;
  objectTemplateData->namedPropertySetter = setter;
  objectTemplateData->namedPropertyQuery = query;
  objectTemplateData->namedPropertyDeleter = remover;
  objectTemplateData->namedPropertyEnumerator = enumerator;
  objectTemplateData->namedPropertyInterceptorData = data;
}

void ObjectTemplate::SetHandler(
    const NamedPropertyHandlerConfiguration& config) {
  SetNamedPropertyHandler(
    reinterpret_cast<NamedPropertyGetterCallback>(config.getter),
    reinterpret_cast<NamedPropertySetterCallback>(config.setter),
    reinterpret_cast<NamedPropertyQueryCallback>(config.query),
    reinterpret_cast<NamedPropertyDeleterCallback>(config.deleter),
    reinterpret_cast<NamedPropertyEnumeratorCallback>(config.enumerator),
    config.data);
}

void ObjectTemplate::SetIndexedPropertyHandler(
    IndexedPropertyGetterCallback getter,
    IndexedPropertySetterCallback setter,
    IndexedPropertyQueryCallback query,
    IndexedPropertyDeleterCallback remover,
    IndexedPropertyEnumeratorCallback enumerator,
    Handle<Value> data) {
  ObjectTemplateData *objectTemplateData = nullptr;
  if (!ExternalData::TryGet(this, &objectTemplateData)) {
    return;
  }

  objectTemplateData->indexedPropertyGetter = getter;
  objectTemplateData->indexedPropertySetter = setter;
  objectTemplateData->indexedPropertyQuery = query;
  objectTemplateData->indexedPropertyDeleter = remover;
  objectTemplateData->indexedPropertyEnumerator = enumerator;
  objectTemplateData->indexedPropertyInterceptorData = data;
}

void ObjectTemplate::SetHandler(
    const IndexedPropertyHandlerConfiguration& config) {
  SetIndexedPropertyHandler(
    config.getter,
    config.setter,
    config.query,
    config.deleter,
    config.enumerator,
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

void ObjectTemplate::SetClassName(Handle<String> className) {
  ObjectTemplateData* objectTemplateData = nullptr;
  if (!ExternalData::TryGet(this, &objectTemplateData)) {
    CHAKRA_ASSERT(false);  // This should never happen
    return;
  }

  objectTemplateData->className = className;
}

Handle<String> ObjectTemplate::GetClassName() {
  ObjectTemplateData* objectTemplateData = nullptr;
  if (!ExternalData::TryGet(this, &objectTemplateData)) {
    CHAKRA_ASSERT(false);  // This should never happen
    return Handle<String>();
  }

  return objectTemplateData->className;
}

}  // namespace v8
