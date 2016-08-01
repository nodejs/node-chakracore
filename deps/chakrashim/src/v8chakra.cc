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

using jsrt::IsolateShim;

Object* TemplateData::EnsureProperties() {
  if (properties.IsEmpty()) {
    properties = Object::New();
  }

  return *properties;
}

bool TemplateData::Is(ExternalData* data) {
  return data->GetType() == ExternalDataTypes::ObjectTemplateData
      || data->GetType() == ExternalDataTypes::FunctionTemplateData;
}

// CHAKRA-TODO: Convert this function to javascript?
// Clone the template properties into the new instance
JsErrorCode TemplateData::CopyPropertiesTo(JsValueRef newInstance) {
  if (properties.IsEmpty()) {
    return JsNoError;
  }

  JsValueRef propertiesRef = *properties;
  JsValueRef propertyNames;
  IfJsErrorRet(JsGetOwnPropertyNames(propertiesRef, &propertyNames));

  unsigned int length;
  IfJsErrorRet(jsrt::GetArrayLength(propertyNames, &length));

  JsPropertyIdRef valueIdRef;
  if (length > 0) {
    valueIdRef = IsolateShim::GetCurrent()->GetCachedPropertyIdRef(
                  jsrt::CachedPropertyIdRef::value);
  }

  for (unsigned int index = 0; index < length; index++) {
    JsValueRef indexValue;
    IfJsErrorRet(JsIntToNumber(index, &indexValue));

    JsValueRef propertyNameValue;
    IfJsErrorRet(JsGetIndexedProperty(propertyNames, indexValue,
                                      &propertyNameValue));

    JsPropertyIdRef propertyId;
    IfJsErrorRet(jsrt::GetPropertyIdFromName(propertyNameValue, &propertyId));

    JsValueRef propertyDescriptor;
    IfJsErrorRet(JsGetOwnPropertyDescriptor(propertiesRef, propertyId,
                                            &propertyDescriptor));

    // If it is a Template, try create an instance
    JsValueRef value;
    IfJsErrorRet(JsGetProperty(propertyDescriptor, valueIdRef, &value));
    ExternalData* data = nullptr;
    if (ExternalData::TryGet(value, &data) && TemplateData::Is(data)) {
      TemplateData* templateData = static_cast<TemplateData*>(data);
      value = templateData->NewInstance(value);
      if (value == JS_INVALID_REFERENCE) {
        return JsErrorFatal;  // Just to indicate failed
      }
      IfJsErrorRet(JsSetProperty(propertyDescriptor, valueIdRef, value, false));
    }

    bool result;
    IfJsErrorRet(JsDefineProperty(newInstance, propertyId, propertyDescriptor,
                                  &result));
  }

  return JsNoError;
}

}  // namespace v8
