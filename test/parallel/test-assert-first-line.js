'use strict';

// Verify that asserting in the very first line produces the expected result.

const common = require('../common');
const assert = require('assert');
const { path } = require('../common/fixtures');

// https://github.com/nodejs/node/pull/17581 added functionality to
// lib/assert.js that made assert messages much nicer.
// However, it relies on non-standard/undocumented V8 error APIs.
// Thus, there is a modification to that file to fall back to the old
// path for ChakraCore

function engineSpecificAssert(v8, cc) {
  return common.engineSpecificMessage({
    v8: `The expression evaluated to a falsy value:\n\n  ${v8}`,
    chakracore: cc
  });
}

assert.throws(
  () => require(path('assert-first-line')),
  {
    name: 'AssertionError [ERR_ASSERTION]',
    message: engineSpecificAssert(
      'ässört.ok(\'\')\n',
      '\'\' == true'
    )
  }
);

assert.throws(
  () => require(path('assert-long-line')),
  {
    name: 'AssertionError [ERR_ASSERTION]',
    message: engineSpecificAssert(
      'assert.ok(\'\')\n',
      '\'\' == true'
    )
  }
);
