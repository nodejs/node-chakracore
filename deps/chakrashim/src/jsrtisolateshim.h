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

#include <unordered_map>

namespace v8
{
  class Isolate;
  class TryCatch;
};

namespace jsrt
{
  enum CachedPropertyIdRef
  {
#define DEF(x,...) x,
#include "jsrtcachedpropertyidref.h"
    Count
  };

  class IsolateShim
  {
  public:

    bool IsolateShim::NewContext(JsContextRef * context, bool exposeGC);
    bool GetMemoryUsage(size_t * memoryUsage);
    bool Dispose();

    static v8::Isolate * New();
    static v8::Isolate * GetCurrentAsIsolate();
    static IsolateShim * GetCurrent();
    static IsolateShim * FromIsolate(v8::Isolate * isolate);
    static void DisposeAll();

    ContextShim * GetContextShim(JsContextRef contextRef);
    JsRuntimeHandle GetRuntimeHandle();
    ContextShim * GetObjectContext(JsValueRef valueRef);

    void Enter();
    void Exit();

    void PushScope(ContextShim::Scope * scope, JsContextRef contextRef);
    void PushScope(ContextShim::Scope * scope, ContextShim * contextShim);
    void PopScope(ContextShim::Scope * scope);

    ContextShim * GetCurrentContextShim();

    // Symbols proeprtyIdRef
    JsPropertyIdRef GetSelfSymbolPropertyIdRef();
    JsPropertyIdRef GetCrossContextTargetSymbolPropertyIdRef();
    JsPropertyIdRef GetKeepAliveObjectSymbolPropertyIdRef();

    // String proeprtyIdRef
    JsPropertyIdRef GetProxyTrapPropertyIdRef(ProxyTraps trap);
    JsPropertyIdRef GetCachedPropertyIdRef(CachedPropertyIdRef cachedPropertyIdRef);

    // CHAKRA-REVIEW: Work around the fact that chakra doesn't support cross context
    // use of object, we need to keep track of the context of these object so we can
    // switch back and forth
    void RegisterJsValueRefContextShim(JsValueRef valueRef);
    void UnregisterJsValueRefContextShim(JsValueRef valueRef);
    ContextShim * GetJsValueRefContextShim(JsValueRef valueRef);

    void DisableExecution();
    bool IsExeuctionDisabled();
    void EnableExecution();
  private:
    // Construction/Destruction should go thru New/Dispose
    IsolateShim(JsRuntimeHandle runtime);
    ~IsolateShim();
    static v8::Isolate * ToIsolate(IsolateShim * isolate);
    static void CALLBACK JsContextBeforeCollectCallback(_In_ JsRef contextRef, _In_opt_ void *data);

    JsPropertyIdRef EnsurePrivateSymbol(JsPropertyIdRef * propertyIdRefPtr);
    JsPropertyIdRef EnsurePropertyIdRef(const wchar_t * name, JsPropertyIdRef * propertyIdRefPtr);

    JsRuntimeHandle runtime;
    JsPropertyIdRef selfSymbolPropertyIdRef;
    JsPropertyIdRef crossContextTargetSymbolPropertyIdRef;
    JsPropertyIdRef keepAliveObjectSymbolPropertyIdRef;
    JsPropertyIdRef cachedPropertyIdRefs[CachedPropertyIdRef::Count];

    ContextShim::Scope * contextScopeStack;
    IsolateShim ** prevnext;
    IsolateShim * next;

    friend class v8::TryCatch;
    v8::TryCatch * tryCatchStackTop;

    std::unordered_map<JsContextRef, ContextShim *> contextShimMap;
    std::unordered_map<JsValueRef, ContextShim *> jsValueRefToContextShimMap;


    // CHAKRA-TODO: support multiple shims
    static IsolateShim * s_isolateList;

    static __declspec(thread) IsolateShim * s_currentIsolate;
  };
};
