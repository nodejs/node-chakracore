# ChakraShim ChangeLog

## 2017-10-03, node-chakracore-8.6.0

### Commits

* [[`f2c076f89a`](https://github.com/nodejs/node-chakracore/commit/f2c076f89a)] - napi,lint: fixing lint errors in node_api_jsrt.cc
* [[`ac5a059e2b`](https://github.com/nodejs/node-chakracore/commit/ac5a059e2b)] - napi: fix property string length
* [[`8d001ffb38`](https://github.com/nodejs/node-chakracore/commit/8d001ffb38)] - napi_jsrt: initialize memory in napi_adjust_external_memory
* [[`dea038833d`](https://github.com/nodejs/node-chakracore/commit/dea038833d)] - disabling inspector/test-contexts because it blocks CI
* [[`77d8bf905a`](https://github.com/nodejs/node-chakracore/commit/77d8bf905a)] - test: fixing jslint issues
* [[`2bc17ae221`](https://github.com/nodejs/node-chakracore/commit/2bc17ae221)] - lib,ttd: updating stale use of lazyConstants
* [[`00ff650ab1`](https://github.com/nodejs/node-chakracore/commit/00ff650ab1)] - napi,jsrt: fixed erroneous version number when registering napi modules
* [[`219ed708b7`](https://github.com/nodejs/node-chakracore/commit/219ed708b7)] - Updating flaky status for moved test
* [[`0b79dd37de`](https://github.com/nodejs/node-chakracore/commit/0b79dd37de)] - test: updating inspector tests to work or be marked as flaky
* [[`76c2d08dda`](https://github.com/nodejs/node-chakracore/commit/76c2d08dda)] - test,chakrashim: fixing v8::External objects to have no prototype
* [[`a642b88f0f`](https://github.com/nodejs/node-chakracore/commit/a642b88f0f)] - test: disabling part of util-format test due to different property ordering
* [[`6ede8e1f78`](https://github.com/nodejs/node-chakracore/commit/6ede8e1f78)] - napi: updating chakra napi implementation to match new spec
* [[`8aa008154a`](https://github.com/nodejs/node-chakracore/commit/8aa008154a)] - src: fixing trace-sync-io flag handling after bad merge
* [[`6db22af238`](https://github.com/nodejs/node-chakracore/commit/6db22af238)] - marking test-http2-client-promisify-connect as failing
* [[`743f6343c0`](https://github.com/nodejs/node-chakracore/commit/743f6343c0)] - test,build: implement no-ops for Promise::Resolver methods, mark tests as flaky
* [[`08a2c07619`](https://github.com/nodejs/node-chakracore/commit/08a2c07619)] - napi: Fix break on linux
* [[`0446278e4e`](https://github.com/nodejs/node-chakracore/commit/0446278e4e)] - build: fix lint issues
* [[`3cd421af84`](https://github.com/nodejs/node-chakracore/commit/3cd421af84)] - napi: Update jsrt implementation
* [[`2cbd199bdb`](https://github.com/nodejs/node-chakracore/commit/2cbd199bdb)] - build: fix lint issues
* [[`1daa223087`](https://github.com/nodejs/node-chakracore/commit/1daa223087)] - napi: Fix jsrt implementation of napi_create_async_work
* [[`fb77947c92`](https://github.com/nodejs/node-chakracore/commit/fb77947c92)] - napi: adding napi_run_script support for chakracore
* [[`598e085949`](https://github.com/nodejs/node-chakracore/commit/598e085949)] - n-api: implement stub for adjust_external_memory
* [[`a838d66cdc`](https://github.com/nodejs/node-chakracore/commit/a838d66cdc)] - ttd: adding ttd tracking logic to aliased_buffer
* [[`96ec10956f`](https://github.com/nodejs/node-chakracore/commit/96ec10956f)] - ttd: undoing custom TTD tracking for async-wrap
* [[`b54102b53d`](https://github.com/nodejs/node-chakracore/commit/b54102b53d)] - Merge tag 'v8.6.0' into v8.6.0-proposal
* [[`85d4313ddf`](https://github.com/nodejs/node-chakracore/commit/85d4313ddf)] - Merge tag 'v8.5.0' into v8.5.0-catchup
* [[`e97bfc8cb5`](https://github.com/nodejs/node-chakracore/commit/e97bfc8cb5)] - [Merge Microsoft/Chakracore@ee46fc449c] [MERGE #3732 @MSLaguana] Preparing for release 1.7.2
* [[`29a51f1791`](https://github.com/nodejs/node-chakracore/commit/29a51f1791)] - [Merge Microsoft/Chakracore@0709816282] [MERGE #3729 @suwc] 17-09 ChakraCore servicing release
* [[`a4eecc6046`](https://github.com/nodejs/node-chakracore/commit/a4eecc6046)] - [Merge Microsoft/Chakracore@e905806855] [MERGE #3682 @dilijev] Fix #2987: Per spec, skip over holes when iterating over localeList
* [[`e155292bc0`](https://github.com/nodejs/node-chakracore/commit/e155292bc0)] - [Merge Microsoft/Chakracore@8dd1b47365] [MERGE #3721 @jianchun] fix incorrect parameter to call ChangeStringLinguisticCase
* [[`c3abd0ad56`](https://github.com/nodejs/node-chakracore/commit/c3abd0ad56)] - [Merge Microsoft/Chakracore@860ad1275e] [MERGE #3719 @jianchun] fix JavascriptFunction::CheckValidDebugThunk CrossSite check
* [[`57b1462886`](https://github.com/nodejs/node-chakracore/commit/57b1462886)] - [Merge Microsoft/Chakracore@6c11e16af2] [MERGE #3707 @sigatrev] Remove unneeded symbol that is breaking RecordInlineeFrameInfo copy prop
* [[`2199111435`](https://github.com/nodejs/node-chakracore/commit/2199111435)] - [Merge Microsoft/Chakracore@f17e37db34] [MERGE #3680 @dilijev] Intl-ICU: Add LanguageTag RegExp; make Intl.*.supportedLocalesOf work under ICU
* [[`a3f3dc67e9`](https://github.com/nodejs/node-chakracore/commit/a3f3dc67e9)] - [Merge Microsoft/Chakracore@d655df5191] [MERGE #3712 @obastemur] fix recyclerList check failing on multi-thread usage
* [[`def07b77ac`](https://github.com/nodejs/node-chakracore/commit/def07b77ac)] - chakrashim: fixing build break with stub v8::Module shim
* [[`bb89ac91b5`](https://github.com/nodejs/node-chakracore/commit/bb89ac91b5)] - [Merge Microsoft/Chakracore@edbeb866a5] [MERGE #3516 @obastemur] perf: Improve hot MOD arithmetic
* [[`9991aac9ce`](https://github.com/nodejs/node-chakracore/commit/9991aac9ce)] - [Merge Microsoft/Chakracore@4f07a58eec] [MERGE #3607 @MSLaguana] Adding a new enumerator flag to support short-term accesses
* [[`c8f4860693`](https://github.com/nodejs/node-chakracore/commit/c8f4860693)] - [Merge Microsoft/Chakracore@743be56892] [MERGE #3702 @sigatrev] Call SetInternalProperty and fix flags
* [[`3396570d32`](https://github.com/nodejs/node-chakracore/commit/3396570d32)] - [Merge Microsoft/Chakracore@daa3fc0313] [MERGE #3604 @obastemur] perf: reduce collision in hot maps
* [[`e3599a3921`](https://github.com/nodejs/node-chakracore/commit/e3599a3921)] - [Merge Microsoft/Chakracore@9264e37437] [MERGE #3605 @obastemur] perf: use wmemcpy instead of memcpy for wide strings
* [[`b0afaed68f`](https://github.com/nodejs/node-chakracore/commit/b0afaed68f)] - [Merge Microsoft/Chakracore@d4c8561029] [MERGE #3541 @obastemur] Better Debug coverage, smaller binary size
* [[`1685cf32d9`](https://github.com/nodejs/node-chakracore/commit/1685cf32d9)] - chakrashim: added v8::Value::IsNullOrUndefined
* [[`270d15f217`](https://github.com/nodejs/node-chakracore/commit/270d15f217)] - [Merge Microsoft/Chakracore@cfebb39b75] [MERGE #3684 @digitalinfinity] Fix broken config parsing in ChakraFull
* [[`2d6277dfd8`](https://github.com/nodejs/node-chakracore/commit/2d6277dfd8)] - chakrashim: updating use of JsCopyString for perf
* [[`9b071a2041`](https://github.com/nodejs/node-chakracore/commit/9b071a2041)] - [Merge Microsoft/Chakracore@7b7ddfe8f3] [MERGE #3609 @MSLaguana] Changing utf8 conversion codex to improve perf and safety
* [[`e1adb2981a`](https://github.com/nodejs/node-chakracore/commit/e1adb2981a)] - [Merge Microsoft/Chakracore@09cf407949] [MERGE #3590 @mrkmarron] TTD: Fix leak on top-level function load and bug in external function inflate.
* [[`35c2017b81`](https://github.com/nodejs/node-chakracore/commit/35c2017b81)] - [Merge Microsoft/Chakracore@fcf9a18f07] [MERGE #3608 @Penguinwizzard] Add a flag to use a stronger array sort for bug analysis.
* [[`fc18b99ee1`](https://github.com/nodejs/node-chakracore/commit/fc18b99ee1)] - [Merge Microsoft/Chakracore@0df53e5b62] [MERGE #3578 @meg-gupta] Dont update valueInfo of bytecode constants
* [[`bc8df9e795`](https://github.com/nodejs/node-chakracore/commit/bc8df9e795)] - [Merge Microsoft/Chakracore@1262d2ea6d] [1.6>1.7] [MERGE #3583 @rajatd] Allow cross-site objects too in ActivationObjectEx::Is
* [[`b43c5eedef`](https://github.com/nodejs/node-chakracore/commit/b43c5eedef)] - [Merge Microsoft/Chakracore@a773e37180] [MERGE #3598 @Cellule] Fix #3597
* [[`44e7610190`](https://github.com/nodejs/node-chakracore/commit/44e7610190)] - [Merge Microsoft/Chakracore@4bf71d047f] [MERGE #3587 @rajatd] Setting internal properties with
* [[`6aff2d8c7e`](https://github.com/nodejs/node-chakracore/commit/6aff2d8c7e)] - [Merge Microsoft/Chakracore@e5c80f382f] [MERGE #3564 @dilijev] Fix space between , and exclude_xplat (rl doesn't handle spaces)
* [[`ebbb2a6146`](https://github.com/nodejs/node-chakracore/commit/ebbb2a6146)] - [Merge Microsoft/Chakracore@e2345e7d4a] [1.6>1.7] [MERGE #3584 @thomasmo] OS#13419689 - DOMFastPathGetter instructions do not propagate destination profile data, adding overhead to inlined getter calls
* [[`b871421a86`](https://github.com/nodejs/node-chakracore/commit/b871421a86)] - chakrashim: added missing Function methods
* [[`3a113372fa`](https://github.com/nodejs/node-chakracore/commit/3a113372fa)] - [Merge Microsoft/Chakracore@fe2ddd367e] [MERGE #3577 @meg-gupta] Remove additional edges in try finally flowgraph transformation
* [[`422dd870aa`](https://github.com/nodejs/node-chakracore/commit/422dd870aa)] - [Merge Microsoft/Chakracore@4a6e233be6] [MERGE #3576 @pleath] Clean up hasDeferredChild, hasRedeferrableChild, and IsRedeferrable() from FuncInfo.
* [[`0b424130a0`](https://github.com/nodejs/node-chakracore/commit/0b424130a0)] - chakrashim: make inspector asyc methods no-op
* [[`e1d5646df2`](https://github.com/nodejs/node-chakracore/commit/e1d5646df2)] - chakrashim: update platform implementation
* [[`0dacfea4a8`](https://github.com/nodejs/node-chakracore/commit/0dacfea4a8)] - [Merge Microsoft/Chakracore@b0c58262f5] Use Phase instead of boolean flag for GloboptInstrString dump
* [[`f270b1aa2f`](https://github.com/nodejs/node-chakracore/commit/f270b1aa2f)] - [Merge Microsoft/Chakracore@f75c14a5d2] [MERGE #3560 @Cellule] WASM: Spec fixes + improve debugging
* [[`c6d7c03235`](https://github.com/nodejs/node-chakracore/commit/c6d7c03235)] - [Merge Microsoft/Chakracore@c966983c14] [MERGE #3570 @Cellule] -on:debugbreak in test builds
* [[`865c082201`](https://github.com/nodejs/node-chakracore/commit/865c082201)] - [Merge Microsoft/Chakracore@4f8f11f61b] [MERGE #3571 @Cellule] Globopt Instr string config flag
* [[`e27adae3c2`](https://github.com/nodejs/node-chakracore/commit/e27adae3c2)] - [Merge Microsoft/Chakracore@6479e1b964] [1.6>1.7] [MERGE #3536 @meg-gupta] Do not peep closure stack symbols
* [[`7520bf696a`](https://github.com/nodejs/node-chakracore/commit/7520bf696a)] - [Merge Microsoft/Chakracore@94db98ccf2] [MERGE #3547 @Cellule] WASM: Update wabt and spec tests
* [[`7865ce1965`](https://github.com/nodejs/node-chakracore/commit/7865ce1965)] - [Merge Microsoft/Chakracore@789ab367c7] [MERGE #3526 @sigatrev] OS#9030969 fix stack trace with jitted loops
* [[`91a87601ff`](https://github.com/nodejs/node-chakracore/commit/91a87601ff)] - Fixing lint issue in jsrtisolateshim.cc
* [[`fb8af2d915`](https://github.com/nodejs/node-chakracore/commit/fb8af2d915)] - chakrashim: ref count data sent to SetEmbedderData
* [[`254a4e3aff`](https://github.com/nodejs/node-chakracore/commit/254a4e3aff)] - chakrashim: removing ObjectPrototypeToStringShim
* [[`98f43c33d8`](https://github.com/nodejs/node-chakracore/commit/98f43c33d8)] - [Merge Microsoft/Chakracore@a4f0672dd0] [MERGE #3550 @xiaoyinl] Fix illegal qualified name in ThreadContext.h (Fix #3507)
* [[`4ca31d4af4`](https://github.com/nodejs/node-chakracore/commit/4ca31d4af4)] - [Merge Microsoft/Chakracore@241da53bd4] [1.6>1.7] [MERGE #3546 @MikeHolman] disable asm.js in ES6 modules
* [[`4b7cfa8e64`](https://github.com/nodejs/node-chakracore/commit/4b7cfa8e64)] - [Merge Microsoft/Chakracore@497c38192d] [MERGE #3232 @xiaoyinl] Fix file handle leak in Helpers.cpp
* [[`b3075ba0b0`](https://github.com/nodejs/node-chakracore/commit/b3075ba0b0)] - [Merge Microsoft/Chakracore@08803454cc] [MERGE #3548 @Cellule] Put back assert that was removed in #3537
* [[`9d94628399`](https://github.com/nodejs/node-chakracore/commit/9d94628399)] - [Merge Microsoft/Chakracore@71d8959010] [MERGE #3235 @xiaoyinl] Fix memory leak in AutoSystemInfo::InitPhysicalProcessorCount
* [[`7a62dbea94`](https://github.com/nodejs/node-chakracore/commit/7a62dbea94)] - [Merge Microsoft/Chakracore@4014154daa] [MERGE #3240 @xiaoyinl] Fix memory leak in WScriptJsrt::LoadTextFileCallback
* [[`9d2463aa2a`](https://github.com/nodejs/node-chakracore/commit/9d2463aa2a)] - [Merge Microsoft/Chakracore@08324e3ada] [MERGE #3539 @Cellule] OS#13149792 WASM: Table grow(0) with initial: 0
* [[`c432f553ac`](https://github.com/nodejs/node-chakracore/commit/c432f553ac)] - [Merge Microsoft/Chakracore@1548eeaf78] [MERGE #3544 @MikeHolman] fix bugs in MemoryOperationLastError
* [[`592041ad9c`](https://github.com/nodejs/node-chakracore/commit/592041ad9c)] - [Merge Microsoft/Chakracore@872220a98a] [1.6>1.7] OS#13129251: Math.min/max should return an integer value when all of its params are integers This change addresses a perf issue where non-inlined Math.min/max are always floating-point vars. In the bug, this causes expensive bailouts in a loop that was setting to a Uin8ClampedArray. The fix is to check whether all of the parameters are tagged integers, and, if so, return an int. With a reduced repro of the scenario from the original page, there is a significant improvement, where the same function takes 20% of the time it did before. Normal usage of Math.max with 3 int parameters set to a var results in taking 65% of the time it did before. Normal usage of Math.max with a float parameter showed a 1-5% regression, depending on where the first non-int parameter is listed.
* [[`54c3f4edce`](https://github.com/nodejs/node-chakracore/commit/54c3f4edce)] - [Merge Microsoft/Chakracore@911cfcb915] [MERGE #3538 @sigatrev] OS:10898061 fix bug with cached scopes and default/destrctured arguments
* [[`464059db71`](https://github.com/nodejs/node-chakracore/commit/464059db71)] - [Merge Microsoft/Chakracore@3103d16574] [MERGE #3512 @Cellule] OOM / SO exception counter
* [[`01a4165770`](https://github.com/nodejs/node-chakracore/commit/01a4165770)] - [Merge Microsoft/Chakracore@4d0e8a6303] [MERGE #3537 @Cellule] Prioritize integer type in Lifetime
* [[`b92a9a59bf`](https://github.com/nodejs/node-chakracore/commit/b92a9a59bf)] - [Merge Microsoft/Chakracore@a662302d6b] [MERGE #3514 @Cellule] WASM - preserve nan bits
* [[`72aa31e53d`](https://github.com/nodejs/node-chakracore/commit/72aa31e53d)] - [Merge Microsoft/Chakracore@b8c3f5e761] [MERGE #3535 @Cellule] Cleanup FncFlags
* [[`d3518ebf0a`](https://github.com/nodejs/node-chakracore/commit/d3518ebf0a)] - [Merge Microsoft/Chakracore@1e02d86605] [MERGE #3534 @MikeHolman] change how we shift addresses from JIT proc to be more futureproof
* [[`90698304ad`](https://github.com/nodejs/node-chakracore/commit/90698304ad)] - [Merge Microsoft/Chakracore@84fd5b247b] [MERGE #3529 @Cellule] OS#11576900 Globopt array compensation
* [[`e04472e51d`](https://github.com/nodejs/node-chakracore/commit/e04472e51d)] - [Merge Microsoft/Chakracore@b78c03930b] [1.6>1.7] [MERGE #3522 @tcare] Fix #2983 WeakMap + HostObject === Sadness (TypeError when using HostObject with WeakMap)
* [[`359274fae5`](https://github.com/nodejs/node-chakracore/commit/359274fae5)] - [Merge Microsoft/Chakracore@6a23c85c43] [1.6>1.7] [MERGE #3533 @jianchun] module circular reference GetExportedNames/ResolveExport bugs
* [[`b07697088f`](https://github.com/nodejs/node-chakracore/commit/b07697088f)] - [Merge Microsoft/Chakracore@f2ff94fb6e] [1.6>1.7] [MERGE #3523 @suwc] Fix issue#3245: Semicolon should not be required after certain module export forms
* [[`245fad4fdb`](https://github.com/nodejs/node-chakracore/commit/245fad4fdb)] - [Merge Microsoft/Chakracore@2468ce3627] [MERGE #3399 @dilijev] Make toGMTString the same function object as toUTCString
* [[`52e082b7fd`](https://github.com/nodejs/node-chakracore/commit/52e082b7fd)] - [Merge Microsoft/Chakracore@cb9be1fe92] [MERGE #3528 @MikeHolman] fix double print in trace:backend
* [[`2771174f85`](https://github.com/nodejs/node-chakracore/commit/2771174f85)] - [Merge Microsoft/Chakracore@9ac1422978] [MERGE #3498 @ricobbe] OS:9682944 Fix bug in which we incorrectly copy-propagated away a load from a stack slot after boxing locals onto the heap
* [[`13bb661b6f`](https://github.com/nodejs/node-chakracore/commit/13bb661b6f)] - [Merge Microsoft/Chakracore@b7cbc50012] [1.6>1.7] [MERGE #3493 @MikeHolman] remove inconsistent check in BuildJITTimeData
* [[`cabe768c39`](https://github.com/nodejs/node-chakracore/commit/cabe768c39)] - [Merge Microsoft/Chakracore@8eacc83904] [1.6>1.7] [MERGE #3488 @akroshg] OS: 11478866 adding a probe_stack to one of GetPrototypeSpecial API.
* [[`2b9f5d07c8`](https://github.com/nodejs/node-chakracore/commit/2b9f5d07c8)] - [Merge Microsoft/Chakracore@0b6a59148a] [1.6>1.7] [MERGE #3518 @Cellule] DetermineSymBoundOffsetOrValueRelativeToLandingPad: Handle case where we are unable to find the matching bound
* [[`998919e8bb`](https://github.com/nodejs/node-chakracore/commit/998919e8bb)] - [Merge Microsoft/Chakracore@47ae4a21b0] [1.6>1.7] [MERGE #3486 @Cellule] WASM - fix assert in jsrt debugger
* [[`ed9edf39e1`](https://github.com/nodejs/node-chakracore/commit/ed9edf39e1)] - [Merge Microsoft/Chakracore@b7e0e7ccb8] [1.6>1.7] [MERGE #3510 @Cellule] WASM: strengthen defer parse
* [[`8699c040a1`](https://github.com/nodejs/node-chakracore/commit/8699c040a1)] - [Merge Microsoft/Chakracore@37742786a9] [1.6>1.7] [MERGE #3515 @MikeHolman] fix padding for rpc structs
* [[`756aff1253`](https://github.com/nodejs/node-chakracore/commit/756aff1253)] - [Merge Microsoft/Chakracore@04cd94e2e1] [1.6>1.7] [MERGE #3499 @meg-gupta] Retain subclass valueinfo while optimizing BailOnNotObject in OptTagChecks
* [[`cb4e16038d`](https://github.com/nodejs/node-chakracore/commit/cb4e16038d)] - [Merge Microsoft/Chakracore@68cc4e864c] [1.6>1.7] [MERGE #3504 @meg-gupta] Fix IV analysis when a loop modifies an induction variable of another sibling loop sharing the same loop parent
* [[`3bf590a89a`](https://github.com/nodejs/node-chakracore/commit/3bf590a89a)] - [Merge Microsoft/Chakracore@7fc3e4ce9a] [1.6>1.7] [MERGE #3336 @suwc] Fix Issue#3064/#3423: Cache conflicts in super property access
* [[`0ab13b6f81`](https://github.com/nodejs/node-chakracore/commit/0ab13b6f81)] -  chakrashim: update changelog

## 2017-08-18, node-chakracore-v8.4.0

### Commits

* [[`aff0749bd8`](https://github.com/nodejs/node-chakracore/commit/aff0749bd8)] - 2017-08-18, Node-ChakraCore Version 8.4.0
* [[`d81f653c86`](https://github.com/nodejs/node-chakracore/commit/d81f653c86)] - test: fix n-api test_constructor
* [[`cde57892b4`](https://github.com/nodejs/node-chakracore/commit/cde57892b4)] - Fixing up improperly ignored test
* [[`d169b37570`](https://github.com/nodejs/node-chakracore/commit/d169b37570)] - test: fixed unit test
* [[`8f3304ff79`](https://github.com/nodejs/node-chakracore/commit/8f3304ff79)] - napi_jsrt: adding napi_get_node_version
* [[`bc4b141c48`](https://github.com/nodejs/node-chakracore/commit/bc4b141c48)] - n-api,test: fixing test breaks for chakracore
* [[`b06061439f`](https://github.com/nodejs/node-chakracore/commit/b06061439f)] - Merge tag 'v8.4.0' into v8.4.0-proposal
* [[`61c16d0239`](https://github.com/nodejs/node-chakracore/commit/61c16d0239)] - chakrashim: update changelog

## 2017-08-16, node-chakracore-v8.3.0

### Commits

* [[`0a22416d67`](https://github.com/nodejs/node-chakracore/commit/0a22416d67)] - 2017-08-16, Node-ChakraCore Version 8.3.0
* [[`5ce62979a9`](https://github.com/nodejs/node-chakracore/commit/5ce62979a9)] - test: cleanup with upstream
* [[`f11801b191`](https://github.com/nodejs/node-chakracore/commit/f11801b191)] - test: update `areAllValuesEqual` to use strings
* [[`a41c632529`](https://github.com/nodejs/node-chakracore/commit/a41c632529)] - src: fix node-v8 build
* [[`d934b663f6`](https://github.com/nodejs/node-chakracore/commit/d934b663f6)] - test: fixed test case for v8
* [[`39876b5607`](https://github.com/nodejs/node-chakracore/commit/39876b5607)] - n-api: implement DataView APIs for ChakraCore
* [[`fef55b4d03`](https://github.com/nodejs/node-chakracore/commit/fef55b4d03)] - n-api: update error functions for JSRT
* [[`26790983ef`](https://github.com/nodejs/node-chakracore/commit/26790983ef)] - test: fix chakracore test failures
* [[`263686aa2b`](https://github.com/nodejs/node-chakracore/commit/263686aa2b)] - Merge tag 'v8.3.0' into 8.3.0-proposal
* [[`73ed2f0d4f`](https://github.com/nodejs/node-chakracore/commit/73ed2f0d4f)] - [Merge Microsoft/Chakracore@e2e415d467] [MERGE #3511 @MSLaguana] Preparing for release 1.7.1
* [[`c7283aa140`](https://github.com/nodejs/node-chakracore/commit/c7283aa140)] - [Merge Microsoft/Chakracore@b1e30e38a9] [MERGE #3487 @Cellule] WASM - improve some error messages
* [[`78d5a93da4`](https://github.com/nodejs/node-chakracore/commit/78d5a93da4)] - [Merge Microsoft/Chakracore@f092881abe] [MERGE #3506 @Cellule] Default case in InliningDecider::GetBuiltInInfoCommon
* [[`4d1aff14a8`](https://github.com/nodejs/node-chakracore/commit/4d1aff14a8)] - [Merge Microsoft/Chakracore@d08f260e7e] [1.6>1.7] [MERGE #3509 @pleath] 17-08 ChakraCore servicing release
* [[`3d822ec017`](https://github.com/nodejs/node-chakracore/commit/3d822ec017)] - [Merge Microsoft/Chakracore@f2b5bb29b9] [1.6>1.7] [MERGE #3468 @Cellule] Strengthen WebAssemblyArrayBuffer.GrowMemory
* [[`39553e7418`](https://github.com/nodejs/node-chakracore/commit/39553e7418)] - [Merge Microsoft/Chakracore@992b1d7a50] [1.6>1.7] [MERGE #3491 @Cellule] WASM - binary version 0xd
* [[`a1a64d130c`](https://github.com/nodejs/node-chakracore/commit/a1a64d130c)] - [Merge Microsoft/Chakracore@e0141fb48b] [MERGE #3503 @sigatrev] Fix Issue #3497: move initialization to constructor
* [[`569128fc0f`](https://github.com/nodejs/node-chakracore/commit/569128fc0f)] - [Merge Microsoft/Chakracore@177a7beaae] [1.6>1.7] [MERGE #3496 @MikeHolman] remove invalid special case in regalloc
* [[`bf0b020da4`](https://github.com/nodejs/node-chakracore/commit/bf0b020da4)] - [Merge Microsoft/Chakracore@be5c231aae] [1.6>1.7] [MERGE #3485 @agarwal-sandeep] OS# 12999605: Fix deleting of DebugContext
* [[`11c68c91a2`](https://github.com/nodejs/node-chakracore/commit/11c68c91a2)] - [Merge Microsoft/Chakracore@c19c1c0b33] [1.6>1.7] [MERGE #3424 @rajatd] OS#9726517 : Give NewScFunc the correct nested function index when we transfrom GetCachedFunc to NewScFunc in the backend
* [[`3ab059b8a8`](https://github.com/nodejs/node-chakracore/commit/3ab059b8a8)] - [Merge Microsoft/Chakracore@895f892129] [MERGE #3489 @obastemur] Convert internal static initializer (thread safe) to global
* [[`c2a86b6eeb`](https://github.com/nodejs/node-chakracore/commit/c2a86b6eeb)] - [Merge Microsoft/Chakracore@1b94b578c3] [MERGE #3455 @obastemur] JSON: Minor improvements
* [[`49f40e0f4f`](https://github.com/nodejs/node-chakracore/commit/49f40e0f4f)] - [Merge Microsoft/Chakracore@5dd3ed0264] [MERGE #3466 @obastemur] ToStringTagHelper: Make sure CompoundString has enough capacity
* [[`b1db7bac29`](https://github.com/nodejs/node-chakracore/commit/b1db7bac29)] - [Merge Microsoft/Chakracore@4559734d46] [MERGE #3454 @obastemur] Performance Improvements.
* [[`a07d1ba171`](https://github.com/nodejs/node-chakracore/commit/a07d1ba171)] - [Merge Microsoft/Chakracore@ac21f6c301] [MERGE #3420 @obastemur] linux: free datetime interface from tz
* [[`446526f583`](https://github.com/nodejs/node-chakracore/commit/446526f583)] - [Merge Microsoft/Chakracore@9c034052fa] [MERGE #3472 @obastemur] JSON: allocate enough cache
* [[`f74f22ee01`](https://github.com/nodejs/node-chakracore/commit/f74f22ee01)] - [Merge Microsoft/Chakracore@48de84a45f] [MERGE #3473 @MSLaguana] Fixing ninja builds as part of parent make
* [[`4d89f02ef8`](https://github.com/nodejs/node-chakracore/commit/4d89f02ef8)] - [Merge Microsoft/Chakracore@353d42059b] [1.6>1.7] [MERGE #3480 @jianchun] fix minor out of bound read (JavascriptString::ToInteger)
* [[`461c1b9248`](https://github.com/nodejs/node-chakracore/commit/461c1b9248)] - [Merge Microsoft/Chakracore@d7fba376b7] [1.6>1.7] [MERGE #3479 @tcare] Port AutoDisableInterrupt updates from internal branch
* [[`c5c5b96d7e`](https://github.com/nodejs/node-chakracore/commit/c5c5b96d7e)] - [Merge Microsoft/Chakracore@1cadbb3b51] [1.6>1.7] [MERGE #3470 @tcare] [Disabled Tests] test/Error/rlexe.xml: tests tagged
* [[`c9ffb2cb34`](https://github.com/nodejs/node-chakracore/commit/c9ffb2cb34)] - [Merge Microsoft/Chakracore@f95b7cbf33] [1.6>1.7] [MERGE #3469 @Cellule] WASM: reserved immediates
* [[`d3accb0c9c`](https://github.com/nodejs/node-chakracore/commit/d3accb0c9c)] - [Merge Microsoft/Chakracore@95f497c17f] [1.6>1.7] [MERGE #3463 @ricobbe] OS#11712101 Hoist CheckObjType out of loops only when the operand's containing object type is also invariant
* [[`3e93b74b61`](https://github.com/nodejs/node-chakracore/commit/3e93b74b61)] - chakrashim: fix console autocomplete in VS Code
* [[`8d74ab1008`](https://github.com/nodejs/node-chakracore/commit/8d74ab1008)] - [Merge Microsoft/Chakracore@42b5d73e48] [1.6>1.7] [MERGE #3471 @mrkmarron] Fix inflate table initialization + clang warnings.
* [[`9be2f7115e`](https://github.com/nodejs/node-chakracore/commit/9be2f7115e)] - [Merge Microsoft/Chakracore@f497760a25] [1.6>1.7] [MERGE #3460 @meg-gupta] Fix order of generating bailout info in ValueNumberLdElem
* [[`b8690b4061`](https://github.com/nodejs/node-chakracore/commit/b8690b4061)] - [Merge Microsoft/Chakracore@06c67e8493] [1.6>1.7] [MERGE #3426 @Cellule] WASM: uint64 -> double edge case
* [[`402e13f735`](https://github.com/nodejs/node-chakracore/commit/402e13f735)] - [Merge Microsoft/Chakracore@943277437f] [MERGE #3465 @obastemur] JSONStack: Improve performance
* [[`b6ed725156`](https://github.com/nodejs/node-chakracore/commit/b6ed725156)] - [Merge Microsoft/Chakracore@ff22ab8fa5] [MERGE #3456 @obastemur] xplat: fix multi-thread builds for node-chakracore
* [[`2585880651`](https://github.com/nodejs/node-chakracore/commit/2585880651)] - [Merge Microsoft/Chakracore@d8112a6ce8] [MERGE #3443 @obastemur] Make expensive entryRecord on Stack check is Debug Only
* [[`4276b7e598`](https://github.com/nodejs/node-chakracore/commit/4276b7e598)] - [Merge Microsoft/Chakracore@35ade809e7] Updating Bytecode Headers
* [[`72e65da1ef`](https://github.com/nodejs/node-chakracore/commit/72e65da1ef)] - [Merge Microsoft/Chakracore@12da6749b9] [1.6>1.7] [MERGE #3459 @sigatrev] fix crashes with cross site typed virtual arrays
* [[`1d4d95b2a9`](https://github.com/nodejs/node-chakracore/commit/1d4d95b2a9)] - [Merge Microsoft/Chakracore@79c13b4641] [1.6>1.7] [MERGE #3464 @aneeshdk] Handle strict mode in console scope
* [[`19468aa345`](https://github.com/nodejs/node-chakracore/commit/19468aa345)] - [Merge Microsoft/Chakracore@96b4d1f4a9] [1.6>1.7] [MERGE #3458 @aneeshdk] When ForceSplitScope flag is provided don't assume that we have non-simple parameter list
* [[`5a65ad2fae`](https://github.com/nodejs/node-chakracore/commit/5a65ad2fae)] - [Merge Microsoft/Chakracore@d43c23a9c4] [MERGE #3462 @kfarnung] Add JsGetDataViewInfo support
* [[`e309967b60`](https://github.com/nodejs/node-chakracore/commit/e309967b60)] - [Merge Microsoft/Chakracore@0eabffef38] [1.6>1.7] [MERGE #3457 @curtisman] Cleanup: Remove unused field in JavascriptLibrary and avoid casting in functionObjectTypesList
* [[`65515ebf39`](https://github.com/nodejs/node-chakracore/commit/65515ebf39)] - [Merge Microsoft/Chakracore@fdba5f1f9b] [1.6>1.7] [MERGE #3435 @suwc] OS12814968: ASSERTION:(lib\runtime\Base/FunctionBody.h) IsFunctionBody()
* [[`2e92601f2c`](https://github.com/nodejs/node-chakracore/commit/2e92601f2c)] - [Merge Microsoft/Chakracore@a8d700f7c4] [1.6>1.7] [MERGE #3451 @rajatd] Ensure that LdHeapArguments is the first thing in the bytecode after recording constants
* [[`b38d76678a`](https://github.com/nodejs/node-chakracore/commit/b38d76678a)] - [Merge Microsoft/Chakracore@6aeb615894] [MERGE #3437 @MSLaguana] Fixes #3430 overzealous type cast
* [[`7cb0191133`](https://github.com/nodejs/node-chakracore/commit/7cb0191133)] - optimize StringUtf8::From
* [[`62fc61254c`](https://github.com/nodejs/node-chakracore/commit/62fc61254c)] - [Merge Microsoft/Chakracore@8a61170da3] [1.6>1.7] [MERGE #3452 @akroshg] Splice helper function should check for side-effect in the prototype.
* [[`5a30dafb65`](https://github.com/nodejs/node-chakracore/commit/5a30dafb65)] - [Merge Microsoft/Chakracore@0c739a73be] [1.6>1.7] [MERGE #3445 @aneeshdk] Keep the same register allocation path for debug and non-debug mode in generators
* [[`8e87b75e31`](https://github.com/nodejs/node-chakracore/commit/8e87b75e31)] - [Merge Microsoft/Chakracore@391e601df6] [1.6>1.7] [MERGE #3428 @curtisman] Fix Issue #3039: Arrow function should terminate the expression unless followed by a comma
* [[`0db54b14c1`](https://github.com/nodejs/node-chakracore/commit/0db54b14c1)] - [Merge Microsoft/Chakracore@786f348d13] [1.6>1.7] [MERGE #3447 @dilijev] Fix #3438: AutoInitLibraryCodeScope: hide Intl.js initialization from debugger in addition to profiler.
* [[`e32bd433ea`](https://github.com/nodejs/node-chakracore/commit/e32bd433ea)] - [Merge Microsoft/Chakracore@76a2210a34] [1.6>1.7] [MERGE #3446 @akroshg] OS: 12891717 Missing conversion for CopyOnAccess Array on ForInObjectEnumerator
* [[`ae3534f138`](https://github.com/nodejs/node-chakracore/commit/ae3534f138)] - chakrashim: fix build break from JsCopyString
* [[`f7848cbd9c`](https://github.com/nodejs/node-chakracore/commit/f7848cbd9c)] - [Merge Microsoft/Chakracore@e0fb812bc1] [1.6>1.7] [MERGE #3440 @LouisLaf] Initialize locals
* [[`470b517e98`](https://github.com/nodejs/node-chakracore/commit/470b517e98)] - [Merge Microsoft/Chakracore@3c7ecf0b80] [MERGE #3433 @kunalspathak] Jsrt: Modify signature of JsCopyString
* [[`2adcecbac8`](https://github.com/nodejs/node-chakracore/commit/2adcecbac8)] - [Merge Microsoft/Chakracore@1ffb64cb4d] [1.6>1.7] [MERGE #3429 @atulkatti] Fixes #586 JsCreateNamedFunction should create a tracked PropertyId for name to ensure toString() works as expected.
* [[`3088134e60`](https://github.com/nodejs/node-chakracore/commit/3088134e60)] - [Merge Microsoft/Chakracore@39b89da350] [1.6>1.7] [MERGE #3436 @agarwal-sandeep] OS# 12840992: ARM: Wrong accessor flag offset passed for Getter/Setter inline cache check
* [[`d294e16224`](https://github.com/nodejs/node-chakracore/commit/d294e16224)] - [Merge Microsoft/Chakracore@55a0eb1825] [MERGE #3432 @MSLaguana] Fixing debug assert failure in WriteStringCopy
* [[`60ce06f472`](https://github.com/nodejs/node-chakracore/commit/60ce06f472)] - [Merge Microsoft/Chakracore@78b3208be5] [MERGE #3353 @obastemur] Debugger: keep new jobs while debugger is attached
* [[`0a29c27a8f`](https://github.com/nodejs/node-chakracore/commit/0a29c27a8f)] - [Merge Microsoft/Chakracore@a9d2418d41] [MERGE #3199 @obastemur] jsrt: don't check runtime on each API call for non-browser
* [[`68380ca52b`](https://github.com/nodejs/node-chakracore/commit/68380ca52b)] - [Merge Microsoft/Chakracore@a9c6c2dc41] [1.6>1.7] [MERGE #3416 @curtisman] Fix Issue #3261: Need to detect invalid null character at the end of the source string
* [[`dfc1581054`](https://github.com/nodejs/node-chakracore/commit/dfc1581054)] - [Merge Microsoft/Chakracore@5af1a8131b] [1.6>1.7] [MERGE #3425 @Cellule] Fix possible infinite loop in String.raw()
* [[`ab2781ace7`](https://github.com/nodejs/node-chakracore/commit/ab2781ace7)] - [Merge Microsoft/Chakracore@10d2f118ed] [1.6>1.7] [MERGE #3413 @atulkatti] Fixes #3065 Generator length property needs to be set of the script function it wraps.
* [[`f389099f4c`](https://github.com/nodejs/node-chakracore/commit/f389099f4c)] - [Merge Microsoft/Chakracore@8fb31110c6] [1.6>1.7] [MERGE #3415 @akroshg] slice helper should check for the ES5array or Proxy in the protototype
* [[`55746ee7a8`](https://github.com/nodejs/node-chakracore/commit/55746ee7a8)] - [Merge Microsoft/Chakracore@3c659bbe63] [1.6>1.7] [MERGE #3414 @curtisman] Fix issue #3393: Remove throwing accessor for caller property on argument object in strict mode
* [[`622673079e`](https://github.com/nodejs/node-chakracore/commit/622673079e)] - [Merge Microsoft/Chakracore@1b75f1bb64] [MERGE #3391 @kfarnung] Report script loads from eval when debugging
* [[`ea57ed7f3a`](https://github.com/nodejs/node-chakracore/commit/ea57ed7f3a)] - [Merge Microsoft/Chakracore@804093ebc3] [1.6>1.7] [MERGE #3338 @MikeHolman] assert that function we are JITing has a body
* [[`c98e0ceb85`](https://github.com/nodejs/node-chakracore/commit/c98e0ceb85)] - [Merge Microsoft/Chakracore@b5ebc92dbc] [1.6>1.7] [MERGE #3417 @MikeHolman] abort inlining dom fastpath if it didn't get registered
* [[`281cd64a00`](https://github.com/nodejs/node-chakracore/commit/281cd64a00)] - chakrashim: updating platform shim
* [[`1b6dd4cda7`](https://github.com/nodejs/node-chakracore/commit/1b6dd4cda7)] - chakrashim: update changelog for v8.2.1

## 2017-07-25, node-chakracore-v8.2.1

### Commits

* [[`b61651512e`](https://github.com/nodejs/node-chakracore/commit/b61651512e)] - 2017-07-25, Node-ChakraCore Version 8.2.1
* [[`0c1534fb74`](https://github.com/nodejs/node-chakracore/commit/0c1534fb74)] - chakrashim: fixing lint issue
* [[`93d295e397`](https://github.com/nodejs/node-chakracore/commit/93d295e397)] - chakrashim: Avoid calling JsCopyString twice to find length
* [[`5c6f12a888`](https://github.com/nodejs/node-chakracore/commit/5c6f12a888)] - chakrashim: using new jsrt interface
* [[`72b7dc2b39`](https://github.com/nodejs/node-chakracore/commit/72b7dc2b39)] - test: add assert to `engineSpecificMessage`
* [[`e04401e636`](https://github.com/nodejs/node-chakracore/commit/e04401e636)] - [Merge Microsoft/Chakracore@e63dc6fe31] [MERGE #3402 @dilijev] Change release/1.7 to RELEASE mode and update bytecode GUID.
* [[`58217deefc`](https://github.com/nodejs/node-chakracore/commit/58217deefc)] - [Merge Microsoft/Chakracore@a95b99fb36] [MERGE #3308 @obastemur] Library: Create less new string object for known texts
* [[`15c2b8e964`](https://github.com/nodejs/node-chakracore/commit/15c2b8e964)] - [Merge Microsoft/Chakracore@9626882ac9] [MERGE #3222 @obastemur] NumberToString: Improve performance and reduce cache memory
* [[`234730468e`](https://github.com/nodejs/node-chakracore/commit/234730468e)] - [Merge Microsoft/Chakracore@e76b30e5ca] [1.6>1.7] [MERGE #3389 @sigatrev] OS#12528802 fix crash accessing FuncObjectOpnd when function object is inlined
* [[`207314d500`](https://github.com/nodejs/node-chakracore/commit/207314d500)] - [Merge Microsoft/Chakracore@8cfe47de52] [1.6>1.7] [MERGE #3400 @jianchun] jsrt: cleanup CHAKRACOREBUILD_ symbol in headers
* [[`37f904636b`](https://github.com/nodejs/node-chakracore/commit/37f904636b)] - [Merge Microsoft/Chakracore@66bc22eceb] [1.6>1.7] [MERGE #3382 @rajatd] On .caller, only invalidate cached scopes for nested parents
* [[`f8dbdd5c03`](https://github.com/nodejs/node-chakracore/commit/f8dbdd5c03)] - [Merge Microsoft/Chakracore@ad54488def] [1.6>1.7] [MERGE #3411 @agarwal-sandeep] Fix -stats:rejit for chakra full
* [[`ec4d0042f8`](https://github.com/nodejs/node-chakracore/commit/ec4d0042f8)] - [Merge Microsoft/Chakracore@36fc51b58f] [1.6>1.7] [MERGE #3412 @curtisman] Fix Issue #3368:  Symbol.species.toString() shouldn't have bracket around the name
* [[`af938b6bf7`](https://github.com/nodejs/node-chakracore/commit/af938b6bf7)] - [Merge Microsoft/Chakracore@cb5557fe2d] [1.6>1.7] [MERGE #3409 @curtisman] Fix Issue #3376:  Escaped yield cannot be an identifier in strict mode
* [[`3d768de09f`](https://github.com/nodejs/node-chakracore/commit/3d768de09f)] - [Merge Microsoft/Chakracore@36316625f2] [1.6>1.7] [MERGE #3377 @suwc] Fix module namespace property attributes
* [[`3e73ac236c`](https://github.com/nodejs/node-chakracore/commit/3e73ac236c)] - [Merge Microsoft/Chakracore@07ef27bd6f] [MERGE #3408 @MSLaguana] Adding Jsrt function JsCopyStringOneByte
* [[`47e05bb9ac`](https://github.com/nodejs/node-chakracore/commit/47e05bb9ac)] - [Merge Microsoft/Chakracore@7d7010683e] [1.6>1.7] [MERGE #3397 @jackhorton] Fix crash when a Map is constructed with custom Map.prototype.set
* [[`6494fd07a4`](https://github.com/nodejs/node-chakracore/commit/6494fd07a4)] - [Merge Microsoft/Chakracore@7829651f00] [1.6>1.7] [MERGE #3328 @suwc] Fix Issue#3217: Reflect.construct permanently corrupts the invoked constructor
* [[`7b06d0f243`](https://github.com/nodejs/node-chakracore/commit/7b06d0f243)] - [Merge Microsoft/Chakracore@083ab297e1] [1.6>1.7] [MERGE #3398 @suwc] OS12503560: assignment to super[prop] not accessing base class property
* [[`b168aa70f4`](https://github.com/nodejs/node-chakracore/commit/b168aa70f4)] - [Merge Microsoft/Chakracore@6489b97aae] [1.6>1.7] [MERGE #3392 @atulkatti] Fixes #3009 The Enumerator built-in has been removed, so removing the tests.
* [[`8dbdd50cb9`](https://github.com/nodejs/node-chakracore/commit/8dbdd50cb9)] - [Merge Microsoft/Chakracore@8bbe7b2e82] [1.6>1.7] [MERGE #3323 @dilijev] Fix #3203, Fix #3204: Intl.getCanonicalLocales: name, toString, cannot call with new
* [[`334671628a`](https://github.com/nodejs/node-chakracore/commit/334671628a)] - [Merge Microsoft/Chakracore@1e7793cfdf] [1.6>1.7] [MERGE #3378 @meg-gupta] TryFinally Fixes
* [[`605163c8ce`](https://github.com/nodejs/node-chakracore/commit/605163c8ce)] - [Merge Microsoft/Chakracore@7df7f780dd] [MERGE #3360 @jackhorton] Fixes #3192, allows promise callback to be set to null
* [[`b393ea5abf`](https://github.com/nodejs/node-chakracore/commit/b393ea5abf)] - [Merge Microsoft/Chakracore@ea9514eac7] [1.6>1.7] [MERGE #3385 @thomasmo] OS#11221802 Address a reliability issue processes are running down during JIT server call
* [[`43606f017e`](https://github.com/nodejs/node-chakracore/commit/43606f017e)] - Merge tag 'v8.2.1' into v8.x
* [[`91485fc44d`](https://github.com/nodejs/node-chakracore/commit/91485fc44d)] - src: fix cpplint error
* [[`ea53d992dc`](https://github.com/nodejs/node-chakracore/commit/ea53d992dc)] - test: mark new flaky test
* [[`51451f6bc1`](https://github.com/nodejs/node-chakracore/commit/51451f6bc1)] - n-api: support napi_fatal_error
* [[`02bb58e7af`](https://github.com/nodejs/node-chakracore/commit/02bb58e7af)] - chakrashim: removed shim for `hasOwnProperty`
* [[`46d1389a41`](https://github.com/nodejs/node-chakracore/commit/46d1389a41)] - n-api: support napi_has_own_property
* [[`54f12fd9ad`](https://github.com/nodejs/node-chakracore/commit/54f12fd9ad)] - n-api: implement delete_property,
* [[`9507a2b418`](https://github.com/nodejs/node-chakracore/commit/9507a2b418)] - chakrashim: fixing cpplint errors
* [[`b47e608789`](https://github.com/nodejs/node-chakracore/commit/b47e608789)] - Merge pull request #328 from obastemur/idle_idle
* [[`0aa2fba05b`](https://github.com/nodejs/node-chakracore/commit/0aa2fba05b)] - test: fixed test-util-format
* [[`9ab193b28f`](https://github.com/nodejs/node-chakracore/commit/9ab193b28f)] - n-api: update napi_wrap error behavior
* [[`cbfb52010b`](https://github.com/nodejs/node-chakracore/commit/cbfb52010b)] - test: fixed a unit test
* [[`f287ae2721`](https://github.com/nodejs/node-chakracore/commit/f287ae2721)] - chakrashim: Added shim for EnqueueMicrotask
* [[`980cf6f78e`](https://github.com/nodejs/node-chakracore/commit/980cf6f78e)] - Merge tag 'v8.2.0' into v8.x
* [[`6c4e3057df`](https://github.com/nodejs/node-chakracore/commit/6c4e3057df)] - build: adding LTO support to configure script
* [[`eb638ccc91`](https://github.com/nodejs/node-chakracore/commit/eb638ccc91)] - [Merge Microsoft/Chakracore@3747582647] [1.6>1.7] [MERGE #3384 @agarwal-sandeep] OS#12654029: ArrayBuffer throw OOM if unable to allocate memory after GC
* [[`64022f15a1`](https://github.com/nodejs/node-chakracore/commit/64022f15a1)] - [Merge Microsoft/Chakracore@e4f3c46cb3] [1.6>1.7] [MERGE #3390 @agarwal-sandeep] Fixes #3387: Fix build break with simd unused variable
* [[`6ee9608170`](https://github.com/nodejs/node-chakracore/commit/6ee9608170)] - Update chakracore to 1.7
* [[`540dc290fc`](https://github.com/nodejs/node-chakracore/commit/540dc290fc)] - chakrashim: update changelog for v8.1.4 release

## 2017-07-14, node-chakracore-v8.1.4

### Commits

* [[`b135790c6d`](https://github.com/nodejs/node-chakracore/commit/b135790c6d)] - [Merge Microsoft/Chakracore@359cca5ec4] [MERGE #3349 @MSLaguana] Update version number to 1.5.3
* [[`bb53c85c12`](https://github.com/nodejs/node-chakracore/commit/bb53c85c12)] - [Merge Microsoft/Chakracore@bd1dba2da8] [MERGE #3341 @akroshg] 17-07 ChakraCore servicing release
* [[`0fbfbf91f7`](https://github.com/nodejs/node-chakracore/commit/0fbfbf91f7)] - [Merge Microsoft/Chakracore@5d9535ac77] [MERGE #3190 @obastemur] xplat: fix ICU lib tilde path problem
* [[`65e3ec19c9`](https://github.com/nodejs/node-chakracore/commit/65e3ec19c9)] - [Merge Microsoft/Chakracore@f91c91297d] [MERGE #3051 @MSLaguana] Updating release notes link
* [[`06599312a2`](https://github.com/nodejs/node-chakracore/commit/06599312a2)] - src: TTD fixes with async-wrap
* [[`a5714cc671`](https://github.com/nodejs/node-chakracore/commit/a5714cc671)] - Merge tag 'v8.1.4' into v8.x
* [[`fa7db0b9eb`](https://github.com/nodejs/node-chakracore/commit/fa7db0b9eb)] - build: split up cpplint to avoid long cmd lines
* [[`fbc9efaa6f`](https://github.com/nodejs/node-chakracore/commit/fbc9efaa6f)] - doc: fix broken markdown link
* [[`e7b475fbb3`](https://github.com/nodejs/node-chakracore/commit/e7b475fbb3)] - doc: update readme to use the master branch
* [[`74c0740ae6`](https://github.com/nodejs/node-chakracore/commit/74c0740ae6)] - test: skip a check in test_handle_scope
* [[`89800a5324`](https://github.com/nodejs/node-chakracore/commit/89800a5324)] - n-api: removed `napi_status_last` usage
* [[`306b517339`](https://github.com/nodejs/node-chakracore/commit/306b517339)] - Merge tag 'v8.1.3' into v8.x

## 2017-06-27, node-chakracore-8.1.2-pre2

### Commits

* [[`4be96a0e53`](https://github.com/nodejs/node-chakracore/commit/4be96a0e53)] - chakrashim: add shim for JSON methods
* [[`360e93eaca`](https://github.com/nodejs/node-chakracore/commit/360e93eaca)] - chakrashim: more update-changelog fixes
* [[`1c364f5e9d`](https://github.com/nodejs/node-chakracore/commit/1c364f5e9d)] - build: chakracore build improvements
* [[`13230d45fb`](https://github.com/nodejs/node-chakracore/commit/13230d45fb)] - tools: Fixed update-changelog

## 2017-06-19, node-chakracore-8.1.2-pre

### Commits

* [[`19f9347b8b`](https://github.com/nodejs/node-chakracore/commit/19f9347b8b)] - Merge tag 'v8.1.2' into v8.x
* [[`acb450af42`](https://github.com/nodejs/node-chakracore/commit/acb450af42)] - [Merge Microsoft/Chakracore@93ec291cff] [MERGE #3166 @jianchun] 17-06 ChakraCore servicing release
* [[`ddb361eb90`](https://github.com/nodejs/node-chakracore/commit/ddb361eb90)] - [Merge Microsoft/Chakracore@b3ec4b6344] Update version number to 1.5.2
* [[`083bf13949`](https://github.com/nodejs/node-chakracore/commit/083bf13949)] - [Merge Microsoft/Chakracore@96387111d7] [MERGE #3135 @obastemur] xplat: Improve ICU fallback conditions
* [[`0290d5b03f`](https://github.com/nodejs/node-chakracore/commit/0290d5b03f)] - test: mark sequential/test-benchmark-child-process as flaky
* [[`e72523bde1`](https://github.com/nodejs/node-chakracore/commit/e72523bde1)] - test: enable addons-napi/test_reference
* [[`7517f978cc`](https://github.com/nodejs/node-chakracore/commit/7517f978cc)] - test: fixing test breaks
* [[`8356c2b242`](https://github.com/nodejs/node-chakracore/commit/8356c2b242)] - Merge tag 'v8.1.1' into v8.x
* [[`16160c237b`](https://github.com/nodejs/node-chakracore/commit/16160c237b)] - build: move SDK detection to npm-cli.js
* [[`25bdb95a57`](https://github.com/nodejs/node-chakracore/commit/25bdb95a57)] - chakrashim: fix debug build failure
* [[`d03aa0a20d`](https://github.com/nodejs/node-chakracore/commit/d03aa0a20d)] - chakrashim: Implementing v8::Message
* [[`fa6b635e3f`](https://github.com/nodejs/node-chakracore/commit/fa6b635e3f)] - build,chakrashim: enabled cpplint for chakrashim
* [[`d94d248882`](https://github.com/nodejs/node-chakracore/commit/d94d248882)] - chakrashim: cleanup inspector code
* [[`d33c32f5ac`](https://github.com/nodejs/node-chakracore/commit/d33c32f5ac)] - [Merge Microsoft/Chakracore@a96455ed5f] [MERGE #3107 @obastemur] xplat: fix script location
* [[`99a22951c0`](https://github.com/nodejs/node-chakracore/commit/99a22951c0)] - [Merge Microsoft/Chakracore@d8c654e810] [MERGE #3102 @dilijev] release/1.5: Update version number to 1.5.1
* [[`5868614ec1`](https://github.com/nodejs/node-chakracore/commit/5868614ec1)] - lib,src: enable TTD record with env variable
* [[`b0681ba229`](https://github.com/nodejs/node-chakracore/commit/b0681ba229)] - test: ignore new failing tests
* [[`4b500dbe23`](https://github.com/nodejs/node-chakracore/commit/4b500dbe23)] - test: mark some test as flaky
* [[`1cf1355f66`](https://github.com/nodejs/node-chakracore/commit/1cf1355f66)] - src: fixing build break
* [[`bb57643592`](https://github.com/nodejs/node-chakracore/commit/bb57643592)] - Merge tag 'v8.1.0' into v8.x
* [[`48524b8340`](https://github.com/nodejs/node-chakracore/commit/48524b8340)] - [Merge Microsoft/Chakracore@07c15e7a55] [1.4>1.5] [MERGE #3092 @leirocks] fix handle leak for recycler concurrent thread
* [[`f92db9dff9`](https://github.com/nodejs/node-chakracore/commit/f92db9dff9)] - n-api: enable napi_wrap() to work with any object
* [[`d8ef944437`](https://github.com/nodejs/node-chakracore/commit/d8ef944437)] - chakrashim: fix undefined values in inspector
* [[`7528ca6106`](https://github.com/nodejs/node-chakracore/commit/7528ca6106)] - test: fixing test breaks
* [[`99c3ef57b2`](https://github.com/nodejs/node-chakracore/commit/99c3ef57b2)] - n-api: Added jsrt implementation for napi_get_version
* [[`09e9f4a28c`](https://github.com/nodejs/node-chakracore/commit/09e9f4a28c)] - chakrashim: updated changelog
* [[`157ff2ce2b`](https://github.com/nodejs/node-chakracore/commit/157ff2ce2b)] - test: marking new flaky test
* [[`a1568bdc31`](https://github.com/nodejs/node-chakracore/commit/a1568bdc31)] - build: fix ARM build
* [[`189106edd8`](https://github.com/nodejs/node-chakracore/commit/189106edd8)] - [Merge Microsoft/Chakracore@c36f7bf022] Ignore VS2017 linker warning LNK4221
* [[`54d80bb1e0`](https://github.com/nodejs/node-chakracore/commit/54d80bb1e0)] - chakrashim: fix inspector asserts
* [[`a2b74e6ee0`](https://github.com/nodejs/node-chakracore/commit/a2b74e6ee0)] - build: restore intl check for inspector
* [[`5fb2c2454b`](https://github.com/nodejs/node-chakracore/commit/5fb2c2454b)] - test: fix/ignore some unit-test
* [[`3d868ab427`](https://github.com/nodejs/node-chakracore/commit/3d868ab427)] - src,n-api: n-api bug fix
* [[`cebc14aa33`](https://github.com/nodejs/node-chakracore/commit/cebc14aa33)] - n-api: fix TypedAarray info for ArrayBuffer offset
* [[`3a047d77dd`](https://github.com/nodejs/node-chakracore/commit/3a047d77dd)] - [Merge Microsoft/Chakracore@f105d21acf] [MERGE #3023 @obastemur] xplat: Packaging shasum check
* [[`f61d10525d`](https://github.com/nodejs/node-chakracore/commit/f61d10525d)] - [Merge Microsoft/Chakracore@4c0fcf1480] [MERGE #2990 @obastemur] xplat: protect CC from un-reasonable stack limits
* [[`9f03510803`](https://github.com/nodejs/node-chakracore/commit/9f03510803)] - [Merge Microsoft/Chakracore@36d1ef2fa7] [MERGE #3015 @dilijev] release/1.5: Update version number to 1.5.0
* [[`00894102ef`](https://github.com/nodejs/node-chakracore/commit/00894102ef)] - build: fixing lint issues
* [[`13e19f4bf5`](https://github.com/nodejs/node-chakracore/commit/13e19f4bf5)] - [Merge Microsoft/Chakracore@e6d6de4f86] [MERGE #3006 @mrkmarron] TTD -- Correct behavior around unhandled exception & debugger
* [[`c4062ed021`](https://github.com/nodejs/node-chakracore/commit/c4062ed021)] - build: marking build as pre-release
* [[`897ea918c6`](https://github.com/nodejs/node-chakracore/commit/897ea918c6)] - build: fixing merge build breaks
* [[`7f1ea0d24f`](https://github.com/nodejs/node-chakracore/commit/7f1ea0d24f)] - Merge tag 'v8.0.0' into v8.x-new
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
