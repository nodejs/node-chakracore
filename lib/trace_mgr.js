'use strict';

//Disable trace handling inside trace handler to avoid recursion
//Set to true to prevent action till loading is complete -- set to false at end
var reentrantDisable = true;

var process = require('process');

var lazyloadDone = false;
var path = undefined;
var fs = undefined;

///////////////////////////////
//Trace calls

/*
*Check and emit time travel trace
* -- invoke remoteTraceManagerObj functions as needed.
*@emitKind string matching one of the emitOption flags
*@optInfo additional info we want to pass in (error code for emitOnExit)
*@result 're-entrant' 'disabled', 'no-sample', 'fail', 'success'
*/
function emitTrace(emitKind, optInfo) {
  var res = emitTrace_helper(emitKind, optInfo);
  if (res.flag === 'success') {
    //This is an intentional programatic breakpoint
    // -- it is only triggerable in --replay-debug mode
    debugger; /*TTD_DEBUG_BREAKPOINT*/ // eslint-disable-line no-debugger
  }
  res.action();
}
exports.emitTrace = emitTrace;

function buildTraceResult(flagv, actionv) {
  var realAction = actionv || function() { };
  return { flag: flagv, action: realAction };
}

function emitTrace_helper(emitKind, optInfo) {
  if (reentrantDisable) {
    return buildTraceResult('re-entrant');
  }

  reentrantDisable = true;
  try {
    if (!global.enabledDiagnosticsTrace || !global.enabledDiagnosticsTrace()) {
      return buildTraceResult('disabled');
    }

    if (!checkGlobalShouldEmit(emitKind, optInfo)) {
      return buildTraceResult('no-sample');
    }

    if (!lazyloadDone) {
      path = require('path');
      fs = require('fs');

      lazyloadDone = true;
    }

    //Process a synchronous write action for a unique trace bin
    //  -- otherwise a async action on a multiple trace bin
    var sampleRes = buildTraceResult('no-sample');
    if (emitKind === 'emitOnExit' ||
      emitKind === 'emitOnException' ||
      emitKind === 'emitOnSigInt') {
      sampleRes = emitSyncTraceKind(emitKind, optInfo);
    } else {
      sampleRes = emitAsyncTraceKind(emitKind);
    }

    if (sampleRes.flag === 'success') {
      updateGlobalSampleStats(emitKind);
    }
    return sampleRes;

  } catch (ex) {
    process.stderr.write('Failed in emitTrace with: ' + ex);
    return buildTraceResult('fail');
  } finally {
    reentrantDisable = false;
  }
}

////
//Helpers for trace calls
function emitSyncTraceKind(emitKind, optInfo) {
  //build up trace name
  var traceName = emitKind;
  if (emitKind === 'emitOnExit') {
    traceName += ('_code-' + optInfo);
  }

  //invoke the trace writer and remote manager if needed
  var resolvedPath = createTraceLogTarget(traceName);
  if (!resolvedPath) {
    return buildTraceResult('fail');
  }

  function action() {
    try {
      process.stderr.write(`    Write error trace to: ${resolvedPath}\n`);
      global.emitTTDLog(resolvedPath);

      if (emitOptions.remoteTraceManagerObj &&
        emitOptions.remoteTraceManagerObj.uploadTraceSync) {
        emitOptions.remoteTraceManagerObj.uploadTraceSync(resolvedPath);
      }
    } catch (ex) {
      process.stderr.write('Failed in emitTrace with: ' + ex);
    }
  }

  return buildTraceResult('success', action);
}

function emitAsyncTraceKind(emitKind) {
  //get trace stack and check if we want to emit
  var stk = generateFuzzyStack(emitKind);
  var entry = checkBinShouldEmit(stk);
  if (!entry) {
    return buildTraceResult('no-sample');
  }

  //build up trace name
  var traceBucketName = `${emitKind}_${stk.fbin}_bucket-${entry.bucketId}`;
  var traceDirName = `trace-${entry.traceCtr}`;
  var traceName = path.join(traceBucketName, traceDirName);

  //invoke the trace writer and remote manager if needed
  var resolvedPath = createTraceLogTarget(traceName);
  if (!resolvedPath) {
    return buildTraceResult('fail');
  }

  function action() {
    try {
      process.stderr.write(`    Write error trace to: ${resolvedPath}\n`);
      global.emitTTDLog(resolvedPath);

      if (emitOptions.remoteTraceManagerObj &&
        emitOptions.remoteTraceManagerObj.uploadTraceAsync) {
        emitOptions.remoteTraceManagerObj.uploadTraceAsync(resolvedPath);
      }
    } catch (ex) {
      process.stderr.write('Failed in emitTrace with: ' + ex);
    }
  }

  //update the bin stats as needed
  updateEmitInfo(entry, resolvedPath);
  return buildTraceResult('success', action);
}

