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

#include <string>

#include "v8chakra.h"
#include "jsrtinspector.h"
#include "jsrtinspectorhelpers.h"

namespace v8 {
  extern THREAD_LOCAL bool g_EnableInspector;
  extern THREAD_LOCAL bool g_EnableReplayDebug;
}

namespace jsrt {

static JsValueRef CHAKRA_CALLBACK Log(
    JsValueRef callee,
    bool isConstructCall,
    JsValueRef *arguments,
    unsigned short argumentCount,  // NOLINT(runtime/int)
    void *callbackState) {
  JsValueRef scriptRef;
  jsrt::StringUtf8 script;

  if (argumentCount >= 2 &&
      jsrt::ToString(arguments[1], &scriptRef, &script) == JsNoError) {
    fprintf(stdout, "%s\n", *script);
#ifdef DEBUG
    fflush(stdout);
#endif
  }

  return JS_INVALID_REFERENCE;
}

static JsValueRef CHAKRA_CALLBACK JsGetScripts(
    JsValueRef callee,
    bool isConstructCall,
    JsValueRef *arguments,
    unsigned short argumentCount,  // NOLINT(runtime/int)
    void *callbackState) {
  JsValueRef sourcesList;
  JsErrorCode errorCode = JsDiagGetScripts(&sourcesList);
  CHAKRA_VERIFY_NOERROR(errorCode);

  return sourcesList;
}

static JsValueRef CHAKRA_CALLBACK JsSetBreakpoint(
    JsValueRef callee,
    bool isConstructCall,
    JsValueRef *arguments,
    unsigned short argumentCount,  // NOLINT(runtime/int)
    void *callbackState) {
  int scriptId;
  int line;
  int column;
  JsValueRef bpObject = JS_INVALID_REFERENCE;

  if (argumentCount >= 4 &&
      jsrt::ValueToInt(arguments[1], &scriptId) == JsNoError &&
      jsrt::ValueToInt(arguments[2], &line) == JsNoError &&
      jsrt::ValueToInt(arguments[3], &column) == JsNoError) {
    JsDiagSetBreakpoint(scriptId, line, column, &bpObject);
  }

  return bpObject;
}

static JsValueRef CHAKRA_CALLBACK JsGetFunctionPosition(
    JsValueRef callee,
    bool isConstructCall,
    JsValueRef *arguments,
    unsigned short argumentCount,  // NOLINT(runtime/int)
    void *callbackState) {
  CHAKRA_VERIFY(argumentCount >= 2);

  JsValueRef valueRef = JS_INVALID_REFERENCE;
  CHAKRA_VERIFY_NOERROR(JsConvertValueToObject(arguments[1], &valueRef));

  JsValueRef funcInfo = JS_INVALID_REFERENCE;
  CHAKRA_VERIFY_NOERROR(JsDiagGetFunctionPosition(valueRef, &funcInfo));

  return funcInfo;
}

static JsValueRef CHAKRA_CALLBACK JsRemoveBreakpoint(
    JsValueRef callee,
    bool isConstructCall,
    JsValueRef *arguments,
    unsigned short argumentCount,  // NOLINT(runtime/int)
    void *callbackState) {
  int bpId = 0;
  if (argumentCount >= 2 &&
    jsrt::ValueToInt(arguments[1], &bpId) == JsNoError) {
    CHAKRA_VERIFY_NOERROR(JsDiagRemoveBreakpoint(bpId));
  }

  return JS_INVALID_REFERENCE;
}

class InspectorBreakQueue {
 public:
  InspectorBreakQueue();
  ~InspectorBreakQueue();
  void Push(v8::InterruptCallback callback, void *data);
  void Swap(std::vector<std::pair<v8::InterruptCallback, void*>> *other);

