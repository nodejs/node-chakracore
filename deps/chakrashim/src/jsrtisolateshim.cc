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
#include "jsrtdebug.h"

/////////////////////////////////////////////////

#ifdef _WIN32
#include <io.h>

typedef wchar_t TTDHostCharType;
typedef struct _wfinddata_t TTDHostFileInfo;
typedef intptr_t TTDHostFindHandle;
typedef struct _stat TTDHostStatType;

#define TTDHostPathSeparator L"\\"
#define TTDHostPathSeparatorChar L'\\'
#define TTDHostFindInvalid -1

size_t TTDHostStringLength(const TTDHostCharType* str) {
    return wcslen(str);
}

void TTDHostInitEmpty(TTDHostCharType* dst) {
    dst[0] = L'\0';
}

void TTDHostInitFromUriBytes(TTDHostCharType* dst, const byte* uriBytes,
    size_t uriBytesLength) {
    memcpy_s(dst, MAX_PATH * sizeof(TTDHostCharType), uriBytes, uriBytesLength);
    dst[uriBytesLength / sizeof(TTDHostCharType)] = L'\0';
}

void TTDConvertToHostString(TTDHostCharType* dst, size_t dstSize,
    const char* srcUri) {
    mbstowcs(dst, srcUri, dstSize);
}

void TTDHostAppend(TTDHostCharType* dst, const TTDHostCharType* src) {
    size_t dpos = TTDHostStringLength(dst);
    size_t srcLength = TTDHostStringLength(src);
    size_t srcByteLength = srcLength * sizeof(TTDHostCharType);

    memcpy_s(dst + dpos, srcByteLength, src, srcByteLength);
    dst[dpos + srcLength] = L'\0';
}

void TTDHostAppendAscii(TTDHostCharType* dst, const char* src) {
    size_t dpos = TTDHostStringLength(dst);
    size_t srcLength = strlen(src);
    for (size_t i = 0; i < srcLength; ++i) {
        dst[dpos + i] = (TTDHostCharType)src[i];
    }
    dst[dpos + srcLength] = L'\0';
}

void TTDHostBuildCurrentExeDirectory(TTDHostCharType* path,
    size_t pathBufferLength) {
    TTDHostCharType exePath[MAX_PATH];
    GetModuleFileNameW(NULL, exePath, MAX_PATH);

    size_t i = wcslen(exePath) - 1;
    while (exePath[i] != TTDHostPathSeparatorChar) {
        --i;
    }
    exePath[i + 1] = L'\0';

    TTDHostAppend(path, exePath);
}

JsTTDStreamHandle TTDHostOpen(const TTDHostCharType* path, bool isWrite) {
    FILE* res = nullptr;
    _wfopen_s(&res, path, isWrite ? L"w+b" : L"r+b");

    return (JsTTDStreamHandle)res;
}

#define TTDHostCWD(dst) _wgetcwd(dst, MAX_PATH)
#define TTDDoPathInit(dst)
#define TTDHostTok(opath, TTDHostPathSeparator, context) wcstok_s(opath, TTDHostPathSeparator, context)
#define TTDHostStat(cpath, statVal) _wstat(cpath, statVal)

#define TTDHostMKDir(cpath) _wmkdir(cpath)
#define TTDHostCHMod(cpath, flags) _wchmod(cpath, flags)
#define TTDHostRMFile(cpath) _wremove(cpath)

#define TTDHostFindFirst(strPattern, FileInformation) _wfindfirst(strPattern, FileInformation)
#define TTDHostFindNext(hFile, FileInformation) _wfindnext(hFile, FileInformation)
#define TTDHostFindClose(hFile) _findclose(hFile)

#define TTDHostDirInfoName(FileInformation) FileInformation.name

#define TTDHostRead(buff, size, handle) fread_s(buff, size, 1, size, (FILE*)handle);
#define TTDHostWrite(buff, size, handle) fwrite(buff, 1, size, (FILE*)handle)
#else
#include <unistd.h>
#include <cstring>
#include <libgen.h>
#include <dirent.h>

#ifdef __APPLE__
#include <mach-o/dyld.h>
#endif

#define MAX_PATH 256
#define MAXDWORD 0xffffffff
#define memcpy_s(DST, DSTLENGTH, SRC, COUNT) memcpy(DST, SRC, COUNT)
#define CALLBACK 

typedef char TTDHostCharType;
typedef struct dirent* TTDHostFileInfo;
typedef DIR* TTDHostFindHandle;
typedef struct stat TTDHostStatType;

