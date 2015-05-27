Evented I/O for javascript.
===

Node.js uses the V8 JavaScript engine by default. This GitHub fork enables
Node.js to optionally use the Chakra JavaScript engine on Windows 10, allowing
Node.js to run on Windows on ARM. Our goal is to merge back into master after
stabilizing this code, fixing key gaps and responding to early community
feedback.

### How it works

To enable building and running Node.js with the Chakra JavaScript engine, we
created a V8 API shim on top of the Chakra runtime hosting API ([JSRT]
(https://msdn.microsoft.com/en-us/library/dn249673(v=vs.94).aspx)). This shim
implemented most essential V8 APIs so that the underlying JavaScript engine
change is transparent to Node.js and other native addon modules written for V8.
All we need is to rebuild node.exe and native addon modules with Chakra.

<a name="windows_with_chakra"></a>
### How to build (with Chakra on Windows)

Prerequisites:

    * Windows 10 (latest)
    * Python 2.6 or 2.7
    * Visual Studio 2015 (RC or later)
    * Windows 10 Tools (Bundled in Visual Studio 2015, or install separately.)

To build node.exe:

```sh
vcbuild chakra nosign [x86|x64|arm]
```

<a name="build_native_addon_modules_with_chakra"></a>
### How to build and install native addon modules

Assume [local_repo] is the dir of your local clone of this repo. Add your build
of node.exe to the front of PATH:

```sh
set path=[local_repo]\release;%path%
```

To build a native addon module:

```sh
node.exe [local_repo]\deps\npm\node_modules\node-gyp\bin\node-gyp.js rebuild --nodedir=[local_repo] --msvs_version=2015
```

To install a native addon module:

```sh
node.exe [local_repo]\deps\npm\bin\npm-cli.js install [native_addon] --nodedir=[local_repo] --msvs_version=2015
```

### Original README

Read original node.js README [here](https://github.com/joyent/node).
