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

#include "v8.h"
#include "jsrtutils.h"
#include <assert.h>

namespace jsrt
{
  struct CrossContextInfo
  {
    ContextShim * contextShim;
    JsValueRef crossContextObject;
  };

  static bool CheckMarshalFailed(JsErrorCode errorCode, bool allowScriptException = false)
  {
    switch (errorCode)
    {
    case JsNoError:
      return false;
    case JsErrorInDisabledState:
    case JsErrorScriptTerminated:
      // Allow script terminate to propagate.
      return true;
    case JsErrorScriptException:
      if (allowScriptException)
      {
        return true;
      }
      // Fall thru
    default:
      // CHAKRA-TODO: Fail fast?
      assert(false);
      return true;
    };

  }

  static CrossContextInfo * GetCrossContextInfo(JsValueRef targetObject)
  {
    JsValueRef crossContextInfoObject;
    if (CheckMarshalFailed(JsGetProperty(targetObject, IsolateShim::GetCurrent()->GetCachedPropertyIdRef(CachedPropertyIdRef::crossContextInfoObject), &crossContextInfoObject)))
    {
      return nullptr;
    }

    CrossContextInfo * crossContextInfo;
    if (CheckMarshalFailed(JsGetExternalData(crossContextInfoObject, (void**)&crossContextInfo)))
    {
      return nullptr;
    }
    return crossContextInfo;
  }

  static bool UnwrapIfCrossContext(JsValueRef valueRef, CrossContextInfo ** info)
  {
    JsValueRef crossContextTarget;
    if (CheckMarshalFailed(JsGetProperty(valueRef, IsolateShim::GetCurrent()->GetCrossContextTargetSymbolPropertyIdRef(), &crossContextTarget)))
    {
      return false;
    }

    bool notCrossContextTarget;
    if (CheckMarshalFailed(IsUndefined(crossContextTarget, &notCrossContextTarget)))
    {
      return false;
    }

    *info = nullptr;
    if (!notCrossContextTarget)
    {
      CrossContextInfo * crossContextInfo = GetCrossContextInfo(crossContextTarget);
      if (crossContextInfo == nullptr)
      {
        return false;
      }
      *info = crossContextInfo;
    }

    return true;
  }

  static JsValueRef MarshalDescriptor(JsValueRef descriptor, ContextShim * fromContextShim, ContextShim * toContextShim)
  {
    JsValueType valueType;
    if (CheckMarshalFailed(JsGetValueType(descriptor, &valueType)))
    {
      return JS_INVALID_REFERENCE;
    }

    // Non object can't be marshaled as a descriptor (as it doesn't have properties)
    switch (valueType)
    {
    case JsUndefined:
    case JsNull:
    case JsNumber:
    case JsString:
    case JsBoolean:
    case JsSymbol:
      return MarshalJsValueRefToContext(descriptor, fromContextShim, toContextShim);
    };

    JsValueRef toContextDescriptor;
    {
      ContextShim::Scope scope(toContextShim);
      if (CheckMarshalFailed(JsCreateObject(&toContextDescriptor)))
      {
        return JS_INVALID_REFERENCE;
      }
    }

    auto fn = [=](JsPropertyIdRef propertyIdRef)
    {
      bool hasValue;
      if (CheckMarshalFailed(JsHasProperty(descriptor, propertyIdRef, &hasValue)))
      {
        return false;
      }

      if (hasValue)
      {
        JsValueRef valueRef;
        if (CheckMarshalFailed(JsGetProperty(descriptor, propertyIdRef, &valueRef)))
        {
          return false;
        }
        JsValueRef toContextValueRef = MarshalJsValueRefToContext(valueRef, fromContextShim, toContextShim);
        if (toContextValueRef == JS_INVALID_REFERENCE)
        {
          return false;
        }
        {
          ContextShim::Scope scope(toContextShim);
          if (CheckMarshalFailed(JsSetProperty(toContextDescriptor, propertyIdRef, toContextValueRef, false)))
          {
            return false;
          }
        }
      }
      return true;
    };
    IsolateShim * isolateShim = toContextShim->GetIsolateShim();

    if (!fn(isolateShim->GetCachedPropertyIdRef(CachedPropertyIdRef::writable))
      || !fn(isolateShim->GetCachedPropertyIdRef(CachedPropertyIdRef::configurable))
      || !fn(isolateShim->GetCachedPropertyIdRef(CachedPropertyIdRef::enumerable))
      || !fn(isolateShim->GetCachedPropertyIdRef(CachedPropertyIdRef::get))
      || !fn(isolateShim->GetCachedPropertyIdRef(CachedPropertyIdRef::set))
      || !fn(isolateShim->GetCachedPropertyIdRef(CachedPropertyIdRef::value))
      )
    {
      return JS_INVALID_REFERENCE;
    }
    return toContextDescriptor;
  }

