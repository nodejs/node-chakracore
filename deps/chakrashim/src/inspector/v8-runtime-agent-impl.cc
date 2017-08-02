/*
 * Copyright (C) 2011 Google Inc. All rights reserved.
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

#include "src/inspector/v8-runtime-agent-impl.h"

#include "src/inspector/inspected-context.h"
#include "src/inspector/protocol/Protocol.h"
#include "src/inspector/string-util.h"
#include "src/inspector/v8-console-message.h"
#include "src/inspector/v8-debugger-agent-impl.h"
#include "src/inspector/v8-debugger.h"
#include "src/inspector/v8-inspector-impl.h"
#include "src/inspector/v8-inspector-session-impl.h"
#include "include/v8-inspector.h"
#include "src/jsrtinspectorhelpers.h"
#include "src/jsrtutils.h"

namespace v8_inspector {

namespace V8RuntimeAgentImplState {
static const char customObjectFormatterEnabled[] =
    "customObjectFormatterEnabled";
static const char runtimeEnabled[] = "runtimeEnabled";
};

using protocol::Runtime::RemoteObject;

static bool hasInternalError(ErrorString* errorString, bool hasError) {
  if (hasError) *errorString = "Internal error";
  return hasError;
}

namespace {

std::unique_ptr<protocol::DictionaryValue> ParseObjectId(
    const String16& objectId) {
  std::unique_ptr<protocol::Value> parsedValue = protocol::parseJSON(objectId);
  if (!parsedValue || parsedValue->type() != protocol::Value::TypeObject) {
    return nullptr;
  }

  return wrapUnique(protocol::DictionaryValue::cast(parsedValue.release()));
}

}  // namespace

V8RuntimeAgentImpl::V8RuntimeAgentImpl(
    V8InspectorSessionImpl* session, protocol::FrontendChannel* FrontendChannel,
    protocol::DictionaryValue* state)
    : m_session(session),
      m_state(state),
      m_frontend(FrontendChannel),
      m_inspector(session->inspector()),
      m_enabled(false) {}

V8RuntimeAgentImpl::~V8RuntimeAgentImpl() {}

void V8RuntimeAgentImpl::evaluate(
    const String16& expression, const Maybe<String16>& objectGroup,
    const Maybe<bool>& includeCommandLineAPI, const Maybe<bool>& silent,
    const Maybe<int>& executionContextId, const Maybe<bool>& returnByValue,
    const Maybe<bool>& generatePreview, const Maybe<bool>& userGesture,
    const Maybe<bool>& awaitPromise,
    std::unique_ptr<EvaluateCallback> callback) {
  ErrorString errorString;

  JsValueRef expStr;
  if (JsCreateStringUtf16(expression.characters16(), expression.length(),
                          &expStr) != JsNoError) {
    errorString = "Out of memory";
    callback->sendFailure(errorString);
    return;
  }

  v8::Local<v8::Value> evalResult =
      jsrt::InspectorHelpers::EvaluateOnCallFrame(
          /* ordinal */ 0, expStr, returnByValue.fromMaybe(false));

  if (evalResult.IsEmpty()) {
    errorString = "Failed to evaluate expression";
    callback->sendFailure(errorString);
    return;
  }

  Maybe<protocol::Runtime::ExceptionDetails> exceptionDetails;
  protocol::ErrorSupport errors;
  std::unique_ptr<protocol::Value> protocolValue =
      toProtocolValue(&errorString, v8::Context::GetCurrent(), evalResult);
  if (!protocolValue) {
    callback->sendSuccess(nullptr, exceptionDetails);
    return;
  }
  std::unique_ptr<protocol::Runtime::RemoteObject> remoteObject =
    protocol::Runtime::RemoteObject::parse(protocolValue.get(), &errors);
  if (!remoteObject) {
    errorString = errors.errors();
    callback->sendFailure(errorString);
    return;
  }

  callback->sendSuccess(std::move(remoteObject), exceptionDetails);
}

void V8RuntimeAgentImpl::awaitPromise(
    const String16& promiseObjectId, const Maybe<bool>& returnByValue,
    const Maybe<bool>& generatePreview,
    std::unique_ptr<AwaitPromiseCallback> callback) {
  // CHAKRA-TODO - Figure out what to do here
  callback->sendFailure("Feature not implemented");
}

