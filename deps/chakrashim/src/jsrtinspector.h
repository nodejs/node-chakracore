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

#ifndef DEPS_CHAKRASHIM_SRC_JSRTINSPECTOR_H_
#define DEPS_CHAKRASHIM_SRC_JSRTINSPECTOR_H_

#include <v8.h>

namespace jsrt {

class InspectorBreakQueue;

class Inspector {
 public:
  static bool IsInspectorEnabled();
  static bool IsReplayDebugEnabled();
  static void StartDebugging(JsRuntimeHandle runtime);
  static void RequestAsyncBreak(JsRuntimeHandle runtime,
                                v8::InterruptCallback callback,
                                void* data);
  static void RequestAsyncBreak(v8::Isolate* isolate);
  static void SetChakraDebugObject(JsValueRef chakraDebugObject);
  static void SetDebugEventHandler(JsDiagDebugEventCallback callback,
                                   void* callbackState);
  static void RemoveBreakpoint(unsigned int breakpointId);
  static void ClearBreakpoints();

 private:
  static void CHAKRA_CALLBACK JsDiagDebugEventHandler(
    JsDiagDebugEvent debugEvent,
    JsValueRef eventData,
    void* callbackState);
  static void InstallHostCallback(JsValueRef chakraDebugObject,
    const char *name,
    JsNativeFunction nativeFunction);

  static JsDiagDebugEventCallback s_callback;
  static void* s_callbackState;
  static std::unique_ptr<InspectorBreakQueue> s_breakQueue;
};
}  // namespace jsrt

#endif  // DEPS_CHAKRASHIM_SRC_JSRTINSPECTOR_H_