//create a directory for emitting the trace (if possible) and return it
function createTraceLogTarget(tracename) {
  var traceRootDir = emitOptions.localTraceDirectory ||
    path.dirname(process.mainModule.filename);

  // Add the PID to the trace name
  tracename = `${tracename}_pid${process.pid}`;

  var resolvedTracePath =
    path.resolve(traceRootDir, '_diagnosticTraces', tracename);

  //ensure directory exists and is empty...
  var ok = ensureTraceTarget(resolvedTracePath);
  if (!ok) {
    return undefined;
  }

  return resolvedTracePath;
}

//ensure directory exists and is empty...
function ensureTraceTarget(pth) {
  //I don't like this and don't want it to be happening so I am going to bail
  if (!path.isAbsolute(pth)) {
    return false;
  }

  var okdir = createTargetDirectory(pth);
  if (!okdir) {
    return false;
  }

  return deleteTargetDirectoryContents(pth);
}

function createTargetDirectory(pth) {
  //see if it just exists and, if so, just return true
  var accessok = fs.constants.R_OK | fs.constants.W_OK | fs.constants.X_OK;
  try {
    fs.accessSync(pth, accessok);
    if (fs.statSync(pth).isDirectory()) {
      return true;
    }
  } catch (ei) { }

  //walk up the directory to see where the first valid part of the path is
  var prefixPath = pth;
  var suffixPaths = [];
  var baseFound = false;
  do {
    //check for bad prefix
    if (prefixPath === path.dirname(prefixPath)) {
      process.stderr.write(`Failed prefix: ${pth} -> ${prefixPath}\n`);
      return false;
    }

    suffixPaths.push(path.basename(prefixPath)); //reverse order
    prefixPath = path.dirname(prefixPath);

    try {
      fs.accessSync(prefixPath, accessok);
      baseFound = fs.statSync(prefixPath).isDirectory();
    } catch (ei) { }
  } while (!baseFound);

  //now extend the prefix with all the suffix parts
  while (suffixPaths.length > 0) {
    try {
      prefixPath = path.resolve(prefixPath, suffixPaths.pop());
      fs.mkdirSync(prefixPath);
    } catch (ec) {
      process.stderr.write(`Failed creating trace directory : ${ec}\n`);
      return false;
    }
  }

  return true;
}

function deleteTargetDirectoryContents(pth) {
  try {
    var items = fs.readdirSync(pth);
    for (var i = 0; i < items.length; i++) {
      var fpath = path.resolve(pth, items[i]);
      var stats = fs.lstatSync(fpath);
      if (stats.isFile()) {
        fs.unlinkSync(fpath);
      } else if (stats.isDirectory()) {
        var recok = deleteTargetDirectoryContents(fpath);
        if (!recok) {
          return false;
        }

        fs.rmdirSync(fpath);
      } else {
        return false; //something strange in here.
      }
    }
  } catch (ex) {
    process.stderr.write(`Failed cleaning directory contents: ${ex}\n`);
    return false;
  }

  return true;
}

//after we take a sample update the sampling stats
function updateGlobalSampleStats(eventKind) {
  currentSampleRate[eventKind] *= emitOptions.backoffFactors[eventKind];

  var updateTime = new Date();
  emitMinTimeValue['emitOnLogWarn'] = new Date(updateTime);
  emitMinTimeValue['emitOnLogError'] = new Date(updateTime);

  if (eventKind === 'emitOnAssert') {
    emitMinTimeValue['emitOnAssert'] = updateTime;
  }
}

///////////////////////////////
//Trace emit manager code

var emitOptions = {
  emitOnExit: 'error', //emit a trace on exit -- off, error, all
  emitOnException: true, //emit a trace on uncaught execption
  emitOnSigInt: true, //emit a trace on sigint
  emitOnLogWarn: true, //check for trace emit on console.warn
  emitOnLogError: true, //check for trace emit on console.error
  emitOnAssert: true, //check for trace emit on console.assert or assert

  globalMinInterval: 500, //min interval between console/assert trace emits
  globalBackoffCancelInterval: 5000, //time when we reset the backoff interval

  //The probability that we sample on the first warn/error/assert encountered
  initialRates: {
    emitOnLogWarn: 0.25,
    emitOnLogError: 0.25,
    emitOnAssert: 1.0
  },

  //The baseline background probability that we sample traces
  baselineRates: {
    emitOnLogWarn: 0.25,
    emitOnLogError: 0.25,
    emitOnAssert: 1.0
  },

  //backoff factor to apply
  backoffFactors: {
    emitOnLogWarn: 0.25,
    emitOnLogError: 0.5,
    emitOnAssert: 0.5
  },

  binMaxSampled: 10, //Max number of traces to take per stack bin
  binBackoffFactor: 0.5, //sample probability #preserved * binBackoffFactor

  localTraceDirectory: undefined, //The root directory for storing traces
  remoteTraceManagerObj: undefined //manager object for remote trace support
};