  static JsValueRef MarshalOwnKeysTrapResult(JsValueRef result, ContextShim * fromContextShim, ContextShim * toContextShim)
  {
    //CHAKRA-TODO: Remove this once Chakra allow returning a proxy from the own key trap
    IsolateShim * isolateShim = toContextShim->GetIsolateShim();
    unsigned int length;
    if ((GetArrayLength(result, &length) != JsNoError) || length == 0)
    {
      return MarshalJsValueRefToContext(result, fromContextShim, toContextShim);
    }
    JsValueRef currentContextResultArray;
    {
      ContextShim::Scope scope(toContextShim);
      if (CheckMarshalFailed(JsCreateArray(length, &currentContextResultArray)))
      {
        return JS_INVALID_REFERENCE;
      }
    }
    for (unsigned int i = 0; i < length; i++)
    {
      JsValueRef indexValueRef;
      if (CheckMarshalFailed(JsIntToNumber(i, &indexValueRef)))
      {
        return JS_INVALID_REFERENCE;
      }
      JsValueRef value;
      if (CheckMarshalFailed(JsGetIndexedProperty(result, indexValueRef, &value)))
      {
        return JS_INVALID_REFERENCE;
      }

      JsValueRef currentContextValue = MarshalJsValueRefToContext(value, fromContextShim, toContextShim);
      if (currentContextValue == JS_INVALID_REFERENCE)
      {
        return JS_INVALID_REFERENCE;
      }
      {
        ContextShim::Scope scope(toContextShim);
        JsValueRef currentContextIndexValueRef;
        if (CheckMarshalFailed(JsIntToNumber(i, &currentContextIndexValueRef)))
        {
          return JS_INVALID_REFERENCE;
        }
        if (CheckMarshalFailed(JsSetIndexedProperty(currentContextResultArray, currentContextIndexValueRef, currentContextValue)))
        {
          return JS_INVALID_REFERENCE;
        }
      }
    }
    return currentContextResultArray;
  }

