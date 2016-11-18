// Flags: --harmony_sharedarraybuffer

'use strict';
const common = require('../common');

if (common.isChakraEngine) {
  console.log('1..0 # Skipped: This test is disabled for chakra engine ' +
          'because SharedArrayBuffer is not yet supported');
  return;
}

const assert = require('assert');
const util = require('util');

/* global SharedArrayBuffer */
const sab = new SharedArrayBuffer(4);
assert.strictEqual(util.format(sab), 'SharedArrayBuffer { byteLength: 4 }');
