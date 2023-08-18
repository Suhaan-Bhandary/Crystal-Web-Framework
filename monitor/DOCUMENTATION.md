# Monitor for C++ Server [Linux]

monitor is a tool that helps develop `C++ Framework from Scratch` based applications by automatically restarting the application when file changes in the directory are detected.

monitor does not require any additional changes to your code or method of development. monitor is a replacement for manually compiling and running the applications. To use monitor, simply call the monitor.sh from the root of the project(It take project root as the base directory).

## Dependencies

- inotify-tools: https://github.com/inotify-tools/inotify-tools/wiki
- monitor uses `jq` for reading the .monitor configuration. Download it from [https://jqlang.github.io/jq/download/](https://jqlang.github.io/jq/download/)

## Running the Server

Go to the Base Directory run the below command:

```bash
./monitor/monitor.sh
```

## Variables

monitor uses .monitor file to take variables as input in json format.

```json
{
  "outputFileName": "httpScratchServer",
  "scripts": {
    "build": "make all",
    "run": "./httpScratchServer"
  },
  "exclude": ["build", "monitor", ".vscode"] // By Default all files starting from . and .git is ignored
}
```
