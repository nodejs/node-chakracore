Evented I/O for javascript.
===
Node.js uses the V8 JavaScript engine by default. This project enables 
Node.js to optionally use the Chakra JavaScript engine on Windows 10, allowing 
Node.js to run on Windows 10 IoT. Our goal is to merge back into master after 
stabilizing this code, fixing key gaps and responding to early community 
feedback.

### How it works

To enable building and running Node.js with the Chakra JavaScript engine, we
created a V8 API shim on top of the Chakra runtime hosting API ([JSRT]
(https://msdn.microsoft.com/en-us/library/dn249673(v=vs.94).aspx)). This shim
implemented most essential V8 APIs so that the underlying JavaScript engine
change is transparent to Node.js and other native addon modules written for V8.
All we need is to rebuild node.exe and native addon modules with Chakra.

### How to get started

You will need the following installed on your PC to start using Node.js with Chakra 

* [Windows 10](http://www.microsoft.com/en-us/windows/windows-10-upgrade)
* [Visual Studio 2015](https://www.visualstudio.com/vs-2015-product-editions)
(with Tools and Windows SDK) 
* [Node.js Tools for Windows IoT](https://github.com/ms-iot/ntvsiot/releases)

Checkout IoT specific instructions and samples [here](IoT-Readme.md).

<a name="windows_with_chakra"></a>
### How to build (with Chakra on Windows)

In addition to the above installations. You will need to install 
[Python 2.6 or 2.7](https://www.python.org) to build Node.js for Chakra

To build node.exe:

```sh
vcbuild chakra nosign [x86|x64|arm]
```

### Original README

Read original node.js README [here](https://github.com/joyent/node).
