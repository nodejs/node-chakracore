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

#include <functional>
#include <vector>

namespace jsrt
{
  class ContextShim
  {
  public:
    // This has the same layout as v8::Context::Scope
    class Scope
    {
    private:
      Scope * previous;
      ContextShim * contextShim;
    public:
      Scope(ContextShim * context);
      ~Scope();

      friend class IsolateShim;
    };

    static ContextShim * New(IsolateShim * isolateShim, bool exposeGC);
    bool EnsureInitialized();

    IsolateShim * GetIsolateShim();
    JsContextRef GetContextRef();

    JsValueRef GetPromiseContinuationFunction();
    JsValueRef GetTrue();
    JsValueRef GetFalse();
    JsValueRef GetUndefined();
    JsValueRef GetNull();
    JsValueRef GetZero();
    JsValueRef GetBooleanObjectConstructor();
    JsValueRef GetNumberObjectConstructor();
    JsValueRef GetStringObjectConstructor();
    JsValueRef GetDateConstructor();
    JsValueRef GetProxyConstructor();
    JsValueRef GetGetOwnPropertyDescriptorFunction();
    JsValueRef GetStringConcatFunction();
    JsValueRef GetProxyOfGlobal();
    JsValueRef GetReflectObject();
    JsValueRef GetReflectFunctionForTrap(ProxyTraps traps);

    JsValueRef GetInstanceOfFunction();
    JsValueRef GetCloneObjectFunction();
    JsValueRef GetGetPropertyNamesFunction();
    JsValueRef GetGetEnumerableNamedPropertiesFunction();
    JsValueRef GetGetEnumerableIndexedPropertiesFunction();
    JsValueRef GetCreateEnumerationIteratorFunction();
    JsValueRef GetCreatePropertyDescriptorsEnumerationIteratorFunction();
    JsValueRef GetGetNamedOwnKeysFunction();
    JsValueRef GetGetIndexedOwnKeysFunction();
    JsValueRef GetCreateEmptyLambdaFunction();

    void * GetAlignedPointerFromEmbedderData(int index);
    void SetAlignedPointerInEmbedderData(int index, void * value);

    static ContextShim * GetCurrent();

    template <class R>
    static R ExecuteInContextOf(JsValueRef object, const std::function<R()> & fn);

  private:
    ContextShim(IsolateShim * isolateShim, JsContextRef context, bool exposeGC);
    bool InitializeBuiltIns();
    bool InitializeProxyOfGlobal();
    bool InitializeReflect();

    template <typename Fn>
    bool InitializeBuiltIn(JsValueRef * builtIntValue, Fn getBuiltIn);
    bool KeepAlive(JsValueRef value);

    bool ExposeGc();
    bool ExecuteChakraPatch();

    IsolateShim * isolateShim;
    JsContextRef context;
    bool initialized;
    bool exposeGC;
    JsValueRef keepAliveObject;
    int builtInCount;

    JsValueRef trueRef;
    JsValueRef falseRef;
    JsValueRef undefinedRef;
    JsValueRef nullRef;
    JsValueRef zero;
    JsValueRef objectConstructor;
    JsValueRef numberObjectConstructor;
    JsValueRef booleanObjectConstructor;
    JsValueRef stringObjectConstructor;
    JsValueRef dateConstructor;
    JsValueRef proxyConstructor;
    JsValueRef globalObject;
    JsValueRef proxyOfGlobal;
    JsValueRef reflectObject;
    JsValueRef reflectFunctions[ProxyTraps::TrapCount];

    JsValueRef getOwnPropertyDescriptorFunction;
    JsValueRef stringConcatFunction;

    JsValueRef promiseContinuationFunction;

    JsValueRef instanceOfFunction;
    JsValueRef cloneObjectFunction;
    JsValueRef isUintFunction;
    JsValueRef getPropertyNamesFunction;

    JsValueRef getEnumerableNamedPropertiesFunction;
    JsValueRef getEnumerableIndexedPropertiesFunction;
    JsValueRef createEnumerationIteratorFunction;
    JsValueRef createPropertyDescriptorsEnumerationIteratorFunction;
    JsValueRef getNamedOwnKeysFunction;
    JsValueRef getIndexedOwnKeysFunction;
    JsValueRef createEmptyLambdaFunction;

    std::vector<void*> embedderData;
  };

  template <class R>
  R ContextShim::ExecuteInContextOf(JsValueRef object, const std::function<R()> & fn)
  {
    IsolateShim * isolateShim = IsolateShim::GetCurrent();
    ContextShim * contextShim = isolateShim->GetJsValueRefContextShim(object);
    if (contextShim != nullptr && contextShim != isolateShim->GetCurrentContextShim())
    {
      ContextShim::Scope scope(contextShim);
      return fn();
    }
    return fn();
  }
};
