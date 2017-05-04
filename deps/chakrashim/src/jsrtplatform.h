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

#pragma once

#include <map>
#include <queue>

#include "v8-platform.h"
#include <uv.h>

namespace jsrt {

class DefaultPlatform : public v8::Platform {
 public:
  DefaultPlatform();
  virtual ~DefaultPlatform();

  bool PumpMessageLoop(v8::Isolate* isolate);

  virtual void CallOnBackgroundThread(v8::Task* task,
                                      ExpectedRuntime expected_runtime) override;
  virtual void CallOnForegroundThread(v8::Isolate* isolate, v8::Task* task) override;
  virtual double MonotonicallyIncreasingTime() override;

 private:
  v8::Task* PopTaskInMainThreadQueue(v8::Isolate* isolate);
  uv_mutex_t m_lock;
  std::map<v8::Isolate*, std::queue<v8::Task*> > main_thread_queue_;
};

}  // namespace jsrt
