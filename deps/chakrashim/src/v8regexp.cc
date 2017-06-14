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

namespace v8 {

using jsrt::IsolateShim;
using jsrt::ContextShim;

static const size_t kMaxFlagCount = 3;

MaybeLocal<RegExp> RegExp::New(Local<Context> context, Handle<String> pattern,
                               Flags flags) {
  ContextShim *contextShim = IsolateShim::GetContextShim(
      (JsContextRef)*context);
  JsValueRef regExpConstructor = contextShim->GetRegExpConstructor();

  char flagsStr[kMaxFlagCount];
  size_t flagsCount = 0;

  if (flags & Flags::kGlobal) {
    flagsStr[flagsCount++] = 'g';
  }

  if (flags & Flags::kIgnoreCase) {
    flagsStr[flagsCount++] = 'i';
  }

  if (flags & Flags::kMultiline) {
    flagsStr[flagsCount++] = 'm';
  }

  CHAKRA_ASSERT(flagsCount <= kMaxFlagCount);

  JsValueRef flagsValue;
  if (JsCreateString(flagsStr, flagsCount, &flagsValue) != JsNoError) {
    return MaybeLocal<RegExp>();
  }

  JsValueRef newObj;
  if (jsrt::ConstructObject(regExpConstructor, (JsValueRef)*pattern, flagsValue,
                            &newObj) != JsNoError) {
    return MaybeLocal<RegExp>();
  }

  return Local<RegExp>::New(newObj);
}

}  // namespace v8