var callStackEmitInfoMap = new Map();
var bucketCtr = 0;

var emitMinTimeValue = {
  emitOnLogWarn: new Date(0),
  emitOnLogError: new Date(0),
  emitOnAssert: new Date(0)
};

var currentSampleRate = {};
for (var srp in emitOptions.initialRates) {
  currentSampleRate[srp] = emitOptions.initialRates[srp];
}

/*
*Update emitOptions from the given options object
*/
function setOptions(optionsObj) {
  for (var opt in optionsObj) {
    //TODO: need more error checking on the validity of the option values
    emitOptions[opt] = optionsObj[opt];
  }

  for (var srp in emitOptions.initialRates) {
    currentSampleRate[srp] = emitOptions.initialRates[srp];
  }
}
exports.setOptions = setOptions;

/*
*Do a preliminary check if the emitKind is even enabled and to do sampling.
*@emitKind string matching one of the emitOption flags
*@optInfo additional info we want to pass in (error code for emitOnExit)
*@result true if we may want to sample false if we definitely do not
*/
function checkGlobalShouldEmit(emitKind, optInfo) {
  //Check if the flag is enabled
  if (emitKind === 'emitOnExit') {
    if (emitOptions.emitOnExit === 'all') {
      return true;
    } else {
      return (emitOptions.emitOnExit === 'error') && (optInfo !== 0);
    }
  } else if (emitKind === 'emitOnException' || emitKind === 'emitOnSigInt') {
    return emitOptions[emitKind];
  } else {
    if (!emitOptions[emitKind]) {
      return false;
    }

    var sampleInterval = new Date() - emitMinTimeValue[emitKind];

    //Don't sample too often no matter what (or we can basically live lock)
    if (sampleInterval < emitOptions.globalMinInterval) {
      return false;
    }

    //Relax our global rate if it has been a while
    if (sampleInterval >= emitOptions.globalBackoffCancelInterval) {
      var currRate = currentSampleRate[emitKind];
      var blRate = emitOptions.baselineRates[emitKind];
      currentSampleRate[emitKind] = Math.max(currRate, blRate);
    }

    //Probabalistic check if we want to do sample new trace
    return Math.random() < currentSampleRate[emitKind];
  }
}

/*
*Given a fuzzy stack see if we want to write a trace for the call
*@fuzzyStack is the stack produced by getFuzzyCallStack
*@result if we want to sample return the entry otherwise undefined
*/
function checkBinShouldEmit(fuzzyStack) {
  var entry = resolveStackEntry(fuzzyStack);

  //stop sampling after max sampled values -- e.g. we don't need 100 repros
  if (entry.traceCtr > emitOptions.binMaxSampled) {
    return undefined;
  }

  //check if we want to sample on this entry -- we don't need every hit on this
  var sampleProb = Math.pow(emitOptions.binBackoffFactor, entry.traceCtr);
  return (Math.random() < sampleProb) ? entry : undefined;
}

/*
*Note that we sampled for this entry and update the info as appropriate
*@sampleEntry the entry we did the sample on
*@sampleName the sample we are adding
*/
function updateEmitInfo(sampleEntry, sampleName) {
  sampleEntry.traceCtr++;
  sampleEntry.samples.push(sampleName);
}

////
//Helpers for emit manager code
function resolveStackEntry(fuzzyStack) {
  if (!callStackEmitInfoMap.has(fuzzyStack.hash)) {
    callStackEmitInfoMap.set(fuzzyStack.hash, []);
  }
  var stackList = callStackEmitInfoMap.get(fuzzyStack.hash);

  for (var i = 0; i < stackList.length; ++i) {
    if (eqFuzzyStacks(fuzzyStack, stackList[i].stack)) {
      return stackList[i];
    }
  }

  stackList.push({
    stack: fuzzyStack,
    bucketId: bucketCtr++,
    traceCtr: 0,
    samples: []
  });
  return stackList[stackList.length - 1];
}

///////////////////////////////
//Fuzzy stack code

//Helper hashcode computation function (dbj2 variant)
function djbHash(str, hash) {
  for (var i = 0; i < str.length; i++) {
    hash = (hash * 33) ^ str.charCodeAt(i);
  }
  return hash;
}

