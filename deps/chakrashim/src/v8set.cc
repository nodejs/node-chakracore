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

Local<Set> Set::New(Isolate* isolate) {
  // CHAKRA-TODO: Figure out what to do here
  //
  // kpathak: chakra_shim.js
  CHAKRA_ASSERT(false);
  return Local<Set>();
}

MaybeLocal<Set> Set::Add(Local<Context>, Local<Value>) {
  // CHAKRA-TODO: Figure out what to do here
  CHAKRA_ASSERT(false);
  return MaybeLocal<Set>();
}

Maybe<bool> Set::Delete(Local<Context>, Local<Value>) {
  // CHAKRA-TODO: Figure out what to do here
  CHAKRA_ASSERT(false);
  return Nothing<bool>();
}

Local<Array> Set::AsArray() const {
  // CHAKRA-TODO: Figure out what to do here
  CHAKRA_ASSERT(false);
  return Local<Array>();
}

}  // namespace v8
