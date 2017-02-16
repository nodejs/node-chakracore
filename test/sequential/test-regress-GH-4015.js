'use strict';
const common = require('../common');
const assert = require('assert');
const exec = require('child_process').exec;

const cmd = '"' + process.execPath + '" ' +
            '"' + common.fixturesDir + '/test-regress-GH-4015.js"';

exec(cmd, function(err, stdout, stderr) {
  const expectedError = common.engineSpecificMessage({
    v8: /RangeError: Maximum call stack size exceeded/,
    chakracore: /Error: Out of stack space/
  });
  assert(expectedError.test(stderr));
});
