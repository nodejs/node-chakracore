'use strict';

const common = require('../common');
const assert = require('assert');

if (!common.hasCrypto) {
  console.log('1..0 # Skipped: missing crypto');
  return;
}

// Ensure accessing ._external doesn't hit an assert in the accessor method.
const tls = require('tls');
{
  const pctx = tls.createSecureContext().context;
  const cctx = Object.create(pctx);
  assert.throws(() => cctx._external, common.engineSpecificMessage({
    v8 : /incompatible receiver/,
    chakracore : /Illegal invocation/}));
  pctx._external;
}
{
  const pctx = tls.createSecurePair().credentials.context;
  const cctx = Object.create(pctx);
  assert.throws(() => cctx._external, common.engineSpecificMessage({
    v8 : /incompatible receiver/,
    chakracore : /Illegal invocation/}));
  pctx._external;
}
