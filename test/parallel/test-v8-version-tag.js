'use strict';
const common = require('../common');
const assert = require('assert');
const v8 = require('v8');

if (common.isChakraEngine)
  common.skip('This test is disabled for chakra engine.');

const versionTag1 = v8.cachedDataVersionTag();
assert.strictEqual(typeof versionTag1, 'number');
assert.strictEqual(v8.cachedDataVersionTag(), versionTag1);

// The value of cachedDataVersionTag is derived from the command line flags and
// detected CPU features. Test that the value does indeed update when flags
// are toggled.
v8.setFlagsFromString('--allow_natives_syntax');

const versionTag2 = v8.cachedDataVersionTag();
assert.strictEqual(typeof versionTag2, 'number');
assert.strictEqual(v8.cachedDataVersionTag(), versionTag2);

assert.notStrictEqual(versionTag1, versionTag2);
