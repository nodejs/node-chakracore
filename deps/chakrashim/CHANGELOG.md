# ChakraShim ChangeLog

## 2018-07-20, node-chakracore-v10.6.0

### Commits

* [[`9df1f424d3`](https://github.com/nodejs/node-chakracore/commit/9df1f424d3)] - 2018-07-20, Node-ChakraCore Version 10.6.0
* [[`7fb22f81c9`](https://github.com/nodejs/node-chakracore/commit/7fb22f81c9)] - deps: update ChakraCore to Microsoft/ChakraCore@0a1d70bb3c
* [[`d5de45d13c`](https://github.com/nodejs/node-chakracore/commit/d5de45d13c)] - deps: update ChakraCore to Microsoft/ChakraCore@1fc4e1f08b
* [[`07558fadfb`](https://github.com/nodejs/node-chakracore/commit/07558fadfb)] - deps: update ChakraCore to Microsoft/ChakraCore@2d76ace493
* [[`07be0fb296`](https://github.com/nodejs/node-chakracore/commit/07be0fb296)] - deps: update ChakraCore to Microsoft/ChakraCore@7aa8981fb0
* [[`509140e0c1`](https://github.com/nodejs/node-chakracore/commit/509140e0c1)] - deps: update ChakraCore to Microsoft/ChakraCore@6b5190f3c9
* [[`eee4a7c5fe`](https://github.com/nodejs/node-chakracore/commit/eee4a7c5fe)] - deps: update ChakraCore to Microsoft/ChakraCore@d7e19fd33d
* [[`f49f2f901a`](https://github.com/nodejs/node-chakracore/commit/f49f2f901a)] - deps: update ChakraCore to Microsoft/ChakraCore@841036aeaa
* [[`6957b448ed`](https://github.com/nodejs/node-chakracore/commit/6957b448ed)] - deps: update ChakraCore to Microsoft/ChakraCore@4374e4407b
* [[`ad7ffc64e1`](https://github.com/nodejs/node-chakracore/commit/ad7ffc64e1)] - deps: update ChakraCore to Microsoft/ChakraCore@88b1be47d2
* [[`5b6d604c45`](https://github.com/nodejs/node-chakracore/commit/5b6d604c45)] - deps: update ChakraCore to Microsoft/ChakraCore@aa4a91e5bb
* [[`0b55feddfd`](https://github.com/nodejs/node-chakracore/commit/0b55feddfd)] - deps: update ChakraCore to Microsoft/ChakraCore@b75a7801f4
* [[`6453a6c015`](https://github.com/nodejs/node-chakracore/commit/6453a6c015)] - deps: update ChakraCore to Microsoft/ChakraCore@9fdb17497c
* [[`4dc944dcea`](https://github.com/nodejs/node-chakracore/commit/4dc944dcea)] - deps: update ChakraCore to Microsoft/ChakraCore@d439708b39
* [[`1c627812f9`](https://github.com/nodejs/node-chakracore/commit/1c627812f9)] - deps: update ChakraCore to Microsoft/ChakraCore@5d30c529d3
* [[`2a19f01820`](https://github.com/nodejs/node-chakracore/commit/2a19f01820)] - deps: update ChakraCore to Microsoft/ChakraCore@2f088a6b07
* [[`5a5a284118`](https://github.com/nodejs/node-chakracore/commit/5a5a284118)] - deps: update ChakraCore to Microsoft/ChakraCore@2bd57e547a
* [[`9c6af757e5`](https://github.com/nodejs/node-chakracore/commit/9c6af757e5)] - deps: update ChakraCore to Microsoft/ChakraCore@78bdc0a31f
* [[`719274b5f7`](https://github.com/nodejs/node-chakracore/commit/719274b5f7)] - deps: update ChakraCore to Microsoft/ChakraCore@05caa1e323
* [[`b20cd15cfb`](https://github.com/nodejs/node-chakracore/commit/b20cd15cfb)] - deps: update ChakraCore to Microsoft/ChakraCore@f023b770a6
* [[`137db2ce9e`](https://github.com/nodejs/node-chakracore/commit/137db2ce9e)] - test: update symbol error regex
* [[`44c74500c9`](https://github.com/nodejs/node-chakracore/commit/44c74500c9)] - test: enable `Promise.finally` tests
* [[`7d75c1c6bd`](https://github.com/nodejs/node-chakracore/commit/7d75c1c6bd)] - test: update Symbol error message
* [[`ef12872fff`](https://github.com/nodejs/node-chakracore/commit/ef12872fff)] - chakrashim: fix for context leak
* [[`fc8c38ad78`](https://github.com/nodejs/node-chakracore/commit/fc8c38ad78)] - deps: update ChakraCore to Microsoft/ChakraCore@288d7fec0d
* [[`4a65d0aaac`](https://github.com/nodejs/node-chakracore/commit/4a65d0aaac)] - chakrashim: update to ChakraCore v1.10.1
* [[`38cb5296a5`](https://github.com/nodejs/node-chakracore/commit/38cb5296a5)] - build,lib,test: fix merge issues
* [[`f5d29d5ec3`](https://github.com/nodejs/node-chakracore/commit/f5d29d5ec3)] - src,test: disable `per_context.js` shim
* [[`3b49f286c9`](https://github.com/nodejs/node-chakracore/commit/3b49f286c9)] - test: skip failing tests for ChakraCore
* [[`43583d9cc6`](https://github.com/nodejs/node-chakracore/commit/43583d9cc6)] - test: skip worker and modules tests
* [[`c57af9036f`](https://github.com/nodejs/node-chakracore/commit/c57af9036f)] - n-api: implement threadsafe for ChakraCore
* [[`59597c21d1`](https://github.com/nodejs/node-chakracore/commit/59597c21d1)] - deps: update ChakraCore to Microsoft/ChakraCore@9c6f4fbcc8
* [[`596e857f6c`](https://github.com/nodejs/node-chakracore/commit/596e857f6c)] - chakrashim: fix lint errors
* [[`10dfbbe01a`](https://github.com/nodejs/node-chakracore/commit/10dfbbe01a)] - deps: update ChakraCore to Microsoft/ChakraCore@5958a4204f
* [[`6e80e81442`](https://github.com/nodejs/node-chakracore/commit/6e80e81442)] - fixing bug in shim where Compile wouldn't work correctly if source had null for a resourceName
* [[`ad2be22454`](https://github.com/nodejs/node-chakracore/commit/ad2be22454)] - stubbing out new chakra shim API, disabling impacted test
* [[`f4fb937ddc`](https://github.com/nodejs/node-chakracore/commit/f4fb937ddc)] - node is now using a V8 API to pass a SideEffectType to Object::SetAccessor.  Updated shim to APIs to accept this param.  Implementation ignores it.
* [[`3cb68f98a7`](https://github.com/nodejs/node-chakracore/commit/3cb68f98a7)] - Merge tag 'v10.6.0' into 10.6.0-proposal
* [[`422c132e6e`](https://github.com/nodejs/node-chakracore/commit/422c132e6e)] - tools: replace object spread with assign
* [[`ed009ce32a`](https://github.com/nodejs/node-chakracore/commit/ed009ce32a)] - Skip tests that started using 'finally' as of e5a3579a50
* [[`41a3649cd4`](https://github.com/nodejs/node-chakracore/commit/41a3649cd4)] - chakrashim: add initial shim of BigUint64Array
* [[`65930d99e7`](https://github.com/nodejs/node-chakracore/commit/65930d99e7)] - test: disabling new esmodule test
* [[`f6b67fbbe3`](https://github.com/nodejs/node-chakracore/commit/f6b67fbbe3)] - test: disable v8-specific test for node-chakracore
* [[`02721d7678`](https://github.com/nodejs/node-chakracore/commit/02721d7678)] - test: remove catch without argument binding and disable failing test cases
* [[`0497d61f43`](https://github.com/nodejs/node-chakracore/commit/0497d61f43)] - test: disable test-worker-workerdata-sharedarraybuffer under node-chakracore
* [[`922ab08826`](https://github.com/nodejs/node-chakracore/commit/922ab08826)] - test: disable worker_threads and serdes tests for chakracore
* [[`8f4dcd0cf3`](https://github.com/nodejs/node-chakracore/commit/8f4dcd0cf3)] - Merge tag 'v10.5.0' into 10.5.0-proposal
* [[`ea169fc628`](https://github.com/nodejs/node-chakracore/commit/ea169fc628)] - Merge tag 'v10.4.1' into 10.4.1-proposal
* [[`005ff3b9d1`](https://github.com/nodejs/node-chakracore/commit/005ff3b9d1)] - Skip tests that started using 'finally' as of e5a3579a50
* [[`4e8953c765`](https://github.com/nodejs/node-chakracore/commit/4e8953c765)] - test: disable BigInt tests in test-util-types for chakracore
* [[`580582a3c9`](https://github.com/nodejs/node-chakracore/commit/580582a3c9)] - Merge tag 'v10.4.0' into 10.4.0-proposal
* [[`9f3b677d1d`](https://github.com/nodejs/node-chakracore/commit/9f3b677d1d)] - test: fix/skip tests for ChakraCore
* [[`1d30d496d0`](https://github.com/nodejs/node-chakracore/commit/1d30d496d0)] - Merge tag 'v10.3.0' into 10.3.0-proposal
* [[`b5e8b239bb`](https://github.com/nodejs/node-chakracore/commit/b5e8b239bb)] - Merge tag 'v10.2.1' into 10.2.1-proposal
* [[`8e908ba600`](https://github.com/nodejs/node-chakracore/commit/8e908ba600)] - deps: ensure that uv handles are cleaned up before freeing them
* [[`3d962af570`](https://github.com/nodejs/node-chakracore/commit/3d962af570)] - test: fix test for ChakraCore
* [[`5e61b49414`](https://github.com/nodejs/node-chakracore/commit/5e61b49414)] - updating test baseline
* [[`8bd7b0efa5`](https://github.com/nodejs/node-chakracore/commit/8bd7b0efa5)] - chakrashim: fix lint errors
* [[`82b66c38af`](https://github.com/nodejs/node-chakracore/commit/82b66c38af)] - napi: apply null-init check to JSRT too
* [[`04db466ccc`](https://github.com/nodejs/node-chakracore/commit/04db466ccc)] - n-api: add cleanup hook APIs for JSRT
* [[`4cbe08afb5`](https://github.com/nodejs/node-chakracore/commit/4cbe08afb5)] - deps: fixing xplat build break
* [[`f850fc0764`](https://github.com/nodejs/node-chakracore/commit/f850fc0764)] - test: stub BigUint64Array in Javascript and disable a bigint test in node-chakracore
* [[`9868c4ad0e`](https://github.com/nodejs/node-chakracore/commit/9868c4ad0e)] - chakrashim: add initial shim of BigUint64Array
* [[`b5d2f8699a`](https://github.com/nodejs/node-chakracore/commit/b5d2f8699a)] - deps: update ChakraCore to Microsoft/ChakraCore@4707cbfa15
* [[`e1fb833094`](https://github.com/nodejs/node-chakracore/commit/e1fb833094)] - lint: fix left/right leaning pointer errors
* [[`670145ad31`](https://github.com/nodejs/node-chakracore/commit/670145ad31)] - worker_threads, chakrashim: fix build breaks caused by addition of worker_threads
* [[`34d250e18c`](https://github.com/nodejs/node-chakracore/commit/34d250e18c)] - chakrashim: mark Local<T>::As as const
* [[`ac129ff1a1`](https://github.com/nodejs/node-chakracore/commit/ac129ff1a1)] - build,chakrashim: fix build breaks after V8 update
* [[`3fc5f19f10`](https://github.com/nodejs/node-chakracore/commit/3fc5f19f10)] - test: mark flaky tests
* [[`360a28bc27`](https://github.com/nodejs/node-chakracore/commit/360a28bc27)] - test: skip failing tests for ChakraCore
* [[`45721abf45`](https://github.com/nodejs/node-chakracore/commit/45721abf45)] - lib, test: fix ChakraCore test failures
* [[`7e50d8dc42`](https://github.com/nodejs/node-chakracore/commit/7e50d8dc42)] - Merge tag 'v10.2.0' into v10.x
* [[`166867dc99`](https://github.com/nodejs/node-chakracore/commit/166867dc99)] - Working on 10.1.1

## 2018-05-23, node-chakracore-v10.1.0

### Commits

* [[`f49890ed8c`](https://github.com/nodejs/node-chakracore/commit/f49890ed8c)] - 2018-05-24, Node-ChakraCore Version 10.1.0
* [[`b612f0e9a4`](https://github.com/nodejs/node-chakracore/commit/b612f0e9a4)] - lib,src: enable TTD auto-trace config
* [[`b621e3c91d`](https://github.com/nodejs/node-chakracore/commit/b621e3c91d)] - deps, test, lib: fix ChakraCore build and test
* [[`7fc38651e9`](https://github.com/nodejs/node-chakracore/commit/7fc38651e9)] - deps: update ChakraCore to Microsoft/ChakraCore@1b469e2352
* [[`a5e48bffa3`](https://github.com/nodejs/node-chakracore/commit/a5e48bffa3)] - chakrashim,test: fixes from merge
* [[`f8f6b8e077`](https://github.com/nodejs/node-chakracore/commit/f8f6b8e077)] - deps,src: workaround vm.runInContext issue with top level "var"
* [[`7a711e0faa`](https://github.com/nodejs/node-chakracore/commit/7a711e0faa)] - deps: fixing microtask behavior with multiple contexts
* [[`7bc70fb4ab`](https://github.com/nodejs/node-chakracore/commit/7bc70fb4ab)] - chakrashim: fix lint errors
* [[`e5d697976c`](https://github.com/nodejs/node-chakracore/commit/e5d697976c)] - deps: update ChakraCore to Microsoft/ChakraCore@fe4d653dc8
* [[`55ba49826b`](https://github.com/nodejs/node-chakracore/commit/55ba49826b)] - chakrashim,test: fix Promise API compatibility
* [[`606e73eacb`](https://github.com/nodejs/node-chakracore/commit/606e73eacb)] - deps: update ChakraCore to Microsoft/ChakraCore@9c82675e54
* [[`a9f5ca4c5a`](https://github.com/nodejs/node-chakracore/commit/a9f5ca4c5a)] - deps: update ChakraCore to Microsoft/ChakraCore@1c76177a11
* [[`079aaadfa2`](https://github.com/nodejs/node-chakracore/commit/079aaadfa2)] - fixing linter errors in test
* [[`8854f00a57`](https://github.com/nodejs/node-chakracore/commit/8854f00a57)] - fixing broken test
* [[`6bc097e6ad`](https://github.com/nodejs/node-chakracore/commit/6bc097e6ad)] - test: disable tests on node-chakracore
* [[`66b5c1c47f`](https://github.com/nodejs/node-chakracore/commit/66b5c1c47f)] - napi: add napi_module_register_by_symbol to node_api_jsrt.cc
* [[`ba1662603d`](https://github.com/nodejs/node-chakracore/commit/ba1662603d)] - lib: fix call stack extraction in trace_mgr.js
* [[`043c068950`](https://github.com/nodejs/node-chakracore/commit/043c068950)] - updating ChakraShim to account for new params in FunctionTemplate::New
* [[`a132dd8d2c`](https://github.com/nodejs/node-chakracore/commit/a132dd8d2c)] - deps: update ChakraCore to Microsoft/ChakraCore@2c6efb563b
* [[`29af11731d`](https://github.com/nodejs/node-chakracore/commit/29af11731d)] - deps: update ChakraCore to Microsoft/ChakraCore@865f63bd26
* [[`9ea0323bc5`](https://github.com/nodejs/node-chakracore/commit/9ea0323bc5)] - deps: fixing additional invalid uses of HandleScope
* [[`b54fd8c090`](https://github.com/nodejs/node-chakracore/commit/b54fd8c090)] - deps: update ChakraCore to Microsoft/ChakraCore@08cf998b1d
* [[`51b6c6bd01`](https://github.com/nodejs/node-chakracore/commit/51b6c6bd01)] - deps: updating incorrect usage of HandleScope
* [[`e04616ca17`](https://github.com/nodejs/node-chakracore/commit/e04616ca17)] - chakrashim: implement proxy methods in chakrashim
* [[`2cd04857e8`](https://github.com/nodejs/node-chakracore/commit/2cd04857e8)] - test: using v8::Context::Scope in cctest EnvironmentTestFixture
* [[`68c92c1a06`](https://github.com/nodejs/node-chakracore/commit/68c92c1a06)] - build: fix chakracore dependency
* [[`8b7991f82d`](https://github.com/nodejs/node-chakracore/commit/8b7991f82d)] - Merge tag 'v10.1.0' into 10.1.0-proposal
* [[`022021833d`](https://github.com/nodejs/node-chakracore/commit/022021833d)] - Working on v10.0.1

## 2018-04-24, node-chakracore-v10.0.0

### Commits

* [[`e3b49f0f99`](https://github.com/nodejs/node-chakracore/commit/e3b49f0f99)] - 2018-04-24, Node-ChakraCore Version 10.0.0
* [[`069a85aa7a`](https://github.com/nodejs/node-chakracore/commit/069a85aa7a)] - Merge tag 'v10.0.0' into v10.x
* [[`0a9fa79c63`](https://github.com/nodejs/node-chakracore/commit/0a9fa79c63)] - src,test: fix V8 engine issues
* [[`eff6d3715d`](https://github.com/nodejs/node-chakracore/commit/eff6d3715d)] - Merge remote-tracking branch 'upstream/v10.x' into v10.x
* [[`88689b5f70`](https://github.com/nodejs/node-chakracore/commit/88689b5f70)] - Merge remote-tracking branch 'upstream/v10.x' into v10.x
* [[`d81f175295`](https://github.com/nodejs/node-chakracore/commit/d81f175295)] - test: skip win32 arm tests for chakracore
* [[`d8c89ceeef`](https://github.com/nodejs/node-chakracore/commit/d8c89ceeef)] - deps: fix libuv registry API error handling
* [[`f82e85b02b`](https://github.com/nodejs/node-chakracore/commit/f82e85b02b)] - lib: make sure `console` is writable
* [[`c68529ec5e`](https://github.com/nodejs/node-chakracore/commit/c68529ec5e)] - deps: update ChakraCore to Microsoft/ChakraCore@3003fadd74
* [[`a5cb4e2a69`](https://github.com/nodejs/node-chakracore/commit/a5cb4e2a69)] - Merge remote-tracking branch 'upstream/v10.x' into v10.x
* [[`e75d19ae4a`](https://github.com/nodejs/node-chakracore/commit/e75d19ae4a)] - Merge remote-tracking branch 'upstream/v10.x' into v10.x
* [[`72598bbb73`](https://github.com/nodejs/node-chakracore/commit/72598bbb73)] - deps: allow openssl to build for WOA
* [[`296a1e17fe`](https://github.com/nodejs/node-chakracore/commit/296a1e17fe)] - test: update tests for ChakraCore
* [[`fad93d074f`](https://github.com/nodejs/node-chakracore/commit/fad93d074f)] - test: fix error message dependency in test-errors-systemerror
* [[`16f3b88efd`](https://github.com/nodejs/node-chakracore/commit/16f3b88efd)] - test: disable test for util.types.isBig(U)Int64Array
* [[`466c003594`](https://github.com/nodejs/node-chakracore/commit/466c003594)] - deps: update ChakraCore to Microsoft/ChakraCore@1ad224f6df
* [[`46966a3ffb`](https://github.com/nodejs/node-chakracore/commit/46966a3ffb)] - chakrashim,src: fix build breaks
* [[`9bd98343e7`](https://github.com/nodejs/node-chakracore/commit/9bd98343e7)] - Merge remote-tracking branch 'upstream/v10.x' into v10.x
* [[`42fe0fe928`](https://github.com/nodejs/node-chakracore/commit/42fe0fe928)] - chakrashim,src: fix lint issues
* [[`0a98060ed9`](https://github.com/nodejs/node-chakracore/commit/0a98060ed9)] - Merge remote-tracking branch 'upstream/v10.x' into v10.x
* [[`8b9997ab16`](https://github.com/nodejs/node-chakracore/commit/8b9997ab16)] - test: disable new es-module test
* [[`d5d61a50a8`](https://github.com/nodejs/node-chakracore/commit/d5d61a50a8)] - chakrashim: update Error.prepareStackTrace shim
* [[`06d2b9c369`](https://github.com/nodejs/node-chakracore/commit/06d2b9c369)] - build, win: allow vcbuild.bat to continue if NASM isn't installed
* [[`ff5e8e7c28`](https://github.com/nodejs/node-chakracore/commit/ff5e8e7c28)] - meta: merge node/master into node-chakracore/master
* [[`5cb4d54d07`](https://github.com/nodejs/node-chakracore/commit/5cb4d54d07)] - deps: update ChakraCore to Microsoft/ChakraCore@495de1a75c
* [[`ba4ef5988f`](https://github.com/nodejs/node-chakracore/commit/ba4ef5988f)] - meta: merge node/master into node-chakracore/master
* [[`a118983ac4`](https://github.com/nodejs/node-chakracore/commit/a118983ac4)] - meta: merge node/master into node-chakracore/master
* [[`29d44c2857`](https://github.com/nodejs/node-chakracore/commit/29d44c2857)] - deps: update ChakraCore to Microsoft/ChakraCore@7f1ab68f0a
* [[`b3e69f36f2`](https://github.com/nodejs/node-chakracore/commit/b3e69f36f2)] - meta: merge node/master into node-chakracore/master
* [[`b165ac7695`](https://github.com/nodejs/node-chakracore/commit/b165ac7695)] - meta: merge node/master into node-chakracore/master
* [[`ed292311f2`](https://github.com/nodejs/node-chakracore/commit/ed292311f2)] - chakrashim,lib,test: fix lint errors
* [[`27685b5a55`](https://github.com/nodejs/node-chakracore/commit/27685b5a55)] - test: fix `test-cli-syntax`
* [[`2206891db6`](https://github.com/nodejs/node-chakracore/commit/2206891db6)] - meta: merge node/master into node-chakracore/master
* [[`f63ed7ca34`](https://github.com/nodejs/node-chakracore/commit/f63ed7ca34)] - chakrashim: add missing shim API
* [[`46bf1742a7`](https://github.com/nodejs/node-chakracore/commit/46bf1742a7)] - meta: merge node/master into node-chakracore/master
* [[`c3f4ab4b1a`](https://github.com/nodejs/node-chakracore/commit/c3f4ab4b1a)] - test: update message baselines
* [[`28b523e70c`](https://github.com/nodejs/node-chakracore/commit/28b523e70c)] - meta: merge node/master into node-chakracore/master
* [[`f92080cfe6`](https://github.com/nodejs/node-chakracore/commit/f92080cfe6)] - meta: merge node/master into node-chakracore/master
* [[`d33d37449f`](https://github.com/nodejs/node-chakracore/commit/d33d37449f)] - test: skip new tests
* [[`acbfb49b38`](https://github.com/nodejs/node-chakracore/commit/acbfb49b38)] - meta: merge node/master into node-chakracore/master Merge 141be923f3 as of 2018-04-01 This commit was automatically generated. For any problems, please contact jackhorton Reviewed-By: Kyle Farnung <kfarnung@microsoft.com>
* [[`96ed842953`](https://github.com/nodejs/node-chakracore/commit/96ed842953)] - deps: update ChakraCore to Microsoft/ChakraCore@8e52453b89
* [[`fd3229c253`](https://github.com/nodejs/node-chakracore/commit/fd3229c253)] - test: disabling tests failing on node-chakracore
* [[`5685d4b41b`](https://github.com/nodejs/node-chakracore/commit/5685d4b41b)] - meta: merge node/master into node-chakracore/master
* [[`1bbf743788`](https://github.com/nodejs/node-chakracore/commit/1bbf743788)] - meta: merge node/master into node-chakracore/master
* [[`1fc54789d9`](https://github.com/nodejs/node-chakracore/commit/1fc54789d9)] - meta: merge node/master into node-chakracore/master
* [[`db8f126c61`](https://github.com/nodejs/node-chakracore/commit/db8f126c61)] - disabling failing test
* [[`e785a77b31`](https://github.com/nodejs/node-chakracore/commit/e785a77b31)] - meta: merge node/master into node-chakracore/master
* [[`60ed062444`](https://github.com/nodejs/node-chakracore/commit/60ed062444)] - meta: merge node/master into node-chakracore/master
* [[`c073aa35e5`](https://github.com/nodejs/node-chakracore/commit/c073aa35e5)] - updating test to account for difference in error name for chakra's stack-overflow errors
* [[`2b23e70689`](https://github.com/nodejs/node-chakracore/commit/2b23e70689)] - fixing lib/error isStackOverflowError to work on node-chakracore
* [[`398f3396aa`](https://github.com/nodejs/node-chakracore/commit/398f3396aa)] - meta: merge node/master into node-chakracore/master
* [[`857281b552`](https://github.com/nodejs/node-chakracore/commit/857281b552)] - test: re-enable assert_throws_stack tests for node-chakracore (#504)
* [[`adee1d52c4`](https://github.com/nodejs/node-chakracore/commit/adee1d52c4)] - fixing build break
* [[`4e8b529fc4`](https://github.com/nodejs/node-chakracore/commit/4e8b529fc4)] - disabling failing test
* [[`da6018d98c`](https://github.com/nodejs/node-chakracore/commit/da6018d98c)] - fixing node-chakracore after taking integration
* [[`fc85d8cf4b`](https://github.com/nodejs/node-chakracore/commit/fc85d8cf4b)] - meta: merge node/master into node-chakracore/master
* [[`d50069c383`](https://github.com/nodejs/node-chakracore/commit/d50069c383)] - chakrashim: add warning for ignored engine flags
* [[`bdb2130646`](https://github.com/nodejs/node-chakracore/commit/bdb2130646)] - lib: disable eslint rule in trace_mgr.js
* [[`038d65bc93`](https://github.com/nodejs/node-chakracore/commit/038d65bc93)] - meta: merge node/master into node-chakracore/master
* [[`631b176797`](https://github.com/nodejs/node-chakracore/commit/631b176797)] - chakrashim: shim missing String constructors
* [[`39281f6056`](https://github.com/nodejs/node-chakracore/commit/39281f6056)] - meta: merge node/master into node-chakracore/master
* [[`7cfa11f768`](https://github.com/nodejs/node-chakracore/commit/7cfa11f768)] - meta: merge node/master into node-chakracore/master
* [[`bd169e3344`](https://github.com/nodejs/node-chakracore/commit/bd169e3344)] - chakrashim,n-api: Add `napi_fatal_exception`
* [[`cdaf0d4604`](https://github.com/nodejs/node-chakracore/commit/cdaf0d4604)] - meta: merge node/master into node-chakracore/master
* [[`3036f13a8c`](https://github.com/nodejs/node-chakracore/commit/3036f13a8c)] - lib,test: stop using `process.jsEngine`
* [[`d2ac2eea18`](https://github.com/nodejs/node-chakracore/commit/d2ac2eea18)] - meta: merge node/master into node-chakracore/master
* [[`df730ba74f`](https://github.com/nodejs/node-chakracore/commit/df730ba74f)] - meta: merge node/master into node-chakracore/master
* [[`dd99ab23c9`](https://github.com/nodejs/node-chakracore/commit/dd99ab23c9)] - test: flaky test `test-async-hooks-destroy-on-gc`
* [[`8946e05758`](https://github.com/nodejs/node-chakracore/commit/8946e05758)] - meta: merge node/master into node-chakracore/master
* [[`aadf3a8941`](https://github.com/nodejs/node-chakracore/commit/aadf3a8941)] - n-api: `napi_make_callback` error handling
* [[`b7d8e8fe02`](https://github.com/nodejs/node-chakracore/commit/b7d8e8fe02)] - meta: merge node/master into node-chakracore/master
* [[`5a55cfbbe9`](https://github.com/nodejs/node-chakracore/commit/5a55cfbbe9)] - test: skip new trace events test
* [[`5bcc9e09ba`](https://github.com/nodejs/node-chakracore/commit/5bcc9e09ba)] - meta: merge node/master into node-chakracore/master
* [[`d3fc4b2d97`](https://github.com/nodejs/node-chakracore/commit/d3fc4b2d97)] - test: chakracore test updates
* [[`4afb088a63`](https://github.com/nodejs/node-chakracore/commit/4afb088a63)] - meta: merge node/master into node-chakracore/master
* [[`ca4423c882`](https://github.com/nodejs/node-chakracore/commit/ca4423c882)] - meta: merge node/master into node-chakracore/master
* [[`feaf114bcd`](https://github.com/nodejs/node-chakracore/commit/feaf114bcd)] - test: skip new failing tests
* [[`398a1ea6da`](https://github.com/nodejs/node-chakracore/commit/398a1ea6da)] - chakrashim: shim new API calls
* [[`ced04719c4`](https://github.com/nodejs/node-chakracore/commit/ced04719c4)] - meta: merge node/master into node-chakracore/master
* [[`ae9a06bece`](https://github.com/nodejs/node-chakracore/commit/ae9a06bece)] - meta: merge node/master into node-chakracore/master
* [[`4adb21cf33`](https://github.com/nodejs/node-chakracore/commit/4adb21cf33)] - meta: merge node/master into node-chakracore/master
* [[`92fd7b1faf`](https://github.com/nodejs/node-chakracore/commit/92fd7b1faf)] - Fix unit test failures
* [[`79b68f06e3`](https://github.com/nodejs/node-chakracore/commit/79b68f06e3)] - meta: merge node/master into node-chakracore/master
* [[`dfc0ee2e8b`](https://github.com/nodejs/node-chakracore/commit/dfc0ee2e8b)] - deps: update ChakraCore to Microsoft/ChakraCore@bfbcfa5b9e
* [[`929d28b282`](https://github.com/nodejs/node-chakracore/commit/929d28b282)] - deps: update ChakraCore to Microsoft/ChakraCore@1b527d50fb
* [[`50a2a45314`](https://github.com/nodejs/node-chakracore/commit/50a2a45314)] - deps: update ChakraCore to Microsoft/ChakraCore@f882a20ce8
* [[`058485bf0f`](https://github.com/nodejs/node-chakracore/commit/058485bf0f)] - deps: update ChakraCore to Microsoft/ChakraCore@8b56bb5028
* [[`f7e7601fae`](https://github.com/nodejs/node-chakracore/commit/f7e7601fae)] - deps: update ChakraCore to Microsoft/ChakraCore@6861888a15
* [[`f78db61b2f`](https://github.com/nodejs/node-chakracore/commit/f78db61b2f)] - meta: merge node/master into node-chakracore/master
* [[`f2d4209cb0`](https://github.com/nodejs/node-chakracore/commit/f2d4209cb0)] - deps: update ChakraCore to Microsoft/ChakraCore@f58e17fd23
* [[`2b4f30801d`](https://github.com/nodejs/node-chakracore/commit/2b4f30801d)] - n-api,test: add int64 test and fix JSRT
* [[`d67ed11778`](https://github.com/nodejs/node-chakracore/commit/d67ed11778)] - chakrashim: fixing build warnings in clang
* [[`83122187ce`](https://github.com/nodejs/node-chakracore/commit/83122187ce)] - test: update inspect Promise output
* [[`8fcb1b43cc`](https://github.com/nodejs/node-chakracore/commit/8fcb1b43cc)] - meta: merge node/master into node-chakracore/master
* [[`ff3d26b481`](https://github.com/nodejs/node-chakracore/commit/ff3d26b481)] - test: update test to look for less specific error message
* [[`f480a95d71`](https://github.com/nodejs/node-chakracore/commit/f480a95d71)] - meta: merge node/master into node-chakracore/master
* [[`bbcf831fa2`](https://github.com/nodejs/node-chakracore/commit/bbcf831fa2)] - lib: removing contents of v8 module under chakracore
* [[`17b469b3f0`](https://github.com/nodejs/node-chakracore/commit/17b469b3f0)] - test: prefer `common.isChakraEngine`
* [[`b27fa80fdc`](https://github.com/nodejs/node-chakracore/commit/b27fa80fdc)] - src: update `napi_get_new_target` to match docs
* [[`4b99e65f41`](https://github.com/nodejs/node-chakracore/commit/4b99e65f41)] - chakrashim: add v8::ScriptCompiler::CreateCodeCache
* [[`91a848ffd7`](https://github.com/nodejs/node-chakracore/commit/91a848ffd7)] - meta: merge node/master into node-chakracore/master
* [[`369f7b9baa`](https://github.com/nodejs/node-chakracore/commit/369f7b9baa)] - test: fix tests for Node-ChakraCore
* [[`57f7bf0bf7`](https://github.com/nodejs/node-chakracore/commit/57f7bf0bf7)] - meta: merge node/master into node-chakracore/master
* [[`678fffbe25`](https://github.com/nodejs/node-chakracore/commit/678fffbe25)] - lib: fix comment typo
* [[`ec7245944a`](https://github.com/nodejs/node-chakracore/commit/ec7245944a)] - test: remove fixed tests
* [[`887e882f7b`](https://github.com/nodejs/node-chakracore/commit/887e882f7b)] - chakrashim: Fix Error.captureStackTrace
* [[`76baebd632`](https://github.com/nodejs/node-chakracore/commit/76baebd632)] - lint: fix lint issues from merge
* [[`2d7142cf80`](https://github.com/nodejs/node-chakracore/commit/2d7142cf80)] - test: make events-uncaught-exception-stack engine-agnostic
* [[`3ae2dfc711`](https://github.com/nodejs/node-chakracore/commit/3ae2dfc711)] - test: partially skip test-repl-underscore in Node-ChakraCore
* [[`1e9ba647ac`](https://github.com/nodejs/node-chakracore/commit/1e9ba647ac)] - test: fix util.types test for Node-ChakraCore
* [[`597adc180d`](https://github.com/nodejs/node-chakracore/commit/597adc180d)] - test: disable unsupported tests in Node-ChakraCore
* [[`df18390c27`](https://github.com/nodejs/node-chakracore/commit/df18390c27)] - chakrashim: add newly-used V8 functions
* [[`11954ba695`](https://github.com/nodejs/node-chakracore/commit/11954ba695)] - meta: merge node/master into node-chakracore/master
* [[`591e9575a6`](https://github.com/nodejs/node-chakracore/commit/591e9575a6)] - deps: update ChakraCore to Microsoft/ChakraCore@7cb85ae5e4
* [[`89fd1836ab`](https://github.com/nodejs/node-chakracore/commit/89fd1836ab)] - meta: merge node/master into node-chakracore/master
* [[`33d99e3868`](https://github.com/nodejs/node-chakracore/commit/33d99e3868)] - deps: update ChakraCore to Microsoft/ChakraCore@1aee42c0ef
* [[`979e36aa59`](https://github.com/nodejs/node-chakracore/commit/979e36aa59)] - test: update new tests to work with Node-ChakraCore
* [[`38d2724f02`](https://github.com/nodejs/node-chakracore/commit/38d2724f02)] - meta: merge node/master into node-chakracore/master
* [[`c48e29d969`](https://github.com/nodejs/node-chakracore/commit/c48e29d969)] - meta: merge node/master into node-chakracore/master
* [[`ed26d5419c`](https://github.com/nodejs/node-chakracore/commit/ed26d5419c)] - meta: merge node/master into node-chakracore/master
* [[`cea4e97406`](https://github.com/nodejs/node-chakracore/commit/cea4e97406)] - test: skip a new trace events test
* [[`951c8350a7`](https://github.com/nodejs/node-chakracore/commit/951c8350a7)] - meta: merge node/master into node-chakracore/master
* [[`2552d1c7a2`](https://github.com/nodejs/node-chakracore/commit/2552d1c7a2)] - doc: fix Markdown linter failures
* [[`2fb4e2892c`](https://github.com/nodejs/node-chakracore/commit/2fb4e2892c)] - chakrashim: switch `var` to `let` and `const`
* [[`5edf74d96a`](https://github.com/nodejs/node-chakracore/commit/5edf74d96a)] - lib: update TTD to use `let` and `const`
* [[`89ea517230`](https://github.com/nodejs/node-chakracore/commit/89ea517230)] - meta: merge node/master into node-chakracore/master
* [[`fece3dd4a7`](https://github.com/nodejs/node-chakracore/commit/fece3dd4a7)] - meta: merge node/master into node-chakracore/master
* [[`b77530504e`](https://github.com/nodejs/node-chakracore/commit/b77530504e)] - test: mark a flaky test
* [[`8d6c6f5b5b`](https://github.com/nodejs/node-chakracore/commit/8d6c6f5b5b)] - fs: skip symlink testing on Windows
* [[`7de203d275`](https://github.com/nodejs/node-chakracore/commit/7de203d275)] - meta: merge node/master into node-chakracore/master
* [[`95382cc791`](https://github.com/nodejs/node-chakracore/commit/95382cc791)] - meta: merge node/master into node-chakracore/master
* [[`4b42bea234`](https://github.com/nodejs/node-chakracore/commit/4b42bea234)] - chakrashim: fix lint issues
* [[`080e431be0`](https://github.com/nodejs/node-chakracore/commit/080e431be0)] - lib: fix lint issues
* [[`0aa918fdba`](https://github.com/nodejs/node-chakracore/commit/0aa918fdba)] - test: remove chakracore condition
* [[`ddad39c07b`](https://github.com/nodejs/node-chakracore/commit/ddad39c07b)] - meta: merge node/master into node-chakracore/master
* [[`f2952070f3`](https://github.com/nodejs/node-chakracore/commit/f2952070f3)] - chakrashim: external object creation improvements
* [[`bedeb29118`](https://github.com/nodejs/node-chakracore/commit/bedeb29118)] - deps: update ChakraCore to Microsoft/ChakraCore@aef2cfad40
* [[`fafec98df5`](https://github.com/nodejs/node-chakracore/commit/fafec98df5)] - meta: merge node/master into node-chakracore/master
* [[`bafa0def68`](https://github.com/nodejs/node-chakracore/commit/bafa0def68)] - chakrashim: fix internal promise behavior
* [[`4f85fd081a`](https://github.com/nodejs/node-chakracore/commit/4f85fd081a)] - meta: merge node/master into node-chakracore/master
* [[`219bf8ccbf`](https://github.com/nodejs/node-chakracore/commit/219bf8ccbf)] - chakrashim: implement promise rejection callback
* [[`a4d7cd5851`](https://github.com/nodejs/node-chakracore/commit/a4d7cd5851)] - deps: update ChakraCore to Microsoft/ChakraCore@000ac9009b
* [[`90c554d1de`](https://github.com/nodejs/node-chakracore/commit/90c554d1de)] - chakrashim: Fix Promise::Resolver
* [[`2ef9cf92e6`](https://github.com/nodejs/node-chakracore/commit/2ef9cf92e6)] - meta: merge node/master into node-chakracore/master
* [[`b7ed19bd30`](https://github.com/nodejs/node-chakracore/commit/b7ed19bd30)] - meta: merge node/master into node-chakracore/master
* [[`ab47a18dde`](https://github.com/nodejs/node-chakracore/commit/ab47a18dde)] - deps: update ChakraCore to Microsoft/ChakraCore@70ad0bd17f
* [[`9faca7fa88`](https://github.com/nodejs/node-chakracore/commit/9faca7fa88)] - deps: update ChakraCore to Microsoft/ChakraCore@bd435f7d1b
* [[`3d9136d611`](https://github.com/nodejs/node-chakracore/commit/3d9136d611)] - chakrashim: missing return for stubbed method
* [[`3aff81769f`](https://github.com/nodejs/node-chakracore/commit/3aff81769f)] - chakrashim: Move `unordered_set` to chakrashim
* [[`bf31cff239`](https://github.com/nodejs/node-chakracore/commit/bf31cff239)] - src,test: cleanup differences with upstream
* [[`15fcb47699`](https://github.com/nodejs/node-chakracore/commit/15fcb47699)] - deps: update ChakraCore to Microsoft/ChakraCore@ce816add94
* [[`e3bfac6021`](https://github.com/nodejs/node-chakracore/commit/e3bfac6021)] - deps: update ChakraCore to Microsoft/ChakraCore@54e986f8a4
* [[`96f1f73b1e`](https://github.com/nodejs/node-chakracore/commit/96f1f73b1e)] - deps: update ChakraCore to Microsoft/ChakraCore@c806128547
* [[`eb818db09b`](https://github.com/nodejs/node-chakracore/commit/eb818db09b)] - deps: adding stubs for new v8 methods
* [[`48e83fa48a`](https://github.com/nodejs/node-chakracore/commit/48e83fa48a)] - meta: merge node/master into node-chakracore/master
* [[`9b3436d531`](https://github.com/nodejs/node-chakracore/commit/9b3436d531)] - deps: update ChakraCore to Microsoft/ChakraCore@95875b5946
* [[`19608c0820`](https://github.com/nodejs/node-chakracore/commit/19608c0820)] - deps: update ChakraCore to Microsoft/ChakraCore@22f9ab82ae
* [[`ae65cc7a09`](https://github.com/nodejs/node-chakracore/commit/ae65cc7a09)] - deps: update ChakraCore to Microsoft/ChakraCore@56d4855cc8
* [[`c057e77f60`](https://github.com/nodejs/node-chakracore/commit/c057e77f60)] - deps,src,test: fixing lint errors
* [[`37859241cc`](https://github.com/nodejs/node-chakracore/commit/37859241cc)] - meta: merge node/master into node-chakracore/master
* [[`39dcd869d0`](https://github.com/nodejs/node-chakracore/commit/39dcd869d0)] - meta: merge node/master into node-chakracore/master
* [[`65a249e5f9`](https://github.com/nodejs/node-chakracore/commit/65a249e5f9)] - meta: merge node/master into node-chakracore/master
* [[`6e938073ba`](https://github.com/nodejs/node-chakracore/commit/6e938073ba)] - meta: merge node/master into node-chakracore/master
* [[`5ac557cfa7`](https://github.com/nodejs/node-chakracore/commit/5ac557cfa7)] - test: updating baselines and expectations
* [[`4604c8f23c`](https://github.com/nodejs/node-chakracore/commit/4604c8f23c)] - meta: merge node/master into node-chakracore/master
* [[`22d6a860d9`](https://github.com/nodejs/node-chakracore/commit/22d6a860d9)] - deps: update ChakraCore to Microsoft/ChakraCore@e81ec848cc
* [[`5932434e90`](https://github.com/nodejs/node-chakracore/commit/5932434e90)] - deps: update ChakraCore to Microsoft/ChakraCore@e3e1a360e5
* [[`46ae2449c6`](https://github.com/nodejs/node-chakracore/commit/46ae2449c6)] - deps: update ChakraCore to Microsoft/ChakraCore@c6c0ff7ef9
* [[`5beee7e25b`](https://github.com/nodejs/node-chakracore/commit/5beee7e25b)] - test: updating baseline to work with chakracore
* [[`3986ac3cd5`](https://github.com/nodejs/node-chakracore/commit/3986ac3cd5)] - meta: merge node/master into node-chakracore/master
* [[`63ce459c99`](https://github.com/nodejs/node-chakracore/commit/63ce459c99)] - meta: merge node/master into node-chakracore/master
* [[`176336f5a5`](https://github.com/nodejs/node-chakracore/commit/176336f5a5)] - src: updating node_api_jsrt to match error messages
* [[`a89631e212`](https://github.com/nodejs/node-chakracore/commit/a89631e212)] - meta: merge node/master into node-chakracore/master
* [[`6faf146672`](https://github.com/nodejs/node-chakracore/commit/6faf146672)] - deps: update ChakraCore to Microsoft/ChakraCore@db6afe6a8f
* [[`3996e8e8ec`](https://github.com/nodejs/node-chakracore/commit/3996e8e8ec)] - deps: update ChakraCore to Microsoft/ChakraCore@e4156a11c8
* [[`297bbef74c`](https://github.com/nodejs/node-chakracore/commit/297bbef74c)] - deps: update ChakraCore to Microsoft/ChakraCore@782cd432cd
* [[`42265eee1d`](https://github.com/nodejs/node-chakracore/commit/42265eee1d)] - deps: update ChakraCore to Microsoft/ChakraCore@8cf80e378c
* [[`673c123d8d`](https://github.com/nodejs/node-chakracore/commit/673c123d8d)] - test: skipping tests using finally
* [[`8dca89d705`](https://github.com/nodejs/node-chakracore/commit/8dca89d705)] - src: changing node_file's usage of v8::Resolver
* [[`354e3064aa`](https://github.com/nodejs/node-chakracore/commit/354e3064aa)] - shim: adding v8::Promise and v8::Promise::Resolver implementations
* [[`7a53fcefce`](https://github.com/nodejs/node-chakracore/commit/7a53fcefce)] - napi: adding napi_*_callback_scope implementations for chakracore
* [[`abe7bd5fa1`](https://github.com/nodejs/node-chakracore/commit/abe7bd5fa1)] - deps: updating chakrashim with new CpuProfiler stubs
* [[`373752f9a0`](https://github.com/nodejs/node-chakracore/commit/373752f9a0)] - meta: merge node/master into node-chakracore/master
* [[`3ad3d00f31`](https://github.com/nodejs/node-chakracore/commit/3ad3d00f31)] - deps: update ChakraCore to Microsoft/ChakraCore@e7945af187
* [[`097fcd0346`](https://github.com/nodejs/node-chakracore/commit/097fcd0346)] - test: Disabling new test that relies on es module support
* [[`6385fdc16e`](https://github.com/nodejs/node-chakracore/commit/6385fdc16e)] - meta: merge node/master into node-chakracore/master
* [[`588f568eb7`](https://github.com/nodejs/node-chakracore/commit/588f568eb7)] - deps: update ChakraCore to Microsoft/ChakraCore@5a1a799161
* [[`dead9fe190`](https://github.com/nodejs/node-chakracore/commit/dead9fe190)] - deps: update ChakraCore to Microsoft/ChakraCore@b52d5c0bb3
* [[`4c7e62f480`](https://github.com/nodejs/node-chakracore/commit/4c7e62f480)] - deps: update ChakraCore to Microsoft/ChakraCore@8af539b73c
* [[`e8ff916d72`](https://github.com/nodejs/node-chakracore/commit/e8ff916d72)] - deps: update ChakraCore to Microsoft/ChakraCore@da37507e78
* [[`acc60e6102`](https://github.com/nodejs/node-chakracore/commit/acc60e6102)] - meta: merge node/master into node-chakracore/master
* [[`ff686dcfd5`](https://github.com/nodejs/node-chakracore/commit/ff686dcfd5)] - meta: merge node/master into node-chakracore/master
* [[`2553cc9166`](https://github.com/nodejs/node-chakracore/commit/2553cc9166)] - meta: merge node/master into node-chakracore/master
* [[`6da5a68bbf`](https://github.com/nodejs/node-chakracore/commit/6da5a68bbf)] - test: disabling tests which require features chakra doesn't support yet
* [[`973cee744e`](https://github.com/nodejs/node-chakracore/commit/973cee744e)] - deps: updating chakrashim with new experimental v8 api stub to fix build
* [[`f7e8fc6bc2`](https://github.com/nodejs/node-chakracore/commit/f7e8fc6bc2)] - meta: merge node/master into node-chakracore/master
* [[`bfc9ea1656`](https://github.com/nodejs/node-chakracore/commit/bfc9ea1656)] - doc: add tips for build OS list
* [[`df36ec4a33`](https://github.com/nodejs/node-chakracore/commit/df36ec4a33)] - deps: update ChakraCore to Microsoft/ChakraCore@f1a6b1015c
* [[`eb9ae3a161`](https://github.com/nodejs/node-chakracore/commit/eb9ae3a161)] - deps: update ChakraCore to Microsoft/ChakraCore@c50dfa8d83
* [[`c7429c73ac`](https://github.com/nodejs/node-chakracore/commit/c7429c73ac)] - deps: update ChakraCore to Microsoft/ChakraCore@aec9a9c70a
* [[`6d92a2a239`](https://github.com/nodejs/node-chakracore/commit/6d92a2a239)] - deps: update ChakraCore to Microsoft/ChakraCore@3b8a6ebedf
* [[`b63776fa99`](https://github.com/nodejs/node-chakracore/commit/b63776fa99)] - deps: update ChakraCore to Microsoft/ChakraCore@dc3f1d2fdf
* [[`f162a30337`](https://github.com/nodejs/node-chakracore/commit/f162a30337)] - file: fixing ttd hooks in node_file
* [[`96b667d6d9`](https://github.com/nodejs/node-chakracore/commit/96b667d6d9)] - test: Disabling vm module tests until node-chakracore supports them.
* [[`0568e8af01`](https://github.com/nodejs/node-chakracore/commit/0568e8af01)] - chakrashim: fixing build failure by adding v8::Module::GetException
* [[`47d543040c`](https://github.com/nodejs/node-chakracore/commit/47d543040c)] - meta: merge node/master into node-chakracore/master
* [[`4a4520eba4`](https://github.com/nodejs/node-chakracore/commit/4a4520eba4)] - meta: merge node/master into node-chakracore/master
* [[`086bab5af3`](https://github.com/nodejs/node-chakracore/commit/086bab5af3)] - meta: merge node/master into node-chakracore/master
* [[`a38d27e868`](https://github.com/nodejs/node-chakracore/commit/a38d27e868)] - meta: merge node/master into node-chakracore/master
* [[`aa3250d606`](https://github.com/nodejs/node-chakracore/commit/aa3250d606)] - deps,test: fixing build break by adding unimplemented v8 functions
* [[`4569b28a1d`](https://github.com/nodejs/node-chakracore/commit/4569b28a1d)] - meta: merge node/master into node-chakracore/master
* [[`33a43bc18e`](https://github.com/nodejs/node-chakracore/commit/33a43bc18e)] - chakrashim: fixing build break by adding new stubs for v8 features
* [[`b9518a5f92`](https://github.com/nodejs/node-chakracore/commit/b9518a5f92)] - meta: merge node/master into node-chakracore/master
* [[`8c30a84663`](https://github.com/nodejs/node-chakracore/commit/8c30a84663)] - deps: update ChakraCore to Microsoft/ChakraCore@91ec1ef92c
* [[`1a2c13cae1`](https://github.com/nodejs/node-chakracore/commit/1a2c13cae1)] - deps: update ChakraCore to Microsoft/ChakraCore@5d123b4b51
* [[`6bdbd0802d`](https://github.com/nodejs/node-chakracore/commit/6bdbd0802d)] - deps: update ChakraCore to Microsoft/ChakraCore@a9a368f3da
* [[`d65786653a`](https://github.com/nodejs/node-chakracore/commit/d65786653a)] - test: fixing test failures
* [[`ec2fb560a1`](https://github.com/nodejs/node-chakracore/commit/ec2fb560a1)] - meta: merge node/master into node-chakracore/master
* [[`d6fe9af015`](https://github.com/nodejs/node-chakracore/commit/d6fe9af015)] - deps: update ChakraCore to Microsoft/ChakraCore@e4493a2fe7
* [[`06c6da9be6`](https://github.com/nodejs/node-chakracore/commit/06c6da9be6)] - deps: update ChakraCore to Microsoft/ChakraCore@a92d32a2d5
* [[`aa7a943355`](https://github.com/nodejs/node-chakracore/commit/aa7a943355)] - deps: update ChakraCore to Microsoft/ChakraCore@e176a29f9d
* [[`df48e70487`](https://github.com/nodejs/node-chakracore/commit/df48e70487)] - meta: merge node/master into node-chakracore/master
* [[`c3d4d6a070`](https://github.com/nodejs/node-chakracore/commit/c3d4d6a070)] - test: Disabling test-inspector-esm
* [[`d55cb6bc28`](https://github.com/nodejs/node-chakracore/commit/d55cb6bc28)] - meta: merge node/master into node-chakracore/master
* [[`bb55bc3cd5`](https://github.com/nodejs/node-chakracore/commit/bb55bc3cd5)] - meta: merge node/master into node-chakracore/master
* [[`280f266738`](https://github.com/nodejs/node-chakracore/commit/280f266738)] - deps: update ChakraCore to Microsoft/ChakraCore@86e6e5cbe9
* [[`59f2d51fe7`](https://github.com/nodejs/node-chakracore/commit/59f2d51fe7)] - meta: merge node/master into node-chakracore/master
* [[`2d7d72fde7`](https://github.com/nodejs/node-chakracore/commit/2d7d72fde7)] - chakrashim: Fixing v8::ArrayBuffer implementation detail
* [[`ad994dd834`](https://github.com/nodejs/node-chakracore/commit/ad994dd834)] - meta: merge node/master into node-chakracore/master
* [[`f881539b10`](https://github.com/nodejs/node-chakracore/commit/f881539b10)] - node.gypi: fix osx build
* [[`0a7a9ab3f3`](https://github.com/nodejs/node-chakracore/commit/0a7a9ab3f3)] - test: modify console-assign-undefined to not rely on error messages
* [[`a90dd55082`](https://github.com/nodejs/node-chakracore/commit/a90dd55082)] - meta: merge node/master into node-chakracore/master
* [[`d9701a908f`](https://github.com/nodejs/node-chakracore/commit/d9701a908f)] - lint: fix lint issues from ttd fixes
* [[`ef8cd26c1f`](https://github.com/nodejs/node-chakracore/commit/ef8cd26c1f)] - deps: update ChakraCore to Microsoft/ChakraCore@45a175d1c7
* [[`e3d6b28036`](https://github.com/nodejs/node-chakracore/commit/e3d6b28036)] - deps: update ChakraCore to Microsoft/ChakraCore@488faf3350
* [[`1316d0b89a`](https://github.com/nodejs/node-chakracore/commit/1316d0b89a)] - deps: update ChakraCore to Microsoft/ChakraCore@5c0bed5a9a
* [[`fb1b8d6878`](https://github.com/nodejs/node-chakracore/commit/fb1b8d6878)] - meta: merge node/master into node-chakracore/master
* [[`c5a23c47f3`](https://github.com/nodejs/node-chakracore/commit/c5a23c47f3)] - lib,test: disable pretty error messages for chakracore
* [[`b2f396473c`](https://github.com/nodejs/node-chakracore/commit/b2f396473c)] - build: fix node-chakracore build after static/shared build update
* [[`dc759ca325`](https://github.com/nodejs/node-chakracore/commit/dc759ca325)] - Add support for innerloop Time-Travel Debugging
* [[`0dc4db517b`](https://github.com/nodejs/node-chakracore/commit/0dc4db517b)] - meta: merge node/master into node-chakracore/master
* [[`2bc8958234`](https://github.com/nodejs/node-chakracore/commit/2bc8958234)] - deps: update ChakraCore to Microsoft/ChakraCore@1dd69bc4ba
* [[`31a5b2e0d5`](https://github.com/nodejs/node-chakracore/commit/31a5b2e0d5)] - deps: update ChakraCore to Microsoft/ChakraCore@dc9c863768
* [[`023dc1b7d0`](https://github.com/nodejs/node-chakracore/commit/023dc1b7d0)] - deps: update ChakraCore to Microsoft/ChakraCore@a5d8155c09
* [[`27699e1941`](https://github.com/nodejs/node-chakracore/commit/27699e1941)] - deps: update ChakraCore to Microsoft/ChakraCore@59f853017f
* [[`91356f08dd`](https://github.com/nodejs/node-chakracore/commit/91356f08dd)] - deps: update ChakraCore to Microsoft/ChakraCore@396234826c
* [[`e41955f8b7`](https://github.com/nodejs/node-chakracore/commit/e41955f8b7)] - deps: update ChakraCore to Microsoft/ChakraCore@965f77965c
* [[`d62672018a`](https://github.com/nodejs/node-chakracore/commit/d62672018a)] - test: disable failing test
* [[`f012696131`](https://github.com/nodejs/node-chakracore/commit/f012696131)] - meta: merge node/master into node-chakracore/master
* [[`a3dccd8127`](https://github.com/nodejs/node-chakracore/commit/a3dccd8127)] - deps: update ChakraCore to Microsoft/ChakraCore@411514394e
* [[`8fb8d14c10`](https://github.com/nodejs/node-chakracore/commit/8fb8d14c10)] - deps: update ChakraCore to Microsoft/ChakraCore@8c0764e1aa
* [[`fb1f064f49`](https://github.com/nodejs/node-chakracore/commit/fb1f064f49)] - deps: update ChakraCore to Microsoft/ChakraCore@24928c5041
* [[`d220b326d2`](https://github.com/nodejs/node-chakracore/commit/d220b326d2)] - deps: update ChakraCore to Microsoft/ChakraCore@77a31ac554
* [[`6368743bba`](https://github.com/nodejs/node-chakracore/commit/6368743bba)] - deps: update ChakraCore to Microsoft/ChakraCore@9f40fcb70b
* [[`2fd0425130`](https://github.com/nodejs/node-chakracore/commit/2fd0425130)] - deps: update ChakraCore to Microsoft/ChakraCore@e93b29b80d
* [[`88a568f35c`](https://github.com/nodejs/node-chakracore/commit/88a568f35c)] - deps: update ChakraCore to Microsoft/ChakraCore@a231b2126f
* [[`4847602a0c`](https://github.com/nodejs/node-chakracore/commit/4847602a0c)] - deps: update ChakraCore to Microsoft/ChakraCore@ae0a7ca8f4
* [[`a8ee7e95fe`](https://github.com/nodejs/node-chakracore/commit/a8ee7e95fe)] - deps: update ChakraCore to Microsoft/ChakraCore@9d70c00ad7
* [[`a5288c0536`](https://github.com/nodejs/node-chakracore/commit/a5288c0536)] - deps: update ChakraCore to Microsoft/ChakraCore@9fe64abd2e
* [[`1727576266`](https://github.com/nodejs/node-chakracore/commit/1727576266)] - deps: update ChakraCore to Microsoft/ChakraCore@28cc7c2c48
* [[`750bbf2527`](https://github.com/nodejs/node-chakracore/commit/750bbf2527)] - deps: update ChakraCore to Microsoft/ChakraCore@cc2aede32e
* [[`37efd78625`](https://github.com/nodejs/node-chakracore/commit/37efd78625)] - deps: update ChakraCore to Microsoft/ChakraCore@172bac86e0
* [[`51077b4d97`](https://github.com/nodejs/node-chakracore/commit/51077b4d97)] - deps: update ChakraCore to Microsoft/ChakraCore@0615a510f6
* [[`3ac3c47989`](https://github.com/nodejs/node-chakracore/commit/3ac3c47989)] - deps: update ChakraCore to Microsoft/ChakraCore@5644e6c636
* [[`5f833aba3d`](https://github.com/nodejs/node-chakracore/commit/5f833aba3d)] - deps: update ChakraCore to Microsoft/ChakraCore@abd18d0bde
* [[`6464e70b16`](https://github.com/nodejs/node-chakracore/commit/6464e70b16)] - meta: merge node/master into node-chakracore/master
* [[`e32462b9c9`](https://github.com/nodejs/node-chakracore/commit/e32462b9c9)] - meta: merge node/master into node-chakracore/master
* [[`0bdb2b1155`](https://github.com/nodejs/node-chakracore/commit/0bdb2b1155)] - deps: update ChakraCore to Microsoft/ChakraCore@172bac86e0
* [[`25a3ac82cc`](https://github.com/nodejs/node-chakracore/commit/25a3ac82cc)] - test: update inspector skips
* [[`f8d2a2850a`](https://github.com/nodejs/node-chakracore/commit/f8d2a2850a)] - deps: update ChakraCore to Microsoft/ChakraCore@acd74547f7
* [[`d3af30fa98`](https://github.com/nodejs/node-chakracore/commit/d3af30fa98)] - deps: update ChakraCore to Microsoft/ChakraCore@9653b070f6
* [[`0fe63521cb`](https://github.com/nodejs/node-chakracore/commit/0fe63521cb)] - deps: update ChakraCore to Microsoft/ChakraCore@483cea69c5
* [[`eb45384061`](https://github.com/nodejs/node-chakracore/commit/eb45384061)] - chakrashim: Fix `SetAccessorProperty` behavior
* [[`1753873833`](https://github.com/nodejs/node-chakracore/commit/1753873833)] - lib,src,test: fixing lint errors
* [[`822d967bd4`](https://github.com/nodejs/node-chakracore/commit/822d967bd4)] - deps: update ChakraCore to Microsoft/ChakraCore@37993b9415
* [[`4cae9e440d`](https://github.com/nodejs/node-chakracore/commit/4cae9e440d)] - deps: update ChakraCore to Microsoft/ChakraCore@4d96e78734
* [[`762da23242`](https://github.com/nodejs/node-chakracore/commit/762da23242)] - deps: update ChakraCore to Microsoft/ChakraCore@3f2f8a7d1c
* [[`38bea7b8ea`](https://github.com/nodejs/node-chakracore/commit/38bea7b8ea)] - deps: update ChakraCore to Microsoft/ChakraCore@14831da3ee
* [[`308d6b5705`](https://github.com/nodejs/node-chakracore/commit/308d6b5705)] - deps: update ChakraCore to Microsoft/ChakraCore@fdd885bc8c
* [[`04bfbc717f`](https://github.com/nodejs/node-chakracore/commit/04bfbc717f)] - deps: update ChakraCore to Microsoft/ChakraCore@7318a88837
* [[`fbd2861859`](https://github.com/nodejs/node-chakracore/commit/fbd2861859)] - deps: update ChakraCore to Microsoft/ChakraCore@2abdaab754
* [[`2f4978abc2`](https://github.com/nodejs/node-chakracore/commit/2f4978abc2)] - deps: update ChakraCore to Microsoft/ChakraCore@b2448404a3
* [[`6f29bccf28`](https://github.com/nodejs/node-chakracore/commit/6f29bccf28)] - deps: update ChakraCore to Microsoft/ChakraCore@cfcbe3b3d2
* [[`e260fa1614`](https://github.com/nodejs/node-chakracore/commit/e260fa1614)] - deps: update ChakraCore to Microsoft/ChakraCore@1d521f5052
* [[`d7b9182019`](https://github.com/nodejs/node-chakracore/commit/d7b9182019)] - deps: update ChakraCore to Microsoft/ChakraCore@50e5667a3f
* [[`ee274afc39`](https://github.com/nodejs/node-chakracore/commit/ee274afc39)] - meta: merge node/master into node-chakracore/master
* [[`0214db53ee`](https://github.com/nodejs/node-chakracore/commit/0214db53ee)] - meta: merge node/master into node-chakracore/master
* [[`948537d02b`](https://github.com/nodejs/node-chakracore/commit/948537d02b)] - test: disable test requiring for...await for node-chakracore
* [[`be82dfb39c`](https://github.com/nodejs/node-chakracore/commit/be82dfb39c)] - meta: merge node/master into node-chakracore/master
* [[`8374411ea3`](https://github.com/nodejs/node-chakracore/commit/8374411ea3)] - napi: add range check to jsrt version of napi_create_dataview
* [[`8347baf9e4`](https://github.com/nodejs/node-chakracore/commit/8347baf9e4)] - meta: merge node/master into node-chakracore/master
* [[`bb91d42162`](https://github.com/nodejs/node-chakracore/commit/bb91d42162)] - meta: merge node/master into node-chakracore/master
* [[`8f7439967b`](https://github.com/nodejs/node-chakracore/commit/8f7439967b)] - test: skip test-tracing test on node-chakracore
* [[`35b8ff2fc1`](https://github.com/nodejs/node-chakracore/commit/35b8ff2fc1)] - meta: merge node/master into node-chakracore/master
* [[`aa23e3fb2e`](https://github.com/nodejs/node-chakracore/commit/aa23e3fb2e)] - meta: merge node/master into node-chakracore/master
* [[`cc364617eb`](https://github.com/nodejs/node-chakracore/commit/cc364617eb)] - meta: merge node/master into node-chakracore/master
* [[`9790e12068`](https://github.com/nodejs/node-chakracore/commit/9790e12068)] - deps: update ChakraCore to Microsoft/ChakraCore@e488ebb777
* [[`77faa4baf3`](https://github.com/nodejs/node-chakracore/commit/77faa4baf3)] - meta: merge node/master into node-chakracore/master
* [[`4a6d224167`](https://github.com/nodejs/node-chakracore/commit/4a6d224167)] - meta: merge node/master into node-chakracore/master
* [[`57e9bb4d0d`](https://github.com/nodejs/node-chakracore/commit/57e9bb4d0d)] - meta: merge node/master into node-chakracore/master
* [[`967a8d20ec`](https://github.com/nodejs/node-chakracore/commit/967a8d20ec)] - meta: merge node/master into node-chakracore/master
* [[`159dcd56f8`](https://github.com/nodejs/node-chakracore/commit/159dcd56f8)] - test: skip flaky test for node-chakracore
* [[`cdb310f5fe`](https://github.com/nodejs/node-chakracore/commit/cdb310f5fe)] - meta: merge node/master into node-chakracore/master
* [[`834b239391`](https://github.com/nodejs/node-chakracore/commit/834b239391)] - meta: merge node/master into node-chakracore/master
* [[`327541453e`](https://github.com/nodejs/node-chakracore/commit/327541453e)] - meta: merge node/master into node-chakracore/master
* [[`3f73796b50`](https://github.com/nodejs/node-chakracore/commit/3f73796b50)] - meta: merge node/master into node-chakracore/master
* [[`b57a8a0d48`](https://github.com/nodejs/node-chakracore/commit/b57a8a0d48)] - test: disable esm unit tests failing on node-chakracore
* [[`5f2fc2d817`](https://github.com/nodejs/node-chakracore/commit/5f2fc2d817)] - meta: merge node/master into node-chakracore/master
* [[`e34de77769`](https://github.com/nodejs/node-chakracore/commit/e34de77769)] - meta: merge node/master into node-chakracore/master
* [[`86c1bc30bd`](https://github.com/nodejs/node-chakracore/commit/86c1bc30bd)] - meta: merge node/master into node-chakracore/master
* [[`7c2d058c18`](https://github.com/nodejs/node-chakracore/commit/7c2d058c18)] - test: skip some tests failing on node-chakracore
* [[`4f7a00f5c1`](https://github.com/nodejs/node-chakracore/commit/4f7a00f5c1)] - meta: merge node/master into node-chakracore/master
* [[`f40894f167`](https://github.com/nodejs/node-chakracore/commit/f40894f167)] - deps: update ChakraCore to Microsoft/ChakraCore@4e2496ab03
* [[`aad4545c3f`](https://github.com/nodejs/node-chakracore/commit/aad4545c3f)] - deps: update ChakraCore to Microsoft/ChakraCore@11715eefd5
* [[`eac59927ff`](https://github.com/nodejs/node-chakracore/commit/eac59927ff)] - deps: update ChakraCore to Microsoft/ChakraCore@7749fb845a
* [[`48ecad9fb1`](https://github.com/nodejs/node-chakracore/commit/48ecad9fb1)] - meta: merge node/master into node-chakracore/master
* [[`db4f534f8b`](https://github.com/nodejs/node-chakracore/commit/db4f534f8b)] - meta: merge node/master into node-chakracore/master
* [[`897588030e`](https://github.com/nodejs/node-chakracore/commit/897588030e)] - chakrashim: add shim for Template::SetAccessorProperty
* [[`03803b830c`](https://github.com/nodejs/node-chakracore/commit/03803b830c)] - doc: not all example code can be run without 1:1
* [[`19b4921e8b`](https://github.com/nodejs/node-chakracore/commit/19b4921e8b)] - doc: adjust TTY wording & add inter-doc links
* [[`9f3daf1608`](https://github.com/nodejs/node-chakracore/commit/9f3daf1608)] - doc: fix fs.existsSync description
* [[`195da8d3c1`](https://github.com/nodejs/node-chakracore/commit/195da8d3c1)] - doc: improve documentation.md
* [[`1dbb83a5c8`](https://github.com/nodejs/node-chakracore/commit/1dbb83a5c8)] - net,src: refactor writeQueueSize tracking
* [[`c347c9bb4e`](https://github.com/nodejs/node-chakracore/commit/c347c9bb4e)] - test: add test for postmortem metadata validation
* [[`85aad98200`](https://github.com/nodejs/node-chakracore/commit/85aad98200)] - stream: rm {writeable/readable}State.length
* [[`8fe0074413`](https://github.com/nodejs/node-chakracore/commit/8fe0074413)] - test: Use countdown in test file
* [[`a783c4fdb5`](https://github.com/nodejs/node-chakracore/commit/a783c4fdb5)] - test: update test-http-content-length to use countdown
* [[`9b4495e5f9`](https://github.com/nodejs/node-chakracore/commit/9b4495e5f9)] - doc: add countdown module to writing tests guide
* [[`c83ebee29a`](https://github.com/nodejs/node-chakracore/commit/c83ebee29a)] - test: coverage for emitExperimentalWarning
* [[`51c237a950`](https://github.com/nodejs/node-chakracore/commit/51c237a950)] - doc: change "Node.js style cb" to "error-first cb"
* [[`ec2e09220f`](https://github.com/nodejs/node-chakracore/commit/ec2e09220f)] - src: replace SetAccessor w/ SetAccessorProperty
* [[`c0b4721736`](https://github.com/nodejs/node-chakracore/commit/c0b4721736)] - deps: update ChakraCore to Microsoft/ChakraCore@99131a0f2c
* [[`3571d06624`](https://github.com/nodejs/node-chakracore/commit/3571d06624)] - deps: update ChakraCore to Microsoft/ChakraCore@4336e29ca2
* [[`03ffff14d8`](https://github.com/nodejs/node-chakracore/commit/03ffff14d8)] - deps: update ChakraCore to Microsoft/ChakraCore@a0d8bdd37e
* [[`ee8f8bfe8f`](https://github.com/nodejs/node-chakracore/commit/ee8f8bfe8f)] - deps: update ChakraCore to Microsoft/ChakraCore@d1c4faf38b
* [[`912038511f`](https://github.com/nodejs/node-chakracore/commit/912038511f)] - meta: merge node/master into node-chakracore/master
* [[`88b3c710c9`](https://github.com/nodejs/node-chakracore/commit/88b3c710c9)] - test: update node-chakracore test baseline
* [[`4ffd79198c`](https://github.com/nodejs/node-chakracore/commit/4ffd79198c)] - meta: merge node/master into node-chakracore/master
* [[`8b1ebf6ba6`](https://github.com/nodejs/node-chakracore/commit/8b1ebf6ba6)] - meta: merge node/master into node-chakracore/master
* [[`78e86800e2`](https://github.com/nodejs/node-chakracore/commit/78e86800e2)] - meta: merge node/master into node-chakracore/master
* [[`7714b4d6bc`](https://github.com/nodejs/node-chakracore/commit/7714b4d6bc)] - test: skip failing test
* [[`d21eee8cca`](https://github.com/nodejs/node-chakracore/commit/d21eee8cca)] - meta: merge node/master into node-chakracore/master
* [[`bc073bfec8`](https://github.com/nodejs/node-chakracore/commit/bc073bfec8)] - meta: merge node/master into node-chakracore/master
* [[`d48f2318e3`](https://github.com/nodejs/node-chakracore/commit/d48f2318e3)] - meta: merge node/master into node-chakracore/master
* [[`f6a4546ee7`](https://github.com/nodejs/node-chakracore/commit/f6a4546ee7)] - meta: merge node/master into node-chakracore/master
* [[`672913c288`](https://github.com/nodejs/node-chakracore/commit/672913c288)] - test: update chakracore baseline
* [[`4bfab5a9a9`](https://github.com/nodejs/node-chakracore/commit/4bfab5a9a9)] - meta: merge node/master into node-chakracore/master
* [[`246f8438d8`](https://github.com/nodejs/node-chakracore/commit/246f8438d8)] - deps: update ChakraCore to Microsoft/ChakraCore@d7d99e1dbe
* [[`bf5c71f951`](https://github.com/nodejs/node-chakracore/commit/bf5c71f951)] - deps: update ChakraCore to Microsoft/ChakraCore@7c4f6a5e48
* [[`300c5b8707`](https://github.com/nodejs/node-chakracore/commit/300c5b8707)] - test: update chakracore baselines
* [[`0f6eee10fd`](https://github.com/nodejs/node-chakracore/commit/0f6eee10fd)] - build: fix build break
* [[`e104f7421e`](https://github.com/nodejs/node-chakracore/commit/e104f7421e)] - meta: merge node/master into node-chakracore/master
* [[`985faa1cf1`](https://github.com/nodejs/node-chakracore/commit/985faa1cf1)] - test: add a new.target test to addons-napi (#452)
* [[`c6b7fcab1b`](https://github.com/nodejs/node-chakracore/commit/c6b7fcab1b)] - napi: use new.target jsrt api (#451)
* [[`d4795cd90f`](https://github.com/nodejs/node-chakracore/commit/d4795cd90f)] - chakrashim: implement new.target support native callbacks (#450)
* [[`7206694a39`](https://github.com/nodejs/node-chakracore/commit/7206694a39)] - deps: update ChakraCore to Microsoft/ChakraCore@638b18600d
* [[`62f742d8d3`](https://github.com/nodejs/node-chakracore/commit/62f742d8d3)] - deps: update ChakraCore to Microsoft/ChakraCore@5983ef6736
* [[`fe86600b48`](https://github.com/nodejs/node-chakracore/commit/fe86600b48)] - deps: update ChakraCore to Microsoft/ChakraCore@3444116dbd
* [[`932bc3034b`](https://github.com/nodejs/node-chakracore/commit/932bc3034b)] - deps: update ChakraCore to Microsoft/ChakraCore@98e7835eef
* [[`da8376c076`](https://github.com/nodejs/node-chakracore/commit/da8376c076)] - deps: update ChakraCore to Microsoft/ChakraCore@12a738b390
* [[`859e314570`](https://github.com/nodejs/node-chakracore/commit/859e314570)] - deps: update ChakraCore to Microsoft/ChakraCore@79ef8eda34
* [[`a5da8cc414`](https://github.com/nodejs/node-chakracore/commit/a5da8cc414)] - chakrashim: update macOS build parameters
* [[`c642810acc`](https://github.com/nodejs/node-chakracore/commit/c642810acc)] - chakrashim: ChakraCore release/1.9@c6ae7416fb
* [[`34a05e7439`](https://github.com/nodejs/node-chakracore/commit/34a05e7439)] - doc: Update nvs instructions in README.md
* [[`055ec67f91`](https://github.com/nodejs/node-chakracore/commit/055ec67f91)] - deps: update ChakraCore to Microsoft/ChakraCore@e20d5c67e4
* [[`e5a7d77fab`](https://github.com/nodejs/node-chakracore/commit/e5a7d77fab)] - deps: update ChakraCore to Microsoft/ChakraCore@cf780acb41
* [[`9b9cd3a240`](https://github.com/nodejs/node-chakracore/commit/9b9cd3a240)] - test: remove orphan status entries
* [[`fb149725f7`](https://github.com/nodejs/node-chakracore/commit/fb149725f7)] - build: node.gyp cleanup
* [[`5b15efc8d3`](https://github.com/nodejs/node-chakracore/commit/5b15efc8d3)] - deps: update ChakraCore to Microsoft/ChakraCore@f6f84bbb4f
* [[`94fbfba0b2`](https://github.com/nodejs/node-chakracore/commit/94fbfba0b2)] - deps: update ChakraCore to Microsoft/ChakraCore@d91fb1dbcd
* [[`e4f244522b`](https://github.com/nodejs/node-chakracore/commit/e4f244522b)] - deps: update ChakraCore to Microsoft/ChakraCore@e02b39aaf6
* [[`c90745af5d`](https://github.com/nodejs/node-chakracore/commit/c90745af5d)] - deps: update ChakraCore to Microsoft/ChakraCore@c6d73ba949
* [[`94af1db591`](https://github.com/nodejs/node-chakracore/commit/94af1db591)] - deps: update ChakraCore to Microsoft/ChakraCore@09034518ef
* [[`29fed8919d`](https://github.com/nodejs/node-chakracore/commit/29fed8919d)] - deps: update ChakraCore to Microsoft/ChakraCore@3e739383a5
* [[`7bee32cdbf`](https://github.com/nodejs/node-chakracore/commit/7bee32cdbf)] - deps: update ChakraCore to Microsoft/ChakraCore@f5ab04247e
* [[`c8e6e2fe8e`](https://github.com/nodejs/node-chakracore/commit/c8e6e2fe8e)] - chakrashim: fix inspector evaluate
* [[`ffa70ffe45`](https://github.com/nodejs/node-chakracore/commit/ffa70ffe45)] - test: skipping V8-specific test
* [[`43bd7a8429`](https://github.com/nodejs/node-chakracore/commit/43bd7a8429)] - chakrashim: fixing misc lint issues
* [[`197665e086`](https://github.com/nodejs/node-chakracore/commit/197665e086)] - meta: merge node/master into node-chakracore/master
* [[`fb3330bdd2`](https://github.com/nodejs/node-chakracore/commit/fb3330bdd2)] - test: disable test case for node-chakracore and fix a couple lint issues
* [[`40a2c902f3`](https://github.com/nodejs/node-chakracore/commit/40a2c902f3)] - meta: merge node/master into node-chakracore/master
* [[`4a19a9b6b2`](https://github.com/nodejs/node-chakracore/commit/4a19a9b6b2)] - meta: merge node/master into node-chakracore/master
* [[`a1eb4f9cac`](https://github.com/nodejs/node-chakracore/commit/a1eb4f9cac)] - test: Fix inspect test under node-chakracore
* [[`1d6dc6e221`](https://github.com/nodejs/node-chakracore/commit/1d6dc6e221)] - meta: merge node/master into node-chakracore/master
* [[`736105559d`](https://github.com/nodejs/node-chakracore/commit/736105559d)] - chakrashim: add dummy shim for v8::ArrayBuffer::Allocator::NewDefaultAllocator
* [[`68c14d780d`](https://github.com/nodejs/node-chakracore/commit/68c14d780d)] - meta: merge node/master into node-chakracore/master
* [[`286db5d331`](https://github.com/nodejs/node-chakracore/commit/286db5d331)] - chakrashim: add dummy shim for v8::ScriptOrModule and v8::Isolate::SetHostImportModuleDynamicallyCallback
* [[`6eea849a9e`](https://github.com/nodejs/node-chakracore/commit/6eea849a9e)] - meta: merge node/master into node-chakracore/master
* [[`936c715b72`](https://github.com/nodejs/node-chakracore/commit/936c715b72)] - test: simplify test/message/testcfg.py
* [[`c2da215ffc`](https://github.com/nodejs/node-chakracore/commit/c2da215ffc)] - Disable tests failing on chakracore
* [[`ec78bf4c00`](https://github.com/nodejs/node-chakracore/commit/ec78bf4c00)] - meta: merge node/master into node-chakracore/master
* [[`3045bb0190`](https://github.com/nodejs/node-chakracore/commit/3045bb0190)] - Fix unit test failures
* [[`a5c675df82`](https://github.com/nodejs/node-chakracore/commit/a5c675df82)] - meta: merge node/master into node-chakracore/master
* [[`a0082788fe`](https://github.com/nodejs/node-chakracore/commit/a0082788fe)] - test: move ChakraCore runtime skips to .status
* [[`5adda3a178`](https://github.com/nodejs/node-chakracore/commit/5adda3a178)] - test: skip tests for ChakraCore that depend on V8
* [[`f588241229`](https://github.com/nodejs/node-chakracore/commit/f588241229)] - test: skip tests that fail for ChakraCore
* [[`b5c739d0ed`](https://github.com/nodejs/node-chakracore/commit/b5c739d0ed)] - test: remove non existing tests from .status files
* [[`a7d03f28ba`](https://github.com/nodejs/node-chakracore/commit/a7d03f28ba)] - meta: merge node/master into node-chakracore/master
* [[`24aeb6abca`](https://github.com/nodejs/node-chakracore/commit/24aeb6abca)] - meta: merge node/master into node-chakracore/master
* [[`3a6b048fb5`](https://github.com/nodejs/node-chakracore/commit/3a6b048fb5)] - meta: merge node/master into node-chakracore/master
* [[`4316cb7f1e`](https://github.com/nodejs/node-chakracore/commit/4316cb7f1e)] - lib: shimming V8-specific code
* [[`d38fe31d87`](https://github.com/nodejs/node-chakracore/commit/d38fe31d87)] - meta: merge node/master into node-chakracore/master
* [[`229ecfe66f`](https://github.com/nodejs/node-chakracore/commit/229ecfe66f)] - deps: update ChakraCore to Microsoft/ChakraCore@156a58e6f4
* [[`223a260d07`](https://github.com/nodejs/node-chakracore/commit/223a260d07)] - meta: merge node/master into node-chakracore/master
* [[`e60e46904a`](https://github.com/nodejs/node-chakracore/commit/e60e46904a)] - n-api: add napi_get_uv_event_loop for JSRT
* [[`92ad9d2cf6`](https://github.com/nodejs/node-chakracore/commit/92ad9d2cf6)] - chakrashim,src: fixed missing shims
* [[`7f06df1423`](https://github.com/nodejs/node-chakracore/commit/7f06df1423)] - meta: merge node/master into node-chakracore/master
* [[`a97f06e077`](https://github.com/nodejs/node-chakracore/commit/a97f06e077)] - doc: fixed lint errors in README files
* [[`ca720a3ed6`](https://github.com/nodejs/node-chakracore/commit/ca720a3ed6)] - chakrashim: fixing ObjectTemplate implementation
* [[`74d08c6d47`](https://github.com/nodejs/node-chakracore/commit/74d08c6d47)] - chakrashim: implement LowMemoryNotification and ifdef DEBUG -> CHAKRA_UNIMPLEMENTED
* [[`77ab790057`](https://github.com/nodejs/node-chakracore/commit/77ab790057)] - chakrashim: remove stale ref objectInstance and save 8bytes
* [[`5db7165bac`](https://github.com/nodejs/node-chakracore/commit/5db7165bac)] - chakrashim: improve hot path calls
* [[`1dca12241c`](https://github.com/nodejs/node-chakracore/commit/1dca12241c)] - deps: updating chakracore gyp file to avoid removed flag
* [[`cf8b577f09`](https://github.com/nodejs/node-chakracore/commit/cf8b577f09)] - chakrashim: fixing ObjectTemplate regression
* [[`e72ff2e880`](https://github.com/nodejs/node-chakracore/commit/e72ff2e880)] - meta: merge node/master into node-chakracore/master
* [[`dd68c0d3fe`](https://github.com/nodejs/node-chakracore/commit/dd68c0d3fe)] - build: fix node-chakracore arm build
* [[`c9fc14bf61`](https://github.com/nodejs/node-chakracore/commit/c9fc14bf61)] - test: fix lint failures
* [[`f5757ebcf9`](https://github.com/nodejs/node-chakracore/commit/f5757ebcf9)] - test: marking non-implemented tests as flaky
* [[`2ea9f30808`](https://github.com/nodejs/node-chakracore/commit/2ea9f30808)] - deps: update ChakraCore to Microsoft/ChakraCore@1649d36e6d
* [[`19c7b741d7`](https://github.com/nodejs/node-chakracore/commit/19c7b741d7)] - meta: merge node/master into node-chakracore/master
* [[`6c36d319b5`](https://github.com/nodejs/node-chakracore/commit/6c36d319b5)] - meta: merge node/master into node-chakracore/master
* [[`bd6ef116b3`](https://github.com/nodejs/node-chakracore/commit/bd6ef116b3)] - meta: merge node/master into node-chakracore/master
* [[`4546394ef7`](https://github.com/nodejs/node-chakracore/commit/4546394ef7)] - test: disable trace events tests for Node-ChakraCore
* [[`b517f11b97`](https://github.com/nodejs/node-chakracore/commit/b517f11b97)] - build: fix build break from merge
* [[`2515ee1d1b`](https://github.com/nodejs/node-chakracore/commit/2515ee1d1b)] - meta: merge node/master into node-chakracore/master
* [[`1bbca333dd`](https://github.com/nodejs/node-chakracore/commit/1bbca333dd)] - deps: update ChakraCore to Microsoft/ChakraCore@17910d087a
* [[`0ffe499adf`](https://github.com/nodejs/node-chakracore/commit/0ffe499adf)] - deps: update ChakraCore to Microsoft/ChakraCore@fffb4e24be
* [[`473fb91eed`](https://github.com/nodejs/node-chakracore/commit/473fb91eed)] - test: update chakracore baseline
* [[`d620a1944d`](https://github.com/nodejs/node-chakracore/commit/d620a1944d)] - meta: merge node/master into node-chakracore/master
* [[`6015e0bc79`](https://github.com/nodejs/node-chakracore/commit/6015e0bc79)] - meta: merge node/master into node-chakracore/master
* [[`01d72be47d`](https://github.com/nodejs/node-chakracore/commit/01d72be47d)] - meta: merge node/master into node-chakracore/master
* [[`47a3e4600e`](https://github.com/nodejs/node-chakracore/commit/47a3e4600e)] - meta: merge node/master into node-chakracore/master
* [[`92aaf569a5`](https://github.com/nodejs/node-chakracore/commit/92aaf569a5)] - build: fix gyp conditions issue
* [[`cb986dc110`](https://github.com/nodejs/node-chakracore/commit/cb986dc110)] - test: add engine-specific message for new test
* [[`35059de215`](https://github.com/nodejs/node-chakracore/commit/35059de215)] - meta: merge node/master into node-chakracore/master
* [[`00702bb1ed`](https://github.com/nodejs/node-chakracore/commit/00702bb1ed)] - lint: fix lint errors
* [[`1749b7c55b`](https://github.com/nodejs/node-chakracore/commit/1749b7c55b)] - test: update test to use engine-specific message
* [[`3590539f8e`](https://github.com/nodejs/node-chakracore/commit/3590539f8e)] - meta: merge node/master into node-chakracore/master
* [[`321e4a1b11`](https://github.com/nodejs/node-chakracore/commit/321e4a1b11)] - chakrashim: fixing lazy interceptor instantiation
* [[`ac89d21d42`](https://github.com/nodejs/node-chakracore/commit/ac89d21d42)] - [Merge Microsoft/Chakracore@7b9c916c04] [MERGE #4217 @jackhorton] Intl-ICU on release/1.7 fixups
* [[`b50ab3eae1`](https://github.com/nodejs/node-chakracore/commit/b50ab3eae1)] - Revert "[Merge Microsoft/Chakracore@42789b26e3] Skip 1.6 commit"
* [[`8b94e26cff`](https://github.com/nodejs/node-chakracore/commit/8b94e26cff)] - Merge pull request #424 from obastemur/imp_ot_mem
* [[`c6863c08c6`](https://github.com/nodejs/node-chakracore/commit/c6863c08c6)] - [Merge Microsoft/Chakracore@42789b26e3] Skip 1.6 commit
* [[`ea40e0ba37`](https://github.com/nodejs/node-chakracore/commit/ea40e0ba37)] - [Merge Microsoft/Chakracore@4c2e0f3d68] [MERGE #4195 @boingoing] Merge unreleased/rs3 to release/1.7
* [[`9f473d41ac`](https://github.com/nodejs/node-chakracore/commit/9f473d41ac)] - meta: merge node/master into node-chakracore/master
* [[`b6b3ed8593`](https://github.com/nodejs/node-chakracore/commit/b6b3ed8593)] - meta: merge node/master into node-chakracore/master
* [[`188e4c2530`](https://github.com/nodejs/node-chakracore/commit/188e4c2530)] - meta: merge node/master into node-chakracore/master
* [[`4cfa823256`](https://github.com/nodejs/node-chakracore/commit/4cfa823256)] - lint: fix lint issue introduced by bad merge
* [[`fca1d9fdb4`](https://github.com/nodejs/node-chakracore/commit/fca1d9fdb4)] - tools: add chakra_tdd.h to install.py
* [[`161577813f`](https://github.com/nodejs/node-chakracore/commit/161577813f)] - meta: merge node/master into node-chakracore/master
* [[`a9a7dd0530`](https://github.com/nodejs/node-chakracore/commit/a9a7dd0530)] - build: fix macro issue
* [[`6a20f8dafb`](https://github.com/nodejs/node-chakracore/commit/6a20f8dafb)] - meta: merge node/master into node-chakracore/master
* [[`20c21347ac`](https://github.com/nodejs/node-chakracore/commit/20c21347ac)] - test: re-mark test as flaky for ChakraCore
* [[`175fe688f1`](https://github.com/nodejs/node-chakracore/commit/175fe688f1)] - test: don't use V8-specific options on ChakraCore
* [[`fda2d93524`](https://github.com/nodejs/node-chakracore/commit/fda2d93524)] - meta: merge node/master into node-chakracore/master
* [[`fdf859098c`](https://github.com/nodejs/node-chakracore/commit/fdf859098c)] - meta: merge node/master into node-chakracore/master
* [[`fe2f176bf4`](https://github.com/nodejs/node-chakracore/commit/fe2f176bf4)] - test: marking new esm test as flaky
* [[`3331186ac3`](https://github.com/nodejs/node-chakracore/commit/3331186ac3)] - Merge nodejs/master
* [[`3aa2c16607`](https://github.com/nodejs/node-chakracore/commit/3aa2c16607)] - Merge nodejs/master
* [[`35c8c3a0d7`](https://github.com/nodejs/node-chakracore/commit/35c8c3a0d7)] - Merge nodejs/master
* [[`3018e1645f`](https://github.com/nodejs/node-chakracore/commit/3018e1645f)] - chakrashim: add new V8 API
* [[`8fd7c3efc8`](https://github.com/nodejs/node-chakracore/commit/8fd7c3efc8)] - Merge nodejs/master
* [[`b48c11fdaf`](https://github.com/nodejs/node-chakracore/commit/b48c11fdaf)] - test: mark new ES Module tests as flaky
* [[`4d1baa251e`](https://github.com/nodejs/node-chakracore/commit/4d1baa251e)] - chakrashim: shim new v8::Module APIs
* [[`5298047d6f`](https://github.com/nodejs/node-chakracore/commit/5298047d6f)] - Merge nodejs/master
* [[`8eefbbb6c5`](https://github.com/nodejs/node-chakracore/commit/8eefbbb6c5)] - test: fixing flaky test flag
* [[`4ddf7934e9`](https://github.com/nodejs/node-chakracore/commit/4ddf7934e9)] - [Merge Microsoft/Chakracore@83b1218425] [MERGE #4136 @boingoing] Support for defer parse of object literal methods
* [[`8f2323e0a4`](https://github.com/nodejs/node-chakracore/commit/8f2323e0a4)] - [Merge Microsoft/Chakracore@b006606339] [MERGE #4100 @obastemur] [optional] Improve OS14363556
* [[`06ba99b724`](https://github.com/nodejs/node-chakracore/commit/06ba99b724)] - [Merge Microsoft/Chakracore@fd7889c423] [MERGE #4018 @obastemur] fix modFunctionIndex is missing on copy
* [[`fb7e3daa89`](https://github.com/nodejs/node-chakracore/commit/fb7e3daa89)] - [Merge Microsoft/Chakracore@0bc7709496] [MERGE #4105 @boingoing] Support for defer parsing lambda functions
* [[`c3c41bce3e`](https://github.com/nodejs/node-chakracore/commit/c3c41bce3e)] - lint: fix lint issue in trace_mgr
* [[`b41ebfbe7f`](https://github.com/nodejs/node-chakracore/commit/b41ebfbe7f)] - test: mark new test as flaky for chakracore
* [[`50d21749dd`](https://github.com/nodejs/node-chakracore/commit/50d21749dd)] - Merge nodejs/master
* [[`4fc16fc683`](https://github.com/nodejs/node-chakracore/commit/4fc16fc683)] - Merge nodejs/master
* [[`eaf6763c1a`](https://github.com/nodejs/node-chakracore/commit/eaf6763c1a)] - test: disable failing vm tests until we add proper support
* [[`5fed2d619d`](https://github.com/nodejs/node-chakracore/commit/5fed2d619d)] - chakrashim: Updating shim to fix build break
* [[`3b38e14b14`](https://github.com/nodejs/node-chakracore/commit/3b38e14b14)] - Merge nodejs/master
* [[`04c1428260`](https://github.com/nodejs/node-chakracore/commit/04c1428260)] - test: marking new esmodule as flaky due to lack of support
* [[`0fa8358a5e`](https://github.com/nodejs/node-chakracore/commit/0fa8358a5e)] - Merge nodejs/master
* [[`559f7cf37e`](https://github.com/nodejs/node-chakracore/commit/559f7cf37e)] - [Merge Microsoft/Chakracore@8dbf8e4721] [MERGE #4035 @rhuanjl] Optimise xplat UtcTimeFromStrCore
* [[`51d838b2b3`](https://github.com/nodejs/node-chakracore/commit/51d838b2b3)] - test: fixing test failure after merge.
* [[`5b3d44e075`](https://github.com/nodejs/node-chakracore/commit/5b3d44e075)] - Merge nodejs/master
* [[`40aeb28ee1`](https://github.com/nodejs/node-chakracore/commit/40aeb28ee1)] - Merge nodejs/master
* [[`9738df7fed`](https://github.com/nodejs/node-chakracore/commit/9738df7fed)] - test: fix up renamed tests
* [[`535c25e87d`](https://github.com/nodejs/node-chakracore/commit/535c25e87d)] - Merge nodejs/master

## 2017-10-21, node-chakracore-8.4.0

### Commits

* [[`1fac205e59`](https://github.com/nodejs/node-chakracore/commit/1fac205e59)] - [Merge Microsoft/Chakracore@ee3378557e] [MERGE #3742 @xiaoyinl] Never use exponential if radix not 10 (Fix #3739, #3740)
* [[`93718f1d99`](https://github.com/nodejs/node-chakracore/commit/93718f1d99)] - [Merge Microsoft/Chakracore@112d0d6a62] [MERGE #4004 @obastemur] numberToString: re-cache on miss and cache more
* [[`e7ddfd89c4`](https://github.com/nodejs/node-chakracore/commit/e7ddfd89c4)] - [Merge Microsoft/Chakracore@345870f035] [MERGE #3979 @boingoing] Remove an assert in `FindOrAddPidRef`
* [[`66bf53e7ad`](https://github.com/nodejs/node-chakracore/commit/66bf53e7ad)] - [Merge Microsoft/Chakracore@865393282c] [MERGE #3994 @boingoing] Change var slot allocation order to avoid loading special symbols from incorrect slots
* [[`84b2e378b5`](https://github.com/nodejs/node-chakracore/commit/84b2e378b5)] - [Merge Microsoft/Chakracore@f28ec6d047] [MERGE #4005 @MSLaguana] Improving handling of internal properties in JSON.stringify
* [[`46976d5f72`](https://github.com/nodejs/node-chakracore/commit/46976d5f72)] - Merge nodejs/master
* [[`24e76a225a`](https://github.com/nodejs/node-chakracore/commit/24e76a225a)] - Merge nodejs/master
* [[`d9429bcadc`](https://github.com/nodejs/node-chakracore/commit/d9429bcadc)] - [Merge Microsoft/Chakracore@625f1b8b81] [MERGE #3745 @xiaoyinl] [README.md] Use HTTPS link for ChakraCore-Samples
* [[`292b472bc9`](https://github.com/nodejs/node-chakracore/commit/292b472bc9)] - [Merge Microsoft/Chakracore@99bc5af01b] [MERGE #3927 @xiaoyinl] Add PROBE_STACK to JSONParser::Walk (Fix #3900)
* [[`3a65db4e71`](https://github.com/nodejs/node-chakracore/commit/3a65db4e71)] - Merge nodejs/master
* [[`9f7c3a5dcd`](https://github.com/nodejs/node-chakracore/commit/9f7c3a5dcd)] - test: disable new esm test
* [[`d35b64c7b1`](https://github.com/nodejs/node-chakracore/commit/d35b64c7b1)] - Merge nodejs/master
* [[`040de95117`](https://github.com/nodejs/node-chakracore/commit/040de95117)] - [Merge Microsoft/Chakracore@ddbe1a2ae8] [MERGE #3971 @boingoing] Do not load 'this' explicitly for class expression call target
* [[`70b46cb7dc`](https://github.com/nodejs/node-chakracore/commit/70b46cb7dc)] - test: mark flaky test, fix common decl
* [[`b836bfcb57`](https://github.com/nodejs/node-chakracore/commit/b836bfcb57)] - Merge nodejs/master
* [[`6611c5c272`](https://github.com/nodejs/node-chakracore/commit/6611c5c272)] - [Merge Microsoft/Chakracore@592555d780] [MERGE #3945 @boingoing] Remove 'this' binding for global and global lambda functions
* [[`1572284556`](https://github.com/nodejs/node-chakracore/commit/1572284556)] - [Merge Microsoft/Chakracore@ccf501ae93] [MERGE #3958 @Cellule] OS#14217306 Update Bytecode GUID
* [[`ea912bb05f`](https://github.com/nodejs/node-chakracore/commit/ea912bb05f)] - test: mark test-repl-pretty-stack tests flaky
* [[`90447b59c2`](https://github.com/nodejs/node-chakracore/commit/90447b59c2)] - Merge nodejs/master
* [[`f7b51b9754`](https://github.com/nodejs/node-chakracore/commit/f7b51b9754)] - test: mark flaky tests
* [[`4e5c00f6c2`](https://github.com/nodejs/node-chakracore/commit/4e5c00f6c2)] - Merge nodejs/master
* [[`2ee2b1e2f0`](https://github.com/nodejs/node-chakracore/commit/2ee2b1e2f0)] - [Merge Microsoft/Chakracore@ca08c59b17] [MERGE #3940 @boingoing] Correctly order special symbol initialization for default arguments
* [[`b54e97870a`](https://github.com/nodejs/node-chakracore/commit/b54e97870a)] - [Merge Microsoft/Chakracore@951b23f750] [MERGE #3932 @boingoing] Loosen an assert in `FindOrAddPidRef`
* [[`8fe55949d3`](https://github.com/nodejs/node-chakracore/commit/8fe55949d3)] - [Merge Microsoft/Chakracore@e3724ebb18] [MERGE #3944 @obastemur] Apply marshaling to property from slot
* [[`22b0806d4b`](https://github.com/nodejs/node-chakracore/commit/22b0806d4b)] - [Merge Microsoft/Chakracore@ea11759ba3] [MERGE #3923 @obastemur] json.stringify: simplify array interface and make sure object is dynamicObject
* [[`c4462527c2`](https://github.com/nodejs/node-chakracore/commit/c4462527c2)] - Merge nodejs/master
* [[`b51dc3856e`](https://github.com/nodejs/node-chakracore/commit/b51dc3856e)] - test: mark test-http2-server-rst-stream as flaky
* [[`12cb6ee693`](https://github.com/nodejs/node-chakracore/commit/12cb6ee693)] - Merge nodejs/master
* [[`6616b34ed3`](https://github.com/nodejs/node-chakracore/commit/6616b34ed3)] - [Merge Microsoft/Chakracore@ffe59396b0] [MERGE #3917 @agarwal-sandeep] 17-10 Security Update that addresses the following issues in ChakraCore
* [[`3e1b61ea30`](https://github.com/nodejs/node-chakracore/commit/3e1b61ea30)] - Merge nodejs/master
* [[`adbea0f539`](https://github.com/nodejs/node-chakracore/commit/adbea0f539)] - [Merge Microsoft/Chakracore@d234b08fd9] [MERGE #3904 @boingoing] Strict mode global lambda functions should use LdThis instead of StrictLdThis
* [[`2ae996d4cd`](https://github.com/nodejs/node-chakracore/commit/2ae996d4cd)] - Merge nodejs/master
* [[`c0981c1fd6`](https://github.com/nodejs/node-chakracore/commit/c0981c1fd6)] - Merge nodejs/master
* [[`2f0645b334`](https://github.com/nodejs/node-chakracore/commit/2f0645b334)] - Merge nodejs/master
* [[`9d58517497`](https://github.com/nodejs/node-chakracore/commit/9d58517497)] - Merge nodejs/master
* [[`4569fd2ebc`](https://github.com/nodejs/node-chakracore/commit/4569fd2ebc)] - chakrashim: shim new V8 api
* [[`ef71af534e`](https://github.com/nodejs/node-chakracore/commit/ef71af534e)] - Merge nodejs/master
* [[`40d100eeea`](https://github.com/nodejs/node-chakracore/commit/40d100eeea)] - Merge nodejs/master
* [[`71200b29fa`](https://github.com/nodejs/node-chakracore/commit/71200b29fa)] - test: disable LargeUtf8 napi test on chakra
* [[`f0a18bec25`](https://github.com/nodejs/node-chakracore/commit/f0a18bec25)] - Merge nodejs/master
* [[`dc0b960855`](https://github.com/nodejs/node-chakracore/commit/dc0b960855)] - lint: fix lint issues
* [[`6384ceb862`](https://github.com/nodejs/node-chakracore/commit/6384ceb862)] - napi_jsrt: initialize memory in napi_adjust_external_memory
* [[`6967b114de`](https://github.com/nodejs/node-chakracore/commit/6967b114de)] - Merge nodejs/master
* [[`b8454fc417`](https://github.com/nodejs/node-chakracore/commit/b8454fc417)] - [Merge Microsoft/Chakracore@13e0e02e3d] [MERGE #3894 @obastemur] perf: simplify stringify + GetOrAddPropertyRecord
* [[`0fbed91215`](https://github.com/nodejs/node-chakracore/commit/0fbed91215)] - [Merge Microsoft/Chakracore@40d6222ef2] [MERGE #3861 @obastemur] jsrt: JsConvertValueToString exit fast if arg. is JavascriptString
* [[`cc177729ee`](https://github.com/nodejs/node-chakracore/commit/cc177729ee)] - [Merge Microsoft/Chakracore@cb2b31e6f1] [MERGE #3892 @boingoing] Top level event handlers have an implicit 'this' binding
* [[`8275b21b74`](https://github.com/nodejs/node-chakracore/commit/8275b21b74)] - [Merge Microsoft/Chakracore@bb03416faf] [MERGE #3880 @obastemur] CrossSite: Fix property check
* [[`d7d956dd0e`](https://github.com/nodejs/node-chakracore/commit/d7d956dd0e)] - [Merge Microsoft/Chakracore@1202ba28d4] [MERGE #3878 @obastemur] fix: fails to compare to undefined on cross context
* [[`0be48b0ba7`](https://github.com/nodejs/node-chakracore/commit/0be48b0ba7)] - [Merge Microsoft/Chakracore@cb6868590e] [MERGE #3882 @obastemur] PropertyCache: assume property's context != object's context
* [[`4f7c99c85e`](https://github.com/nodejs/node-chakracore/commit/4f7c99c85e)] - [Merge Microsoft/Chakracore@4f6b659a41] [MERGE #3857 @jackhorton] Fix issue with calling JSON.stringify on an object with internal properties
* [[`8fc6067a72`](https://github.com/nodejs/node-chakracore/commit/8fc6067a72)] - [Merge Microsoft/Chakracore@802c6a2aae] [MERGE #3864 @obastemur] json: allow totalNumPropertyCount == 0 (assert)
* [[`3fcd7a308d`](https://github.com/nodejs/node-chakracore/commit/3fcd7a308d)] - [Merge Microsoft/Chakracore@c2ccf4f0a7] [1.6>1.7] [MERGE #3866 @Penguinwizzard] Re-enable RecyclerChecker runs
* [[`bc53a2e6d0`](https://github.com/nodejs/node-chakracore/commit/bc53a2e6d0)] - [Merge Microsoft/Chakracore@36e9f41ca2] [1.6>1.7] [MERGE #3854 @Penguinwizzard] Unblock CI
* [[`577d63c45a`](https://github.com/nodejs/node-chakracore/commit/577d63c45a)] - [Merge Microsoft/Chakracore@c2f92dd01e] [MERGE #3840 @dilijev] Intl-ICU: Merge branch `intl-icu`: Intl.NumberFormat and node-cc build integration into branch `release/1.7`
* [[`5384b59fa1`](https://github.com/nodejs/node-chakracore/commit/5384b59fa1)] - [Merge Microsoft/Chakracore@3f39bafe41] [1.6>1.7] [MERGE #3849 @MSLaguana] Updating CI to use clang 3.9 after CI image change.
* [[`233d9ddd4a`](https://github.com/nodejs/node-chakracore/commit/233d9ddd4a)] - napi: fix property string length
* [[`9ba2671582`](https://github.com/nodejs/node-chakracore/commit/9ba2671582)] - [Merge Microsoft/Chakracore@7f0403e0e8] [MERGE #3845 @obastemur] CanonicalizeAccessor returns wrong results on cross thread
* [[`6852c75c61`](https://github.com/nodejs/node-chakracore/commit/6852c75c61)] - [Merge Microsoft/Chakracore@d54ede6ce9] [MERGE #3731 @obastemur] pal: do not init PAL sync manager thread
* [[`b0da3179fc`](https://github.com/nodejs/node-chakracore/commit/b0da3179fc)] - [Merge Microsoft/Chakracore@32a6edd28c] [MERGE #3788 @obastemur] perf: Improve internal type discovery and getPropertyInternal
* [[`22ff05ea90`](https://github.com/nodejs/node-chakracore/commit/22ff05ea90)] - [Merge Microsoft/Chakracore@f90129c6d0] [MERGE #3758 @jackhorton] Make -v actually enable verbose makefiles
* [[`e6078df8ec`](https://github.com/nodejs/node-chakracore/commit/e6078df8ec)] - [Merge Microsoft/Chakracore@c6b3f201b1] [MERGE #3751 @obastemur] perf: Improve JSON.stringify performance
* [[`886f300a34`](https://github.com/nodejs/node-chakracore/commit/886f300a34)] - [Merge Microsoft/Chakracore@0678ae03e0] [MERGE #3842 @obastemur] wabt: delete wabt config.h
* [[`27b0196a1c`](https://github.com/nodejs/node-chakracore/commit/27b0196a1c)] - [Merge Microsoft/Chakracore@98454c40e9] [MERGE #3501 @obastemur] Perf Improvements
* [[`9d92d19889`](https://github.com/nodejs/node-chakracore/commit/9d92d19889)] - [Merge Microsoft/Chakracore@a9325a3ac1] [MERGE #3774 @obastemur] perf: less collision - less comparison
* [[`da8076836e`](https://github.com/nodejs/node-chakracore/commit/da8076836e)] - [Merge Microsoft/Chakracore@03feaf00c9] [MERGE #3806 @obastemur] OSX: Use the max between rsp vs USRSTACK**
* [[`f78c6941ae`](https://github.com/nodejs/node-chakracore/commit/f78c6941ae)] - [Merge Microsoft/Chakracore@61d8817fda] [MERGE #3799 @obastemur] Update min-max to GET_MIN-GET_MAX
* [[`8b22a65b5a`](https://github.com/nodejs/node-chakracore/commit/8b22a65b5a)] - test: unmarking test_general as flaky
* [[`6d975aedc5`](https://github.com/nodejs/node-chakracore/commit/6d975aedc5)] - test: fixing test_general to work with imprecise GC
* [[`0995107989`](https://github.com/nodejs/node-chakracore/commit/0995107989)] - test: disabling new unsupported es-module tests
* [[`3cab2f7265`](https://github.com/nodejs/node-chakracore/commit/3cab2f7265)] - Merge nodejs/master
* [[`d1741202f3`](https://github.com/nodejs/node-chakracore/commit/d1741202f3)] - [Merge Microsoft/Chakracore@965098b204] [MERGE #3835 @boingoing] Fix null dereference in LoadAllConstants when code has no 'this' binding
* [[`d6ff9c99bf`](https://github.com/nodejs/node-chakracore/commit/d6ff9c99bf)] - [Merge Microsoft/Chakracore@f40d571b5d] [MERGE #3725 @obastemur] xplat: improve virtual memory region lookups
* [[`a9f5394b01`](https://github.com/nodejs/node-chakracore/commit/a9f5394b01)] - Merge nodejs/master
* [[`beaa30c708`](https://github.com/nodejs/node-chakracore/commit/beaa30c708)] - Merge nodejs/master
* [[`c926e5841c`](https://github.com/nodejs/node-chakracore/commit/c926e5841c)] - Merge nodejs/master
* [[`5645545deb`](https://github.com/nodejs/node-chakracore/commit/5645545deb)] - Merge nodejs/master
* [[`aaa4557cd5`](https://github.com/nodejs/node-chakracore/commit/aaa4557cd5)] - test: fix bad merge
* [[`d608d2035d`](https://github.com/nodejs/node-chakracore/commit/d608d2035d)] - napi,jsrt: fixed erroneous version number when registering napi modules
* [[`55f0a0bfc8`](https://github.com/nodejs/node-chakracore/commit/55f0a0bfc8)] - test: fixing jslint issues
* [[`320dba0776`](https://github.com/nodejs/node-chakracore/commit/320dba0776)] - test: updating inspector tests to work or be marked as flaky
* [[`ee6df1f1c7`](https://github.com/nodejs/node-chakracore/commit/ee6df1f1c7)] - test,chakrashim: fixing v8::External objects to have no prototype
* [[`c091e2a811`](https://github.com/nodejs/node-chakracore/commit/c091e2a811)] - Merge nodejs/master
* [[`221c9de34a`](https://github.com/nodejs/node-chakracore/commit/221c9de34a)] - napi: updating chakra napi implementation to match new spec
* [[`03ac20b47a`](https://github.com/nodejs/node-chakracore/commit/03ac20b47a)] - Merge nodejs/master
* [[`670049aa9a`](https://github.com/nodejs/node-chakracore/commit/670049aa9a)] - test,build: fix merge test failures
* [[`4360d62f97`](https://github.com/nodejs/node-chakracore/commit/4360d62f97)] - chakrashim: shimming new apis
* [[`e1c180c525`](https://github.com/nodejs/node-chakracore/commit/e1c180c525)] - Merge nodejs/master
* [[`f6df153c05`](https://github.com/nodejs/node-chakracore/commit/f6df153c05)] - [Merge Microsoft/Chakracore@2f293c45df] [MERGE #3743 @boingoing] Binding for special named properties
* [[`7e91f99a9c`](https://github.com/nodejs/node-chakracore/commit/7e91f99a9c)] - [Merge Microsoft/Chakracore@a42c41d387] [MERGE #3768 @MSLaguana] Adding error checking to JsCreateWeakReference
* [[`7ca7c6cc49`](https://github.com/nodejs/node-chakracore/commit/7ca7c6cc49)] - build: added --with-intl flag on linux, android, and macOS
* [[`76593a56fd`](https://github.com/nodejs/node-chakracore/commit/76593a56fd)] - Merge nodejs/master
* [[`3c1d3f7676`](https://github.com/nodejs/node-chakracore/commit/3c1d3f7676)] - chakrashim: shim updated apis
* [[`a51443ece3`](https://github.com/nodejs/node-chakracore/commit/a51443ece3)] - Merge nodejs/master
* [[`b3585b4fa6`](https://github.com/nodejs/node-chakracore/commit/b3585b4fa6)] - napi: Fix break on linux
* [[`82e55e01cf`](https://github.com/nodejs/node-chakracore/commit/82e55e01cf)] - [Merge Microsoft/Chakracore@9753783bc9] [MERGE #3634 @xiaoyinl] Remove undefined functions in ScriptContext.h
* [[`7b439471ac`](https://github.com/nodejs/node-chakracore/commit/7b439471ac)] - [Merge Microsoft/Chakracore@2e48b602dd] [MERGE #3554 @xiaoyinl] Fix typos
* [[`405da295fe`](https://github.com/nodejs/node-chakracore/commit/405da295fe)] - [Merge Microsoft/Chakracore@281cfa876d] [MERGE #3569 @xiaoyinl] Update doc for JsCreateString(Utf16)
* [[`97a696a58d`](https://github.com/nodejs/node-chakracore/commit/97a696a58d)] - build: fix lint issues
* [[`c785c3ecf0`](https://github.com/nodejs/node-chakracore/commit/c785c3ecf0)] - test: Fix test breaks in node-chakracore
* [[`9ef12bbd55`](https://github.com/nodejs/node-chakracore/commit/9ef12bbd55)] - test: initial test failure fixes
* [[`8b93249afa`](https://github.com/nodejs/node-chakracore/commit/8b93249afa)] - napi: Update jsrt implementation
* [[`da9dce2767`](https://github.com/nodejs/node-chakracore/commit/da9dce2767)] - Merge nodejs/master
* [[`f33e171aed`](https://github.com/nodejs/node-chakracore/commit/f33e171aed)] - build: fix lint issues
* [[`258df302ad`](https://github.com/nodejs/node-chakracore/commit/258df302ad)] - test: fixing AliasedBuffer tests to enter Isolate
* [[`8f2b2e2d5b`](https://github.com/nodejs/node-chakracore/commit/8f2b2e2d5b)] - napi: Fix jsrt implementation of napi_create_async_work
* [[`f7144b56b6`](https://github.com/nodejs/node-chakracore/commit/f7144b56b6)] - test,build: implement no-ops for Promise::Resolver methods, mark tests as flaky
* [[`00d5417765`](https://github.com/nodejs/node-chakracore/commit/00d5417765)] - Merge nodejs/master
* [[`b4d318e35e`](https://github.com/nodejs/node-chakracore/commit/b4d318e35e)] - ttd: adding ttd tracking logic to aliased_buffer
* [[`e7cf9f81e2`](https://github.com/nodejs/node-chakracore/commit/e7cf9f81e2)] - [Merge Microsoft/Chakracore@8bc3d1afe8] [MERGE #3734 @sigatrev] OS:13674598 use type specialized sym for upward exposed uses check
* [[`3b5981feac`](https://github.com/nodejs/node-chakracore/commit/3b5981feac)] - marking test-http2-client-promisify-connect as failing
* [[`76fda577b5`](https://github.com/nodejs/node-chakracore/commit/76fda577b5)] - Merge nodejs/master
* [[`7f6cac3721`](https://github.com/nodejs/node-chakracore/commit/7f6cac3721)] - env:  fixing line endings
* [[`c20ed4e370`](https://github.com/nodejs/node-chakracore/commit/c20ed4e370)] - ttd: undoing custom TTD tracking for async-wrap
* [[`918e435e63`](https://github.com/nodejs/node-chakracore/commit/918e435e63)] - Updating flaky status for moved test
* [[`1a55d558ff`](https://github.com/nodejs/node-chakracore/commit/1a55d558ff)] - Merge nodejs/master
* [[`18d71bac1d`](https://github.com/nodejs/node-chakracore/commit/18d71bac1d)] - napi: adding napi_run_script support for chakracore
* [[`06d06cbd4b`](https://github.com/nodejs/node-chakracore/commit/06d06cbd4b)] - Merge nodejs/master
* [[`41e2789bef`](https://github.com/nodejs/node-chakracore/commit/41e2789bef)] - [Merge Microsoft/Chakracore@190c0f01a6] [MERGE #3635 @xiaoyinl] Numbers up to 10^21 shouldn't be displayed exponentially (Fixes #2751)
* [[`774fa24d49`](https://github.com/nodejs/node-chakracore/commit/774fa24d49)] - [Merge Microsoft/Chakracore@1d7a5fe423] [MERGE #3736 @dilijev] OS#13255820 OS#13255822: Fix uninitialized locals and check return value of function that initializes them.
* [[`f4b4e13bfd`](https://github.com/nodejs/node-chakracore/commit/f4b4e13bfd)] - [Merge Microsoft/Chakracore@d19bf32818] [MERGE #3724 @dilijev] Revert non-optimal pattern in Intl.js
* [[`f859c1e3c6`](https://github.com/nodejs/node-chakracore/commit/f859c1e3c6)] - [Merge Microsoft/Chakracore@ee46fc449c] [MERGE #3732 @MSLaguana] Preparing for release 1.7.2
* [[`c596ca5dca`](https://github.com/nodejs/node-chakracore/commit/c596ca5dca)] - [Merge Microsoft/Chakracore@0709816282] [MERGE #3729 @suwc] 17-09 ChakraCore servicing release
* [[`561476a0ee`](https://github.com/nodejs/node-chakracore/commit/561476a0ee)] - [Merge Microsoft/Chakracore@e905806855] [MERGE #3682 @dilijev] Fix #2987: Per spec, skip over holes when iterating over localeList
* [[`de917cb050`](https://github.com/nodejs/node-chakracore/commit/de917cb050)] - [Merge Microsoft/Chakracore@8dd1b47365] [MERGE #3721 @jianchun] fix incorrect parameter to call ChangeStringLinguisticCase
* [[`e1b78151d9`](https://github.com/nodejs/node-chakracore/commit/e1b78151d9)] - [Merge Microsoft/Chakracore@860ad1275e] [MERGE #3719 @jianchun] fix JavascriptFunction::CheckValidDebugThunk CrossSite check
* [[`d251636ac2`](https://github.com/nodejs/node-chakracore/commit/d251636ac2)] - [Merge Microsoft/Chakracore@6c11e16af2] [MERGE #3707 @sigatrev] Remove unneeded symbol that is breaking RecordInlineeFrameInfo copy prop
* [[`a8c98f91f7`](https://github.com/nodejs/node-chakracore/commit/a8c98f91f7)] - [Merge Microsoft/Chakracore@f17e37db34] [MERGE #3680 @dilijev] Intl-ICU: Add LanguageTag RegExp; make Intl.*.supportedLocalesOf work under ICU
* [[`bda058019d`](https://github.com/nodejs/node-chakracore/commit/bda058019d)] - [Merge Microsoft/Chakracore@d655df5191] [MERGE #3712 @obastemur] fix recyclerList check failing on multi-thread usage
* [[`5dcfd2b018`](https://github.com/nodejs/node-chakracore/commit/5dcfd2b018)] - Merge nodejs/master
* [[`b748ade9b2`](https://github.com/nodejs/node-chakracore/commit/b748ade9b2)] - Fixing promise test to not require unhandled promise rejections
* [[`470cb487fc`](https://github.com/nodejs/node-chakracore/commit/470cb487fc)] - chakrashim: fixing build break with stub v8::Module shim
* [[`a7e5d14488`](https://github.com/nodejs/node-chakracore/commit/a7e5d14488)] - Merge nodejs/master
* [[`44c368e740`](https://github.com/nodejs/node-chakracore/commit/44c368e740)] - [Merge Microsoft/Chakracore@edbeb866a5] [MERGE #3516 @obastemur] perf: Improve hot MOD arithmetic
* [[`3bb49ae60e`](https://github.com/nodejs/node-chakracore/commit/3bb49ae60e)] - [Merge Microsoft/Chakracore@4f07a58eec] [MERGE #3607 @MSLaguana] Adding a new enumerator flag to support short-term accesses
* [[`54afc678c3`](https://github.com/nodejs/node-chakracore/commit/54afc678c3)] - [Merge Microsoft/Chakracore@743be56892] [MERGE #3702 @sigatrev] Call SetInternalProperty and fix flags
* [[`d0491cec84`](https://github.com/nodejs/node-chakracore/commit/d0491cec84)] - [Merge Microsoft/Chakracore@daa3fc0313] [MERGE #3604 @obastemur] perf: reduce collision in hot maps
* [[`c0ab65f8b3`](https://github.com/nodejs/node-chakracore/commit/c0ab65f8b3)] - [Merge Microsoft/Chakracore@9264e37437] [MERGE #3605 @obastemur] perf: use wmemcpy instead of memcpy for wide strings
* [[`d52560813c`](https://github.com/nodejs/node-chakracore/commit/d52560813c)] - [Merge Microsoft/Chakracore@d4c8561029] [MERGE #3541 @obastemur] Better Debug coverage, smaller binary size
* [[`8042c2995f`](https://github.com/nodejs/node-chakracore/commit/8042c2995f)] - Merge nodejs/master
* [[`96c79098b3`](https://github.com/nodejs/node-chakracore/commit/96c79098b3)] - Merge nodejs/master
* [[`022a42df4f`](https://github.com/nodejs/node-chakracore/commit/022a42df4f)] - test: skip failing test on chakracore
* [[`39523bb3b4`](https://github.com/nodejs/node-chakracore/commit/39523bb3b4)] - chakrashim: added v8::Value::IsNullOrUndefined
* [[`be461f4efd`](https://github.com/nodejs/node-chakracore/commit/be461f4efd)] - Merge nodejs/master
* [[`353d3d517a`](https://github.com/nodejs/node-chakracore/commit/353d3d517a)] - Merge nodejs/master
* [[`7220aaa118`](https://github.com/nodejs/node-chakracore/commit/7220aaa118)] - [Merge Microsoft/Chakracore@cfebb39b75] [MERGE #3684 @digitalinfinity] Fix broken config parsing in ChakraFull
* [[`84298010ce`](https://github.com/nodejs/node-chakracore/commit/84298010ce)] - Merge nodejs/master
* [[`4bfd54ec25`](https://github.com/nodejs/node-chakracore/commit/4bfd54ec25)] - Merge nodejs/master
* [[`a5a5a1e57e`](https://github.com/nodejs/node-chakracore/commit/a5a5a1e57e)] - Merge nodejs/master
* [[`070fe0f475`](https://github.com/nodejs/node-chakracore/commit/070fe0f475)] - test: fix node-chakracore tests on linux
* [[`8524de53b8`](https://github.com/nodejs/node-chakracore/commit/8524de53b8)] - n-api: implement stub for adjust_external_memory
* [[`676e81390f`](https://github.com/nodejs/node-chakracore/commit/676e81390f)] - Merge nodejs/master
* [[`154a03e7da`](https://github.com/nodejs/node-chakracore/commit/154a03e7da)] - Merge nodejs/master
* [[`58217d2354`](https://github.com/nodejs/node-chakracore/commit/58217d2354)] - Merge nodejs/master
* [[`b5716076e6`](https://github.com/nodejs/node-chakracore/commit/b5716076e6)] - Merge nodejs/master
* [[`9a8953d003`](https://github.com/nodejs/node-chakracore/commit/9a8953d003)] - Merge nodejs/master
* [[`85767d6de3`](https://github.com/nodejs/node-chakracore/commit/85767d6de3)] - chakrashim: updating use of JsCopyString for perf
* [[`4a122aa929`](https://github.com/nodejs/node-chakracore/commit/4a122aa929)] - [Merge Microsoft/Chakracore@7b7ddfe8f3] [MERGE #3609 @MSLaguana] Changing utf8 conversion codex to improve perf and safety
* [[`3712f08a00`](https://github.com/nodejs/node-chakracore/commit/3712f08a00)] - test: fix breaking tests
* [[`65cc69dbc4`](https://github.com/nodejs/node-chakracore/commit/65cc69dbc4)] - Merge nodejs/master
* [[`094ccb1a4e`](https://github.com/nodejs/node-chakracore/commit/094ccb1a4e)] - [Merge Microsoft/Chakracore@09cf407949] [MERGE #3590 @mrkmarron] TTD: Fix leak on top-level function load and bug in external function inflate.
* [[`f839c0b546`](https://github.com/nodejs/node-chakracore/commit/f839c0b546)] - [Merge Microsoft/Chakracore@fcf9a18f07] [MERGE #3608 @Penguinwizzard] Add a flag to use a stronger array sort for bug analysis.
* [[`bca99b030b`](https://github.com/nodejs/node-chakracore/commit/bca99b030b)] - [Merge Microsoft/Chakracore@0df53e5b62] [MERGE #3578 @meg-gupta] Dont update valueInfo of bytecode constants
* [[`f3ad3e0998`](https://github.com/nodejs/node-chakracore/commit/f3ad3e0998)] - [Merge Microsoft/Chakracore@1262d2ea6d] [1.6>1.7] [MERGE #3583 @rajatd] Allow cross-site objects too in ActivationObjectEx::Is
* [[`55fe1adfb3`](https://github.com/nodejs/node-chakracore/commit/55fe1adfb3)] - [Merge Microsoft/Chakracore@a773e37180] [MERGE #3598 @Cellule] Fix #3597
* [[`2af05eb9ec`](https://github.com/nodejs/node-chakracore/commit/2af05eb9ec)] - test: fix line endings for test/addons-napi/test_constructor
* [[`e218ee3042`](https://github.com/nodejs/node-chakracore/commit/e218ee3042)] - test: unmark addons-napi/test_promise as flaky
* [[`3e706ba35d`](https://github.com/nodejs/node-chakracore/commit/3e706ba35d)] - napi_jsrt: implement promise
* [[`dcb87d42e5`](https://github.com/nodejs/node-chakracore/commit/dcb87d42e5)] - test: mark addons-napi/test_promise as flaky
* [[`9249837ca7`](https://github.com/nodejs/node-chakracore/commit/9249837ca7)] - Merge nodejs/master
* [[`3f0ef26ada`](https://github.com/nodejs/node-chakracore/commit/3f0ef26ada)] - [Merge Microsoft/Chakracore@4bf71d047f] [MERGE #3587 @rajatd] Setting internal properties with
* [[`e27d1b32a3`](https://github.com/nodejs/node-chakracore/commit/e27d1b32a3)] - [Merge Microsoft/Chakracore@e5c80f382f] [MERGE #3564 @dilijev] Fix space between , and exclude_xplat (rl doesn't handle spaces)
* [[`b344ac7ce5`](https://github.com/nodejs/node-chakracore/commit/b344ac7ce5)] - [Merge Microsoft/Chakracore@e2345e7d4a] [1.6>1.7] [MERGE #3584 @thomasmo] OS#13419689 - DOMFastPathGetter instructions do not propagate destination profile data, adding overhead to inlined getter calls
* [[`7cda2d1078`](https://github.com/nodejs/node-chakracore/commit/7cda2d1078)] - test: fixing new test issues
* [[`ed7b578fe6`](https://github.com/nodejs/node-chakracore/commit/ed7b578fe6)] - n-api: add napi_remove_wrap using JSRT
* [[`82742cd848`](https://github.com/nodejs/node-chakracore/commit/82742cd848)] - chakrashim: added missing Function methods
* [[`df059be1fb`](https://github.com/nodejs/node-chakracore/commit/df059be1fb)] - Merge nodejs/master
* [[`6bb5c009ac`](https://github.com/nodejs/node-chakracore/commit/6bb5c009ac)] - [Merge Microsoft/Chakracore@fe2ddd367e] [MERGE #3577 @meg-gupta] Remove additional edges in try finally flowgraph transformation
* [[`d2426a0c66`](https://github.com/nodejs/node-chakracore/commit/d2426a0c66)] - [Merge Microsoft/Chakracore@4a6e233be6] [MERGE #3576 @pleath] Clean up hasDeferredChild, hasRedeferrableChild, and IsRedeferrable() from FuncInfo.
* [[`3ad4aede73`](https://github.com/nodejs/node-chakracore/commit/3ad4aede73)] - Merge nodejs/master
* [[`624b083eee`](https://github.com/nodejs/node-chakracore/commit/624b083eee)] - Merge nodejs/master
* [[`37c49dd434`](https://github.com/nodejs/node-chakracore/commit/37c49dd434)] - test: marking tests as flaky after merge
* [[`a42597e1f5`](https://github.com/nodejs/node-chakracore/commit/a42597e1f5)] - chakrashim: make inspector asyc methods no-op
* [[`69899137a8`](https://github.com/nodejs/node-chakracore/commit/69899137a8)] - chakrashim: update platform implementation
* [[`965b52d459`](https://github.com/nodejs/node-chakracore/commit/965b52d459)] - Merge nodejs/master
* [[`7f362734e2`](https://github.com/nodejs/node-chakracore/commit/7f362734e2)] - [Merge Microsoft/Chakracore@b0c58262f5] Use Phase instead of boolean flag for GloboptInstrString dump
* [[`02a78aee65`](https://github.com/nodejs/node-chakracore/commit/02a78aee65)] - [Merge Microsoft/Chakracore@f75c14a5d2] [MERGE #3560 @Cellule] WASM: Spec fixes + improve debugging
* [[`a2c870c2b9`](https://github.com/nodejs/node-chakracore/commit/a2c870c2b9)] - [Merge Microsoft/Chakracore@c966983c14] [MERGE #3570 @Cellule] -on:debugbreak in test builds
* [[`cc446d2cd8`](https://github.com/nodejs/node-chakracore/commit/cc446d2cd8)] - [Merge Microsoft/Chakracore@4f8f11f61b] [MERGE #3571 @Cellule] Globopt Instr string config flag
* [[`dc313a3897`](https://github.com/nodejs/node-chakracore/commit/dc313a3897)] - [Merge Microsoft/Chakracore@6479e1b964] [1.6>1.7] [MERGE #3536 @meg-gupta] Do not peep closure stack symbols
* [[`9b55f8c538`](https://github.com/nodejs/node-chakracore/commit/9b55f8c538)] - [Merge Microsoft/Chakracore@94db98ccf2] [MERGE #3547 @Cellule] WASM: Update wabt and spec tests
* [[`cc3a8042f2`](https://github.com/nodejs/node-chakracore/commit/cc3a8042f2)] - [Merge Microsoft/Chakracore@789ab367c7] [MERGE #3526 @sigatrev] OS#9030969 fix stack trace with jitted loops
* [[`e6c513a7b6`](https://github.com/nodejs/node-chakracore/commit/e6c513a7b6)] - Fixing lint issue in jsrtisolateshim.cc
* [[`69337a6c0c`](https://github.com/nodejs/node-chakracore/commit/69337a6c0c)] - doc: fix typo in cli.md
* [[`77b2170f97`](https://github.com/nodejs/node-chakracore/commit/77b2170f97)] - test: preserve env in test cases
* [[`9fdf911587`](https://github.com/nodejs/node-chakracore/commit/9fdf911587)] - build: remove duplicated code
* [[`4205c96de0`](https://github.com/nodejs/node-chakracore/commit/4205c96de0)] - http2: handful of http/2 src cleanups
* [[`ae035c62a2`](https://github.com/nodejs/node-chakracore/commit/ae035c62a2)] - tools: delete an unused argument
* [[`51af17bd18`](https://github.com/nodejs/node-chakracore/commit/51af17bd18)] - http: don't double-fire the req error event
* [[`57604d1c33`](https://github.com/nodejs/node-chakracore/commit/57604d1c33)] - doc: added napi_get_value_string_latin1
* [[`221e72dc19`](https://github.com/nodejs/node-chakracore/commit/221e72dc19)] - test: refactor async-hooks/test-httparser tests
* [[`d0b6370188`](https://github.com/nodejs/node-chakracore/commit/d0b6370188)] - http2: Expose Http2ServerRequest/Response
* [[`c717daef0b`](https://github.com/nodejs/node-chakracore/commit/c717daef0b)] - doc: fix word wrapping for api stability boxes
* [[`e0cf4c8aa3`](https://github.com/nodejs/node-chakracore/commit/e0cf4c8aa3)] - test: use reserved invalid hostname for tests
* [[`e44983b5bc`](https://github.com/nodejs/node-chakracore/commit/e44983b5bc)] - test: add missing console.error to exec-maxBuffer
* [[`0da2820956`](https://github.com/nodejs/node-chakracore/commit/0da2820956)] - tools: checkout for unassigned DEP00XX codes
* [[`ae5075f239`](https://github.com/nodejs/node-chakracore/commit/ae5075f239)] - 2017-08-15, Version 8.4.0 (Current)
* [[`5e016439c0`](https://github.com/nodejs/node-chakracore/commit/5e016439c0)] - chakrashim: ref count data sent to SetEmbedderData
* [[`fd0d4a3356`](https://github.com/nodejs/node-chakracore/commit/fd0d4a3356)] - chakrashim: removing ObjectPrototypeToStringShim
* [[`e6cc414876`](https://github.com/nodejs/node-chakracore/commit/e6cc414876)] - Fixing test failures after merge
* [[`f5ce7f07b6`](https://github.com/nodejs/node-chakracore/commit/f5ce7f07b6)] - Merge nodejs/master
* [[`a18199857c`](https://github.com/nodejs/node-chakracore/commit/a18199857c)] - [Merge Microsoft/Chakracore@a4f0672dd0] [MERGE #3550 @xiaoyinl] Fix illegal qualified name in ThreadContext.h (Fix #3507)
* [[`1a928c9641`](https://github.com/nodejs/node-chakracore/commit/1a928c9641)] - [Merge Microsoft/Chakracore@241da53bd4] [1.6>1.7] [MERGE #3546 @MikeHolman] disable asm.js in ES6 modules
* [[`d24abe93de`](https://github.com/nodejs/node-chakracore/commit/d24abe93de)] - [Merge Microsoft/Chakracore@497c38192d] [MERGE #3232 @xiaoyinl] Fix file handle leak in Helpers.cpp
* [[`430122c04c`](https://github.com/nodejs/node-chakracore/commit/430122c04c)] - [Merge Microsoft/Chakracore@08803454cc] [MERGE #3548 @Cellule] Put back assert that was removed in #3537
* [[`c186f8f08f`](https://github.com/nodejs/node-chakracore/commit/c186f8f08f)] - [Merge Microsoft/Chakracore@71d8959010] [MERGE #3235 @xiaoyinl] Fix memory leak in AutoSystemInfo::InitPhysicalProcessorCount
* [[`7a49ba490a`](https://github.com/nodejs/node-chakracore/commit/7a49ba490a)] - disabling inspector/test-contexts because it blocks CI
* [[`ed8e2060d5`](https://github.com/nodejs/node-chakracore/commit/ed8e2060d5)] - Fixing lint issues
* [[`2f9535c3ba`](https://github.com/nodejs/node-chakracore/commit/2f9535c3ba)] - test: fixing test_constructor
* [[`40abd9c535`](https://github.com/nodejs/node-chakracore/commit/40abd9c535)] - Merge nodejs/master
* [[`dfeb1a210b`](https://github.com/nodejs/node-chakracore/commit/dfeb1a210b)] - [Merge Microsoft/Chakracore@4014154daa] [MERGE #3240 @xiaoyinl] Fix memory leak in WScriptJsrt::LoadTextFileCallback
* [[`38f7761907`](https://github.com/nodejs/node-chakracore/commit/38f7761907)] - [Merge Microsoft/Chakracore@08324e3ada] [MERGE #3539 @Cellule] OS#13149792 WASM: Table grow(0) with initial: 0
* [[`f2c4f34e7d`](https://github.com/nodejs/node-chakracore/commit/f2c4f34e7d)] - [Merge Microsoft/Chakracore@1548eeaf78] [MERGE #3544 @MikeHolman] fix bugs in MemoryOperationLastError
* [[`9a5c085d20`](https://github.com/nodejs/node-chakracore/commit/9a5c085d20)] - [Merge Microsoft/Chakracore@872220a98a] [1.6>1.7] OS#13129251: Math.min/max should return an integer value when all of its params are integers This change addresses a perf issue where non-inlined Math.min/max are always floating-point vars. In the bug, this causes expensive bailouts in a loop that was setting to a Uin8ClampedArray. The fix is to check whether all of the parameters are tagged integers, and, if so, return an int. With a reduced repro of the scenario from the original page, there is a significant improvement, where the same function takes 20% of the time it did before. Normal usage of Math.max with 3 int parameters set to a var results in taking 65% of the time it did before. Normal usage of Math.max with a float parameter showed a 1-5% regression, depending on where the first non-int parameter is listed.
* [[`13a534bf78`](https://github.com/nodejs/node-chakracore/commit/13a534bf78)] - [Merge Microsoft/Chakracore@911cfcb915] [MERGE #3538 @sigatrev] OS:10898061 fix bug with cached scopes and default/destrctured arguments
* [[`ba9f047844`](https://github.com/nodejs/node-chakracore/commit/ba9f047844)] - napi_jsrt: adding napi_get_node_version
* [[`8b9f135d29`](https://github.com/nodejs/node-chakracore/commit/8b9f135d29)] - Merge nodejs/master
* [[`5aa5042ea2`](https://github.com/nodejs/node-chakracore/commit/5aa5042ea2)] - [Merge Microsoft/Chakracore@3103d16574] [MERGE #3512 @Cellule] OOM / SO exception counter
* [[`72a80d8e1b`](https://github.com/nodejs/node-chakracore/commit/72a80d8e1b)] - [Merge Microsoft/Chakracore@4d0e8a6303] [MERGE #3537 @Cellule] Prioritize integer type in Lifetime
* [[`2d113bc847`](https://github.com/nodejs/node-chakracore/commit/2d113bc847)] - [Merge Microsoft/Chakracore@a662302d6b] [MERGE #3514 @Cellule] WASM - preserve nan bits
* [[`541e6c4179`](https://github.com/nodejs/node-chakracore/commit/541e6c4179)] - [Merge Microsoft/Chakracore@b8c3f5e761] [MERGE #3535 @Cellule] Cleanup FncFlags
* [[`1b94b02ceb`](https://github.com/nodejs/node-chakracore/commit/1b94b02ceb)] - [Merge Microsoft/Chakracore@1e02d86605] [MERGE #3534 @MikeHolman] change how we shift addresses from JIT proc to be more futureproof
* [[`4bb7a0e7a3`](https://github.com/nodejs/node-chakracore/commit/4bb7a0e7a3)] - [Merge Microsoft/Chakracore@84fd5b247b] [MERGE #3529 @Cellule] OS#11576900 Globopt array compensation
* [[`da67bd3b19`](https://github.com/nodejs/node-chakracore/commit/da67bd3b19)] - [Merge Microsoft/Chakracore@b78c03930b] [1.6>1.7] [MERGE #3522 @tcare] Fix #2983 WeakMap + HostObject === Sadness (TypeError when using HostObject with WeakMap)
* [[`c95552f44a`](https://github.com/nodejs/node-chakracore/commit/c95552f44a)] - [Merge Microsoft/Chakracore@6a23c85c43] [1.6>1.7] [MERGE #3533 @jianchun] module circular reference GetExportedNames/ResolveExport bugs
* [[`95e0e9d928`](https://github.com/nodejs/node-chakracore/commit/95e0e9d928)] - Fixing up improperly ignored test
* [[`72c4ba2d41`](https://github.com/nodejs/node-chakracore/commit/72c4ba2d41)] - Merge nodejs/master
* [[`0f8f365b06`](https://github.com/nodejs/node-chakracore/commit/0f8f365b06)] - [Merge Microsoft/Chakracore@f2ff94fb6e] [1.6>1.7] [MERGE #3523 @suwc] Fix issue#3245: Semicolon should not be required after certain module export forms
* [[`a7ef8c4df7`](https://github.com/nodejs/node-chakracore/commit/a7ef8c4df7)] - [Merge Microsoft/Chakracore@2468ce3627] [MERGE #3399 @dilijev] Make toGMTString the same function object as toUTCString
* [[`7fa476d632`](https://github.com/nodejs/node-chakracore/commit/7fa476d632)] - [Merge Microsoft/Chakracore@cb9be1fe92] [MERGE #3528 @MikeHolman] fix double print in trace:backend
* [[`7dd3358d6e`](https://github.com/nodejs/node-chakracore/commit/7dd3358d6e)] - Merge nodejs/master
* [[`9860eb9c79`](https://github.com/nodejs/node-chakracore/commit/9860eb9c79)] - [Merge Microsoft/Chakracore@9ac1422978] [MERGE #3498 @ricobbe] OS:9682944 Fix bug in which we incorrectly copy-propagated away a load from a stack slot after boxing locals onto the heap
* [[`65a260edc9`](https://github.com/nodejs/node-chakracore/commit/65a260edc9)] - [Merge Microsoft/Chakracore@b7cbc50012] [1.6>1.7] [MERGE #3493 @MikeHolman] remove inconsistent check in BuildJITTimeData
* [[`a0e62efc1f`](https://github.com/nodejs/node-chakracore/commit/a0e62efc1f)] - [Merge Microsoft/Chakracore@8eacc83904] [1.6>1.7] [MERGE #3488 @akroshg] OS: 11478866 adding a probe_stack to one of GetPrototypeSpecial API.
* [[`3fcd66e454`](https://github.com/nodejs/node-chakracore/commit/3fcd66e454)] - [Merge Microsoft/Chakracore@0b6a59148a] [1.6>1.7] [MERGE #3518 @Cellule] DetermineSymBoundOffsetOrValueRelativeToLandingPad: Handle case where we are unable to find the matching bound
* [[`8a7b363e80`](https://github.com/nodejs/node-chakracore/commit/8a7b363e80)] - [Merge Microsoft/Chakracore@47ae4a21b0] [1.6>1.7] [MERGE #3486 @Cellule] WASM - fix assert in jsrt debugger
* [[`550e98b150`](https://github.com/nodejs/node-chakracore/commit/550e98b150)] - [Merge Microsoft/Chakracore@b7e0e7ccb8] [1.6>1.7] [MERGE #3510 @Cellule] WASM: strengthen defer parse
* [[`cd1dc7a0ac`](https://github.com/nodejs/node-chakracore/commit/cd1dc7a0ac)] - [Merge Microsoft/Chakracore@37742786a9] [1.6>1.7] [MERGE #3515 @MikeHolman] fix padding for rpc structs
* [[`500fbd2c0b`](https://github.com/nodejs/node-chakracore/commit/500fbd2c0b)] - [Merge Microsoft/Chakracore@04cd94e2e1] [1.6>1.7] [MERGE #3499 @meg-gupta] Retain subclass valueinfo while optimizing BailOnNotObject in OptTagChecks
* [[`aea72b88e2`](https://github.com/nodejs/node-chakracore/commit/aea72b88e2)] - [Merge Microsoft/Chakracore@68cc4e864c] [1.6>1.7] [MERGE #3504 @meg-gupta] Fix IV analysis when a loop modifies an induction variable of another sibling loop sharing the same loop parent
* [[`fd0944ad44`](https://github.com/nodejs/node-chakracore/commit/fd0944ad44)] - [Merge Microsoft/Chakracore@7fc3e4ce9a] [1.6>1.7] [MERGE #3336 @suwc] Fix Issue#3064/#3423: Cache conflicts in super property access
* [[`dc0f0d7baf`](https://github.com/nodejs/node-chakracore/commit/dc0f0d7baf)] - Merge nodejs/master
* [[`8c646a00ef`](https://github.com/nodejs/node-chakracore/commit/8c646a00ef)] - Merge nodejs/master
* [[`14a7dfd3a7`](https://github.com/nodejs/node-chakracore/commit/14a7dfd3a7)] - [Merge Microsoft/Chakracore@e2e415d467] [MERGE #3511 @MSLaguana] Preparing for release 1.7.1
* [[`96f3d3182a`](https://github.com/nodejs/node-chakracore/commit/96f3d3182a)] - [Merge Microsoft/Chakracore@b1e30e38a9] [MERGE #3487 @Cellule] WASM - improve some error messages
* [[`1975118347`](https://github.com/nodejs/node-chakracore/commit/1975118347)] - [Merge Microsoft/Chakracore@f092881abe] [MERGE #3506 @Cellule] Default case in InliningDecider::GetBuiltInInfoCommon
* [[`901e089e6b`](https://github.com/nodejs/node-chakracore/commit/901e089e6b)] - [Merge Microsoft/Chakracore@d08f260e7e] [1.6>1.7] [MERGE #3509 @pleath] 17-08 ChakraCore servicing release
* [[`243ce933fa`](https://github.com/nodejs/node-chakracore/commit/243ce933fa)] - test: fixing test breaks for chakracore
* [[`e8134715d8`](https://github.com/nodejs/node-chakracore/commit/e8134715d8)] - Merge nodejs/master
* [[`2cd82b3d93`](https://github.com/nodejs/node-chakracore/commit/2cd82b3d93)] - [Merge Microsoft/Chakracore@f2b5bb29b9] [1.6>1.7] [MERGE #3468 @Cellule] Strengthen WebAssemblyArrayBuffer.GrowMemory
* [[`b826cd83ab`](https://github.com/nodejs/node-chakracore/commit/b826cd83ab)] - [Merge Microsoft/Chakracore@992b1d7a50] [1.6>1.7] [MERGE #3491 @Cellule] WASM - binary version 0xd
* [[`5ab8f8505f`](https://github.com/nodejs/node-chakracore/commit/5ab8f8505f)] - [Merge Microsoft/Chakracore@e0141fb48b] [MERGE #3503 @sigatrev] Fix Issue #3497: move initialization to constructor
* [[`08d5ff1ea3`](https://github.com/nodejs/node-chakracore/commit/08d5ff1ea3)] - Merge branch 'master' of https://github.com/nodejs/node-chakracore into _NodeJsFIPump_temp__08082017162152
* [[`151758e7e2`](https://github.com/nodejs/node-chakracore/commit/151758e7e2)] - Fixed inspector test
* [[`8038ab44fa`](https://github.com/nodejs/node-chakracore/commit/8038ab44fa)] - Merge nodejs/master
* [[`4abaa11d3b`](https://github.com/nodejs/node-chakracore/commit/4abaa11d3b)] - Merge nodejs/master
* [[`a6205d39d5`](https://github.com/nodejs/node-chakracore/commit/a6205d39d5)] - Merge nodejs/master
* [[`6f1ce57c79`](https://github.com/nodejs/node-chakracore/commit/6f1ce57c79)] - [Merge Microsoft/Chakracore@c19c1c0b33] [1.6>1.7] [MERGE #3424 @rajatd] OS#9726517 : Give NewScFunc the correct nested function index when we transfrom GetCachedFunc to NewScFunc in the backend
* [[`c421c9a503`](https://github.com/nodejs/node-chakracore/commit/c421c9a503)] - [Merge Microsoft/Chakracore@895f892129] [MERGE #3489 @obastemur] Convert internal static initializer (thread safe) to global
* [[`470fbc1ca6`](https://github.com/nodejs/node-chakracore/commit/470fbc1ca6)] - [Merge Microsoft/Chakracore@1b94b578c3] [MERGE #3455 @obastemur] JSON: Minor improvements
* [[`21a5850b50`](https://github.com/nodejs/node-chakracore/commit/21a5850b50)] - [Merge Microsoft/Chakracore@5dd3ed0264] [MERGE #3466 @obastemur] ToStringTagHelper: Make sure CompoundString has enough capacity
* [[`ad75cf4b76`](https://github.com/nodejs/node-chakracore/commit/ad75cf4b76)] - [Merge Microsoft/Chakracore@4559734d46] [MERGE #3454 @obastemur] Performance Improvements.
* [[`fc63967b96`](https://github.com/nodejs/node-chakracore/commit/fc63967b96)] - [Merge Microsoft/Chakracore@ac21f6c301] [MERGE #3420 @obastemur] linux: free datetime interface from tz
* [[`eae5a4f31e`](https://github.com/nodejs/node-chakracore/commit/eae5a4f31e)] - [Merge Microsoft/Chakracore@9c034052fa] [MERGE #3472 @obastemur] JSON: allocate enough cache
* [[`3661e26d30`](https://github.com/nodejs/node-chakracore/commit/3661e26d30)] - [Merge Microsoft/Chakracore@48de84a45f] [MERGE #3473 @MSLaguana] Fixing ninja builds as part of parent make
* [[`acc838e98c`](https://github.com/nodejs/node-chakracore/commit/acc838e98c)] - test: update `areAllValuesEqual` to use strings
* [[`0bac21f222`](https://github.com/nodejs/node-chakracore/commit/0bac21f222)] - test: fixed unit test
* [[`745709fccb`](https://github.com/nodejs/node-chakracore/commit/745709fccb)] - Merge nodejs/master
* [[`357fdf6a0f`](https://github.com/nodejs/node-chakracore/commit/357fdf6a0f)] - Merge nodejs/master
* [[`c97ccb6d21`](https://github.com/nodejs/node-chakracore/commit/c97ccb6d21)] - test: fixed unit test
* [[`0ee6ef8617`](https://github.com/nodejs/node-chakracore/commit/0ee6ef8617)] - Merge nodejs/master
* [[`ceb71b387e`](https://github.com/nodejs/node-chakracore/commit/ceb71b387e)] - [Merge Microsoft/Chakracore@353d42059b] [1.6>1.7] [MERGE #3480 @jianchun] fix minor out of bound read (JavascriptString::ToInteger)
* [[`66eabc710a`](https://github.com/nodejs/node-chakracore/commit/66eabc710a)] - [Merge Microsoft/Chakracore@d7fba376b7] [1.6>1.7] [MERGE #3479 @tcare] Port AutoDisableInterrupt updates from internal branch
* [[`aeb5aa7055`](https://github.com/nodejs/node-chakracore/commit/aeb5aa7055)] - [Merge Microsoft/Chakracore@1cadbb3b51] [1.6>1.7] [MERGE #3470 @tcare] [Disabled Tests] test/Error/rlexe.xml: tests tagged
* [[`2481345b3d`](https://github.com/nodejs/node-chakracore/commit/2481345b3d)] - [Merge Microsoft/Chakracore@f95b7cbf33] [1.6>1.7] [MERGE #3469 @Cellule] WASM: reserved immediates
* [[`59446aec53`](https://github.com/nodejs/node-chakracore/commit/59446aec53)] - [Merge Microsoft/Chakracore@95f497c17f] [1.6>1.7] [MERGE #3463 @ricobbe] OS#11712101 Hoist CheckObjType out of loops only when the operand's containing object type is also invariant
* [[`fb72fa2aa8`](https://github.com/nodejs/node-chakracore/commit/fb72fa2aa8)] - chakrashim: fix console autocomplete in VS Code
* [[`eaa8771ce3`](https://github.com/nodejs/node-chakracore/commit/eaa8771ce3)] - [Merge Microsoft/Chakracore@42b5d73e48] [1.6>1.7] [MERGE #3471 @mrkmarron] Fix inflate table initialization + clang warnings.
* [[`17de83996e`](https://github.com/nodejs/node-chakracore/commit/17de83996e)] - [Merge Microsoft/Chakracore@f497760a25] [1.6>1.7] [MERGE #3460 @meg-gupta] Fix order of generating bailout info in ValueNumberLdElem
* [[`035f5192e5`](https://github.com/nodejs/node-chakracore/commit/035f5192e5)] - [Merge Microsoft/Chakracore@06c67e8493] [1.6>1.7] [MERGE #3426 @Cellule] WASM: uint64 -> double edge case
* [[`d1414ef53b`](https://github.com/nodejs/node-chakracore/commit/d1414ef53b)] - [Merge Microsoft/Chakracore@943277437f] [MERGE #3465 @obastemur] JSONStack: Improve performance
* [[`0ef48c1bf8`](https://github.com/nodejs/node-chakracore/commit/0ef48c1bf8)] - [Merge Microsoft/Chakracore@ff22ab8fa5] [MERGE #3456 @obastemur] xplat: fix multi-thread builds for node-chakracore
* [[`3f30262e15`](https://github.com/nodejs/node-chakracore/commit/3f30262e15)] - [Merge Microsoft/Chakracore@d8112a6ce8] [MERGE #3443 @obastemur] Make expensive entryRecord on Stack check is Debug Only
* [[`e37fb5f4c6`](https://github.com/nodejs/node-chakracore/commit/e37fb5f4c6)] - src,test: fixing V8 lint/test issues
* [[`270aaa1e4d`](https://github.com/nodejs/node-chakracore/commit/270aaa1e4d)] - test: fixed test case for v8
* [[`a90fa44c15`](https://github.com/nodejs/node-chakracore/commit/a90fa44c15)] - src: fix node-v8 build
* [[`3eefd29533`](https://github.com/nodejs/node-chakracore/commit/3eefd29533)] - test: fixed test-vm-new-script-new-context
* [[`fe2de51a34`](https://github.com/nodejs/node-chakracore/commit/fe2de51a34)] - Merge nodejs/master
* [[`02f56352aa`](https://github.com/nodejs/node-chakracore/commit/02f56352aa)] - [Merge Microsoft/Chakracore@35ade809e7] Updating Bytecode Headers
* [[`88e3388c1e`](https://github.com/nodejs/node-chakracore/commit/88e3388c1e)] - [Merge Microsoft/Chakracore@12da6749b9] [1.6>1.7] [MERGE #3459 @sigatrev] fix crashes with cross site typed virtual arrays
* [[`69b4e798df`](https://github.com/nodejs/node-chakracore/commit/69b4e798df)] - [Merge Microsoft/Chakracore@79c13b4641] [1.6>1.7] [MERGE #3464 @aneeshdk] Handle strict mode in console scope
* [[`dbb61e7b49`](https://github.com/nodejs/node-chakracore/commit/dbb61e7b49)] - n-api: implement DataView APIs for ChakraCore
* [[`8a8e0840cd`](https://github.com/nodejs/node-chakracore/commit/8a8e0840cd)] - build: fix build break when omitting icu
* [[`60735ab034`](https://github.com/nodejs/node-chakracore/commit/60735ab034)] - [Merge Microsoft/Chakracore@96b4d1f4a9] [1.6>1.7] [MERGE #3458 @aneeshdk] When ForceSplitScope flag is provided don't assume that we have non-simple parameter list
* [[`3ccc5a9e0e`](https://github.com/nodejs/node-chakracore/commit/3ccc5a9e0e)] - [Merge Microsoft/Chakracore@d43c23a9c4] [MERGE #3462 @kfarnung] Add JsGetDataViewInfo support
* [[`712a17f8fe`](https://github.com/nodejs/node-chakracore/commit/712a17f8fe)] - [Merge Microsoft/Chakracore@0eabffef38] [1.6>1.7] [MERGE #3457 @curtisman] Cleanup: Remove unused field in JavascriptLibrary and avoid casting in functionObjectTypesList
* [[`24170afcd7`](https://github.com/nodejs/node-chakracore/commit/24170afcd7)] - [Merge Microsoft/Chakracore@fdba5f1f9b] [1.6>1.7] [MERGE #3435 @suwc] OS12814968: ASSERTION:(lib\runtime\Base/FunctionBody.h) IsFunctionBody()
* [[`9d20414457`](https://github.com/nodejs/node-chakracore/commit/9d20414457)] - [Merge Microsoft/Chakracore@a8d700f7c4] [1.6>1.7] [MERGE #3451 @rajatd] Ensure that LdHeapArguments is the first thing in the bytecode after recording constants
* [[`e4929c2c3c`](https://github.com/nodejs/node-chakracore/commit/e4929c2c3c)] - [Merge Microsoft/Chakracore@6aeb615894] [MERGE #3437 @MSLaguana] Fixes #3430 overzealous type cast
* [[`9d9aa93c3a`](https://github.com/nodejs/node-chakracore/commit/9d9aa93c3a)] - Merge nodejs/master
* [[`3fe3388bb6`](https://github.com/nodejs/node-chakracore/commit/3fe3388bb6)] - optimize StringUtf8::From
* [[`f3e978b5fb`](https://github.com/nodejs/node-chakracore/commit/f3e978b5fb)] - Merge nodejs/master
* [[`b65205b58c`](https://github.com/nodejs/node-chakracore/commit/b65205b58c)] - [Merge Microsoft/Chakracore@8a61170da3] [1.6>1.7] [MERGE #3452 @akroshg] Splice helper function should check for side-effect in the prototype.
* [[`8a90ffbf0a`](https://github.com/nodejs/node-chakracore/commit/8a90ffbf0a)] - [Merge Microsoft/Chakracore@0c739a73be] [1.6>1.7] [MERGE #3445 @aneeshdk] Keep the same register allocation path for debug and non-debug mode in generators
* [[`035dc23dd2`](https://github.com/nodejs/node-chakracore/commit/035dc23dd2)] - [Merge Microsoft/Chakracore@391e601df6] [1.6>1.7] [MERGE #3428 @curtisman] Fix Issue #3039: Arrow function should terminate the expression unless followed by a comma
* [[`0f50e7c043`](https://github.com/nodejs/node-chakracore/commit/0f50e7c043)] - [Merge Microsoft/Chakracore@786f348d13] [1.6>1.7] [MERGE #3447 @dilijev] Fix #3438: AutoInitLibraryCodeScope: hide Intl.js initialization from debugger in addition to profiler.
* [[`a86339e146`](https://github.com/nodejs/node-chakracore/commit/a86339e146)] - [Merge Microsoft/Chakracore@76a2210a34] [1.6>1.7] [MERGE #3446 @akroshg] OS: 12891717 Missing conversion for CopyOnAccess Array on ForInObjectEnumerator
* [[`37d92c9443`](https://github.com/nodejs/node-chakracore/commit/37d92c9443)] - chakrashim: fix build break from JsCopyString
* [[`0fa38ec9b4`](https://github.com/nodejs/node-chakracore/commit/0fa38ec9b4)] - [Merge Microsoft/Chakracore@e0fb812bc1] [1.6>1.7] [MERGE #3440 @LouisLaf] Initialize locals
* [[`248f46c7a7`](https://github.com/nodejs/node-chakracore/commit/248f46c7a7)] - [Merge Microsoft/Chakracore@3c7ecf0b80] [MERGE #3433 @kunalspathak] Jsrt: Modify signature of JsCopyString
* [[`a1d72791a4`](https://github.com/nodejs/node-chakracore/commit/a1d72791a4)] - [Merge Microsoft/Chakracore@1ffb64cb4d] [1.6>1.7] [MERGE #3429 @atulkatti] Fixes #586 JsCreateNamedFunction should create a tracked PropertyId for name to ensure toString() works as expected.
* [[`daaa8f3269`](https://github.com/nodejs/node-chakracore/commit/daaa8f3269)] - [Merge Microsoft/Chakracore@39b89da350] [1.6>1.7] [MERGE #3436 @agarwal-sandeep] OS# 12840992: ARM: Wrong accessor flag offset passed for Getter/Setter inline cache check
* [[`fceec74d31`](https://github.com/nodejs/node-chakracore/commit/fceec74d31)] - [Merge Microsoft/Chakracore@55a0eb1825] [MERGE #3432 @MSLaguana] Fixing debug assert failure in WriteStringCopy
* [[`6dd968ee1f`](https://github.com/nodejs/node-chakracore/commit/6dd968ee1f)] - [Merge Microsoft/Chakracore@78b3208be5] [MERGE #3353 @obastemur] Debugger: keep new jobs while debugger is attached
* [[`f2aa054954`](https://github.com/nodejs/node-chakracore/commit/f2aa054954)] - [Merge Microsoft/Chakracore@a9d2418d41] [MERGE #3199 @obastemur] jsrt: don't check runtime on each API call for non-browser
* [[`6c4f4b969c`](https://github.com/nodejs/node-chakracore/commit/6c4f4b969c)] - [Merge Microsoft/Chakracore@a9c6c2dc41] [1.6>1.7] [MERGE #3416 @curtisman] Fix Issue #3261: Need to detect invalid null character at the end of the source string
* [[`268abc08ee`](https://github.com/nodejs/node-chakracore/commit/268abc08ee)] - [Merge Microsoft/Chakracore@5af1a8131b] [1.6>1.7] [MERGE #3425 @Cellule] Fix possible infinite loop in String.raw()
* [[`f2a913c3c6`](https://github.com/nodejs/node-chakracore/commit/f2a913c3c6)] - [Merge Microsoft/Chakracore@10d2f118ed] [1.6>1.7] [MERGE #3413 @atulkatti] Fixes #3065 Generator length property needs to be set of the script function it wraps.
* [[`e37deb07f1`](https://github.com/nodejs/node-chakracore/commit/e37deb07f1)] - [Merge Microsoft/Chakracore@8fb31110c6] [1.6>1.7] [MERGE #3415 @akroshg] slice helper should check for the ES5array or Proxy in the protototype
* [[`1ab71b9a03`](https://github.com/nodejs/node-chakracore/commit/1ab71b9a03)] - Merge nodejs/master
* [[`412da8f839`](https://github.com/nodejs/node-chakracore/commit/412da8f839)] - chakrashim: fixing lint issue
* [[`f42116050b`](https://github.com/nodejs/node-chakracore/commit/f42116050b)] - [Merge Microsoft/Chakracore@3c659bbe63] [1.6>1.7] [MERGE #3414 @curtisman] Fix issue #3393: Remove throwing accessor for caller property on argument object in strict mode
* [[`f9033a50a0`](https://github.com/nodejs/node-chakracore/commit/f9033a50a0)] - [Merge Microsoft/Chakracore@1b75f1bb64] [MERGE #3391 @kfarnung] Report script loads from eval when debugging
* [[`ecdf077782`](https://github.com/nodejs/node-chakracore/commit/ecdf077782)] - [Merge Microsoft/Chakracore@804093ebc3] [1.6>1.7] [MERGE #3338 @MikeHolman] assert that function we are JITing has a body
* [[`03c57bb965`](https://github.com/nodejs/node-chakracore/commit/03c57bb965)] - [Merge Microsoft/Chakracore@b5ebc92dbc] [1.6>1.7] [MERGE #3417 @MikeHolman] abort inlining dom fastpath if it didn't get registered
* [[`1d56946c4f`](https://github.com/nodejs/node-chakracore/commit/1d56946c4f)] - Merge nodejs/master
* [[`a7927e47b1`](https://github.com/nodejs/node-chakracore/commit/a7927e47b1)] - Merge nodejs/master
* [[`43125ad3fa`](https://github.com/nodejs/node-chakracore/commit/43125ad3fa)] - chakrashim: Avoid calling JsCopyString twice to find length
* [[`92eb5617ad`](https://github.com/nodejs/node-chakracore/commit/92eb5617ad)] - chakrashim: using new jsrt interface
* [[`b3e37a4125`](https://github.com/nodejs/node-chakracore/commit/b3e37a4125)] - lib: update constants usage in TTD code
* [[`2ae811797b`](https://github.com/nodejs/node-chakracore/commit/2ae811797b)] - test: add assert to `engineSpecificMessage`
* [[`87cdb276e5`](https://github.com/nodejs/node-chakracore/commit/87cdb276e5)] - Merge nodejs/master
* [[`45e1f76e2f`](https://github.com/nodejs/node-chakracore/commit/45e1f76e2f)] - [Merge Microsoft/Chakracore@e63dc6fe31] [MERGE #3402 @dilijev] Change release/1.7 to RELEASE mode and update bytecode GUID.
* [[`23cff2b629`](https://github.com/nodejs/node-chakracore/commit/23cff2b629)] - [Merge Microsoft/Chakracore@a95b99fb36] [MERGE #3308 @obastemur] Library: Create less new string object for known texts
* [[`c4f7940b58`](https://github.com/nodejs/node-chakracore/commit/c4f7940b58)] - [Merge Microsoft/Chakracore@9626882ac9] [MERGE #3222 @obastemur] NumberToString: Improve performance and reduce cache memory
* [[`d8a93cd8d7`](https://github.com/nodejs/node-chakracore/commit/d8a93cd8d7)] - [Merge Microsoft/Chakracore@e76b30e5ca] [1.6>1.7] [MERGE #3389 @sigatrev] OS#12528802 fix crash accessing FuncObjectOpnd when function object is inlined
* [[`ab22d654e3`](https://github.com/nodejs/node-chakracore/commit/ab22d654e3)] - [Merge Microsoft/Chakracore@8cfe47de52] [1.6>1.7] [MERGE #3400 @jianchun] jsrt: cleanup CHAKRACOREBUILD_ symbol in headers
* [[`5195b969ec`](https://github.com/nodejs/node-chakracore/commit/5195b969ec)] - [Merge Microsoft/Chakracore@66bc22eceb] [1.6>1.7] [MERGE #3382 @rajatd] On .caller, only invalidate cached scopes for nested parents
* [[`4c77b78334`](https://github.com/nodejs/node-chakracore/commit/4c77b78334)] - [Merge Microsoft/Chakracore@ad54488def] [1.6>1.7] [MERGE #3411 @agarwal-sandeep] Fix -stats:rejit for chakra full
* [[`79c597e14e`](https://github.com/nodejs/node-chakracore/commit/79c597e14e)] - [Merge Microsoft/Chakracore@36fc51b58f] [1.6>1.7] [MERGE #3412 @curtisman] Fix Issue #3368:  Symbol.species.toString() shouldn't have bracket around the name
* [[`ca69080ce8`](https://github.com/nodejs/node-chakracore/commit/ca69080ce8)] - [Merge Microsoft/Chakracore@cb5557fe2d] [1.6>1.7] [MERGE #3409 @curtisman] Fix Issue #3376:  Escaped yield cannot be an identifier in strict mode
* [[`6fdb58c11d`](https://github.com/nodejs/node-chakracore/commit/6fdb58c11d)] - [Merge Microsoft/Chakracore@36316625f2] [1.6>1.7] [MERGE #3377 @suwc] Fix module namespace property attributes
* [[`77136f7865`](https://github.com/nodejs/node-chakracore/commit/77136f7865)] - [Merge Microsoft/Chakracore@07ef27bd6f] [MERGE #3408 @MSLaguana] Adding Jsrt function JsCopyStringOneByte
* [[`66567b6fb9`](https://github.com/nodejs/node-chakracore/commit/66567b6fb9)] - Merge nodejs/master
* [[`eba78668db`](https://github.com/nodejs/node-chakracore/commit/eba78668db)] - [Merge Microsoft/Chakracore@7d7010683e] [1.6>1.7] [MERGE #3397 @jackhorton] Fix crash when a Map is constructed with custom Map.prototype.set
* [[`cad84b437a`](https://github.com/nodejs/node-chakracore/commit/cad84b437a)] - [Merge Microsoft/Chakracore@7829651f00] [1.6>1.7] [MERGE #3328 @suwc] Fix Issue#3217: Reflect.construct permanently corrupts the invoked constructor
* [[`1114aad60d`](https://github.com/nodejs/node-chakracore/commit/1114aad60d)] - [Merge Microsoft/Chakracore@083ab297e1] [1.6>1.7] [MERGE #3398 @suwc] OS12503560: assignment to super[prop] not accessing base class property
* [[`9c4761af96`](https://github.com/nodejs/node-chakracore/commit/9c4761af96)] - [Merge Microsoft/Chakracore@6489b97aae] [1.6>1.7] [MERGE #3392 @atulkatti] Fixes #3009 The Enumerator built-in has been removed, so removing the tests.
* [[`9a00f404db`](https://github.com/nodejs/node-chakracore/commit/9a00f404db)] - [Merge Microsoft/Chakracore@8bbe7b2e82] [1.6>1.7] [MERGE #3323 @dilijev] Fix #3203, Fix #3204: Intl.getCanonicalLocales: name, toString, cannot call with new
* [[`b17eccefda`](https://github.com/nodejs/node-chakracore/commit/b17eccefda)] - Merge nodejs/master
* [[`94daa51439`](https://github.com/nodejs/node-chakracore/commit/94daa51439)] - [Merge Microsoft/Chakracore@1e7793cfdf] [1.6>1.7] [MERGE #3378 @meg-gupta] TryFinally Fixes
* [[`8c21939516`](https://github.com/nodejs/node-chakracore/commit/8c21939516)] - [Merge Microsoft/Chakracore@7df7f780dd] [MERGE #3360 @jackhorton] Fixes #3192, allows promise callback to be set to null
* [[`a1730fb366`](https://github.com/nodejs/node-chakracore/commit/a1730fb366)] - [Merge Microsoft/Chakracore@ea9514eac7] [1.6>1.7] [MERGE #3385 @thomasmo] OS#11221802 Address a reliability issue processes are running down during JIT server call
* [[`07975da206`](https://github.com/nodejs/node-chakracore/commit/07975da206)] - Merge nodejs/master
* [[`cf5ac0740a`](https://github.com/nodejs/node-chakracore/commit/cf5ac0740a)] - chakrashim: removed shim for `hasOwnProperty`
* [[`7784b248ea`](https://github.com/nodejs/node-chakracore/commit/7784b248ea)] - n-api: support napi_has_own_property
* [[`e96815120d`](https://github.com/nodejs/node-chakracore/commit/e96815120d)] - [Merge Microsoft/Chakracore@3747582647] [1.6>1.7] [MERGE #3384 @agarwal-sandeep] OS#12654029: ArrayBuffer throw OOM if unable to allocate memory after GC
* [[`50c888a616`](https://github.com/nodejs/node-chakracore/commit/50c888a616)] - [Merge Microsoft/Chakracore@e4f3c46cb3] [1.6>1.7] [MERGE #3390 @agarwal-sandeep] Fixes #3387: Fix build break with simd unused variable
* [[`00f2d35811`](https://github.com/nodejs/node-chakracore/commit/00f2d35811)] - build: adding LTO support to configure script
* [[`a932bee96c`](https://github.com/nodejs/node-chakracore/commit/a932bee96c)] - Update chakracore to 1.7
* [[`5e9409cb67`](https://github.com/nodejs/node-chakracore/commit/5e9409cb67)] - chakrashim: fixing cpplint errors
* [[`8cc9d842d6`](https://github.com/nodejs/node-chakracore/commit/8cc9d842d6)] - Merge nodejs/master
* [[`d8ae639e47`](https://github.com/nodejs/node-chakracore/commit/d8ae639e47)] - test: fixed test-util-format
* [[`9706c731c2`](https://github.com/nodejs/node-chakracore/commit/9706c731c2)] - Merge nodejs/master
* [[`13b164e4c0`](https://github.com/nodejs/node-chakracore/commit/13b164e4c0)] - Merge nodejs/master
* [[`f69cdcc065`](https://github.com/nodejs/node-chakracore/commit/f69cdcc065)] - Merge nodejs/master
* [[`b4373c4648`](https://github.com/nodejs/node-chakracore/commit/b4373c4648)] - n-api: update error functions for JSRT
* [[`75b95adba6`](https://github.com/nodejs/node-chakracore/commit/75b95adba6)] - Merge nodejs/master
* [[`10cb1f8e46`](https://github.com/nodejs/node-chakracore/commit/10cb1f8e46)] - Merge pull request #328 from obastemur/idle_idle
* [[`e427aa7f56`](https://github.com/nodejs/node-chakracore/commit/e427aa7f56)] - Merge nodejs/master
* [[`e8adf24e8f`](https://github.com/nodejs/node-chakracore/commit/e8adf24e8f)] - [Merge Microsoft/Chakracore@359cca5ec4] [MERGE #3349 @MSLaguana] Update version number to 1.5.3
* [[`94878af024`](https://github.com/nodejs/node-chakracore/commit/94878af024)] - [Merge Microsoft/Chakracore@bd1dba2da8] [MERGE #3341 @akroshg] 17-07 ChakraCore servicing release
* [[`825eb35e5e`](https://github.com/nodejs/node-chakracore/commit/825eb35e5e)] - n-api: update napi_wrap error behavior
* [[`e3cc1fd0cd`](https://github.com/nodejs/node-chakracore/commit/e3cc1fd0cd)] - Merge nodejs/master
* [[`86056ab173`](https://github.com/nodejs/node-chakracore/commit/86056ab173)] - Merge nodejs/master
* [[`441eb77441`](https://github.com/nodejs/node-chakracore/commit/441eb77441)] - test: update parallel/test-assert for chakracore
* [[`9888ef6906`](https://github.com/nodejs/node-chakracore/commit/9888ef6906)] - Merge nodejs/master
* [[`f5bcbe9aba`](https://github.com/nodejs/node-chakracore/commit/f5bcbe9aba)] - src: TTD fixes with async-wrap and zlib
* [[`cebd7cf2b9`](https://github.com/nodejs/node-chakracore/commit/cebd7cf2b9)] - Merge nodejs/master
* [[`272cab8acf`](https://github.com/nodejs/node-chakracore/commit/272cab8acf)] - Merge nodejs/master
* [[`05d537e5d4`](https://github.com/nodejs/node-chakracore/commit/05d537e5d4)] - test: disable addons-napi test_object
* [[`1e49eb83d3`](https://github.com/nodejs/node-chakracore/commit/1e49eb83d3)] - Merge nodejs/master
* [[`c291178542`](https://github.com/nodejs/node-chakracore/commit/c291178542)] - test: fixed test for chakracore
* [[`50025eebe6`](https://github.com/nodejs/node-chakracore/commit/50025eebe6)] - Merge nodejs/master
* [[`c1b255c728`](https://github.com/nodejs/node-chakracore/commit/c1b255c728)] - Merge nodejs/master
* [[`0b3e40c1eb`](https://github.com/nodejs/node-chakracore/commit/0b3e40c1eb)] - Merge nodejs/master
* [[`98b06aef28`](https://github.com/nodejs/node-chakracore/commit/98b06aef28)] - build: split up cpplint to avoid long cmd lines
* [[`1477c31237`](https://github.com/nodejs/node-chakracore/commit/1477c31237)] - cctest: Fix cctest failure on windows
* [[`1b50f3d40c`](https://github.com/nodejs/node-chakracore/commit/1b50f3d40c)] - src: fix cpplint error
* [[`1417fa2ee4`](https://github.com/nodejs/node-chakracore/commit/1417fa2ee4)] - chakrashim: updating platform shim
* [[`a6e02dddd1`](https://github.com/nodejs/node-chakracore/commit/a6e02dddd1)] - Merge nodejs/master
* [[`a4ee1e229c`](https://github.com/nodejs/node-chakracore/commit/a4ee1e229c)] - Merge nodejs/master
* [[`908169778a`](https://github.com/nodejs/node-chakracore/commit/908169778a)] - n-api: implement delete_property,
* [[`46286ea322`](https://github.com/nodejs/node-chakracore/commit/46286ea322)] - Merge nodejs/master
* [[`123552a1a8`](https://github.com/nodejs/node-chakracore/commit/123552a1a8)] - Merge nodejs/master
* [[`d38dd54e52`](https://github.com/nodejs/node-chakracore/commit/d38dd54e52)] - Merge nodejs/master
* [[`b2c8524774`](https://github.com/nodejs/node-chakracore/commit/b2c8524774)] - Merge nodejs/master
* [[`33b1b13649`](https://github.com/nodejs/node-chakracore/commit/33b1b13649)] - Merge nodejs/master
* [[`471fecd897`](https://github.com/nodejs/node-chakracore/commit/471fecd897)] - Merge nodejs/master
* [[`dcde7f54fc`](https://github.com/nodejs/node-chakracore/commit/dcde7f54fc)] - Merge nodejs/master
* [[`c4274ce3c1`](https://github.com/nodejs/node-chakracore/commit/c4274ce3c1)] - Merge nodejs/master
* [[`ffa055e086`](https://github.com/nodejs/node-chakracore/commit/ffa055e086)] - test: skip a check in test_handle_scope
* [[`53b8790ac2`](https://github.com/nodejs/node-chakracore/commit/53b8790ac2)] - n-api: removed `napi_status_last` usage
* [[`3326b072c4`](https://github.com/nodejs/node-chakracore/commit/3326b072c4)] - Merge nodejs/master
* [[`21ac436d26`](https://github.com/nodejs/node-chakracore/commit/21ac436d26)] - doc: fix broken markdown link
* [[`29e3acf6c3`](https://github.com/nodejs/node-chakracore/commit/29e3acf6c3)] - doc: update readme to use the master branch
* [[`e70a663def`](https://github.com/nodejs/node-chakracore/commit/e70a663def)] - Merge nodejs/master
* [[`543829278a`](https://github.com/nodejs/node-chakracore/commit/543829278a)] - Merge nodejs/master
* [[`7c8ccf55a7`](https://github.com/nodejs/node-chakracore/commit/7c8ccf55a7)] - Merge nodejs/master
* [[`f123b4587f`](https://github.com/nodejs/node-chakracore/commit/f123b4587f)] - Merge nodejs/master
* [[`7770ab1199`](https://github.com/nodejs/node-chakracore/commit/7770ab1199)] - Merge nodejs/master
* [[`85ccdd1656`](https://github.com/nodejs/node-chakracore/commit/85ccdd1656)] - chakrashim: add shim for JSON methods
* [[`15ab3ed5b0`](https://github.com/nodejs/node-chakracore/commit/15ab3ed5b0)] - test: fixed a unit test
* [[`1ebe88faa7`](https://github.com/nodejs/node-chakracore/commit/1ebe88faa7)] - chakrashim: Added shim for EnqueueMicrotask
* [[`59d1cdc470`](https://github.com/nodejs/node-chakracore/commit/59d1cdc470)] - Merge nodejs/master
* [[`e002386c3a`](https://github.com/nodejs/node-chakracore/commit/e002386c3a)] - [Merge Microsoft/Chakracore@5d9535ac77] [MERGE #3190 @obastemur] xplat: fix ICU lib tilde path problem
* [[`123ce1a685`](https://github.com/nodejs/node-chakracore/commit/123ce1a685)] - Merge nodejs/master
* [[`6d7f46e233`](https://github.com/nodejs/node-chakracore/commit/6d7f46e233)] - Merge nodejs/master
* [[`a090a7dd80`](https://github.com/nodejs/node-chakracore/commit/a090a7dd80)] - [Merge Microsoft/Chakracore@f91c91297d] [MERGE #3051 @MSLaguana] Updating release notes link
* [[`0f1feb93fd`](https://github.com/nodejs/node-chakracore/commit/0f1feb93fd)] - test: mark test-off-with-session-then-on as flaky
* [[`0078c29a63`](https://github.com/nodejs/node-chakracore/commit/0078c29a63)] - Merge nodejs/master
* [[`4073d3665f`](https://github.com/nodejs/node-chakracore/commit/4073d3665f)] - chakrashim: fix changelog using script update
* [[`d7d84de73f`](https://github.com/nodejs/node-chakracore/commit/d7d84de73f)] - chakrashim: more update-changelog fixes
* [[`cc4fa1fe84`](https://github.com/nodejs/node-chakracore/commit/cc4fa1fe84)] - build: chakracore build improvements
* [[`021fbca6bc`](https://github.com/nodejs/node-chakracore/commit/021fbca6bc)] - tools: Fixed update-changelog
* [[`fec1ab7e49`](https://github.com/nodejs/node-chakracore/commit/fec1ab7e49)] - test: fixing inspector test break
* [[`4f8f708db2`](https://github.com/nodejs/node-chakracore/commit/4f8f708db2)] - Merge nodejs/master
* [[`fcf3f3fa54`](https://github.com/nodejs/node-chakracore/commit/fcf3f3fa54)] - [Merge Microsoft/Chakracore@93ec291cff] [MERGE #3166 @jianchun] 17-06 ChakraCore servicing release
* [[`5749eae469`](https://github.com/nodejs/node-chakracore/commit/5749eae469)] - Merge nodejs/master
* [[`a220edef9c`](https://github.com/nodejs/node-chakracore/commit/a220edef9c)] - test: mark sequential/test-benchmark-child-process as flaky
* [[`60086f4a12`](https://github.com/nodejs/node-chakracore/commit/60086f4a12)] - [Merge Microsoft/Chakracore@b3ec4b6344] Update version number to 1.5.2
* [[`2890542485`](https://github.com/nodejs/node-chakracore/commit/2890542485)] - test: enable addons-napi/test_reference
* [[`d41d2eed5a`](https://github.com/nodejs/node-chakracore/commit/d41d2eed5a)] - [Merge Microsoft/Chakracore@96387111d7] [MERGE #3135 @obastemur] xplat: Improve ICU fallback conditions
* [[`76c7a22b66`](https://github.com/nodejs/node-chakracore/commit/76c7a22b66)] - test: ignore new failing tests
* [[`e1fc8c0621`](https://github.com/nodejs/node-chakracore/commit/e1fc8c0621)] - chakrashim: fix debug build failure
* [[`eeec9f1bae`](https://github.com/nodejs/node-chakracore/commit/eeec9f1bae)] - build: move SDK detection to npm-cli.js
* [[`00a6407bc8`](https://github.com/nodejs/node-chakracore/commit/00a6407bc8)] - Merge nodejs/master
* [[`453242bbfa`](https://github.com/nodejs/node-chakracore/commit/453242bbfa)] - Merge nodejs/master
* [[`30e1a5dbc0`](https://github.com/nodejs/node-chakracore/commit/30e1a5dbc0)] - test: mark some test as flaky
* [[`69ebf8273e`](https://github.com/nodejs/node-chakracore/commit/69ebf8273e)] - Merge nodejs/master
* [[`cb859bc137`](https://github.com/nodejs/node-chakracore/commit/cb859bc137)] - chakrashim: Implementing v8::Message
* [[`90e0c543ce`](https://github.com/nodejs/node-chakracore/commit/90e0c543ce)] - Merge nodejs/master into xplat
* [[`b8d1f7c4b7`](https://github.com/nodejs/node-chakracore/commit/b8d1f7c4b7)] - Merge nodejs/master
* [[`df8e7cab7d`](https://github.com/nodejs/node-chakracore/commit/df8e7cab7d)] - build,chakrashim: enabled cpplint for chakrashim
* [[`39c745f0e0`](https://github.com/nodejs/node-chakracore/commit/39c745f0e0)] - chakrashim: cleanup inspector code
* [[`a7fc0fe0e8`](https://github.com/nodejs/node-chakracore/commit/a7fc0fe0e8)] - test: disable N-API test
* [[`715199c58a`](https://github.com/nodejs/node-chakracore/commit/715199c58a)] - src: fix bad merge
* [[`3231bc3086`](https://github.com/nodejs/node-chakracore/commit/3231bc3086)] - Merge nodejs/master
* [[`15611495fe`](https://github.com/nodejs/node-chakracore/commit/15611495fe)] - [Merge Microsoft/Chakracore@a96455ed5f] [MERGE #3107 @obastemur] xplat: fix script location
* [[`425e628db5`](https://github.com/nodejs/node-chakracore/commit/425e628db5)] - [Merge Microsoft/Chakracore@d8c654e810] [MERGE #3102 @dilijev] release/1.5: Update version number to 1.5.1
* [[`aebd439c38`](https://github.com/nodejs/node-chakracore/commit/aebd439c38)] - lib,src: enable TTD record with env variable
* [[`16f5814ce7`](https://github.com/nodejs/node-chakracore/commit/16f5814ce7)] - [Merge Microsoft/Chakracore@07c15e7a55] [1.4>1.5] [MERGE #3092 @leirocks] fix handle leak for recycler concurrent thread
* [[`8cf4aa9961`](https://github.com/nodejs/node-chakracore/commit/8cf4aa9961)] - Merge nodejs/master into xplat
* [[`11ead4bcaf`](https://github.com/nodejs/node-chakracore/commit/11ead4bcaf)] - Merge nodejs/master into xplat_FI
* [[`9cc4824fad`](https://github.com/nodejs/node-chakracore/commit/9cc4824fad)] - n-api: enable napi_wrap() to work with any object
* [[`dc9a360042`](https://github.com/nodejs/node-chakracore/commit/dc9a360042)] - chakrashim: fix undefined values in inspector
* [[`913ab2a159`](https://github.com/nodejs/node-chakracore/commit/913ab2a159)] - Merge nodejs/master into xplat
* [[`4b46242389`](https://github.com/nodejs/node-chakracore/commit/4b46242389)] - test: fix new test for chakracore
* [[`028afb2876`](https://github.com/nodejs/node-chakracore/commit/028afb2876)] - src: fixing merge build break
* [[`9528ea8a89`](https://github.com/nodejs/node-chakracore/commit/9528ea8a89)] - Merge nodejs/master into xplat
* [[`7f4f9abd03`](https://github.com/nodejs/node-chakracore/commit/7f4f9abd03)] - test: fix unit test failures after merge
* [[`7aa5a91bac`](https://github.com/nodejs/node-chakracore/commit/7aa5a91bac)] - Merge nodejs/master into xplat
* [[`716841e343`](https://github.com/nodejs/node-chakracore/commit/716841e343)] - Merge pull request #276 from MSLaguana/updateVcbuild
* [[`7077edcdee`](https://github.com/nodejs/node-chakracore/commit/7077edcdee)] - n-api: Added jsrt implementation for napi_get_version
* [[`448b346ccd`](https://github.com/nodejs/node-chakracore/commit/448b346ccd)] - Merge nodejs/master into xplat
* [[`a9ef5bc9ba`](https://github.com/nodejs/node-chakracore/commit/a9ef5bc9ba)] - chakrashim: updated changelog

## 2017-05-31, node-chakracore-8.0.0-pre3

### Commits

* [[`e35fd86558`](https://github.com/nodejs/node-chakracore/commit/e35fd86558)] - test: marking new flaky test
* [[`262f7e0ac6`](https://github.com/nodejs/node-chakracore/commit/262f7e0ac6)] - build: fix ARM build
* [[`32f5986f47`](https://github.com/nodejs/node-chakracore/commit/32f5986f47)] - [Merge Microsoft/Chakracore@c36f7bf022] Ignore VS2017 linker warning LNK4221
* [[`10886533a7`](https://github.com/nodejs/node-chakracore/commit/10886533a7)] - Merge nodejs/master into xplat
* [[`184b3dd57e`](https://github.com/nodejs/node-chakracore/commit/184b3dd57e)] - chakrashim: fix inspector asserts
* [[`cf718ec451`](https://github.com/nodejs/node-chakracore/commit/cf718ec451)] - Merge nodejs/master into xplat
* [[`4da26c05fe`](https://github.com/nodejs/node-chakracore/commit/4da26c05fe)] - build: restore intl check for inspector
* [[`2c293b3ffc`](https://github.com/nodejs/node-chakracore/commit/2c293b3ffc)] - src: fix InspectorStarted macro guard
* [[`21b478d23f`](https://github.com/nodejs/node-chakracore/commit/21b478d23f)] - test: fix/ignore some unit-test
* [[`567f18c094`](https://github.com/nodejs/node-chakracore/commit/567f18c094)] - src,n-api: n-api bug fix
* [[`ac38ac9ab2`](https://github.com/nodejs/node-chakracore/commit/ac38ac9ab2)] - Merge nodejs/master into xplat
* [[`04f48ad9a4`](https://github.com/nodejs/node-chakracore/commit/04f48ad9a4)] - n-api: fix TypedAarray info for ArrayBuffer offset
* [[`ff36192d84`](https://github.com/nodejs/node-chakracore/commit/ff36192d84)] - [Merge Microsoft/Chakracore@f105d21acf] [MERGE #3023 @obastemur] xplat: Packaging shasum check
* [[`1897c8cf91`](https://github.com/nodejs/node-chakracore/commit/1897c8cf91)] - [Merge Microsoft/Chakracore@4c0fcf1480] [MERGE #2990 @obastemur] xplat: protect CC from un-reasonable stack limits
* [[`fc1864ced0`](https://github.com/nodejs/node-chakracore/commit/fc1864ced0)] - [Merge Microsoft/Chakracore@36d1ef2fa7] [MERGE #3015 @dilijev] release/1.5: Update version number to 1.5.0
* [[`74474a15d3`](https://github.com/nodejs/node-chakracore/commit/74474a15d3)] - src: Added missing changes
* [[`fc537460f1`](https://github.com/nodejs/node-chakracore/commit/fc537460f1)] - Merge nodejs/master into xplat
* [[`eb30926dc6`](https://github.com/nodejs/node-chakracore/commit/eb30926dc6)] - build: fixing lint issues
* [[`ccdb0b241d`](https://github.com/nodejs/node-chakracore/commit/ccdb0b241d)] - [Merge Microsoft/Chakracore@e6d6de4f86] [MERGE #3006 @mrkmarron] TTD -- Correct behavior around unhandled exception & debugger
* [[`e8c42b52ca`](https://github.com/nodejs/node-chakracore/commit/e8c42b52ca)] - Merge nodejs/master into xplat
* [[`2bd43bfa2c`](https://github.com/nodejs/node-chakracore/commit/2bd43bfa2c)] - Merge nodejs/master into xplat Merge 2b541471db as of 2017-05-21. This is an automatically created merge. For any problems please contact @kunalspathak.
* [[`4fc607f278`](https://github.com/nodejs/node-chakracore/commit/4fc607f278)] - Merge nodejs/master to xplat
* [[`b3b36260bb`](https://github.com/nodejs/node-chakracore/commit/b3b36260bb)] - test: Resolved and fixed a unit test
* [[`65a4750e24`](https://github.com/nodejs/node-chakracore/commit/65a4750e24)] - Merge nodejs/master to xplat
* [[`50eba656fc`](https://github.com/nodejs/node-chakracore/commit/50eba656fc)] - [Merge Microsoft/Chakracore@34c3ab1519] [MERGE #2976 @rajatd] Fix linux builds in 2.0
* [[`0e386eb18b`](https://github.com/nodejs/node-chakracore/commit/0e386eb18b)] - [Merge Microsoft/Chakracore@796a580cc0] Fix build [GlobOpt.cpp]
* [[`8f7d131f07`](https://github.com/nodejs/node-chakracore/commit/8f7d131f07)] - [Merge Microsoft/Chakracore@73d6109a24] [1.4>2.0] [MERGE #2959 @rajatd] 17-05 ChakraCore servicing release
* [[`b9d061120b`](https://github.com/nodejs/node-chakracore/commit/b9d061120b)] - [Merge Microsoft/Chakracore@58225fe1db] [MERGE #2936 @MSLaguana] Exposing a new JSRT method to get additional information about exceptions
* [[`b60c0243d4`](https://github.com/nodejs/node-chakracore/commit/b60c0243d4)] - chakrashim: fixed missing conditional body
* [[`65289ed50b`](https://github.com/nodejs/node-chakracore/commit/65289ed50b)] - chakrashim: clean up legacy debugger code
* [[`9bf51e4b4d`](https://github.com/nodejs/node-chakracore/commit/9bf51e4b4d)] - chakrashim: fixing jslint issues
* [[`91a9798c6d`](https://github.com/nodejs/node-chakracore/commit/91a9798c6d)] - test: enable inspector tests
* [[`fb4e1e6d60`](https://github.com/nodejs/node-chakracore/commit/fb4e1e6d60)] - build,src: enable intl for chakracore
* [[`ff42f9d275`](https://github.com/nodejs/node-chakracore/commit/ff42f9d275)] - n-api: Fix test-addons-napi failures (#249)
* [[`9752ce1400`](https://github.com/nodejs/node-chakracore/commit/9752ce1400)] - Merge nodejs/master into xplat
* [[`d07825c844`](https://github.com/nodejs/node-chakracore/commit/d07825c844)] - Merge nodejs/master to xplat
* [[`a8d00065a9`](https://github.com/nodejs/node-chakracore/commit/a8d00065a9)] - build,test: Fixed test and build issue
* [[`75c91ca118`](https://github.com/nodejs/node-chakracore/commit/75c91ca118)] - chakrashim: Fixed build errors
* [[`16922b450b`](https://github.com/nodejs/node-chakracore/commit/16922b450b)] - Fix N-API JSRT bugs found by new tests
* [[`297b176ede`](https://github.com/nodejs/node-chakracore/commit/297b176ede)] - Merge nodejs/node into xplat
* [[`bfbf736f0c`](https://github.com/nodejs/node-chakracore/commit/bfbf736f0c)] - chakrashim, test: Build failures and skipped test
* [[`ca32671f91`](https://github.com/nodejs/node-chakracore/commit/ca32671f91)] - Merge nodejs/node into xplat
* [[`f3d3097ac0`](https://github.com/nodejs/node-chakracore/commit/f3d3097ac0)] - inspector: enable TimeTravel support
* [[`af912e8fd5`](https://github.com/nodejs/node-chakracore/commit/af912e8fd5)] - inspector: assert when clearing breakpoints
* [[`28cdc33c8d`](https://github.com/nodejs/node-chakracore/commit/28cdc33c8d)] - n-api: Minor updates for review feedback
* [[`cc20967556`](https://github.com/nodejs/node-chakracore/commit/cc20967556)] - n-api: Make sources conditional on V8 / ChakraCore
* [[`f8cbdde7d7`](https://github.com/nodejs/node-chakracore/commit/f8cbdde7d7)] - n-api: Fix cc lint errors in node_api_jsrt.cc
* [[`5cad632a3b`](https://github.com/nodejs/node-chakracore/commit/5cad632a3b)] - n-api: Merge N-API JSRT from abi-stable-node
* [[`283781f1b7`](https://github.com/nodejs/node-chakracore/commit/283781f1b7)] - test: updating tests to new error messages
* [[`ad6108e51d`](https://github.com/nodejs/node-chakracore/commit/ad6108e51d)] - chakrashim: Updated ChakraCore to release/2.0
* [[`f9f10b4450`](https://github.com/nodejs/node-chakracore/commit/f9f10b4450)] - chakrashim,src,test: fix build issues
* [[`7e6de45b08`](https://github.com/nodejs/node-chakracore/commit/7e6de45b08)] - test: mark tests that fail with ChakraCore on CI
* [[`32b144d85a`](https://github.com/nodejs/node-chakracore/commit/32b144d85a)] - test: fix details in diff to node/master
* [[`4751de5962`](https://github.com/nodejs/node-chakracore/commit/4751de5962)] - chakrashim,lib,test: fixing build/test issues
* [[`fe44f54949`](https://github.com/nodejs/node-chakracore/commit/fe44f54949)] - chakrashim: fixes and updates for TTD
* [[`9a7af26982`](https://github.com/nodejs/node-chakracore/commit/9a7af26982)] - test: fixed unit test and jslint issues
* [[`c577509b27`](https://github.com/nodejs/node-chakracore/commit/c577509b27)] - Merge nodejs/master into xplat
* [[`7f33b4cbf3`](https://github.com/nodejs/node-chakracore/commit/7f33b4cbf3)] - chakrashim: Updated ChakraCore to release/2.0
* [[`97002d0ca0`](https://github.com/nodejs/node-chakracore/commit/97002d0ca0)] - inspector: fixing lambda call frames
* [[`a043cb86fe`](https://github.com/nodejs/node-chakracore/commit/a043cb86fe)] - inspector: implement inspector for chakrashim
* [[`3d40cc1cd5`](https://github.com/nodejs/node-chakracore/commit/3d40cc1cd5)] - inspector: copying the v8_inspector to chakrashim
* [[`f9272fd060`](https://github.com/nodejs/node-chakracore/commit/f9272fd060)] - chakrashim: Updated ChakraCore to release/2.0
* [[`2849020614`](https://github.com/nodejs/node-chakracore/commit/2849020614)] - test: fixing jslint error
* [[`5bd7d4d626`](https://github.com/nodejs/node-chakracore/commit/5bd7d4d626)] - chakrashim: enable building with Node ICU on Linux
* [[`481ca0e595`](https://github.com/nodejs/node-chakracore/commit/481ca0e595)] - test: fix tests after merge
* [[`361c4bbee2`](https://github.com/nodejs/node-chakracore/commit/361c4bbee2)] - chakrashim: Fix build after merge
* [[`ac1e9e0087`](https://github.com/nodejs/node-chakracore/commit/ac1e9e0087)] - Merge node/master into xplat

## 2017-04-27, node-chakracore-8.0.0-pre2

### Commits

* [[`265fc0bedc`](https://github.com/nodejs/node-chakracore/commit/265fc0bedc)] - chakrashim: fixing Linux build issue
* [[`5e3480dd8f`](https://github.com/nodejs/node-chakracore/commit/5e3480dd8f)] - doc: fixing links in markdown
* [[`25ca0d9eb8`](https://github.com/nodejs/node-chakracore/commit/25ca0d9eb8)] - chakrashim: fix cpplint warns
* [[`203bf79284`](https://github.com/nodejs/node-chakracore/commit/203bf79284)] - chakrashim,lib,test: fixes for build and tests
* [[`d1813bf736`](https://github.com/nodejs/node-chakracore/commit/d1813bf736)] - chakrashim,test: stub out serdes classes
* [[`ff666e867d`](https://github.com/nodejs/node-chakracore/commit/ff666e867d)] - Merge branch 'master' into xplat
* [[`2f5ee3943d`](https://github.com/nodejs/node-chakracore/commit/2f5ee3943d)] - chakracore: Sync chakracore to release/2.0
* [[`b6f1dc4be2`](https://github.com/nodejs/node-chakracore/commit/b6f1dc4be2)] - src,test,chakrashim: Misc Fixes
* [[`3176f076c5`](https://github.com/nodejs/node-chakracore/commit/3176f076c5)] - Merge 'nodejs/master' into xplat
* [[`a35216f4b2`](https://github.com/nodejs/node-chakracore/commit/a35216f4b2)] - src: Fix for build break
* [[`a361d49285`](https://github.com/nodejs/node-chakracore/commit/a361d49285)] - chakrashim,test: Revert previous changes for chakracore
* [[`491475dca0`](https://github.com/nodejs/node-chakracore/commit/491475dca0)] - Merge remote-tracking branch 'nodejs/master' into xplat
* [[`e170da8841`](https://github.com/nodejs/node-chakracore/commit/e170da8841)] - chakracore: Update chakracore release/2.0
* [[`87ce098018`](https://github.com/nodejs/node-chakracore/commit/87ce098018)] - Fix for TTD with fs stat changes. (#194)
* [[`b7a68b5faa`](https://github.com/nodejs/node-chakracore/commit/b7a68b5faa)] - src,test: Fixed merge conflicts
* [[`08a0e89806`](https://github.com/nodejs/node-chakracore/commit/08a0e89806)] - Merge remote-tracking branch 'nodejs/master' into xplat
* [[`c0297990ba`](https://github.com/nodejs/node-chakracore/commit/c0297990ba)] - lib: Update for jslint and documentation resources
* [[`374a1d81bb`](https://github.com/nodejs/node-chakracore/commit/374a1d81bb)] - test,chakrashim: Updated v8-version
* [[`4ef4e8415e`](https://github.com/nodejs/node-chakracore/commit/4ef4e8415e)] - Merge remote-tracking branch 'nodejs/master' into xplat
* [[`4c63073cea`](https://github.com/nodejs/node-chakracore/commit/4c63073cea)] - test: Skipped few testcases
* [[`55946a6aa6`](https://github.com/nodejs/node-chakracore/commit/55946a6aa6)] - Merge remote-tracking branch 'nodejs/master' into xplat
* [[`20fa6b1909`](https://github.com/nodejs/node-chakracore/commit/20fa6b1909)] - chakrashim: Enable node-chakracore build with icu on mac
* [[`9eb3d7c9b0`](https://github.com/nodejs/node-chakracore/commit/9eb3d7c9b0)] - chakracore: Updated chakracore to release/2.0
* [[`bdf6c12555`](https://github.com/nodejs/node-chakracore/commit/bdf6c12555)] - Update TTD-README.md
* [[`6129379586`](https://github.com/nodejs/node-chakracore/commit/6129379586)] - Update TTD-README.md
* [[`9805badfd4`](https://github.com/nodejs/node-chakracore/commit/9805badfd4)] - test, chakrashim: Shimmed an API
* [[`8a531cd495`](https://github.com/nodejs/node-chakracore/commit/8a531cd495)] - Merge remote-tracking branch 'nodejs/master' into xplat
* [[`4bac92195b`](https://github.com/nodejs/node-chakracore/commit/4bac92195b)] - Update TTD-README.md
* [[`00eeb6b15e`](https://github.com/nodejs/node-chakracore/commit/00eeb6b15e)] - Update TTD-README.md
* [[`cd04a3d503`](https://github.com/nodejs/node-chakracore/commit/cd04a3d503)] - chakracore: Updated chakracore to release/2.0
* [[`c51a730473`](https://github.com/nodejs/node-chakracore/commit/c51a730473)] - core,chakracore,chakrashim: TTD updates
* [[`fd355edcd9`](https://github.com/nodejs/node-chakracore/commit/fd355edcd9)] - chakrashim: Updated v8-version
* [[`ebeae1523c`](https://github.com/nodejs/node-chakracore/commit/ebeae1523c)] - test: Fixed unit tests
* [[`8fe0274d95`](https://github.com/nodejs/node-chakracore/commit/8fe0274d95)] - tools, src: Fix build break
* [[`ad2a20e802`](https://github.com/nodejs/node-chakracore/commit/ad2a20e802)] - Merge 'nodejs/master' into xplat
* [[`ed2964b0a9`](https://github.com/nodejs/node-chakracore/commit/ed2964b0a9)] - chakrashim, test: Chakrashim fixes
* [[`233a053031`](https://github.com/nodejs/node-chakracore/commit/233a053031)] - WIP: Fix build after merge
* [[`83d2df34c2`](https://github.com/nodejs/node-chakracore/commit/83d2df34c2)] - jslint: fix jslint errors after merge
* [[`02443adc1d`](https://github.com/nodejs/node-chakracore/commit/02443adc1d)] - Merge remote-tracking branch 'nodejs/master'
* [[`05e0f044bc`](https://github.com/nodejs/node-chakracore/commit/05e0f044bc)] - chakrashim: fix xplat build
* [[`8248ff6d26`](https://github.com/nodejs/node-chakracore/commit/8248ff6d26)] - test: fix up jslint warnings
* [[`9129ea787f`](https://github.com/nodejs/node-chakracore/commit/9129ea787f)] - chakrashim: fixup to match latest ChakraCore
* [[`54ecd987fc`](https://github.com/nodejs/node-chakracore/commit/54ecd987fc)] - chakrashim: Update ChakraCore to commit 49e376d
* [[`7ae4a2b731`](https://github.com/nodejs/node-chakracore/commit/7ae4a2b731)] - Merge pull request #172 from nodejs/cc_config
* [[`9716837055`](https://github.com/nodejs/node-chakracore/commit/9716837055)] - chakrashim: Fix `GetOwnPropertyNames` and `GetPropertyNames`
* [[`8e66382a18`](https://github.com/nodejs/node-chakracore/commit/8e66382a18)] - Ported fixes from ChakraCore to fix TTD issues
* [[`2096aa2083`](https://github.com/nodejs/node-chakracore/commit/2096aa2083)] - Merge branch 'xplat-merge' into xplat
* [[`6413bab8a5`](https://github.com/nodejs/node-chakracore/commit/6413bab8a5)] - Merge pull request #164 from jackTheRipper/mac-os-readme
* [[`ddd22d6204`](https://github.com/nodejs/node-chakracore/commit/ddd22d6204)] - deps: fix interaction with debugger in deps chakracore
* [[`9538b4aa4f`](https://github.com/nodejs/node-chakracore/commit/9538b4aa4f)] - chakrashim: fix compatibility with NAN
* [[`a1092a7e8d`](https://github.com/nodejs/node-chakracore/commit/a1092a7e8d)] - build: include chakrashim headers in install.py
* [[`121a342bb3`](https://github.com/nodejs/node-chakracore/commit/121a342bb3)] - build: include chakrashim in the executable
* [[`efeaf88f0c`](https://github.com/nodejs/node-chakracore/commit/efeaf88f0c)] - build: enable cross-compilation in vcbuild.bat
* [[`64480a53f2`](https://github.com/nodejs/node-chakracore/commit/64480a53f2)] - build: add ChakraCore SDK to packages
* [[`e11f649ec2`](https://github.com/nodejs/node-chakracore/commit/e11f649ec2)] - Update README.md (#149)
* [[`3d3d6e199f`](https://github.com/nodejs/node-chakracore/commit/3d3d6e199f)] - Update TTD-README.md

## 2016-11-26, node-chakracore-7.0.0-pre10

### Commits

* [[`cc9c687c24`](https://github.com/nodejs/node-chakracore/commit/cc9c687c24)] - Fix cleanup issue in ChakraCore xplat
* [[`638947dd10`](https://github.com/nodejs/node-chakracore/commit/638947dd10)] - Update ChakraCore to release/2.0-pre branch
* [[`17dda8c21d`](https://github.com/nodejs/node-chakracore/commit/17dda8c21d)] - Updated gif URL for reverse continue feature
* [[`d87ffdd05a`](https://github.com/nodejs/node-chakracore/commit/d87ffdd05a)] - Update TTD-README.md
* [[`59950ad1e8`](https://github.com/nodejs/node-chakracore/commit/59950ad1e8)] - How to get started with TTD
* [[`4c7321728d`](https://github.com/nodejs/node-chakracore/commit/4c7321728d)] - build: fix build on macos and fix crash on exit
* [[`5c0da902ff`](https://github.com/nodejs/node-chakracore/commit/5c0da902ff)] - Merge pull request #143 from agarwal-sandeep/xplat-ttd-squash
* [[`90e3033ab7`](https://github.com/nodejs/node-chakracore/commit/90e3033ab7)] - build: fix node-v8 build after merge
* [[`b661c07633`](https://github.com/nodejs/node-chakracore/commit/b661c07633)] - test: skip SharedArrayBuffer tests for chakracore
* [[`a6333edc7a`](https://github.com/nodejs/node-chakracore/commit/a6333edc7a)] - node-chakracore: Fix test/message/timeout_throw.js baseline
* [[`ff2d2213d5`](https://github.com/nodejs/node-chakracore/commit/ff2d2213d5)] - chakrashim: update chakracore to
* [[`9c745fd8ca`](https://github.com/nodejs/node-chakracore/commit/9c745fd8ca)] - chakrashim: Added missing IsSharedArrayBuffer API
* [[`9814c0b133`](https://github.com/nodejs/node-chakracore/commit/9814c0b133)] - Merge branch 'nodejs/master' into xplat
* [[`dd5a011356`](https://github.com/nodejs/node-chakracore/commit/dd5a011356)] - chakrashim: Use new xplat string APIs
* [[`3ec2f4fc6e`](https://github.com/nodejs/node-chakracore/commit/3ec2f4fc6e)] - chakrashim: Fix JsParse/JsRun APIs
* [[`e3860a2918`](https://github.com/nodejs/node-chakracore/commit/e3860a2918)] - chakrashim: update chakracore (2016-11-14)
* [[`f7d965f3fb`](https://github.com/nodejs/node-chakracore/commit/f7d965f3fb)] - Merge branch 'chakracore-master' into xplat
* [[`8e815577f9`](https://github.com/nodejs/node-chakracore/commit/8e815577f9)] - build: remove a pattern that only matched the MSI
* [[`d850b3d44d`](https://github.com/nodejs/node-chakracore/commit/d850b3d44d)] - build: disable MSI by default for ChakraCore
* [[`93c8379ba3`](https://github.com/nodejs/node-chakracore/commit/93c8379ba3)] - build: add ChakraCore DLL and PDB to packages
* [[`af3f785840`](https://github.com/nodejs/node-chakracore/commit/af3f785840)] - test: Fixed test failures
* [[`e9dd0aae36`](https://github.com/nodejs/node-chakracore/commit/e9dd0aae36)] - chakrashim: add chakracore experimental String APIs
* [[`d65fcad738`](https://github.com/nodejs/node-chakracore/commit/d65fcad738)] - chakrashim: update chakracore (2016-10-25)
* [[`af79e3fe6b`](https://github.com/nodejs/node-chakracore/commit/af79e3fe6b)] - tools: Resolved merged conflicts
* [[`732d81e2d3`](https://github.com/nodejs/node-chakracore/commit/732d81e2d3)] - Merge chakracore-master into xplat
* [[`0fd9426976`](https://github.com/nodejs/node-chakracore/commit/0fd9426976)] - chakrashim: re-enable chakra-config
* [[`2968f504ef`](https://github.com/nodejs/node-chakracore/commit/2968f504ef)] - Merge pull request #125 from obastemur/remove_shared
* [[`cecbc36ab0`](https://github.com/nodejs/node-chakracore/commit/cecbc36ab0)] - Merge pull request #124 from nodejs/osx_10_12
* [[`bd17a62fe4`](https://github.com/nodejs/node-chakracore/commit/bd17a62fe4)] - Merge pull request #123 from nodejs/xplat_build_fix
* [[`e7cb647bf6`](https://github.com/nodejs/node-chakracore/commit/e7cb647bf6)] - Merge pull request #117 from obastemur/xplat_osx
* [[`bea58b0120`](https://github.com/nodejs/node-chakracore/commit/bea58b0120)] - chakrashim: update chakracore (2016-09-06)
* [[`744b83c407`](https://github.com/nodejs/node-chakracore/commit/744b83c407)] - Merge 'chakracore-master' into xplat (2016-09-06)
* [[`9cb6342ef9`](https://github.com/nodejs/node-chakracore/commit/9cb6342ef9)] - README: update chakracore dependencies
* [[`edbc95ea19`](https://github.com/nodejs/node-chakracore/commit/edbc95ea19)] - Merge remote-tracking branch 'chakracore-master' into xplat
* [[`097b7b8b31`](https://github.com/nodejs/node-chakracore/commit/097b7b8b31)] - chakrashim: update chakracore (2016-08-01)
* [[`471b930d20`](https://github.com/nodejs/node-chakracore/commit/471b930d20)] - README: update to include Linux instructions
* [[`9c7c47d220`](https://github.com/nodejs/node-chakracore/commit/9c7c47d220)] - chakrashim: use updated chakracore
* [[`e1b7745bed`](https://github.com/nodejs/node-chakracore/commit/e1b7745bed)] - chakrashim: add chakracore experimental String APIs
* [[`9040442591`](https://github.com/nodejs/node-chakracore/commit/9040442591)] - Merge remote-tracking branch 'chakracore-master' into xplat
* [[`ab8fd541c9`](https://github.com/nodejs/node-chakracore/commit/ab8fd541c9)] - chakrashim: update chakracore (2016-07-22)
* [[`5021d49441`](https://github.com/nodejs/node-chakracore/commit/5021d49441)] - Merge remote-tracking branch 'chakracore-master' into xplat
* [[`f21cdaf984`](https://github.com/nodejs/node-chakracore/commit/f21cdaf984)] - chakrashim: update chakracore (2016-07-15)
* [[`ae1b9d4927`](https://github.com/nodejs/node-chakracore/commit/ae1b9d4927)] - chakrashim: build on Linux
* [[`b9c21932d8`](https://github.com/nodejs/node-chakracore/commit/b9c21932d8)] - chakrashim: remove --engine test arg
* [[`03633a70bb`](https://github.com/nodejs/node-chakracore/commit/03633a70bb)] - chakrashim: patch to build with linux chakracore
* [[`1ddc979544`](https://github.com/nodejs/node-chakracore/commit/1ddc979544)] - chakrashim: switch to chakracore/linux

## 2016-09-09, node-chakracore-7.0.0-pre9

### Commits

* [[`3bce961699`](https://github.com/nodejs/node-chakracore/commit/3bce961699)] - 2016-09-09, node-chakracore-7.0.0-pre9
* [[`0124235ee0`](https://github.com/nodejs/node-chakracore/commit/0124235ee0)] - test,tools: test fixes after merge
* [[`16bae56807`](https://github.com/nodejs/node-chakracore/commit/16bae56807)] - Merge remote-tracking branch 'nodejs/master' into chakracore-master
* [[`76da668665`](https://github.com/nodejs/node-chakracore/commit/76da668665)] - test: Updated baseline for chakracore
* [[`0bddd5da66`](https://github.com/nodejs/node-chakracore/commit/0bddd5da66)] - chakrashim: Make chakracore 1.3 build
* [[`3fa1e12b73`](https://github.com/nodejs/node-chakracore/commit/3fa1e12b73)] - chakracore: Updated to 1.3
* [[`a606f84dea`](https://github.com/nodejs/node-chakracore/commit/a606f84dea)] - src: fix build break for !NODE_USE_V8_PLATFORM
* [[`45ff9e9021`](https://github.com/nodejs/node-chakracore/commit/45ff9e9021)] - chakrashim: Added missing API in `v8.h`
* [[`913ea18f9d`](https://github.com/nodejs/node-chakracore/commit/913ea18f9d)] - Merge 'nodejs/master' into chakracore-master
* [[`65e0fc283e`](https://github.com/nodejs/node-chakracore/commit/65e0fc283e)] - test: fixed unit test
* [[`2525355b1e`](https://github.com/nodejs/node-chakracore/commit/2525355b1e)] - Merge 'nodejs/master' into chakracore-master
* [[`acb42253a8`](https://github.com/nodejs/node-chakracore/commit/acb42253a8)] - chakrashim: Improve AddLocal codepath

## 2016-07-22, node-chakracore-xplat-7.0.0-pre8

### Commits

* [[`471b930d20`](https://github.com/nodejs/node-chakracore/commit/471b930d20)] - README: update to include Linux instructions
* [[`9c7c47d220`](https://github.com/nodejs/node-chakracore/commit/9c7c47d220)] - chakrashim: use updated chakracore
* [[`e1b7745bed`](https://github.com/nodejs/node-chakracore/commit/e1b7745bed)] - chakrashim: add chakracore experimental String APIs
* [[`9040442591`](https://github.com/nodejs/node-chakracore/commit/9040442591)] - Merge remote-tracking branch 'chakracore-master' into xplat
* [[`ab8fd541c9`](https://github.com/nodejs/node-chakracore/commit/ab8fd541c9)] - chakrashim: update chakracore (2016-07-22)
* [[`5021d49441`](https://github.com/nodejs/node-chakracore/commit/5021d49441)] - Merge remote-tracking branch 'chakracore-master' into xplat
* [[`f21cdaf984`](https://github.com/nodejs/node-chakracore/commit/f21cdaf984)] - chakrashim: update chakracore (2016-07-15)
* [[`ae1b9d4927`](https://github.com/nodejs/node-chakracore/commit/ae1b9d4927)] - chakrashim: build on Linux
* [[`b9c21932d8`](https://github.com/nodejs/node-chakracore/commit/b9c21932d8)] - chakrashim: remove --engine test arg
* [[`03633a70bb`](https://github.com/nodejs/node-chakracore/commit/03633a70bb)] - chakrashim: patch to build with linux chakracore
* [[`1ddc979544`](https://github.com/nodejs/node-chakracore/commit/1ddc979544)] - chakrashim: switch to chakracore/linux
* [[`5eb80319a4`](https://github.com/nodejs/node-chakracore/commit/5eb80319a4)] - chakrashim: a few refactors for clang
* [[`6695095912`](https://github.com/nodejs/node-chakracore/commit/6695095912)] - chakrashim: pass compile flag to disable simdjs when building chakracore
* [[`b68580e947`](https://github.com/nodejs/node-chakracore/commit/b68580e947)] - deps: disable features using compile flag (simdjs)
* [[`c78c10f828`](https://github.com/nodejs/node-chakracore/commit/c78c10f828)] - Revert "deps: Expose `--harmony-simd` in node+chakracore"
* [[`d078ffc5b8`](https://github.com/nodejs/node-chakracore/commit/d078ffc5b8)] - Revert "deps: Modified chakracore to enable/disable SIMD"
* [[`06fc26df5d`](https://github.com/nodejs/node-chakracore/commit/06fc26df5d)] - test: fixed failing test cases
* [[`c2ce8329fb`](https://github.com/nodejs/node-chakracore/commit/c2ce8329fb)] - build: fixes post merge
* [[`93015ca33f`](https://github.com/nodejs/node-chakracore/commit/93015ca33f)] - Merge 'nodejs/master' into chakracore-master
* [[`32b85f9f0f`](https://github.com/nodejs/node-chakracore/commit/32b85f9f0f)] - chakrashim: bug fixes for Error APIs
* [[`fa65467af6`](https://github.com/nodejs/node-chakracore/commit/fa65467af6)] - win,node-gyp: work around __pfnDliNotifyHook2 type change

## 2016-06-23, node-chakracore-7.0.0-pre8

### Commits

* [[`bc6327fdd3`](https://github.com/nodejs/node-chakracore/commit/bc6327fdd3)] - 2016-06-21, node-chakracore-7.0.0-pre8
* [[`89dbaf8851`](https://github.com/nodejs/node-chakracore/commit/89dbaf8851)] - build: merged conditions for v8_inspector
* [[`f2ad5323e0`](https://github.com/nodejs/node-chakracore/commit/f2ad5323e0)] - lib,test: fixed jslint errors
* [[`b3610fe8f4`](https://github.com/nodejs/node-chakracore/commit/b3610fe8f4)] - test: fixed unit test for error message
* [[`8b48ffde44`](https://github.com/nodejs/node-chakracore/commit/8b48ffde44)] - src,build: added todo for isolate_data
* [[`0402b9e177`](https://github.com/nodejs/node-chakracore/commit/0402b9e177)] - Merge 'nodejs/master' into chakracore-master
* [[`1ec0729ac8`](https://github.com/nodejs/node-chakracore/commit/1ec0729ac8)] - chakrashim: implement triggering GC when idle
* [[`8641dc0a9d`](https://github.com/nodejs/node-chakracore/commit/8641dc0a9d)] - doc,deps,test:  fixed deps\test after upgrade to 1.2
* [[`4dd7c223d0`](https://github.com/nodejs/node-chakracore/commit/4dd7c223d0)] - build: do not build icu for chakracore-master
* [[`8f00534443`](https://github.com/nodejs/node-chakracore/commit/8f00534443)] - deps: Modified chakracore to enable/disable SIMD
* [[`7cd4c61fa4`](https://github.com/nodejs/node-chakracore/commit/7cd4c61fa4)] - chakrashim: updated chakracore to 1.2
* [[`407e16c0e7`](https://github.com/nodejs/node-chakracore/commit/407e16c0e7)] - test: Suppress debugger test and fixed jslint
* [[`5556a39abe`](https://github.com/nodejs/node-chakracore/commit/5556a39abe)] - deps: Expose `--harmony-simd` in node+chakracore
* [[`2684f39f4f`](https://github.com/nodejs/node-chakracore/commit/2684f39f4f)] - deps: Modified chakracore to enable/disable SIMD
* [[`3b6a55e5c4`](https://github.com/nodejs/node-chakracore/commit/3b6a55e5c4)] - Merge 'nodejs/master into chakracore-master
* [[`8d11795cf8`](https://github.com/nodejs/node-chakracore/commit/8d11795cf8)] - tool: modified eslint rule for line-breaks
* [[`2048b2c890`](https://github.com/nodejs/node-chakracore/commit/2048b2c890)] - test: Disabled a debugger unit test
* [[`6fd464cbd3`](https://github.com/nodejs/node-chakracore/commit/6fd464cbd3)] - Merge 'nodejs/master' into chakracore-master
* [[`b1e66d751b`](https://github.com/nodejs/node-chakracore/commit/b1e66d751b)] - tools: fixed jslint errors after merge
* [[`60e2711353`](https://github.com/nodejs/node-chakracore/commit/60e2711353)] - Merge nodejs/master into chakracore-master
* [[`574b01ed1c`](https://github.com/nodejs/node-chakracore/commit/574b01ed1c)] - test: fixed test case after merge
* [[`7c88be6551`](https://github.com/nodejs/node-chakracore/commit/7c88be6551)] - deps: added chakrashim apis for Proxy
* [[`ca75446655`](https://github.com/nodejs/node-chakracore/commit/ca75446655)] - Merge branch 'nodejs/master' into chakracore-master
* [[`c7129ab9f5`](https://github.com/nodejs/node-chakracore/commit/c7129ab9f5)] - test: update message/...chakracore.out
* [[`007b8f6099`](https://github.com/nodejs/node-chakracore/commit/007b8f6099)] - Merge branch 'nodejs/master' into chakracore-master
* [[`2a8b3b003c`](https://github.com/nodejs/node-chakracore/commit/2a8b3b003c)] - chakrashim: correct Template properties support

## 2016-04-20, node-chakracore-6.0.0-pre7

### Commits

* [[`acca384ba1`](https://github.com/nodejs/node-chakracore/commit/acca384ba1)] - 2016-04-20, node-chakracore-6.0.0-pre7
* [[`589206a43f`](https://github.com/nodejs/node-chakracore/commit/589206a43f)] - chakrashim: remove a jslint comment
* [[`5dafb03f7d`](https://github.com/nodejs/node-chakracore/commit/5dafb03f7d)] - Merge branch 'nodejs/master' into chakracore-master
* [[`8fb96bc713`](https://github.com/nodejs/node-chakracore/commit/8fb96bc713)] - test: update 5 node-chakracore message test baselines
* [[`40b5f7cb5e`](https://github.com/nodejs/node-chakracore/commit/40b5f7cb5e)] - test: fix wrong python script indentation
* [[`bb0707b8f2`](https://github.com/nodejs/node-chakracore/commit/bb0707b8f2)] - test: patch tests for chakra
* [[`df8d10e0a8`](https://github.com/nodejs/node-chakracore/commit/df8d10e0a8)] - chakrashim: correct function callback context
* [[`41d9652d09`](https://github.com/nodejs/node-chakracore/commit/41d9652d09)] - chakrashim: let caller TryCatch record exception
* [[`53c2d8e8e1`](https://github.com/nodejs/node-chakracore/commit/53c2d8e8e1)] - test: skip null-buffer-neuter on node-chakracore
* [[`313cec1c10`](https://github.com/nodejs/node-chakracore/commit/313cec1c10)] - chakrashim: add test/addons dependency stubs
* [[`0cd4cb887c`](https://github.com/nodejs/node-chakracore/commit/0cd4cb887c)] - gyp: revert incorrect quote_cmd fix
* [[`c864edf09b`](https://github.com/nodejs/node-chakracore/commit/c864edf09b)] - Merge branch 'nodejs/master' into chakracore-master

## 2016-04-01, node-chakracore-6.0.0-pre6

### Commits

* [[`7f2f649895`](https://github.com/nodejs/node-chakracore/commit/7f2f649895)] - 2016-04-01, node-chakracore-6.0.0-pre6
* [[`730cd890ab`](https://github.com/nodejs/node-chakracore/commit/730cd890ab)] - win,build: fix arm build
* [[`d9469f1006`](https://github.com/nodejs/node-chakracore/commit/d9469f1006)] - chakrashim: update chakracore to 1.1.0.5
* [[`b838007b79`](https://github.com/nodejs/node-chakracore/commit/b838007b79)] - chakrashim: use microtask queue to run Promise
* [[`2723d94d82`](https://github.com/nodejs/node-chakracore/commit/2723d94d82)] - test: address 2 new failures on node-chakracore
* [[`88dc6548c1`](https://github.com/nodejs/node-chakracore/commit/88dc6548c1)] - Merge remote-tracking branch 'nodejs/master'
* [[`803af694c5`](https://github.com/nodejs/node-chakracore/commit/803af694c5)] - chakrashim: add dummy Debug.MakeMirror
* [[`e7b09ef9e2`](https://github.com/nodejs/node-chakracore/commit/e7b09ef9e2)] - chakrashim: better StackTrace API compat
* [[`c6d6e6b2f7`](https://github.com/nodejs/node-chakracore/commit/c6d6e6b2f7)] - chakrashim: implement new APIs added in v8
* [[`58ff8db189`](https://github.com/nodejs/node-chakracore/commit/58ff8db189)] - Merge remote-tracking branch 'nodejs/master'
* [[`878b8fd412`](https://github.com/nodejs/node-chakracore/commit/878b8fd412)] - chakrashim: fix ObjectTemplate callbacks compat
* [[`75ace7dcc5`](https://github.com/nodejs/node-chakracore/commit/75ace7dcc5)] - chakrashim: ensure chakra_shim_native buffer is null terminated
* [[`adfadccf20`](https://github.com/nodejs/node-chakracore/commit/adfadccf20)] - test: fix unittest for Chakra engine
* [[`d875ac9d9c`](https://github.com/nodejs/node-chakracore/commit/d875ac9d9c)] - Merge remote-tracking branch 'nodejs/master'
* [[`9e23f005e5`](https://github.com/nodejs/node-chakracore/commit/9e23f005e5)] - chakrashim: allow Persistent::Reset after V8::Dispose

## 2016-02-29, node-chakracore-6.0.0-pre5

### Commits

* [[`fc3cf3e602`](https://github.com/nodejs/node-chakracore/commit/fc3cf3e602)] - deps: Updated chakracore to 1.1.0.4
* [[`e2fc3385f5`](https://github.com/nodejs/node-chakracore/commit/e2fc3385f5)] - test: Skip running test-tick-processor
* [[`50d1014b03`](https://github.com/nodejs/node-chakracore/commit/50d1014b03)] - chakrashim: Make --v8-options work
* [[`8932128466`](https://github.com/nodejs/node-chakracore/commit/8932128466)] - Merge remote-tracking branch 'nodejs/master'

## 2016-02-10, node-chakracore-6.0.0-pre4

### Commits

* [[`a34dd031b6`](https://github.com/nodejs/node-chakracore/commit/a34dd031b6)] - deps: Update chakracore to 1.1.0.3
* [[`ee02cb6486`](https://github.com/nodejs/node-chakracore/commit/ee02cb6486)] - chakrashim: Fixed breaking v8 APIs after merge
* [[`53bfef30c0`](https://github.com/nodejs/node-chakracore/commit/53bfef30c0)] - Merge remote-tracking branch 'nodejs/master' into chnext

## 2016-01-29, node-chakracore-6.0.0-pre3

### Commits

* [[`668b620a02`](https://github.com/nodejs/node-chakracore/commit/668b620a02)] - deps: Updated chakracore to 1.1.0.2
* [[`88ce3d56e0`](https://github.com/nodejs/node-chakracore/commit/88ce3d56e0)] - chakrashim: Update v8 version
* [[`e33559243e`](https://github.com/nodejs/node-chakracore/commit/e33559243e)] - chakrashim: Fixed v8 APIs
* [[`d720b2f26f`](https://github.com/nodejs/node-chakracore/commit/d720b2f26f)] - Merge remote-tracking branch 'nodejs/master' into chnext
* [[`87d88dd97f`](https://github.com/nodejs/node-chakracore/commit/87d88dd97f)] - doc: Fix broken link in README.md
* [[`fb09510343`](https://github.com/nodejs/node-chakracore/commit/fb09510343)] - test: Disabled more unit test for chakracore
* [[`c2c63373fb`](https://github.com/nodejs/node-chakracore/commit/c2c63373fb)] - chakrashim: Bump-up the v8-version.
* [[`c4f1afc8f2`](https://github.com/nodejs/node-chakracore/commit/c4f1afc8f2)] - chakrashim: Fixed v8 APIs after merge
* [[`c4dee625b3`](https://github.com/nodejs/node-chakracore/commit/c4dee625b3)] - Merge remote-tracking branch 'nodejs/master' into chnext
* [[`d69e2c82a3`](https://github.com/nodejs/node-chakracore/commit/d69e2c82a3)] - chakrashim: Add 'Debug' object only when needed

## 2016-01-15, node-chakracore-6.0.0-pre2

### Commits

* [[`e300485dbd`](https://github.com/nodejs/node-chakracore/commit/e300485dbd)] - chakrashim: tweak chakra_shim.js for eslint
* [[`5fe1c8f564`](https://github.com/nodejs/node-chakracore/commit/5fe1c8f564)] - docs: revert README.md bad merge
* [[`a3007fa58b`](https://github.com/nodejs/node-chakracore/commit/a3007fa58b)] - chakrashim: sync to chakracore 1.1.0.1
* [[`a5804ddb7d`](https://github.com/nodejs/node-chakracore/commit/a5804ddb7d)] - tool: Removed unnecessary arg from vcbuild.bat
* [[`3518aeee29`](https://github.com/nodejs/node-chakracore/commit/3518aeee29)] - deps: Update chakrashim's v8 header file version
* [[`1e3e8b81dd`](https://github.com/nodejs/node-chakracore/commit/1e3e8b81dd)] - Merge remote-tracking branch 'nodejs/master' into chnext
* [[`65b49fc532`](https://github.com/nodejs/node-chakracore/commit/65b49fc532)] - doc: Added IoT-Readme.md

## 2016-01-13, node-chakracore-6.0.0-pre1

### Commits

* [[`5d860fd86b`](https://github.com/nodejs/node-chakracore/commit/5d860fd86b)] - chakrashim: sync to chakracore release/1.1
* [[`6c70170eaf`](https://github.com/nodejs/node-chakracore/commit/6c70170eaf)] - chakrashim: Fixed multiple unittest bugs
* [[`ee68330fc7`](https://github.com/nodejs/node-chakracore/commit/ee68330fc7)] - chakrashim: Fix for nan v2.2 APIs
* [[`6bc3ef3fa1`](https://github.com/nodejs/node-chakracore/commit/6bc3ef3fa1)] - doc: Updated README.md for chakracore
* [[`49bb95d014`](https://github.com/nodejs/node-chakracore/commit/49bb95d014)] - test: Fixed unit test for chakra engine
* [[`889af372c3`](https://github.com/nodejs/node-chakracore/commit/889af372c3)] - chakrashim: add chakracore source
* [[`cb4788a1c2`](https://github.com/nodejs/node-chakracore/commit/cb4788a1c2)] - Merge remote-tracking branch 'nodejs/master' into chnext-chakracore
* [[`8d722fa371`](https://github.com/nodejs/node-chakracore/commit/8d722fa371)] - chakrashim: Implement IsMap/IsSet after merge
* [[`b259f11a90`](https://github.com/nodejs/node-chakracore/commit/b259f11a90)] - chakrashim: update AUTHORS
* [[`377347e915`](https://github.com/nodejs/node-chakracore/commit/377347e915)] - chakrashim: handle index properties in Object::Get/Set
* [[`a1a43a2aaa`](https://github.com/nodejs/node-chakracore/commit/a1a43a2aaa)] - chakrashim: lint errors cleanup
* [[`d1e310e633`](https://github.com/nodejs/node-chakracore/commit/d1e310e633)] - Show JS exception details for FATAL ERROR message
* [[`f44ff42988`](https://github.com/nodejs/node-chakracore/commit/f44ff42988)] - chakrashim: misc chakrashim related cleanup
* [[`f14d33795b`](https://github.com/nodejs/node-chakracore/commit/f14d33795b)] - chakrashim: handle Value::ToInteger overflow case
* [[`b6e38eb441`](https://github.com/nodejs/node-chakracore/commit/b6e38eb441)] - chakrashim: Added dummy 'Debug' in global object
* [[`04446e4648`](https://github.com/nodejs/node-chakracore/commit/04446e4648)] - chakrashim: Implemented ScriptOrigin signature
* [[`7953699687`](https://github.com/nodejs/node-chakracore/commit/7953699687)] - Merge remote-tracking branch 'nodejs/master' into chnext
* [[`26c1f6df22`](https://github.com/nodejs/node-chakracore/commit/26c1f6df22)] - chakrashim: JsCollectGarbage and cleanup
* [[`98d84cd8d4`](https://github.com/nodejs/node-chakracore/commit/98d84cd8d4)] - Merge remote-tracking branch 'nodejs/master' into chnext
* [[`1f2ab92a3a`](https://github.com/nodejs/node-chakracore/commit/1f2ab92a3a)] - chakrashim: Remove option to build with 'chakra'
* [[`c115f422f0`](https://github.com/nodejs/node-chakracore/commit/c115f422f0)] - chakrashim: Removed --no-duplicate-basename option
* [[`c9cf4ff042`](https://github.com/nodejs/node-chakracore/commit/c9cf4ff042)] - chakrashim: isMapIterator/isSetIterator bug fix
* [[`82ad0b3fe6`](https://github.com/nodejs/node-chakracore/commit/82ad0b3fe6)] - Added back jslint prefer-const rule
* [[`0e514a58ea`](https://github.com/nodejs/node-chakracore/commit/0e514a58ea)] - Merge remote-tracking branch 'nodejs/master' into chnext
* [[`5a3fca2936`](https://github.com/nodejs/node-chakracore/commit/5a3fca2936)] - chakrashim: Update chakracore to new header
* [[`490ef5860c`](https://github.com/nodejs/node-chakracore/commit/490ef5860c)] - chakrashim: Implemeted v8 APIs after merge
* [[`52780c70e4`](https://github.com/nodejs/node-chakracore/commit/52780c70e4)] - Merge remote-tracking branch 'nodejs/master' into chnext
* [[`5f6bc43b80`](https://github.com/nodejs/node-chakracore/commit/5f6bc43b80)] - chakrashim: Fixed discrepencies in ToInteger
* [[`0c45b5c7ff`](https://github.com/nodejs/node-chakracore/commit/0c45b5c7ff)] - chakrashim,msi: build with chakracore
* [[`e4eec51852`](https://github.com/nodejs/node-chakracore/commit/e4eec51852)] - chakrashim: Implemented 2 new nan.h APIs
* [[`777a04256c`](https://github.com/nodejs/node-chakracore/commit/777a04256c)] - Revert "lib,buffer: temp workaround chakra bug"
* [[`1a9ddec782`](https://github.com/nodejs/node-chakracore/commit/1a9ddec782)] - Merge remote-tracking branch 'node/master' into chnext
* [[`e4207e2a2f`](https://github.com/nodejs/node-chakracore/commit/e4207e2a2f)] - lib,buffer: temp workaround chakra bug
* [[`3a55f98d9a`](https://github.com/nodejs/node-chakracore/commit/3a55f98d9a)] - lib: mark Buffer/SlowBuffer not supporting @@species
* [[`91b675a8b3`](https://github.com/nodejs/node-chakracore/commit/91b675a8b3)] - Merge remote-tracking branch 'node/master' into chnext
* [[`2d1b2b4440`](https://github.com/nodejs/node-chakracore/commit/2d1b2b4440)] - chakrashim: update AUTHORS, cpplint
* [[`e4b077ca51`](https://github.com/nodejs/node-chakracore/commit/e4b077ca51)] - chakrashim: add implement v8::FunctionTemplate::Inherit I was implement v8::FunctionTemplate::Inherit and use in my game engine.
* [[`189df5f0aa`](https://github.com/nodejs/node-chakracore/commit/189df5f0aa)] - chakrashim: fix Object::SetInternalField … When use SetInternalField with Interger will assert:
* [[`f017c07056`](https://github.com/nodejs/node-chakracore/commit/f017c07056)] - chakrashim: update AUTHORS
* [[`17b1ed81b1`](https://github.com/nodejs/node-chakracore/commit/17b1ed81b1)] - chakrashim: remove unused code
* [[`7f4cf1fb58`](https://github.com/nodejs/node-chakracore/commit/7f4cf1fb58)] - chakrashim: remove supportsOverrideToString
* [[`47477f613b`](https://github.com/nodejs/node-chakracore/commit/47477f613b)] - chakrashim: fix index/toInteger int overflow
* [[`56dc0bb132`](https://github.com/nodejs/node-chakracore/commit/56dc0bb132)] - chakrashim: fix Object::SetInternalField
* [[`7a1fb01a7c`](https://github.com/nodejs/node-chakracore/commit/7a1fb01a7c)] - win,cctest: link as console subsystem
* [[`c10332c4d3`](https://github.com/nodejs/node-chakracore/commit/c10332c4d3)] - chakrashim: fix x64 build
* [[`776499c1d6`](https://github.com/nodejs/node-chakracore/commit/776499c1d6)] - chakrashim: Removed word 'TH2' from chnext codebase
* [[`efc1c4b690`](https://github.com/nodejs/node-chakracore/commit/efc1c4b690)] - chakrashim: fix TryCatch exception reporting
* [[`ace0109cdc`](https://github.com/nodejs/node-chakracore/commit/ace0109cdc)] - chakrashim: remove unused code
* [[`436704c4d7`](https://github.com/nodejs/node-chakracore/commit/436704c4d7)] - Merge remote-tracking branch 'node/master' into chnext
* [[`fea88a456b`](https://github.com/nodejs/node-chakracore/commit/fea88a456b)] - chakrashim: revert StackFrame method renaming
* [[`566cff6eb3`](https://github.com/nodejs/node-chakracore/commit/566cff6eb3)] - chakrashim: minor cleanup
* [[`8d73cfe0c0`](https://github.com/nodejs/node-chakracore/commit/8d73cfe0c0)] - chakrashim: Code refactoring for cached propertyIds
* [[`c932f01c22`](https://github.com/nodejs/node-chakracore/commit/c932f01c22)] - Merge remote-tracking branch 'node/master' into chnext
* [[`33d79fa095`](https://github.com/nodejs/node-chakracore/commit/33d79fa095)] - chakrashim: some minor shim cleanups
* [[`72f07025d2`](https://github.com/nodejs/node-chakracore/commit/72f07025d2)] - chakrashim: more v8.h shim alignment for nan.h
* [[`8519d3703b`](https://github.com/nodejs/node-chakracore/commit/8519d3703b)] - chakrashim: Add support for --use-strict flag
* [[`c21ef4b482`](https://github.com/nodejs/node-chakracore/commit/c21ef4b482)] - chakrashim: remove SAL annotations
* [[`56a5330950`](https://github.com/nodejs/node-chakracore/commit/56a5330950)] - chakrashim: do not pass nullptr to JsCallFunction
* [[`b461501a67`](https://github.com/nodejs/node-chakracore/commit/b461501a67)] - chakrashim: align Local/Persistent with official v8.h
* [[`39685df138`](https://github.com/nodejs/node-chakracore/commit/39685df138)] - chakrashim: Refactored LoadProc changes added earlier
* [[`2c45af262e`](https://github.com/nodejs/node-chakracore/commit/2c45af262e)] - chakrashim: Load chakra.dll dynamically for TH2
* [[`e282ea040c`](https://github.com/nodejs/node-chakracore/commit/e282ea040c)] - chakrashim: add missing Exception methods
* [[`a58a3cb062`](https://github.com/nodejs/node-chakracore/commit/a58a3cb062)] - chakrashim : Minor code cleanup
* [[`42923c5d7d`](https://github.com/nodejs/node-chakracore/commit/42923c5d7d)] - Merge remote-tracking branch 'node/master'
* [[`8cbbfbc730`](https://github.com/nodejs/node-chakracore/commit/8cbbfbc730)] - chakrashim: some v8.h shim cleanup
* [[`5a54c00ecd`](https://github.com/nodejs/node-chakracore/commit/5a54c00ecd)] - Merge pull request #64 from Microsoft/CodeCleanup
* [[`da8a753f48`](https://github.com/nodejs/node-chakracore/commit/da8a753f48)] - Merge remote-tracking branch 'iojs/master' into chnext
* [[`c50ad9de08`](https://github.com/nodejs/node-chakracore/commit/c50ad9de08)] - Merge remote-tracking branch 'iojs/next' into chnext
* [[`9a88a51d96`](https://github.com/nodejs/node-chakracore/commit/9a88a51d96)] - chakrashim: handle Symbol as property name
* [[`e17502f00d`](https://github.com/nodejs/node-chakracore/commit/e17502f00d)] - chakrashim: improve typed array and instanceof perf
* [[`a3d55afcca`](https://github.com/nodejs/node-chakracore/commit/a3d55afcca)] - chakrashim: some performance improvements
* [[`f33e43f1e9`](https://github.com/nodejs/node-chakracore/commit/f33e43f1e9)] - chakrashim: Use new Jsrt APIs JsGetContextOfObject, JsGetContextData and JsSetContextData
* [[`8851bb6bfe`](https://github.com/nodejs/node-chakracore/commit/8851bb6bfe)] - chakrashim: Removed cross context shim code
* [[`87862dc259`](https://github.com/nodejs/node-chakracore/commit/87862dc259)] - chakrashim: enable chakra experimental features
* [[`d24d422cc7`](https://github.com/nodejs/node-chakracore/commit/d24d422cc7)] - chakrashim: implement ArrayBuffer::New on external memory
* [[`39db229cc9`](https://github.com/nodejs/node-chakracore/commit/39db229cc9)] - Merge remote-tracking branch 'iojs/next' into chnext
* [[`45411a78ea`](https://github.com/nodejs/node-chakracore/commit/45411a78ea)] - chakrashim: changes to build with iojs next
* [[`f2183642d0`](https://github.com/nodejs/node-chakracore/commit/f2183642d0)] - chakrashim: add gyp flag msvs_use_library_dependency_inputs
* [[`d57d102759`](https://github.com/nodejs/node-chakracore/commit/d57d102759)] - gyp: add msvs_use_library_dependency_inputs support
* [[`0a2b36a256`](https://github.com/nodejs/node-chakracore/commit/0a2b36a256)] - chakrashim: fix Script::Compile AV
* [[`549e280811`](https://github.com/nodejs/node-chakracore/commit/549e280811)] - chakrashim: specify msvs_windows_target_platform_version
* [[`914f1b4dc2`](https://github.com/nodejs/node-chakracore/commit/914f1b4dc2)] - gyp: add msvs_windows_target_platform_version support
* [[`9a1945b9a4`](https://github.com/nodejs/node-chakracore/commit/9a1945b9a4)] - chakrashim: revert error types proxy change
* [[`9d2faefb88`](https://github.com/nodejs/node-chakracore/commit/9d2faefb88)] - win,gyp: quote action command
* [[`f345fb0b9a`](https://github.com/nodejs/node-chakracore/commit/f345fb0b9a)] - chakrashim: skip setweak/clearweak work when disposing runtime
* [[`79d4877184`](https://github.com/nodejs/node-chakracore/commit/79d4877184)] - repl,chakra: make 'Unterminated ...' errors recoverable
* [[`70f8352e27`](https://github.com/nodejs/node-chakracore/commit/70f8352e27)] - chakrashim: use proxy to patch error types
* [[`26ff29c344`](https://github.com/nodejs/node-chakracore/commit/26ff29c344)] - chakrashim: fix some cross-context bugs
* [[`93adbaad74`](https://github.com/nodejs/node-chakracore/commit/93adbaad74)] - chakrashim: display out of memory error message
* [[`0c41b4522f`](https://github.com/nodejs/node-chakracore/commit/0c41b4522f)] - chakrashim: fix cross context strictmode/bound function
* [[`94628e09bc`](https://github.com/nodejs/node-chakracore/commit/94628e09bc)] - chakrashim: revive object during ClearWeak
* [[`8a10839e58`](https://github.com/nodejs/node-chakracore/commit/8a10839e58)] - chakrashim: vm proxy and other fixes
* [[`13de41ae3d`](https://github.com/nodejs/node-chakracore/commit/13de41ae3d)] - build: refactor engine switch and process.jsEngine
* [[`4fb398a05a`](https://github.com/nodejs/node-chakracore/commit/4fb398a05a)] - chakrashim: reformat to pass cpplint
* [[`07547c2424`](https://github.com/nodejs/node-chakracore/commit/07547c2424)] - chakrashim: per isolate message listeners and embedded data
* [[`2bb7fc3b32`](https://github.com/nodejs/node-chakracore/commit/2bb7fc3b32)] - chakrashim: clean up warning on std::map use
* [[`ffe5db1925`](https://github.com/nodejs/node-chakracore/commit/ffe5db1925)] - chakrashim: fix toString on cross context objects
* [[`f60c528758`](https://github.com/nodejs/node-chakracore/commit/f60c528758)] - chakrashim: callbackinfo holder and accessor signature
* [[`aa96260f0d`](https://github.com/nodejs/node-chakracore/commit/aa96260f0d)] - node-gyp: revert arm specific libs change
* [[`c2b8d43fa8`](https://github.com/nodejs/node-chakracore/commit/c2b8d43fa8)] - chakrashim: disable warning c4091 and remove arm specific lib
* [[`8403d34afa`](https://github.com/nodejs/node-chakracore/commit/8403d34afa)] - npm: change node-gyp module to support chakrashim and ARM
* [[`6c445c6281`](https://github.com/nodejs/node-chakracore/commit/6c445c6281)] - openssl: enable building on ARM with chakrashim
* [[`47000c74f4`](https://github.com/nodejs/node-chakracore/commit/47000c74f4)] - add chakrashim and enable node.js on chakra