//helper is absolute path copied from path -- avoid neededing to require it
function directIsAbsoluteW32(pth) {
  const len = pth.length;
  if (len === 0)
    return false;

  var code = pth.charCodeAt(0);
  if (code === 47/*/*/ || code === 92/*\*/) {
    return true;
  } else if ((code >= 65/*A*/ && code <= 90/*Z*/) ||
    (code >= 97/*a*/ && code <= 122/*z*/)) {
    // Possible device root

    if (len > 2 && pth.charCodeAt(1) === 58/*:*/) {
      code = pth.charCodeAt(2);
      if (code === 47/*/*/ || code === 92/*\*/)
        return true;
    }
  }
  return false;
}

function directIsAbsolutePosix(pth) {
  return pth.length > 0 && pth.charCodeAt(0) === 47/*/*/;
}

var directIsAbsolute = (process.platform === 'win32') ?
  directIsAbsoluteW32 :
  directIsAbsolutePosix;

/*
* Create and return a fuzzy stack match for the current call.
*/
function generateFuzzyStack(eventKind) {
  //Create an array of the file/lines for user space code in the call stack
  var errstk = new Error()
    .stack
    .split('\n')
    .slice(1)
    .map(function(frame) {
      return frame.substring(frame.indexOf('(') + 1, frame.lastIndexOf(')'));
    })
    .filter(function(frame) {
      return directIsAbsolute(frame);
    });

  var lastframe = errstk[errstk.length - 1];
  var fname = lastframe.substr(lastframe.lastIndexOf(path.sep) + 1)
      .replace('.js:', '_line-')
      .replace(':', '_column-');

  //Identify which frames are recursive (appear multiple times in the stack)
  var recFrames = new Map();
  var hasRecFrames = false;
  for (var i = 0; i < errstk.length; ++i) {
    if (recFrames.has(errstk[i])) {
      hasRecFrames = true;
      recFrames.set(errstk[i], true);
    } else {
      recFrames.set(errstk[i], false);
    }
  }

  if (hasRecFrames) {
    //Compress any recursive frames
    var cpos = 0;
    var fpos = 0;
    while (fpos < errstk.length) {
      if (recFrames.get(errstk[fpos])) {
        var recArray = [];
        var spanpos = fpos;
        var spanend = errstk.lastIndexOf(errstk[fpos]);
        while (spanpos <= spanend) {
          if (recArray.indexOf(errstk[spanpos]) === -1) {
            recArray.push(errstk[spanpos]);
          }

          spanend = Math.max(spanend, errstk.lastIndexOf(errstk[spanpos]));
          spanpos++;
        }

        errstk[cpos] = recArray.sort();
        fpos = spanend + 1;
      } else {
        if (cpos !== fpos) {
          errstk[cpos] = errstk[fpos];
        }
        fpos++;
      }

      cpos++;
    }

    errstk = errstk.slice(0, cpos);
  }

  var chash = 5381;
  for (i = 0; i < errstk.length; ++i) {
    if (Array.isArray(errstk[i])) {
      for (var j = 0; j < errstk[i].length; ++j) {
        chash = djbHash(errstk[i][j], chash);
      }
    } else {
      chash = djbHash(errstk[i], chash);
    }
  }

  return {
    kind: eventKind,
    fbin: fname,
    hash: chash,
    stack: errstk
  };
}

/*
* Check if 2 fuzzy stacks are equal.
*/
function eqFuzzyStacks(s1, s2) {
  if (s1.hash !== s2.hash || s1.stack.length !== s2.stack.length) {
    return false;
  }

  for (var i = 0; i < s1.stack.length; ++i) {
    if (Array.isArray(s1.stack[i]) !== Array.isArray(s2.stack[i])) {
      return false;
    }

    if (Array.isArray(s1.stack[i])) {
      if (s1.stack[i].length !== s2.stack[i].length) {
        return false;
      }

      for (var j = 0; j < s1.stack[i].length; ++j) {
        if (s1.stack[j] !== s2.stack[j]) {
          return false;
        }
      }
    } else {
      if (s1.stack[i] !== s2.stack[i]) {
        return false;
      }
    }
  }

  return true;
}

///////////////////////////////
//Bind handlers

function onExitHandler(code) {
  emitTrace('emitOnExit', code);
}
exports.onExitHandler = onExitHandler;

function onUncaughtExceptionHandler() {
  emitTrace('emitOnException');
}
exports.onUncaughtExceptionHandler = onUncaughtExceptionHandler;

function onSigIntHandler(hasUserHandler) {
  emitTrace('emitOnSigInt');

  if (!hasUserHandler) {
    //Really exit without triggering any other events
    process.reallyExit(0);
  }
}
exports.onSigIntHandler = onSigIntHandler;

reentrantDisable = false;
