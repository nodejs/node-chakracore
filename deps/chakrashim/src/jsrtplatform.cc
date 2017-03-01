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

#include "jsrtplatform.h"
#include "v8chakra.h"

namespace jsrt {

  DefaultPlatform::DefaultPlatform() {
    uv_mutex_init(&m_lock);
  }

  DefaultPlatform::~DefaultPlatform() {
    uv_mutex_destroy(&m_lock);
  }

  bool DefaultPlatform::PumpMessageLoop(v8::Isolate* isolate) {
    v8::Task* task = nullptr;

    uv_mutex_lock(&m_lock);

    task = PopTaskInMainThreadQueue(isolate);

    uv_mutex_unlock(&m_lock);

    if (task != nullptr) {
      task->Run();
      delete task;

      return true;
    }

    return false;
  }

  void DefaultPlatform::CallOnBackgroundThread(
      v8::Task* task,
      v8::Platform::ExpectedRuntime expected_runtime) {
    // CHAKRA-TODO: Figure out what to do here.
    CHAKRA_ASSERT(false);
  }

  void DefaultPlatform::CallOnForegroundThread(v8::Isolate* isolate,
                                               v8::Task* task) {
    uv_mutex_lock(&m_lock);

    main_thread_queue_[isolate].push(task);

    uv_mutex_unlock(&m_lock);
  }

  double DefaultPlatform::MonotonicallyIncreasingTime() {
    // CHAKRA-TODO: Figure out what to do here.
    CHAKRA_ASSERT(false);

    return 0;
  }

  v8::Task* DefaultPlatform::PopTaskInMainThreadQueue(v8::Isolate* isolate) {
    auto it = main_thread_queue_.find(isolate);
    if (it == main_thread_queue_.end() || it->second.empty()) {
      return nullptr;
    }
    v8::Task* task = it->second.front();
    it->second.pop();
    return task;
  }
}  // namespace jsrt
