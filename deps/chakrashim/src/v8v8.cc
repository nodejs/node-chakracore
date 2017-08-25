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

#include <algorithm>
#include "v8.h"
#include "v8chakra.h"
#include "jsrtutils.h"
#include "v8-debug.h"
#include "libplatform/libplatform.h"
#include "libplatform/v8-tracing.h"

#ifndef _WIN32
#include "ChakraCoreVersion.h"
#endif

namespace v8 {

static const size_t kMaxVersionLength = 32;

bool g_disposed = false;
bool g_exposeGC = false;
bool g_useStrict = false;
bool g_disableIdleGc = false;
bool g_trace_debug_json = false;

HeapStatistics::HeapStatistics()
    : total_heap_size_(0),
      total_heap_size_executable_(0),
      total_physical_size_(0),
      total_available_size_(0),
      used_heap_size_(0),
      heap_size_limit_(0),
      malloced_memory_(0),
      peak_malloced_memory_(0),
      does_zap_garbage_(0) {}

const char *V8::GetVersion() {
  static char versionStr[kMaxVersionLength] = {};

  if (versionStr[0] == '\0') {
#ifdef _WIN32
    HMODULE hModule;
    if (GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                          TEXT(NODE_ENGINE), &hModule)) {
      WCHAR filename[_MAX_PATH];  // NOLINT(runtime/arrays)
      DWORD len = GetModuleFileNameW(hModule, filename, _countof(filename));
      if (len > 0) {
        DWORD dwHandle = 0;
        DWORD size = GetFileVersionInfoSizeExW(0, filename, &dwHandle);
        if (size > 0) {
          std::unique_ptr<BYTE[]> info(new BYTE[size]);
          if (GetFileVersionInfoExW(0, filename, dwHandle, size, info.get())) {
            UINT len = 0;
            VS_FIXEDFILEINFO* vsfi = nullptr;
            if (VerQueryValueW(info.get(),
                               L"\\", reinterpret_cast<LPVOID*>(&vsfi), &len)) {
              sprintf_s(versionStr, "%d.%d.%d.%d",
                        HIWORD(vsfi->dwFileVersionMS),
                        LOWORD(vsfi->dwFileVersionMS),
                        HIWORD(vsfi->dwFileVersionLS),
                        LOWORD(vsfi->dwFileVersionLS));
            }
          }
        }
      }
    }
#else
  snprintf(versionStr, kMaxVersionLength, "%d.%d.%d.%d",
           CHAKRA_CORE_MAJOR_VERSION, CHAKRA_CORE_MINOR_VERSION,
           CHAKRA_CORE_VERSION_RELEASE, CHAKRA_CORE_VERSION_RELEASE_QFE);
#endif
  }

  return versionStr;
}

void Isolate::SetFatalErrorHandler(FatalErrorCallback that) {
  // CONSIDER: Ignoring for now, since we don't have an equivalent concept.
}

void V8::SetFlagsFromString(const char* str, int length) {
  // CHAKRA-TODO
}

static bool equals(const char* str, const char* pat) {
  return strcmp(str, pat) == 0;
}

template <size_t N>
static bool startsWith(const char* str, const char (&prefix)[N]) {
  return strncmp(str, prefix, N - 1) == 0;
}




