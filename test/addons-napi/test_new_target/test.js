'use strict';

const common = require('../../common');
const assert = require('assert');
const binding = require(`./build/${common.buildType}/binding`);

class Class extends binding.BaseClass {
  constructor() {
    super();
    this.method();
  }
  method() {
    this.ok = true;
  }
}

assert.ok(new Class() instanceof binding.BaseClass);
assert.ok(new Class().ok);
assert.ok(new binding.Constructor(binding.Constructor) instanceof binding.Constructor);
assert.ok(binding.OrdinaryFunction());
