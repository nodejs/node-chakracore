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

(function () {
  // Save original builtIns
  var Object_defineProperty = Object.defineProperty,
      Object_getOwnPropertyDescriptor = Object.getOwnPropertyDescriptor,
      Object_getOwnPropertyNames = Object.getOwnPropertyNames,
      Object_keys = Object.keys;

  function StackFrame(funcName, fileName, lineNumber, columnNumber) {
    this.funcName = funcName;
    this.fileName = fileName;
    this.lineNumber = lineNumber;
    this.columnNumber = columnNumber;
  }

  StackFrame.prototype.getFunctionName = function() {
    return this.funcName;
  };

  StackFrame.prototype.getFileName = function() {
    return this.fileName;
  };

  StackFrame.prototype.getLineNumber = function() {
    return this.lineNumber;
  };

  StackFrame.prototype.getColumnNumber = function() {
    return this.columnNumber;
  };

  StackFrame.prototype.isEval = function() {
    // TODO
    return false;
  };

  StackFrame.prototype.toString = function() {
    return (this.funcName ? this.funcName : 'Anonymous function') + ' (' +
      this.fileName + ':' + this.lineNumber + ':' + this.columnNumber + ')';
  };

  function patchErrorStack() {
    function prepareStackTrace(error, stack) {
      var stackString = (error.name ? error.name : 'Error') +
        ': ' + error.message;

      for (var i = 0; i < stack.length; i++) {
        stackString += '\n   at ' + stack[i].toString();
      }

      return stackString;
    }

    function parseStack(stack, skipDepth) {
      // remove the first line so this function won't be seen
      var splittedStack = stack.split('\n');
      splittedStack.splice(0, 2);
      var errstack = [];

      for (var i = skipDepth; i < splittedStack.length; i++) {
        var parens = /\(/.exec(splittedStack[i]);
        var funcName = splittedStack[i].substr(6, parens.index - 7);
        if (funcName === 'Anonymous function') {
          funcName = null;
        }

        var location = splittedStack[i].substr(parens.index + 1,
            splittedStack[i].length - parens.index - 2);

        var fileName = location;
        var lineNumber = 0;
        var columnNumber = 0;

        var colonPattern = /:[0-9]+/g;
        var firstColon = colonPattern.exec(location);
        if (firstColon) {
          fileName = location.substr(0, firstColon.index);

          var secondColon = colonPattern.exec(location);
          if (secondColon) {
            lineNumber = parseInt(location.substr(firstColon.index + 1,
                secondColon.index - firstColon.index - 1), 10);
            columnNumber = parseInt(location.substr(secondColon.index + 1,
                location.length - secondColon.index), 10);
          }
        }
        errstack.push(
            new StackFrame(funcName, fileName, lineNumber, columnNumber));
      }
      return errstack;
    }

    function findFuncDepth(func) {
      try {
        var curr = Error.captureStackTrace.caller;
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
        // Strict mode may throw on .caller
      }

      return 0;
    }

    Error.captureStackTrace = function(err, func) {
      var currentStack;
      try { throw new Error; } catch (e) { currentStack = e.stack; }
      var isPrepared = false;
      var skipDepth = func ? findFuncDepth(func) : 0;
      Object_defineProperty(err, 'stack', {
        get: function() {
          if (isPrepared) {
            return currentStack;
          }
          var errstack = parseStack(currentStack, skipDepth);
          if (Error.prepareStackTrace) {
            currentStack = Error.prepareStackTrace(err, errstack);
          } else {
            currentStack = prepareStackTrace(err, errstack);
          }
          isPrepared = true;
          return currentStack;
        },
        set: function(value) {
          currentStack = value;
          isPrepared = true;
        },
        configurable: true,
        enumerable: false
      });
    };
  }

  function cloneObject(source, target) {
    Object_getOwnPropertyNames(source).forEach(function(key) {
      try {
        var desc = Object_getOwnPropertyDescriptor(source, key);
        if (desc.value === source) desc.value = target;
        Object_defineProperty(target, key, desc);
      } catch (e) {
        // Catch sealed properties errors
      }
    });
  }

  // Chakra Error instances have some enumerable properties (error number and
  // stack), causing node formatting differences. Try make those properties
  // non-enumerable when creating Error instances.
  // NOTE: This doesn't work if Error is created in Chakra runtime.
  function patchErrorTypes() {
    [Error, EvalError, RangeError, ReferenceError, SyntaxError, TypeError,
      URIError
    ].forEach(function (builtInError) {
      var name = builtInError.name;
      this[name] = function () {
        var e = builtInError.apply(this, arguments);
        Object_keys(e).forEach(function (key) {
          Object_defineProperty(e, key, { enumerable: false });
        });
        return e;
      };
      cloneObject(builtInError, this[name]);
    });
  }

  function patchUtils(utils) {
    var isUintRegex = /^(0|[1-9]\\d*)$/;
    var isUint = function(value) {
      var result = isUintRegex.test(value);
      isUintRegex.lastIndex = 0;
      return result;
    };
    utils.isInstanceOf = function(a, b) {
      return (a instanceof b);
    };
    utils.cloneObject = cloneObject;
    utils.forEachNonConfigurableProperty = function (source, callback) {
      Object_getOwnPropertyNames(source).forEach(function (key) {
        var desc = Object_getOwnPropertyDescriptor(source, key);
        if (desc && !desc.configurable && !callback(key, desc)) {
          return false;
        }
      });

      return true;
    };
    utils.getPropertyNames = function(a) {
      var names = [];
      for(var propertyName in a) {
        names.push(propertyName);
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
            return { done: true }
          return { value : props[i++] };
        }
      };
    };
    utils.createPropertyDescriptorsEnumerationIterator = function(props) {
      var i = 0;
      return {
        next: function() {
          if (i === props.length) return { done: true }
          return { name : props[i++], enumerable : true };
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
    utils.isBound = function(func) {
      var desc = Object_getOwnPropertyDescriptor(func, 'caller');
      return !!(desc && desc.get);
    };
    utils.createEmptyLambdaFunction = function(isBound) {
      var func = () => {};
      // return a bound function if target is a bound function
      return isBound ? func.bind({}) : func;
    };
    utils.throwAccessorErrorFunction = (function () {
      var x = utils.createEmptyLambdaFunction(true);
      return Object_getOwnPropertyDescriptor(x, 'caller').get;
    })();
  }

  // patch console
  patchErrorTypes();
  patchErrorStack();

  // this is the keepAlive object that we will put some utilities function on
  patchUtils(this);
})
