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
using jsrt::ContextShim;
using jsrt::CachedPropertyIdRef;

Local<ArrayBuffer> ArrayBufferView::Buffer() {
  JsValueRef result;
  if (JsGetTypedArrayInfo(this,
                          nullptr, &result, nullptr, nullptr) != JsNoError) {
    return Local<ArrayBuffer>();
  }
  return static_cast<ArrayBuffer*>(result);
}

size_t ArrayBufferView::ByteOffset() {
  unsigned int result;
  if (JsGetTypedArrayInfo(this,
                          nullptr, nullptr, &result, nullptr) != JsNoError) {
    return 0;
  }
  return result;
}

size_t ArrayBufferView::ByteLength() {
  unsigned int result;
  if (JsGetTypedArrayInfo(this,
                          nullptr, nullptr, nullptr, &result) != JsNoError) {
    return 0;
  }
  return result;
}

static JsErrorCode NewTypedArray(ContextShim::GlobalType constructorIndex,
                                 Handle<ArrayBuffer> array_buffer,
                                 size_t byte_offset, size_t length,
                                 JsValueRef* result) {
  Isolate* iso = Isolate::GetCurrent();
  HandleScope handleScope(iso);

  ContextShim* contextShim = ContextShim::GetCurrent();
  JsValueRef constructor = contextShim->GetGlobalType(constructorIndex);
  JsValueRef args[4] = {
    contextShim->GetUndefined(),
    *array_buffer,
    *Uint32::NewFromUnsigned(iso, static_cast<uint32_t>(byte_offset)),
    *Uint32::NewFromUnsigned(iso, static_cast<uint32_t>(length)),
  };
  return JsConstructObject(constructor, args, _countof(args), result);
}

template <class T>
Local<T> NewTypedArray(ContextShim::GlobalType constructorIndex,
                       Handle<ArrayBuffer> array_buffer,
                       size_t byte_offset, size_t length) {
  JsValueRef result;
  if (NewTypedArray(constructorIndex, array_buffer, byte_offset, length,
                    &result) != JsNoError) {
    return Local<T>();
  }
  return static_cast<T*>(result);
}

Local<Uint8Array> Uint8Array::New(Handle<ArrayBuffer> array_buffer,
                                  size_t byte_offset, size_t length) {
  return NewTypedArray<Uint8Array>(ContextShim::GlobalType::Uint8Array,
                                   array_buffer, byte_offset, length);
}

Uint8Array* Uint8Array::Cast(Value* obj) {
  CHAKRA_ASSERT(obj->IsUint8Array());
  return static_cast<Uint8Array*>(obj);
}

Local<Uint32Array> Uint32Array::New(Handle<ArrayBuffer> array_buffer,
                                    size_t byte_offset, size_t length) {
  return NewTypedArray<Uint32Array>(ContextShim::GlobalType::Uint32Array,
                                   array_buffer, byte_offset, length);
}

Uint32Array* Uint32Array::Cast(Value* obj) {
  // CHAKRA-TODO: CHAKRA_ASSERT(obj->IsUint32Array());
  return static_cast<Uint32Array*>(obj);
}

}  // namespace v8
