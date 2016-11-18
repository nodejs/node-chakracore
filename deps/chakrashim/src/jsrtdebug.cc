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

#include "jsrtdebug.h"
#include "jsrtutils.h"

namespace v8 {
  extern __declspec(thread) bool g_EnableDebug;
}

namespace jsrt {

  MessageQueue Debugger::messageQueue;
  v8::Debug::MessageHandler Debugger::handler = nullptr;

  MessageImpl::MessageImpl(v8::Local<v8::String> v8Str) {
    this->v8Str = v8Str;
  }

  v8::Isolate* MessageImpl::GetIsolate() const {
    return jsrt::IsolateShim::GetCurrentAsIsolate();
  }

  v8::Local<v8::String> MessageImpl::JsValueRefToV8String(JsValueRef result) {
    JsValueRef scriptRef;
    const wchar_t *script;

    JsErrorCode errorCode = jsrt::ToString(result, &scriptRef, &script);
    CHAKRA_VERIFY_NOERROR(errorCode);

    v8::Local<v8::String> v8Str = v8::String::NewFromTwoByte(
      jsrt::IsolateShim::GetCurrentAsIsolate(),
      reinterpret_cast<uint16_t*>(const_cast<wchar_t*>(script)));
    return v8Str;
  }

  v8::Local<v8::String> MessageImpl::GetJSON() const {
    return this->v8Str;
  }

  MessageQueue::MessageQueue() {
    this->waitingForMessage = false;
    this->isProcessingDebuggerMsg = false;
    this->debugEventProcessCount = 0;
    this->msgQueue = new std::queue<uint16_t*>();

    int err = uv_sem_init(&newMsgSem, 0);
    CHAKRA_VERIFY(err == 0);
    err = uv_mutex_init(&msgMutex);
    CHAKRA_VERIFY(err == 0);
  }

  MessageQueue::~MessageQueue() {
    uv_mutex_destroy(&msgMutex);
    uv_sem_destroy(&newMsgSem);
    delete this->msgQueue;
    this->msgQueue = nullptr;
  }

  void MessageQueue::SaveMessage(const uint16_t* command, int length) {
    uint16_t* commandCopy = new uint16_t[length + 1];
    for (int i = 0; i < length; i++) commandCopy[i] = command[i];
    commandCopy[length] = '\0';

    uv_mutex_lock(&msgMutex);
    this->msgQueue->push(commandCopy);

    if (this->waitingForMessage) {
      uv_sem_post(&newMsgSem);
      this->waitingForMessage = false;
    }

    uv_mutex_unlock(&msgMutex);
  }

  uint16_t* MessageQueue::PopMessage() {
    uv_mutex_lock(&msgMutex);
    uint16_t* command = this->msgQueue->front();
    this->msgQueue->pop();
    uv_mutex_unlock(&msgMutex);
    return command;
  }

  bool MessageQueue::IsEmpty() {
    return this->msgQueue->empty();
  }

  void MessageQueue::WaitForMessage() {
    uv_mutex_lock(&msgMutex);

    if (!this->IsEmpty()) {
      uv_mutex_unlock(&msgMutex);
      return;
    }

    this->waitingForMessage = true;
    uv_mutex_unlock(&msgMutex);
    uv_sem_wait(&newMsgSem);
  }

  void MessageQueue::SetProcessMessageFunctions(
    JsValueRef chakraDebugObject,
    JsValueRef processDebuggerMessageFn,
    JsValueRef processDebugEventFn,
    JsValueRef processShouldContinueFn) {
    this->chakraDebugObject = chakraDebugObject;
    this->processDebuggerMessageFn = processDebuggerMessageFn;
    this->processDebugEventFn = processDebugEventFn;
    this->processShouldContinueFn = processShouldContinueFn;
  }