void V8RuntimeAgentImpl::callFunctionOn(
    const String16& objectId, const String16& expression,
    const Maybe<protocol::Array<protocol::Runtime::CallArgument>>&
        optionalArguments,
    const Maybe<bool>& silent, const Maybe<bool>& returnByValue,
    const Maybe<bool>& generatePreview, const Maybe<bool>& userGesture,
    const Maybe<bool>& awaitPromise,
    std::unique_ptr<CallFunctionOnCallback> callback) {
  // CHAKRA-TODO - This is used to execute an arbitrary JS function against a
  // given object. We don't have a good way to handle this right now in
  // ChakraCore.
  callback->sendFailure("Feature not implemented");
}

void V8RuntimeAgentImpl::getProperties(
    ErrorString* errorString, const String16& objectId,
    const Maybe<bool>& ownProperties, const Maybe<bool>& accessorPropertiesOnly,
    const Maybe<bool>& generatePreview,
    std::unique_ptr<protocol::Array<protocol::Runtime::PropertyDescriptor>>*
        result,
    Maybe<protocol::Array<protocol::Runtime::InternalPropertyDescriptor>>*
        internalProperties,
    Maybe<protocol::Runtime::ExceptionDetails>* exceptionDetails) {
  using protocol::Runtime::InternalPropertyDescriptor;

  if (accessorPropertiesOnly.fromMaybe(false)) {
    // We don't support accessorPropertiesOnly lookups.
    *result = protocol::Array<protocol::Runtime::PropertyDescriptor>::create();
    return;
  }

  std::unique_ptr<protocol::DictionaryValue> parsedId = ParseObjectId(objectId);
  if (parsedId == nullptr) {
    return;
  }

  int handle;
  int ordinal;
  protocol::String name;

  v8::Local<v8::Value> resultValue;

  if (parsedId->getInteger("handle", &handle)) {
    resultValue = jsrt::InspectorHelpers::GetWrappedProperties(handle);
  } else if (parsedId->getInteger("ordinal", &ordinal) &&
           parsedId->getString("name", &name)) {
    JsValueRef stackProperties;
    if (JsDiagGetStackProperties(ordinal, &stackProperties) != JsNoError) {
      *errorString = "Invalid ordinal value";
      return;
    }

    if (name == "locals") {
      resultValue = jsrt::InspectorHelpers::GetWrappedStackLocals(
          stackProperties);
    } else if (name == "globals") {
      JsValueRef globals;
      if (jsrt::GetProperty(stackProperties, jsrt::CachedPropertyIdRef::globals,
                            &globals) != JsNoError) {
        *errorString = "Invalid stack property name";
        return;
      }

      int handle;
      if (jsrt::GetProperty(globals, jsrt::CachedPropertyIdRef::handle,
                            &handle) != JsNoError) {
        *errorString = "Unable to find object";
        return;
      }

      resultValue = jsrt::InspectorHelpers::GetWrappedProperties(handle);
    } else {
      *errorString = "Invalid stack property name";
      return;
    }
  } else {
    *errorString = "Unable to parse object ID";
    return;
  }

  if (hasInternalError(errorString, resultValue.IsEmpty())) {
    return;
  }

  std::unique_ptr<protocol::Value> protocolValue =
      toProtocolValue(errorString, v8::Context::GetCurrent(), resultValue);
  if (!protocolValue) {
    return;
  }

  protocol::ErrorSupport errors(errorString);
  std::unique_ptr<protocol::Array<protocol::Runtime::PropertyDescriptor>>
      parsedResult =
          protocol::Array<protocol::Runtime::PropertyDescriptor>::parse(
              protocolValue.get(), &errors);
  if (hasInternalError(errorString, errors.hasErrors())) {
    return;
  }

  *result = std::move(parsedResult);
}

void V8RuntimeAgentImpl::releaseObject(ErrorString* errorString,
                                       const String16& objectId) {
  // CHAKRA-TODO - Figure out what to do here
}

void V8RuntimeAgentImpl::releaseObjectGroup(ErrorString*,
                                            const String16& objectGroup) {
  // CHAKRA-TODO - Figure out what to do here
}

void V8RuntimeAgentImpl::runIfWaitingForDebugger(ErrorString* errorString) {
  m_inspector->client()->runIfWaitingForDebugger(m_session->contextGroupId());
}

void V8RuntimeAgentImpl::setCustomObjectFormatterEnabled(ErrorString*,
                                                         bool enabled) {
  m_state->setBoolean(V8RuntimeAgentImplState::customObjectFormatterEnabled,
                      enabled);
  m_session->setCustomObjectFormatterEnabled(enabled);
}

void V8RuntimeAgentImpl::discardConsoleEntries(ErrorString*) {
  V8ConsoleMessageStorage* storage =
      m_inspector->ensureConsoleMessageStorage(m_session->contextGroupId());
  storage->clear();
}

