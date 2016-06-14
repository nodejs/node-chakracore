'use strict';
const common = require('../common');
const assert = require('assert');
const spawn = require('child_process').spawn;

if (common.isChakraEngine) {
  console.log('1..0 # Skipped: This test is disabled for chakra engine ' +
  'because debugger support is not implemented yet.');
  return;
}

const child = spawn(process.execPath, ['--debug-brk=' + common.PORT, '-i']);
child.stderr.once('data', common.mustCall(function() {
  child.stdin.end('.exit');
}));

child.on('exit', common.mustCall(function(c) {
  assert.strictEqual(c, 0);
}));
