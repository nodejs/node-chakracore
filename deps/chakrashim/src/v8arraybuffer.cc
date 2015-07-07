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
using jsrt::CachedPropertyIdRef;

Local<ArrayBuffer> ArrayBuffer::New(Isolate* isolate, size_t byte_length) {
  return New(isolate, nullptr, byte_length);
}

Local<ArrayBuffer> ArrayBuffer::New(Isolate* isolate,
                                    void* data, size_t byte_length,
                                    ArrayBufferCreationMode mode) {
  JsValueRef result;
  if (JsCreateArrayBuffer(data,
                          static_cast<unsigned int>(byte_length),
                          &result) != JsNoError) {
    return Local<ArrayBuffer>();
  }
  return static_cast<ArrayBuffer*>(result);
}

size_t ArrayBuffer::ByteLength() const {
  int result;
  if (jsrt::GetProperty(const_cast<ArrayBuffer*>(this),
                        IsolateShim::GetCurrent()->GetCachedPropertyIdRef(
                          CachedPropertyIdRef::byteLength),
                        &result) != JsNoError) {
    return 0;
  }
  return result;
}

void ArrayBuffer::Neuter() {
  // Chakra: not supported, ignore
}

ArrayBuffer::Contents ArrayBuffer::GetContents() {
  BYTE* buffer;
  unsigned int bufferLength;
  if (JsGetArrayBufferStorage(this, &buffer, &bufferLength) != JsNoError) {
    return Contents();
  }

  Contents contents;
  contents.data_ = buffer;
  contents.byte_length_ = bufferLength;
  return contents;
}

ArrayBuffer* ArrayBuffer::Cast(Value* obj) {
  return static_cast<ArrayBuffer*>(obj);
}

}  // namespace v8