void V8::SetFlagsFromCommandLine(int *argc, char **argv, bool remove_flags) {
  for (int i = 1; i < *argc; i++) {
    // Note: Node now exits on invalid options. We may not recognize V8 flags
    // and fail here, causing Node to exit.
    char *arg = argv[i];
    if (equals("--expose-gc", arg) || equals("--expose_gc", arg)) {
      g_exposeGC = true;
      if (remove_flags) {
        argv[i] = nullptr;
      }
    } else if (equals("--use-strict", arg) || equals("--use_strict", arg)) {
      g_useStrict = true;
      if (remove_flags) {
        argv[i] = nullptr;
      }
    } else if (equals("--off-idlegc", arg) || equals("--off_idlegc", arg)) {
      g_disableIdleGc = true;
      if (remove_flags) {
        argv[i] = nullptr;
      }
    } else if (equals("--trace-debug-json", arg) ||
      equals("--trace_debug_json", arg)) {
      g_trace_debug_json = true;
      if (remove_flags) {
        argv[i] = nullptr;
      }
    } else if (remove_flags &&
               (startsWith(
                 arg, "--debug")  // Ignore some flags to reduce unit test noise
                || startsWith(arg, "--harmony")
                || startsWith(arg, "--stack-size=")
                || startsWith(arg, "--nolazy"))) {
      argv[i] = nullptr;
    } else if (equals("--help", arg)) {
        printf(
          "Options:\n"
          " --use_strict (enforce strict mode)\n"
          "     type: bool  default: false\n"
          " --expose_gc (expose gc extension)\n"
          "     type: bool  default: false\n"
          " --off_idlegc (turn off idle GC)\n"
          " --harmony_simd (enable \"harmony simd\" (in progress))\n"
          " --harmony (Other flags are ignored in node running with "
          "chakracore)\n"
          " --debug (Ignored in node running with chakracore)\n"
          " --stack-size (Ignored in node running with chakracore)\n");
        exit(0);
    }
  }

  if (remove_flags) {
    char** end = std::remove(argv + 1, argv + *argc, nullptr);
    *argc = static_cast<int>(end - argv);
  }
}

bool V8::Initialize() {
  if (g_disposed) {
    return false;  // Can no longer Initialize if Disposed
  }

  return true;
}

void V8::SetEntropySource(EntropySource entropy_source) {
  // CHAKRA-TODO
}

bool V8::IsDead() {
  return g_disposed;
}

bool V8::Dispose() {
  g_disposed = true;
  jsrt::IsolateShim::DisposeAll();
  return true;
}

void V8::TerminateExecution(Isolate* isolate) {
  isolate->TerminateExecution();
}

bool V8::IsExeuctionDisabled(Isolate* isolate) {
  return jsrt::IsolateShim::FromIsolate(isolate)->IsExeuctionDisabled();
}

void V8::CancelTerminateExecution(Isolate* isolate) {
  isolate->CancelTerminateExecution();
}

void V8::FromJustIsNothing() {
  jsrt::Fatal("v8::FromJust: %s", "Maybe value is Nothing.");
}

void V8::ToLocalEmpty() {
  jsrt::Fatal("v8::ToLocalChecked: %s", "Empty MaybeLocal.");
}

namespace platform {
  void SetTracingController(
      v8::Platform* platform,
      v8::platform::tracing::TracingController* tracing_controller) {
    jsrt::Unimplemented("TracingController");
  }
}  // namespace platform

namespace platform {
namespace tracing {
  void TracingController::StopTracing() {
    jsrt::Unimplemented("TracingController");
  }

  void TracingController::StartTracing(TraceConfig*) {
    jsrt::Unimplemented("TracingController");
  }

  void TracingController::Initialize(TraceBuffer*) {
    jsrt::Unimplemented("TracingController");
  }

  const uint8_t* TracingController::GetCategoryGroupEnabled(
      const char* category_group) {
    jsrt::Unimplemented("TracingController");
    return nullptr;
  }

  void TracingController::UpdateTraceEventDuration(
      const uint8_t* category_enabled_flag, const char* name, uint64_t handle) {
    jsrt::Unimplemented("TracingController");
  }

  void TraceConfig::AddIncludedCategory(char const*) {
    jsrt::Unimplemented("TraceConfig");
  }

  TraceObject* TraceBufferChunk::AddTraceEvent(size_t*) {
    jsrt::Unimplemented("TraceBufferChunk");
    return 0;
  }

  void TraceBufferChunk::Reset(uint32_t) {
    jsrt::Unimplemented("TraceBufferChunk");
  }

  TraceBufferChunk::TraceBufferChunk(uint32_t) {
    jsrt::Unimplemented("TraceBufferChunk");
  }

  TraceObject::~TraceObject() {
    // Intentionally left empty to suppress warning C4722.
  }

  TraceWriter* TraceWriter::CreateJSONTraceWriter(std::ostream&) {
    jsrt::Unimplemented("TraceWriter");
    return 0;
  }
}  // namespace tracing
}  // namespace platform
}  // namespace v8
