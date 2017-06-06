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
#include <string>
#include <cstdlib>
#include <assert.h>

namespace jsrt {

// Trap names:
static CachedPropertyIdRef
const s_proxyTrapCachedPropertyIdRef[ProxyTraps::TrapCount] = {
  CachedPropertyIdRef::apply,
  CachedPropertyIdRef::construct,
  CachedPropertyIdRef::defineProperty,
  CachedPropertyIdRef::deleteProperty,
  CachedPropertyIdRef::enumerate,
  CachedPropertyIdRef::get,
  CachedPropertyIdRef::getOwnPropertyDescriptor,
  CachedPropertyIdRef::getPrototypeOf,
  CachedPropertyIdRef::has,
  CachedPropertyIdRef::isExtensible,
  CachedPropertyIdRef::ownKeys,
  CachedPropertyIdRef::preventExtensions,
  CachedPropertyIdRef::set,
  CachedPropertyIdRef::setPrototypeOf
};

CachedPropertyIdRef GetProxyTrapCachedPropertyIdRef(ProxyTraps trap) {
  return s_proxyTrapCachedPropertyIdRef[trap];
}

JsErrorCode SetPropertyOnTrapConfig(
    ProxyTraps trap, JsNativeFunction callback, JsValueRef configObj) {
  if (callback == nullptr) {
    return JsNoError;
  }

  JsErrorCode error;

  JsValueRef func;
  error = JsCreateFunction(callback, nullptr, &func);
  if (error != JsNoError) {
    return error;
  }

  JsPropertyIdRef prop =
    IsolateShim::GetCurrent()->GetProxyTrapPropertyIdRef(trap);

  error = JsSetProperty(configObj, prop, func, false);
  return error;
}

JsErrorCode CreateProxyTrapConfig(
    const JsNativeFunction proxyConf[ProxyTraps::TrapCount],
    JsValueRef *confObj) {
  JsErrorCode error = JsNoError;

  error = JsCreateObject(confObj);

  if (error != JsNoError) {
    return error;
  }

  // Set the properties of the proxy configuration object according to the given
  // map of proxy traps and function handlers For each proxy trap - set the
  // given handler using the appropriate javascript name
  for (int i = 0; i < ProxyTraps::TrapCount; i++) {
    error = SetPropertyOnTrapConfig((ProxyTraps)i, proxyConf[i], *confObj);
    if (error != JsNoError) {
      return error;
    }
  }

  return JsNoError;
}

JsErrorCode CreateProxy(
    JsValueRef target,
    const JsNativeFunction config[ProxyTraps::TrapCount],
    JsValueRef *result) {
  JsErrorCode error;

  JsValueRef proxyConfigObj;
  error = CreateProxyTrapConfig(config, &proxyConfigObj);
  if (error != JsNoError) {
    return error;
  }

  JsValueRef proxyConstructorRef =
    ContextShim::GetCurrent()->GetProxyConstructor();
  error = ConstructObject(proxyConstructorRef, target, proxyConfigObj, result);
  return error;
}

JsErrorCode TryParseUInt32(
    JsValueRef strRef, bool* isUInt32, unsigned int *uint32Value) {
  JsErrorCode error;

  *isUInt32 = false;

  // check that every character in the str is a digit, and that the string does
  // not start with a zero, unless it is a zero
  StringUtf8 strPtr;

  error = strPtr.From(strRef);
  if (error != JsNoError) {
    // If strRef is not a string, just return not Uint32
    return error == JsErrorInvalidArgument ? JsNoError : error;
  }

  // empty string
  if (strPtr.length() == 0) {
    return JsNoError;
  }

  // deal with the case in which zero is the first letter, in which we will
  // accept it only if the string reperesents zero itself
  if (strPtr[0] == '0') {
    if (strPtr.length() == 1) {
      *uint32Value = 0;
      *isUInt32 = true;
    }

    return JsNoError;
  }

  // iterate over the charecters, allow only digits:
  for (int i = 0; i < strPtr.length(); i++) {
    if (strPtr[i] < '0' || strPtr[i] > '9') {
      return JsNoError;
    }
  }

  // use std:stoull as it the most comprehenisve way to convert string to int
  // there is some performance issue here, so we might optimize this code using
  // the results in here: string to int conversion - naive approach is the
  // fastest:
  // http://tinodidriksen.com/2010/02/16/cpp-convert-string-to-int-speed/

  char* strEnd;
  unsigned long longVal = std::strtoul(strPtr,  // NOLINT(runtime/int)
                                       &strEnd,
                                       10);
  if (strEnd != strPtr + strPtr.length()) {
    return JsNoError;
  }

  // xplat-todo: revisit range check
  if (longVal > UINT32_MAX) {
    // check if errno is set:
    if (errno == ERANGE) {
      return JsNoError;
    }
  }

  *isUInt32 = true;
  *uint32Value = longVal;
  return JsNoError;
}

}  // namespace jsrt
