// Flags: --expose-internals
'use strict';

const common = require('../common');
const assert = require('assert');
const { internalBinding } = require('internal/test/binding');
const uv = internalBinding('uv');

// Ensures that the `UV_...` values in internalBinding('uv')
// are constants.

const keys = Object.keys(uv);
keys.forEach((key) => {
  if (key.startsWith('UV_')) {
    const val = uv[key];
    assert.throws(() => uv[key] = 1,
                  common.engineSpecificMessage({
                    v8: /^TypeError: Cannot assign to read only property/,
                    chakracore: /^TypeError: Assignment to read-only properties is not allowed in strict mode$/
                  })
    );
    assert.strictEqual(uv[key], val);
  }
});
