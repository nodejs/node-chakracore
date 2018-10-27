'use strict';

const common = require('../common.js');

if (process.versions.hasOwnProperty('chakracore')) {
  // tracing events are not supported in node-chakracore
  // however, the benchmark harness fails if it runs a file without a benchmark
  // set up instead of disabling the entire misc benchmark, we can simply shim
  // this benchmark
  const bench = common.createBenchmark(function main({ n }) {
    bench.start();
    while (--n > 0) {
      --n;
    }
    bench.end(n);
  }, {
    n: [100000]
  });
  return;
}

const bench = common.createBenchmark(main, {
  n: [100000],
  method: ['trace', 'isTraceCategoryEnabled']
}, {
  flags: ['--expose-internals', '--trace-event-categories', 'foo']
});

const {
  TRACE_EVENT_PHASE_NESTABLE_ASYNC_BEGIN: kBeforeEvent
} = common.binding('constants').trace;

function doTrace(n, trace) {
  bench.start();
  for (var i = 0; i < n; i++) {
    trace(kBeforeEvent, 'foo', 'test', 0, 'test');
  }
  bench.end(n);
}

function doIsTraceCategoryEnabled(n, isTraceCategoryEnabled) {
  bench.start();
  for (var i = 0; i < n; i++) {
    isTraceCategoryEnabled('foo');
    isTraceCategoryEnabled('bar');
  }
  bench.end(n);
}

function main({ n, method }) {
  const {
    trace,
    isTraceCategoryEnabled
  } = common.binding('trace_events');

  switch (method) {
    case '':
    case 'trace':
      doTrace(n, trace);
      break;
    case 'isTraceCategoryEnabled':
      doIsTraceCategoryEnabled(n, isTraceCategoryEnabled);
      break;
    default:
      throw new Error(`Unexpected method "${method}"`);
  }
}
