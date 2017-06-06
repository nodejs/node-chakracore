// Copyright 2016 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DEPS_CHAKRASHIM_SRC_INSPECTOR_PROTOCOL_PLATFORM_H_
#define DEPS_CHAKRASHIM_SRC_INSPECTOR_PROTOCOL_PLATFORM_H_

#include <memory>

#include "src/base/logging.h"

namespace v8_inspector {

template <typename T>
std::unique_ptr<T> wrapUnique(T* ptr) {
  return std::unique_ptr<T>(ptr);
}

}  // namespace v8_inspector

#endif  // DEPS_CHAKRASHIM_SRC_INSPECTOR_PROTOCOL_PLATFORM_H_
