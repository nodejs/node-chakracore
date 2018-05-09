'use strict';
const common = require('../common');

// This test ensures that unnecessary prototypes are no longer
// being generated by Environment::NewFunctionTemplate.

//
// broken for some reason on Node-chakracore
// see https://github.com/nodejs/node-chakracore/issues/536
//
if (!common.isChakraEngine) {
  const assert = require('assert');

  [
    process.binding('udp_wrap').UDP.prototype.bind6,
    process.binding('tcp_wrap').TCP.prototype.bind6,
    process.binding('udp_wrap').UDP.prototype.send6,
    process.binding('tcp_wrap').TCP.prototype.bind,
    process.binding('udp_wrap').UDP.prototype.close,
    process.binding('tcp_wrap').TCP.prototype.open
  ].forEach((binding, i) => {
    assert.strictEqual('prototype' in binding, false, `Test ${i} failed`);
  });
}