'use strict';
var common = require('../common');
var assert = require('assert');
var exec = require('child_process').exec;
var path = require('path');

function errExec(script, callback) {
  var cmd = '"' + process.argv[0] + '" "' +
            path.join(common.fixturesDir, script) + '"';
  return exec(cmd, function(err, stdout, stderr) {
    // There was some error
    assert.ok(err);

    if (!common.isChakraEngine) {
      // More than one line of error output. (not necessarily for chakra engine)
      assert.ok(stderr.split('\n').length > 2);
    }

    if (!common.isChakraEngine) { // chakra does not output script
      // Assert the script is mentioned in error output.
      assert.ok(stderr.indexOf(script) >= 0);
    }

    // Proxy the args for more tests.
    callback(err, stdout, stderr);
  });
}


// Simple throw error
errExec('throws_error.js', common.mustCall(function(err, stdout, stderr) {
  assert.ok(/blah/.test(stderr));
}));


// Trying to JSON.parse(undefined)
errExec('throws_error2.js', common.mustCall(function(err, stdout, stderr) {
  assert.ok(/SyntaxError/.test(stderr));
}));


// Trying to JSON.parse(undefined) in nextTick
errExec('throws_error3.js', common.mustCall(function(err, stdout, stderr) {
  assert.ok(/SyntaxError/.test(stderr));
}));


// throw ILLEGAL error
errExec('throws_error4.js', common.mustCall(function(err, stdout, stderr) {
  if (!common.isChakraEngine) { // chakra does not output source line
    assert.ok(/\/\*\*/.test(stderr));
  }
  assert.ok(/SyntaxError/.test(stderr));
}));

// Specific long exception line doesn't result in stack overflow
errExec('throws_error5.js', common.mustCall(function(err, stdout, stderr) {
  assert.ok(/SyntaxError/.test(stderr));
}));

// Long exception line with length > errorBuffer doesn't result in assertion
errExec('throws_error6.js', common.mustCall(function(err, stdout, stderr) {
  assert.ok(/SyntaxError/.test(stderr));
}));

// Object that throws in toString() doesn't print garbage
errExec('throws_error7.js', common.mustCall(function(err, stdout, stderr) {
  assert.ok(/<toString\(\) threw exception/.test(stderr));
}));
