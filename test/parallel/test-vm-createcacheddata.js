'use strict';

const common = require('../common');

// Chakra doesn't yet support createCachedData.
// See bug https://github.com/nodejs/node-chakracore/issues/560
if (!common.isChakraEngine) {

const { Script } = require('vm');
const assert = require('assert');

const source = 'function x() {} const y = x();';

const script = new Script(source);
let cachedData = script.createCachedData();
assert(cachedData instanceof Buffer);

assert(!new Script(source, { cachedData }).cachedDataRejected);

script.runInNewContext();

for (let i = 0; i < 10; i += 1) {
  cachedData = script.createCachedData();

  assert(!new Script(source, { cachedData }).cachedDataRejected);
}
}
