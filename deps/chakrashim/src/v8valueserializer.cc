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

Maybe<bool> ValueSerializer::Delegate::WriteHostObject(Isolate* isolate,
                                                       Local<Object> object) {
  CHAKRA_UNIMPLEMENTED();
  return Nothing<bool>();
}

Maybe<uint32_t> ValueSerializer::Delegate::GetSharedArrayBufferId(
    Isolate* isolate, Local<SharedArrayBuffer> shared_array_buffer) {
  CHAKRA_UNIMPLEMENTED();
  return Nothing<unsigned int>();
}

void* ValueSerializer::Delegate::ReallocateBufferMemory(void* old_buffer,
                                                        size_t size,
                                                        size_t* actual_size) {
  CHAKRA_UNIMPLEMENTED();
  return nullptr;
}

void ValueSerializer::Delegate::FreeBufferMemory(void* buffer) {
  CHAKRA_UNIMPLEMENTED();
}

ValueSerializer::ValueSerializer(Isolate* isolate) {
  CHAKRA_UNIMPLEMENTED();
}

ValueSerializer::ValueSerializer(Isolate* isolate, Delegate* delegate) {
  CHAKRA_UNIMPLEMENTED();
}

ValueSerializer::~ValueSerializer() {
  // Intentionally left empty to suppress warning C4722.
}

void ValueSerializer::WriteHeader() {
  CHAKRA_UNIMPLEMENTED();
}

Maybe<bool> ValueSerializer::WriteValue(Local<Context> context,
                                        Local<Value> value) {
  CHAKRA_UNIMPLEMENTED();
  return Nothing<bool>();
}

std::pair<uint8_t*, size_t> ValueSerializer::Release() {
  CHAKRA_UNIMPLEMENTED();
  return std::pair<uint8_t*, size_t>();
}

void ValueSerializer::TransferArrayBuffer(uint32_t transfer_id,
                                          Local<ArrayBuffer> array_buffer) {
  CHAKRA_UNIMPLEMENTED();
}

void ValueSerializer::SetTreatArrayBufferViewsAsHostObjects(bool mode) {
  CHAKRA_UNIMPLEMENTED();
}

void ValueSerializer::WriteUint32(uint32_t value) {
  CHAKRA_UNIMPLEMENTED();
}

void ValueSerializer::WriteUint64(uint64_t value) {
  CHAKRA_UNIMPLEMENTED();
}

void ValueSerializer::WriteDouble(double value) {
  CHAKRA_UNIMPLEMENTED();
}

void ValueSerializer::WriteRawBytes(const void* source, size_t length) {
  CHAKRA_UNIMPLEMENTED();
}

}  // namespace v8
