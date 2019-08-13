# Node.js on ChakraCore

This project enables Node.js to optionally use the
[ChakraCore](https://github.com/Microsoft/ChakraCore) JavaScript engine. This
project is still **work in progress** and not an officially supported Node.js
branch. For more context into this project, please refer to the
[original PR](https://github.com/nodejs/node/pull/4765).

### How it works

To enable building and running Node.js with the ChakraCore JavaScript engine, a
V8 API shim (ChakraShim) is created on top of the ChakraCore runtime hosting API
([JSRT](https://github.com/Microsoft/ChakraCore/wiki/JavaScript-Runtime-(JSRT)-Overview).
ChakraShim implements the most essential V8 APIs so that the underlying
JavaScript engine change is transparent to Node.js and other native addon
modules written for V8.

A rebuild of node and native addon modules with ChakraCore is required for this
to work.

### Time Travel Debugging
Time-Travel debugging is an exciting new addition to Node.js debugging, first
introduced in Node-ChakraCore, and now supported by
[VSCode](https://github.com/microsoft/vscode/). We are developing in the open
and want to share our progress to get feedback, bug reports, functionality
requests, and pull-requests from the community.

Check out this [Time-Travel Debugging](TTD-README.md)
page to learn how to get started with TTD on Node-ChakraCore with VSCode.

### Node.js API (NAPI)
Node-ChakraCore is an active participant in the
[ABI-Stable-Node](https://github.com/nodejs/abi-stable-node) project also called
NAPI. The goal of this project is to ease the lives of native add-ons
developers, by providing a stable Node API which also guarantees stable ABI
across disparate Node versions. This allows native modules to just work across
different versions and flavors of Node.js without recompilations, reducing the
maintenance cost for module developers and thus improving compatibility.

Head over to the [NAPI Project repo](https://github.com/nodejs/abi-stable-node)
to learn more about it and ways to get involved.

### Installing prebuilt Node-ChakraCore binaries

You can download and install stable prebuilt Node-ChakraCore from the
[releases page](https://github.com/nodejs/node-chakracore/releases).

_For installing Node-ChakraCore side-by-side your existing Node installation, we
recommend [Node Version Switcher (NVS)](https://github.com/jasongin/nvs)._

```console
$ nvs remote chakracore https://nodejs.org/download/chakracore-release/
$ nvs add chakracore/latest
$ nvs use chakracore
```

Nightly builds of Node-ChakraCore are available at
[https://nodejs.org/download/chakracore-nightly/](https://nodejs.org/download/chakracore-nightly/)
Please use the following instructions to install the nightly builds.

```console
$ nvs remote chakracore-nightly https://nodejs.org/download/chakracore-nightly/
$ nvs add chakracore-nightly/latest
$ nvs use chakracore-nightly
```

### Building Node-ChakraCore

Node-ChakraCore can be built on Windows, Linux, and macOS.

If you are looking to build this yourself, here's what you will need.

#### Windows

Prerequisites:
* Windows 7 SP1 or higher (Windows 8.1 or higher for ARM builds)
* [Python 2.6 or 2.7](https://www.python.org)
* [Visual Studio](https://www.visualstudio.com/en-us/downloads/download-visual-studio-vs.aspx)

Build Command:
```console
> vcbuild [x86|x64|arm]
```

To run unit tests

```console
> vcbuild nobuild test ignore-flaky [x86|x64|arm]
```

To test if Node.js was built correctly with ChakraCore:

```console
> node -e "console.log('Hello from Node.js ' + process.jsEngine)"
Hello from Node.js chakracore
```

#### Linux

Install building tools and ChakraCore dependencies:
Clang 3.7+ / CMake 3.2 are required

Debian/Ubuntu:
```console
$ sudo apt-get install -y build-essential cmake clang libicu-dev
```

Fedora:
```console
$ su
$ dnf group install -y "Development Tools" "C Development Tools and Libraries"
$ dnf install -y git cmake clang gcc gcc-c++ kernel-devel python llvm
$ dnf install -y lttng-ust-devel.x86_64 libicu-devel.x86_64 libstdc++-static.x86_64
```

Clone this repo and check out **`master`** branch:

```console
$ git checkout master
```

Build:

```console
$ ./configure
$ make
```

To run unit tests:

```console
$ FLAKY_TESTS=dontcare make test
```

To test if Node.js was built correctly with ChakraCore:

```console
$ node -e "console.log('Hello from Node.js ' + process.jsEngine)"
Hello from Node.js chakracore
```

#### macOS

Prerequisites:
* Xcode v7 or higher
* [Homebrew](https://brew.sh/)

Install the building tools:

```console
$ xcode-select --install
$ brew install cmake
```

Clone this repo and check out **`master`** branch:

```console
$ git checkout master
```

Build:

```console
$ ./configure
$ make
```

To test if Node.js was built correctly with ChakraCore:

```console
$ ./node -e "console.log('Hello from Node.js ' + process.jsEngine)"
Hello from Node.js chakracore
```

### Issues

Please report all issues related to Node-ChakraCore on our
[issues page](https://github.com/nodejs/node-chakracore/issues).
