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

MaybeLocal<Object> ValueDeserializer::Delegate::ReadHostObject(
    Isolate* isolate) {
  CHAKRA_UNIMPLEMENTED();
  return Local<Object>();
}

ValueDeserializer::ValueDeserializer(Isolate* isolate, const uint8_t* data,
                                     size_t size, Delegate* delegate) {
  CHAKRA_UNIMPLEMENTED();
}

ValueDeserializer::~ValueDeserializer() {
  // Intentionally left empty to suppress warning C4722.
}

Maybe<bool> ValueDeserializer::ReadHeader(Local<Context> context) {
  CHAKRA_UNIMPLEMENTED();
  return Nothing<bool>();
}

MaybeLocal<Value> ValueDeserializer::ReadValue(Local<Context> context) {
  CHAKRA_UNIMPLEMENTED();
  return Local<Value>();
}


void ValueDeserializer::TransferArrayBuffer(uint32_t transfer_id,
                                            Local<ArrayBuffer> array_buffer) {
  CHAKRA_UNIMPLEMENTED();
}

void ValueDeserializer::TransferSharedArrayBuffer(
    uint32_t id, Local<SharedArrayBuffer> shared_array_buffer) {
  CHAKRA_UNIMPLEMENTED();
}

uint32_t ValueDeserializer::GetWireFormatVersion() const {
  CHAKRA_UNIMPLEMENTED();
  return 0;
}

bool ValueDeserializer::ReadUint32(uint32_t* value) {
  CHAKRA_UNIMPLEMENTED();
  return false;
}

bool ValueDeserializer::ReadUint64(uint64_t* value) {
  CHAKRA_UNIMPLEMENTED();
  return false;
}

bool ValueDeserializer::ReadDouble(double* value) {
  CHAKRA_UNIMPLEMENTED();
  return false;
}

bool ValueDeserializer::ReadRawBytes(size_t length, const void** data) {
  CHAKRA_UNIMPLEMENTED();
  return false;
}

}  // namespace v8
