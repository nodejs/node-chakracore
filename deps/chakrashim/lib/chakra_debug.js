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
  var globalExecutionState;
  var pendingMessages = [];

  var Logger = (function() {
    var logTypes = {
      API: 1,
      DebugJson: 2,
      Error: 3
    };

    var enabledLogType = [];
    enabledLogType[logTypes.API] = false; // For development purpose
    enabledLogType[logTypes.DebugJson] = traceDebugJson === true;
    enabledLogType[logTypes.Error] = true; // Always enabled

    function logFunc(logType, argsArray) {
      if (enabledLogType[logType]) {
        var printStr = '';
        for (var index in argsArray) {
          var obj = argsArray[index];
          if (obj != undefined) {
            printStr += (typeof obj != 'string') ? JSON.stringify(obj) : obj;
            printStr += ': ';
          }
        }
        chakraDebug.log(printStr);
      }
    }

    return {
      LogAPI: function(msg, obj) {
        logFunc(logTypes.API, arguments);
      },
      LogDebugJson: function(msg, obj) {
        logFunc(logTypes.DebugJson, arguments);
      },
      LogError: function(msg, obj) {
        logFunc(logTypes.Error, arguments);
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
                              funcInfo.scriptId, scriptObj, bpLine, bpColumn);
      if (v8Breakpoint.Set()) {
        bpId = v8Breakpoint.Id();
        DebugManager.BreakpointManager.AddBreakpoint(v8Breakpoint);
      }
    }

    return bpId;
  };

  function AddChildrens(obj) {
    if ('display' in obj) {
      if (!('value' in obj) || !isFinite(obj['value']) ||
        obj['value'] == null || obj['value'] == undefined) {
        obj['value'] = obj['display'];
      }
      if (!('text' in obj)) {
        obj['text'] = obj['display'];
      }
      if (!('className' in obj)) {
        obj['className'] = obj['type'];
      }
    }

    if ('className' in obj && obj['className'] == 'Object') {
      obj.constructorFunction = {
        'ref': globalExecutionState.GetObjectEvalHandle()
      };
      obj.protoObject = {
        'ref': globalExecutionState.GetObjectProtoEvalHandle()
      };
      obj.prototypeObject = {
        'ref': globalExecutionState.GetObjectPrototypeEvalHandle()
      };
    }

    var PROPERTY_ATTRIBUTE_HAVE_CHILDRENS = 0x1;
    //var PROPERTY_ATTRIBUTE_READ_ONLY_VALUE = 0x2;

    if (('propertyAttributes' in obj) &&
      ((obj['propertyAttributes'] & PROPERTY_ATTRIBUTE_HAVE_CHILDRENS) ==
      PROPERTY_ATTRIBUTE_HAVE_CHILDRENS)) {
      var objectHandle = obj['handle'];

      var childProperties = CallHostFunction(chakraDebug.JsDiagGetProperties,
        objectHandle, 0, 1000);

      var propertiesArray = [];
      ['properties', 'debuggerOnlyProperties'].forEach(
        function(propertyCategory) {
          childProperties[propertyCategory].map(function(property) {
            propertiesArray.push({
              'name': property['name'],
              'propertyType': 0,
              'ref': property['handle']
            });
          });
        });
      obj['properties'] = propertiesArray;
    }
  }

  var DebugManager = (function() {
    var _internalHandle = Number.MAX_SAFE_INTEGER;

    function GetNextInternalHandle() {
      return --_internalHandle;
    }

    var _nextResponseSeq = 0;

    function Response(request) {
      this.seq = _nextResponseSeq++;
      this.type = 'response';
      this.request_seq = request.seq;
      this.command = request.command;
      this.success = false;
      this.running = globalExecutionState ? false : true;
    }

    var _nextEventSeq = 0;

    function Event(eventName) {
      this.seq = _nextEventSeq++;
      this.type = 'event';
      this.event = eventName;
      this.success = true;
      this.running = globalExecutionState ? false : true;
    }

    var Utility = {
      MakeResponse: function(request) {
        return new Response(request);
      },
      MakeEvent: function(eventName) {
        return new Event(eventName);
      },
      CreateScopeAndRef: function(scopeType, propertiesArray, index,
                                    frameIndex) {
        var scopeRef = GetNextInternalHandle();
        var scopeAndRef = {};
        scopeAndRef.scope = {
          'type': scopeType,
          'index': index,
          'frameIndex': frameIndex,
          'object': {
            'ref': scopeRef
          }
        };
        var properties = [];
        propertiesArray.map(function(property) {
          properties.push({
            'name': property['name'],
            'propertyType': 0,
            'ref': property['handle']
          });
        });
        scopeAndRef.ref = {
          'handle': scopeRef,
          'type': 'object',
          'className': 'Object',
          'constructorFunction': {
            'ref': globalExecutionState.GetObjectEvalHandle()
          },
          'protoObject': {
            'ref': globalExecutionState.GetObjectProtoEvalHandle()
          },
          'prototypeObject': {
            'ref': globalExecutionState.GetObjectPrototypeEvalHandle()
          },
          'properties': properties
        };

        return scopeAndRef;
      }
    };

    var _breakpoints = [];
    var _pendingBreakpoints = [];

    var BreakpointManager = {
      GetBreakpoints: function() {
        var breakpoints = CallHostFunction(chakraDebug.JsDiagGetBreakpoints);
        var activeBreakpointList = [];
        breakpoints.forEach(function(element, index, array) {
          var bpId = element.breakpointId;
          activeBreakpointList[bpId] = _breakpoints[bpId];
        });
        _breakpoints = activeBreakpointList;
        return _breakpoints;
      },
      GetBreakpoint: function(id) {
        return _breakpoints[id];
      },
      AddBreakpoint: function(v8Breakpoint) {
        _breakpoints[v8Breakpoint.Id()] = v8Breakpoint;
      },
      AddPendingBreakpoint: function(request) {
        _pendingBreakpoints.push(request);
      },
      ProcessPendingBreakpoints: function() {
        var unResolvedBreakpoints = [];
        var pendingBreakpointRequest;
        while (pendingBreakpointRequest = _pendingBreakpoints.shift()) {
          Logger.LogDebugJson(
            'ProcessPendingBreakpoints pendingBreakpointRequest',
            pendingBreakpointRequest);
          try {
            var response = new DebugManager.Utility.MakeResponse(
                                                    pendingBreakpointRequest);
            var v8CommandProcessor = new V8CommandProcessor();
            v8CommandProcessor[pendingBreakpointRequest.command](
                                   pendingBreakpointRequest, response, false);
            if (response.success) {
              var responseJson = JSON.stringify(response);
              Logger.LogDebugJson('ProcessPendingBreakpoints responseJson',
                                  responseJson);
              chakraDebug.SendDelayedRespose(responseJson);
            } else {
              unResolvedBreakpoints.push(pendingBreakpointRequest);
            }
          } catch (ex) {
            Logger.LogError('ProcessPendingBreakpoints exception: ' +
                           ex.message,
                           ex.stack);
          }
        }
        Array.prototype.push.apply(_pendingBreakpoints, unResolvedBreakpoints);
      },
      RemoveBreakpoint: function(id) {
        var v8Breakpoint = _breakpoints[id];
        if (v8Breakpoint) {
          v8Breakpoint.Remove();
          delete _breakpoints[id];
        }
      }
    };

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
      },
      IsScriptHandle: function(handle) {
        return (_scriptHandles[handle]) ? true : false;
      },
      GetScriptFromHandle: function(handle) {
        for (var i = 0; i < _scripts.length; ++i) {
          if (_scripts[i] && _scripts[i].GetHandle() == handle) {
            return _scripts[i];
          }
        }
      },
      GetScriptFromFileName: function(fileName) {
        var scripts = this.GetScripts();
        for (var i = 0; i < scripts.length; ++i) {
          if (scripts[i] && scripts[i].IsSameFileName(fileName)) {
            return scripts[i];
          }
        }
      },
      GetScriptFromMatchingFileName: function(regexToMatch) {
        var scripts = this.GetScripts();
        for (var i = 0; i < scripts.length; ++i) {
          if (scripts[i] && scripts[i].GetFileName() &&
            scripts[i].GetFileName().match(regexToMatch)) {
            return scripts[i];
          }
        }
      }
    };

    return {
      Utility: Utility, BreakpointManager: BreakpointManager,
      ScriptsManager: ScriptsManager
    };
  })();


  var ExecutionStateType = {
    Break: 0,
    Async: 1
  };

  function ExecutionState(stateType, scriptId) {
    this.type = stateType;
    this.breakScriptId = scriptId;
    this.frames = undefined;
    this.objectEval = undefined;
    this.objectProtoEval = undefined;
    this.objectPrototypeEval = undefined;
  }

  ExecutionState.prototype.ChangeToBreak = function() {
    this.type = ExecutionStateType.Break;
  };

  ExecutionState.prototype.IsAsyncBreak = function() {
    return this.type == ExecutionStateType.Async;
  };

  ExecutionState.prototype.GetBreakScriptId = function() {
    return this.breakScriptId;
  };

  ExecutionState.prototype.GetFrames = function() {
    if (!this.frames || this.frames.length == 0) {
      var _frames = [];
      var stackTrace = CallHostFunction(chakraDebug.JsDiagGetStackTrace);
      stackTrace.forEach(function(element, index, array) {
        _frames.push(new V8Frame(element));
      });

      this.frames = _frames;
    }
    return this.frames;
  };

  ExecutionState.prototype.GetObjectEvalHandle = function() {
    if (!this.objectEval) {
      var objEval = CallHostFunction(chakraDebug.JsDiagEvaluate, 'Object', 0);
      if (!objEval[0]) {
        Logger.LogError('Evaluating Object failed');
      }
      this.objectEval = objEval[1];
    }
    return this.objectEval.handle;
  };

  ExecutionState.prototype.GetObjectProtoEvalHandle = function() {
    if (!this.objectProtoEval) {
      var objEval = CallHostFunction(chakraDebug.JsDiagEvaluate,
        'Object.__proto__', 0);
      if (!objEval[0]) {
        Logger.LogError('Evaluating Object.__proto__ failed');
      }
      this.objectProtoEval = objEval[1];
    }
    return this.objectProtoEval.handle;
  };

  ExecutionState.prototype.GetObjectPrototypeEvalHandle = function() {
    if (!this.objectPrototypeEval) {
      var objEval = CallHostFunction(chakraDebug.JsDiagEvaluate,
        'Object.prototype', 0);
      if (!objEval[0]) {
        Logger.LogError('Evaluating Object.prototype failed');
      }
      this.objectPrototypeEval = objEval[1];
    }
    return this.objectPrototypeEval.handle;
  };


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
      (this.GetFileName().toLowerCase() == fileName.toLowerCase())) {
      return true;
    }
    return false;
  };


  function V8Frame(stackTrace) {
    this.stackProperties = CallHostFunction(
                      chakraDebug.JsDiagGetStackProperties, stackTrace.index);

    var receiverRef = stackTrace.functionHandle;
    if ('thisObject' in this.stackProperties &&
      'handle' in this.stackProperties.thisObject) {
      receiverRef = this.stackProperties.thisObject.handle;
    }

    var isAtReturn = false;
    if ('returnValue' in this.stackProperties &&
      'handle' in this.stackProperties.returnValue) {
      isAtReturn = true;
    }

    this.frame = {};
    this.frame.type = 'frame';
    this.frame.index = stackTrace.index;
    this.frame.receiver = {};
    this.frame.receiver.ref = receiverRef;
    this.frame.func = {};
    this.frame.func.ref = stackTrace.functionHandle;
    this.frame.script = {};
    this.frame.script.ref = DebugManager.ScriptsManager.GetScript(
                                             stackTrace.scriptId).GetHandle();
    this.frame.constructCall = false;
    this.frame.atReturn = isAtReturn;
    this.frame.debuggerFrame = false;
    this.frame.arguments = [];
    this.frame.locals = [];
    this.frame.position = 0;
    this.frame.line = stackTrace.line;
    this.frame.column = stackTrace.column;
    this.frame.sourceLineText = stackTrace.sourceText;
    this.frame.scopes = [];
    this.frame.text = '';
  }

  V8Frame.prototype.GetStackProperties = function() {
    return this.stackProperties;
  };

  V8Frame.prototype.GetProtocolObject = function() {
    return this.frame;
  };

  V8Frame.prototype.GetIndex = function() {
    return this.frame.index;
  };

  V8Frame.prototype.Evaluate = function(expression) {
    return CallHostFunction(chakraDebug.JsDiagEvaluate, expression,
      this.frame.index);
  };


  function V8Breakpoint(type, target, scriptObject, line, column) {
    this.type = type;
    this.target = target;
    this.scriptObject = scriptObject;
    this.line = line;
    this.column = column;
  }

  V8Breakpoint.prototype.GetTypeString = function() {
    if (this.type == Debug.ScriptBreakPointType.ScriptId) {
      return 'scriptId';
    } else if (this.type == Debug.ScriptBreakPointType.ScriptName) {
      return 'scriptName';
    } else if (this.type == Debug.ScriptBreakPointType.ScriptRegExp) {
      return 'scriptRegExp';
    }
  };

  V8Breakpoint.prototype.Set = function() {
    // {"breakpointId":1,"line":2,"column":4,"scriptId":1,"parentScriptId":2}
    var bpObject = CallHostFunction(chakraDebug.JsDiagSetBreakpoint,
      this.scriptObject.GetId(), this.line, this.column);
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

    if (this.type == Debug.ScriptBreakPointType.ScriptRegExp) {
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


  function V8CommandProcessor() {}
  // Supported commands
  V8CommandProcessor.prototype.scripts = function(request, response) {
    var v8Scripts = DebugManager.ScriptsManager.GetScripts();

    if (request.arguments) {
      var ids;
      var filter;
      var includeSource = false;

      if (request.arguments.ids) {
        ids = request.arguments.ids;
      }

      if (request.arguments.includeSource) {
        includeSource = true;
      }

      if (request.arguments.filter) {
        filter = request.arguments.filter;
      }

      var body = [];
      v8Scripts.forEach(function(element, index, array) {
        var found = true;

        if (ids != null) {
          found = false;
          for (var i = 0; i < ids.length; ++i) {
            if (ids[i] == element.GetId()) {
              found = true;
              break;
            }
          }
        }

        if (filter != null) {
          found = filter == element.GetFileName();
        }

        if (found) {
          var scriptObj = element.GetProtocolObject();
          if (includeSource) {
            var chakraSourceObj = CallHostFunctionNoLog(
              chakraDebug.JsDiagGetSource, element.GetId());
            scriptObj.source = chakraSourceObj.source;
            scriptObj.lineCount = chakraSourceObj.lineCount;
            scriptObj.sourceLength = chakraSourceObj.sourceLength;
          }
          body.push(scriptObj);
        }
      });

      response.success = true;
      response.body = body;
    }
  };

  V8CommandProcessor.prototype.source = function(request, response) {
    var chakraSourceObj = CallHostFunctionNoLog(chakraDebug.JsDiagGetSource,
      globalExecutionState.GetBreakScriptId());
    response.success = true;
    response.body = {};
    response.body.source = chakraSourceObj.source;
    response.body.fromLine = 0;
    response.body.toLine = chakraSourceObj.lineCount;
    response.body.fromPosition = 0;
    response.body.toPosition = chakraSourceObj.sourceLength;
    response.body.totalLines = chakraSourceObj.lineCount;
  };

  V8CommandProcessor.prototype.continue = function(request, response) {
    var success = true;
    if (request.arguments && request.arguments.stepaction) {
      var jsDiagSetStepType = 0;
      if (request.arguments.stepaction == 'in') {
        /* JsDiagStepTypeStepIn */
        jsDiagSetStepType = 0;
      } else if (request.arguments.stepaction == 'out') {
        /* JsDiagStepTypeStepOut */
        jsDiagSetStepType = 1;
      } else if (request.arguments.stepaction == 'next') {
        /* JsDiagStepTypeStepOver */
        jsDiagSetStepType = 2;
      } else {
        throw new Error('Unhandled stepaction: ' +
          request.arguments.stepaction);
      }

      if (!CallHostFunction(chakraDebug.JsDiagSetStepType,
        jsDiagSetStepType)) {
        success = false;
      }
    }

    response.success = success;

    // We are continuing, delete the globalExecutionState
    globalExecutionState = undefined;
  };

  V8CommandProcessor.prototype.setbreakpoint = function(request, response,
    optAddToPending) {
    var breakpointType;
    var breakpointScript;
    var breakpointTarget;
    var line = 0;
    var column = 0;

    if (request.arguments) {
      var scriptObject;
      if (request.arguments.type == 'scriptRegExp') {
        var targetRegex = new RegExp(request.arguments.target);
        scriptObject =
          DebugManager.ScriptsManager.GetScriptFromMatchingFileName(
          targetRegex);
        if (scriptObject) {
          breakpointScript = scriptObject;
          breakpointType = Debug.ScriptBreakPointType.ScriptRegExp;
          breakpointTarget = request.arguments.target;
        }
      } else if (request.arguments.type == 'script' &&
        (request.arguments.target == null ||
        request.arguments.target == undefined)) {
        var breakScriptId = globalExecutionState.GetBreakScriptId();
        scriptObject = DebugManager.ScriptsManager.GetScript(breakScriptId);
        if (scriptObject) {
          breakpointScript = scriptObject;
          breakpointType = Debug.ScriptBreakPointType.ScriptId;
          breakpointTarget = request.arguments.target;
        }
      } else if (request.arguments.type == 'script' &&
        request.arguments.target) {
        scriptObject = DebugManager.ScriptsManager.GetScriptFromFileName(
          request.arguments.target);
        if (scriptObject) {
          breakpointScript = scriptObject;
          breakpointType = Debug.ScriptBreakPointType.ScriptName;
          breakpointTarget = request.arguments.target;
        }
      } else if (request.arguments.type == 'scriptId' &&
        request.arguments.target) {
        var scriptId = parseInt(request.arguments.target);
        scriptObject = DebugManager.ScriptsManager.GetScript(scriptId);
        if (scriptObject) {
          breakpointScript = scriptObject;
          breakpointType = Debug.ScriptBreakPointType.ScriptId;
          breakpointTarget = request.arguments.target;
        }
      }

      if (typeof request.arguments.line == 'number') {
        line = request.arguments.line;
      }

      if (typeof request.arguments.column == 'number') {
        column = request.arguments.column;
      }
    }

    if (breakpointScript) {
      var v8Breakpoint = new V8Breakpoint(breakpointType, breakpointTarget,
        breakpointScript, line, column);
      var success = false;
      if (v8Breakpoint.Set()) {
        success = true;
        DebugManager.BreakpointManager.AddBreakpoint(v8Breakpoint);
      }
      response.success = success;
      response.body = request.arguments;
      response.body.actual_locations = [];
      if (success) {
        response.body.breakpoint = v8Breakpoint.Id();
      }
    } else {
      if (optAddToPending != false) {
        DebugManager.BreakpointManager.AddPendingBreakpoint(request);
      }
    }
  };

  V8CommandProcessor.prototype.backtrace = function(request, response) {
    var frames = globalExecutionState.GetFrames();
    var fromFrame = 0;
    var toFrame = frames.length;

    if (request.arguments) {
      if (request.arguments.fromFrame != undefined) {
        fromFrame = request.arguments.fromFrame;
      }
      if (request.arguments.toFrame != undefined &&
        request.arguments.toFrame < toFrame) {
        toFrame = request.arguments.toFrame;
      }
    }

    response.body = {};
    response.body.fromFrame = fromFrame,
    response.body.toFrame = toFrame,
    response.body.totalFrames = frames.length,
    response.body.frames = [];
    response.refs = [];

    for (var i = fromFrame; i < toFrame; ++i) {
      response.body.frames.push(frames[i].GetProtocolObject());
    }

    response.success = true;
  };

  V8CommandProcessor.prototype.lookup = function(request, response) {
    var handlesResult = {};
    request.arguments.handles.map(function(handle) {
      var handleObject = null;
      if (DebugManager.ScriptsManager.IsScriptHandle(handle)) {
        handleObject = DebugManager.ScriptsManager.GetScriptFromHandle(handle);
        if (handleObject) {
          handleObject = handleObject.GetProtocolObject();
        }
      } else {
        handleObject = CallHostFunction(chakraDebug.JsDiagGetObjectFromHandle,
          handle);
        if (handleObject) {
          AddChildrens(handleObject);
        }
      }

      if (handleObject) {
        if ('fileName' in handleObject && !('name' in handleObject)) {
          handleObject['name'] = handleObject['fileName'];
        }

        if ('scriptId' in handleObject && !('id' in handleObject)) {
          handleObject['id'] = handleObject['scriptId'];
        }

        if ('scriptType' in handleObject && !('name' in handleObject)) {
          handleObject['name'] = handleObject['scriptType'];
        }

        handlesResult[handle] = handleObject;
      }
    });

    response.success = true;
    response.body = handlesResult;
    response.refs = [];
  };

  V8CommandProcessor.prototype.evaluate = function(request, response) {
    if (globalExecutionState) {
      var frames = globalExecutionState.GetFrames();
      var frame = frames[0];
      if (request.arguments) {
        if (typeof request.arguments.frame == 'number') {
          for (var i = 0; i < frames.length; ++i) {
            if (request.arguments.frame == frames[i].GetIndex()) {
              frame = frames[i];
              break;
            }
          }
        } else if (request.arguments.global == true) {
          frame = frames[frames.length - 1];
        }

        var evalResult = frame.Evaluate(request.arguments.expression);
        AddChildrens(evalResult[1]);
        response.success = evalResult[0];
        response.body = evalResult[1];
        response.refs = [];
      } else {
        response.success = false;
      }
    } else {
      response.pending = true;
      pendingMessages.push({ request: request, response: response });
    }
  };

  V8CommandProcessor.prototype.threads = function(request, response) {
    response.body = {
      'totalThreads': 1,
      'threads': [{
        'current': true,
        'id': 1
      }
      ]
    };
    response.success = true;
    response.refs = [];
  };

  V8CommandProcessor.prototype.setexceptionbreak =
    function(request, response) {
      var enabled = false;
      // JsDiagBreakOnExceptionAttributeNone
      var breakOnExceptionAttribute = 0;
      if (request.arguments) {
        if (request.arguments.enabled) {
          enabled = request.arguments.enabled;
        }

        if (enabled && request.arguments.type) {
          if (request.arguments.type == 'all') {
            // JsDiagBreakOnExceptionAttributeUncaught |
            // JsDiagBreakOnExceptionAttributeFirstChance
            breakOnExceptionAttribute = 0x1 | 0x2;
          } else if (request.arguments.type == 'uncaught') {
            // JsDiagBreakOnExceptionAttributeUncaught
            breakOnExceptionAttribute = 0x1;
          }
        }
      }

      var success = CallHostFunction(chakraDebug.JsDiagSetBreakOnException,
        breakOnExceptionAttribute);
      response.success = success ? true : false;

      response.body = {};
      response.body.type = request.arguments.type;
      response.body.enabled = enabled;
      response.refs = [];
    };

  V8CommandProcessor.prototype.scopes = function(request, response) {
    var frames = globalExecutionState.GetFrames();
    var frameIndex = -1;
    var frame;
    for (var i = 0; i < frames.length; ++i) {
      if (frames[i].GetIndex() == request.arguments.frameNumber) {
        frameIndex = frames[i].GetIndex();
        frame = frames[i];
        break;
      }
    }

    if (frameIndex != -1) {
      var props = frame.GetStackProperties();
      var scopes = [];
      var refs = [];
      var locals = [];

      ['exception', 'arguments', 'thisObject', 'returnValue'].forEach(
        function(prop) {
          if (prop in props) {
            locals.push(props[prop]);
          }
        });

      ['functionCallsReturn', 'locals'].forEach(function(prop) {
        if (prop in props) {
          props[prop].forEach(function(element) {
            locals.push(element);
          });
        }
      });

      //var scopesMap = { 'locals': 1, 'globals': 0, 'scopes': 3 };
      if (locals.length > 0) {
        var scopeAndRef = DebugManager.Utility.CreateScopeAndRef(1, locals,
          frameIndex, request.arguments.frame_index);
        scopes.push(scopeAndRef.scope);
        refs.push(scopeAndRef.ref);
      }

      if (props['scopes'] && props['scopes'].length > 0) {
        var allScopeProperties = [];
        props['scopes'].map(function(scope) {
          var scopeHandle = scope.handle;

          var scopeProperties = CallHostFunction(
            chakraDebug.JsDiagGetProperties, scopeHandle, 0, 1000);

          scopeProperties['properties'].map(function(property) {
            allScopeProperties.push(property);
          });
          scopeProperties['debuggerOnlyProperties'].map(function(dbgProp) {
            allScopeProperties.push(dbgProp);
          });
        });

        if (allScopeProperties.length > 0) {
          var allScopeAndRef = DebugManager.Utility.CreateScopeAndRef(3,
            allScopeProperties, frameIndex, request.arguments.frame_index);
          scopes.push(allScopeAndRef.scope);
          refs.push(allScopeAndRef.ref);
        }
      }

      if (props['globals'] && props['globals'].handle) {
        var globalsProps = CallHostFunction(chakraDebug.JsDiagGetProperties,
          props['globals'].handle, 0, 5000);

        var globalProperties = [];
        globalsProps['properties'].map(function(glbProperty) {
          globalProperties.push(glbProperty);
        });
        globalsProps['debuggerOnlyProperties'].map(function(glbDbgProp) {
          globalProperties.push(glbDbgProp);
        });
        if (globalProperties.length > 0) {
          var glbScopeAndRef = DebugManager.Utility.CreateScopeAndRef(0,
            globalProperties, frameIndex, request.arguments.frame_index);
          scopes.push(glbScopeAndRef.scope);
          refs.push(glbScopeAndRef.ref);
        }
      }

      response['success'] = true;
      response['refs'] = refs;
      response['body'] = {
        'fromScope': 0,
        'toScope': scopes.length,
        'totalScopes': scopes.length,
        'scopes': scopes
      };
    }
  };

  V8CommandProcessor.prototype.scope = function(request, response) { };

  V8CommandProcessor.prototype.listbreakpoints = function(request, response) {
    // {'command':'listbreakpoints','type':'request','seq':25}
    var breakOnExceptionAttribute = CallHostFunction(
      chakraDebug.JsDiagGetBreakOnException);

    response.success = true;
    response.body = {};
    response.body.breakpoints = [];
    response.body.breakOnExceptions = breakOnExceptionAttribute != 0;
    response.body.breakOnUncaughtExceptions =
      (breakOnExceptionAttribute & 0x1) == 0x1;

    var breakpoints = DebugManager.BreakpointManager.GetBreakpoints();

    breakpoints.forEach(function(element, index, array) {
      if (element) {
        response.body.breakpoints.push(element.GetProtocolObject());
      }
    });
  };

  V8CommandProcessor.prototype.clearbreakpoint = function(request, response) {
    if (request.arguments.breakpoint) {
      response.success = true;
      DebugManager.BreakpointManager.RemoveBreakpoint(
        request.arguments.breakpoint);
    }
  };

  V8CommandProcessor.prototype.suspend = function(request, response) {
    if (globalExecutionState) {
      response.success = true;
      globalExecutionState.ChangeToBreak();
    } else {
      request.pending = true;
      pendingMessages.push({ request: request, response: response });
    }
  };

  // Not supported commands
  V8CommandProcessor.prototype.break = function(request, response) { };
  V8CommandProcessor.prototype.changebreakpoint =
    function(request, response) { };
  V8CommandProcessor.prototype.changelive = function(request, response) { };
  V8CommandProcessor.prototype.clearbreakpointgroup =
    function(request, response) { };
  V8CommandProcessor.prototype.disconnect = function(request, response) { };
  V8CommandProcessor.prototype.flags = function(request, response) { };
  V8CommandProcessor.prototype.frame = function(request, response) { };
  V8CommandProcessor.prototype.gc = function(request, response) { };
  V8CommandProcessor.prototype.references = function(request, response) { };
  V8CommandProcessor.prototype.restartframe = function(request, response) { };
  V8CommandProcessor.prototype.setvariablevalue =
    function(request, response) { };
  V8CommandProcessor.prototype.v8flag = function(request, response) { };
  V8CommandProcessor.prototype.version = function(request, response) { };


  var ChakraDebugEvent = {
    SourceCompile: 0,
    CompileError: 1,
    Breakpoint: 2,
    StepComplete: 3,
    DebuggerStatement: 4,
    AsyncBreak: 5,
    RuntimeException: 6
  };

  function ChakraDebugEventProcessor(eventData) {
    this.eventData = eventData;
    this.ProcessPendingMessages = function(isAsyncBreak) {
      var pendingObject;
      while (pendingObject = pendingMessages.shift()) {
        try {
          var request = pendingObject.request;
          var response = pendingObject.response;
          Logger.LogAPI('ProcessPendingMessages request', request);
          var v8CommandProcessor = new V8CommandProcessor();
          v8CommandProcessor[request.command](request, response);
          if (!request.pending) {
            var responseJson = JSON.stringify(response);
            Logger.LogDebugJson('ProcessPendingMessages responseJson',
                                responseJson);
            chakraDebug.SendDelayedRespose(responseJson);
          }
        } catch (ex) {
          Logger.LogError('ProcessPendingMessages exception: ' + ex.message,
                          ex.stack);
        }
      }
    };
  }

  ChakraDebugEventProcessor.prototype[ChakraDebugEvent.SourceCompile] =
    function() {
      var event = DebugManager.Utility.MakeEvent('afterCompile');
      event.body = {};

      var scriptObj = DebugManager.ScriptsManager.GetScript(
        this.eventData.scriptId);
      event.body.script = scriptObj.GetProtocolObject();

      DebugManager.BreakpointManager.ProcessPendingBreakpoints();

      return event;
    };

  ChakraDebugEventProcessor.prototype[ChakraDebugEvent.CompileError] =
    function() { };

  ChakraDebugEventProcessor.prototype[ChakraDebugEvent.Breakpoint] =
    function() {
      globalExecutionState = new ExecutionState(ExecutionStateType.Break,
        this.eventData.scriptId);

      this.ProcessPendingMessages(false);

      var scriptObj = DebugManager.ScriptsManager.GetScript(
        globalExecutionState.GetBreakScriptId());
      var event = DebugManager.Utility.MakeEvent('break');
      event.body = {};
      event.body.sourceLine = this.eventData.line;
      event.body.sourceColumn = this.eventData.column;
      event.body.sourceLineText = this.eventData.sourceText;
      event.body.script = scriptObj.GetProtocolObject();

      var v8Breakpoint = DebugManager.BreakpointManager.GetBreakpoint(
        this.eventData.breakpointId);
      if (v8Breakpoint) {
        v8Breakpoint.IncreaseHit();
        event.body.breakpoints = [v8Breakpoint.Id()];
      }

      return event;
    };

  ChakraDebugEventProcessor.prototype[ChakraDebugEvent.StepComplete] =
    ChakraDebugEventProcessor.prototype[ChakraDebugEvent.Breakpoint];

  ChakraDebugEventProcessor.prototype[ChakraDebugEvent.DebuggerStatement] =
    ChakraDebugEventProcessor.prototype[ChakraDebugEvent.Breakpoint];

  ChakraDebugEventProcessor.prototype[ChakraDebugEvent.AsyncBreak] =
    function() {

      globalExecutionState = new ExecutionState(ExecutionStateType.Async,
        this.eventData.scriptId);

      this.ProcessPendingMessages(true);
    };

  ChakraDebugEventProcessor.prototype[ChakraDebugEvent.RuntimeException] =
    function() {
      globalExecutionState = new ExecutionState(ExecutionStateType.Break,
        this.eventData.scriptId);

      this.ProcessPendingMessages(false);

      var scriptObj = DebugManager.ScriptsManager.GetScript(
        globalExecutionState.GetBreakScriptId());
      var event = DebugManager.Utility.MakeEvent('exception');
      event.body = {};
      event.body.sourceLine = this.eventData.line;
      event.body.sourceColumn = this.eventData.column;
      event.body.sourceLineText = this.eventData.sourceText;
      event.body.script = scriptObj.GetProtocolObject();
      return event;
    };

  function DebugEventToString(debugEvent) {
    switch (debugEvent) {
      case 0:
        return 'JsDiagDebugEventSourceCompile';
      case 1:
        return 'JsDiagDebugEventCompileError';
      case 2:
        return 'JsDiagDebugEventBreakpoint';
      case 3:
        return 'JsDiagDebugEventStepComplete';
      case 4:
        return 'JsDiagDebugEventDebuggerStatement';
      case 5:
        return 'JsDiagDebugEventAsyncBreak';
      case 6:
        return 'JsDiagDebugEventRuntimeException';
      default:
        return 'Unhandled JsDiagDebugEvent: ' + debugEvent;
    }
  }

  var chakraDebug = {};

  chakraDebug.ProcessDebuggerMessage = function(request) {
    Logger.LogAPI('ProcessDebuggerMessage request', request);
    try {
      request = JSON.parse(request);
      var response = new DebugManager.Utility.MakeResponse(request);
      var v8CommandProcessor = new V8CommandProcessor();
      v8CommandProcessor[request.command](request, response);

      var responseJson = response.pending ? undefined :
        JSON.stringify(response);
      Logger.LogDebugJson('ProcessDebuggerMessage responseJson', responseJson);
      return responseJson;
    } catch (ex) {
      Logger.LogError('ProcessDebuggerMessage exception: ' + ex.message,
                      ex.stack);
    }
  };

  chakraDebug.ProcessDebugEvent = function(debugEvent, eventData) {
    Logger.LogAPI('ProcessDebugEvent debugEvent: ' +
      DebugEventToString(debugEvent) + ', eventData: ' +
      JSON.stringify(eventData));
    try {
      var chakraDebugEventProcessor = new ChakraDebugEventProcessor(eventData);
      var event = chakraDebugEventProcessor[debugEvent]();
      if (typeof event != 'undefined') {
        event = JSON.stringify(event);
      }
      Logger.LogDebugJson('ProcessDebugEvent debugEvent: ' +
        DebugEventToString(debugEvent) + ', event: ' + event);
      return event;
    } catch (ex) {
      Logger.LogError('ProcessDebugEvent exception: ' + ex.message, ex.stack);
    }
  };

  chakraDebug.ProcessShouldContinue = function(debugEvent) {
    var shouldContinue = true;

    if (!globalExecutionState) {
      shouldContinue = true;
    } else if (typeof debugEvent == 'number' &&
      ((debugEvent == 0 /*JsDiagDebugEventSourceCompile*/) ||
      (debugEvent == 1 /*JsDiagDebugEventCompileError*/))) {
      shouldContinue = true;
    } else if (globalExecutionState.IsAsyncBreak()) {
      globalExecutionState = undefined;
      shouldContinue = true;
    } else {
      shouldContinue = false;
    }
    return shouldContinue;
  };

  keepAliveObject.chakraDebug = chakraDebug;

  return keepAliveObject.chakraDebug;
});
