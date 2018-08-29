'use strict';

// Tests below are not from WPT.

const common = require('../common');
const assert = require('assert');
const URLSearchParams = require('url').URLSearchParams;

{
  const params = new URLSearchParams();
  common.expectsError(() => {
    params.set.call(undefined);
  }, {
    code: 'ERR_INVALID_THIS',
    type: TypeError,
    message: 'Value of "this" must be of type URLSearchParams'
  });
  common.expectsError(() => {
    params.set('a');
  }, {
    code: 'ERR_MISSING_ARGS',
    type: TypeError,
    message: 'The "name" and "value" arguments must be specified'
  });

  const obj = {
    toString() { throw new Error('toString'); },
    valueOf() { throw new Error('valueOf'); }
  };
  const sym = Symbol();
  assert.throws(() => params.append(obj, 'b'), /^Error: toString$/);
  assert.throws(() => params.append('a', obj), /^Error: toString$/);
  assert.throws(() => params.append(sym, 'b'),
    common.engineSpecificMessage({
      v8: /^TypeError: Cannot convert a Symbol value to a string$/,
      chakracore: /^TypeError: No implicit conversion of Symbol to String$/
    }));
  assert.throws(() => params.append('a', sym),
    common.engineSpecificMessage({
      v8: /^TypeError: Cannot convert a Symbol value to a string$/,
      chakracore: /^TypeError: No implicit conversion of Symbol to String$/
    }));
}