  static bool CheckNewNonConfigurableProperty(ContextShim *contextShim,
    JsValueRef prop, JsValueRef descriptor, JsValueRef fakeTargetObject, CrossContextInfo * crossContextInfo)
  {
    IsolateShim * isolateShim = contextShim->GetIsolateShim();
    bool hasProperty;
    JsPropertyIdRef configurablePropertyIdRef = isolateShim->GetCachedPropertyIdRef(CachedPropertyIdRef::configurable);
    if (CheckMarshalFailed(JsHasProperty(descriptor, configurablePropertyIdRef, &hasProperty)))
    {
      return false;
    }
    if (!hasProperty)
    {
      return true;
    }

    JsValueRef configruableValue;
    if (CheckMarshalFailed(JsGetProperty(descriptor, configurablePropertyIdRef, &configruableValue)))
    {
      return false;
    }
    bool configurable;
    if (CheckMarshalFailed(JsBooleanToBool(configruableValue, &configurable)))
    {
      return false;
    }

    if (configurable)
    {
      return true;
    }

    bool hasPropertyOnFakeTarget;
    if (CheckMarshalFailed(JsHasProperty(fakeTargetObject, prop, &hasPropertyOnFakeTarget)))
    {
      return false;
    }

    if (hasPropertyOnFakeTarget)
    {
      // The fake target has the property, and the only reason is that it was non-configurable before anyway
      // So we don't have to define it again
      return true;
    }

    // We need to define the property on the fakeTargetObject, need to figure out whether it is data or accessor

    // Get the property's id ref
    JsPropertyIdRef propertyIdRef = JS_INVALID_REFERENCE;
    if (CheckMarshalFailed(GetPropertyIdFromValue(prop, &propertyIdRef)))
    {
      return false;
    }

    JsValueRef newDescriptor;
    if (CheckMarshalFailed(JsCreateObject(&newDescriptor)))
    {
      return false;
    }

    if (CheckMarshalFailed(JsSetProperty(newDescriptor, configurablePropertyIdRef, configruableValue, false)))
    {
      return false;
    }

    JsPropertyIdRef valuePropertyIdRef = isolateShim->GetCachedPropertyIdRef(CachedPropertyIdRef::value);
    JsPropertyIdRef getPropertyIdRef = isolateShim->GetCachedPropertyIdRef(CachedPropertyIdRef::get);

    bool isDataDescriptor;
    if (CheckMarshalFailed(JsHasProperty(descriptor, valuePropertyIdRef, &isDataDescriptor)))
    {
      return false;
    }

    if (!isDataDescriptor)
    {
      bool isAccessorDescriptor;
      if (CheckMarshalFailed(JsHasProperty(descriptor, getPropertyIdRef, &isAccessorDescriptor)))
      {
        return false;
      }
      if (!isAccessorDescriptor)
      {
        if (CheckMarshalFailed(JsHasProperty(descriptor, isolateShim->GetCachedPropertyIdRef(CachedPropertyIdRef::set), &isAccessorDescriptor)))
        {
          return false;
        }


        if (!isAccessorDescriptor)
        {
          // A generic descriptor, we need to see what kind of property this is on the actual cross context target
          ContextShim::Scope scope(crossContextInfo->contextShim);
          JsValueRef crossContextDescriptor;
          if (CheckMarshalFailed(JsGetOwnPropertyDescriptor(crossContextInfo->crossContextObject, propertyIdRef, &crossContextDescriptor)))
          {
            return false;
          }

          if (CheckMarshalFailed(JsHasProperty(crossContextDescriptor, valuePropertyIdRef, &isDataDescriptor)))
          {
            return false;
          }
        }
      }
    }

    if (CheckMarshalFailed(JsSetProperty(newDescriptor, isDataDescriptor ? valuePropertyIdRef : getPropertyIdRef, contextShim->GetUndefined(), false)))
    {
      return false;
    }

    bool defined;
    if (CheckMarshalFailed(JsDefineProperty(fakeTargetObject, propertyIdRef, newDescriptor, &defined)))
    {
      return false;
    }

    if (!defined)
    {
      // CHAKRA-TODO: Fail fast?
      assert(false);
      return false;
    }
    return true;
  }

