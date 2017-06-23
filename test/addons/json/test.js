'use strict';

const common = require('../../common');
const assert = require('assert');
const binding = require(`./build/${common.buildType}/binding`);

const obj = { some_key: 'some_value' };
const str = JSON.stringify(obj);

assert.strictEqual(str, binding.stringify(obj));
assert.strictEqual(JSON.stringify(obj, undefined, 5),
                   binding.stringify(obj, 5));
assert.strictEqual('string', typeof binding.stringify(obj));

assert.strictEqual(obj.some_key, binding.parse(str).some_key);
assert.strictEqual('object', typeof binding.parse(str));
