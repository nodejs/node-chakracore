'use strict';

const common = require('../common');
const assert = require('assert');
const os = require('os');

const eol = common.isWindows ? '\r\n' : '\n';

assert.strictEqual(os.EOL, eol);

common.expectsError(function() {
  os.EOL = 123;
}, {
  type: TypeError,
  message: common.engineSpecificMessage({
    v8: /^Cannot assign to read only property 'EOL' of object '#<Object>'$/,
    chakracore: /^Assignment to read-only properties is not allowed in strict mode$/
  })
});

const foo = 'foo';
Object.defineProperties(os, {
  EOL: {
    configurable: true,
    enumerable: true,
    writable: false,
    value: foo
  }
});
assert.strictEqual(os.EOL, foo);
