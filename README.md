Node.js
===
Node.js uses the V8 JavaScript engine by default. This project enables
Node.js to optionally use the Chakra JavaScript engine. Our goal is to merge
back into master after stabilizing this code, fixing key gaps and responding to
early community feedback.

### How it works

To enable building and running Node.js with the Chakra JavaScript engine, we
created a V8 API shim on top of the Chakra runtime hosting API ([JSRT]
(https://msdn.microsoft.com/en-us/library/dn249673(v=vs.94).aspx)). This shim
implemented most essential V8 APIs so that the underlying JavaScript engine
change is transparent to Node.js and other native addon modules written for V8.
All we need is to rebuild node.exe and native addon modules with Chakra.

### How to get started

* Download and install from [here](https://github.com/Microsoft/node/releases).
* Checkout Windows 10 IoT specific instructions and samples [here](IoT-Readme.md).

<a name="windows_with_chakra"></a>
### How to build
Prerequisites:

* [Python 2.6 or 2.7](https://www.python.org)
* [Visual
Studio](https://www.visualstudio.com/en-us/downloads/download-visual-studio-vs.aspx)

```sh
vcbuild chakracore nosign [x86|x64|arm]
```

### Original README

Read original node.js README [here](https://github.com/nodejs/node).
