// Copyright 2016 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "src/inspector/v8-inspector-session-impl.h"

#include "src/inspector/inspected-context.h"
#include "src/inspector/protocol/Protocol.h"
#include "src/inspector/search-util.h"
#include "src/inspector/string-util.h"
#include "src/inspector/v8-console-agent-impl.h"
#include "src/inspector/v8-debugger-agent-impl.h"
#include "src/inspector/v8-debugger.h"
#include "src/inspector/v8-inspector-impl.h"
#include "src/inspector/v8-runtime-agent-impl.h"
#include "src/inspector/v8-schema-agent-impl.h"
#include "src/inspector/v8-timetravel-agent-impl.h"
#include "src/jsrtinspector.h"
#include "src/jsrtutils.h"

namespace v8_inspector {

// static
bool V8InspectorSession::canDispatchMethod(const StringView& method) {
  return stringViewStartsWith(method,
                              protocol::Runtime::Metainfo::commandPrefix) ||
         stringViewStartsWith(method,
                              protocol::Debugger::Metainfo::commandPrefix) ||
         stringViewStartsWith(method,
                              protocol::Console::Metainfo::commandPrefix) ||
         stringViewStartsWith(method,
                              protocol::Schema::Metainfo::commandPrefix) ||
         stringViewStartsWith(method,
                              protocol::TimeTravel::Metainfo::commandPrefix);
}

std::unique_ptr<V8InspectorSessionImpl> V8InspectorSessionImpl::create(
    V8InspectorImpl* inspector, int contextGroupId,
    V8Inspector::Channel* channel, const StringView& state) {
  return wrapUnique(
      new V8InspectorSessionImpl(inspector, contextGroupId, channel, state));
}

V8InspectorSessionImpl::V8InspectorSessionImpl(V8InspectorImpl* inspector,
                                               int contextGroupId,
                                               V8Inspector::Channel* channel,
                                               const StringView& savedState)
    : m_contextGroupId(contextGroupId),
      m_inspector(inspector),
      m_channel(channel),
      m_dispatcher(this),
      m_state(nullptr),
      m_runtimeAgent(nullptr),
      m_debuggerAgent(nullptr),
      m_consoleAgent(nullptr),
      m_schemaAgent(nullptr) {
  if (savedState.length()) {
    std::unique_ptr<protocol::Value> state =
        protocol::parseJSON(toString16(savedState));
    if (state) m_state = protocol::DictionaryValue::cast(std::move(state));
    if (!m_state) m_state = protocol::DictionaryValue::create();
  } else {
    m_state = protocol::DictionaryValue::create();
  }

  m_runtimeAgent = wrapUnique(new V8RuntimeAgentImpl(
      this, this, agentState(protocol::Runtime::Metainfo::domainName)));
  protocol::Runtime::Dispatcher::wire(&m_dispatcher, m_runtimeAgent.get());

  m_debuggerAgent = wrapUnique(new V8DebuggerAgentImpl(
      this, this, agentState(protocol::Debugger::Metainfo::domainName)));
  protocol::Debugger::Dispatcher::wire(&m_dispatcher, m_debuggerAgent.get());

  m_consoleAgent = wrapUnique(new V8ConsoleAgentImpl(
      this, this, agentState(protocol::Console::Metainfo::domainName)));
  protocol::Console::Dispatcher::wire(&m_dispatcher, m_consoleAgent.get());

  m_schemaAgent = wrapUnique(new V8SchemaAgentImpl(
      this, this, agentState(protocol::Schema::Metainfo::domainName)));
  protocol::Schema::Dispatcher::wire(&m_dispatcher, m_schemaAgent.get());

  m_timeTravelAgent = wrapUnique(new V8TimeTravelAgentImpl(
      this, this, agentState(protocol::TimeTravel::Metainfo::domainName)));
  protocol::TimeTravel::Dispatcher::wire(&m_dispatcher,
                                         m_timeTravelAgent.get());

  if (savedState.length()) {
    m_runtimeAgent->restore();
    m_debuggerAgent->restore();
    m_consoleAgent->restore();
  }
}

V8InspectorSessionImpl::~V8InspectorSessionImpl() {
  ErrorString errorString;
  m_consoleAgent->disable(&errorString);
  m_debuggerAgent->disable(&errorString);
  m_runtimeAgent->disable(&errorString);

  m_inspector->disconnect(this);
}

protocol::DictionaryValue* V8InspectorSessionImpl::agentState(
    const String16& name) {
  protocol::DictionaryValue* state = m_state->getObject(name);
  if (!state) {
    std::unique_ptr<protocol::DictionaryValue> newState =
        protocol::DictionaryValue::create();
    state = newState.get();
    m_state->setObject(name, std::move(newState));
  }
  return state;
}

void V8InspectorSessionImpl::sendProtocolResponse(int callId,
                                                  const String16& message) {
  m_channel->sendResponse(callId, toStringBuffer(message));
}

void V8InspectorSessionImpl::sendProtocolNotification(const String16& message) {
  m_channel->sendNotification(toStringBuffer(message));
}

void V8InspectorSessionImpl::flushProtocolNotifications() {
  m_channel->flushProtocolNotifications();
}

void V8InspectorSessionImpl::reset() {
  m_debuggerAgent->reset();
  m_runtimeAgent->reset();
}

void V8InspectorSessionImpl::setCustomObjectFormatterEnabled(bool enabled) {
  // CHAKRA-TODO - Figure out what to do here.
  CHAKRA_UNIMPLEMENTED();
}

void V8InspectorSessionImpl::reportAllContexts(V8RuntimeAgentImpl* agent) {
  const V8InspectorImpl::ContextByIdMap* contexts =
      m_inspector->contextGroup(m_contextGroupId);
  if (!contexts) return;
  for (auto& idContext : *contexts)
    agent->reportExecutionContextCreated(idContext.second.get());
}

void V8InspectorSessionImpl::dispatchProtocolMessage(
    const StringView& message) {
  m_dispatcher.dispatch(protocol::parseJSON(message));
}

std::unique_ptr<StringBuffer> V8InspectorSessionImpl::stateJSON() {
  String16 json = m_state->toJSONString();
  return StringBufferImpl::adopt(json);
}

std::vector<std::unique_ptr<protocol::Schema::API::Domain>>
V8InspectorSessionImpl::supportedDomains() {
  std::vector<std::unique_ptr<protocol::Schema::Domain>> domains =
      supportedDomainsImpl();
  std::vector<std::unique_ptr<protocol::Schema::API::Domain>> result;
  for (size_t i = 0; i < domains.size(); ++i)
    result.push_back(std::move(domains[i]));
  return result;
}

std::vector<std::unique_ptr<protocol::Schema::Domain>>
V8InspectorSessionImpl::supportedDomainsImpl() {
  std::vector<std::unique_ptr<protocol::Schema::Domain>> result;
  result.push_back(protocol::Schema::Domain::create()
                       .setName(protocol::Runtime::Metainfo::domainName)
                       .setVersion(protocol::Runtime::Metainfo::version)
                       .build());
  result.push_back(protocol::Schema::Domain::create()
                       .setName(protocol::Debugger::Metainfo::domainName)
                       .setVersion(protocol::Debugger::Metainfo::version)
                       .build());
  result.push_back(protocol::Schema::Domain::create()
                       .setName(protocol::Schema::Metainfo::domainName)
                       .setVersion(protocol::Schema::Metainfo::version)
                       .build());

  if (m_timeTravelAgent->enabled()) {
    result.push_back(protocol::Schema::Domain::create()
                        .setName(protocol::TimeTravel::Metainfo::domainName)
                        .setVersion(protocol::TimeTravel::Metainfo::version)
                        .build());
  }

  return result;
}

void V8InspectorSessionImpl::schedulePauseOnNextStatement(
    const StringView& breakReason, const StringView& breakDetails) {
  m_debuggerAgent->schedulePauseOnNextStatement(
      toString16(breakReason),
      protocol::DictionaryValue::cast(protocol::parseJSON(breakDetails)));
}

void V8InspectorSessionImpl::cancelPauseOnNextStatement() {
  m_debuggerAgent->cancelPauseOnNextStatement();
}

void V8InspectorSessionImpl::breakProgram(const StringView& breakReason,
                                          const StringView& breakDetails) {
  m_debuggerAgent->breakProgram(
      toString16(breakReason),
      protocol::DictionaryValue::cast(protocol::parseJSON(breakDetails)));
}

void V8InspectorSessionImpl::setSkipAllPauses(bool skip) {
  ErrorString errorString;
  m_debuggerAgent->setSkipAllPauses(&errorString, skip);
}

void V8InspectorSessionImpl::resume() {
  ErrorString errorString;
  m_debuggerAgent->resume(&errorString);
}

void V8InspectorSessionImpl::stepOver() {
  ErrorString errorString;
  m_debuggerAgent->stepOver(&errorString);
}

std::vector<std::unique_ptr<protocol::Debugger::API::SearchMatch>>
V8InspectorSessionImpl::searchInTextByLines(const StringView& text,
                                            const StringView& query,
                                            bool caseSensitive, bool isRegex) {
  // TODO(dgozman): search may operate on StringView and avoid copying |text|.
  std::vector<std::unique_ptr<protocol::Debugger::SearchMatch>> matches =
      searchInTextByLinesImpl(this, toString16(text), toString16(query),
                              caseSensitive, isRegex);
  std::vector<std::unique_ptr<protocol::Debugger::API::SearchMatch>> result;
  for (size_t i = 0; i < matches.size(); ++i)
    result.push_back(std::move(matches[i]));
  return result;
}

}  // namespace v8_inspector
