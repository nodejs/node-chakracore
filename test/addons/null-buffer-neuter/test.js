'use strict';
// Flags: --expose-gc

const common = require('../../common');
const binding = require(`./build/${common.buildType}/binding`);

if (common.isChakraEngine) {
  common.skip('This test is disabled for chakra engine because it depends ' +
              'on v8 GC behavior. Chakra GC may find reference-like data ' +
              'on native stack and may not do the GC as expected.');
  return;
}

binding.run();
