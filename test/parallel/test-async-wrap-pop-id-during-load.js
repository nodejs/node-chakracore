'use strict';

const common = require('../common');

if (process.argv[2] === 'async') {
  common.disableCrashOnUnhandledRejection();
  async function fn() {
    fn();
    throw new Error();
  }
  return (async function() { await fn(); })();
}

const assert = require('assert');
const { spawnSync } = require('child_process');

const engineArgs = [];
if (!common.isChakraEngine) {
  engineArgs.push('--stack_size=150');
}

const ret = spawnSync(
  process.execPath,
  [...engineArgs, __filename, 'async']
);
assert.strictEqual(ret.status, 0,
                   `EXIT CODE: ${ret.status}, STDERR:\n${ret.stderr}`);
const stderr = ret.stderr.toString('utf8', 0, 2048);
assert.ok(!/async.*hook/i.test(stderr));
assert.ok(stderr.includes('UnhandledPromiseRejectionWarning: Error'), stderr);
