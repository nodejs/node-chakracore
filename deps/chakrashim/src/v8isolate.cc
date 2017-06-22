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

#include "v8.h"
#include "v8-profiler.h"
#include "jsrtutils.h"

namespace v8 {

HeapProfiler dummyHeapProfiler;
CpuProfiler dummyCpuProfiler;

Isolate* Isolate::NewWithTTDSupport(const CreateParams& params,
                      size_t optReplayUriLength, const char* optReplayUri,
                      bool doRecord, bool doReplay, bool doDebug,
                      uint32_t snapInterval, uint32_t snapHistoryLength) {
  Isolate* iso = jsrt::IsolateShim::New(optReplayUriLength, optReplayUri,
                                        doRecord, doReplay, doDebug,
                                        snapInterval, snapHistoryLength);

  if (params.array_buffer_allocator) {
    CHAKRA_VERIFY(!jsrt::IsolateShim::FromIsolate(iso)->arrayBufferAllocator);
    jsrt::IsolateShim::FromIsolate(iso)->arrayBufferAllocator =
        params.array_buffer_allocator;
  }
  return iso;
}

Isolate* Isolate::New(const CreateParams& params) {
  return NewWithTTDSupport(params, 0, nullptr, false, false, false, UINT32_MAX,
                           UINT32_MAX);
}

Isolate* Isolate::New() {
  return jsrt::IsolateShim::New(0, nullptr,
                                false, false, false,
                                UINT32_MAX, UINT32_MAX);
}

Isolate *Isolate::GetCurrent() {
  return jsrt::IsolateShim::GetCurrentAsIsolate();
}

void Isolate::SetAbortOnUncaughtExceptionCallback(
  AbortOnUncaughtExceptionCallback callback) {
  // CHAKRA-TODO: To be implemented
}

void Isolate::Enter() {
  return jsrt::IsolateShim::FromIsolate(this)->Enter();
}

void Isolate::Exit() {
  // CONSIDER: Isolates are only used for debugging
  return jsrt::IsolateShim::FromIsolate(this)->Exit();
}

void Isolate::Dispose() {
  jsrt::IsolateShim::FromIsolate(this)->Dispose();
}

int64_t Isolate::AdjustAmountOfExternalAllocatedMemory(
    int64_t change_in_bytes) {
  // CHAKRA-TODO: We don't support adding external memory pressure at the
  // moment.
  return 0;
}

void Isolate::SetData(uint32_t slot, void* data) {
  return jsrt::IsolateShim::FromIsolate(this)->SetData(slot, data);
}

void* Isolate::GetData(uint32_t slot) {
  return jsrt::IsolateShim::FromIsolate(this)->GetData(slot);
}

bool Isolate::RunSingleStepOfReverseMoveLoop(v8::Isolate* isolate,
                                             uint64_t* moveMode,
                                             int64_t* nextEventTime) {
    return jsrt::IsolateShim::RunSingleStepOfReverseMoveLoop(isolate,
                                                             moveMode,
                                                             nextEventTime);
}

uint32_t Isolate::GetNumberOfDataSlots() {
  return 0;
}

bool Isolate::InContext() {
  return !GetCurrentContext().IsEmpty();
}

Local<Context> Isolate::GetCurrentContext() {
  return Context::GetCurrent();
}

void Isolate::SetPromiseRejectCallback(PromiseRejectCallback callback) {
  // CHAKRA does not support this explicit callback
}

void Isolate::SetPromiseHook(PromiseHook hook) {
  // CHAKRA-TODO: Unimplemented
}

bool Isolate::AddMessageListener(MessageCallback that, Handle<Value> data) {
  // Ignore data parameter.  Node doesn't use it.
  return jsrt::IsolateShim::FromIsolate(this)->AddMessageListener(
    reinterpret_cast<void*>(that));
}

void Isolate::RemoveMessageListeners(MessageCallback that) {
  jsrt::IsolateShim::FromIsolate(this)->RemoveMessageListeners(
    reinterpret_cast<void*>(that));
}

void Isolate::SetCaptureStackTraceForUncaughtExceptions(
  bool capture, int frame_limit,
  StackTrace::StackTraceOptions options) {
  // CHAKRA-TODO: Figure out what to do here
  //
  // kpathak: you might want to look into mechanism of captureStackTrace in
  // chakra_shim.js.
}

void Isolate::SetJitCodeEventHandler(JitCodeEventOptions options,
                                     JitCodeEventHandler event_handler) {
  // CHAKRA-TODO: This is for ETW events, we don't have equivalent but might not
  // need it because we do our own ETW tracing.
}

void Isolate::EnqueueMicrotask(MicrotaskCallback microtask, void* data) {
  // CHAKRA-TODO: Current microTask implementation only support queueing
  // javascript functions. Need to add support to queue native functions
  // And update dequeue code to check and call appropriate function
  // (native vs. javascript)
}

void Isolate::RunMicrotasks() {
  jsrt::ContextShim::GetCurrent()->RunMicrotasks();
}

void Isolate::SetAutorunMicrotasks(bool autorun) {
}

Local<Value> Isolate::ThrowException(Local<Value> exception) {
  JsSetException(*exception);
  return Undefined(this);
}

HeapProfiler* Isolate::GetHeapProfiler() {
  return &dummyHeapProfiler;
}

CpuProfiler* Isolate::GetCpuProfiler() {
  return &dummyCpuProfiler;
}

void Isolate::AddGCPrologueCallback(
  GCCallback callback, GCType gc_type_filter) {
}

void Isolate::RemoveGCPrologueCallback(GCCallback callback) {
}

void Isolate::AddGCEpilogueCallback(
  GCCallback callback, GCType gc_type_filter) {
}

void Isolate::RemoveGCEpilogueCallback(GCCallback callback) {
}

void Isolate::CancelTerminateExecution() {
  jsrt::IsolateShim::FromIsolate(this)->EnableExecution();
}

void Isolate::RequestInterrupt(InterruptCallback callback, void* data) {
  jsrt::IsolateShim::FromIsolate(this)->RequestInterrupt(callback, data);
}

void Isolate::TerminateExecution() {
  jsrt::IsolateShim::FromIsolate(this)->DisableExecution();
}

void Isolate::RequestGarbageCollectionForTesting(GarbageCollectionType type) {
  JsCollectGarbage(jsrt::IsolateShim::FromIsolate(this)->GetRuntimeHandle());
}

void Isolate::SetCounterFunction(CounterLookupCallback) {
  CHAKRA_UNIMPLEMENTED();
}

void Isolate::SetCreateHistogramFunction(CreateHistogramCallback) {
  CHAKRA_UNIMPLEMENTED();
}

void Isolate::SetAddHistogramSampleFunction(AddHistogramSampleCallback) {
  CHAKRA_UNIMPLEMENTED();
}

bool Isolate::IdleNotificationDeadline(double deadline_in_seconds) {
  CHAKRA_UNIMPLEMENTED();
  return false;
}

bool Isolate::IdleNotification(int idle_time_in_ms) {
  CHAKRA_UNIMPLEMENTED();
  return false;
}

void Isolate::LowMemoryNotification() {
  CHAKRA_UNIMPLEMENTED();
}

int Isolate::ContextDisposedNotification() {
  CHAKRA_UNIMPLEMENTED();
  return 0;
}

void Isolate::GetHeapStatistics(HeapStatistics *heap_statistics) {
  size_t memoryUsage;
  if (!jsrt::IsolateShim::FromIsolate(this)->GetMemoryUsage(&memoryUsage)) {
    return;
  }
  // CONSIDER: V8 distinguishes between "total" size and "used" size
  heap_statistics->set_heap_size(memoryUsage);
}

size_t Isolate::NumberOfHeapSpaces() {
  // Chakra doesn't expose HEAP space stats
  return 0;
}

bool Isolate::GetHeapSpaceStatistics(HeapSpaceStatistics* space_statistics,
                                     size_t index) {
  // Chakra doesn't expose HEAP space stats
  return true;
}

Isolate::DisallowJavascriptExecutionScope::DisallowJavascriptExecutionScope(
  Isolate* isolate,
  OnFailure on_failure) {
  // CHAKRA-TODO: Figure out what to do here
}

Isolate::DisallowJavascriptExecutionScope::~DisallowJavascriptExecutionScope() {
  // CHAKRA-TODO: Figure out what to do here
}

}  // namespace v8
