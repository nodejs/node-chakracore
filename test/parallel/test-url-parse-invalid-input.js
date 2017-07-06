'use strict';
const common = require('../common');
const assert = require('assert');
const url = require('url');

// https://github.com/joyent/node/issues/568
[
  [undefined, 'undefined'],
  [null, 'null'],
  [true, 'boolean'],
  [false, 'boolean'],
  [0.0, 'number'],
  [0, 'number'],
  [[], 'object'],
  [{}, 'object'],
  [() => {}, 'function'],
  [Symbol('foo'), 'symbol']
].forEach(([val, type]) => {
  const error = common.expectsError({
    code: 'ERR_INVALID_ARG_TYPE',
    type: TypeError,
    message: `The "url" argument must be of type string. Received type ${type}`
  });
  assert.throws(() => { url.parse(val); }, error);
});

const engineSpecificMalformedUrlError =
    common.engineSpecificMessage({
      v8: /^URIError: URI malformed$/,
      chakracore: /^URIError: The URI to be decoded is not a valid encoding$/
    });

assert.throws(() => { url.parse('http://%E0%A4%A@fail'); }, engineSpecificMalformedUrlError);