  void MessageQueue::ProcessDebuggerMessage() {
    if (this->isProcessingDebuggerMsg) {
      return;
    }

    if (!this->IsEmpty()) {
      this->isProcessingDebuggerMsg = true;

      uint16_t* command = this->PopMessage();
      v8::Local<v8::String> commandJson = v8::String::NewFromTwoByte(
        jsrt::IsolateShim::GetCurrentAsIsolate(), command);

      JsValueRef args[] = { jsrt::GetUndefined(), *commandJson };
      JsValueRef responseRef;
      JsErrorCode errorCode = JsCallFunction(
        this->processDebuggerMessageFn, args, _countof(args), &responseRef);
      CHAKRA_VERIFY_NOERROR(errorCode);

      delete command;

      JsValueType resultType;
      errorCode = JsGetValueType(responseRef, &resultType);
      CHAKRA_VERIFY_NOERROR(errorCode);

      if (resultType == JsString) {
        v8::Local<v8::String> v8Str =
          MessageImpl::JsValueRefToV8String(responseRef);
        MessageImpl* msgImpl = new MessageImpl(v8Str);
        Debugger::handler(*msgImpl);
      }

      this->isProcessingDebuggerMsg = false;
    }
  }

  void MessageQueue::ProcessDebugEvent(
    JsDiagDebugEvent debugEvent,
    JsValueRef eventData) {
    JsValueRef debugEventRef;
    JsErrorCode errorCode = JsIntToNumber(debugEvent, &debugEventRef);
    CHAKRA_VERIFY_NOERROR(errorCode);

    JsValueRef args[] = { jsrt::GetUndefined(), debugEventRef, eventData };
    JsValueRef result;
    errorCode = JsCallFunction(
      this->processDebugEventFn, args, _countof(args), &result);
    CHAKRA_VERIFY_NOERROR(errorCode);

    JsValueType resultType;
    errorCode = JsGetValueType(result, &resultType);
    CHAKRA_VERIFY_NOERROR(errorCode);

    if (resultType == JsString) {
      v8::Local<v8::String> v8Str = MessageImpl::JsValueRefToV8String(result);
      MessageImpl* msgImpl = new MessageImpl(v8Str);
      Debugger::handler(*msgImpl);
    }
  }

  bool MessageQueue::ShouldContinue(JsDiagDebugEvent debugEvent) {
    JsValueRef debugEventRef;
    JsErrorCode errorCode = JsIntToNumber(debugEvent, &debugEventRef);
    CHAKRA_VERIFY_NOERROR(errorCode);

    JsValueRef args[] = { jsrt::GetUndefined(), debugEventRef };
    JsValueRef result;
    errorCode = JsCallFunction(
      this->processShouldContinueFn, args, _countof(args), &result);
    CHAKRA_VERIFY_NOERROR(errorCode);

    JsValueType resultType;
    errorCode = JsGetValueType(result, &resultType);
    CHAKRA_VERIFY_NOERROR(errorCode);

    CHAKRA_VERIFY(resultType == JsBoolean);

    bool shouldContinue = true;
    errorCode = JsBooleanToBool(result, &shouldContinue);
    CHAKRA_VERIFY_NOERROR(errorCode);

    return shouldContinue;
  }

  void CALLBACK JsDiagDebugEventHandler(
    JsDiagDebugEvent debugEvent,
    JsValueRef eventData,
    void* callbackState) {
    if (Debugger::handler == nullptr) {
      return;
    }

    Debugger::messageQueue.debugEventProcessCount++;
    bool isMsgToProcess = !Debugger::messageQueue.IsEmpty();
    bool shouldContinue = true;
    do {
      if (eventData != nullptr) {
        Debugger::messageQueue.ProcessDebugEvent(debugEvent, eventData);
        eventData = nullptr;
      }

      // Process all messages received from debugger
      if (Debugger::messageQueue.debugEventProcessCount <= 1) {
        isMsgToProcess = !Debugger::messageQueue.IsEmpty();
        while (!Debugger::messageQueue.isProcessingDebuggerMsg &&
          isMsgToProcess) {
          Debugger::messageQueue.ProcessDebuggerMessage();
          isMsgToProcess = !Debugger::messageQueue.IsEmpty();
        }
      }

      shouldContinue = Debugger::messageQueue.ShouldContinue(debugEvent);
      if (!shouldContinue) {
        Debugger::messageQueue.WaitForMessage();
      }

      isMsgToProcess = !Debugger::messageQueue.IsEmpty();
    } while (isMsgToProcess &&
      !Debugger::messageQueue.isProcessingDebuggerMsg && !shouldContinue);

    Debugger::messageQueue.debugEventProcessCount--;
  }

