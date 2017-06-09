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

Local<String> Message::Get() const {
  // CHAKRA-TODO: Figure out what to do here
  return Local<String>();
}

MaybeLocal<String> Message::GetSourceLine(Local<Context> context) const {
  JsPropertyIdRef source = jsrt::IsolateShim::GetCurrent()
      ->GetCachedPropertyIdRef(jsrt::CachedPropertyIdRef::source);

  JsValueRef result;
  if (JsGetProperty((JsValueRef)this, source, &result) != JsNoError) {
    return Local<String>();
  }

  return Local<String>::New(result);
}

Local<String> Message::GetSourceLine() const {
  return FromMaybe(GetSourceLine(Local<Context>()));
}

ScriptOrigin Message::GetScriptOrigin() const {
  // CHAKRA-TODO: Figure out what to do here
  return ScriptOrigin(Local<String>());
}

Local<StackTrace> Message::GetStackTrace() const {
  // CHAKRA-TODO: Figure out what to do here
  return Local<StackTrace>();
}

Handle<Value> Message::GetScriptResourceName() const {
  JsPropertyIdRef url = jsrt::IsolateShim::GetCurrent()
      ->GetCachedPropertyIdRef(jsrt::CachedPropertyIdRef::url);

  JsValueRef result;
  if (JsGetProperty((JsValueRef)this, url, &result) != JsNoError) {
    return Local<Value>();
  }
  return Handle<Value>::New(result);
}

Maybe<int> Message::GetLineNumber(Local<Context> context) const {
  JsPropertyIdRef lineProp = jsrt::IsolateShim::GetCurrent()
      ->GetCachedPropertyIdRef(jsrt::CachedPropertyIdRef::line);

  JsValueRef result;
  if (JsGetProperty((JsValueRef)this, lineProp, &result) != JsNoError) {
    return Nothing<int>();
  }
  int line;
  if (jsrt::ValueToIntLikely(result, &line) != JsNoError) {
    return Nothing<int>();
  }
  return Just<int>(line + 1);
}

int Message::GetLineNumber() const {
  return FromMaybe(GetLineNumber(Local<Context>()));
}

Maybe<int> Message::GetStartColumn(Local<Context> context) const {
  JsPropertyIdRef columnProp = jsrt::IsolateShim::GetCurrent()
      ->GetCachedPropertyIdRef(jsrt::CachedPropertyIdRef::column);

  JsValueRef result;
  if (JsGetProperty((JsValueRef)this, columnProp, &result) != JsNoError) {
    return Nothing<int>();
  }
  int column;
  if (jsrt::ValueToIntLikely(result, &column) != JsNoError) {
    return Nothing<int>();
  }
  return Just<int>(column);
}

int Message::GetStartColumn() const {
  return FromMaybe(GetStartColumn(Local<Context>()));
}

Maybe<int> Message::GetEndColumn(Local<Context> context) const {
  int column = GetStartColumn();
  JsPropertyIdRef lengthProp = jsrt::IsolateShim::GetCurrent()
      ->GetCachedPropertyIdRef(jsrt::CachedPropertyIdRef::length);


  JsValueRef result;
  if (JsGetProperty((JsValueRef)this, lengthProp, &result) != JsNoError) {
    return Nothing<int>();
  }
  int length;
  if (jsrt::ValueToIntLikely(result, &length) != JsNoError) {
    return Nothing<int>();
  }

  return Just<int>(column + length + 1);
}

int Message::GetEndColumn() const {
  return FromMaybe(GetEndColumn(Local<Context>()));
}

}  // namespace v8