#define TTDHostPathSeparator ((TTDHostCharType*)"/")
#define TTDHostPathSeparatorChar ((TTDHostCharType)'/')
#define TTDHostFindInvalid nullptr

#define TTDHostCharConvert(X) ((char*)X)
#define TTDHostUtf8CharConvert(X) ((TTDHostCharType*)X)

size_t TTDHostStringLength(const TTDHostCharType* str) {
    return strlen(TTDHostCharConvert(str));
}

void TTDHostInitEmpty(TTDHostCharType* dst) {
    dst[0] = '\0';
}

void TTDConvertToHostString(TTDHostCharType* dst, size_t dstSize,
    const char* srcUri) {
    size_t srcUriLength = strlen(srcUri);
    memcpy_s(dst, dstSize * sizeof(TTDHostCharType), srcUri, srcUriLength);
    dst[srcUriLength / sizeof(TTDHostCharType)] = '\0';
}

void TTDHostInitFromUriBytes(TTDHostCharType* dst, const byte* uriBytes,
    size_t uriBytesLength) {
    memcpy_s(dst, MAX_PATH * sizeof(TTDHostCharType), uriBytes, uriBytesLength);
    dst[uriBytesLength / sizeof(TTDHostCharType)] = '\0';
}

void TTDHostAppend(TTDHostCharType* dst, const TTDHostCharType* src) {
    size_t dpos = TTDHostStringLength(dst);
    size_t srcLength = TTDHostStringLength(src);
    size_t srcByteLength = srcLength * sizeof(TTDHostCharType);

    memcpy_s(dst + dpos, srcByteLength, src, srcByteLength);
    dst[dpos + srcLength] = '\0';
}

void TTDHostAppendAscii(TTDHostCharType* dst, const char* src) {
    size_t dpos = TTDHostStringLength(dst);
    size_t srcLength = strlen(src);
    size_t srcByteLength = srcLength * sizeof(TTDHostCharType);

    memcpy_s(dst + dpos, srcByteLength, src, srcByteLength);
    dst[dpos + srcLength] = '\0';
}

void TTDHostBuildCurrentExeDirectory(TTDHostCharType* path, 
    size_t pathBufferLength) {
    TTDHostCharType exePath[MAX_PATH];
    // TODO: xplattodo move this logic to PAL
#ifdef __APPLE__
    uint32_t tmpPathSize = sizeof(exePath);
    _NSGetExecutablePath(TTDHostCharConvert(exePath), &tmpPathSize);
    size_t len = TTDHostStringLength(exePath);
#else
    size_t len = readlink("/proc/self/exe", TTDHostCharConvert(exePath), MAX_PATH);
#endif

    size_t i = len - 1;
    while (exePath[i] != TTDHostPathSeparatorChar) {
        --i;
    }
    exePath[i + 1] = '\0';

    TTDHostAppend(path, exePath);
}

JsTTDStreamHandle TTDHostOpen(const TTDHostCharType* path, bool isWrite) {
    return (JsTTDStreamHandle)fopen(TTDHostCharConvert(path), isWrite ? "w+b" : "r+b");
}

#define TTDHostCWD(dst) TTDHostUtf8CharConvert(getcwd(TTDHostCharConvert(dst), MAX_PATH))
#define TTDDoPathInit(dst) TTDHostAppend(dst, TTDHostPathSeparator)
#define TTDHostTok(opath, TTDHostPathSeparator, context) TTDHostUtf8CharConvert(strtok(TTDHostCharConvert(opath), TTDHostCharConvert(TTDHostPathSeparator)))
#define TTDHostStat(cpath, statVal) stat(TTDHostCharConvert(cpath), statVal)

#define TTDHostMKDir(cpath) mkdir(TTDHostCharConvert(cpath), 0777)
#define TTDHostCHMod(cpath, flags) chmod(TTDHostCharConvert(cpath), flags)
#define TTDHostRMFile(cpath) remove(TTDHostCharConvert(cpath))

#define TTDHostFindFirst(strPattern, FileInformation) opendir(TTDHostCharConvert(strPattern))
#define TTDHostFindNext(hFile, FileInformation) (*FileInformation = readdir(hFile))
#define TTDHostFindClose(hFile) closedir(hFile)

#define TTDHostDirInfoName(FileInformation) TTDHostUtf8CharConvert(FileInformation->d_name)