  static JsValueRef CALLBACK Log(
    JsValueRef callee,
    bool isConstructCall,
    JsValueRef *arguments,
    unsigned short argumentCount,
    void *callbackState) {
    JsValueRef scriptRef;
    const wchar_t *script;

    if (argumentCount > 1 &&
      jsrt::ToString(arguments[1], &scriptRef, &script) == JsNoError) {
      fwprintf(stdout, L"%s\n", script);
#ifdef DEBUG
      flushall();
#endif
    }

    return JS_INVALID_REFERENCE;
  }

  static JsValueRef CALLBACK SendDelayedRespose(
    JsValueRef callee,
    bool isConstructCall,
    JsValueRef *arguments,
    unsigned short argumentCount,
    void *callbackState) {
    JsValueType resultType;
    JsErrorCode errorCode = JsGetValueType(arguments[1], &resultType);
    CHAKRA_VERIFY_NOERROR(errorCode);

    if (resultType == JsString) {
      v8::Local<v8::String> v8Str = MessageImpl::JsValueRefToV8String(
        arguments[1]);
      MessageImpl* msgImpl = new MessageImpl(v8Str);
      Debugger::handler(*msgImpl);
    }

    return JS_INVALID_REFERENCE;
  }

  static JsValueRef CALLBACK JsGetScripts(
    JsValueRef callee,
    bool isConstructCall,
    JsValueRef *arguments,
    unsigned short argumentCount,
    void *callbackState) {
    JsValueRef sourcesList;
    JsErrorCode errorCode = JsDiagGetScripts(&sourcesList);
    CHAKRA_VERIFY_NOERROR(errorCode);

    return sourcesList;
  }

  static JsValueRef CALLBACK JsGetSource(
    JsValueRef callee,
    bool isConstructCall,
    JsValueRef *arguments,
    unsigned short argumentCount,
    void *callbackState) {
    int scriptId;
    JsValueRef source = JS_INVALID_REFERENCE;
    if (argumentCount > 1 &&
      jsrt::ValueToInt(arguments[1], &scriptId) == JsNoError) {
      JsErrorCode errorCode = JsDiagGetSource(scriptId, &source);
      CHAKRA_VERIFY_NOERROR(errorCode);
    }

    return source;
  }

  static JsValueRef CALLBACK JsSetStepType(
    JsValueRef callee,
    bool isConstructCall,
    JsValueRef *arguments,
    unsigned short argumentCount,
    void *callbackState) {
    bool success = false;
    int stepType;
    if (argumentCount > 1 &&
      jsrt::ValueToInt(arguments[1], &stepType) == JsNoError) {
      JsErrorCode errorCode = JsDiagSetStepType((JsDiagStepType)stepType);
      CHAKRA_VERIFY_NOERROR(errorCode);
      success = true;
    }

    JsValueRef returnRef = JS_INVALID_REFERENCE;
    JsBoolToBoolean(success, &returnRef);

    return returnRef;
  }

  static JsValueRef CALLBACK JsSetBreakpoint(
    JsValueRef callee,
    bool isConstructCall,
    JsValueRef *arguments,
    unsigned short argumentCount,
    void *callbackState) {
    int scriptId;
    int line;
    int column;
    JsValueRef bpObject = JS_INVALID_REFERENCE;

    if (argumentCount > 3 &&
      jsrt::ValueToInt(arguments[1], &scriptId) == JsNoError &&
      jsrt::ValueToInt(arguments[2], &line) == JsNoError &&
      jsrt::ValueToInt(arguments[3], &column) == JsNoError) {
      JsErrorCode errorCode = JsDiagSetBreakpoint(
        scriptId, line, column, &bpObject);
    }

    return bpObject;
  }

  static JsValueRef CALLBACK JsGetFunctionPosition(
    JsValueRef callee,
    bool isConstructCall,
    JsValueRef *arguments,
    unsigned short argumentCount,
    void *callbackState) {
    JsValueRef valueRef = JS_INVALID_REFERENCE;
    JsErrorCode errorCode = JsConvertValueToObject(arguments[1], &valueRef);
    CHAKRA_VERIFY_NOERROR(errorCode);
    JsValueRef funcInfo = JS_INVALID_REFERENCE;
    errorCode = JsDiagGetFunctionPosition(valueRef, &funcInfo);
    CHAKRA_VERIFY_NOERROR(errorCode);

    return funcInfo;
  }