  template <ProxyTraps trap, unsigned short reflectArgumentCount>
  static JsValueRef CALLBACK CrossContextCallback(_In_ JsValueRef callee, _In_ bool isConstructCall, _In_ JsValueRef *arguments, _In_ unsigned short argumentCount, _In_opt_ void *callbackState)
  {
    ContextShim * currentContextShim = ContextShim::GetCurrent();
    JsValueRef fakeTargetObject = arguments[1];
    if (trap == ProxyTraps::GetTrap)
    {
      // Check for unwarping
      JsValueRef prop = arguments[2];
      JsValueType type;
      JsPropertyIdRef propertyIdRef;
      if (JsGetValueType(prop, &type) == JsNoError
        && type == JsSymbol
        && JsGetPropertyIdFromSymbol(prop, &propertyIdRef) == JsNoError
        && propertyIdRef == currentContextShim->GetIsolateShim()->GetCrossContextTargetSymbolPropertyIdRef())
      {
        return fakeTargetObject;
      }
    }

    CrossContextInfo * crossContextInfo = GetCrossContextInfo(fakeTargetObject);
    if (crossContextInfo == nullptr)
    {
      return JS_INVALID_REFERENCE;
    }

    ContextShim * targetContextShim = crossContextInfo->contextShim;
    JsValueRef reflect_arguments[reflectArgumentCount + 1];    // plus 1 for the this pointer
    reflect_arguments[0] = targetContextShim->GetReflectObject();
    reflect_arguments[1] = crossContextInfo->crossContextObject;  // target

    unsigned short actualArgumentCount = min(reflectArgumentCount + 1, argumentCount);
    for (unsigned short i = 2; i < actualArgumentCount; i++)
    {
      JsValueRef marshalledArgument;
      if (trap == ProxyTraps::DefinePropertyTrap && i == 3)
      {
        // CHAKRA-TODO: Remove this once Chakra works with proxy as property descriptor.
        // Manual deep copy marshalling the relevent bits for now.
        marshalledArgument = MarshalDescriptor(arguments[i], currentContextShim, targetContextShim);
      }
      else
      {
        marshalledArgument = MarshalJsValueRefToContext(arguments[i], currentContextShim, targetContextShim);
      }
      if (marshalledArgument == JS_INVALID_REFERENCE)
      {
        return marshalledArgument;
      }
      reflect_arguments[i] = marshalledArgument;
    }

    JsValueRef returnValue;
    {
      // Switch to the object's context and get the property

      ContextShim::Scope scope(targetContextShim);
      JsValueRef result;
      if (CheckMarshalFailed(JsCallFunction(targetContextShim->GetReflectFunctionForTrap(trap), reflect_arguments, actualArgumentCount, &result), true))
      {
        return JS_INVALID_REFERENCE;
      }

      if (trap == ProxyTraps::OwnKeysTrap)
      {
        // CHAKRA-TODO: Remove this once Chakra allow returning a proxy from the own key trap
        // Manual deep copy marshalling the relevent bits for now.
        return MarshalOwnKeysTrapResult(result, targetContextShim, currentContextShim);
      }
      if (trap == ProxyTraps::GetOwnPropertyDescriptorTrap)
      {
        // CHAKRA-TODO: Remove this once Chakra works with proxy as property descriptor.
        // Manual deep copy marshalling the relevent bits for now.
        return MarshalDescriptor(result, targetContextShim, currentContextShim);
      }
      // Marshal the property value to the current context
      returnValue =  MarshalJsValueRefToContext(result, targetContextShim, currentContextShim);
    }

    if (trap == ProxyTraps::DefinePropertyTrap)
    {
      // For define property trap, if we are defining a non configurable property, we need to have a corresponing
      // in the fake proxy target object that we have.
      if (!CheckNewNonConfigurableProperty(currentContextShim, arguments[2], arguments[3], fakeTargetObject, crossContextInfo))
      {
        return JS_INVALID_REFERENCE;
      }
    }

    return returnValue;
  }


  static void CALLBACK CrossContextInfoFinalizeCallback(void * data)
  {
    delete (CrossContextInfo *)data;
  }

  static JsValueRef CALLBACK DummyCallback(_In_ JsValueRef callee, _In_ bool isConstructCall, _In_ JsValueRef *arguments, _In_ unsigned short argumentCount, _In_opt_ void *callbackState)
  {
    return GetUndefined();
  }

