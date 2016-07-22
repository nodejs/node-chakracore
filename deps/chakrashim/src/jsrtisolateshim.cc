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
#include <assert.h>
#include <vector>
#include <algorithm>
#include "v8-debug.h"

namespace v8 {
extern bool g_disableIdleGc;
}
namespace jsrt {

    void ConvertToStdString(const wchar_t* src, std::string& dest)
    {
        size_t strlen = wcslen(src);
        for(size_t i = 0; i < strlen; ++i)
        {
            dest.push_back((char)src[i]);
        }
    }
    void CreateDirectoryIfNeeded(const char* path)
    {
        bool isPathDirName = (path[strlen(path) - 1] == L'\\');
        std::string fullpath(path);
        if(!isPathDirName)
        {
            fullpath.append("\\");
        }
        DWORD dwAttrib = GetFileAttributes(fullpath.c_str());
        if((dwAttrib != INVALID_FILE_ATTRIBUTES) && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY))
        {
            return;
        }
        BOOL success = CreateDirectory(fullpath.c_str(), NULL);
        if(!success)
        {
            DWORD lastError = GetLastError();
            LPTSTR pTemp = NULL;
            FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ARGUMENT_ARRAY, NULL, lastError, LANG_NEUTRAL, (LPTSTR)&pTemp, 0, NULL);
            fprintf(stderr, ": %s", pTemp);
        }
    }
    void DeleteDirectory(const char* path)
    {
        HANDLE hFile;
        WIN32_FIND_DATA FileInformation;
        bool isPathDirName = (path[strlen(path) - 1] == L'\\');
        std::string strPattern(path);
        if(!isPathDirName)
        {
            strPattern.append("\\");
        }
        strPattern.append("*.*");
        hFile = ::FindFirstFile(strPattern.c_str(), &FileInformation);
        if(hFile != INVALID_HANDLE_VALUE)
        {
            do
            {
                if(FileInformation.cFileName[0] != '.')
                {
                    std::string strFilePath(path);
                    if(!isPathDirName)
                    {
                        strFilePath.append("\\");
                    }
                    strFilePath.append(FileInformation.cFileName);
                    if(FileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                    {
                        DeleteDirectory(strFilePath.c_str());
                        ::RemoveDirectory(strFilePath.c_str());
                    }
                    else
                    {
                        ::SetFileAttributes(strFilePath.c_str(), FILE_ATTRIBUTE_NORMAL);
                        ::DeleteFile(strFilePath.c_str());
                    }
                }
            } while(::FindNextFile(hFile, &FileInformation) == TRUE);
            ::FindClose(hFile);
        }
    }
    void GetFileFromURI(const char* uri, std::string& res)
    {
        int urilen = (int)strlen(uri);
        int fpos = 0;
        for(int spos = urilen - 1; spos >= 0; --spos)
        {
            if(uri[spos] == L'\\' || uri[spos] == L'/')
            {
                fpos = spos + 1;
                break;
            }
        }
        res.append(uri + fpos);
    }
    void GetDefaultTTDDirectory(std::string& res, const char* optExtraDir)
    {
        char* path = (char*)CoTaskMemAlloc(MAX_PATH * sizeof(char));
        path[0] = '\0';
        GetModuleFileName(NULL, path, MAX_PATH);
        char* spos = path + strlen(path);
        while(spos != path && *spos != '\\')
        {
            spos--;
        }
        int ccount = (int)((((byte*)spos) - ((byte*)path)) / sizeof(char));
        res.append(path, 0, ccount);
        if(res.back() != '\\')
        {
            res.append("\\");
        }
        res.append(optExtraDir);
        if(res.back() != '\\')
        {
            res.append("\\");
        }
        CoTaskMemFree(path);
    }
    void CALLBACK GetTTDDirectory(const wchar_t* uri, wchar_t** fullTTDUri)
    {
        std::string logDir;
        std::string uriStr;
        ConvertToStdString(uri, uriStr);
        GetDefaultTTDDirectory(logDir, uriStr.c_str());
        if(logDir.back() != '\\')
        {
            logDir.push_back('\\');
        }
        const char* sstr = logDir.c_str();
        int uriLength = (int)(strlen(sstr) + 1);
        *fullTTDUri = (wchar_t*)CoTaskMemAlloc(uriLength * sizeof(wchar_t));
        for(int i = 0; i < uriLength; ++i)
        {
            (*fullTTDUri)[i] = (wchar_t)sstr[i];
        }
    }
    void CALLBACK TTInitializeForWriteLogStreamCallback(const wchar_t* uri)
    {
        std::string uriStr;
        ConvertToStdString(uri, uriStr);
        CreateDirectoryIfNeeded(uriStr.c_str());
        DeleteDirectory(uriStr.c_str());
    }
    static HANDLE TTOpenStream_Helper(const char* uri, bool read, bool write)
    {
        HANDLE res = INVALID_HANDLE_VALUE;
        if(read)
        {
            res = CreateFile(uri, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        }
        else
        {
            res = CreateFile(uri, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        }
        if(res == INVALID_HANDLE_VALUE)
        {
            DWORD lastError = GetLastError();
            LPTSTR pTemp = NULL;
            FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ARGUMENT_ARRAY, NULL, lastError, LANG_NEUTRAL, (LPTSTR)&pTemp, 0, NULL);
            fprintf(stderr, ": %s\n", pTemp);
            fprintf(stderr, "Failed on file: %s\n", uri);
        }
        return res;
    }
    HANDLE CALLBACK TTGetLogStreamCallback(const wchar_t* uri, bool read, bool write)
    {
        std::string uriString;
        ConvertToStdString(uri, uriString);
        std::string logFile(uriString);
        logFile.append("ttdlog.log");
        return TTOpenStream_Helper(logFile.c_str(), read, write);
    }
    HANDLE CALLBACK TTGetSnapshotStreamCallback(const wchar_t* uri, const wchar_t* snapId, bool read, bool write)
    {
        std::string uriStr;
        ConvertToStdString(uri, uriStr);
        std::string snapIdStr;
        ConvertToStdString(snapId, snapIdStr);
        std::string snapFile(uriStr);
        snapFile.append("\\snap_");
        snapFile.append(snapIdStr);
        snapFile.append(".snp");
        return TTOpenStream_Helper(snapFile.c_str(), read, write);
    }
    HANDLE CALLBACK TTGetSrcCodeStreamCallback(const wchar_t* snapContainerUri, const wchar_t* documentid, const wchar_t* srcFileName, bool read, bool write)
    {
        std::string snapContainerUriStr;
        ConvertToStdString(snapContainerUri, snapContainerUriStr);
        std::string documentidStr;
        ConvertToStdString(documentid, documentidStr);
        std::string srcFileNameStr;
        ConvertToStdString(srcFileName, srcFileNameStr);
        std::string sFile;
        GetFileFromURI(srcFileNameStr.c_str(), sFile);
        std::string srcPath(snapContainerUriStr);
        srcPath.append(documentidStr);
        srcPath.append("_");
        srcPath.append(sFile);
        return TTOpenStream_Helper(srcPath.c_str(), read, write);
    }
    bool CALLBACK TTReadBytesFromStreamCallback(HANDLE strm, BYTE* buff, DWORD size, DWORD* readCount)
    {
        *readCount = 0;
        BOOL ok = ReadFile(strm, buff, size, readCount, NULL);
        return ok ? true : false;
    }
    bool CALLBACK TTWriteBytesToStreamCallback(HANDLE strm, BYTE* buff, DWORD size, DWORD* writtenCount)
    {
        BOOL ok = WriteFile(strm, buff, size, writtenCount, NULL);
        return ok ? true : false;
    }
    void CALLBACK TTFlushAndCloseStreamCallback(HANDLE strm, bool read, bool write)
    {
        if(strm != INVALID_HANDLE_VALUE)
        {
            if(write)
            {
                FlushFileBuffers(strm);
            }
            CloseHandle(strm);
        }
    }
/* static */ __declspec(thread) IsolateShim * IsolateShim::s_currentIsolate;
/* static */ __declspec(thread) IsolateShim * IsolateShim::s_previousIsolate;
/* static */ IsolateShim * IsolateShim::s_isolateList = nullptr;

IsolateShim::IsolateShim(JsRuntimeHandle runtime)
    : runtime(runtime),
      contextScopeStack(nullptr),
      symbolPropertyIdRefs(),
      cachedPropertyIdRefs(),
      embeddedData(),
      isDisposing(false),
      tryCatchStackTop(nullptr),
      g_arrayBufferAllocator(nullptr),
      debugContext(nullptr) {
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

/* static */ v8::Isolate * IsolateShim::New(const char* uri, bool doRecord, bool doReplay, uint32_t snapInterval, uint32_t snapHistoryLength) {
  // CHAKRA-TODO: Disable multiple isolate for now until it is fully implemented
  /*
  if (s_isolateList != nullptr) {
    CHAKRA_UNIMPLEMENTED_("multiple isolate");
    return nullptr;
  }
  */
  bool disableIdleGc = v8::g_disableIdleGc;
    JsRuntimeAttributes attributes = static_cast<JsRuntimeAttributes>(
      JsRuntimeAttributeAllowScriptInterrupt |
      JsRuntimeAttributeEnableExperimentalFeatures |
        (disableIdleGc ? JsRuntimeAttributeNone : JsRuntimeAttributeEnableIdleProcessing));

  JsRuntimeHandle runtime;
  JsErrorCode error;
  if(uri == nullptr) {
      error = JsCreateRuntime(attributes, nullptr, &runtime);
  }
  else
  {
      if(doRecord) {
          error = JsTTDCreateRecordRuntime(static_cast<JsRuntimeAttributes>(JsRuntimeAttributeAllowScriptInterrupt | JsRuntimeAttributeEnableExperimentalFeatures), const_cast<char*>(uri), strlen(uri), snapInterval, snapHistoryLength, nullptr, &runtime);
      }
      else {
          error = JsTTDCreateDebugRuntime(static_cast<JsRuntimeAttributes>(JsRuntimeAttributeAllowScriptInterrupt | JsRuntimeAttributeEnableExperimentalFeatures), const_cast<char*>(uri), strlen(uri), nullptr, &runtime);
      }
      if(error == JsNoError) {
          JsTTDSetIOCallbacks(runtime, &GetTTDDirectory, &TTInitializeForWriteLogStreamCallback, &TTGetLogStreamCallback, &TTGetSnapshotStreamCallback, &TTGetSrcCodeStreamCallback, &TTReadBytesFromStreamCallback, &TTWriteBytesToStreamCallback, &TTFlushAndCloseStreamCallback);
      }
  }
  if (error != JsNoError) {
    return nullptr;
  }
  
  if (v8::Debug::IsDebugExposed()) {
    // If JavaScript debugging APIs need to be exposed then
    // runtime should be in debugging mode from start
    v8::Debug::StartDebugging(runtime);
  }

  IsolateShim* newIsolateshim = new IsolateShim(runtime);
  if (!disableIdleGc) {
    uv_prepare_init(uv_default_loop(), newIsolateshim->idleGc_prepare_handle());
    uv_unref(reinterpret_cast<uv_handle_t*>(newIsolateshim->idleGc_prepare_handle()));
    uv_timer_init(uv_default_loop(), newIsolateshim->idleGc_timer_handle());
    uv_unref(reinterpret_cast<uv_handle_t*>(newIsolateshim->idleGc_timer_handle()));
  }
  return ToIsolate(newIsolateshim);
}

/* static */ IsolateShim * IsolateShim::FromIsolate(v8::Isolate * isolate) {
  return reinterpret_cast<jsrt::IsolateShim *>(isolate);
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
void CALLBACK IsolateShim::JsContextBeforeCollectCallback(JsRef contextRef,
                                                          void *data) {
  IsolateShim * isolateShim = reinterpret_cast<IsolateShim *>(data);
  ContextShim * contextShim = isolateShim->GetContextShim(contextRef);
  delete contextShim;
}

bool IsolateShim::NewContext(JsContextRef * context, bool exposeGC, bool runUnderTT,
                             JsValueRef globalObjectTemplateInstance) {
  ContextShim * contextShim =
    ContextShim::New(this, exposeGC, runUnderTT, globalObjectTemplateInstance);
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
  ContextShim* contextShim = static_cast<jsrt::ContextShim *>(data);
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
        JsGetAndClearException(&exception) == JsNoError) {
    }

    // Don't crash even if we fail to set the context
    JsErrorCode errorCode = JsSetCurrentContext(
      prevScope->contextShim->GetContextRef());
    CHAKRA_ASSERT(errorCode == JsNoError);

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

static wchar_t const *
const s_cachedPropertyIdRefNames[CachedPropertyIdRef::Count] = {
#define DEF(x) L#x,
#include "jsrtcachedpropertyidref.inc"
};

JsPropertyIdRef IsolateShim::GetCachedPropertyIdRef(
    CachedPropertyIdRef cachedPropertyIdRef) {
  return GetCachedPropertyId(cachedPropertyIdRefs, cachedPropertyIdRef,
                    [](CachedPropertyIdRef index, JsPropertyIdRef* propIdRef) {
    return JsGetPropertyIdFromName(s_cachedPropertyIdRefNames[index],
                                   propIdRef) == JsNoError;
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

/*static*/ bool IsolateShim::RunSingleStepOfReverseMoveLoop(v8::Isolate* isolate, uint64_t* moveMode, int64_t* nextEventTime)
{
    INT64 snapEventTime = -1;
    INT64 snapEventEndTime = -1;
    int64_t origNETime = *nextEventTime;
    JsTTDMoveMode _moveMode = (JsTTDMoveMode)(*moveMode);
    JsRuntimeHandle rHandle = jsrt::IsolateShim::FromIsolate(isolate)->GetRuntimeHandle();
    bool needFreshCtxs = false;
    JsErrorCode error = JsTTDGetSnapTimeTopLevelEventMove(rHandle, _moveMode, nextEventTime, &needFreshCtxs, &snapEventTime, &snapEventEndTime);
    if(error != JsNoError)
    {
        if(error == JsErrorCategoryUsage)
        {
            printf("Start time not in log range.");
            ExitProcess(0);
        }
        else
        {
            printf("Fatal Error in Move!!!");
            ExitProcess(1);
        }
    }
    JsTTDPrepContextsForTopLevelEventMove(rHandle, needFreshCtxs);
    if((*moveMode & JsTTDMoveMode::JsTTDMoveScanIntervalBeforeDebugExecute) == JsTTDMoveMode::JsTTDMoveScanIntervalBeforeDebugExecute)
    {
        JsTTDPreExecuteSnapShotInterval(snapEventTime, snapEventEndTime, ((JsTTDMoveMode)(*moveMode)));
        _moveMode = (JsTTDMoveMode)(_moveMode & ~JsTTDMoveMode::JsTTDMoveScanIntervalBeforeDebugExecute); //did scan so no longer needed
        error = JsTTDGetSnapTimeTopLevelEventMove(rHandle, _moveMode, nextEventTime, &needFreshCtxs, &snapEventTime, nullptr);
        if(error != JsNoError)
        {
            if(error == JsErrorCategoryUsage)
            {
                printf("Start time not in log range.");
                ExitProcess(0);
            }
            else
            {
                printf("Fatal Error in Move!!!");
                ExitProcess(1);
            }
        }
        JsTTDPrepContextsForTopLevelEventMove(rHandle, needFreshCtxs);
    }
    JsTTDMoveToTopLevelEvent(_moveMode, snapEventTime, *nextEventTime);
    JsErrorCode res = JsTTDReplayExecution(&_moveMode, nextEventTime);
    //update before we return
    *moveMode = (uint64_t)_moveMode;
    if(*nextEventTime == -1)
    {
        printf("\nReached end of Execution -- Exiting.\n");
        return FALSE;
    }
    return TRUE;
}
}  // namespace jsrt
