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
  Local<Value> JSON::Parse(Local<String> json_string) {
    v8::Isolate* iso = jsrt::IsolateShim::GetCurrentAsIsolate();
    return FromMaybe(JSON::Parse(iso, json_string));
  }

  MaybeLocal<Value> JSON::Parse(Isolate* isolate,
                                Local<String> json_string) {
    Local<Context> context = isolate->GetCurrentContext();
    return JSON::Parse(context, json_string);
  }

  MaybeLocal<Value> JSON::Parse(Local<Context> context,
                                Local<String> json_string) {
    jsrt::ContextShim *contextShim = jsrt::IsolateShim::GetContextShim(
        (JsContextRef)*context);

    JsValueRef jsonParseFunction = contextShim->GetjsonParseFunction();
    JsValueRef obj = JS_INVALID_REFERENCE;

    if (jsrt::CallFunction(jsonParseFunction, (JsValueRef)*json_string,
                           &obj) != JsNoError) {
      return Local<Value>();
    }

    return Local<Value>::New(obj);
  }

  MaybeLocal<String> JSON::Stringify(Local<Context> context,
                                     Local<Object> json_object,
                                     Local<String> gap) {
    jsrt::ContextShim *contextShim = jsrt::IsolateShim::GetContextShim(
        (JsContextRef)*context);

    JsValueRef jsonStringifyFunction = contextShim->GetjsonStringifyFunction();
    JsValueRef str = JS_INVALID_REFERENCE;
    JsValueRef actualGap =
        gap.IsEmpty() ? jsrt::GetUndefined() : static_cast<JsValueRef>(*gap);

    if (jsrt::CallFunction(jsonStringifyFunction, (JsValueRef)*json_object,
        jsrt::GetUndefined(), actualGap, &str) != JsNoError) {
      return Local<String>();
    }

    return Local<String>::New(str);
  }
}  // namespace v8
