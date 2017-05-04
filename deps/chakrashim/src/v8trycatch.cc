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

namespace v8 {

TryCatch::TryCatch(Isolate* isolate)
    : metadata(JS_INVALID_REFERENCE),
      rethrow(false),
      user(true),
      verbose(false) {
  jsrt::IsolateShim * isolateShim = jsrt::IsolateShim::GetCurrent();
  prev = isolateShim->tryCatchStackTop;
  isolateShim->tryCatchStackTop = this;
}

TryCatch::~TryCatch() {
  if (!rethrow) {
    GetAndClearException();
  }

  jsrt::IsolateShim::GetCurrent()->tryCatchStackTop = prev;
}

bool TryCatch::HasCaught() const {
  if (metadata == JS_INVALID_REFERENCE) {
    const_cast<TryCatch*>(this)->GetAndClearException();
  }

  if (metadata != JS_INVALID_REFERENCE) {
    return true;
  }
  bool hasException;
  JsErrorCode errorCode = JsHasException(&hasException);
  if (errorCode != JsNoError) {
    if (errorCode == JsErrorInDisabledState) {
      return true;
    }
    // Should never get errorCode other than JsNoError/JsErrorInDisabledState
    CHAKRA_ASSERT(false);
    return false;
  }

  return hasException;
}

bool TryCatch::HasTerminated() const {
  return jsrt::IsolateShim::GetCurrent()->IsExeuctionDisabled();
}

void TryCatch::GetAndClearException() {
  bool hasException;
  JsErrorCode errorCode = JsHasException(&hasException);
  if (errorCode != JsNoError) {
    // If script is in disabled state, no need to fail here.
    // We will fail subsequent calls to Jsrt anyway.
    CHAKRA_ASSERT(errorCode == JsErrorInDisabledState);
    return;
  }

  if (hasException) {
    JsValueRef metadataRef;
    errorCode = JsGetAndClearExceptionWithMetadata(&metadataRef);
    // We came here through JsHasException, so script shouldn't be in disabled
    // state.
    CHAKRA_ASSERT(errorCode != JsErrorInDisabledState);
    if (errorCode == JsNoError) {
      metadata = metadataRef;
    }
  }
}

Handle<Value> TryCatch::ReThrow() {
  JsValueRef error = this->EnsureException();
  if (error == JS_INVALID_REFERENCE) {
    return Local<Value>();
  }

  if (JsSetException(error) != JsNoError) {
    return Handle<Value>();
  }
  rethrow = true;

  return Local<Value>::New(error);
}

Local<Value> TryCatch::Exception() const {
  JsValueRef error = this->EnsureException();
  if (error == JS_INVALID_REFERENCE) {
    return Local<Value>();
  }

  return Local<Value>::New(error);
}

MaybeLocal<Value> TryCatch::StackTrace(Local<Context> context) const {
  JsValueRef error = this->EnsureException();
  if (error == JS_INVALID_REFERENCE) {
    return Local<Value>();
  }

  JsPropertyIdRef stack = jsrt::IsolateShim::GetCurrent()
      ->GetCachedPropertyIdRef(jsrt::CachedPropertyIdRef::stack);

  JsValueRef trace;
  if (JsGetProperty(error, stack, &trace) != JsNoError) {
    return Local<Value>();
  }

  return Local<Value>::New(trace);
}

Local<Value> TryCatch::StackTrace() const {
  return FromMaybe(StackTrace(Local<Context>()));
}

Local<v8::Message> TryCatch::Message() const {
  if (metadata == JS_INVALID_REFERENCE) {
    const_cast<TryCatch*>(this)->GetAndClearException();
  }

  if (metadata == JS_INVALID_REFERENCE) {
    return Local<v8::Message>();
  }
  return Local<v8::Message>::New(metadata);
}

void TryCatch::SetVerbose(bool value) {
  this->verbose = value;
}

bool TryCatch::IsVerbose() const {
  return this->verbose;
}

void TryCatch::CheckReportExternalException() {
  // Let caller TryCatch record the exception
  TryCatch* tryCatch = (prev != nullptr && prev->user) ? prev : this;
  if (tryCatch == prev) {
    tryCatch->GetAndClearException();
  }

  // This is only used by Function::Call. If caller does not use TryCatch to
  // handle external exceptions, or uses a TryCatch and SetVerbose(),
  // we'll report the external exception message (triggers uncaughtException).
  if (prev == nullptr || prev->verbose) {
    jsrt::IsolateShim::GetCurrent()->ForEachMessageListener(
      [tryCatch](void * messageListener) {
        ((v8::MessageCallback)messageListener)(tryCatch->Message(),
                                               tryCatch->Exception());
    });
  } else {
    rethrow = true;  // Otherwise leave the exception as is
  }
}

JsValueRef TryCatch::EnsureException() const {
  if (metadata == JS_INVALID_REFERENCE) {
    const_cast<TryCatch*>(this)->GetAndClearException();
  }

  if (metadata == JS_INVALID_REFERENCE) {
    return JS_INVALID_REFERENCE;
  }

  JsPropertyIdRef err = jsrt::IsolateShim::GetCurrent()
      ->GetCachedPropertyIdRef(jsrt::CachedPropertyIdRef::exception);

  JsValueRef exception;
  if (JsGetProperty(metadata, err, &exception) != JsNoError) {
    return JS_INVALID_REFERENCE;
  }

  return exception;
}

}  // namespace v8
