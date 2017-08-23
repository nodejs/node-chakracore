// Copyright 2013 the V8 project authors. All rights reserved.
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

#ifndef V8_V8_PLATFORM_H_
#define V8_V8_PLATFORM_H_

#include <stdint.h>
#include <string>
#include <memory>

namespace v8 {

class Isolate;

class Task {
 public:
  virtual ~Task() {}
  virtual void Run() = 0;
};

/**
* The interface represents complex arguments to trace events.
*/
class ConvertableToTraceFormat {
public:
    virtual ~ConvertableToTraceFormat() = default;

    /**
    * Append the class info to the provided |out| string. The appended
    * data must be a valid JSON object. Strings must be properly quoted, and
    * escaped. There is no processing applied to the content after it is
    * appended.
    */
    virtual void AppendAsTraceFormat(std::string* out) const = 0;
};

class TracingController {
 public:
  virtual ~TracingController() = default;

  virtual const uint8_t* GetCategoryGroupEnabled(const char* name) {
    static uint8_t no = 0;
    return &no;
  }

  virtual uint64_t AddTraceEvent(
      char phase, const uint8_t* category_enabled_flag, const char* name,
      const char* scope, uint64_t id, uint64_t bind_id, int32_t num_args,
      const char** arg_names, const uint8_t* arg_types,
      const uint64_t* arg_values,
      std::unique_ptr<ConvertableToTraceFormat>* arg_convertables,
      unsigned int flags) {
    return 0;
  }

  virtual void UpdateTraceEventDuration(const uint8_t* category_enabled_flag,
                                        const char* name, uint64_t handle) {}

  class TraceStateObserver {
   public:
    virtual ~TraceStateObserver() = default;
    virtual void OnTraceEnabled() = 0;
    virtual void OnTraceDisabled() = 0;
  };

  virtual void AddTraceStateObserver(TraceStateObserver*) {}
  virtual void RemoveTraceStateObserver(TraceStateObserver*) {}
};

class Platform {
 public:
  enum ExpectedRuntime {
    kShortRunningTask,
    kLongRunningTask
  };

  virtual ~Platform() = default;

  virtual size_t NumberOfAvailableBackgroundThreads() { return 0; }
  virtual void CallOnBackgroundThread(Task* task,
                                      ExpectedRuntime expected_runtime) = 0;
  virtual void CallOnForegroundThread(Isolate* isolate, Task* task) = 0;
  virtual void CallDelayedOnForegroundThread(Isolate* isolate, Task* task,
                                             double delay_in_seconds) = 0;

  virtual bool IdleTasksEnabled(Isolate* isolate) {
    return false;
  }

  virtual double MonotonicallyIncreasingTime() = 0;
  typedef void(*StackTracePrinter)();

  virtual StackTracePrinter GetStackTracePrinter() { return nullptr; }

  virtual TracingController* GetTracingController() = 0;

  virtual const uint8_t* GetCategoryGroupEnabled(const char* name) {
    static uint8_t no = 0;
    return &no;
  }

  virtual const char* GetCategoryGroupName(
      const uint8_t* category_enabled_flag) {
    static const char dummy[] = "dummy";
    return dummy;
  }

  virtual uint64_t AddTraceEvent(
      char phase, const uint8_t* category_enabled_flag, const char* name,
      const char* scope, uint64_t id, uint64_t bind_id, int32_t num_args,
      const char** arg_names, const uint8_t* arg_types,
      const uint64_t* arg_values, unsigned int flags) {
    return 0;
  }

  virtual uint64_t AddTraceEvent(
      char phase, const uint8_t* category_enabled_flag, const char* name,
      const char* scope, uint64_t id, uint64_t bind_id, int32_t num_args,
      const char** arg_names, const uint8_t* arg_types,
      const uint64_t* arg_values,
      std::unique_ptr<ConvertableToTraceFormat>* arg_convertables,
      unsigned int flags) {
    return AddTraceEvent(phase, category_enabled_flag, name, scope, id, bind_id,
                         num_args, arg_names, arg_types, arg_values, flags);
  }

  virtual void UpdateTraceEventDuration(const uint8_t* category_enabled_flag,
                                        const char* name, uint64_t handle) {}

  typedef v8::TracingController::TraceStateObserver TraceStateObserver;
  virtual void AddTraceStateObserver(TraceStateObserver*) {}
  virtual void RemoveTraceStateObserver(TraceStateObserver*) {}
};

}  // namespace v8

#endif  // V8_V8_PLATFORM_H_
