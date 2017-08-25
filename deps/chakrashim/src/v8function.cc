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

using jsrt::IsolateShim;
using jsrt::ContextShim;
using jsrt::PropertyDescriptorOptionValues;
using jsrt::DefineProperty;

MaybeLocal<Function> Function::New(Local<Context> context,
                                   FunctionCallback callback,
                                   Local<Value> data,
                                   int length,
                                   ConstructorBehavior behavior) {
  Local<FunctionTemplate> funcTemplate = FunctionTemplate::New(
    IsolateShim::GetCurrentAsIsolate(),
    callback,
    data,
    Local<Signature>(),
    length);

  if (behavior == ConstructorBehavior::kThrow) {
    funcTemplate->RemovePrototype();
  }

  return Local<Function>(funcTemplate->GetFunction());
}

MaybeLocal<Object> Function::NewInstance(Local<Context> context,
                                         int argc, Handle<Value> argv[]) const {
  jsrt::JsArguments<> args(argc + 1);
  args[0] = jsrt::GetUndefined();

  if (argc > 0) {
    for (int i = 0; i < argc; i++) {
      args[i + 1] = *argv[i];
    }
  }

  JsValueRef newInstance;
  if (JsConstructObject((JsValueRef)this,
                        args, argc + 1, &newInstance) != JsNoError) {
    return Local<Object>();
  }

  return Local<Object>::New(newInstance);
}

Local<Object> Function::NewInstance(int argc, Handle<Value> argv[]) const {
  return FromMaybe(NewInstance(Local<Context>(), argc, argv));
}

Local<Object> Function::NewInstance() const {
  return NewInstance(0, nullptr);
}

MaybeLocal<Value> Function::Call(Local<Context> context,
                                 Handle<Value> recv, int argc,
                                 Handle<Value> argv[]) {
  IsolateShim::GetCurrent()->SetScriptExecuted();

  jsrt::JsArguments<> args(argc + 1);
  args[0] = *recv;

  for (int i = 0; i < argc; i++) {
    args[i + 1] = *argv[i];
  }

  JsValueRef result;
  {
    TryCatch tryCatch;
    tryCatch.SetNonUser();
    if (JsCallFunction((JsValueRef)this, args, argc + 1,
                       &result) != JsNoError) {
      tryCatch.CheckReportExternalException();
      return Local<Value>();
    }
  }
  return Local<Value>::New(result);
}

Local<Value> Function::Call(Handle<Value> recv,
                            int argc, Handle<Value> argv[]) {
  return FromMaybe(Call(Local<Context>(), recv, argc, argv));
}

void Function::SetName(Handle<String> name) {
  JsErrorCode error = jsrt::DefineProperty((JsValueRef)this,
                 IsolateShim::GetCurrent()->GetCachedPropertyIdRef(
                   jsrt::CachedPropertyIdRef::name),
                 PropertyDescriptorOptionValues::False,
                 PropertyDescriptorOptionValues::False,
                 PropertyDescriptorOptionValues::False,
                 (JsValueRef)*name,
                 JS_INVALID_REFERENCE,
                 JS_INVALID_REFERENCE);
  CHAKRA_ASSERT(error == JsNoError);
  UNUSED(error);
}

Local<Value> Function::GetName() const {
  // CHAKRA-TODO: Figure out what to do here
  //
  // kpathak: no JSrt API to do this. probably do it in chakra_shim.js?
  CHAKRA_ASSERT(false);
  return Local<Value>();
}

Local<Value> Function::GetInferredName() const {
  // CHAKRA-TODO: Figure out what to do here
  CHAKRA_ASSERT(false);
  return Local<Value>();
}

Local<Value> Function::GetDebugName() const {
  // CHAKRA-TODO: Figure out what to do here
  CHAKRA_ASSERT(false);
  return Local<Value>();
}

const int Function::kLineOffsetNotFound = -1;

int Function::GetScriptLineNumber() const {
  // CHAKRA-TODO: Figure out what to do here
  CHAKRA_ASSERT(false);
  return 0;
}

int Function::GetScriptColumnNumber() const {
  // CHAKRA-TODO: Figure out what to do here
  CHAKRA_ASSERT(false);
  return 0;
}

int Function::ScriptId() const {
  // CHAKRA-TODO: Figure out what to do here
  CHAKRA_ASSERT(false);
  return 0;
}

Local<Value> Function::GetBoundFunction() const {
  CHAKRA_ASSERT(false);
  return Local<Value>();
}

Function *Function::Cast(Value *obj) {
  CHAKRA_ASSERT(obj->IsFunction());
  return static_cast<Function*>(obj);
}

}  // namespace v8
