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

namespace v8 {

class Isolate;

class Task {
 public:
  virtual ~Task() {}
  virtual void Run() = 0;
};

class Platform {
 public:
  enum ExpectedRuntime {
    kShortRunningTask,
    kLongRunningTask
  };

  virtual ~Platform() {}
  virtual void CallOnBackgroundThread(Task* task,
                                      ExpectedRuntime expected_runtime) = 0;
  virtual void CallOnForegroundThread(Isolate* isolate, Task* task) = 0;
  virtual double MonotonicallyIncreasingTime() = 0;

  /**
  * Called by TRACE_EVENT* macros, don't call this directly.
  * The name parameter is a category group for example:
  * TRACE_EVENT0("v8,parse", "V8.Parse")
  * The pointer returned points to a value with zero or more of the bits
  * defined in CategoryGroupEnabledFlags.
  **/
  virtual const uint8_t* GetCategoryGroupEnabled(const char* name) {
    static uint8_t no = 0;
    return &no;
  }

  /**
  * Gets the category group name of the given category_enabled_flag pointer.
  * Usually used while serliazing TRACE_EVENTs.
  **/
  virtual const char* GetCategoryGroupName(
      const uint8_t* category_enabled_flag) {
    static const char dummy[] = "dummy";
    return dummy;
  }

  /**
  * Adds a trace event to the platform tracing system. This function call is
  * usually the result of a TRACE_* macro from trace_event_common.h when
  * tracing and the category of the particular trace are enabled. It is not
  * advisable to call this function on its own; it is really only meant to be
  * used by the trace macros. The returned handle can be used by
  * UpdateTraceEventDuration to update the duration of COMPLETE events.
  */
  virtual uint64_t AddTraceEvent(
      char phase, const uint8_t* category_enabled_flag, const char* name,
      const char* scope, uint64_t id, uint64_t bind_id, int32_t num_args,
      const char** arg_names, const uint8_t* arg_types,
      const uint64_t* arg_values, unsigned int flags) {
    return 0;
  }

  /**
  * Sets the duration field of a COMPLETE trace event. It must be called with
  * the handle returned from AddTraceEvent().
  **/
  virtual void UpdateTraceEventDuration(const uint8_t* category_enabled_flag,
      const char* name, uint64_t handle) {}
};

}  // namespace v8

#endif  // V8_V8_PLATFORM_H_