  static JsValueRef MarshalObjectToContext(JsValueType valueType, JsValueRef valueRef, ContextShim * contextShim, ContextShim * toContextShim)
  {
    JsValueRef crossContextObject = valueRef;
    JsValueType crossContextObjectValueType = valueType;
    ContextShim * fromContextShim = contextShim;

    if (valueType == JsObject)
    {
      // Proxy's value type is JsObject
      // Unwrap the object if it is already cross site.
      CrossContextInfo * crossContextInfo;
      if (!UnwrapIfCrossContext(valueRef, &crossContextInfo))
      {
        return JS_INVALID_REFERENCE;
      }

      if (crossContextInfo != nullptr)
      {
        fromContextShim = crossContextInfo->contextShim;
        crossContextObject = crossContextInfo->crossContextObject;
        if (fromContextShim == toContextShim)
        {
          return crossContextObject;
        }
      }

      {
        ContextShim::Scope scope(fromContextShim);
        if (CheckMarshalFailed(JsGetValueType(crossContextObject, &crossContextObjectValueType)))
        {
          return JS_INVALID_REFERENCE;
        }
      }
    }

    // If the cross site object is a function, we need the target object to be "callable" to allow
    // apply trap to work, but the callable function can't have a non-configuable prototype, where
    // proxy will complain about us hiding the non-configurable prototype if the real cross context
    // object doesn't have it.a lamda function fits the bill here.

    // Here is the object tree, all in the context of "toContextShim"
    // If crossContextObject is an object:
    // Proxy
    //  |- targetObject (external object)
    //      |- crossContextObject (external data)         -- keep the object alive
    //      |- externalobject (property: keepAliveContext)
    //          |- context                                -- keep the context alive
    //      |- externalobject (property: crossContextInfo)
    //          |- CrossContextInfo (external data)       -- to hold information about the crossContextObject's context
    //              |- context
    //              |- crossContextObject -- again here for convenience (instead of going thru targetObject.keepAlive and get it's external data
    // If crossContextObject is a function:
    // Proxy
    //  |- targetObject (empty lambda function)
    //      |- externalobject (property: keepAlive)
    ///         |- crossContextObject (external data)     -- keep object alive
    //      |- externalobject (property: keepAliveContext)
    //          |- context                                -- keep the context alive
    //      |- externalobject (property: crossContextInfo)
    //          |- CrossContextInfo (external data)       -- to hold information about the crossContextObject's context
    //              |- context
    //              |- crossContextObject -- again here for convenience (instead of going thru targetObject.keepAlive and get it's external data

    ContextShim::Scope scope(toContextShim);

    JsNativeFunction proxyConf[ProxyTraps::TrapCount] = {};
    proxyConf[ProxyTraps::ApplyTrap] = CrossContextCallback<ProxyTraps::ApplyTrap, 3>;
    proxyConf[ProxyTraps::ConstructTrap] = CrossContextCallback<ProxyTraps::ConstructTrap, 2>;
    proxyConf[ProxyTraps::DefinePropertyTrap] = CrossContextCallback<ProxyTraps::DefinePropertyTrap, 3>;
    proxyConf[ProxyTraps::DeletePropertyTrap] = CrossContextCallback<ProxyTraps::DeletePropertyTrap, 2>;
    proxyConf[ProxyTraps::EnumerateTrap] = CrossContextCallback<ProxyTraps::EnumerateTrap, 1>;
    proxyConf[ProxyTraps::GetTrap] = CrossContextCallback<ProxyTraps::GetTrap, 3>;
    proxyConf[ProxyTraps::GetOwnPropertyDescriptorTrap] = CrossContextCallback<ProxyTraps::GetOwnPropertyDescriptorTrap, 2>;
    proxyConf[ProxyTraps::GetPrototypeOfTrap] = CrossContextCallback<ProxyTraps::GetPrototypeOfTrap, 1>;
    proxyConf[ProxyTraps::HasTrap] = CrossContextCallback<ProxyTraps::HasTrap, 2>;
    proxyConf[ProxyTraps::IsExtensibleTrap] = CrossContextCallback<ProxyTraps::IsExtensibleTrap, 1>;
    proxyConf[ProxyTraps::OwnKeysTrap] = CrossContextCallback<ProxyTraps::OwnKeysTrap, 1>;
    proxyConf[ProxyTraps::PreventExtensionsTrap] = CrossContextCallback<ProxyTraps::PreventExtensionsTrap, 1>;
    proxyConf[ProxyTraps::SetTrap] = CrossContextCallback<ProxyTraps::SetTrap, 4>;
    proxyConf[ProxyTraps::SetPrototypeOfTrap] = CrossContextCallback<ProxyTraps::SetPrototypeOfTrap, 2>;

    IsolateShim * isolateShim = toContextShim->GetIsolateShim();
    JsValueRef targetObject;
    if (valueType == JsFunction || valueType == JsArray)
    {
      if (valueType == JsFunction)
      {
        // Use a function as the target object so that the proxy can be called
        if (CheckMarshalFailed(JsCallFunction(toContextShim->GetCreateEmptyLambdaFunction(), nullptr, 0, &targetObject)))
        {
          return JS_INVALID_REFERENCE;
        }
      }
      else
      {
        // valueType == JsArray
        //Use an array as the target object so that Array.isArray will return the right result.
        if (CheckMarshalFailed(JsCreateArray(0, &targetObject)))
        {
          return JS_INVALID_REFERENCE;
        }
      }

      JsValueRef keepAliveObject;
      if (CheckMarshalFailed(JsCreateFunction(DummyCallback, crossContextObject, &keepAliveObject)))
      {
        return JS_INVALID_REFERENCE;
      }
      if (CheckMarshalFailed(JsSetProperty(targetObject, isolateShim->GetCachedPropertyIdRef(CachedPropertyIdRef::crossContextKeepAliveObject), keepAliveObject, false)))
      {
        return JS_INVALID_REFERENCE;
      }
    }
    else
    {
      if (CheckMarshalFailed(JsCreateExternalObject(crossContextObject, nullptr, &targetObject)))
      {
        return JS_INVALID_REFERENCE;
      }
    }

    JsValueRef keepAliveContext;
    if (CheckMarshalFailed(JsCreateExternalObject(fromContextShim->GetContextRef(), nullptr, &keepAliveContext)))
    {
      return JS_INVALID_REFERENCE;
    }

    if (CheckMarshalFailed(JsSetProperty(targetObject, isolateShim->GetCachedPropertyIdRef(CachedPropertyIdRef::crossContextKeepAliveContext), keepAliveContext, false)))
    {
      return JS_INVALID_REFERENCE;
    }

    CrossContextInfo * crossContextInfo = new CrossContextInfo();
    crossContextInfo->contextShim = fromContextShim;
    crossContextInfo->crossContextObject = crossContextObject;
    JsValueRef crossContextInfoObject;
    if (CheckMarshalFailed(JsCreateExternalObject(crossContextInfo, CrossContextInfoFinalizeCallback, &crossContextInfoObject)))
    {
      delete crossContextInfo;
      return JS_INVALID_REFERENCE;
    }

    if (CheckMarshalFailed(JsSetProperty(targetObject, isolateShim->GetCachedPropertyIdRef(CachedPropertyIdRef::crossContextInfoObject), crossContextInfoObject, false)))
    {
      return JS_INVALID_REFERENCE;
    }

    JsValueRef proxy;
    if (CheckMarshalFailed(CreateProxy(targetObject, proxyConf, &proxy)))
    {
      return JS_INVALID_REFERENCE;
    }
    return proxy;
  }

