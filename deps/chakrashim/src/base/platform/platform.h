// Copyright 2012 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This module contains the platform-specific code. This make the rest of the
// code less dependent on operating system, compilers and runtime libraries.
// This module does specifically not deal with differences between different
// processor architecture.
// The platform classes have the same definition for all platforms. The
// implementation for a particular platform is put in platform_<os>.cc.
// The build system then uses the implementation for the target platform.
//
// This design has been chosen because it is simple and fast. Alternatively,
// the platform dependent classes could have been implemented using abstract
// superclasses with virtual methods and having specializations for each
// platform. This design was rejected because it was more complicated and
// slower. It would require factory methods for selecting the right
// implementation and the overhead of virtual methods for performance
// sensitive like mutex locking/unlocking.

#ifndef DEPS_CHAKRASHIM_SRC_BASE_PLATFORM_PLATFORM_H_
#define DEPS_CHAKRASHIM_SRC_BASE_PLATFORM_PLATFORM_H_

#include <cstdarg>
#include "src/base/macros.h"

#if defined(__GNUC__)
#define PRINTF_FORMAT(format_param, dots_param) \
  __attribute__((format(printf, format_param, dots_param)))
#else
#define PRINTF_FORMAT(format_param, dots_param)
#endif

namespace v8 {
namespace base {

class OS {
 public:
  static PRINTF_FORMAT(1, 2) void PrintError(const char* format, ...);
  static PRINTF_FORMAT(1, 0) void VPrintError(const char* format, va_list args);
  static void Abort();
  static PRINTF_FORMAT(3, 4) int SNPrintF(char* str, int length,
      const char* format, ...);
 private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(OS);
};

}  // namespace base
}  // namespace v8

#endif  // DEPS_CHAKRASHIM_SRC_BASE_PLATFORM_PLATFORM_H_
