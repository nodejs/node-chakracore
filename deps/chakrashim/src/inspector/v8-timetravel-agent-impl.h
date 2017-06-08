// Copyright 2015 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DEPS_CHAKRASHIM_SRC_INSPECTOR_V8_TIMETRAVEL_AGENT_IMPL_H_
#define DEPS_CHAKRASHIM_SRC_INSPECTOR_V8_TIMETRAVEL_AGENT_IMPL_H_

#include <vector>

#include "src/base/macros.h"
#include "src/inspector/protocol/TimeTravel.h"
#include "src/inspector/protocol/Forward.h"

namespace v8_inspector {

class V8Debugger;
class V8InspectorImpl;
class V8InspectorSessionImpl;

using protocol::ErrorString;

class V8TimeTravelAgentImpl : public protocol::TimeTravel::Backend {
 public:
  V8TimeTravelAgentImpl(V8InspectorSessionImpl*, protocol::FrontendChannel*,
                      protocol::DictionaryValue* state);
  ~V8TimeTravelAgentImpl() override;

  // Part of the protocol.
  void reverse(ErrorString*) override;
  void stepBack(ErrorString*) override;

  bool enabled();

 private:
  bool checkEnabled(ErrorString*);

  V8InspectorSessionImpl* m_session;
  protocol::TimeTravel::Frontend m_frontend;
  protocol::DictionaryValue* m_state;

  DISALLOW_COPY_AND_ASSIGN(V8TimeTravelAgentImpl);
};

}  // namespace v8_inspector

#endif  // DEPS_CHAKRASHIM_SRC_INSPECTOR_V8_TIMETRAVEL_AGENT_IMPL_H_
