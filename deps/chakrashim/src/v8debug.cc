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

#include "jsrtutils.h"
#include "jsrtdebug.h"

namespace v8 {

  __declspec(thread) bool g_EnableDebug = false;

  void Debug::EnableDebug() {
    g_EnableDebug = true;
  }

  Local<Context> Debug::GetDebugContext(Isolate* isolate) {
    jsrt::IsolateShim* isoShim = jsrt::IsolateShim::FromIsolate(isolate);
    JsContextRef debugContextRef = JS_INVALID_REFERENCE;
    if (isoShim->debugContext == JS_INVALID_REFERENCE) {
      HandleScope scope(isolate);
      debugContextRef = *Context::New(isolate);
      isoShim->debugContext = isoShim->GetContextShim(
        debugContextRef);
      JsAddRef(debugContextRef, nullptr);

      Local<Object> global = Context::GetCurrent()->Global();

      // CHAKRA-TODO: Chakra doesn't fully implement the debugger without
      // --debug flag. Add a dummy 'Debug' on global object if it doesn't
      // already exist.
      {
        Context::Scope context_scope(debugContextRef);
        jsrt::ContextShim* contextShim = jsrt::ContextShim::GetCurrent();
        JsValueRef ensureDebug = contextShim->GetensureDebugFunction();
        JsValueRef unused;
        if (jsrt::CallFunction(ensureDebug, *global, &unused) != JsNoError) {
          return Local<Context>();
        }
      }
    }

    return static_cast<Context*>(isoShim->debugContext->GetContextRef());
  }

  void Debug::SendCommand(Isolate* isolate,
    const uint16_t* command, int length,
    ClientData* client_data) {
    CHAKRA_ASSERT(client_data == NULL);

    // Save command in a queue and when JsDiagDebugEventHandler
    // is called we need to process the queue
    jsrt::Debugger::messageQueue.SaveMessage(command, length);

    // Request async break from engine so that we can process
    // commands on JsDiagDebugEventAsyncBreak
    JsErrorCode errorCode = JsDiagRequestAsyncBreak(
      jsrt::IsolateShim::FromIsolate(isolate)->GetRuntimeHandle());
    CHAKRA_VERIFY_NOERROR(errorCode);
  }

  void Debug::SetMessageHandler(Isolate* isolate, MessageHandler handler) {
    jsrt::Debugger::handler = handler;
  }

}  // namespace v8
