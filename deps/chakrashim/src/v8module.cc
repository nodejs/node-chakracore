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

#include "v8chakra.h"

// CHAKRA-TODO: Find a way to support es6 modules under
// the constraints of the node implementation

namespace v8 {
  Module::Status Module::GetStatus() const {
    return kUninstantiated;
  }

  int Module::GetModuleRequestsLength() const {
      return 0;
  }

  Local<String> Module::GetModuleRequest(int i) const {
      return Local<String>();
  }

  int Module::GetIdentityHash() const {
      return 0;
  }

  bool Module::Instantiate(Local<Context> context, ResolveCallback callback) {
      return false;
  }

  Maybe<bool> Module::InstantiateModule(Local<Context> context,
    ResolveCallback callback) {
      return Just(false);
  }

  MaybeLocal<Value> Module::Evaluate(Local<Context> context) {
      return Local<Value>();
  }

  Local<Value> Module::GetModuleNamespace() {
    return Local<Value>();
  }

}  // namespace v8
