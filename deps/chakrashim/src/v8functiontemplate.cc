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
#include "jsrtutils.h"

namespace v8 {

using jsrt::IsolateShim;
using jsrt::ContextShim;

class FunctionCallbackData : public ExternalData {
 public:
  static const ExternalDataTypes ExternalDataType =
    ExternalDataTypes::FunctionCallbackData;

 private:
  FunctionCallback callback;
  Persistent<Value> data;
  Persistent<Signature> signature;

  Persistent<ObjectTemplate> instanceTemplate;
  Persistent<Object> prototype;

 public:
  FunctionCallbackData(FunctionCallback callback,
                       Local<Value> data,
                       Local<Signature> signature,
                       Local<ObjectTemplate> instanceTemplate)
      : ExternalData(ExternalDataType),
        callback(callback),
        data(nullptr, data),
        signature(nullptr, signature),
        instanceTemplate(nullptr, instanceTemplate) {
  }

  ~FunctionCallbackData() {
    data.Reset();
    signature.Reset();
    instanceTemplate.Reset();
    prototype.Reset();
  }

  static void CHAKRA_CALLBACK FinalizeCallback(void *data) {
    if (data != nullptr) {
      FunctionCallbackData* functionCallbackData =
        reinterpret_cast<FunctionCallbackData*>(data);
      delete functionCallbackData;
    }
  }

  void SavePrototype(Local<Object> prototype) {
    this->prototype.Reset(nullptr, prototype);
  }

  Local<Object> NewInstance() {
    Utils::EnsureObjectTemplate(&instanceTemplate);
    return !instanceTemplate.IsEmpty() ?
      instanceTemplate->NewInstance(prototype) : Local<Object>();
  }

  bool CheckSignature(Local<Object> thisPointer,
                      JsValueRef *arguments,
                      unsigned short argumentCount,  // NOLINT(runtime/int)
                      Local<Object>* holder) {
    if (signature.IsEmpty()) {
      *holder = thisPointer;
      return true;
    }

    return Utils::CheckSignature(signature.As<FunctionTemplate>(),
                                 thisPointer, holder);
  }

  static JsValueRef CHAKRA_CALLBACK FunctionInvoked(
      JsValueRef callee,
      bool isConstructCall,
      JsValueRef *arguments,
      unsigned short argumentCount,  // NOLINT(runtime/int)
      void *callbackState) {
    CHAKRA_VERIFY(argumentCount >= 1);

    // Script engine could have switched context. Make sure to invoke the
    // CHAKRA_CALLBACK in the current callee context.
    ContextShim* contextShim = IsolateShim::GetContextShimOfObject(callee);
    ContextShim::Scope contextScope(contextShim);
    HandleScope scope(nullptr);

    FunctionCallbackData* callbackData = nullptr;
    if (!ExternalData::TryGet(JsValueRef(callbackState), &callbackData)) {
      CHAKRA_ASSERT(false);  // This should never happen
      return JS_INVALID_REFERENCE;
    }

    Local<Object> thisPointer;
    ++arguments;  // skip the this argument

    if (isConstructCall) {
      thisPointer = callbackData->NewInstance();
      if (thisPointer.IsEmpty()) {
        return JS_INVALID_REFERENCE;
      }
    } else {
      thisPointer = static_cast<Object*>(arguments[-1]);
    }

    if (callbackData->callback != nullptr) {
      Local<Object> holder;
      if (!callbackData->CheckSignature(*thisPointer,
                                        arguments, argumentCount, &holder)) {
        return JS_INVALID_REFERENCE;
      }

      FunctionCallbackInfo<Value> args(
        reinterpret_cast<Value**>(arguments),
        argumentCount - 1,
        thisPointer,
        holder,
        isConstructCall,
        callbackData->data,
        static_cast<Function*>(callee));

      callbackData->callback(args);
      Handle<Value> result = args.GetReturnValue().Get();

      // if this is a regualr function call return the result, otherwise this is
      // a constructor call return the new instance
      if (!isConstructCall) {
        return *result;
      } else if (!result.IsEmpty()) {
        if (!result->IsUndefined() && !result->IsNull()) {
          return *result;
        }
      }
    }

    // no CHAKRA_CALLBACK is attach just return the new instance
    return *thisPointer;
  }
};

class FunctionTemplateData : public TemplateData {
 public:
  static const ExternalDataTypes ExternalDataType =
    ExternalDataTypes::FunctionTemplateData;

