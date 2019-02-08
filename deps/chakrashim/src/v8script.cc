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

#include "v8chakra.h"
#include <memory>
#include <string>

namespace v8 {

using CachedPropertyIdRef = jsrt::CachedPropertyIdRef;

THREAD_LOCAL JsSourceContext currentContext;
extern bool g_useStrict;

Local<Script> Script::Compile(Handle<String> source, ScriptOrigin* origin) {
  return FromMaybe(Compile(Local<Context>(), source, origin));
}

// CHAKRA-TODO: Convert this function to javascript
// Create a object to hold the script infomration
static JsErrorCode CreateScriptObject(JsValueRef sourceRef,
                                      JsValueRef filenameRef,
                                      JsValueRef scriptFunction,
                                      JsValueRef * scriptObject) {
  JsErrorCode error = JsCreateObject(scriptObject);
  if (error != JsNoError) {
    return error;
  }

  error = jsrt::SetProperty(*scriptObject, CachedPropertyIdRef::source,
                            sourceRef);
  if (error != JsNoError) {
    return error;
  }

  error = jsrt::SetProperty(*scriptObject, CachedPropertyIdRef::filename,
                            filenameRef);
  if (error != JsNoError) {
    return error;
  }

  return jsrt::SetProperty(*scriptObject, CachedPropertyIdRef::function,
                           scriptFunction);
}

const ScriptCompiler::CachedData* ScriptCompiler::Source::GetCachedData()
    const {
  return this->cached_data;
}

MaybeLocal<Script> Script::CompileWithParserState(Local<Context> context,
                                                  Handle<String> source,
                                                  const uint8_t* buffer,
                                                  unsigned int bufferLength,
                                                  ScriptOrigin* origin) {
  JsValueRef filenameRef;
  const char* filename = "";
  if (origin != nullptr) {
    filenameRef = *origin->ResourceName();
  } else {
    if (JsCreateString(filename, strlen(filename), &filenameRef) != JsNoError) {
      return Local<Script>();
    }
  }
  JsValueRef sourceRef;
  jsrt::StringUtf8 script;
  if (jsrt::ToString(*source, &sourceRef, &script) != JsNoError) {
    return Local<Script>();
  }

  JsValueRef scriptFunction;
  // TODO: isolate?
  JsValueRef parserState =
      *v8::ArrayBuffer::New(nullptr, (void*)buffer, bufferLength);
  if (jsrt::DeserializeParserState(&script,
                                   currentContext++,
                                   filenameRef,
                                   parserState,
                                   &scriptFunction) != JsNoError) {
    return Local<Script>();
  }

  JsValueRef scriptObject;
  if (CreateScriptObject(
          sourceRef, filenameRef, scriptFunction, &scriptObject) != JsNoError) {
    return Local<Script>();
  }

  return Local<Script>::New(scriptObject);
}

// Compiled script object, bound to the context that was active when this
// function was called. When run it will always use this context.
MaybeLocal<Script> Script::Compile(Local<Context> context,
                                   Handle<String> source,
                                   ScriptOrigin* origin) {
  JsErrorCode error = JsNoError;
  JsValueRef filenameRef;
  const char* filename = "";

  if (origin != nullptr) {
    filenameRef = *origin->ResourceName();
  }

  if (origin == nullptr || filenameRef == nullptr) {
    error = JsCreateString(filename,
                           strlen(filename),
                           &filenameRef);
  }

  if (error == JsNoError) {
    JsValueRef sourceRef;
    jsrt::StringUtf8 script;
    error = jsrt::ToString(*source, &sourceRef, &script);
    if (error == JsNoError) {
      JsValueRef scriptFunction;
      error = jsrt::ParseScript(&script,
                                currentContext++,
                                filenameRef,
                                g_useStrict,
                                &scriptFunction);
      if (error == JsNoError) {
        JsValueRef scriptObject;
        error = CreateScriptObject(sourceRef, filenameRef, scriptFunction,
                                   &scriptObject);
        if (error == JsNoError) {
          return Local<Script>::New(scriptObject);
        }
      }
    }
  }
  return Local<Script>();
}

Local<Script> Script::Compile(Handle<String> source,
                              Handle<String> file_name) {
  ScriptOrigin origin(file_name);
  return FromMaybe(Compile(Local<Context>(), source, &origin));
}

MaybeLocal<Value> Script::Run(Local<Context> context) {
  jsrt::IsolateShim::GetCurrent()->SetScriptExecuted();

  JsValueRef scriptFunction;
  if (jsrt::GetProperty(this, CachedPropertyIdRef::function,
                        &scriptFunction) != JsNoError) {
    return Local<Value>();
  }

  JsValueRef result;
  if (jsrt::CallFunction(scriptFunction, &result) != JsNoError) {
    return Local<Value>();
  }

  return Local<Value>::New(result);
}

Local<Value> Script::Run() {
  return FromMaybe(Run(Local<Context>()));
}

static void CHAKRA_CALLBACK UnboundScriptFinalizeCallback(void * data) {
  JsValueRef * unboundScriptData = static_cast<JsValueRef *>(data);
  delete unboundScriptData;
}
Local<UnboundScript> Script::GetUnboundScript() {
  // Chakra doesn't support unbound script, the script object contains all the
  // information to recompile

  JsValueRef * unboundScriptData = new JsValueRef;
  JsValueRef unboundScriptRef;
  if (JsCreateExternalObject(unboundScriptData,
                             UnboundScriptFinalizeCallback,
                             &unboundScriptRef) != JsNoError) {
    delete unboundScriptData;
    return Local<UnboundScript>();
  }

  if (jsrt::SetProperty(unboundScriptRef, CachedPropertyIdRef::script,
                        this) != JsNoError) {
    delete unboundScriptData;
    return Local<UnboundScript>();
  }
  *unboundScriptData = unboundScriptRef;
  return Local<UnboundScript>::New(unboundScriptRef);
}

Local<Script> UnboundScript::BindToCurrentContext() {
  jsrt::ContextShim * contextShim =
    jsrt::IsolateShim::GetContextShimOfObject(this);
  if (contextShim == jsrt::ContextShim::GetCurrent()) {
    JsValueRef scriptRef;
    if (jsrt::GetProperty(this, CachedPropertyIdRef::script,
                          &scriptRef) != JsNoError) {
      return Local<Script>();
    }
    // Same context, we can reuse the same script object
    return Local<Script>::New(scriptRef);
  }

  // Create a script object in another context
  jsrt::StringUtf8 source;
  jsrt::StringUtf8 filename;
  JsValueRef originalFilenameRef;
  {
    jsrt::ContextShim::Scope scope(contextShim);
    JsValueRef scriptRef;
    if (jsrt::GetProperty(this, CachedPropertyIdRef::script,
                          &scriptRef) != JsNoError) {
      return Local<Script>();
    }

    JsValueRef originalSourceRef;
    if (jsrt::GetProperty(scriptRef, CachedPropertyIdRef::source,
                          &originalSourceRef) != JsNoError) {
      return Local<Script>();
    }
    if (jsrt::GetProperty(scriptRef, CachedPropertyIdRef::filename,
                          &originalFilenameRef) != JsNoError) {
      return Local<Script>();
    }
    if (source.From(originalSourceRef) != JsNoError) {
      return Local<Script>();
    }
    if (filename.From(originalFilenameRef) != JsNoError) {
      return Local<Script>();
    }
  }

  JsValueRef scriptFunction;
  if (jsrt::ParseScript(&source, currentContext++, originalFilenameRef,
                        g_useStrict, &scriptFunction) != JsNoError) {
    return Local<Script>();
  }

  JsValueRef sourceRef;

  if (JsCreateString(*source, source.length(),
                     &sourceRef) != JsNoError) {
    return Local<Script>();
  }

  JsValueRef filenameRef;
  if (JsCreateString(*filename, filename.length(),
                     &filenameRef) != JsNoError) {
    return Local<Script>();
  }

  JsValueRef scriptObject;
  if (CreateScriptObject(sourceRef,
                         filenameRef,
                         scriptFunction,
                         &scriptObject) != JsNoError) {
    return Local<Script>();
  }

  return Local<Script>::New(scriptObject);
}

int UnboundScript::GetId() {
  // CHAKRA-TODO: Figure out what to do here
  CHAKRA_ASSERT(false);
  return 0;
}

ScriptCompiler::CachedData* ScriptCompiler::CreateCodeCache(
    Local<UnboundScript> unbound_script) {
  JsValueRef unboundScriptRef = *unbound_script;
  jsrt::ContextShim* contextShim =
      jsrt::ContextShim::FromJsValueRef(unboundScriptRef);

  jsrt::StringUtf8 source;
  jsrt::StringUtf8 filename;
  JsValueRef originalFilenameRef;
  {
    jsrt::ContextShim::Scope scope(contextShim);
    JsValueRef scriptRef;
    if (jsrt::GetProperty(unboundScriptRef,
                          CachedPropertyIdRef::script,
                          &scriptRef) != JsNoError) {
      return nullptr;
    }

    JsValueRef originalSourceRef;
    if (jsrt::GetProperty(scriptRef,
                          CachedPropertyIdRef::source,
                          &originalSourceRef) != JsNoError) {
      return nullptr;
    }
    if (jsrt::GetProperty(scriptRef,
                          CachedPropertyIdRef::filename,
                          &originalFilenameRef) != JsNoError) {
      return nullptr;
    }
    if (source.From(originalSourceRef) != JsNoError) {
      return nullptr;
    }
    if (filename.From(originalFilenameRef) != JsNoError) {
      return nullptr;
    }
  }

  JsValueRef bufferVal;
  if (jsrt::SerializeParserState(&source,
                                 currentContext++,
                                 originalFilenameRef,
                                 g_useStrict,
                                 &bufferVal) != JsNoError) {
    return nullptr;
  }

  uint8_t* buffer = nullptr;
  unsigned int bufferLength = 0;
  if (jsrt::GetArrayBufferStorage(bufferVal, &buffer, &bufferLength) !=
      JsNoError) {
    return nullptr;
  }

  // TODO: manage memory
  return new ScriptCompiler::CachedData(buffer, bufferLength);
}

ScriptCompiler::CachedData* ScriptCompiler::CreateCodeCache(
    Local<UnboundScript> unbound_script, Local<String> source) {
  return ScriptCompiler::CreateCodeCache(unbound_script);
}

MaybeLocal<UnboundScript> ScriptCompiler::CompileUnboundScript(
    Isolate* isolate, Source* source, CompileOptions options) {
  MaybeLocal<Script> maybe = Compile(Local<Context>(), source, options);
  if (maybe.IsEmpty()) {
    return Local<UnboundScript>();
  }
  return FromMaybe(maybe)->GetUnboundScript();
}

Local<UnboundScript> ScriptCompiler::CompileUnbound(
    Isolate* isolate, Source* source, CompileOptions options) {
  return FromMaybe(CompileUnboundScript(isolate, source, options));
}

MaybeLocal<Script> ScriptCompiler::Compile(Local<Context> context,
                                           Source* source,
                                           CompileOptions options) {
  ScriptOrigin origin(source->resource_name);
  if (options == kConsumeCodeCache && source->cached_data) {
    return Script::CompileWithParserState(context,
                                          source->source_string,
                                          source->cached_data->data,
                                          source->cached_data->length,
                                          &origin);
  } else {
    return Script::Compile(context, source->source_string, &origin);
  }
}

Local<Script> ScriptCompiler::Compile(Isolate* isolate,
                                      Source* source,
                                      CompileOptions options) {
  return FromMaybe(Compile(Local<Context>(), source, options));
}

uint32_t ScriptCompiler::CachedDataVersionTag() {
  return 0;
}

MaybeLocal<Module> ScriptCompiler::CompileModule(
    Isolate* isolate, Source* source) {
  // CHAKRA-TODO: how do we compile a module without running it?
  return Local<Module>();
  }

MaybeLocal<Function> ScriptCompiler::CompileFunctionInContext(
  Local<Context> context, Source* source, size_t arguments_count,
  Local<String> arguments[], size_t context_extension_count,
  Local<Object> context_extensions[],
  CompileOptions options,
  NoCacheReason no_cache_reason) {

  std::wstring gen =
    L"(function() {"
    L"return function() {"
    ;

  if (context_extension_count != 0) {
    gen += L"with (this) {";
    for (size_t i = 1; i < context_extension_count; i++) {
      gen += L"with (arguments[" + std::to_wstring(i - 1);
      gen += L"]) {";
    }
  }

  // body start

  gen += L" return function(";

  for (size_t i = 0; i < arguments_count; i++) {
    if (i != 0) { gen += L","; }
    v8::String::Value value(Isolate::GetCurrent(), arguments[i]);
    gen += (wchar_t *)*value;
  }

  gen += L") {";

  v8::String::Value sourceValue(Isolate::GetCurrent(), source->source_string);
  gen += (wchar_t *)*sourceValue;

  gen += L"};";

  // body end

  for (size_t i = 0; i < context_extension_count; i++) {
    gen += L"}";
  }

  gen += L"};})()";

  JsValueRef genStr;
  if (JsCreateStringUtf16((uint16_t*)gen.c_str(), gen.length(), &genStr) != JsNoError) {
    return MaybeLocal<Function>();
  }
  JsValueRef genFunc;
  if (JsRun(genStr, currentContext++, *source->resource_name, JsParseScriptAttributeNone, &genFunc) != JsNoError) {
    return MaybeLocal<Function>();
  }

  JsValueRef* args;
  size_t cargs;
  if (context_extension_count == 0) {
    JsValueRef undef;
    if (JsGetUndefinedValue(&undef) != JsNoError) {
      return MaybeLocal<Function>();
    }
    args = &undef;
    cargs = 1;
  } else {
    args = (JsValueRef*)context_extensions;
    cargs = context_extension_count;
  }
  JsValueRef retFunc;
  if (JsCallFunction(genFunc, args, cargs, &retFunc) !=
      JsNoError) {
    return MaybeLocal<Function>();
  }

  return Utils::ToLocal<Function>(retFunc);
}

}  // namespace v8
