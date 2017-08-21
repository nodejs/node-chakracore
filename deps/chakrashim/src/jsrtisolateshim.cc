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
#include "jsrtutils.h"
#include "chakra_natives.h"
#include <assert.h>
#include <vector>
#include <algorithm>
#include "v8-debug.h"
#include "jsrtinspector.h"

/////////////////////////////////////////////////

void TTReportLastIOError() {
#ifdef _WIN32
  DWORD lastError = GetLastError();
  LPTSTR pTemp = NULL;
  FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                FORMAT_MESSAGE_ARGUMENT_ARRAY,
                NULL, lastError, 0, (LPTSTR)&pTemp, 0, NULL);
  fprintf(stderr, "Error is: %i %s\n", lastError, pTemp);
#else
  fprintf(stderr, "Error is: %i %s\n", errno, strerror(errno));
#endif
}

#ifdef _WIN32
#include <io.h>

#define TTHostPathSep '\\'

bool TTDHostPathEndsWithSepChar(size_t pathLength, const char* path) {
  char ec = path[pathLength - 1];

  if ((ec & 0x80) != 0) {
    // a multibyte char so definitely not path separator
    return false;
  } else {
    // both ok for windows
    return (ec == '\\' || ec == '/');
  }
}

JsTTDStreamHandle TTDHostOpen(size_t pathLength, const char* path,
                              bool isWrite) {
  errno_t err = 0;
  FILE* res = nullptr;
  wchar_t wpath[MAX_PATH];

  int length = MultiByteToWideChar(CP_UTF8, 0, path,
                                   static_cast<int>(pathLength), wpath,
                                   MAX_PATH);
  if (length == 0 || length >= MAX_PATH) {
    fprintf(stderr, "Failed to open file %s\n", path);
    fprintf(stderr, "Path overflowed MAX_PATH or conversion failed\n.");
  }

  wpath[length] = L'\0';
  err = _wfopen_s(&res, wpath, isWrite ? L"w+b" : L"r+b");

  if (err != 0) {
    fprintf(stderr, "Failed to open file %s\n", path);
    fprintf(stderr, "Expanded to %ls on Windows\n", wpath);
    TTReportLastIOError();

    exit(1);
  }

  return static_cast<JsTTDStreamHandle>(res);
}

#define TTDHostRead(buff, size, handle) \
        fread_s(buff, size, 1, size, static_cast<FILE*>(handle))

#define TTDHostWrite(buff, size, handle) \
        fwrite(buff, 1, size,  static_cast<FILE*>(handle))
#else
#include <unistd.h>
#include <cstring>

#define memcpy_s(DST, DSTLENGTH, SRC, COUNT) memcpy(DST, SRC, COUNT)
#define CALLBACK

#define TTHostPathSep '/'

bool TTDHostPathEndsWithSepChar(size_t pathLength, const char* path) {
  char ec = path[pathLength - 1];

  if ((ec & 0x80) != 0) {
    // a multibyte char so definitely not path separator
    return false;
  } else {
    // only forward allowed
    return ec == '/';
  }
}

JsTTDStreamHandle TTDHostOpen(size_t pathLength, const char* path,
                              bool isWrite) {
  FILE* res = fopen(path, isWrite ? "w+b" : "r+b");
  if (res == nullptr) {
    fprintf(stderr, "Failed to open file %s\n", path);
    TTReportLastIOError();

    exit(1);
  }

  return static_cast<JsTTDStreamHandle>(res);
}

#define TTDHostRead(buff, size, handle) \
        fread(buff, 1, size, static_cast<FILE*>(handle))

#define TTDHostWrite(buff, size, handle) \
        fwrite(buff, 1, size, static_cast<FILE*>(handle))
#endif

#define MAX_URI_LENGTH 512

JsTTDStreamHandle CALLBACK TTCreateStreamCallback(size_t uriLength,
                                                  const char* uri,
                                                  size_t asciiNameLength,
                                                  const char* asciiName,
                                                  bool read, bool write) {
  if (uriLength == 0) {
    fprintf(stderr, "Empty path not allowed.\n");
    exit(1);
  }

  if (uriLength + asciiNameLength + 2 > MAX_URI_LENGTH) {
    fprintf(stderr, "Max allowed path length is %u, given path %s%s\n",
            MAX_URI_LENGTH, uri, asciiName);
    exit(1);
  }

  char path[MAX_URI_LENGTH];
  memset(path, 0, MAX_URI_LENGTH);
  memcpy_s(path, MAX_URI_LENGTH, uri, uriLength);

  size_t pathLength = uriLength;
  if (!TTDHostPathEndsWithSepChar(pathLength, path)) {
    path[pathLength] = TTHostPathSep;
    pathLength++;
  }

  memcpy_s(path + pathLength, MAX_URI_LENGTH - pathLength, asciiName,
           asciiNameLength);

  // error is reported and abort in host open
  return TTDHostOpen(pathLength + asciiNameLength, path, write);
}

