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

#ifndef DEPS_CHAKRASHIM_SRC_JSRTINSPECTORHELPERS_H_
#define DEPS_CHAKRASHIM_SRC_JSRTINSPECTORHELPERS_H_

#include <v8.h>

namespace jsrt {

class InspectorHelpers {
 public:
  static JsErrorCode TryCopyProperty(JsValueRef sourceObj,
                                     JsPropertyIdRef sourceIdRef,
                                     JsValueRef destObj,
                                     JsPropertyIdRef destIdRef,
                                     bool* wasCopied = nullptr);
  static JsErrorCode TryCopyPropertyString(JsValueRef sourceObj,
                                           JsPropertyIdRef sourceIdRef,
                                           JsValueRef destObj,
                                           JsPropertyIdRef destIdRef,
                                           bool* wasCopied = nullptr);

  static v8::Local<v8::Value> WrapEvaluateObject(JsValueRef sourceObject);
  static v8::Local<v8::Object> WrapCallFrameDetails(JsValueRef callFrame);
  static v8::Local<v8::Value> WrapRuntimeException(JsValueRef eventData);
  static v8::Local<v8::Value> WrapPropertiesArray(JsValueRef properties);
  static v8::Local<v8::Value> GetWrappedProperties(int handle);
  static v8::Local<v8::Value> GetWrappedStackLocals(JsValueRef stackProperties);

  static v8::Local<v8::Value> EvaluateOnCallFrame(int ordinal,
                                                  JsValueRef expression,
                                                  bool returnByValue,
                                                  bool* isError = nullptr);
  static v8::Local<v8::Value> EvaluateOnCallFrame(JsValueRef callFrame,
                                                  JsValueRef expression,
                                                  bool returnByValue,
                                                  bool* isError = nullptr);

  static v8::Local<v8::Value> GetScriptSource(unsigned int scriptId);

  static JsRuntimeHandle GetRuntimeFromIsolate(v8::Isolate* isolate);

 private:
  static void WrapObject(JsValueRef obj, JsValueRef* wrappedObj);
  static void WrapProperty(JsValueRef property, JsValueRef* wrappedProperty);
  static v8::Local<v8::Value> WrapException(JsValueRef exception);
};

}  // namespace jsrt

#endif  // DEPS_CHAKRASHIM_SRC_JSRTINSPECTORHELPERS_H_
