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

String::Utf8Value::Utf8Value(Handle<v8::Value> obj)
    : _str(nullptr), _length(0) {
  Handle<String> str = obj->ToString();
  if (str.IsEmpty()) {
    return;
  }

  jsrt::StringUtf8 strUtf8;

  if (strUtf8.From(*str) == JsNoError) {
    _length = strUtf8.length();
    _str = strUtf8.Detach();
    _str[_length] = '\0';
  }
}

String::Utf8Value::~Utf8Value() {
  if (_str != nullptr) {
    free(_str);
    _str = nullptr;
    _length = 0;
  }
}

String::Value::Value(Handle<v8::Value> obj) : _str(nullptr), _length(0) {
  Handle<String> str = obj->ToString();
  if (str.IsEmpty()) {
    return;
  }

  _length = str->Length();
  _str = new uint16_t[_length + 1];
  str->Write(_str);
}

String::Value::~Value() {
  if (_str != nullptr) {
    delete _str;
  }
}

int String::Length() const {
  int length = 0;

  if (JsGetStringLength((JsValueRef)this, &length) != JsNoError) {
    return 0;
  }

  return length;
}

int String::Utf8Length() const {
  jsrt::StringUtf8 str;
  if (str.LengthFrom((JsValueRef)this) != JsNoError) {
    // error
    return 0;
  }
  return str.length();
}

int String::Write(uint16_t *buffer, int start, int length, int options) const {
  size_t count = 0;
  if (JsCopyStringUtf16((JsValueRef)this, start, length,
                         buffer, &count) == JsNoError) {
    if (!(options & String::NO_NULL_TERMINATION) &&
        (length == -1 || count < length)) {
      buffer[count] = '\0';
    }
  }
  return count;
}

int String::WriteOneByte(
    uint8_t* buffer, int start, int length, int options) const {
  size_t count = 0;
  if (JsCopyStringOneByte((JsValueRef)this, start, length,
                          reinterpret_cast<char *>(buffer),
                          &count) == JsNoError) {
    if (!(options & String::NO_NULL_TERMINATION) &&
        (length == -1 || count < length)) {
      buffer[count] = '\0';
    }
  }
  return count;
}

int String::WriteUtf8(
    char *buffer, int length, int *nchars_ref, int options) const {
  if (length < 0) {
    // in case length was not provided we want to copy the whole string
    length = String::kMaxLength;
  }

  size_t count = 0;
  if (JsCopyString((JsValueRef)this,
                   buffer, length, &count, nullptr) == JsNoError) {
    if (count < (unsigned)length && !(options & String::NO_NULL_TERMINATION)) {
      // Utf8 version count includes null terminator
      buffer[count++] = 0;
    }
  }

  if (nchars_ref) {
    *nchars_ref = count;
  }

  return static_cast<int>(count);
}

Local<String> String::Empty(Isolate* isolate) {
  return FromMaybe(Utils::NewString("", 0));
}

String* String::Cast(v8::Value *obj) {
  CHAKRA_ASSERT(obj->IsString());
  return static_cast<String*>(obj);
}

MaybeLocal<String> Utils::NewString(const char *data, int length) {
  if (length < 0) {
    length = static_cast<int>(strlen(data));
  }

  JsValueRef strRef;
  if (JsCreateString(data, length, &strRef) != JsNoError) {
    return Local<String>();
  }

  return Local<String>::New(strRef);
}

MaybeLocal<String> String::NewFromUtf8(Isolate* isolate,
                                       const char* data,
                                       v8::NewStringType type,
                                       int length) {
  if (length < 0) {
    length = strlen(data);
  }

  JsValueRef strRef;
  if (JsCreateString(data, length, &strRef) != JsNoError) {
    return Local<String>();
  }

  return Local<String>::New(strRef);
}

Local<String> String::NewFromUtf8(Isolate* isolate,
                                  const char* data,
                                  NewStringType type,
                                  int length) {
  return FromMaybe(NewFromUtf8(isolate, data,
                               static_cast<v8::NewStringType>(type), length));
}

