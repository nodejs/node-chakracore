'use strict';
const common = require('../common');
const path = require('path');
const assert = require('assert');

try {
  require(path.join(common.fixturesDir, 'invalid.json'));
} catch (err) {
  const re = common.engineSpecificMessage({
    v8: /test[/\\]fixtures[/\\]invalid.json: Unexpected string/,
    chakracore:
    /test[/\\]fixtures[/\\]invalid.json: JSON.parse Error: Expected '}'/
  });

  const i = err.message.match(re);
  assert.notStrictEqual(null, i, 'require() json error should include path');
}