 private:
  FunctionCallback callback;
  Persistent<Value> data;
  Persistent<Signature> signature;

  Persistent<ObjectTemplate> prototypeTemplate;
  Persistent<ObjectTemplate> instanceTemplate;
  Persistent<FunctionTemplate> parent;
  Persistent<Function> functionInstance;
  bool removePrototype;

 public:
  explicit FunctionTemplateData(FunctionCallback callback,
                                Local<Value> data,
                                Local<Signature> signature)
      : TemplateData(ExternalDataType),
        callback(callback),
        data(nullptr, data),
        signature(nullptr, signature),
        removePrototype(false) {
  }

  ~FunctionTemplateData() {
    data.Reset();
    signature.Reset();

    prototypeTemplate.Reset();
    instanceTemplate.Reset();
    parent.Reset();
    functionInstance.Reset();
  }

  static void CHAKRA_CALLBACK FinalizeCallback(void *data) {
    if (data != nullptr) {
      FunctionTemplateData * functionTemplateData =
        reinterpret_cast<FunctionTemplateData*>(data);
      delete functionTemplateData;
    }
  }

  void SetCallHandler(FunctionCallback callback, Local<Value> data) {
    this->callback = callback;
    this->data = data;
  }

  void SetRemovePrototype(bool removePrototype) {
    this->removePrototype = removePrototype;
  }

  void Inherit(Local<FunctionTemplate> parent) {
    this->parent = parent;
  }

  ObjectTemplate* EnsurePrototypeTemplate() {
    return Utils::EnsureObjectTemplate(&prototypeTemplate);
  }

  ObjectTemplate* EnsureInstanceTemplate() {
    return Utils::EnsureObjectTemplate(&instanceTemplate);
  }

  Function* EnsureFunction() {
    if (functionInstance.IsEmpty()) {
      FunctionCallbackData* callbackData =
        new FunctionCallbackData(callback, data, signature, instanceTemplate);

      JsValueRef funcCallbackObjectRef;
      JsErrorCode error = JsCreateExternalObject(
        callbackData,
        FunctionCallbackData::FinalizeCallback,
        &funcCallbackObjectRef);
      if (error != JsNoError) {
        delete callbackData;
        return nullptr;
      }

      Local<String> className = !instanceTemplate.IsEmpty() ?
        instanceTemplate->GetClassName() : Local<String>();

      JsValueRef function = nullptr;
      {
        if (!className.IsEmpty()) {
          error = JsCreateNamedFunction(*className,
                                        FunctionCallbackData::FunctionInvoked,
                                        funcCallbackObjectRef, &function);
        } else {
          error = JsCreateFunction(FunctionCallbackData::FunctionInvoked,
                                  funcCallbackObjectRef, &function);
        }

        if (error != JsNoError) {
          return nullptr;
        }
      }

      if (!removePrototype) {
        Local<Object> prototype;
        {
          IsolateShim* iso = IsolateShim::GetCurrent();
          prototype = !prototypeTemplate.IsEmpty() ?
            prototypeTemplate->NewInstance() : Object::New();

          // inherit from parent
          if (!parent.IsEmpty()) {
            JsValueRef parentPrototype;
            if (JsGetProperty(*parent->GetFunction(),
                              iso->GetCachedPropertyIdRef(
                                jsrt::CachedPropertyIdRef::prototype),
                              &parentPrototype) != JsNoError ||
                JsSetPrototype(*prototype,
                               parentPrototype) != JsNoError) {
              return nullptr;
            }
          }

          if (!className.IsEmpty()) {
            if (jsrt::DefineProperty(*prototype,
                iso->GetToStringTagSymbolPropertyIdRef(),
                jsrt::PropertyDescriptorOptionValues::True, /* writable */
                jsrt::PropertyDescriptorOptionValues::False, /* enumerable */
                jsrt::PropertyDescriptorOptionValues::True, /* configurable */
                *className,
                JS_INVALID_REFERENCE,
                JS_INVALID_REFERENCE) != JsNoError) {
              return nullptr;
            }
          }

          if (jsrt::DefineProperty(*prototype,
              iso->GetCachedPropertyIdRef(
                  jsrt::CachedPropertyIdRef::constructor),
              jsrt::PropertyDescriptorOptionValues::True, /* writable */
              jsrt::PropertyDescriptorOptionValues::False, /* enumerable */
              jsrt::PropertyDescriptorOptionValues::True, /* configurable */
              function, /* value */
              JS_INVALID_REFERENCE, /* getter */
              JS_INVALID_REFERENCE /* setter */)
              != JsNoError || JsSetProperty(function,
                            iso->GetCachedPropertyIdRef(
                              jsrt::CachedPropertyIdRef::prototype),
                            *prototype, false) != JsNoError) {
            return nullptr;
          }
        }

        callbackData->SavePrototype(prototype);
      }

      if (CopyPropertiesTo(function) != JsNoError) {
        return nullptr;
      }

      this->functionInstance = function;
    }

    return *this->functionInstance;
  }

