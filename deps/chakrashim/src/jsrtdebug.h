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

#pragma once

#include <queue>
#include "uv.h"
#include "v8-debug.h"

namespace jsrt {
class MessageImpl : public v8::Debug::Message {
 public:
  explicit MessageImpl(v8::Local<v8::String> v8Str);
  virtual v8::Local<v8::String> GetJSON() const;
  virtual v8::Isolate* GetIsolate() const;
  static v8::Local<v8::String> JsValueRefToV8String(JsValueRef result);

 private:
  v8::Local<v8::String> v8Str;
};

class MessageQueue {
 public:
  MessageQueue();
  ~MessageQueue();
  void SaveMessage(const uint16_t* command, int length);
  uint16_t* PopMessage();
  void SetProcessMessageFunctions(
    JsValueRef chakraDebugObject,
    JsValueRef processDebuggerMessageFn,
    JsValueRef processDebugEventFn,
    JsValueRef processShouldContinueFn);
  void ProcessDebuggerMessage();
  void ProcessDebugEvent(
    JsDiagDebugEvent debugEvent,
    JsValueRef eventData);
  bool ShouldContinue(JsDiagDebugEvent debugEvent);
  bool IsEmpty();
  void WaitForMessage();
  bool waitingForMessage;
  bool isProcessingDebuggerMsg;
  int debugEventProcessCount;

 private:
  std::queue<uint16_t*> *msgQueue;
  JsValueRef chakraDebugObject;
  JsValueRef processDebuggerMessageFn;
  JsValueRef processDebugEventFn;
  JsValueRef processShouldContinueFn;
  uv_sem_t newMsgSem;
  uv_cond_t cond;
  uv_mutex_t mutex;
  uv_mutex_t msgMutex;
};

class Debugger {
 public:
  static MessageQueue messageQueue;
  static v8::Debug::MessageHandler handler;
  static bool IsDebugEnabled();
  static void StartDebugging(JsRuntimeHandle runtime);
  static void SetChakraDebugObject(JsValueRef chakraDebugObject);
 private:
  static void InstallHostCallback(JsValueRef chakraDebugObject,
    const wchar_t *name,
    JsNativeFunction nativeFunction);
};
}  // namespace jsrt
