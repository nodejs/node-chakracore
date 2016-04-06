'use strict';
// Flags: --expose-gc

const common = require('../../common');
var binding = require('./build/Release/binding');

if (common.isChakraEngine) {
  console.log(`1..0 # Skipped: This test is disabled for chakra engine
  because it depends on v8 GC behavior. Chakra GC may find reference-like
  data on native stack and may not do the GC as expected.`);
  return;
}

binding.run();
