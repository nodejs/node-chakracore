'use strict';

const common = require('../common');

if (common.isChakraEngine) {
  console.log('1..0 # Skipped: This test is disabled for chakra engine ' +
  'because debugger support is not implemented yet.');
  return;
}

const assert = require('assert');
const spawnSync = require('child_process').spawnSync;
const args = [`--debug-brk=${common.PORT}`, '-e', '0'];
const proc = spawnSync(process.execPath, args, {encoding: 'utf8'});
assert(/Debugger listening on/.test(proc.stderr));
