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
    return Local<Resolver>();
  }

  Local<Resolver> Resolver::New(Isolate* isolate) {
    return New(isolate->GetCurrentContext()).ToLocalChecked();
  }

  Local<Promise> Resolver::GetPromise() {
    CHAKRA_ASSERT(false);
    return Local<Promise>();
  }

  Resolver* Resolver::Cast(Value* obj) {
    return static_cast<Resolver*>(obj);
  }

  Maybe<bool> Resolver::Resolve(Local<Context> context, Local<Value> value) {
    return Just(false);
  }

  void Resolver::Resolve(Local<Value> value) {
    Local<Context> context;
    Resolve(context, value);
  }

  Maybe<bool> Resolver::Reject(Local<Context> context, Local<Value> value) {
    return Just(false);
  }
}  // namespace v8
