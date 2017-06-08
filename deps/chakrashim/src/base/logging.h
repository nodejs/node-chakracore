// Copyright 2012 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DEPS_CHAKRASHIM_SRC_BASE_LOGGING_H_
#define DEPS_CHAKRASHIM_SRC_BASE_LOGGING_H_

#include <cstring>
#include <sstream>
#include <string>

#include "v8config.h"

#if defined(__GNUC__)
#define PRINTF_FORMAT(format_param, dots_param) \
  __attribute__((format(printf, format_param, dots_param)))
#else
#define PRINTF_FORMAT(format_param, dots_param)
#endif

extern "C" PRINTF_FORMAT(3, 4)
void V8_Fatal(const char* file, int line, const char* format, ...);

#ifdef DEBUG
#define UNREACHABLE()                           \
  V8_Fatal(__FILE__, __LINE__, "unreachable code")
#else
#define UNREACHABLE() V8_Fatal("", 0, "unreachable code")
#endif


namespace v8 {
namespace base {

// CHECK dies with a fatal error if condition is not true.  It is *not*
// controlled by DEBUG, so the check will be executed regardless of
// compilation mode.
//
// We make sure CHECK et al. always evaluates their arguments, as
// doing CHECK(FunctionWithSideEffect()) is a common idiom.
#define CHECK(condition)                                             \
  do {                                                               \
    if (V8_UNLIKELY(!(condition))) {                                 \
      V8_Fatal(__FILE__, __LINE__, "Check failed: %s.", #condition); \
    }                                                                \
  } while (0)


#ifdef DEBUG

// Helper macro for binary operators.
// Don't use this macro directly in your code, use CHECK_EQ et al below.
#define CHECK_OP(name, op, lhs, rhs)                                    \
  do {                                                                  \
    if (std::string* _msg = ::v8::base::Check##name##Impl(              \
            (lhs), (rhs), #lhs " " #op " " #rhs)) {                     \
      V8_Fatal(__FILE__, __LINE__, "Check failed: %s.", _msg->c_str()); \
      delete _msg;                                                      \
    }                                                                   \
  } while (0)

#else

// Make all CHECK functions discard their log strings to reduce code
// bloat for official release builds.
#define CHECK_OP(name, op, lhs, rhs) CHECK((lhs)op(rhs))

#endif


// Build the error message string.  This is separate from the "Impl"
// function template because it is not performance critical and so can
// be out of line, while the "Impl" code should be inline. Caller
// takes ownership of the returned string.
template <typename Lhs, typename Rhs>
std::string* MakeCheckOpString(Lhs const& lhs, Rhs const& rhs,
    char const* msg) {
    std::ostringstream ss;
    ss << msg << " (" << lhs << " vs. " << rhs << ")";
    return new std::string(ss.str());
}

// Commonly used instantiations of MakeCheckOpString<>. Explicitly instantiated
// in logging.cc.
#define DEFINE_MAKE_CHECK_OP_STRING(type)                     \
  extern template std::string* MakeCheckOpString<type, type>( \
      type const&, type const&, char const*);
        DEFINE_MAKE_CHECK_OP_STRING(unsigned long long)  // NOLINT(runtime/int)
#undef DEFINE_MAKE_CHECK_OP_STRING


// Helper functions for CHECK_OP macro.
// The (int, int) specialization works around the issue that the compiler
// will not instantiate the template version of the function on values of
// unnamed enum type - see comment below.
// The (float, float) and (double, double) instantiations are explicitly
// externialized to ensure proper 32/64-bit comparisons on x86.
#define DEFINE_CHECK_OP_IMPL(NAME, op)                                         \
  template <typename Lhs, typename Rhs>                                        \
  V8_INLINE std::string* Check##NAME##Impl(Lhs const& lhs, Rhs const& rhs,     \
                                           char const* msg) {                  \
    return V8_LIKELY(lhs op rhs) ? nullptr : MakeCheckOpString(lhs, rhs, msg); \
  }                                                                            \
  V8_INLINE std::string* Check##NAME##Impl(int lhs, int rhs,                   \
                                           char const* msg) {                  \
    return V8_LIKELY(lhs op rhs) ? nullptr : MakeCheckOpString(lhs, rhs, msg); \
  }                                                                            \
  extern template std::string* Check##NAME##Impl<float, float>(                \
      float const& lhs, float const& rhs, char const* msg);                    \
  extern template std::string* Check##NAME##Impl<double, double>(              \
      double const& lhs, double const& rhs, char const* msg);
            DEFINE_CHECK_OP_IMPL(LT, <)
#undef DEFINE_CHECK_OP_IMPL

#define CHECK_LT(lhs, rhs) CHECK_OP(LT, <, lhs, rhs)

}  // namespace base
}  // namespace v8


// The DCHECK macro is equivalent to CHECK except that it only
// generates code in debug builds.
#ifdef DEBUG
#define DCHECK(condition)      CHECK(condition)
#define DCHECK_LT(v1, v2)      CHECK_LT(v1, v2)
#else
#define DCHECK(condition)      ((void) 0)
#define DCHECK_LT(v1, v2)      ((void) 0)
#endif

#endif  // DEPS_CHAKRASHIM_SRC_BASE_LOGGING_H_