#define TTDHostRead(buff, size, handle) fread(buff, 1, size, (FILE*)handle)
#define TTDHostWrite(buff, size, handle) fwrite(buff, 1, size, (FILE*)handle)
#endif

void TTReportLastIOErrorAsNeeded(bool ok, const char* msg) {
    if (!ok) {
#ifdef _WIN32
        DWORD lastError = GetLastError();
        LPTSTR pTemp = NULL;
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_ARGUMENT_ARRAY,
            NULL, lastError, 0, (LPTSTR)&pTemp, 0, NULL);
        fprintf(stderr, "Error is: %i %s\n", lastError, pTemp);
#else
        fprintf(stderr, "Error is: %i %s\n", errno, strerror(errno));
#endif
        fprintf(stderr, "Message is: %s\n", msg);
    }
}

void CreateDirectoryIfNeeded(size_t uriByteLength, const byte* uriBytes) {
    TTDHostCharType opath[MAX_PATH];
    TTDHostInitFromUriBytes(opath, uriBytes, uriByteLength);

    TTDHostCharType cpath[MAX_PATH];
    TTDHostInitEmpty(cpath);
    TTDDoPathInit(cpath);

    TTDHostStatType statVal;
    TTDHostCharType* context = nullptr;
    TTDHostCharType* token = TTDHostTok(opath, TTDHostPathSeparator, &context);
    TTDHostAppend(cpath, token);

    // At least 1 part of the path must exist so iterate until we find it
    while (TTDHostStat(cpath, &statVal) == -1) {
        token = TTDHostTok(nullptr, TTDHostPathSeparator, &context);
        TTDHostAppend(cpath, TTDHostPathSeparator);
        TTDHostAppend(cpath, token);
    }

    // Continue until we hit the part that doesn't exist (or end of the path)
    while (token != nullptr && TTDHostStat(cpath, &statVal) != -1) {
        token = TTDHostTok(nullptr, TTDHostPathSeparator, &context);
        if (token != nullptr) {
            TTDHostAppend(cpath, TTDHostPathSeparator);
            TTDHostAppend(cpath, token);
        }
    }

    // Now if there is path left then continue build up the directory tree as we go
    while (token != nullptr) {
        TTDHostMKDir(cpath);

        token = TTDHostTok(nullptr, TTDHostPathSeparator, &context);
        if (token != nullptr) {
            TTDHostAppend(cpath, TTDHostPathSeparator);
            TTDHostAppend(cpath, token);
        }
    }
}

void CleanDirectory(size_t uriByteLength, const byte* uriBytes) {
    TTDHostFindHandle hFile;
    TTDHostFileInfo FileInformation = { 0 };

    TTDHostCharType strPattern[MAX_PATH];
    TTDHostInitFromUriBytes(strPattern, uriBytes, uriByteLength);
    TTDHostAppendAscii(strPattern, "*.*");

    hFile = TTDHostFindFirst(strPattern, &FileInformation);
    if (hFile != TTDHostFindInvalid) {
        do {
            if (TTDHostDirInfoName(FileInformation)[0] != '.') {
                TTDHostCharType strFilePath[MAX_PATH];
                TTDHostInitFromUriBytes(strFilePath, uriBytes, uriByteLength);
                TTDHostAppend(strFilePath, TTDHostDirInfoName(FileInformation));

                // Set file attributes
                TTDHostCHMod(strFilePath, S_IREAD | S_IWRITE);
                TTDHostRMFile(strFilePath);
            }
        } while (TTDHostFindNext(hFile, &FileInformation) != TTDHostFindInvalid);

        // Close handle
        TTDHostFindClose(hFile);
    }
}

void GetTTDDirectory(const char* curi, size_t* uriByteLength, byte* uriBytes) {
    TTDHostCharType turi[MAX_PATH];
    TTDHostInitEmpty(turi);

    TTDHostCharType wcuri[MAX_PATH];
    TTDConvertToHostString(wcuri, MAX_PATH, curi);

    if (curi[0] != '~') {
        TTDHostCWD(turi);
        TTDHostAppend(turi, TTDHostPathSeparator);

        TTDHostAppend(turi, wcuri);
    } else {
        TTDHostBuildCurrentExeDirectory(turi, MAX_PATH);

        TTDHostAppendAscii(turi, "_ttdlog");
        TTDHostAppend(turi, TTDHostPathSeparator);

        TTDHostAppend(turi, wcuri + 1);
    }

    // Add a path separator if one is not already present
    if (curi[strlen(curi) - 1] != (char)TTDHostPathSeparatorChar) {
        TTDHostAppend(turi, TTDHostPathSeparator);
    }

    size_t turiLength = TTDHostStringLength(turi);

    size_t byteLengthWNull = (turiLength + 1) * sizeof(TTDHostCharType);
    memcpy_s(uriBytes, byteLengthWNull, turi, byteLengthWNull);

    *uriByteLength = turiLength * sizeof(TTDHostCharType);
}

