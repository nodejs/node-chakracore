'use strict';
const common = require('../../common');
const assert = require('assert');

// testing handle scope api calls
const testHandleScope = require(`./build/${common.buildType}/binding`);

testHandleScope.NewScope();

assert.ok(testHandleScope.NewScopeEscape() instanceof Object);

if (!common.isChakraEngine) {
  testHandleScope.NewScopeEscapeTwice();
}

assert.throws(
  () => {
    testHandleScope.NewScopeWithException(() => { throw new RangeError(); });
  },
  RangeError);