bool CALLBACK TTReadBytesFromStreamCallback(JsTTDStreamHandle handle,
                                            byte* buff, size_t size,
                                            size_t* readCount) {
  bool ok = false;
  *readCount = TTDHostRead(buff, size, handle);
  ok = (*readCount != 0);

  if (!ok) {
    fprintf(stderr, "Failed read from stream!!!");
    TTReportLastIOError();

    exit(1);
  }

  return ok;
}

bool CALLBACK TTWriteBytesToStreamCallback(JsTTDStreamHandle handle,
                                           const byte* buff, size_t size,
                                           size_t* writtenCount) {
  bool ok = false;
  *writtenCount = TTDHostWrite(buff, size, handle);
  ok = (*writtenCount == size);

  if (!ok) {
    fprintf(stderr, "Failed read from stream!!!");
    TTReportLastIOError();

    exit(1);
  }

  return ok;
}

void CALLBACK TTFlushAndCloseStreamCallback(JsTTDStreamHandle handle,
                                            bool read, bool write) {
  fflush(static_cast<FILE*>(handle));
  fclose(static_cast<FILE*>(handle));
}

/////////////////////////////////////////////////

namespace v8 {
extern bool g_disableIdleGc;
}
namespace jsrt {

/* static */ THREAD_LOCAL IsolateShim * IsolateShim::s_currentIsolate;
/* static */ THREAD_LOCAL IsolateShim * IsolateShim::s_previousIsolate;

/* static */ IsolateShim * IsolateShim::s_isolateList = nullptr;

IsolateShim::IsolateShim(JsRuntimeHandle runtime)
    : arrayBufferAllocator(nullptr),
      debugContext(nullptr),
      runtime(runtime),
      symbolPropertyIdRefs(),
      cachedPropertyIdRefs(),
      isDisposing(false),
      contextScopeStack(nullptr),
      tryCatchStackTop(nullptr),
      embeddedData() {
  // CHAKRA-TODO: multithread locking for s_isolateList?
  this->prevnext = &s_isolateList;
  this->next = s_isolateList;
  s_isolateList = this;
}

IsolateShim::~IsolateShim() {
  // Nothing to do here, Dispose already did everything
  assert(runtime == JS_INVALID_REFERENCE);
  assert(this->next == nullptr);
  assert(this->prevnext == nullptr);

  if (IsolateShim::IsIdleGcEnabled()) {
    uv_close(reinterpret_cast<uv_handle_t*>(idleGc_prepare_handle()), nullptr);
    uv_close(reinterpret_cast<uv_handle_t*>(idleGc_timer_handle()), nullptr);
  }
}

/* static */ v8::Isolate * IsolateShim::New(size_t optReplayUriLength,
                                            const char* optReplayUri,
                                            bool doRecord, bool doReplay,
                                            bool doDebug, uint32_t snapInterval,
                                            uint32_t snapHistoryLength) {
  bool disableIdleGc = v8::g_disableIdleGc;
  JsRuntimeAttributes attributes = static_cast<JsRuntimeAttributes>(
      JsRuntimeAttributeAllowScriptInterrupt |
      JsRuntimeAttributeEnableExperimentalFeatures |
      (disableIdleGc ? JsRuntimeAttributeNone :
                        JsRuntimeAttributeEnableIdleProcessing));

  JsRuntimeHandle runtime;
  JsErrorCode error;
  if (!(doRecord || doReplay)) {
      error = JsCreateRuntime(attributes, nullptr, &runtime);
  } else {
    if (doRecord) {
      error = JsTTDCreateRecordRuntime(attributes, snapInterval,
                                       snapHistoryLength,
                                       &TTCreateStreamCallback,
                                       &TTWriteBytesToStreamCallback,
                                       &TTFlushAndCloseStreamCallback,
                                       nullptr, &runtime);
    } else {
      error = JsTTDCreateReplayRuntime(attributes, optReplayUri,
                                       optReplayUriLength, doDebug,
                                       &TTCreateStreamCallback,
                                       &TTReadBytesFromStreamCallback,
                                       &TTFlushAndCloseStreamCallback,
                                       nullptr, &runtime);
    }
  }

  if (error != JsNoError) {
    return nullptr;
  }

  if (Inspector::IsInspectorEnabled()) {
    // If JavaScript debugging APIs need to be exposed then
    // runtime should be in debugging mode from start
    Inspector::StartDebugging(runtime);
  }

  IsolateShim* newIsolateshim = new IsolateShim(runtime);
  if (!disableIdleGc) {
    uv_prepare_init(uv_default_loop(), newIsolateshim->idleGc_prepare_handle());
    uv_unref(reinterpret_cast<uv_handle_t*>(
      newIsolateshim->idleGc_prepare_handle()));
    uv_timer_init(uv_default_loop(),
      newIsolateshim->idleGc_timer_handle());
    uv_unref(reinterpret_cast<uv_handle_t*>(
      newIsolateshim->idleGc_timer_handle()));
  }
  return ToIsolate(newIsolateshim);
}

/* static */ IsolateShim * IsolateShim::FromIsolate(v8::Isolate * isolate) {
  return reinterpret_cast<IsolateShim *>(isolate);
}

/* static */ v8::Isolate * IsolateShim::ToIsolate(IsolateShim * isolateShim) {
  // v8::Isolate has no data member, so we can just pretend
  return reinterpret_cast<v8::Isolate *>(isolateShim);
}

/* static */ v8::Isolate * IsolateShim::GetCurrentAsIsolate() {
  return ToIsolate(s_currentIsolate);
}

/* static */ IsolateShim *IsolateShim::GetCurrent() {
  assert(s_currentIsolate);
  return s_currentIsolate;
}

void IsolateShim::Enter() {
  // CHAKRA-TODO: we don't support multiple isolate currently, this also doesn't
  // support reentrence
  assert(s_currentIsolate == nullptr || s_currentIsolate == this);
  s_previousIsolate = s_currentIsolate;
  s_currentIsolate = this;
}

void IsolateShim::Exit() {
  // CHAKRA-TODO: we don't support multiple isolate currently, this also doesn't
  // support reentrence
  assert(s_currentIsolate == this);
  s_currentIsolate = s_previousIsolate;
  s_previousIsolate = nullptr;
}

JsRuntimeHandle IsolateShim::GetRuntimeHandle() {
  return runtime;
}

bool IsolateShim::Dispose() {
  isDisposing = true;
  {
    // Disposing the runtime may cause finalize call back to run
    // Set the current IsolateShim scope
    v8::Isolate::Scope scope(ToIsolate(this));
    if (JsDisposeRuntime(runtime) != JsNoError) {
      // Can't do much at this point. Assert that this doesn't happen in debug
      CHAKRA_ASSERT(false);
      return false;
    }
  }

  // CHAKRA-TODO: multithread locking for s_isolateList?
  if (this->next) {
    this->next->prevnext = this->prevnext;
  }
  *this->prevnext = this->next;

  runtime = JS_INVALID_REFERENCE;
  this->next = nullptr;
  this->prevnext = nullptr;

  delete this;
  return true;
}

bool IsolateShim::IsDisposing() {
  return isDisposing;
}

// CHAKRA-TODO: This is not called after cross context work in chakra. Fix this
// else we will leak chakrashim object.
void CHAKRA_CALLBACK IsolateShim::JsContextBeforeCollectCallback(
    JsRef contextRef, void *data) {
  IsolateShim * isolateShim = reinterpret_cast<IsolateShim *>(data);
  ContextShim * contextShim = isolateShim->GetContextShim(contextRef);

  // TTD_NODE
  JsTTDNotifyContextDestroy(contextRef);

  delete contextShim;
}

bool IsolateShim::NewContext(JsContextRef * context, bool exposeGC,
                             bool useGlobalTTState,
                             JsValueRef globalObjectTemplateInstance) {
  ContextShim * contextShim = ContextShim::New(this, exposeGC,
                                               useGlobalTTState,
                                               globalObjectTemplateInstance);
  if (contextShim == nullptr) {
    return false;
  }
  JsContextRef contextRef = contextShim->GetContextRef();
  if (JsSetObjectBeforeCollectCallback(contextRef,
                                this,
                                JsContextBeforeCollectCallback) != JsNoError) {
    delete contextShim;
    return false;
  }
  if (JsSetContextData(contextRef, contextShim) != JsNoError) {
    delete contextShim;
    return false;
  }
  *context = contextRef;
  return true;
}

/* static */
ContextShim * IsolateShim::GetContextShim(JsContextRef contextRef) {
  assert(contextRef != JS_INVALID_REFERENCE);
  void *data;
  if (JsGetContextData(contextRef, &data) != JsNoError) {
    return nullptr;
  }
  ContextShim* contextShim = static_cast<ContextShim *>(data);
  return contextShim;
}

bool IsolateShim::GetMemoryUsage(size_t * memoryUsage) {
  return (JsGetRuntimeMemoryUsage(runtime, memoryUsage) == JsNoError);
}

void IsolateShim::DisposeAll() {
  // CHAKRA-TODO: multithread locking for s_isolateList?
  IsolateShim * curr = s_isolateList;
  s_isolateList = nullptr;
  while (curr) {
    curr->Dispose();
    curr = curr->next;
  }
}

void IsolateShim::PushScope(
    ContextShim::Scope * scope, JsContextRef contextRef) {
  PushScope(scope, GetContextShim(contextRef));
}

void IsolateShim::PushScope(
    ContextShim::Scope * scope, ContextShim * contextShim) {
  scope->contextShim = contextShim;
  scope->previous = this->contextScopeStack;
  this->contextScopeStack = scope;

  // Don't crash even if we fail to set the context
  JsErrorCode errorCode = JsSetCurrentContext(contextShim->GetContextRef());
  CHAKRA_ASSERT(errorCode == JsNoError);
  UNUSED(errorCode);

  contextShim->EnsureInitialized();
}

void IsolateShim::PopScope(ContextShim::Scope * scope) {
  assert(this->contextScopeStack == scope);
  ContextShim::Scope * prevScope = scope->previous;
  if (prevScope != nullptr) {
    JsValueRef exception = JS_INVALID_REFERENCE;
    bool hasException;
    if (scope->contextShim != prevScope->contextShim &&
        JsHasException(&hasException) == JsNoError &&
        hasException &&
        JsGetAndClearException(&exception) == JsNoError) {;}

    // Don't crash even if we fail to set the context
    JsErrorCode errorCode = JsSetCurrentContext(
      prevScope->contextShim->GetContextRef());
    CHAKRA_ASSERT(errorCode == JsNoError);
    UNUSED(errorCode);

    // Propagate the exception to parent scope
    if (exception != JS_INVALID_REFERENCE) {
      JsSetException(exception);
    }
  } else {
    JsSetCurrentContext(JS_INVALID_REFERENCE);
  }
  this->contextScopeStack = prevScope;
}

ContextShim * IsolateShim::GetCurrentContextShim() {
  return this->contextScopeStack->contextShim;
}

JsPropertyIdRef IsolateShim::GetSelfSymbolPropertyIdRef() {
  return GetCachedSymbolPropertyIdRef(CachedSymbolPropertyIdRef::self);
}

JsPropertyIdRef IsolateShim::GetKeepAliveObjectSymbolPropertyIdRef() {
  return GetCachedSymbolPropertyIdRef(CachedSymbolPropertyIdRef::__keepalive__);
}

template <class Index, class CreatePropertyIdFunc>
JsPropertyIdRef GetCachedPropertyId(
    JsPropertyIdRef cache[], Index index,
    const CreatePropertyIdFunc& createPropertyId) {
  JsPropertyIdRef propIdRef = cache[index];
  if (propIdRef == JS_INVALID_REFERENCE) {
    if (createPropertyId(index, &propIdRef)) {
      JsAddRef(propIdRef, nullptr);
      cache[index] = propIdRef;
    }
  }
  return propIdRef;
}

JsPropertyIdRef IsolateShim::GetCachedSymbolPropertyIdRef(
    CachedSymbolPropertyIdRef cachedSymbolPropertyIdRef) {
  CHAKRA_ASSERT(this->GetCurrentContextShim() != nullptr);
  return GetCachedPropertyId(symbolPropertyIdRefs, cachedSymbolPropertyIdRef,
                    [](CachedSymbolPropertyIdRef, JsPropertyIdRef* propIdRef) {
      JsValueRef newSymbol;
      return JsCreateSymbol(JS_INVALID_REFERENCE, &newSymbol) == JsNoError &&
        JsGetPropertyIdFromSymbol(newSymbol, propIdRef) == JsNoError;
  });
}

static char const *
const s_cachedPropertyIdRefNames[CachedPropertyIdRef::Count] = {
#define DEF(x) #x,
#include "jsrtcachedpropertyidref.inc"
};

JsPropertyIdRef IsolateShim::GetCachedPropertyIdRef(
    CachedPropertyIdRef cachedPropertyIdRef) {
  return GetCachedPropertyId(cachedPropertyIdRefs, cachedPropertyIdRef,
                    [](CachedPropertyIdRef index, JsPropertyIdRef* propIdRef) {
    return CreatePropertyId(s_cachedPropertyIdRefNames[index],
      propIdRef) == JsNoError;
  });
}

JsPropertyIdRef IsolateShim::GetToStringTagSymbolPropertyIdRef() {
  return GetCachedPropertyId(cachedPropertyIdRefs,
                CachedPropertyIdRef::Symbol_toStringTag,
                [this](CachedPropertyIdRef index, JsPropertyIdRef* propIdRef) {
    JsValueRef toStringTagSymbol;
    JsValueRef symbolObject =
      this->GetCurrentContextShim()
      ->GetGlobalType(ContextShim::GlobalType::Symbol);
    JsPropertyIdRef toStringTag = this
      ->GetCachedPropertyIdRef(jsrt::CachedPropertyIdRef::toStringTag);
    if (JsGetProperty(symbolObject,
                      toStringTag,
                      &toStringTagSymbol) != JsNoError) {
      return false;
    }

    if (JsGetPropertyIdFromSymbol(toStringTagSymbol, propIdRef) != JsNoError) {
      return false;
    }
    return true;
  });
}


JsPropertyIdRef IsolateShim::GetProxyTrapPropertyIdRef(ProxyTraps trap) {
  return GetCachedPropertyIdRef(GetProxyTrapCachedPropertyIdRef(trap));
}

/* static */
ContextShim * IsolateShim::GetContextShimOfObject(JsValueRef valueRef) {
  JsContextRef contextRef;
  if (JsGetContextOfObject(valueRef, &contextRef) != JsNoError) {
    return nullptr;
  }
  assert(contextRef != nullptr);
  return GetContextShim(contextRef);
}

void IsolateShim::RequestInterrupt(v8::InterruptCallback callback, void* data) {
  Inspector::RequestAsyncBreak(this->GetRuntimeHandle(), callback, data);
}

void IsolateShim::DisableExecution() {
  // CHAKRA: Error handling?
  JsDisableRuntimeExecution(this->GetRuntimeHandle());
}

void IsolateShim::EnableExecution() {
  // CHAKRA: Error handling?
  JsEnableRuntimeExecution(this->GetRuntimeHandle());
}

bool IsolateShim::IsExeuctionDisabled() {
  bool isDisabled;
  if (JsIsRuntimeExecutionDisabled(this->GetRuntimeHandle(),
                                   &isDisabled) == JsNoError) {
    return isDisabled;
  }
  return false;
}

bool IsolateShim::AddMessageListener(void * that) {
  try {
    messageListeners.push_back(that);
    return true;
  } catch(...) {
    return false;
  }
}

void IsolateShim::RemoveMessageListeners(void * that) {
  auto i = std::remove(messageListeners.begin(), messageListeners.end(), that);
  messageListeners.erase(i, messageListeners.end());
}

void IsolateShim::SetData(uint32_t slot, void* data) {
  if (slot >= _countof(this->embeddedData)) {
    CHAKRA_UNIMPLEMENTED_("Invalid embedded data index");
  }
  embeddedData[slot] = data;
}

void* IsolateShim::GetData(uint32_t slot) {
  return slot < _countof(this->embeddedData) ? embeddedData[slot] : nullptr;
}

JsValueRef IsolateShim::GetChakraShimJsArrayBuffer() {
  JsValueRef chakraShimArrayBuffer = JS_INVALID_REFERENCE;
  CHAKRA_VERIFY_NOERROR(JsCreateExternalArrayBuffer(
      static_cast<void*>(const_cast<uint8_t*>(raw_chakra_shim_value)),
      sizeof(raw_chakra_shim_value), nullptr, nullptr, &chakraShimArrayBuffer));

  return chakraShimArrayBuffer;
}

JsValueRef IsolateShim::GetChakraInspectorShimJsArrayBuffer() {
  JsValueRef chakraInspectorShimArrayBuffer = JS_INVALID_REFERENCE;
  CHAKRA_VERIFY_NOERROR(JsCreateExternalArrayBuffer(
      static_cast<void*>(const_cast<uint8_t*>(raw_chakra_inspector_value)),
      sizeof(raw_chakra_inspector_value), nullptr, nullptr,
      &chakraInspectorShimArrayBuffer));

  return chakraInspectorShimArrayBuffer;
}

/*static*/
bool IsolateShim::RunSingleStepOfReverseMoveLoop(v8::Isolate* isolate,
                                                 uint64_t* moveMode,
                                                 int64_t* nextEventTime) {
    int64_t snapEventTime = -1;
    int64_t snapEventEndTime = -1;
    int64_t origNETime = *nextEventTime;
    // TODO(mrkmarron) JsTTDMoveMode is no longer 64 bit wide, fix use of
    // moveMode parameter, convert to 32 bit?
    JsTTDMoveMode _moveMode = (JsTTDMoveMode)(*moveMode);
    JsRuntimeHandle rHandle =
        jsrt::IsolateShim::FromIsolate(isolate)->GetRuntimeHandle();

    // if mode is reverse continue then we need to scan
    if ((_moveMode & JsTTDMoveMode::JsTTDMoveScanIntervalForContinue)
                     == JsTTDMoveMode::JsTTDMoveScanIntervalForContinue) {
        int64_t ciStart = -1;
        int64_t ciEnd = -1;
        JsTTDGetSnapShotBoundInterval(rHandle, *nextEventTime,
                                      &ciStart, &ciEnd);

        *nextEventTime = -1;
        JsTTDPreExecuteSnapShotInterval(
            rHandle, ciStart, ciEnd,
            (JsTTDMoveMode)(*moveMode | JsTTDMoveMode::JsTTDMoveScanIntervalForContinueInActiveBreakpointSegment),  // NOLINT(whitespace/line_length)
            nextEventTime);

        while (*nextEventTime == -1) {
            int64_t newCiStart = -1;
            JsTTDGetPreviousSnapshotInterval(rHandle, ciStart, &newCiStart);

            if (newCiStart == -1) {
                break;
            }

            ciEnd = ciStart;
            ciStart = newCiStart;
            JsTTDPreExecuteSnapShotInterval(rHandle, ciStart, ciEnd,
                                            ((JsTTDMoveMode)(*moveMode)),
                                            nextEventTime);
        }

        // did scan so no longer needed
        _moveMode = (JsTTDMoveMode)(_moveMode &
                     ~JsTTDMoveMode::JsTTDMoveScanIntervalForContinue);
    }

    if (*nextEventTime == -1) {
        _moveMode = (JsTTDMoveMode)(JsTTDMoveMode::JsTTDMoveFirstEvent |
                                    JsTTDMoveMode::JsTTDMoveBreakOnEntry);
    }

    JsErrorCode timeError = JsTTDGetSnapTimeTopLevelEventMove(
        rHandle, _moveMode, 0, nextEventTime, &snapEventTime,
        &snapEventEndTime);

    if (timeError != JsNoError) {
        if (timeError == JsErrorCategoryUsage) {
            fprintf(stderr, "Start time not in log range.");
            exit(0);
        } else {
            fprintf(stderr, "Fatal Error in Move!!!");
            exit(1);
        }
    }

    JsErrorCode moveError = JsTTDMoveToTopLevelEvent(rHandle, _moveMode,
                                                     snapEventTime,
                                                     *nextEventTime);
    if (moveError != JsNoError) {
        fprintf(stderr, "Fatal Error in Move Action!!!");
        exit(1);
    }

    JsErrorCode replayError = JsTTDReplayExecution(&_moveMode, nextEventTime);
    if (replayError != JsNoError && replayError != JsErrorCategoryScript) {
        fprintf(stderr, "Fatal Error in Replay Action!!!");
        exit(1);
    }

    // update before we return
    *moveMode = (uint64_t)_moveMode;
    if (*nextEventTime == -1) {
        fprintf(stderr, "\nReached end of Execution -- Exiting.\n");
        return false;
    }
    return true;
}
}  // namespace jsrt