  static JsValueRef CALLBACK JsGetStackTrace(
    JsValueRef callee,
    bool isConstructCall,
    JsValueRef *arguments,
    unsigned short argumentCount,
    void *callbackState) {
    JsValueRef stackInfo = JS_INVALID_REFERENCE;
    JsErrorCode errorCode = JsDiagGetStackTrace(&stackInfo);
    CHAKRA_VERIFY_NOERROR(errorCode);

    return stackInfo;
  }

  static JsValueRef CALLBACK JsGetStackProperties(
    JsValueRef callee,
    bool isConstructCall,
    JsValueRef *arguments,
    unsigned short argumentCount,
    void *callbackState) {
    int frameIndex;
    JsValueRef properties = JS_INVALID_REFERENCE;
    if (argumentCount > 1 &&
      jsrt::ValueToInt(arguments[1], &frameIndex) == JsNoError) {
      JsErrorCode errorCode = JsDiagGetStackProperties(frameIndex, &properties);
      CHAKRA_VERIFY_NOERROR(errorCode);
    }

    return properties;
  }

  static JsValueRef CALLBACK JsGetObjectFromHandle(
    JsValueRef callee,
    bool isConstructCall,
    JsValueRef *arguments,
    unsigned short argumentCount,
    void *callbackState) {
    JsValueRef properties = JS_INVALID_REFERENCE;
    int handle;
    if (argumentCount > 1 &&
      jsrt::ValueToInt(arguments[1], &handle) == JsNoError) {
      JsErrorCode errorCode = JsDiagGetObjectFromHandle(handle, &properties);
    }

    return properties;
  }

  static JsValueRef CALLBACK JsGetBreakpoints(
    JsValueRef callee,
    bool isConstructCall,
    JsValueRef *arguments,
    unsigned short argumentCount,
    void *callbackState) {
    JsValueRef properties = JS_INVALID_REFERENCE;
    JsErrorCode errorCode = JsDiagGetBreakpoints(&properties);
    CHAKRA_VERIFY_NOERROR(errorCode);

    return properties;
  }

  static JsValueRef CALLBACK JsRemoveBreakpoint(
    JsValueRef callee,
    bool isConstructCall,
    JsValueRef *arguments,
    unsigned short argumentCount,
    void *callbackState) {
    int bpId;
    if (argumentCount > 1 &&
      jsrt::ValueToInt(arguments[1], &bpId) == JsNoError) {
      JsErrorCode errorCode = JsDiagRemoveBreakpoint(bpId);
      CHAKRA_VERIFY_NOERROR(errorCode);
    }

    return JS_INVALID_REFERENCE;
  }

  static JsValueRef CALLBACK JsGetProperties(
    JsValueRef callee,
    bool isConstructCall,
    JsValueRef *arguments,
    unsigned short argumentCount,
    void *callbackState) {
    JsValueRef properties = JS_INVALID_REFERENCE;
    int objectHandle;
    int fromCount;
    int totalCount;

    if (argumentCount > 3 &&
      jsrt::ValueToInt(arguments[1], &objectHandle) == JsNoError &&
      jsrt::ValueToInt(arguments[2], &fromCount) == JsNoError &&
      jsrt::ValueToInt(arguments[3], &totalCount) == JsNoError) {
      JsErrorCode errorCode = JsDiagGetProperties(
        objectHandle, fromCount, totalCount, &properties);
      CHAKRA_VERIFY_NOERROR(errorCode);
    }
    return properties;
  }

