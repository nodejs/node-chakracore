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
#include <memory>

namespace v8 {

enum AccessorType {
  Setter = 0,
  Getter = 1
};

typedef struct AcessorExternalDataType {
  Persistent<Value> data;
  Persistent<AccessorSignature> signature;
  AccessorType type;
  union {
    AccessorNameGetterCallback getter;
    AccessorNameSetterCallback setter;
  };
  Persistent<Name> propertyName;

  bool CheckSignature(Local<Object> thisPointer, Local<Object>* holder) {
    if (signature.IsEmpty()) {
      *holder = thisPointer;
      return true;
    }

    Local<FunctionTemplate> receiver = signature.As<FunctionTemplate>();
    return Utils::CheckSignature(receiver, thisPointer, holder);
  }
} AccessorExternalData;

struct InternalFieldDataStruct {
  void** internalFieldPointers;
  int size;
};

Maybe<bool> Object::Set(Local<Context> context,
                        Local<Value> key, Local<Value> value) {
  return Set(key, value, PropertyAttribute::None, /*force*/false);
}

// CHAKRA-TODO: Convert this function to javascript
bool Object::Set(Handle<Value> key, Handle<Value> value) {
  return FromMaybe(Set(Local<Context>(), key, value));
}

Maybe<bool> Object::Set(Handle<Value> key, Handle<Value> value,
                        PropertyAttribute attribs, bool force) {
  JsPropertyIdRef idRef;

  if (jsrt::GetPropertyIdFromValue((JsValueRef)*key, &idRef) != JsNoError) {
    return Nothing<bool>();
  }

  // Do it faster if there are no property attributes
  if (!force && attribs == None) {
    if (JsSetProperty((JsValueRef)this,
                      idRef, (JsValueRef)*value, false) != JsNoError) {
      return Nothing<bool>();
    }
  } else {  // we have attributes just use it
    jsrt::PropertyDescriptorOptionValues writable =
      jsrt::PropertyDescriptorOptionValues::False;

    if ((attribs & ReadOnly) == 0) {
      writable = jsrt::PropertyDescriptorOptionValues::True;
    }

    jsrt::PropertyDescriptorOptionValues enumerable =
      jsrt::PropertyDescriptorOptionValues::False;

    if ((attribs & DontEnum) == 0) {
      enumerable = jsrt::PropertyDescriptorOptionValues::True;
    }

    jsrt::PropertyDescriptorOptionValues configurable =
      jsrt::PropertyDescriptorOptionValues::False;

    if ((attribs & DontDelete) == 0) {
      configurable = jsrt::PropertyDescriptorOptionValues::True;
    }

    if (jsrt::DefineProperty((JsValueRef)this,
                             idRef,
                             writable,
                             enumerable,
                             configurable,
                             (JsValueRef)*value,
                             JS_INVALID_REFERENCE,
                             JS_INVALID_REFERENCE) != JsNoError) {
      return Nothing<bool>();
    }
  }

  return Just(true);
}

Maybe<bool> Object::Set(Local<Context> context,
                        uint32_t index, Local<Value> value) {
  if (jsrt::SetIndexedProperty((JsValueRef)this, index,
                               (JsValueRef)(*value)) != JsNoError) {
    return Nothing<bool>();
  }

  return Just(true);
}

Maybe<bool> Object::DefineOwnProperty(
    Local<Context> context, Local<Name> key, Local<Value> value,
    PropertyAttribute attributes) {
  return Set(key, value, attributes, /*force*/true);
}

Maybe<bool> Object::DefineProperty(Local<v8::Context> context,
                                   Local<Name> key,
                                   PropertyDescriptor& descriptor) {
  JsPropertyIdRef idRef;

  if (jsrt::GetPropertyIdFromValue((JsValueRef)*key, &idRef) != JsNoError) {
    return Nothing<bool>();
  }

  Local<Value> value = descriptor.value();
  Local<Value> get = descriptor.get();
  Local<Value> set = descriptor.set();

  // Do it faster if there are no property attributes nor accessors
  if (get.IsEmpty() && set.IsEmpty() &&
      !descriptor.has_enumerable() &&
      !descriptor.has_configurable() &&
      !descriptor.has_writable()) {
    if (JsSetProperty((JsValueRef)this,
                      idRef, (JsValueRef)*value, false) != JsNoError) {
      return Nothing<bool>();
    }
  } else {  // we have attributes just use it
    jsrt::PropertyDescriptorOptionValues enumerable =
        jsrt::PropertyDescriptorOptionValues::None;
    jsrt::PropertyDescriptorOptionValues configurable =
        jsrt::PropertyDescriptorOptionValues::None;
    jsrt::PropertyDescriptorOptionValues writable =
        jsrt::PropertyDescriptorOptionValues::None;

    if (descriptor.has_enumerable()) {
      enumerable = descriptor.enumerable() ?
          jsrt::PropertyDescriptorOptionValues::True :
          jsrt::PropertyDescriptorOptionValues::False;
    }

    if (descriptor.has_configurable()) {
      configurable = descriptor.configurable() ?
          jsrt::PropertyDescriptorOptionValues::True :
          jsrt::PropertyDescriptorOptionValues::False;
    }

    if (descriptor.has_writable()) {
      writable = descriptor.writable() ?
          jsrt::PropertyDescriptorOptionValues::True :
          jsrt::PropertyDescriptorOptionValues::False;
    }

    if (jsrt::DefineProperty((JsValueRef)this,
                             idRef,
                             writable,
                             enumerable,
                             configurable,
                             (JsValueRef)*value,
                             (JsValueRef)*get,
                             (JsValueRef)*set) != JsNoError) {
      return Nothing<bool>();
    }
  }

  return Just(true);
}

bool Object::Set(uint32_t index, Handle<Value> value) {
  return FromMaybe(Set(Local<Context>(), index, value));
}

Maybe<bool> Object::ForceSet(Local<Context> context,
                             Local<Value> key, Local<Value> value,
                             PropertyAttribute attribs) {
  return Set(key, value, attribs, /*force*/true);
}

bool Object::ForceSet(Handle<Value> key, Handle<Value> value,
                      PropertyAttribute attribs) {
  return FromMaybe(ForceSet(Local<Context>(), key, value, attribs));
}

MaybeLocal<Value> Object::Get(Local<Context> context, Local<Value> key) {
  JsPropertyIdRef idRef;
  if (jsrt::GetPropertyIdFromValue((JsValueRef)*key, &idRef) != JsNoError) {
    return Local<Value>();
  }

  JsValueRef valueRef;
  if (JsGetProperty((JsValueRef)this, idRef, &valueRef) != JsNoError) {
    return Local<Value>();
  }

  return Local<Value>::New(valueRef);
}

Local<Value> Object::Get(Handle<Value> key) {
  return FromMaybe(Get(Local<Context>(), key));
}

MaybeLocal<Value> Object::Get(Local<Context> context, uint32_t index) {
  JsValueRef valueRef;

  if (jsrt::GetIndexedProperty((JsValueRef)this, index,
                               &valueRef) != JsNoError) {
    return Local<Value>();
  }

  return Local<Value>::New(valueRef);
}

Local<Value> Object::Get(uint32_t index) {
  return FromMaybe(Get(Local<Context>(), index));
}

Maybe<PropertyAttribute> Object::GetPropertyAttributes(Local<Context> context,
                                                       Local<Value> key) {
  JsValueRef getPropertyAttributesFunction =
    jsrt::ContextShim::GetCurrent()->GetgetPropertyAttributesFunction();

  JsValueRef result;
  if (jsrt::CallFunction(getPropertyAttributesFunction, (JsValueRef) this,
                         (JsValueRef)*key, &result) != JsNoError) {
    return Nothing<PropertyAttribute>();
  }

  int attributesValue;
  if (jsrt::ValueToInt(result, &attributesValue) != JsNoError ||
      attributesValue == -1) {
    return Nothing<PropertyAttribute>();
  }

  return Just(static_cast<PropertyAttribute>(attributesValue));
}

PropertyAttribute Object::GetPropertyAttributes(Handle<Value> key) {
  return FromMaybe(GetPropertyAttributes(Local<Context>(), key));
}

MaybeLocal<Value> Object::GetOwnPropertyDescriptor(Local<Context> context,
                                                   Local<String> key) {
  JsValueRef result;
  if (jsrt::GetOwnPropertyDescriptor(this, *key, &result) != JsNoError) {
    return Local<Value>();
  }
  return Local<Value>::New(result);
}

Local<Value> Object::GetOwnPropertyDescriptor(Local<String> key) {
  return FromMaybe(GetOwnPropertyDescriptor(Local<Context>(), key));
}

Maybe<bool> Object::Has(Local<Context> context, Local<Value> key) {
  JsPropertyIdRef idRef;
  if (jsrt::GetPropertyIdFromValue((JsValueRef)*key, &idRef) != JsNoError) {
    return Nothing<bool>();
  }

  bool result;
  if (JsHasProperty(this, idRef, &result) != JsNoError) {
    return Nothing<bool>();
  }

  return Just(result);
}

bool Object::Has(Handle<Value> key) {
  return FromMaybe(Has(Local<Context>(), key));
}

Maybe<bool> Object::Delete(Local<Context> context, Local<Value> key) {
  JsPropertyIdRef idRef;
  if (jsrt::GetPropertyIdFromValue((JsValueRef)*key, &idRef) != JsNoError) {
    return Nothing<bool>();
  }

  JsValueRef result;
  if (JsDeleteProperty(this, idRef, false, &result) != JsNoError) {
    return Nothing<bool>();
  }

  return Just(Local<Value>(result)->BooleanValue());
}

bool Object::Delete(Handle<Value> key) {
  return FromMaybe(Delete(Local<Context>(), key));
}

Maybe<bool> Object::Has(Local<Context> context, uint32_t index) {
  bool result;
  if (jsrt::HasIndexedProperty(this, index, &result) != JsNoError) {
    return Nothing<bool>();
  }
  return Just(result);
}

bool Object::Has(uint32_t index) {
  return FromMaybe(Has(Local<Context>(), index));
}

Maybe<bool> Object::Delete(Local<Context> context, uint32_t index) {
  if (jsrt::DeleteIndexedProperty((JsValueRef)this, index) != JsNoError) {
    return Nothing<bool>();
  }
  return Just(true);
}

bool Object::Delete(uint32_t index) {
  return FromMaybe(Delete(Local<Context>(), index));
}

void CHAKRA_CALLBACK AcessorExternalObjectFinalizeCallback(void *data) {
  if (data != nullptr) {
    AccessorExternalData *accessorData =
      static_cast<AccessorExternalData*>(data);
    delete accessorData;
  }
}

Maybe<bool> Object::SetAccessor(Handle<Name> name,
                                AccessorNameGetterCallback getter,
                                AccessorNameSetterCallback setter,
                                v8::Handle<Value> data,
                                AccessControl settings,
                                PropertyAttribute attributes,
                                Handle<AccessorSignature> signature) {
  JsValueRef getterRef = JS_INVALID_REFERENCE;
  JsValueRef setterRef = JS_INVALID_REFERENCE;

  JsPropertyIdRef idRef;
  if (jsrt::GetPropertyIdFromName((JsValueRef)*name, &idRef) != JsNoError) {
    return Nothing<bool>();
  }

  if (getter != nullptr) {
    AccessorExternalData *externalData = new AccessorExternalData();
    externalData->type = Getter;
    externalData->propertyName = name;
    externalData->getter = getter;
    externalData->data = data;
    externalData->signature = signature;

    if (jsrt::CreateFunctionWithExternalData(
            Utils::AccessorHandler, externalData,
            AcessorExternalObjectFinalizeCallback, &getterRef) != JsNoError) {
      delete externalData;
      return Nothing<bool>();
    }
  }

  if (setter != nullptr) {
    AccessorExternalData *externalData = new AccessorExternalData();
    externalData->type = Setter;
    externalData->propertyName = name;
    externalData->setter = setter;
    externalData->data = data;
    externalData->signature = signature;

    if (jsrt::CreateFunctionWithExternalData(
            Utils::AccessorHandler, externalData,
            AcessorExternalObjectFinalizeCallback, &setterRef) != JsNoError) {
      delete externalData;
      return Nothing<bool>();
    }
  }

  jsrt::PropertyDescriptorOptionValues enumerable =
      jsrt::GetPropertyDescriptorOptionValue(!(attributes & DontEnum));
  jsrt::PropertyDescriptorOptionValues configurable =
      jsrt::GetPropertyDescriptorOptionValue(!(attributes & DontDelete));

  // CHAKRA-TODO: we ignore  AccessControl for now..

  if (jsrt::DefineProperty((JsValueRef)this,
                           idRef,
                           jsrt::PropertyDescriptorOptionValues::None,
                           enumerable,
                           configurable,
                           JS_INVALID_REFERENCE,
                           getterRef,
                           setterRef) != JsNoError) {
    return Nothing<bool>();
  }

  return Just(true);
}

Maybe<bool> Object::SetAccessor(Local<Context> context,
                                Local<Name> name,
                                AccessorNameGetterCallback getter,
                                AccessorNameSetterCallback setter,
                                MaybeLocal<Value> data,
                                AccessControl settings,
                                PropertyAttribute attribute) {
  return SetAccessor(name, getter, setter, FromMaybe(data), settings, attribute,
                     Local<AccessorSignature>());
}

bool Object::SetAccessor(Handle<String> name,
                         AccessorGetterCallback getter,
                         AccessorSetterCallback setter,
                         v8::Handle<Value> data,
                         AccessControl settings,
                         PropertyAttribute attributes) {
  return FromMaybe(SetAccessor(
    name,
    reinterpret_cast<AccessorNameGetterCallback>(getter),
    reinterpret_cast<AccessorNameSetterCallback>(setter),
    data, settings, attributes, Handle<AccessorSignature>()));
}

bool Object::SetAccessor(Handle<Name> name,
                         AccessorNameGetterCallback getter,
                         AccessorNameSetterCallback setter,
                         Handle<Value> data,
                         AccessControl settings,
                         PropertyAttribute attribute) {
  return FromMaybe(SetAccessor(
    Local<Context>(), name, getter, setter, data, settings, attribute));
}

MaybeLocal<Array> Object::GetPropertyNames(Local<Context> context) {
  JsValueRef arrayRef;

  if (jsrt::GetPropertyNames((JsValueRef)this, &arrayRef) != JsNoError) {
    return Local<Array>();
  }

  return Local<Array>::New(arrayRef);
}

Local<Array> Object::GetPropertyNames() {
  return FromMaybe(GetPropertyNames(Local<Context>()));
}

MaybeLocal<Array> Object::GetOwnPropertyNames(Local<Context> context) {
  ContextShim* contextShim = ContextShim::GetCurrent();
  JsValueRef getOwnPropertyNamesFunction =
    contextShim->GetgetOwnPropertyNamesFunction();

  JsValueRef result;

  if (jsrt::CallFunction(getOwnPropertyNamesFunction, (JsValueRef)this,
                         &result) != JsNoError) {
    return Local<Array>();
  }
  return Local<Array>::New(result);
}

Local<Array> Object::GetOwnPropertyNames() {
  return FromMaybe(GetOwnPropertyNames(Local<Context>()));
}

Local<Value> Object::GetPrototype() {
  JsValueRef protoypeRef;

  if (JsGetPrototype((JsValueRef)this, &protoypeRef) != JsNoError) {
    return Local<Value>();
  }

  return Local<Value>::New(static_cast<Value*>(protoypeRef));
}

Maybe<bool> Object::SetPrototype(Local<Context> context,
                                 Local<Value> prototype) {
  if (JsSetPrototype((JsValueRef)this, *prototype) != JsNoError) {
    return Nothing<bool>();
  }

  return Just(true);
}

bool Object::SetPrototype(Handle<Value> prototype) {
  return FromMaybe(SetPrototype(Local<Context>(), prototype));
}

MaybeLocal<String> Object::ObjectProtoToString(Local<Context> context) {
  ContextShim* contextShim = ContextShim::GetCurrent();
  JsValueRef toString = contextShim->GetToStringFunction();

  JsValueRef result;
  JsValueRef args[] = { this };
  if (JsCallFunction(toString, args, _countof(args), &result) != JsNoError) {
    return Local<String>();
  }
  return Local<String>::New(result);
}

Local<String> Object::ObjectProtoToString() {
  return FromMaybe(ObjectProtoToString(Local<Context>()));
}

Local<String> v8::Object::GetConstructorName() {
  JsValueRef constructor;
  if (jsrt::GetObjectConstructor((JsValueRef)this,
                                 &constructor) != JsNoError) {
    return Local<String>();
  }

  jsrt::IsolateShim* iso = jsrt::IsolateShim::FromIsolate(this->GetIsolate());
  JsPropertyIdRef idRef = iso->GetCachedPropertyIdRef(
      jsrt::CachedPropertyIdRef::name);

  JsValueRef name;
  if (JsGetProperty(constructor, idRef, &name) != JsNoError) {
    return Local<String>();
  }

  return Local<String>::New(static_cast<String*>(name));
}

Maybe<bool> Object::HasOwnProperty(Local<Context> context, Local<Name> key) {
  JsValueRef result;
  if (jsrt::HasOwnProperty((JsValueRef)this,
                           (JsValueRef)*key, &result) != JsNoError) {
    return Nothing<bool>();
  }

  return Local<Value>(result)->BooleanValue(context);
}

bool Object::HasOwnProperty(Handle<String> key) {
  return FromMaybe(HasOwnProperty(Local<Context>(), key));
}

Maybe<bool> Object::HasRealNamedProperty(Local<Context> context,
                                         Local<Name> key) {
  return Has(context, key);
}

bool Object::HasRealNamedProperty(Handle<String> key) {
  return FromMaybe(HasRealNamedProperty(Local<Context>(), key));
}

Maybe<bool> Object::HasRealIndexedProperty(Local<Context> context,
                                           uint32_t index) {
  return Has(context, index);
}

bool Object::HasRealIndexedProperty(uint32_t index) {
  return FromMaybe(HasRealIndexedProperty(Local<Context>(), index));
}

MaybeLocal<Value> Object::GetRealNamedProperty(Local<Context> context,
                                               Local<Name> key) {
  // CHAKRA-TODO: how to skip interceptors?
  return Get(context, key);
}

Local<Value> Object::GetRealNamedProperty(Handle<String> key) {
  return FromMaybe(GetRealNamedProperty(Local<Context>(), key));
}

Maybe<PropertyAttribute> Object::GetRealNamedPropertyAttributes(
  Local<Context> context, Local<Name> key) {
  // CHAKRA-TODO: This walks prototype chain skipping interceptors
  return GetPropertyAttributes(context, key);
}

JsValueRef CHAKRA_CALLBACK Utils::AccessorHandler(
    JsValueRef callee,
    bool isConstructCall,
    JsValueRef *arguments,
    unsigned short argumentCount,  // NOLINT(runtime/int)
    void *callbackState) {
  void *externalData;
  JsValueRef result = JS_INVALID_REFERENCE;

  if (JsGetUndefinedValue(&result) != JsNoError) {
    return JS_INVALID_REFERENCE;
  }

  if (jsrt::GetExternalData(callee, &externalData) != JsNoError) {
    return result;
  }

  if (externalData == nullptr) {
    return result;
  }

  AccessorExternalData *accessorData =
    static_cast<AccessorExternalData*>(externalData);
  Local<Value> dataLocal = accessorData->data;

  JsValueRef thisRef = JS_INVALID_REFERENCE;
  if (argumentCount > 0) {
    thisRef = arguments[0];
  }
  // this is ok since the first argument will stay on the stack as long as we
  // are in this function
  Local<Object> thisLocal(static_cast<Object*>(thisRef));

  Local<Object> holder;
  if (!accessorData->CheckSignature(thisLocal, &holder)) {
    return JS_INVALID_REFERENCE;
  }

  Local<Name> propertyNameLocal = accessorData->propertyName;
  switch (accessorData->type) {
    case Setter:
    {
      CHAKRA_VERIFY(argumentCount == 2);
      PropertyCallbackInfo<void> info(dataLocal, thisLocal, holder);
      accessorData->setter(
          propertyNameLocal, static_cast<Value*>(arguments[1]), info);
      break;
    }
    case Getter:
    {
      PropertyCallbackInfo<Value> info(dataLocal, thisLocal, holder);
      accessorData->getter(propertyNameLocal, info);
      result = info.GetReturnValue().Get();
      break;
    }
     default:
      break;
  }

  return result;
}

Maybe<bool> Object::HasPrivate(Local<Context> context, Local<Private> key) {
  return Just(jsrt::HasPrivate((JsValueRef)this, (JsValueRef)*key));
}


Maybe<bool> Object::DeletePrivate(Local<Context> context, Local<Private> key) {
  return Just(jsrt::DeletePrivate((JsValueRef)this, (JsValueRef)*key));
}

Local<Value> Object::GetHiddenValue(Handle<String> key) {
  JsValueRef result;
  if (jsrt::GetPrivate((JsValueRef)this, (JsValueRef)*key, &result)
      != JsNoError) {
    result = jsrt::GetUndefined();
  }

  return Local<Value>::New(static_cast<Value*>(result));
}

MaybeLocal<Value> Object::GetPrivate(Local<Context> context,
                                     Local<Private> key) {
  JsValueRef result;
  if (jsrt::GetPrivate((JsValueRef)this, (JsValueRef)*key, &result)
      != JsNoError) {
    result = jsrt::GetUndefined();
  }

  return Local<Value>::New(static_cast<Value*>(result));
}

bool Object::SetHiddenValue(Handle<String> key, Handle<Value> value) {
  if (jsrt::SetPrivate((JsValueRef)this, (JsValueRef)*key,
                           (JsValueRef)*value) != JsNoError) {
    return false;
  }

  return true;
}

Maybe<bool> Object::SetPrivate(Local<Context> context, Local<Private> key,
                               Local<Value> value) {
  if (jsrt::SetPrivate((JsValueRef)this, (JsValueRef)*key,
                           (JsValueRef)*value) != JsNoError) {
    return Just(false);
  }

  return Just(true);
}


ObjectTemplate* Object::GetObjectTemplate() {
  ObjectData *objectData = nullptr;
  return Utils::GetObjectData(this, &objectData) == JsNoError
          && objectData != nullptr ?
    *objectData->objectTemplate : nullptr;
}

JsErrorCode Utils::GetObjectData(Object* object, ObjectData** objectData) {
  *objectData = nullptr;

  if (object->IsUndefined()) {
    return JsNoError;
  }

  JsErrorCode error;
  JsValueRef self = object;
  {
    JsPropertyIdRef selfSymbolIdRef =
      jsrt::IsolateShim::GetCurrent()->GetSelfSymbolPropertyIdRef();
    if (selfSymbolIdRef != JS_INVALID_REFERENCE) {
      JsValueRef result;
      error = JsGetProperty(object, selfSymbolIdRef, &result);
      if (error != JsNoError) {
        return error;
      }

      if (!Local<Value>(result)->IsUndefined()) {
        self = result;
      }
    }
  }

  return ExternalData::GetExternalData(self, objectData);
}

int Object::InternalFieldCount() {
  ObjectData* objectData;
  if (Utils::GetObjectData(this, &objectData) != JsNoError || !objectData) {
    return 0;
  }

  return objectData->internalFieldCount;
}

Local<Value> Object::GetInternalField(int index) {
  ObjectData::FieldValue* field = ObjectData::GetInternalField(this, index);
  return field ? field->GetRef() : nullptr;
}

void Object::SetInternalField(int index, Handle<Value> value) {
  ObjectData::FieldValue* field = ObjectData::GetInternalField(this, index);
  if (field) {
    field->SetRef(*value);
  }
}

void* Object::GetAlignedPointerFromInternalField(int index) {
  ObjectData::FieldValue* field = ObjectData::GetInternalField(this, index);
  return field ? field->GetPointer() : nullptr;
}

void Object::SetAlignedPointerInInternalField(int index, void *value) {
  ObjectData::FieldValue* field = ObjectData::GetInternalField(this, index);
  if (field) {
    field->SetPointer(value);
  }
}

Local<Object> Object::Clone() {
  JsValueRef constructor;
  if (jsrt::GetObjectConstructor((JsValueRef)this,
                                 &constructor) != JsNoError) {
    return Local<Object>();
  }

  JsValueRef obj;
  if (jsrt::ConstructObject(constructor, &obj) != JsNoError) {
    return Local<Object>();
  }

  if (jsrt::CloneObject((JsValueRef)this, obj) != JsNoError) {
    return Local<Object>();
  }

  return Local<Object>::New(obj);
}

Local<Context> Object::CreationContext() {
  jsrt::ContextShim * contextShim =
    jsrt::IsolateShim::GetContextShimOfObject(this);
  if (contextShim == nullptr) {
    return Local<Context>();
  }
  return Local<Context>::New(
    static_cast<Context *>(contextShim->GetContextRef()));
}

Isolate* Object::GetIsolate() {
  return jsrt::IsolateShim::GetContextShimOfObject(this)->
      GetIsolateShim()->GetCurrentAsIsolate();
}

Local<Object> Object::New(Isolate* isolate) {
  JsValueRef newObjectRef;
  if (JsCreateObject(&newObjectRef) != JsNoError) {
    return Local<Object>();
  }

  return Local<Object>::New(static_cast<Object*>(newObjectRef));
}

Object *Object::Cast(Value *obj) {
  CHAKRA_ASSERT(obj->IsObject());
  return static_cast<Object*>(obj);
}

Maybe<bool> Object::CreateDataProperty(Local<Context> context, Local<Name> key,
                                       Local<Value> value) {
  return Set(context, key, value);
}

Maybe<bool> Object::CreateDataProperty(Local<Context> context, uint32_t index,
                                       Local<Value> value) {
  return Set(context, index, value);
}

void Object::SetAccessorProperty(Local<Name> name, Local<Function> getter,
                                 Local<Function> setter,
                                 PropertyAttribute attribute,
                                 AccessControl settings) {
  JsPropertyIdRef idRef;
  if (jsrt::GetPropertyIdFromName((JsValueRef)*name, &idRef) != JsNoError) {
    return;
  }

  jsrt::PropertyDescriptorOptionValues enumerable =
      jsrt::GetPropertyDescriptorOptionValue(!(attribute & DontEnum));
  jsrt::PropertyDescriptorOptionValues configurable =
      jsrt::GetPropertyDescriptorOptionValue(!(attribute & DontDelete));

  // CHAKRA-TODO: we ignore  AccessControl for now..

  if (jsrt::DefineProperty((JsValueRef)this,
                           idRef,
                           jsrt::PropertyDescriptorOptionValues::None,
                           enumerable,
                           configurable,
                           JS_INVALID_REFERENCE,
                           (JsValueRef)*getter,
                           (JsValueRef)*setter) != JsNoError) {
    return;
  }
}

Maybe<bool> Object::SetIntegrityLevel(Local<Context> context,
                                      IntegrityLevel level) {
  // CHAKRA-TODO: implement this later?
  return Nothing<bool>();
}

}  // namespace v8
