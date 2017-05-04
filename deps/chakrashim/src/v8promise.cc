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

Promise::Promise() { }

Local<Value> Promise::Result() {
  return Local<Value>();
}

Promise::PromiseState Promise::State() {
  return PromiseState::kFulfilled;
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

}  // namespace v8
