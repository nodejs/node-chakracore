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

/* eslint-disable strict */
(function(keepAlive) {
  // Save original builtIns
  var
    Function_prototype_toString = Function.prototype.toString,
    Object_defineProperty = Object.defineProperty,
    Object_getOwnPropertyDescriptor = Object.getOwnPropertyDescriptor,
    Object_getOwnPropertyNames = Object.getOwnPropertyNames,
    Object_keys = Object.keys,
    Object_prototype_toString = Object.prototype.toString,
    Object_setPrototypeOf = Object.setPrototypeOf,
    Reflect_apply = Reflect.apply,
    Reflect_construct = Reflect.construct,
    Map_keys = Map.prototype.keys,
    Map_values = Map.prototype.values,
    Map_entries = Map.prototype.entries,
    Set_entries = Set.prototype.entries,
    Set_values = Set.prototype.values,
    Symbol_keyFor = Symbol.keyFor,
    Symbol_for = Symbol.for,
    Global_ParseInt = parseInt,
    JSON_parse = JSON.parse,
    JSON_stringify = JSON.stringify;
  var BuiltInError = Error;
  var global = this;

  // Simulate V8 JavaScript stack trace API
  function StackFrame(func, funcName, fileName, lineNumber, columnNumber) {
    this.column = columnNumber;
    this.lineNumber = lineNumber;
    this.scriptName = fileName;
    this.functionName = funcName;
    this.function = func;
  }

  StackFrame.prototype.getFunction = function() {
    // TODO: Fix if .stack is called from different callsite
    // from where Error() or Error.captureStackTrace was called
    return this.function;
  };

  StackFrame.prototype.getTypeName = function() {
    //TODO : Fix this
    return this.functionName;
  };

  StackFrame.prototype.getMethodName = function() {
    return this.functionName;
  };

  StackFrame.prototype.getFunctionName = function() {
    return this.functionName;
  };

  StackFrame.prototype.getFileName = function() {
    return this.scriptName;
  };

  StackFrame.prototype.getLineNumber = function() {
    return this.lineNumber;
  };

  StackFrame.prototype.getColumnNumber = function() {
    return this.column;
  };

  StackFrame.prototype.isEval = function() {
    // TODO
    return false;
  };

  StackFrame.prototype.isToplevel = function() {
    // TODO
    return false;
  };

  StackFrame.prototype.isNative = function() {
    // TODO
    return false;
  };

  StackFrame.prototype.isConstructor = function() {
    // TODO
    return false;
  };

  StackFrame.prototype.toString = function() {
    return (this.functionName || 'Anonymous function') + ' (' +
      this.scriptName + ':' + this.lineNumber + ':' + this.column + ')';
  };

  // default StackTrace stringify function
  function prepareStackTrace(error, stack) {
    var stackString = (error.name || 'Error') + ': ' + (error.message || '');

    for (var i = 0; i < stack.length; i++) {
      stackString += '\n   at ' + stack[i].toString();
    }

    return stackString;
  }

  // Parse 'stack' string into StackTrace frames. Skip top 'skipDepth' frames,
  // and optionally skip top to 'startName' function frames.
  function parseStack(stack, skipDepth, startName) {
    var stackSplitter = /\)\s*at/;
    var reStackDetails = /\s(?:at\s)?(.*)\s\((.*)/;
    var fileDetailsSplitter = /:(\d+)/;

    var curr = parseStack;
    var splittedStack = stack.split(stackSplitter);
    var errstack = [];

    for (var i = 0; i < splittedStack.length; i++) {
      // parseStack has 1 frame lesser than skipDepth. So skip calling .caller
      // once. After that, continue calling .caller
      if (skipDepth !== 1 && curr) {
        try {
          curr = curr.caller;
        } catch (e) {
          curr = undefined;  // .caller might not be allowed in curr's context
        }
      }

      if (skipDepth-- > 0) {
        continue;
      }

      var func = curr;
      var stackDetails = reStackDetails.exec(splittedStack[i]);
      var funcName = stackDetails[1];

      if (startName) {
        if (funcName === startName) {
          startName = undefined;
        }
        continue;
      }
      if (funcName === 'Anonymous function') {
        funcName = null;
      }

      var fileDetails = stackDetails[2].split(fileDetailsSplitter);

      var fileName = fileDetails[0];
      var lineNumber = fileDetails[1] ? fileDetails[1] : 0;
      var columnNumber = fileDetails[3] ? fileDetails[3] : 0;

      errstack.push(new StackFrame(func, funcName, fileName, lineNumber,
                                   columnNumber));
    }
    return errstack;
  }

  function findFuncDepth(func) {
    if (!func) {
      return 0;
    }

    try {
      var curr = privateCaptureStackTrace.caller;
      var limit = Error.stackTraceLimit;
      var skipDepth = 0;
      while (curr) {
        skipDepth++;
        if (curr === func) {
          return skipDepth;
        }
        if (skipDepth > limit) {
          return 0;
        }
        curr = curr.caller;
      }
    } catch (e) {
      // Strict mode may throw on .caller. Will try to match by function name.
      return -1;
    }

    return 0;
  }

  function withStackTraceLimitOffset(offset, f) {
    var oldLimit = BuiltInError.stackTraceLimit;
    if (typeof oldLimit === 'number') {
      BuiltInError.stackTraceLimit = oldLimit + offset;
    }
    try {
      return f();
    } finally {
      BuiltInError.stackTraceLimit = oldLimit;
    }
  }

  function captureStackTrace(err, func) {
    // skip 3 frames: lambda, withStackTraceLimitOffset, this frame
    return privateCaptureStackTrace(
      err, func,
      withStackTraceLimitOffset(3, () => new BuiltInError()),
      3);
  }

  // private captureStackTrace implementation
  //  err, func -- args from Error.captureStackTrace
  //  e -- a new Error object which already captured stack
  //  skipDepth -- known number of top frames to be skipped
  function privateCaptureStackTrace(err, func, e, skipDepth) {
    var currentStack = e;
    var isPrepared = false;
    var oldStackDesc = Object_getOwnPropertyDescriptor(e, 'stack');

    var funcSkipDepth = findFuncDepth(func);
    var startFuncName = (func && funcSkipDepth < 0) ? func.name : undefined;
    skipDepth += Math.max(funcSkipDepth - 1, 0);

    var currentStackTrace;
    function ensureStackTrace() {
      if (!currentStackTrace) {
        currentStackTrace = parseStack(
          Reflect_apply(oldStackDesc.get, e, []) || '', // Call saved old getter
          skipDepth, startFuncName);
      }
      return currentStackTrace;
    }

    function stackGetter() {
      if (!isPrepared) {
        var prep = Error.prepareStackTrace || prepareStackTrace;
        stackSetter(prep(err, ensureStackTrace()));
      }
      return currentStack;
    }
    function stackSetter(value) {
      currentStack = value;
      isPrepared = true;
      // Notify original Error object of this setter call. Without knowing
      // this Chakra runtime would reset stack at throw time.
      Reflect_apply(oldStackDesc.set, e, [value]);
    }

    // To retain overriden stackAccessors below,notify Chakra runtime to not
    // reset stack for this error object.
    if (e !== err) {
      Reflect_apply(oldStackDesc.set, err, ['']);
    }

    Object_defineProperty(err, 'stack', {
      get: stackGetter, set: stackSetter, configurable: true, enumerable: false
    });

    return ensureStackTrace; // For chakrashim native to get stack frames
  }

  // patch Error types to hook with Error.captureStackTrace/prepareStackTrace
  function patchErrorTypes() {
    // save a map from wrapper to builtin native types
    var typeToNative = new Map();

    // patch all these builtin Error types
    [
      Error, EvalError, RangeError, ReferenceError, SyntaxError, TypeError,
      URIError
    ].forEach(function(type) {
      function newType() {
        var e = withStackTraceLimitOffset(
          3, () => Reflect_construct(type, arguments, new.target || newType));
        // skip 3 frames: lambda, withStackTraceLimitOffset, this frame
        privateCaptureStackTrace(e, undefined, e, 3);
        return e;
      }

      Object_defineProperty(newType, 'name', {
        value: type.name,
        writable: false, enumerable: false, configurable: true
      });
      newType.prototype = type.prototype;
      newType.prototype.constructor = newType;
      if (type !== BuiltInError) {
        Object_setPrototypeOf(newType, Error);
      }
      global[type.name] = newType;
      typeToNative.set(newType, type);
    });

    // Delegate Error.stackTraceLimit to builtin Error constructor
    Object_defineProperty(Error, 'stackTraceLimit', {
      enumerable: false,
      configurable: true,
      get: function() { return BuiltInError.stackTraceLimit; },
      set: function(value) { BuiltInError.stackTraceLimit = value; }
    });

    Function.prototype.toString = function toString() {
      return Reflect_apply(Function_prototype_toString,
                           typeToNative.get(this) || this, arguments);
    };
    typeToNative.set(Function.prototype.toString, Function_prototype_toString);
  }

  function patchErrorStack() {
    Error.captureStackTrace = captureStackTrace;
  }

  var mapIteratorProperty = 'MapIteratorIndicator';
  function patchMapIterator() {
    var originalMapMethods = [];
    originalMapMethods.push(['entries', Map_entries]);
    originalMapMethods.push(['values', Map_values]);
    originalMapMethods.push(['keys', Map_keys]);

    originalMapMethods.forEach(function(pair) {
      Map.prototype[pair[0]] = function() {
        var result = pair[1].apply(this);
        Object_defineProperty(
          result, mapIteratorProperty,
          { value: true, enumerable: false, writable: false });
        return result;
      };
    });
  }

  var setIteratorProperty = 'SetIteratorIndicator';
  function patchSetIterator() {
    var originalSetMethods = [];
    originalSetMethods.push(['entries', Set_entries]);
    originalSetMethods.push(['values', Set_values]);

    originalSetMethods.forEach(function(pair) {
      Set.prototype[pair[0]] = function() {
        var result = pair[1].apply(this);
        Object_defineProperty(
          result, setIteratorProperty,
          { value: true, enumerable: false, writable: false });
        return result;
      };
    });
  }

  // Ensure global Debug object if not already exists, and patch it.
  function ensureDebug(otherGlobal) {
    if (!global.Debug) {
      Object_defineProperty(global, 'Debug', {
        value: {}, enumerable: false, configurable: false, writable: false
      });
    }

    otherProcess = otherGlobal.process;
    patchDebug(global.Debug);
  }

  var otherProcess;

  function patchDebug(Debug) {
    if (!Debug || Debug.MakeMirror) {
      return;
    }

    class Mirror {
      constructor(type) {
        this.type_ = type;
      }
      type() {
        return this.type_;
      }
    }

    class ValueMirror extends Mirror {
      constructor(type, value) {
        super(type);
        this.value_ = value;
      }
      value() {
        return this.value_;
      }
    }

    class UndefinedMirror extends ValueMirror {
      constructor() {
        super('undefined', undefined);
      }
    }
    const undefinedMirror = new UndefinedMirror();

    class ObjectMirror extends ValueMirror {
      constructor(type, value) {
        super(type || 'object', value);
      }
    }

    class PromiseMirror extends ObjectMirror {
      constructor(value) {
        super('promise', value);
      }
      status() {
        return '<unknown>';
      }
      promiseValue() {
        return new ValueMirror('<unknown>', '<unknown>');
      }
    }

    const util = otherProcess.binding('util');

    Debug.MakeMirror = (value) => {
      if (util.isPromise(value)) {
        return new PromiseMirror(value);
      }

      // Not supporting other types
      return undefinedMirror;
    };
  }

  // Simulate v8 micro tasks queue
  var microTasks = [];

  function patchUtils(utils) {
    var isUintRegex = /^(0|[1-9]\d*)$/;

    function isUint(value) {
      var result = isUintRegex.test(value);
      isUintRegex.lastIndex = 0;
      return result;
    }
    utils.cloneObject = function(source, target) {
      Object_getOwnPropertyNames(source).forEach(function(key) {
        try {
          var desc = Object_getOwnPropertyDescriptor(source, key);
          if (desc.value === source) desc.value = target;
          Object_defineProperty(target, key, desc);
        } catch (e) {
          // Catch sealed properties errors
        }
      });
    };
    utils.getPropertyNames = function(a) {
      var names = [];
      for (var propertyName in a) {
        if (isUint(propertyName)) {
          names.push(Global_ParseInt(propertyName));
        } else {
          names.push(propertyName);
        }
      }
      return names;
    };
    utils.getEnumerableNamedProperties = function(obj) {
      var props = [];
      for (var key in obj) {
        if (!isUint(key))
          props.push(key);
      }
      return props;
    };
    utils.getEnumerableIndexedProperties = function(obj) {
      var props = [];
      for (var key in obj) {
        if (isUint(key))
          props.push(key);
      }
      return props;
    };
    utils.createEnumerationIterator = function(props) {
      var i = 0;
      return {
        next: function() {
          if (i === props.length)
            return { done: true };
          return { value: props[i++] };
        }
      };
    };
    utils.createPropertyDescriptorsEnumerationIterator = function(props) {
      var i = 0;
      return {
        next: function() {
          if (i === props.length) return { done: true };
          return { name: props[i++], enumerable: true };
        }
      };
    };
    utils.getNamedOwnKeys = function(obj) {
      var props = [];
      Object_keys(obj).forEach(function(item) {
        if (!isUint(item))
          props.push(item);
      });
      return props;
    };
    utils.getIndexedOwnKeys = function(obj) {
      var props = [];
      Object_keys(obj).forEach(function(item) {
        if (isUint(item))
          props.push(item);
      });
      return props;
    };
    utils.getStackTrace = function() {
      return captureStackTrace({}, undefined)();
    };
    utils.isMapIterator = function(value) {
      return value[mapIteratorProperty] === true;
    };
    utils.isSetIterator = function(value) {
      return value[setIteratorProperty] === true;
    };
    function compareType(o, expectedType) {
      return Object_prototype_toString.call(o) === '[object ' +
            expectedType + ']';
    }
    utils.isBooleanObject = function(obj) {
      return compareType(obj, 'Boolean');
    };
    utils.isDate = function(obj) {
      return compareType(obj, 'Date');
    };
    utils.isMap = function(obj) {
      return compareType(obj, 'Map');
    };
    utils.isNativeError = function(obj) {
      return compareType(obj, 'Error') ||
        obj instanceof Error ||
        obj instanceof EvalError ||
        obj instanceof RangeError ||
        obj instanceof ReferenceError ||
        obj instanceof SyntaxError ||
        obj instanceof TypeError ||
        obj instanceof URIError;
    };
    utils.isPromise = function(obj) {
      return compareType(obj, 'Object') && obj instanceof Promise;
    };
    utils.isRegExp = function(obj) {
      return compareType(obj, 'RegExp');
    };
    utils.isAsyncFunction = function(obj) {
      // CHAKRA-TODO
      return false;
    };
    utils.isSet = function(obj) {
      return compareType(obj, 'Set');
    };
    utils.isStringObject = function(obj) {
      return compareType(obj, 'String');
    };
    utils.isNumberObject = function(obj) {
      return compareType(obj, 'Number');
    };
    utils.isArgumentsObject = function(obj) {
      return compareType(obj, 'Arguments');
    };
    utils.isGeneratorObject = function(obj) {
      return compareType(obj, 'Generator');
    };
    utils.isWeakMap = function(obj) {
      return compareType(obj, 'WeakMap');
    };
    utils.isWeakSet = function(obj) {
      return compareType(obj, 'WeakSet');
    };
    utils.isSymbolObject = function(obj) {
      return compareType(obj, 'Symbol');
    };
    utils.isName = function(obj) {
      return compareType(obj, 'String') || compareType(obj, 'Symbol');
    };
    utils.getSymbolKeyFor = function(symbol) {
      return Symbol_keyFor(symbol);
    };
    utils.getSymbolFor = function(key) {
      return Symbol_for(key);
    };
    utils.jsonParse = function(text, reviver) {
      return JSON_parse(text, reviver);
    };
    utils.jsonStringify = function(value, replacer, space) {
      return JSON_stringify(value, replacer, space);
    };
    utils.ensureDebug = ensureDebug;
    utils.enqueueMicrotask = function(task) {
      microTasks.push(task);
    };
    utils.dequeueMicrotask = function(task) {
      return microTasks.shift();
    };
    utils.isProxy = function(value) {
      // CHAKRA-TODO: Need to add JSRT API to detect this
      return false;
    };
    utils.getPropertyAttributes = function(object, value) {
      var descriptor = Object_getOwnPropertyDescriptor(object, value);
      if (descriptor === undefined) {
        return -1;
      }

      var attributes = 0;
      // taken from v8.h. Update if this changes in future
      const ReadOnly = 1,
        DontEnum = 2,
        DontDelete = 4;

      if (!descriptor.writable) {
        attributes |= ReadOnly;
      }
      if (!descriptor.enumerable) {
        attributes |= DontEnum;
      }
      if (!descriptor.configurable) {
        attributes |= DontDelete;
      }
      return attributes;
    };
    utils.getOwnPropertyNames = function(obj) {
      var ownPropertyNames = Object_getOwnPropertyNames(obj);
      var i = 0;
      while (i < ownPropertyNames.length) {
        var item = ownPropertyNames[i];
        if (isUint(item)) {
          ownPropertyNames[i] = Global_ParseInt(item);
          i++;
          continue;
        }
        // As per spec, getOwnPropertyNames() first include
        // numeric properties followed by non-numeric
        break;
      }
      return ownPropertyNames;
    };
  }

  patchErrorTypes();
  patchErrorStack();
  patchMapIterator();
  patchSetIterator();

  patchUtils(keepAlive);
});
