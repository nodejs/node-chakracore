'use strict';

const common = require('../common');
const assert = require('assert');
const uv = process.binding('uv');

// Ensures that the `UV_...` values in process.binding('uv')
// are constants.

const keys = Object.keys(uv);
keys.forEach((key) => {
  if (key === 'errname')
    return; // skip this
  const val = uv[key];
  assert.throws(
    () => uv[key] = 1,
    common.engineSpecificMessage({
      v8: /^TypeError: Cannot assign to read only property/,
      chakracore: /^TypeError: Assignment to read-only properties is not allowed in strict mode$/
    })
  );
  assert.strictEqual(uv[key], val);
});