void CALLBACK TTInitializeForWriteLogStreamCallback(size_t uriByteLength,
    const byte* uriBytes) {
    // If the directory does not exist then we want to create it
    CreateDirectoryIfNeeded(uriByteLength, uriBytes);

    // Clear the logging directory so it is ready for us to write into
    CleanDirectory(uriByteLength, uriBytes);
}

bool g_ttdUseRelocatedSources = false;

JsTTDStreamHandle CALLBACK TTCreateStreamCallback(size_t uriByteLength,
    const byte* uriBytes, const char* asciiResourceName, bool read, bool write,
    byte** relocatedUri, size_t* relocatedUriLength) {
    void* res = nullptr;
    TTDHostCharType path[MAX_PATH];
    TTDHostInitFromUriBytes(path, uriBytes, uriByteLength);
    TTDHostAppendAscii(path, asciiResourceName);

#if _WIN32
    if (g_ttdUseRelocatedSources && relocatedUri != nullptr) {
        size_t bytelen = (TTDHostStringLength(path) + 1) * sizeof(TTDHostCharType);
        *relocatedUriLength = TTDHostStringLength(path);
        *relocatedUri = (byte*)CoTaskMemAlloc(bytelen);

        TTDHostInitEmpty((TTDHostCharType*)*relocatedUri);
        TTDHostAppend((TTDHostCharType*)*relocatedUri, path);
    }
#endif

    res = TTDHostOpen(path, write);
    if (res == nullptr) {
#if _WIN32
        fwprintf(stderr, L"Filename: %ls\n", (wchar_t*)path);
#else
        fprintf(stderr, "Filename: %s\n", (char*)path);
#endif
    }

    TTReportLastIOErrorAsNeeded(res != nullptr, "Failed File Open");
    return res;
}

bool CALLBACK TTReadBytesFromStreamCallback(JsTTDStreamHandle handle,
    byte* buff, size_t size, size_t* readCount) {
    if (size > MAXDWORD) {
        *readCount = 0;
        return false;
    }

    bool ok = false;
    *readCount = TTDHostRead(buff, size, (FILE*)handle);
    ok = (*readCount != 0);

    TTReportLastIOErrorAsNeeded(ok, "Failed Read!!!");

    return ok ? true : false;
}

bool CALLBACK TTWriteBytesToStreamCallback(JsTTDStreamHandle handle,
    const byte* buff, size_t size, size_t* writtenCount) {
    if (size > MAXDWORD) {
        *writtenCount = 0;
        return false;
    }

    bool ok = false;
    *writtenCount = TTDHostWrite(buff, size, (FILE*)handle);
    ok = (*writtenCount == size);

    TTReportLastIOErrorAsNeeded(ok, "Failed Read!!!");

    return ok ? true : false;
}

