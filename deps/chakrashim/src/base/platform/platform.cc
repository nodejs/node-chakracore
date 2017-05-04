// Copyright 2012 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Platform-specific code for Win32.

// Secure API functions are not available using MinGW with msvcrt.dll
// on Windows XP. Make sure MINGW_HAS_SECURE_API is not defined to
// disable definition of secure API functions in standard headers that
// would conflict with our own implementation.
#ifdef __MINGW32__
#include <_mingw.h>
#ifdef MINGW_HAS_SECURE_API
#undef MINGW_HAS_SECURE_API
#endif  // MINGW_HAS_SECURE_API
#endif  // __MINGW32__

#include <limits>

#include "src/base/macros.h"
#include "src/base/platform/platform.h"


namespace v8 {
namespace base {


// Print error message to console.
void OS::PrintError(const char* format, ...) {
}


void OS::VPrintError(const char* format, va_list args) {
}


int OS::SNPrintF(char* str, int length, const char* format, ...) {
  va_list args;
  va_start(args, format);
  int result = vsnprintf(str, length, format, args);
  va_end(args);
  return result;
}


void OS::Abort() {
  // Make sure function doesn't return.
  abort();
}

}  // namespace base
}  // namespace v8
