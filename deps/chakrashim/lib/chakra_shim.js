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

(function() {
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
      Object.defineProperty(err, 'stack', {
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

  function patchTypedArrays() {
    ArrayBuffer.prototype.slice = function(begin, end) {
      if (!begin) {
        throw new Error('Wrong number of arguments.');
      }

      // boundary/arguments check - we do exactly the same ones as in node's
      // ArrayBuffer

      if (!end) {
        end = this.byteLength;
      }

      if (begin < 0) {
        begin = this.byteLength + begin;
      }

      if (begin < 0) {
        begin = 0;
      }

      if (begin > this.byteLength) {
        begin = this.byteLength;
      }

      if (end < 0) {
        end = this.byteLength + end;
      }

      if (end < 0) {
        end = 0;
      }

      if (end > this.byteLength) {
        end = this.byteLength;
      }

      if (begin > end) {
        begin = end;
      }

      var slice_length = end - begin;
      var slicedBuffer = new ArrayBuffer(slice_length);

      var sourceDataView = new DataView(this);
      var destDataView = new DataView(slicedBuffer);

      for (var i = 0; i < slice_length; i++) {
        destDataView.setInt8(i, sourceDataView.getInt8(begin + i));
      }

      return slicedBuffer;
    };

    // patch slice method for each type:
    [Int8Array, Uint8Array, Uint8ClampedArray, Int16Array,
      Uint16Array, Int32Array, Uint32Array, Float32Array,
      Float64Array].forEach(function(item) {
        item.prototype.slice = item.prototype.subarray;
        item.prototype.get = function(i) { return this[i]; };
        item.prototype.set = function(first, second) {
          if (first instanceof Array) {
            // in this case - first is an array of values, second is the offset
            for (var j = 0; j < first.length; j++) {
              this[j + second] = first[j];
            }
          }
          else {
            this[first] = second;
          }
        }
      });
  }

  function patchJsonParse() {
    var JsonParseOriginal = JSON.parse;

    var JsonParse = function() {
      return JsonParseOriginal.apply(this, arguments);
    }

    JSON.parse = JsonParse;
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
    utils.cloneObject = function(source, target) {
      Object.getOwnPropertyNames(source).forEach(function(key) {
        try {
          var desc = Object.getOwnPropertyDescriptor(source, key);
          if (desc.value === source) desc.value = target;
          Object.defineProperty(target, key, desc);
        } catch (e) {
          // Catch sealed properties errors
        }
      });
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
      Object.keys(obj).forEach(function(item) {
        if (!isUint(item))
          props.push(item);
      });
      return props;
    };
    utils.getIndexedOwnKeys = function(obj) {
      var props = [];
      Object.keys(obj).forEach(function(item) {
        if (isUint(item))
          props.push(item);
      });
      return props;
    };
    utils.createEmptyLambdaFunction = function() {
      return () => {};
    };
  }

  // patch console
  patchErrorStack();
  patchTypedArrays();
  patchJsonParse();

  // this is the keepAlive object that we will put some utilities function on
  patchUtils(this);
})
