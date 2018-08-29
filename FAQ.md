# Frequently Asked Questions

- [Can I use npm node-modules with nodejs-mobile?](#can-i-use-npm-node-modules-with-nodejs-mobile)
- [Are all Node.js APIs supported on mobile?](#are-all-nodejs-apis-supported-on-mobile)
- [Trying to write a file results in an error. What's going on?](#trying-to-write-a-file-results-in-an-error-whats-going-on)
- [Are Node.js native modules supported?](#are-nodejs-native-modules-supported)
- [How can I improve Node.js load times?](#how-can-i-improve-nodejs-load-times)
- [Can I run two or more Node.js instances?](#can-i-run-two-or-more-nodejs-instances)
- [Can I run Node.js code in a WebView?](#can-i-run-nodejs-code-in-a-webview)
- [Can you support a plugin for the X mobile framework?](#can-you-support-a-plugin-for-the-x-mobile-framework)

## Can I use npm node-modules with nodejs-mobile?

npm modules can be used with nodejs-mobile. They need to be installed at development time in the application source folder that contains the Node.js project files. There are samples that show how to use npm modules for [Android](https://github.com/janeasystems/nodejs-mobile-samples/tree/master/android/native-gradle-node-folder) and [iOS](https://github.com/janeasystems/nodejs-mobile-samples/tree/master/ios/native-xcode-node-folder) when using the native library directly. There are instructions in the [nodejs-mobile-cordova](https://github.com/janeasystems/nodejs-mobile-cordova#node-modules) and [nodejs-mobile-react-native](https://github.com/janeasystems/nodejs-mobile-react-native#node-modules) plugins README on how to use them.

## Are all Node.js APIs supported on mobile?

Not every API is supported on mobile, the main reason for this being that the mobile operating systems won't allow applications to call certain APIs that are expected to be available on other operating systems. Examples:
- APIs that create new processes, such as `child_process.spawn()` or `child_process.fork()` will run into permission issues.
- Using `process.exit()` is not allowed by the Apple App Store guildelines.
- `os.cpus()` may return inconsistent/unreliable results, since different OS versions will have different permissions for accessing CPU information.

## Trying to write a file results in an error. What's going on?

Mobile platforms are different than the usual desktop platforms in that they require applications to write in specific sandboxed paths and don't have permissions to write elsewhere. You should pass an appropriate writable path for your use case to the Node.js runtime and write there. An API call to return the path most regularly used for data in each platform has been added to the [nodejs-mobile-cordova](https://github.com/janeasystems/nodejs-mobile-cordova#cordovaappdatadir) and [nodejs-mobile-react-native](https://github.com/janeasystems/nodejs-mobile-react-native#rn_bridgeappdatadir) plugins.

## Are Node.js native modules supported?

Node native modules, which contain native code, are able to run on nodejs-mobile, as long as they can be cross-compiled for the target platform / CPU. The cross-compiling feature is integrated into the plugins and instructions can be found in the [nodejs-mobile-cordova](https://github.com/janeasystems/nodejs-mobile-cordova#native-modules) or in the [nodejs-mobile-react-native](https://github.com/janeasystems/nodejs-mobile-react-native#native-modules) README, but only Linux and MacOS development machines are currently supported. Modules that contain custom build steps and platform specific code may need workarounds/changes to get them to work. We've created a github repository so that the workarounds/changes can be discussed and shared: https://github.com/janeasystems/nodejs-mobile-module-compat

## How can I improve Node.js load times?

Applications that contain a large number of files in the Node.js project can have their load times decreased by reducing the number of files. While installing npm modules, these can be installed with the `--production` flag, so that modules that are used for development only are not included in your project, e.g.: `npm install --production <module_name>`. Using tools that merge all nodejs project files into a bundle, such as [`noderify`](https://www.npmjs.com/package/noderify) and using the bundle instead has been observed to improve load times in most situations.

## Can I run two or more Node.js instances?

No. The runtime expects to be run as a single instance in the process. In practice this should not preclude any usage scenarios, given node's asynchronous nature. Multiple sub-tasks can be executed by simply loading all the corresponding modules with `require` from a main script.

## Can I run Node.js code in a WebView?

No. Node.js uses a libuv event loop at its core, which is different than the event loop in the WebView. Having the node runtime run in its own thread also prevents Node.js tasks for interfering with the UI, which might cause responsiveness issues.
The supported usage scenario is that nodejs-mobile runs in a background thread and the UI (in this case a WebView) must use a communication mechanism to send/receive data from Node.js.
This technique is used in the `nodejs-mobile-cordova` plugin, where Cordova uses a dedicated thread to run Node.js alongside the WebView.

## Can you support a plugin for the X mobile framework?

We are currently focused on supporting cordova and react-native plugins only, but we are open to community contributions for other frameworks.
If you are interested in a particular framework, please see if [an issue](https://github.com/janeasystems/nodejs-mobile/issues/) for it has already been opened, and let us know about your interest in there. Otherwise, feel free to open a new issue.

