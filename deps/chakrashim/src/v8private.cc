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

#include "v8.h"
#include "jsrtutils.h"

namespace v8 {

Local<Value> Private::Name() const {
  return reinterpret_cast<const Symbol*>(this)->Name();
}

Local<Private> Private::New(Isolate* isolate,
                                   Local<String> name) {
  JsValueRef result;

  if (JsCreateSymbol(*name, &result) != JsNoError) {
    return Local<Private>();
  }

  return Local<Private>::New(result);
}

Local<Private> Private::ForApi(Isolate* isolate, Local<String> name) {
  jsrt::IsolateShim* iso = jsrt::IsolateShim::GetCurrent();
  jsrt::ContextShim* contextShim = iso->GetCurrentContextShim();

  JsValueRef getSymbolForFunction = contextShim->GetgetSymbolForFunction();
  JsValueRef symbolForKey;

  if (jsrt::CallFunction(getSymbolForFunction, (JsValueRef)*name,
                         &symbolForKey) != JsNoError) {
    return Local<Private>();
  }

  return Local<Private>(symbolForKey);
}

}  // namespace v8
