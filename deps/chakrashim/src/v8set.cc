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
  JsValueRef setConstructor =
    ContextShim::GetCurrent()->GetSetConstructor();

  JsValueRef newSetRef;
  if (jsrt::ConstructObject(setConstructor, &newSetRef) != JsNoError) {
    CHAKRA_ASSERT(false);
    return Local<Set>();
  }

  return Local<Set>::New(newSetRef);
}

MaybeLocal<Set> Set::Add(Local<Context>, Local<Value> key) {
  JsValueRef setAddFunction =
    ContextShim::GetCurrent()->GetSetAddFunction();

  JsValueRef setAddResult;
  JsValueRef args[] = { (JsValueRef)this, *key };
  if (JsCallFunction(setAddFunction, args, _countof(args),
                     &setAddResult) != JsNoError) {
    CHAKRA_ASSERT(false);
    return MaybeLocal<Set>();
  }

  return Local<Set>::New(setAddResult);
}

Maybe<bool> Set::Delete(Local<Context>, Local<Value>) {
  // CHAKRA-TODO: Figure out what to do here
  CHAKRA_ASSERT(false);
  return Nothing<bool>();
}

Local<Array> Set::AsArray() const {
  JsValueRef arrayConstructor =
      ContextShim::GetCurrent()->GetArrayConstructor();

  JsValueRef arrayFromFunction;
  if (jsrt::GetProperty(arrayConstructor, jsrt::CachedPropertyIdRef::from,
                        &arrayFromFunction) != JsNoError) {
    CHAKRA_ASSERT(false);
    return Local<Array>();
  }

  JsValueRef arrayFromResult;
  if (jsrt::CallFunction(arrayFromFunction, (JsValueRef)this,
                         &arrayFromResult) != JsNoError) {
    CHAKRA_ASSERT(false);
    return Local<Array>();
  }

  return Local<Array>::New(arrayFromResult);
}

}  // namespace v8
