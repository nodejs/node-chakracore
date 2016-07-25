# Time Travel Debugging with Node-ChakraCore

Stepping forward after hitting a breakpoint is a familiar experience but what 
if you can step back in time?  Frequently developers find themselves hunting 
for failure clues (e.g. error text) in the log files and then searching for 
that in the code.  Once the log statement is found in the source code, the 
developer is left to re-construct the context for the error.  

Not anymore!  Time-Travel debugging (TTD) allows developers to look at the 
faulting code within the full fidelity of the debugger with all the runtime 
context preserved.  TTD works on the record and playback principle, the record 
mode creates a trace file during execution which can then be played back 
allowing developers to deeply inspect the code as it was during the original 
execution. 

## How to get started
This is a preview of the TTD functionality we are adding to Node & ChakraCore. 
There are bugs and performance issues that make it unsuitable for use as a 
production debugger. However, we are developing in the open we want to share 
our progress and hopefully  get feedback, bug reports, functionality requests, 
and pull-requests from the community. We are working to stabilize and rapidly 
improve the TTD functionality so check in often for updates!

### Setup
To get started with TTD you will need the following:

- VSCode Insider Build from here 
- Install Node-ChakraCore with TTD from [here]()  
   (or build the ```debugging-ttd-preview``` branch from [node/node-chakracore](https://github.com/nodejs/node-chakracore))

Note: Currently TTD support is available on Windows only.  Linux support will be available soon.

### Record TTD trace
To record a trace for debugging run the TTD enabled build of node with the record flag:   
```node.exe  -TTRecord:<Location to save Trace>  <app script>```  
Where the location of the saved trace is a **relative path** from the location of the TTD node.exe file.

### Debug a TTD Trace with VSCode
Make the following additions to the launch.json configuration in the VSCode project: 
- Add the following to ```runtimeArgs``` ```["--nolazy", "-TTReplay:<Location of saved Trace>", "-TTBreakFirst"]```.
- Set the runtime executable to the TTD enabled build of node.js.  

### Sample Program
The code for the WorkItemTracker sample is available [here](https://github.com/mrkmarron/WorkItemTrackerDemo).

## Feedback
Please let us know on our [issues page](https://github.com/nodejs/node-chakracore/issues) if you have any question or comment. 

