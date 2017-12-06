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

Local<Map> Map::New(Isolate* isolate) {
  JsValueRef mapConstructor =
    ContextShim::GetCurrent()->GetMapConstructor();

  JsValueRef newMapRef;
  if (jsrt::ConstructObject(mapConstructor, &newMapRef) != JsNoError) {
    CHAKRA_ASSERT(false);
    return Local<Map>();
  }

  return Local<Map>::New(newMapRef);
}

MaybeLocal<Value> Map::Get(Local<Context> context, Local<Value> key) {
  JsValueRef mapGetFunction =
    ContextShim::GetCurrent()->GetMapGetFunction();

  JsValueRef mapGetResult;
  JsValueRef args[] = { (JsValueRef)this, *key };
  if (JsCallFunction(mapGetFunction, args, _countof(args),
                     &mapGetResult) != JsNoError) {
    CHAKRA_ASSERT(false);
    return MaybeLocal<Value>();
  }

  return Local<Value>::New(mapGetResult);
}

MaybeLocal<Map> Map::Set(Local<Context> context, Local<Value> key,
                         Local<Value> value) {
  JsValueRef mapSetFunction =
    ContextShim::GetCurrent()->GetMapSetFunction();

  JsValueRef mapSetResult;
  JsValueRef args[] = { (JsValueRef)this, *key, *value };
  if (JsCallFunction(mapSetFunction, args, _countof(args),
                     &mapSetResult) != JsNoError) {
    CHAKRA_ASSERT(false);
    return MaybeLocal<Map>();
  }

  return Local<Map>::New(mapSetResult);
}

Maybe<bool> Map::Has(Local<Context> context, Local<Value> key) {
  JsValueRef mapHasFunction =
    ContextShim::GetCurrent()->GetMapHasFunction();

  JsValueRef mapHasResult;
  JsValueRef args[] = { (JsValueRef)this, *key };
  if (JsCallFunction(mapHasFunction, args, _countof(args),
                     &mapHasResult) != JsNoError) {
    CHAKRA_ASSERT(false);
    return Nothing<bool>();
  }

  bool hasResult = false;
  if (jsrt::ValueToNative</*LIKELY*/true>(JsConvertValueToBoolean,
                                          JsBooleanToBool,
                                          mapHasResult,
                                          &hasResult) != JsNoError) {
    CHAKRA_ASSERT(false);
    return Nothing<bool>();
  }

  return Just(hasResult);
}

Map* Map::Cast(Value* obj) {
  CHAKRA_ASSERT(obj->IsMap());
  return static_cast<Map*>(obj);
}

}  // namespace v8
