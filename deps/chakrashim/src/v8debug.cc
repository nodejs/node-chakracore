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
#include "v8-debug.h"

namespace v8 {

  THREAD_LOCAL bool g_EnableInspector = false;
  THREAD_LOCAL bool g_EnableReplayDebug = false;

  void Debug::EnableInspector(bool enableReplayDebug) {
    g_EnableInspector = true;
    g_EnableReplayDebug = enableReplayDebug;
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

  bool Debug::SetDebugEventListener(Isolate* isolate, EventCallback that,
    Local<Value> data) {
    return false;
  }

  void Debug::SetLiveEditEnabled(Isolate* isolate, bool enable) {
    // CHAKRA-TODO: Figure out what to do here
    //
    // @digitalinfinity: This is edit and continue, right? I don't recall if
    // there are JSRT APIs enabled for this today but if there aren't, it would
    // be an interesting exercise to see what would be needed here (and how
    // Chakra's Edit and Continue differs from v8's). @jianxu would be the
    // expert here in Sandeep's absence
    CHAKRA_ASSERT(false);
  }

}  // namespace v8
