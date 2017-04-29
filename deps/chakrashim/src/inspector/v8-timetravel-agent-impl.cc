// Copyright 2015 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "src/inspector/v8-timetravel-agent-impl.h"
#include "src/inspector/v8-inspector-session-impl.h"
#include "src/inspector/v8-debugger-agent-impl.h"
#include "src/jsrtinspector.h"

namespace v8_inspector {

V8TimeTravelAgentImpl::V8TimeTravelAgentImpl(
    V8InspectorSessionImpl* session, protocol::FrontendChannel* frontendChannel,
    protocol::DictionaryValue* state)
    : m_session(session),
      m_frontend(frontendChannel),
      m_state(state) {}

V8TimeTravelAgentImpl::~V8TimeTravelAgentImpl() {}

bool V8TimeTravelAgentImpl::checkEnabled(ErrorString* errorString) {
  if (enabled()) {
    return true;
  }

  *errorString = "TimeTravel agent is not enabled";
  return false;
}

bool V8TimeTravelAgentImpl::enabled() {
  return jsrt::Inspector::IsReplayDebugEnabled();
}

void V8TimeTravelAgentImpl::reverse(ErrorString* errorString) {
  if (!checkEnabled(errorString)) {
      return;
  }

  m_session->debuggerAgent()->reverse(errorString);
}

void V8TimeTravelAgentImpl::stepBack(ErrorString* errorString) {
  if (!checkEnabled(errorString)) {
      return;
  }

  m_session->debuggerAgent()->stepBack(errorString);
}

}  // namespace v8_inspector
