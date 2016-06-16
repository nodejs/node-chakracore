'use strict';

const common = require('../common');
const assert = require('assert');
const path = require('path');
const repl = require('repl');
let found = false;

process.on('exit', () => {
  assert.strictEqual(found, true);
});

common.ArrayStream.prototype.write = function(output) {
  if (common.engineSpecificMessage({
    v8: /var foo bar;/,
    chakracore: /Expected ';'/  // chakra does not show source
  }).test(output))
    found = true;
};

const putIn = new common.ArrayStream();
repl.start('', putIn);
let file = path.join(common.fixturesDir, 'syntax', 'bad_syntax');

if (common.isWindows)
  file = file.replace(/\\/g, '\\\\');

putIn.run(['.clear']);
putIn.run([`require('${file}');`]);
