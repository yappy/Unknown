# How to build

## Linux
Only tested on Debian 9!

### dependencies
* C/C++ toolchains and fundamental tools
  * apt-get build-essential
  * yum groupinstall "Development Tools"
* cmake
* SDL2
  * apt-get install libsdl2-dev libsdl2-image-dev
  * yum install SDL2-devel ...

### build
```
$ cd build/linux
$ ./cmake.sh
$ make -C build/Debug
$ make -C build/Release

To clean,
$ make -C build/Debug clean
$ make -C build/Release clean

To clean fully,
$ rm -rf build/
```

## Mac OS
Not yet.

## Windows
Only tested on Windows 10 + Visual Studio 2017
### dependencies
* Visual Studio 2017 (Visual C++)
* cmake (You can set PATH at installer)
* SDL2 is already prepaired in the repository.

### build
1. Move to build/{win32|win64}
1. Double-click cmake.bat
1. Double-click build.bat

Delete build/ dir to clean fully.
