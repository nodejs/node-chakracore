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
  using Resolver = Promise::Resolver;
  // CHAKRA-TODO: Unimplemented completely
  MaybeLocal<Resolver> Resolver::New(Local<Context> context) {
    JsValueRef resolver;
    if (JsCreateObject(&resolver) != JsNoError) {
      return Local<Resolver>();
    }

    JsValueRef promise, resolve, reject;
    if (JsCreatePromise(&promise, &resolve, &reject) != JsNoError) {
      return Local<Resolver>();
    }

    if (jsrt::SetProperty(resolver, jsrt::CachedPropertyIdRef::promise, promise) != JsNoError) {
      return Local<Resolver>();
    }

    if (jsrt::SetProperty(resolver, jsrt::CachedPropertyIdRef::resolve, resolve) != JsNoError) {
      return Local<Resolver>();
    }

    if (jsrt::SetProperty(resolver, jsrt::CachedPropertyIdRef::reject, reject) != JsNoError) {
      return Local<Resolver>();
    }

    JsValueRef state;
    jsrt::UintToValue(0, &state);
    if (jsrt::SetProperty(promise, jsrt::CachedPropertyIdRef::value, state) != JsNoError) {
      return Local<Resolver>();
    }

    return Local<Resolver>::New(static_cast<Resolver*>(resolver));
  }

  Local<Resolver> Resolver::New(Isolate* isolate) {
    return New(isolate->GetCurrentContext()).ToLocalChecked();
  }

  Local<Promise> Resolver::GetPromise() {
    JsValueRef promise;
    if (jsrt::GetProperty(this, jsrt::CachedPropertyIdRef::promise, &promise) != JsNoError) {
      return Local<Promise>();
    }
    return Local<Promise>::New(static_cast<Promise*>(promise));
  }

  Resolver* Resolver::Cast(Value* obj) {
    return static_cast<Resolver*>(obj);
  }

  Maybe<bool> Resolver::Resolve(Local<Context> context, Local<Value> value) {
    JsValueRef resolve;
    if (jsrt::GetProperty(this, jsrt::CachedPropertyIdRef::resolve, &resolve) != JsNoError) {
      return Nothing<bool>();
    }

    JsValueRef promise;
    if (jsrt::GetProperty(this, jsrt::CachedPropertyIdRef::promise, &promise) != JsNoError) {
      return Nothing<bool>();
    }

    if (jsrt::SetProperty(promise, jsrt::CachedPropertyIdRef::value, *value) != JsNoError) {
      return Nothing<bool>();
    }

    JsValueRef state;
    jsrt::UintToValue(1, &state);
    if (jsrt::SetProperty(promise, jsrt::CachedPropertyIdRef::value, state) != JsNoError) {
      return Nothing<bool>();
    }
    
    JsValueRef result;
    JsValueRef args[2];
    args[0] = this; // ? What is the "this" of the resolver here?
    args[1] = reinterpret_cast<JsValueRef>(*value);

    if (JsCallFunction(resolve, args, 2, &result) != JsNoError) {
      return Nothing<bool>();
    }

    return Just(true);
  }

  void Resolver::Resolve(Local<Value> value) {
    Local<Context> context;
    Resolve(context, value);
  }

  Maybe<bool> Resolver::Reject(Local<Context> context, Local<Value> value) {
    JsValueRef reject;
    if (jsrt::GetProperty(this, jsrt::CachedPropertyIdRef::reject, &reject) != JsNoError) {
      return Nothing<bool>();
    }

    JsValueRef promise;
    if (jsrt::GetProperty(this, jsrt::CachedPropertyIdRef::promise, &promise) != JsNoError) {
      return Nothing<bool>();
    }

    if (jsrt::SetProperty(promise, jsrt::CachedPropertyIdRef::value, *value) != JsNoError) {
      return Nothing<bool>();
    }

    JsValueRef state;
    jsrt::UintToValue(2, &state);
    if (jsrt::SetProperty(promise, jsrt::CachedPropertyIdRef::value, state) != JsNoError) {
      return Nothing<bool>();
    }
    
    JsValueRef result;
    JsValueRef args[2];
    args[0] = this; // ? What is the "this" of the resolver here?
    args[1] = reinterpret_cast<JsValueRef>(*value);

    if (JsCallFunction(reject, args, 2, &result) != JsNoError) {
      return Nothing<bool>();
    }

    return Just(true);
  }
}  // namespace v8