  static JsValueRef CALLBACK JsEvaluate(
    JsValueRef callee,
    bool isConstructCall,
    JsValueRef *arguments,
    unsigned short argumentCount,
    void *callbackState) {
    int frameIndex;
    JsValueRef scriptRef;
    const wchar_t *script;
    JsValueRef resultArray = JS_INVALID_REFERENCE;
    JsValueRef result = JS_INVALID_REFERENCE;

    if (argumentCount > 2 &&
      jsrt::ValueToInt(arguments[2], &frameIndex) == JsNoError &&
      jsrt::ToString(arguments[1], &scriptRef, &script) == JsNoError) {
      JsErrorCode errorCode = JsDiagEvaluate(script, frameIndex, &result);
      if (errorCode != JsNoError && errorCode != JsErrorScriptException) {
        jsrt::Fatal("internal error %s(%d): %d", __FILE__, __LINE__,
          errorCode);
      }

      JsCreateArray(2, &resultArray);

      bool success = errorCode == JsNoError ? true : false;
      JsValueRef resultBool;
      JsBoolToBoolean(success, &resultBool);

      JsValueRef arrayIndex;
      JsIntToNumber(0, &arrayIndex);
      JsSetIndexedProperty(resultArray, arrayIndex, resultBool);
      JsIntToNumber(1, &arrayIndex);
      JsSetIndexedProperty(resultArray, arrayIndex, result);
    }

    return resultArray;
  }

  static JsValueRef CALLBACK JsEvaluateScript(
    JsValueRef callee,
    bool isConstructCall,
    JsValueRef *arguments,
    unsigned short argumentCount,
    void *callbackState) {
    JsValueRef scriptRef;
    const wchar_t *script;
    JsValueRef result = JS_INVALID_REFERENCE;

    if (argumentCount > 1 &&
      jsrt::ToString(arguments[1], &scriptRef, &script) == JsNoError) {
      JsErrorCode errorCode = JsRunScript(
        script, JS_SOURCE_CONTEXT_NONE, L"", &result);
      if (errorCode != JsNoError) {
        JsValueRef excep;
        JsGetAndClearException(&excep);
      }
    }

    return result;
  }

  static JsValueRef CALLBACK JsSetBreakOnException(
    JsValueRef callee,
    bool isConstructCall,
    JsValueRef *arguments,
    unsigned short argumentCount,
    void *callbackState) {
    int breakOnExceptionAttributes;
    bool success = false;

    if (argumentCount > 1 && jsrt::ValueToInt(arguments[1],
      &breakOnExceptionAttributes) == JsNoError &&
      JsDiagSetBreakOnException(
        jsrt::IsolateShim::GetCurrent()->GetRuntimeHandle(),
        (JsDiagBreakOnExceptionAttributes)breakOnExceptionAttributes) ==
      JsNoError) {
      success = true;
    }

    JsValueRef returnRef;
    JsBoolToBoolean(success, &returnRef);

    return returnRef;
  }

  static JsValueRef CALLBACK JsGetBreakOnException(
    JsValueRef callee,
    bool isConstructCall,
    JsValueRef *arguments,
    unsigned short argumentCount,
    void *callbackState) {
    JsDiagBreakOnExceptionAttributes breakOnExceptionAttributes =
      JsDiagBreakOnExceptionAttributeNone;

    JsErrorCode errorCode = JsDiagGetBreakOnException(
      jsrt::IsolateShim::GetCurrent()->GetRuntimeHandle(),
      &breakOnExceptionAttributes);
    CHAKRA_VERIFY_NOERROR(errorCode);

    JsValueRef returnRef = JS_INVALID_REFERENCE;
    errorCode = JsIntToNumber(breakOnExceptionAttributes, &returnRef);
    CHAKRA_VERIFY_NOERROR(errorCode);

    return returnRef;
  }

  bool Debugger::IsDebugEnabled() {
    return v8::g_EnableDebug;
  }

  void Debugger::StartDebugging(JsRuntimeHandle runtime) {
    JsErrorCode errorCode = JsDiagStartDebugging(
      runtime, JsDiagDebugEventHandler, nullptr);
    CHAKRA_VERIFY_NOERROR(errorCode);
  }

  void Debugger::InstallHostCallback(JsValueRef chakraDebugObject,
    const wchar_t *name, JsNativeFunction nativeFunction) {
    JsValueRef nameVar;
    JsErrorCode errorCode = JsPointerToString(name, wcslen(name), &nameVar);
    CHAKRA_VERIFY_NOERROR(errorCode);

    JsValueRef funcRef;
    errorCode = JsCreateNamedFunction(
      nameVar, nativeFunction, nullptr, &funcRef);
    CHAKRA_VERIFY_NOERROR(errorCode);

    JsPropertyIdRef propertyIdRef;
    errorCode = JsGetPropertyIdFromName(name, &propertyIdRef);
    CHAKRA_VERIFY_NOERROR(errorCode);

    errorCode = JsSetProperty(chakraDebugObject, propertyIdRef, funcRef, true);
    CHAKRA_VERIFY_NOERROR(errorCode);
  }

