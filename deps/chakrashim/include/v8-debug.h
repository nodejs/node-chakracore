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

namespace v8 {

enum DebugEvent {
  Break = 1,
  Exception = 2,
  NewFunction = 3,
  BeforeCompile = 4,
  AfterCompile = 5,
  CompileError = 6,
  AsyncTaskEvent = 7,
};

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

  class EventDetails {
  public:
    /**
    * Event type.
    */
    virtual DebugEvent GetEvent() const = 0;

    /**
    * Access to execution state and event data of the debug event. Don't store
    * these cross callbacks as their content becomes invalid.
    */
    virtual Local<Object> GetExecutionState() const = 0;
    virtual Local<Object> GetEventData() const = 0;
    virtual Local<Context> GetEventContext() const = 0;
    virtual Isolate* GetIsolate() const = 0;
    virtual ~EventDetails() {}
  };


  typedef void (*DebugMessageDispatchHandler)();
  typedef void(*EventCallback)(const EventDetails& event_details);
  typedef void (*MessageHandler)(const Message& message);

  static bool SetDebugEventListener(Isolate* isolate, EventCallback that,
    Local<Value> data = Local<Value>());
  static void DebugBreak(Isolate *isolate = NULL) {}
  static MaybeLocal<Value> GetMirror(Local<Context> context,
                                     Handle<Value> obj) {
    return MaybeLocal<Value>();
  }
  static void ProcessDebugMessages(Isolate* isolate) {}
  static Local<Context> GetDebugContext(Isolate* isolate);

  static void EnableInspector(bool enableReplayDebug = false);

  static void SetLiveEditEnabled(Isolate* isolate, bool enable);
};

}  // namespace v8
