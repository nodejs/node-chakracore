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

namespace v8 {

THREAD_LOCAL HandleScope *current = nullptr;

HandleScope::HandleScope(Isolate* isolate)
    : _prev(current),
      _locals(),
      _count(0),
      _contextRef(JS_INVALID_REFERENCE),
      _addRefRecordHead(nullptr) {
  _locals[0] = JS_INVALID_REFERENCE;
  current = this;
}

HandleScope::~HandleScope() {
  current = _prev;

  AddRefRecord * currRecord = this->_addRefRecordHead;
  while (currRecord != nullptr) {
    AddRefRecord * nextRecord = currRecord->_next;
    // Don't crash even if we fail to release the scope
    JsErrorCode errorCode = JsRelease(currRecord->_ref, nullptr);
    CHAKRA_ASSERT(errorCode == JsNoError);
    UNUSED(errorCode);
    delete currRecord;
    currRecord = nextRecord;
  }
}

HandleScope *HandleScope::GetCurrent() {
  return current;
}

bool HandleScope::AddLocal(JsValueRef value) {
  // _locals is full, save them in _locals[0]
  if (_count == kOnStackLocals) {
    if (_locals[0] == JS_INVALID_REFERENCE) {
      if (JsCreateArray(0, &_locals[0]) != JsNoError) {
        return AddLocalAddRef(value);
      }
    }

    JsValueRef arrayPushFunction =
        jsrt::ContextShim::GetCurrent()->GetArrayPushFunction();

    JsValueRef result;
    if (JsCallFunction(arrayPushFunction, _locals,
                       _countof(_locals), &result) != JsNoError) {
      return AddLocalAddRef(value);
    }
    _count = 0;
  }
  _locals[1 + _count++] = value;
  return true;
}

bool HandleScope::AddLocalContext(JsContextRef value) {
  // JsContextRef are not javascript values, so we can't put them
  // in the array.  We will need to AddRef/Release them.

  if (_contextRef == JS_INVALID_REFERENCE) {
    // HandleScope are on the stack, so no need to AddRef/Release it.
    _contextRef = value;
    return true;
  }

  return AddLocalAddRef(value);
}

bool HandleScope::AddLocalAddRef(JsRef value) {
  if (JsAddRef(value, nullptr) != JsNoError) {
    return false;
  }

  AddRefRecord * newAddRefRecord = new AddRefRecord;
  newAddRefRecord->_ref = value;
  newAddRefRecord->_next = this->_addRefRecordHead;
  this->_addRefRecordHead = newAddRefRecord;
  return true;
}

}  // namespace v8
