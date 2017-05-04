/*
 * Copyright (c) 2010, Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "src/inspector/java-script-call-frame.h"
#include <assert.h>

#include "src/inspector/string-util.h"

#include "include/v8-debug.h"
#include "src/jsrtinspectorhelpers.h"

namespace v8_inspector {

using jsrt::InspectorHelpers;

JavaScriptCallFrame::JavaScriptCallFrame(v8::Local<v8::Context> debuggerContext,
                                         JsValueRef callFrame)
    : m_isolate(debuggerContext->GetIsolate()),
      m_debuggerContext(m_isolate, debuggerContext),
      m_callFrame(callFrame) {
  JsAddRef(m_callFrame, nullptr);
}

JavaScriptCallFrame::~JavaScriptCallFrame() {
  JsRelease(m_callFrame, nullptr);
}

int JavaScriptCallFrame::sourceID() const {
  int scriptId;
  if (InspectorHelpers::GetIntProperty(m_callFrame, "scriptId", &scriptId) != JsNoError) {
    assert(false);
    return 0;
  }

  return scriptId;
}

int JavaScriptCallFrame::line() const {
  int line;
  if (InspectorHelpers::GetIntProperty(m_callFrame, "line", &line) != JsNoError) {
    assert(false);
    return 0;
  }

  return line;
}

int JavaScriptCallFrame::column() const {
  int column;
  if (InspectorHelpers::GetIntProperty(m_callFrame, "column", &column) != JsNoError) {
    assert(false);
    return 0;
  }

  return column;
}

int JavaScriptCallFrame::contextId() const {
  return 1;
}

bool JavaScriptCallFrame::isAtReturn() const {
  int index;
  if (InspectorHelpers::GetIntProperty(m_callFrame, "index", &index) != JsNoError) {
    assert(false);
    return false;
  }

  JsValueRef properties;
  if (JsDiagGetStackProperties(index, &properties) != JsNoError) {
    assert(false);
    return false;
  }

  bool hasProp;
  if (InspectorHelpers::HasProperty(m_callFrame, "returnValue", &hasProp) != JsNoError) {
    assert(false);
    return false;
  }

  if (!hasProp) {
    return false;
  }

  JsValueRef propVal;
  if (InspectorHelpers::GetProperty(m_callFrame, "returnValue", &propVal) != JsNoError) {
    assert(false);
    return false;
  }

  if (InspectorHelpers::HasProperty(propVal, "handle", &hasProp) != JsNoError) {
    assert(false);
    return false;
  }

  return hasProp;
}

v8::Local<v8::Object> JavaScriptCallFrame::details() const {
  return jsrt::InspectorHelpers::WrapCallFrameDetails(m_callFrame);
}

v8::MaybeLocal<v8::Value> JavaScriptCallFrame::evaluate(
    v8::Local<v8::Value> expression, bool* isError) {
  return jsrt::InspectorHelpers::EvaluateOnCallFrame(
      m_callFrame, reinterpret_cast<JsValueRef>(*expression), false, isError);
}

v8::MaybeLocal<v8::Value> JavaScriptCallFrame::restart() {
  // CHAKRA-TODO - Figure out what to do here.
  assert(false);
  return v8::MaybeLocal<v8::Value>();
}

v8::MaybeLocal<v8::Value> JavaScriptCallFrame::setVariableValue(
    int scopeNumber, v8::Local<v8::Value> variableName,
    v8::Local<v8::Value> newValue) {
  // CHAKRA-TODO - Figure out what to do here.
  assert(false);
  return v8::MaybeLocal<v8::Value>();
}

}  // namespace v8_inspector