  void Debugger::SetChakraDebugObject(JsValueRef chakraDebugObject) {
    JsPropertyIdRef propertyIdRef;

    JsErrorCode errorCode = JsGetPropertyIdFromName(
      L"ProcessDebuggerMessage", &propertyIdRef);
    CHAKRA_VERIFY_NOERROR(errorCode);

    JsValueRef processDebuggerMessageFn;
    errorCode = JsGetProperty(chakraDebugObject,
      propertyIdRef, &processDebuggerMessageFn);
    CHAKRA_VERIFY_NOERROR(errorCode);

    errorCode = JsGetPropertyIdFromName(L"ProcessDebugEvent",
      &propertyIdRef);
    CHAKRA_VERIFY_NOERROR(errorCode);

    JsValueRef processDebugEventFn;
    errorCode = JsGetProperty(chakraDebugObject,
      propertyIdRef, &processDebugEventFn);
    CHAKRA_VERIFY_NOERROR(errorCode);

    errorCode = JsGetPropertyIdFromName(L"ProcessShouldContinue",
      &propertyIdRef);
    CHAKRA_VERIFY_NOERROR(errorCode);

    JsValueRef processShouldContinueFn;
    errorCode = JsGetProperty(chakraDebugObject,
      propertyIdRef, &processShouldContinueFn);
    CHAKRA_VERIFY_NOERROR(errorCode);

    Debugger::messageQueue.SetProcessMessageFunctions(chakraDebugObject,
      processDebuggerMessageFn, processDebugEventFn, processShouldContinueFn);

    Debugger::InstallHostCallback(chakraDebugObject,
      L"log", Log);
    Debugger::InstallHostCallback(chakraDebugObject,
      L"JsDiagGetScripts", JsGetScripts);
    Debugger::InstallHostCallback(chakraDebugObject,
      L"JsDiagGetSource", JsGetSource);
    Debugger::InstallHostCallback(chakraDebugObject,
      L"JsDiagSetStepType", JsSetStepType);
    Debugger::InstallHostCallback(chakraDebugObject,
      L"JsDiagSetBreakpoint", JsSetBreakpoint);
    Debugger::InstallHostCallback(chakraDebugObject,
      L"JsDiagGetFunctionPosition", JsGetFunctionPosition);
    Debugger::InstallHostCallback(chakraDebugObject,
      L"JsDiagGetStackTrace", JsGetStackTrace);
    Debugger::InstallHostCallback(chakraDebugObject,
      L"JsDiagGetStackProperties", JsGetStackProperties);
    Debugger::InstallHostCallback(chakraDebugObject,
      L"JsDiagGetObjectFromHandle", JsGetObjectFromHandle);
    Debugger::InstallHostCallback(chakraDebugObject,
      L"JsDiagEvaluateScript", JsEvaluateScript);
    Debugger::InstallHostCallback(chakraDebugObject,
      L"JsDiagEvaluate", JsEvaluate);
    Debugger::InstallHostCallback(chakraDebugObject,
      L"JsDiagGetBreakpoints", JsGetBreakpoints);
    Debugger::InstallHostCallback(chakraDebugObject,
      L"JsDiagGetProperties", JsGetProperties);
    Debugger::InstallHostCallback(chakraDebugObject,
      L"JsDiagRemoveBreakpoint", JsRemoveBreakpoint);
    Debugger::InstallHostCallback(chakraDebugObject,
      L"JsDiagSetBreakOnException", JsSetBreakOnException);
    Debugger::InstallHostCallback(chakraDebugObject,
      L"JsDiagGetBreakOnException", JsGetBreakOnException);
    Debugger::InstallHostCallback(chakraDebugObject,
      L"SendDelayedRespose", SendDelayedRespose);
  }

}  // namespace jsrt
