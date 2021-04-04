# Windows Command Line Passer

Do you have a Windows application that you want to pass a certain command line arguments every time you start it? This app might be the solution!

This app will read a configuration file from its current directory (different from working directory), and it will run the specified Windows app with the command line arguments as specified in the configuration. This app will also append any additional command line arguments passed to this app to the specified Windows app. The launched Windows app will have the correct working directory as intended.

## Usage

This app will read a configuration file in the same directory as this app with the filename `parampasscfg.txt`. This app can be renamed to any name. This file can be encoded in UTF-8 or UTF-16 LE, but with Windows line endings (CRLF). The contents of this file is as follows:

```
program to launch.exe
command line arguments
```

The "program to launch" can be an absolute or relative. Relative names will be resolved against the location of this app, not the working directory nor `PATH` environment variable.

The "command line arguments" should be specified as if it were typed in command prompt.

## Example Configuration

In this example, we will configure Chromium browser to reduce its memory usage by limiting the number of spawned renderer processes. We would also like to move the Chromium's cache to another directory. To perform this action, move this app to Chromium's install folder, rename the original `chromium.exe` to any other name (e.g. `chromium-original.exe`) and rename this app to `chromium.exe`. Then create `parampasscfg.txt` with the following content:

```
chromium-original.exe
--renderer-process-limit=1 --disk-cache-dir=E:\chromium_cache
```

Now any instance of Chromium launched by any means, including opening link, opening from desktop or start menu, and opening from pinned taskbar will have those arguments applied.