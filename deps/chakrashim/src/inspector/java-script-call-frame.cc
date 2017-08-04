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
#include "src/jsrtinspectorhelpers.h"
#include "src/jsrtutils.h"

namespace v8_inspector {

using jsrt::InspectorHelpers;

JavaScriptCallFrame::JavaScriptCallFrame(JsValueRef callFrame)
    : m_callFrame(callFrame) {
  JsAddRef(m_callFrame, nullptr);
}

JavaScriptCallFrame::~JavaScriptCallFrame() {
  JsRelease(m_callFrame, nullptr);
}

int JavaScriptCallFrame::sourceID() const {
  int scriptId;
  CHAKRA_VERIFY_NOERROR(jsrt::GetProperty(m_callFrame,
                                          jsrt::CachedPropertyIdRef::scriptId,
                                          &scriptId));

  return scriptId;
}

int JavaScriptCallFrame::line() const {
  int line;
  CHAKRA_VERIFY_NOERROR(jsrt::GetProperty(m_callFrame,
                                          jsrt::CachedPropertyIdRef::line,
                                          &line));

  return line;
}

int JavaScriptCallFrame::column() const {
  int column;
  CHAKRA_VERIFY_NOERROR(jsrt::GetProperty(m_callFrame,
                                          jsrt::CachedPropertyIdRef::column,
                                          &column));

  return column;
}

int JavaScriptCallFrame::contextId() const {
  return 1;
}

bool JavaScriptCallFrame::isAtReturn() const {
  int index;
  CHAKRA_VERIFY_NOERROR(jsrt::GetProperty(m_callFrame,
                                          jsrt::CachedPropertyIdRef::index,
                                          &index));

  JsValueRef properties;
  CHAKRA_VERIFY_NOERROR(JsDiagGetStackProperties(index, &properties));

  bool hasProp;
  CHAKRA_VERIFY_NOERROR(jsrt::HasProperty(
      m_callFrame, jsrt::CachedPropertyIdRef::returnValue, &hasProp));

  if (!hasProp) {
    return false;
  }

  JsValueRef propVal;
  CHAKRA_VERIFY_NOERROR(jsrt::GetProperty(
      m_callFrame, jsrt::CachedPropertyIdRef::returnValue, &propVal));

  CHAKRA_VERIFY_NOERROR(jsrt::HasProperty(propVal,
                                          jsrt::CachedPropertyIdRef::handle,
                                          &hasProp));

  return hasProp;
}

v8::Local<v8::Object> JavaScriptCallFrame::details() const {
  return jsrt::InspectorHelpers::WrapCallFrameDetails(m_callFrame);
}

v8::MaybeLocal<v8::Value> JavaScriptCallFrame::evaluate(
    v8::Local<v8::Value> expression, bool returnByValue, bool* isError) {
  return jsrt::InspectorHelpers::EvaluateOnCallFrame(
      m_callFrame, reinterpret_cast<JsValueRef>(*expression), returnByValue,
      isError);
}

v8::MaybeLocal<v8::Value> JavaScriptCallFrame::restart() {
  // CHAKRA-TODO - Figure out what to do here.
  CHAKRA_UNIMPLEMENTED();
  return v8::MaybeLocal<v8::Value>();
}

v8::MaybeLocal<v8::Value> JavaScriptCallFrame::setVariableValue(
    int scopeNumber, v8::Local<v8::Value> variableName,
    v8::Local<v8::Value> newValue) {
  // CHAKRA-TODO - Figure out what to do here.
  CHAKRA_UNIMPLEMENTED();
  return v8::MaybeLocal<v8::Value>();
}

}  // namespace v8_inspector
