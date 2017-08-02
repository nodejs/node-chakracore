// Copyright Microsoft. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the 'Software'), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and / or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.

/* eslint-disable strict */
(function(globalObject, keepAliveObject, traceDebugJson) {
  var Logger = (function() {
    var logTypes = {
      API: 1
    };

    var enabledLogType = [];
    enabledLogType[logTypes.API] = false; // For development purpose

    function logFunc(logType, argsArray) {
      if (enabledLogType[logType]) {
        var printStr = '';
        for (var index in argsArray) {
          var obj = argsArray[index];
          if (obj !== undefined) {
            printStr += (typeof obj !== 'string') ? JSON.stringify(obj) : obj;
            printStr += ': ';
          }
        }
        chakraDebug.log(printStr);
      }
    }

    return {
      LogAPI: function(msg, obj) {
        logFunc(logTypes.API, arguments);
      }
    };
  })();

  function CallHostFunctionNoLog(fn) {
    var args = [].slice.call(arguments, 1);
    var start = new Date();
    var result = fn.apply(undefined, args);
    var end = new Date();
    Logger.LogAPI(`${fn.name}(${args}):${end - start}`);
    return result;
  }

  function CallHostFunction(fn) {
    var args = [].slice.call(arguments, 1);
    var start = new Date();
    var result = fn.apply(undefined, args);
    var end = new Date();
    Logger.LogAPI(`${fn.name}(${args}):${end - start}`, result);
    return result;
  }

  if (!globalObject.Debug) {
    globalObject.Debug = {};
  }

  var Debug = globalObject.Debug;

  Debug.ScriptBreakPointType = {
    ScriptId: 0,
    ScriptName: 1,
    ScriptRegExp: 2
  };

  Debug.setListener = function(fn) { };

  Debug.setBreakPoint = function(func, line, column) {
    Logger.LogAPI(`Debug.setBreakPoint(${func},${line},${column})`);
    var funcInfo = CallHostFunction(chakraDebug.JsDiagGetFunctionPosition,
                                    func);
    var bpId = -1;

    if (funcInfo.scriptId >= 0) {
      var scriptObj = DebugManager.ScriptsManager.GetScript(funcInfo.scriptId);
      var bpLine = funcInfo.firstStatementLine + line;
      var bpColumn = funcInfo.firstStatementColumn + column;
      var v8Breakpoint = new V8Breakpoint(Debug.ScriptBreakPointType.ScriptId,
                                          funcInfo.scriptId, scriptObj, bpLine,
                                          bpColumn);
      if (v8Breakpoint.Set()) {
        bpId = v8Breakpoint.Id();
      }
    }

    return bpId;
  };

  var DebugManager = (function() {
    var _internalHandle = Number.MAX_SAFE_INTEGER;

    function GetNextInternalHandle() {
      return --_internalHandle;
    }

    var _scripts = [];
    var _scriptHandles = [];

    var ScriptsManager = {
      GetScripts: function() {
        var scriptsArray = CallHostFunctionNoLog(chakraDebug.JsDiagGetScripts);
        scriptsArray.forEach(function(element, index, array) {
          if (!_scripts[element.scriptId]) {
            var scriptHandle = GetNextInternalHandle();
            _scriptHandles[scriptHandle] = scriptHandle;
            _scripts[element.scriptId] = new V8Script(element, scriptHandle);
          }
        });
        return _scripts;
      },
      GetScript: function(scriptId) {
        if (!_scripts[scriptId]) {
          this.GetScripts();
        }
        return _scripts[scriptId];
      }
    };

    return {
      ScriptsManager: ScriptsManager
    };
  })();


  function V8Script(chakraScript, scriptHandle) {
    this.chakraScript = chakraScript;
    this.script = {};
    this.script.handle = scriptHandle;
    this.script.type = 'script';
    this.script.name = ('fileName' in chakraScript) ?
      chakraScript.fileName : chakraScript.scriptType;
    this.script.id = chakraScript.scriptId;
    this.script.lineOffset = 0;
    this.script.columnOffset = 0;
    this.script.lineCount = chakraScript.lineCount;
    this.script.sourceStart = '';
    this.script.sourceLength = chakraScript.sourceLength;
    this.script.scriptType = 2;
    this.script.compilationType = 0;
    this.script.text = `${this.script.name} (lines: ${this.script.lineCount})`;
  }

  V8Script.prototype.GetId = function() {
    return this.script.id;
  };

  V8Script.prototype.GetFileName = function() {
    return this.chakraScript.fileName;
  };

  V8Script.prototype.GetScriptName = function() {
    return this.name;
  };

  V8Script.prototype.GetHandle = function() {
    return this.script.handle;
  };

  V8Script.prototype.GetProtocolObject = function() {
    return this.script;
  };

  V8Script.prototype.IsSameFileName = function(fileName) {
    if (fileName && this.GetFileName() &&
      (this.GetFileName().toLowerCase() === fileName.toLowerCase())) {
      return true;
    }
    return false;
  };


  function V8Breakpoint(type, target, scriptObject, line, column) {
    this.type = type;
    this.target = target;
    this.scriptObject = scriptObject;
    this.line = line;
    this.column = column;
  }

  V8Breakpoint.prototype.GetTypeString = function() {
    if (this.type === Debug.ScriptBreakPointType.ScriptId) {
      return 'scriptId';
    } else if (this.type === Debug.ScriptBreakPointType.ScriptName) {
      return 'scriptName';
    } else if (this.type === Debug.ScriptBreakPointType.ScriptRegExp) {
      return 'scriptRegExp';
    }
  };

  V8Breakpoint.prototype.Set = function() {
    // {"breakpointId":1,"line":2,"column":4,"scriptId":1,"parentScriptId":2}
    var bpObject = CallHostFunction(chakraDebug.JsDiagSetBreakpoint,
                                    this.scriptObject.GetId(),
                                    this.line, this.column);
    if (!bpObject) {
      return false;
    }
    this.v8breakpoint = {};
    this.v8breakpoint.number = bpObject.breakpointId;
    this.v8breakpoint.line = bpObject.line;
    this.v8breakpoint.column = bpObject.column;
    this.v8breakpoint.groupId = null;
    this.v8breakpoint.hit_count = 0;
    this.v8breakpoint.active = true;
    this.v8breakpoint.condition = null;
    this.v8breakpoint.ignoreCount = 0;
    this.v8breakpoint.actual_locations = [];
    this.v8breakpoint.actual_locations[0] = {};
    this.v8breakpoint.actual_locations[0].line = bpObject.line;
    this.v8breakpoint.actual_locations[0].column = bpObject.column;
    this.v8breakpoint.actual_locations[0].script_id = bpObject.scriptId;
    this.v8breakpoint.type = this.GetTypeString();
    this.v8breakpoint.script_id = bpObject.scriptId;
    this.v8breakpoint.script_name = this.scriptObject.GetScriptName();

    if (this.type === Debug.ScriptBreakPointType.ScriptRegExp) {
      this.v8breakpoint.script_regexp = this.target;
    }

    return true;
  };

  V8Breakpoint.prototype.IncreaseHit = function() {
    this.v8breakpoint.hit_count++;
  };

  V8Breakpoint.prototype.Remove = function() {
    CallHostFunction(chakraDebug.JsDiagRemoveBreakpoint, this.Id());
  };

  V8Breakpoint.prototype.Id = function() {
    return this.v8breakpoint.number;
  };

  V8Breakpoint.prototype.line = function() {
    return this.bpObject.line;
  };

  V8Breakpoint.prototype.column = function() {
    return this.bpObject.column;
  };

  V8Breakpoint.prototype.GetProtocolObject = function() {
    return this.v8breakpoint;
  };

  V8Breakpoint.prototype.actual_locations = function() {
    return this.v8breakpoint.actual_locations;
  };

  var chakraDebug = {};

  keepAliveObject.chakraDebug = chakraDebug;

  return keepAliveObject.chakraDebug;
});
