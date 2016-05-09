'use strict';
const common = require('../common');
const assert = require('assert');

var expected_keys = ['ares', 'http_parser', 'modules', 'node',
                     'uv', 'zlib'];
expected_keys.push(process.jsEngine);

if (common.hasCrypto) {
  expected_keys.push('openssl');
}

if (typeof Intl !== 'undefined') {
  expected_keys.push('icu');
}

expected_keys.sort();
const actual_keys = Object.keys(process.versions).sort();

assert.deepStrictEqual(actual_keys, expected_keys);