void V8RuntimeAgentImpl::compileScript(
    ErrorString* errorString, const String16& expression,
    const String16& sourceURL, bool persistScript,
    const Maybe<int>& executionContextId, Maybe<String16>* scriptId,
    Maybe<protocol::Runtime::ExceptionDetails>* exceptionDetails) {
  // CHAKRA-TODO - Figure out what to do here
  CHAKRA_UNIMPLEMENTED();
}

void V8RuntimeAgentImpl::runScript(
    const String16& scriptId, const Maybe<int>& executionContextId,
    const Maybe<String16>& objectGroup, const Maybe<bool>& silent,
    const Maybe<bool>& includeCommandLineAPI, const Maybe<bool>& returnByValue,
    const Maybe<bool>& generatePreview, const Maybe<bool>& awaitPromise,
    std::unique_ptr<RunScriptCallback> callback) {
  // CHAKRA-TODO - Figure out what to do here
  callback->sendFailure("Feature not implemented");
}

void V8RuntimeAgentImpl::restore() {
  if (!m_state->booleanProperty(V8RuntimeAgentImplState::runtimeEnabled, false))
    return;
  m_frontend.executionContextsCleared();
  ErrorString error;
  enable(&error);
  if (m_state->booleanProperty(
          V8RuntimeAgentImplState::customObjectFormatterEnabled, false))
    m_session->setCustomObjectFormatterEnabled(true);
}

void V8RuntimeAgentImpl::enable(ErrorString* errorString) {
  if (m_enabled) return;
  m_inspector->client()->beginEnsureAllContextsInGroup(
      m_session->contextGroupId());
  m_enabled = true;
  m_state->setBoolean(V8RuntimeAgentImplState::runtimeEnabled, true);
  m_inspector->enableStackCapturingIfNeeded();
  m_session->reportAllContexts(this);
  V8ConsoleMessageStorage* storage =
      m_inspector->ensureConsoleMessageStorage(m_session->contextGroupId());
  for (const auto& message : storage->messages()) {
    if (!reportMessage(message.get(), false)) return;
  }
}

void V8RuntimeAgentImpl::disable(ErrorString* errorString) {
  if (!m_enabled) return;
  m_enabled = false;
  m_state->setBoolean(V8RuntimeAgentImplState::runtimeEnabled, false);
  m_inspector->disableStackCapturingIfNeeded();
  reset();
  m_inspector->client()->endEnsureAllContextsInGroup(
      m_session->contextGroupId());
}

void V8RuntimeAgentImpl::reset() {
  if (m_enabled) {
    if (const V8InspectorImpl::ContextByIdMap* contexts =
            m_inspector->contextGroup(m_session->contextGroupId())) {
      for (auto& idContext : *contexts) idContext.second->setReported(false);
    }
    m_frontend.executionContextsCleared();
  }
}

void V8RuntimeAgentImpl::reportExecutionContextCreated(
    InspectedContext* context) {
  if (!m_enabled) return;
  context->setReported(true);
  std::unique_ptr<protocol::Runtime::ExecutionContextDescription> description =
      protocol::Runtime::ExecutionContextDescription::create()
          .setId(context->contextId())
          .setName(context->humanReadableName())
          .setOrigin(context->origin())
          .build();
  if (!context->auxData().isEmpty())
    description->setAuxData(protocol::DictionaryValue::cast(
        protocol::parseJSON(context->auxData())));
  m_frontend.executionContextCreated(std::move(description));
}

void V8RuntimeAgentImpl::reportExecutionContextDestroyed(
    InspectedContext* context) {
  if (m_enabled && context->isReported()) {
    context->setReported(false);
    m_frontend.executionContextDestroyed(context->contextId());
  }
}

void V8RuntimeAgentImpl::inspect(
    std::unique_ptr<protocol::Runtime::RemoteObject> objectToInspect,
    std::unique_ptr<protocol::DictionaryValue> hints) {
  if (m_enabled)
    m_frontend.inspectRequested(std::move(objectToInspect), std::move(hints));
}

void V8RuntimeAgentImpl::messageAdded(V8ConsoleMessage* message) {
  if (m_enabled) reportMessage(message, true);
}

bool V8RuntimeAgentImpl::reportMessage(V8ConsoleMessage* message,
  bool generatePreview) {
  message->reportToFrontend(&m_frontend, m_session, generatePreview);
  m_frontend.flush();
  return m_inspector->hasConsoleMessageStorage(m_session->contextGroupId());
}

}  // namespace v8_inspector
