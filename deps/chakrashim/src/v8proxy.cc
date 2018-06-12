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
Local<Object> Proxy::GetTarget() {
  JsValueRef returnValue = JS_INVALID_REFERENCE;
  bool isProxy = false;
  JsGetProxyProperties((JsValueRef) this, &isProxy, &returnValue, nullptr);
  if (!isProxy || returnValue == JS_INVALID_REFERENCE) {
    returnValue = jsrt::GetUndefined();
  }
  return Local<Object>::New(returnValue);
}

Local<Value> Proxy::GetHandler() {
  JsValueRef returnValue = JS_INVALID_REFERENCE;
  bool isProxy = false;
  JsGetProxyProperties((JsValueRef) this, &isProxy, nullptr, &returnValue);
  if (!isProxy || returnValue == JS_INVALID_REFERENCE) {
    returnValue = jsrt::GetUndefined();
  }
  return Local<Object>::New(returnValue);
}

Proxy* Proxy::Cast(v8::Value* obj) {
  CHAKRA_ASSERT(obj->IsProxy());
  return static_cast<Proxy*>(obj);
}
}  // namespace v8
