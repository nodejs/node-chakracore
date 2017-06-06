// Copyright 2016 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef DEPS_CHAKRASHIM_SRC_INSPECTOR_V8_INSPECTOR_SESSION_IMPL_H_
#define DEPS_CHAKRASHIM_SRC_INSPECTOR_V8_INSPECTOR_SESSION_IMPL_H_

#include <vector>

#include "src/base/macros.h"
#include "src/inspector/protocol/Forward.h"
#include "src/inspector/protocol/Runtime.h"
#include "src/inspector/protocol/Schema.h"

#include "include/v8-inspector.h"

namespace v8_inspector {

class RemoteObjectIdBase;
class V8ConsoleAgentImpl;
class V8DebuggerAgentImpl;
class V8InspectorImpl;
class V8RuntimeAgentImpl;
class V8SchemaAgentImpl;
class V8TimeTravelAgentImpl;

using protocol::ErrorString;

class V8InspectorSessionImpl : public V8InspectorSession,
                               public protocol::FrontendChannel {
 public:
  static std::unique_ptr<V8InspectorSessionImpl> create(
      V8InspectorImpl*, int contextGroupId, V8Inspector::Channel*,
      const StringView& state);
  ~V8InspectorSessionImpl();

  V8InspectorImpl* inspector() const { return m_inspector; }
  V8ConsoleAgentImpl* consoleAgent() { return m_consoleAgent.get(); }
  V8DebuggerAgentImpl* debuggerAgent() { return m_debuggerAgent.get(); }
  V8SchemaAgentImpl* schemaAgent() { return m_schemaAgent.get(); }
  V8RuntimeAgentImpl* runtimeAgent() { return m_runtimeAgent.get(); }
  V8TimeTravelAgentImpl* timeTravelAgent() { return m_timeTravelAgent.get(); }
  int contextGroupId() const { return m_contextGroupId; }

  void reset();
  void reportAllContexts(V8RuntimeAgentImpl*);
  void setCustomObjectFormatterEnabled(bool);
  std::vector<std::unique_ptr<protocol::Schema::Domain>> supportedDomainsImpl();

  // V8InspectorSession implementation.
  void dispatchProtocolMessage(const StringView& message) override;
  std::unique_ptr<StringBuffer> stateJSON() override;
  std::vector<std::unique_ptr<protocol::Schema::API::Domain>> supportedDomains()
      override;
  void schedulePauseOnNextStatement(const StringView& breakReason,
                                    const StringView& breakDetails) override;
  void cancelPauseOnNextStatement() override;
  void breakProgram(const StringView& breakReason,
                    const StringView& breakDetails) override;
  void setSkipAllPauses(bool) override;
  void resume() override;
  void stepOver() override;
  std::vector<std::unique_ptr<protocol::Debugger::API::SearchMatch>>
  searchInTextByLines(const StringView& text, const StringView& query,
                      bool caseSensitive, bool isRegex) override;
  static const unsigned kInspectedObjectBufferSize = 5;

 private:
  V8InspectorSessionImpl(V8InspectorImpl*, int contextGroupId,
                         V8Inspector::Channel*, const StringView& state);
  protocol::DictionaryValue* agentState(const String16& name);

  // protocol::FrontendChannel implementation.
  void sendProtocolResponse(int callId, const String16& message) override;
  void sendProtocolNotification(const String16& message) override;
  void flushProtocolNotifications() override;

  int m_contextGroupId;
  V8InspectorImpl* m_inspector;
  V8Inspector::Channel* m_channel;

  protocol::UberDispatcher m_dispatcher;
  std::unique_ptr<protocol::DictionaryValue> m_state;

  std::unique_ptr<V8RuntimeAgentImpl> m_runtimeAgent;
  std::unique_ptr<V8DebuggerAgentImpl> m_debuggerAgent;
  std::unique_ptr<V8ConsoleAgentImpl> m_consoleAgent;
  std::unique_ptr<V8SchemaAgentImpl> m_schemaAgent;
  std::unique_ptr<V8TimeTravelAgentImpl> m_timeTravelAgent;

  DISALLOW_COPY_AND_ASSIGN(V8InspectorSessionImpl);
};

}  // namespace v8_inspector

#endif  // DEPS_CHAKRASHIM_SRC_INSPECTOR_V8_INSPECTOR_SESSION_IMPL_H_