MaybeLocal<String> String::NewFromOneByte(Isolate* isolate,
                                          const uint8_t* data,
                                          v8::NewStringType type,
                                          int length) {
  if (length < 0) {
    length = strlen((const char*)data);
  }

  // convert latin-1 to utf8 for JsCreateString
  uint8_t* utf8 = new uint8_t[length*2];
  uint8_t* p = utf8;

  for (size_t i = 0; i < length; i++) {
    uint8_t ch = data[i];
    if (ch < 0x80) {
      *p++ = ch;
    } else {
      *p++ = 0xc0 | ch >> 6;
      *p++ = 0x80 | (ch & 0x3f);
    }
  }

  size_t utf8length = p - utf8;

  JsValueRef strRef;
  JsErrorCode ret = JsCreateString((const char*)utf8, utf8length, &strRef);

  delete[] utf8;

  if (ret != JsNoError) {
    return Local<String>();
  }

  return Local<String>::New(strRef);
}

Local<String> String::NewFromOneByte(Isolate* isolate,
                                     const uint8_t* data,
                                     NewStringType type,
                                     int length) {
  return FromMaybe(NewFromOneByte(isolate, data,
                                  static_cast<v8::NewStringType>(type),
                                  length));
}

static size_t WideStrLen(const uint16_t *p) {
  size_t len = 0;
  while (*p++) {
    len++;
  }
  return len;
}

MaybeLocal<String> String::NewFromTwoByte(Isolate* isolate,
                                          const uint16_t* data,
                                          v8::NewStringType type,
                                          int length) {
  if (length < 0) {
    length = WideStrLen(data);
  }

  JsValueRef strRef;
  if (JsCreateStringUtf16(data, length, &strRef) != JsNoError) {
    return Local<String>();
  }

  return Local<String>::New(strRef);
}

Local<String> String::NewFromTwoByte(Isolate* isolate,
                                     const uint16_t* data,
                                     NewStringType type,
                                     int length) {
  return FromMaybe(NewFromTwoByte(isolate, data,
                                  static_cast<v8::NewStringType>(type),
                                  length));
}

Local<String> String::Concat(Handle<String> left, Handle<String> right) {
  JsValueRef args[] = { *left, *right };
  JsValueRef result;

  JsValueRef stringConcatFunction =
    jsrt::ContextShim::GetCurrent()->GetStringConcatFunction();
  JsErrorCode error = JsCallFunction(stringConcatFunction,
                                     args, _countof(args), &result);
  if (error != JsNoError) {
    return Local<String>();
  }

  return Local<String>::New(result);
}

MaybeLocal<String> String::NewExternalTwoByte(
    Isolate* isolate, ExternalStringResource* resource) {
  if (resource->data() != nullptr) {
    auto newStr = NewFromTwoByte(nullptr, resource->data(),
                                 v8::NewStringType::kNormal,
                                 static_cast<int>(resource->length()));
    resource->Dispose();
    return newStr;
  }

  // otherwise the resource is empty just delete it and return an empty string
  resource->Dispose();
  return Empty(nullptr);
}

Local<String> String::NewExternal(Isolate* isolate,
                                  ExternalStringResource* resource) {
  return FromMaybe(NewExternalTwoByte(isolate, resource));
}

MaybeLocal<String> String::NewExternalOneByte(
    Isolate* isolate, ExternalOneByteStringResource* resource) {
  if (resource->data() != nullptr) {
    auto newStr = NewFromOneByte(
      nullptr,
      reinterpret_cast<const uint8_t*>(resource->data()),
      v8::NewStringType::kNormal,
      static_cast<int>(resource->length()));

    resource->Dispose();
    return newStr;
  }

  // otherwise the resource is empty just delete it and return an empty string
  resource->Dispose();
  return Empty(nullptr);
}

Local<String> String::NewExternal(Isolate* isolate,
                                  ExternalOneByteStringResource *resource) {
  return FromMaybe(NewExternalOneByte(isolate, resource));
}

}  // namespace v8

