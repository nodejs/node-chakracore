// Copyright 2008 the V8 project authors. All rights reserved.
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials provided
//       with the distribution.
//     * Neither the name of Google Inc. nor the names of its
//       contributors may be used to endorse or promote products derived
//       from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#pragma once
#include <v8.h>
#include "uv.h"
#include <queue>

namespace v8 {
    class V8_EXPORT MessageQueue {
    public:
        MessageQueue();
        ~MessageQueue();
        void SaveMessage(const uint16_t* command, int length);
        wchar_t* PopMessage();
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
        std::queue<wchar_t*> *msgQueue;
        JsValueRef chakraDebugObject;
        JsValueRef processDebuggerMessageFn;
        JsValueRef processDebugEventFn;
        JsValueRef processShouldContinueFn;
        uv_sem_t newMsgSem;
        uv_cond_t cond;
        uv_mutex_t mutex;

        uv_mutex_t msgMutex;
    };

// NOT IMPLEMENTED
class V8_EXPORT Debug {
 public:
  class ClientData {
  };

  class Message {
   public:
    virtual ~Message() {}
    virtual Handle<String> GetJSON() const = 0;
    virtual Isolate* GetIsolate() const = 0;
  };

  typedef void (*DebugMessageDispatchHandler)();
  typedef void (*MessageHandler)(const Message& message);

  static MessageQueue messageQueue;
  static MessageHandler handler;
  static void ExposeDebug();
  static void DebugBreak(Isolate *isolate = NULL) {}
  static void SetDebugMessageDispatchHandler(
    DebugMessageDispatchHandler handler, bool provide_locker = false) {}
  static bool EnableAgent(
    const char *name = NULL, int port = 0, bool wait_for_connection = false);
  static void Dispose();
  static void DisableAgent() {}
  static bool IsAgentEnabled();
  static void ProcessDebugMessages(Isolate* isolate) {}
  static Local<Context> GetDebugContext(Isolate* isolate);
  static void SetMessageHandler(Isolate* isolate, MessageHandler handler);
  static void SendCommand(Isolate* isolate,
      const uint16_t* command, int length,
      ClientData* client_data = NULL);
  static MaybeLocal<Value> GetMirror(Local<Context> context,
                                     Handle<Value> obj) {
    return MaybeLocal<Value>();
  }
  static void StartDebugging(JsRuntimeHandle runtime);
  static void SetChakraDebugObject(JsValueRef chakraDebugObject);
  static void InstallHostCallback(
      JsValueRef chakraDebugObject,
      const wchar_t *name,
      JsNativeFunction nativeFunction);
  static bool IsDebugExposed();
};

class MessageImpl : public Debug::Message {
  public:
    MessageImpl(Local<String> v8Str);
    virtual Local<String> GetJSON() const;
    virtual Isolate* GetIsolate() const;
    static Local<String> JsValueRefToV8String(JsValueRef result);
  private:
    Local<String> v8Str;
};

}  // namespace v8
