'use strict';

require('../common');
const assert = require('assert');
const vm = require('vm');

const responses = [];
function saveResult(x) {
  responses.push(x);
}

const p1 = new Promise((resolve, reject) => resolve(1));
p1.then(saveResult).then(() => { saveResult(6); });

const p2 = new Promise((resolve, reject) => reject(2));
p2.catch(saveResult);

const p5 = vm.runInNewContext(`
const p3 = new Promise((resolve, reject) => resolve(3)).then(saveResult);
p3.then(() => { saveResult(7); });
const p4 = new Promise((resolve, reject) => reject(4)).catch(saveResult);
new Promise((resolve, reject) => resolve(5));
`, { saveResult });
p5.then(saveResult);

// Note: using setImmediate here since that is a macrotask and will happen
// after the microtask/promise queue is completely drained.
setImmediate(() => {
  console.log(responses);
  assert.strictEqual(responses.length, 7);
  assert.deepStrictEqual(responses, [1, 2, 3, 4, 5, 6, 7]);
});
