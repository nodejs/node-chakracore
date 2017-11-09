'use strict';
// Flags: --expose-gc

const common = require('../../common');
const binding = require(`./build/${common.buildType}/test_buffer`);
const assert = require('assert');

assert.strictEqual(binding.newBuffer().toString(), binding.theText);
assert.strictEqual(binding.newExternalBuffer().toString(), binding.theText);

// Don't rely on Chakra's GC to behave the same as v8's.
if (process.jsEngine !== 'chakracore') {
  console.log('gc1');
  global.gc();
  assert.strictEqual(binding.getDeleterCallCount(), 1,
                     'deleter was not called');
}

assert.strictEqual(binding.copyBuffer().toString(), binding.theText);

let buffer = binding.staticBuffer();
assert.strictEqual(binding.bufferHasInstance(buffer), true,
                   'buffer type checking fails');
assert.strictEqual(binding.bufferInfo(buffer), true);
buffer = null;

// Don't rely on Chakra's GC to behave the same as v8's.
if (process.jsEngine !== 'chakracore') {
  global.gc();
  console.log('gc2');
  assert.strictEqual(binding.getDeleterCallCount(), 2,
                     'deleter was not called');
}
