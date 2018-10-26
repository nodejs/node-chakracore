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
  Local<PrimitiveArray> PrimitiveArray::New(Isolate* isolate, int length) {
    JsValueRef newArrayRef;

    if (JsCreateArray(length, &newArrayRef) != JsNoError) {
      return Local<PrimitiveArray>();
    }

    return Local<PrimitiveArray>::New(newArrayRef);
  }

  int PrimitiveArray::Length() const {
    unsigned int length = 0;
    jsrt::GetArrayLength((JsValueRef)this, &length);
    return length;
  }

  void PrimitiveArray::Set(int index, Local<Primitive> item) {
    PrimitiveArray::Set(nullptr, index, item);
  }

  Local<Primitive> PrimitiveArray::Get(int index) {
    return PrimitiveArray::Get(nullptr, index);
  }

  void PrimitiveArray::Set(Isolate* isolate, int index, Local<Primitive> item) {
    jsrt::SetIndexedProperty((JsValueRef)this, index, *item);
  }

  Local<Primitive> PrimitiveArray::Get(Isolate* isolate, int index) {
    JsValueRef value;

    if (jsrt::GetIndexedProperty((JsValueRef)this, index, &value) !=
        JsNoError) {
      return Local<Primitive>();
    }

    return Local<Primitive>::New(value);
  }
}  // namespace v8