void CALLBACK TTFlushAndCloseStreamCallback(JsTTDStreamHandle handle,
    bool read, bool write) {
    fflush((FILE*)handle);
    fclose((FILE*)handle);
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

/* static */ v8::Isolate * IsolateShim::New(const char* uri,
    bool doRecord, bool doReplay, bool doDebug, bool useRelocatedSrc,
    uint32_t snapInterval, uint32_t snapHistoryLength) {
  bool disableIdleGc = v8::g_disableIdleGc;
    JsRuntimeAttributes attributes = static_cast<JsRuntimeAttributes>(
      JsRuntimeAttributeAllowScriptInterrupt |
      JsRuntimeAttributeEnableExperimentalFeatures |
        (disableIdleGc ? JsRuntimeAttributeNone : JsRuntimeAttributeEnableIdleProcessing));

  JsRuntimeHandle runtime;
  JsErrorCode error;
  if (uri == nullptr) {
      error = JsCreateRuntime(attributes, nullptr, &runtime);
  } else {
    g_ttdUseRelocatedSources = useRelocatedSrc;

    byte ttUri[MAX_PATH * sizeof(TTDHostCharType)];
    size_t ttUriByteLength = 0;
    GetTTDDirectory(uri, &ttUriByteLength, ttUri);

    if (doRecord) {
      error = JsTTDCreateRecordRuntime(attributes, ttUri, ttUriByteLength,
                                       snapInterval, snapHistoryLength,
                                       &TTInitializeForWriteLogStreamCallback,
                                       &TTCreateStreamCallback,
                                       &TTReadBytesFromStreamCallback,
                                       &TTWriteBytesToStreamCallback,
                                       &TTFlushAndCloseStreamCallback,
                                       nullptr, &runtime);
    } else {
      error = JsTTDCreateReplayRuntime(attributes, ttUri, ttUriByteLength,
                                       doDebug, 
                                       &TTInitializeForWriteLogStreamCallback,
                                       &TTCreateStreamCallback,
                                       &TTReadBytesFromStreamCallback,
                                       &TTWriteBytesToStreamCallback,
                                       &TTFlushAndCloseStreamCallback,
                                       nullptr, &runtime);
    }
  }

  if (error != JsNoError) {
    return nullptr;
  }

  if (Debugger::IsDebugEnabled()) {
    // If JavaScript debugging APIs need to be exposed then
    // runtime should be in debugging mode from start
    Debugger::StartDebugging(runtime);
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

JsValueRef IsolateShim::GetChakraShimJsArrayBuffer() {
  JsValueRef chakraShimArrayBuffer;
  CHAKRA_VERIFY(JsCreateExternalArrayBuffer(
                (void*)chakra_shim_native,
                sizeof(chakra_shim_native),
                nullptr, nullptr,
                &chakraShimArrayBuffer) == JsNoError);
  return chakraShimArrayBuffer;
}

JsValueRef IsolateShim::GetChakraDebugShimJsArrayBuffer() {
  JsValueRef chakraDebugShimArrayBuffer;
  CHAKRA_VERIFY(JsCreateExternalArrayBuffer(
                (void*)chakra_debug_native,
                sizeof(chakra_debug_native),
                nullptr, nullptr,
                &chakraDebugShimArrayBuffer) == JsNoError);
  return chakraDebugShimArrayBuffer;
}

/*static*/
bool IsolateShim::RunSingleStepOfReverseMoveLoop(v8::Isolate* isolate,
                                                 uint64_t* moveMode,
                                                 int64_t* nextEventTime) {
    int64_t snapEventTime = -1;
    int64_t snapEventEndTime = -1;
    int64_t origNETime = *nextEventTime;
    JsTTDMoveMode _moveMode = (JsTTDMoveMode)(*moveMode);
    JsRuntimeHandle rHandle = jsrt::IsolateShim::FromIsolate(isolate)->GetRuntimeHandle();

    // if mode is reverse continue then we need to scan
    if ((_moveMode & JsTTDMoveMode::JsTTDMoveScanIntervalForContinue)
                     == JsTTDMoveMode::JsTTDMoveScanIntervalForContinue) {
        int64_t ciStart = -1;
        int64_t ciEnd = -1;
        JsTTDGetSnapShotBoundInterval(rHandle, *nextEventTime,
                                      &ciStart, &ciEnd);

        *nextEventTime = -1;
        JsTTDPreExecuteSnapShotInterval(rHandle, ciStart, ciEnd,
                                        ((JsTTDMoveMode)(*moveMode)),
                                        nextEventTime);

        while (*nextEventTime == -1) {
            int64_t newCiStart = -1;
            JsTTDGetPreviousSnapshotInterval(rHandle, ciStart, &newCiStart);

            if (newCiStart == -1) {
                // no previous so break on first
                _moveMode = (JsTTDMoveMode)(_moveMode |
                    JsTTDMoveMode::JsTTDMoveFirstEvent);
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

    JsErrorCode error = JsTTDGetSnapTimeTopLevelEventMove(rHandle, _moveMode,
                                                          nextEventTime,
                                                          &snapEventTime,
                                                          &snapEventEndTime);

    if (error != JsNoError) {
        if (error == JsErrorCategoryUsage) {
            printf("Start time not in log range.");
            exit(0);
        } else {
            printf("Fatal Error in Move!!!");
            exit(1);
        }
    }

    JsTTDMoveToTopLevelEvent(rHandle, _moveMode, snapEventTime, *nextEventTime);
    JsErrorCode res = JsTTDReplayExecution(&_moveMode, nextEventTime);

    // update before we return
    *moveMode = (uint64_t)_moveMode;
    if (*nextEventTime == -1) {
        printf("\nReached end of Execution -- Exiting.\n");
        return false;
    }
    return true;
}
}  // namespace jsrt
