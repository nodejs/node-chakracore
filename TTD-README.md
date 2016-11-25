# Time Travel Debugging with Node-ChakraCore

Stepping forward after hitting a breakpoint is a familiar experience but what 
if you could step back in time?  Frequently developers find themselves hunting 
for failure clues (e.g. error text) in the log files and then searching for 
that in the code.  Once the log statement is found in the source code, the 
developer is left to re-construct the context for the error.  

Not anymore!  Time-Travel debugging (TTD) allows developers to look at the 
faulting code within the full fidelity of the debugger with all the runtime 
context preserved.  TTD works on the record and playback principle, the record 
mode creates a trace file during execution which can then be played back 
allowing developers to deeply inspect the code as it was during the original 
execution. 

TTD functionality is being added to Node-ChakraCore and is supported by [VSCode](https://aka.ms/vscode-insider). 
We are developing in the open and want to share our progress to get feedback,
bug reports, functionality requests, and pull-requests from the community. 

## What's New
* TTD reaches Beta milestone on Windows
  * Added new feature **"Reverse Continue"**
  * Improved performance and reliability

* TTD preview available on macOS and Ubuntu 16.04
  * Added TTD support on experimental Node-ChakraCore cross-platform builds

![VSCode showing TTD with step back and reverse continue](https://cloud.githubusercontent.com/assets/1898161/20216922/a007d232-a81d-11e6-8631-df5f285d623b.gif)
### How to get started
To get started with TTD you will need the following:

- Install [VSCode Insider build](https://aka.ms/vscode-insider) 
- Install [Node-ChakraCore](https://github.com/nodejs/node-chakracore/releases)

### Record TTD trace
To record a trace for debugging run the TTD enabled build of node with the record flag:   
```node.exe -TTRecord:<Location to save Trace>  <app script>```  
Where the location of the saved trace is a **relative path** from the location of the TTD node.exe file.

### Replay a TTD Trace on the command line:
```node.exe -TTReplay:<Location of saved Trace>```

### Debug a TTD Trace with VSCode
Make the following additions to the launch.json configuration in the VSCode project: 
- Add the following to ```runtimeArgs``` ```["-TTDebug:<Location of saved Trace>", "-TTBreakFirst"]```.
- Set the runtime executable and cwd paths as needed for the TTD enabled build of node.js and location of the saved trace.

### Sample Program
The code for the Remote File System sample demo'ed at NodeInteractive is available [here](http://research.microsoft.com/en-us/um/people/marron/samples/RFSDemo.zip).

(Note: This demo requires TypeScript)

## Feedback
Please let us know on our [issues page](https://github.com/nodejs/node-chakracore/issues) if you have any question or comment. 

