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

struct PropertyDescriptor::PrivateData {
  PrivateData()
      : enumerable(false),
        has_enumerable(false),
        configurable(false),
        has_configurable(false),
        writable(false),
        has_writable(false),
        value(JS_INVALID_REFERENCE),
        get(JS_INVALID_REFERENCE),
        set(JS_INVALID_REFERENCE) {}

  bool has_value() { return value != JS_INVALID_REFERENCE; }
  bool has_get() { return get != JS_INVALID_REFERENCE; }
  bool has_set() { return set != JS_INVALID_REFERENCE; }

  bool enumerable : 1;
  bool has_enumerable : 1;
  bool configurable : 1;
  bool has_configurable : 1;
  bool writable : 1;
  bool has_writable : 1;
  JsValueRef value;
  JsValueRef get;
  JsValueRef set;
};

PropertyDescriptor::PropertyDescriptor() : private_(new PrivateData()) {}

PropertyDescriptor::PropertyDescriptor(Local<Value> value)
    : private_(new PrivateData()) {
  private_->value = reinterpret_cast<JsValueRef>(*value);
  if (private_->value != JS_INVALID_REFERENCE) {
    JsAddRef(private_->value, nullptr);
  }
}

PropertyDescriptor::PropertyDescriptor(Local<Value> value,
                                       bool writable)
    : private_(new PrivateData()) {
  private_->value = reinterpret_cast<JsValueRef>(*value);
  if (private_->value != JS_INVALID_REFERENCE) {
    JsAddRef(private_->value, nullptr);
  }
  private_->has_writable = true;
  private_->writable = writable;
}

PropertyDescriptor::PropertyDescriptor(Local<Value> get,
                                       Local<Value> set)
    : private_(new PrivateData()) {
  CHAKRA_ASSERT(get.IsEmpty() || get->IsUndefined() || get->IsFunction());
  CHAKRA_ASSERT(set.IsEmpty() || set->IsUndefined() || set->IsFunction());
  private_->get = reinterpret_cast<JsValueRef>(*get);
  private_->set = reinterpret_cast<JsValueRef>(*set);
  if (private_->get != JS_INVALID_REFERENCE) {
    JsAddRef(private_->get, nullptr);
  }
  if (private_->set != JS_INVALID_REFERENCE) {
    JsAddRef(private_->set, nullptr);
  }
}

PropertyDescriptor::~PropertyDescriptor() {
  if (private_->value != JS_INVALID_REFERENCE) {
    JsRelease(private_->value, nullptr);
  }
  if (private_->get != JS_INVALID_REFERENCE) {
    JsRelease(private_->get, nullptr);
  }
  if (private_->set != JS_INVALID_REFERENCE) {
    JsRelease(private_->set, nullptr);
  }
  delete private_;
}

Local<Value> PropertyDescriptor::value() const {
  CHAKRA_ASSERT(private_->has_value());
  return Local<Value>::New(private_->value);
}

Local<Value> PropertyDescriptor::get() const {
  return Local<Value>::New(private_->get);
}

Local<Value> PropertyDescriptor::set() const {
  return Local<Value>::New(private_->set);
}

bool PropertyDescriptor::has_value() const {
  return private_->has_value();
}
bool PropertyDescriptor::has_get() const {
  return private_->has_get();
}
bool PropertyDescriptor::has_set() const {
  return private_->has_set();
}

bool PropertyDescriptor::writable() const {
  CHAKRA_ASSERT(private_->has_writable);
  return private_->writable;
}

bool PropertyDescriptor::has_writable() const {
  return private_->has_writable;
}

void PropertyDescriptor::set_enumerable(bool enumerable) {
  private_->has_enumerable = true;
  private_->enumerable = enumerable;
}

bool PropertyDescriptor::enumerable() const {
  CHAKRA_ASSERT(private_->has_enumerable);
  return private_->enumerable;
}

bool PropertyDescriptor::has_enumerable() const {
  return private_->has_enumerable;
}

void PropertyDescriptor::set_configurable(bool configurable) {
  private_->has_configurable = true;
  private_->configurable = configurable;
}

bool PropertyDescriptor::configurable() const {
  CHAKRA_ASSERT(private_->has_configurable);
  return private_->configurable;
}

bool PropertyDescriptor::has_configurable() const {
  return private_->has_configurable;
}

}  // namespace v8
