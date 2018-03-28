'use strict';

require('../common');
const assert = require('assert').strict;

// Temporarily disabled on chakra.
if (!common.isChakraEngine) {
assert.throws(() => { throw new Error('foo'); }, { bar: true });
}