  virtual JsValueRef NewInstance(JsValueRef templateRef) {
#ifdef DEBUG
    FunctionTemplateData* data = nullptr;
    CHAKRA_ASSERT(ExternalData::TryGet(templateRef, &data) && data == this);
#endif
    FunctionTemplate* functionTemplate =
      static_cast<FunctionTemplate*>(templateRef);
    return *functionTemplate->GetFunction();
  }
};

Local<FunctionTemplate> FunctionTemplate::New(Isolate* isolate,
                                              FunctionCallback callback,
                                              Local<Value> data,
                                              Local<Signature> signature,
                                              int length) {
  FunctionTemplateData * templateData =
    new FunctionTemplateData(callback, data, signature);
  JsValueRef functionTemplateRef;
  JsErrorCode error = JsCreateExternalObject(
    templateData, FunctionTemplateData::FinalizeCallback, &functionTemplateRef);
  if (error != JsNoError) {
    delete templateData;
    return Local<FunctionTemplate>();
  }
  return Local<FunctionTemplate>::New(functionTemplateRef);
}

MaybeLocal<Function> FunctionTemplate::GetFunction(Local<Context> context) {
  FunctionTemplateData* functionTemplateData = nullptr;
  if (!ExternalData::TryGet(this, &functionTemplateData)) {
    CHAKRA_ASSERT(false);  // This should never happen
    return Local<Function>();
  }

  return Local<Function>(functionTemplateData->EnsureFunction());
}

Local<Function> FunctionTemplate::GetFunction() {
  return FromMaybe(GetFunction(Local<Context>()));
}

Local<ObjectTemplate> FunctionTemplate::InstanceTemplate() {
  FunctionTemplateData* functionTemplateData = nullptr;
  if (!ExternalData::TryGet(this, &functionTemplateData)) {
    CHAKRA_ASSERT(false);  // This should never happen
    return Local<ObjectTemplate>();
  }

  return functionTemplateData->EnsureInstanceTemplate();
}

Local<ObjectTemplate> FunctionTemplate::PrototypeTemplate() {
  FunctionTemplateData* functionTemplateData = nullptr;
  if (!ExternalData::TryGet(this, &functionTemplateData)) {
    CHAKRA_ASSERT(false);  // This should never happen
    return Local<ObjectTemplate>();
  }

  return functionTemplateData->EnsurePrototypeTemplate();
}

void FunctionTemplate::SetClassName(Handle<String> name) {
  Local<ObjectTemplate> instanceTemplate = InstanceTemplate();
  if (!instanceTemplate.IsEmpty()) {
    instanceTemplate->SetClassName(name);
  }
}

void FunctionTemplate::SetHiddenPrototype(bool value) {
  // CHAKRA-TODO
}

void FunctionTemplate::SetCallHandler(FunctionCallback callback,
                                      Handle<Value> data) {
  FunctionTemplateData* functionTemplateData = nullptr;
  if (!ExternalData::TryGet(this, &functionTemplateData)) {
    CHAKRA_ASSERT(false);
    return;
  }

  functionTemplateData->SetCallHandler(callback, data);
}

bool FunctionTemplate::HasInstance(Handle<Value> object) {
  return jsrt::InstanceOf(*object, *GetFunction());
}

void FunctionTemplate::Inherit(Handle<FunctionTemplate> parent) {
  FunctionTemplateData* functionTemplateData = nullptr;
  if (!ExternalData::TryGet(this, &functionTemplateData)) {
    CHAKRA_ASSERT(false);
    return;
  }

  functionTemplateData->Inherit(parent);
}

void FunctionTemplate::RemovePrototype() {
  FunctionTemplateData* functionTemplateData = nullptr;
  if (!ExternalData::TryGet(this, &functionTemplateData)) {
    CHAKRA_ASSERT(false);
    return;
  }

  functionTemplateData->SetRemovePrototype(true);
}

}  // namespace v8