  JsValueRef MarshalJsValueRefToContext(JsValueRef valueRef, ContextShim * fromContextShim, ContextShim * toContextShim)
  {
    assert(fromContextShim == ContextShim::GetCurrent());
    assert(fromContextShim != toContextShim);

    JsValueType valueType;
    if (CheckMarshalFailed(JsGetValueType(valueRef, &valueType)))
    {
      return JS_INVALID_REFERENCE;
    }

    {
      switch (valueType)
      {
      case JsUndefined:
        return toContextShim->GetUndefined();
      case JsNull:
        return toContextShim->GetNull();
      case JsNumber:
      {
        double value;
        if (!CheckMarshalFailed(JsNumberToDouble(valueRef, &value)))
        {
          ContextShim::Scope scope(toContextShim);
          JsValueRef returnValueRef;
          if (!CheckMarshalFailed(JsDoubleToNumber(value, &returnValueRef)))
          {
            return returnValueRef;
          }
        }
        return JS_INVALID_REFERENCE;
      }
      case JsString:
      {
        wchar_t const * str;
        size_t len;
        if (!CheckMarshalFailed(JsStringToPointer(valueRef, &str, &len)))
        {
          ContextShim::Scope scope(toContextShim);
          JsValueRef returnValueRef;
          if (!CheckMarshalFailed(JsPointerToString(str, len, &returnValueRef)))
          {
            return returnValueRef;
          }
        }
        return JS_INVALID_REFERENCE;
      }
      case JsBoolean:
        return valueRef == fromContextShim->GetTrue() ? toContextShim->GetTrue() : toContextShim->GetFalse();
      case JsSymbol:
      {
        JsPropertyIdRef propertyId;
        if (!CheckMarshalFailed(JsGetPropertyIdFromSymbol(valueRef, &propertyId)))
        {
          ContextShim::Scope scope(toContextShim);
          JsValueRef returnValueRef;
          if (!CheckMarshalFailed(JsGetSymbolFromPropertyId(propertyId, &returnValueRef)))
          {
            return returnValueRef;
          }
        }
        return JS_INVALID_REFERENCE;
      }
      default:
        return MarshalObjectToContext(valueType, valueRef, fromContextShim, toContextShim);
      };
    }
  }

