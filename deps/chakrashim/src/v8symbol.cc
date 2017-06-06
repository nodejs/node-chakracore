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
#include "v8chakra.h"
#include "jsrtutils.h"

namespace v8 {

Symbol::Symbol() { }

Local<Symbol> Symbol::New(Isolate* isolate, Local<String> name) {
  return Local<Symbol>::New(isolate, From(name));
}

Local<Symbol> Symbol::From(Local<String> name) {
  JsValueRef sym;
  JsValueRef description = (JsValueRef)*name;

  if (JsCreateSymbol(description, &sym) != JsNoError) {
    return Local<Symbol>();
  }

  return Local<Symbol>::New(static_cast<Symbol*>(sym));
}

Symbol* Symbol::Cast(Value* obj) {
  CHAKRA_ASSERT(obj->IsSymbol());
  return static_cast<Symbol*>(obj);
}

Local<Value> Symbol::Name() const {
  jsrt::IsolateShim* iso = jsrt::IsolateShim::GetCurrent();
  jsrt::ContextShim* contextShim = iso->GetCurrentContextShim();

  JsValueRef getSymbolKeyForFunction =
    contextShim->GetgetSymbolKeyForFunction();
  JsValueRef symbolDescription;

  if (jsrt::CallFunction(getSymbolKeyForFunction, (JsValueRef)this,
                         &symbolDescription) != JsNoError) {
    return Local<Value>();
  }

  return Local<Value>(symbolDescription);
}

}  // namespace v8
