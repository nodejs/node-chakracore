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

#ifndef DEPS_CHAKRASHIM_SRC_JSRTISOLATESHIM_H_
#define DEPS_CHAKRASHIM_SRC_JSRTISOLATESHIM_H_

#include "uv.h"
#include <vector>

// CHAKRA-TODO : now that node is using libc++ for C++11 support can we remove
// all OSX_SDK_TR1 specialization?
#if !defined(OSX_SDK_TR1) && defined(__APPLE__)
#include <AvailabilityMacros.h>
#if __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ < MAC_OS_X_VERSION_10_9
#define OSX_SDK_TR1
#endif
#endif

#include <unordered_map>

namespace v8 {

class Isolate;
class TryCatch;
extern bool g_disableIdleGc;
}  // namespace v8

namespace jsrt {

enum CachedPropertyIdRef : int {
#define DEF(x, ...) x,
#include "jsrtcachedpropertyidref.inc"
  Count
};

enum CachedSymbolPropertyIdRef {
#define DEFSYMBOL(x, ...) x,
#include "jsrtcachedpropertyidref.inc"
  SymbolCount
};

class IsolateShim {
 public:
  v8::ArrayBuffer::Allocator* arrayBufferAllocator;
  bool NewContext(JsContextRef * context, bool exposeGC, bool useGlobalTTState,
                               JsValueRef globalObjectTemplateInstance);
  bool GetMemoryUsage(size_t * memoryUsage);
  bool Dispose();
  bool IsDisposing();

  static v8::Isolate * New(size_t optReplayUriLength, const char* optReplayUri,
                           bool doRecord, bool doReplay, bool doDebug,
                           uint32_t snapInterval, uint32_t snapHistoryLength);

  static v8::Isolate * GetCurrentAsIsolate();
  static IsolateShim * GetCurrent();
  static IsolateShim * FromIsolate(v8::Isolate * isolate);
  static void DisposeAll();

  static ContextShim * GetContextShim(JsContextRef contextRef);
  JsRuntimeHandle GetRuntimeHandle();
  static ContextShim * GetContextShimOfObject(JsValueRef valueRef);

  void Enter();
  void Exit();

  void PushScope(ContextShim::Scope * scope, JsContextRef contextRef);
  void PushScope(ContextShim::Scope * scope, ContextShim * contextShim);
  void PopScope(ContextShim::Scope * scope);

  // TTD_NODE
  static bool RunSingleStepOfReverseMoveLoop(v8::Isolate* isolate,
                                             uint64_t* moveMode,
                                             int64_t* nextEventTime);

  ContextShim * GetCurrentContextShim();

  // Symbols propertyIdRef
  JsPropertyIdRef GetSelfSymbolPropertyIdRef();
  JsPropertyIdRef GetToStringTagSymbolPropertyIdRef();
  JsPropertyIdRef GetKeepAliveObjectSymbolPropertyIdRef();
  JsPropertyIdRef GetCachedSymbolPropertyIdRef(
    CachedSymbolPropertyIdRef cachedSymbolPropertyIdRef);

  // String propertyIdRef
  JsPropertyIdRef GetProxyTrapPropertyIdRef(ProxyTraps trap);
  JsPropertyIdRef GetCachedPropertyIdRef(
    CachedPropertyIdRef cachedPropertyIdRef);

  void RequestInterrupt(v8::InterruptCallback callback, void* data);
  void DisableExecution();
  bool IsExeuctionDisabled();
  void EnableExecution();
  static inline bool IsIdleGcEnabled() {
    return !v8::g_disableIdleGc;
  }

  bool AddMessageListener(void * that);
  void RemoveMessageListeners(void * that);
  template <typename Fn>
  void ForEachMessageListener(Fn fn) {
    for (auto i = messageListeners.begin(); i != messageListeners.end(); i++) {
      fn(*i);
    }
  }

  JsValueRef GetChakraShimJsArrayBuffer();
  JsValueRef GetChakraInspectorShimJsArrayBuffer();

  void SetData(unsigned int slot, void* data);
  void* GetData(unsigned int slot);

  ContextShim* debugContext;

  inline uv_prepare_t* idleGc_prepare_handle() {
    return &idleGc_prepare_handle_;
  }

  inline uv_timer_t* idleGc_timer_handle() {
    return &idleGc_timer_handle_;
  }

  inline bool IsJsScriptExecuted() {
    return jsScriptExecuted;
  }

  inline void SetScriptExecuted() {
    jsScriptExecuted = true;
  }
  inline void ResetScriptExecuted() {
    jsScriptExecuted = false;
  }

  inline void SetIsIdleGcScheduled() {
    isIdleGcScheduled = true;
  }

  inline void ResetIsIdleGcScheduled() {
    isIdleGcScheduled = false;
  }

  inline bool IsIdleGcScheduled() {
    return isIdleGcScheduled;
  }

 private:
  // Construction/Destruction should go thru New/Dispose
  explicit IsolateShim(JsRuntimeHandle runtime);
  ~IsolateShim();
  static v8::Isolate * ToIsolate(IsolateShim * isolate);
  static void CHAKRA_CALLBACK JsContextBeforeCollectCallback(JsRef contextRef,
                                                             void *data);

  JsRuntimeHandle runtime;
  JsPropertyIdRef symbolPropertyIdRefs[CachedSymbolPropertyIdRef::SymbolCount];
  JsPropertyIdRef cachedPropertyIdRefs[CachedPropertyIdRef::Count];
  bool isDisposing;

  ContextShim::Scope * contextScopeStack;
  IsolateShim ** prevnext;
  IsolateShim * next;

  friend class v8::TryCatch;
  v8::TryCatch * tryCatchStackTop;

  std::vector<void *> messageListeners;

  // Node only has 4 slots (internals::Internals::kNumIsolateDataSlots = 4)
  void * embeddedData[4];

  // CHAKRA-TODO: support multiple shims
  static IsolateShim * s_isolateList;

  static THREAD_LOCAL IsolateShim * s_currentIsolate;
  static THREAD_LOCAL IsolateShim * s_previousIsolate;

  uv_prepare_t idleGc_prepare_handle_;
  uv_timer_t idleGc_timer_handle_;
  bool jsScriptExecuted = false;
  bool isIdleGcScheduled = false;
};
}  // namespace jsrt

#endif  // DEPS_CHAKRASHIM_SRC_JSRTISOLATESHIM_H_
