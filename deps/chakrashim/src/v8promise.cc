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

#include <array>

namespace v8 {

class PromiseResolverData : public ExternalData {
 public:
  static const ExternalDataTypes ExternalDataType =
    ExternalDataTypes::PromiseResolverData;

 private:
  Persistent<Value> resolve;
  Persistent<Value> reject;

 public:
  PromiseResolverData(Local<Value> resolve,
                      Local<Value> reject)
      : ExternalData(ExternalDataType),
        resolve(nullptr, resolve),
        reject(nullptr, reject) {}

  ~PromiseResolverData() {
    this->resolve.Reset();
    this->reject.Reset();
  }

  static void CHAKRA_CALLBACK FinalizeCallback(void *data) {
    if (data != nullptr) {
      PromiseResolverData* promiseResolverData =
          reinterpret_cast<PromiseResolverData*>(data);
      delete promiseResolverData;
    }
  }

  JsErrorCode Resolve(Local<Value> value) {
    JsValueRef result = nullptr;
    std::array<JsValueRef, 2> args = { jsrt::GetUndefined(), *value };
    return JsCallFunction(*(this->resolve), args.data(), args.size(), &result);
  }

  JsErrorCode Reject(Local<Value> value) {
    JsValueRef result = nullptr;
    std::array<JsValueRef, 2> args = { jsrt::GetUndefined(), *value };
    return JsCallFunction(*(this->reject), args.data(), args.size(), &result);
  }
};

Promise::Promise() { }

Local<Value> Promise::Result() {
  JsValueRef result = JS_INVALID_REFERENCE;
  if (JsGetPromiseResult(reinterpret_cast<JsValueRef>(this),
                         &result) != JsNoError) {
    return Local<Value>();
  }

  return result;
}

Promise::PromiseState Promise::State() {
  JsPromiseState state = JsPromiseStatePending;
  if (JsGetPromiseState(reinterpret_cast<JsValueRef>(this),
                        &state) != JsNoError) {
    return PromiseState::kPending;
  }

  switch (state) {
    case JsPromiseStateFulfilled:
      return PromiseState::kFulfilled;

    case JsPromiseStateRejected:
      return PromiseState::kRejected;

    default:
      return PromiseState::kPending;
  }
}

MaybeLocal<Promise> Promise::Then(Local<Context> context,
                                  Local<Function> handler) {
  CHAKRA_ASSERT(false);
  return Local<Promise>();
}

MaybeLocal<Promise> Promise::Catch(Local<Context> context,
                                   Local<Function> handler) {
  CHAKRA_ASSERT(false);
  return Local<Promise>();
}

Promise* Promise::Cast(Value* obj) {
  CHAKRA_ASSERT(obj->IsPromise());
  return static_cast<Promise*>(obj);
}

MaybeLocal<Promise::Resolver> Promise::Resolver::New(Local<Context> context) {
  JsValueRef promise, resolve, reject;
  if (JsCreatePromise(&promise, &resolve, &reject) != JsNoError) {
    return Local<Promise::Resolver>();
  }

  PromiseResolverData* data = new PromiseResolverData(resolve, reject);
  if (jsrt::AddExternalData(
          promise, data, PromiseResolverData::FinalizeCallback) != JsNoError) {
    delete data;
    return Local<Promise::Resolver>();
  }

  return Local<Promise::Resolver>::New(promise);
}

Local<Promise::Resolver> Promise::Resolver::New(Isolate* isolate) {
  return New(isolate->GetCurrentContext()).ToLocalChecked();
}

Local<Promise> Promise::Resolver::GetPromise() {
  return Local<Promise>::New(static_cast<JsValueRef>(this));
}

Promise::Resolver* Promise::Resolver::Cast(Value* obj) {
  CHAKRA_ASSERT(obj->IsPromise());
  return static_cast<Promise::Resolver*>(obj);
}

Maybe<bool> Promise::Resolver::Resolve(Local<Context> context,
                                       Local<Value> value) {
  PromiseResolverData* data = nullptr;
  if (!ExternalData::TryGetFromProperty(this, jsrt::GetExternalPropertyId(),
                                        &data)) {
    return Nothing<bool>();
  }

  if (data->Resolve(value) != JsNoError) {
    return Nothing<bool>();
  }

  return Just(true);
}

void Promise::Resolver::Resolve(Local<Value> value) {
  Local<Context> context;
  Resolve(context, value);
}

Maybe<bool> Promise::Resolver::Reject(Local<Context> context,
                                      Local<Value> value) {
  PromiseResolverData* data = nullptr;
  if (!ExternalData::TryGetFromProperty(this, jsrt::GetExternalPropertyId(),
                                        &data)) {
    return Nothing<bool>();
  }

  if (data->Reject(value) != JsNoError) {
    return Nothing<bool>();
  }

  return Just(true);
}

}  // namespace v8