 private:
  std::vector<std::pair<v8::InterruptCallback, void*>> m_queue;
  uv_mutex_t m_queueMutex;
};

InspectorBreakQueue::InspectorBreakQueue() {
  int err = uv_mutex_init(&m_queueMutex);
  CHAKRA_VERIFY(err == 0);
}

InspectorBreakQueue::~InspectorBreakQueue() {
  uv_mutex_destroy(&m_queueMutex);
}

void InspectorBreakQueue::Push(v8::InterruptCallback callback, void *data) {
  uv_mutex_lock(&m_queueMutex);
  m_queue.emplace_back(callback, data);
  uv_mutex_unlock(&m_queueMutex);
}

void InspectorBreakQueue::Swap(
    std::vector<std::pair<v8::InterruptCallback, void*>> *other) {
  uv_mutex_lock(&m_queueMutex);
  m_queue.swap(*other);
  uv_mutex_unlock(&m_queueMutex);
}

JsDiagDebugEventCallback Inspector::s_callback = nullptr;
void* Inspector::s_callbackState = nullptr;
std::unique_ptr<InspectorBreakQueue> Inspector::s_breakQueue =
    std::unique_ptr<InspectorBreakQueue>(new InspectorBreakQueue());

bool Inspector::IsInspectorEnabled() {
  return v8::g_EnableInspector;
}

bool Inspector::IsReplayDebugEnabled() {
  return v8::g_EnableReplayDebug;
}

void Inspector::StartDebugging(JsRuntimeHandle runtime) {
  JsErrorCode errorCode = JsDiagStartDebugging(
    runtime, JsDiagDebugEventHandler, nullptr);
  CHAKRA_VERIFY_NOERROR(errorCode);
}

void Inspector::RequestAsyncBreak(JsRuntimeHandle runtime,
                                  v8::InterruptCallback callback,
                                  void* data) {
  s_breakQueue->Push(callback, data);
  JsErrorCode errorCode = JsDiagRequestAsyncBreak(runtime);
  CHAKRA_VERIFY_NOERROR(errorCode);
}

void Inspector::RequestAsyncBreak(v8::Isolate* isolate) {
  JsRuntimeHandle handle =
      jsrt::IsolateShim::FromIsolate(isolate)->GetRuntimeHandle();
  JsDiagRequestAsyncBreak(handle);
}

void Inspector::SetChakraDebugObject(JsValueRef chakraDebugObject) {
  Inspector::InstallHostCallback(chakraDebugObject,
    "log", Log);
  Inspector::InstallHostCallback(chakraDebugObject,
    "JsDiagGetScripts", JsGetScripts);
  Inspector::InstallHostCallback(chakraDebugObject,
    "JsDiagSetBreakpoint", JsSetBreakpoint);
  Inspector::InstallHostCallback(chakraDebugObject,
    "JsDiagGetFunctionPosition", JsGetFunctionPosition);
  Inspector::InstallHostCallback(chakraDebugObject,
    "JsDiagRemoveBreakpoint", JsRemoveBreakpoint);
}

void Inspector::SetDebugEventHandler(JsDiagDebugEventCallback callback,
                                      void* callbackState) {
  CHAKRA_ASSERT(s_callback == nullptr || callback == nullptr);

  s_callback = callback;

  if (s_callback != nullptr) {
    s_callbackState = callbackState;
  } else {
    s_callbackState = nullptr;
  }
}

void Inspector::RemoveBreakpoint(unsigned int breakpointId) {
  JsErrorCode err = JsDiagRemoveBreakpoint(breakpointId);

  // Ignore invalid argument as the breakpoint may have overlapped with an
  // existing one.
  if (err != JsErrorInvalidArgument) {
    CHAKRA_VERIFY_NOERROR(err);
  }
}

void Inspector::ClearBreakpoints() {
  JsValueRef breakpoints;
  CHAKRA_VERIFY_NOERROR(JsDiagGetBreakpoints(&breakpoints));

  unsigned int length;
  CHAKRA_VERIFY_NOERROR(jsrt::GetArrayLength(breakpoints, &length));

  for (unsigned int i = 0; i < length; i++) {
    JsValueRef breakpoint;
    CHAKRA_VERIFY_NOERROR(jsrt::GetIndexedProperty(breakpoints, i,
                                                    &breakpoint));

    int breakpointId;
    CHAKRA_VERIFY_NOERROR(jsrt::GetProperty(breakpoint,
                                            CachedPropertyIdRef::breakpointId,
                                            &breakpointId));

    RemoveBreakpoint(breakpointId);
  }
}

void CHAKRA_CALLBACK Inspector::JsDiagDebugEventHandler(
  JsDiagDebugEvent debugEvent,
  JsValueRef eventData,
  void* callbackState) {
  // TTD_NODE
  JsTTDPauseTimeTravelBeforeRuntimeOperation();

  if (debugEvent == JsDiagDebugEventAsyncBreak) {
    std::vector<std::pair<v8::InterruptCallback, void*>> interruptCallbacks;
    s_breakQueue->Swap(&interruptCallbacks);

    for (auto const &item : interruptCallbacks) {
      item.first(IsolateShim::GetCurrentAsIsolate(), item.second);
    }
  }

  if (s_callback != nullptr) {
    s_callback(debugEvent, eventData, s_callbackState);
  }

  // TTD_NODE
  JsTTDReStartTimeTravelAfterRuntimeOperation();
}

void Inspector::InstallHostCallback(JsValueRef chakraDebugObject,
    const char *name, JsNativeFunction nativeFunction) {
  JsValueRef nameVar;
  CHAKRA_VERIFY_NOERROR(JsCreateString(name, strlen(name), &nameVar));

  JsValueRef funcRef;
  CHAKRA_VERIFY_NOERROR(JsCreateNamedFunction(nameVar, nativeFunction,
                                              nullptr, &funcRef));

  CHAKRA_VERIFY_NOERROR(jsrt::SetProperty(chakraDebugObject, nameVar,
                                          funcRef));
}

}  // namespace jsrt