  JsValueRef CALLBACK ObjectPrototypeToStringCrossContextShim(JsValueRef callee, bool isConstructCall, JsValueRef *arguments, unsigned short argumentCount, void *callbackState)
  {
    if (argumentCount >= 1)
    {
      JsValueRef arg = arguments[0];

      if (arg == nullptr)
      {
        if (CheckMarshalFailed(JsGetGlobalObject(&arg)))
        {
          return JS_INVALID_REFERENCE;
        }
      }

      JsValueType valueType;
      if (CheckMarshalFailed(JsGetValueType(arg, &valueType)))
      {
        return JS_INVALID_REFERENCE;
      }

      // Non object can't be marshaled as a descriptor (as it doesn't have properties)
      switch (valueType)
      {
      case JsUndefined:
      case JsNull:
      case JsNumber:
      case JsString:
      case JsBoolean:
      case JsSymbol:
        break;
      default:
        {
          ContextShim * originalContextShim = ContextShim::GetCurrent();
          CrossContextInfo * crossContextInfo;
          if (!UnwrapIfCrossContext(arg, &crossContextInfo))
          {
            return JS_INVALID_REFERENCE;
          }
          if (crossContextInfo != nullptr)
          {
            ContextShim::Scope scope(crossContextInfo->contextShim);

            JsValueRef result;
            JsValueRef newArguments[] = { crossContextInfo->crossContextObject };
            if (JsCallFunction(crossContextInfo->contextShim->GetObjectPrototypeToStringFunction(),
              newArguments, _countof(newArguments), &result) != JsNoError)
            {
              return JS_INVALID_REFERENCE;
            }
            return MarshalJsValueRefToContext(result, crossContextInfo->contextShim, originalContextShim);
          }
        }
      };
    }
    JsValueRef result;
    if (JsCallFunction(ContextShim::GetCurrent()->GetObjectPrototypeToStringFunction(),
      arguments, argumentCount, &result) != JsNoError)
    {
      return JS_INVALID_REFERENCE;
    }
    return result;
  }
}
