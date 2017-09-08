# How to build

## Linux
Only tested in Debian!

### dependencies
* C/C++ toolchains and fundamental tools
  * apt-get build-essential
  * yum groupinstall "Development Tools"
* cmake
* SDL2
  * apt-get install libsdl2-dev
  * yum install SDL2-devel

### build
```
$ cd build/
$ ./cmake.sh
$ make -C linux/Debug
$ make -C linux/Release

To clean,
$ make -C linux/Debug clean
$ make -C linux/Release clean

To clean fully,
$ rm -rf linux/
```

## Mac OS X
Not yet.

## Windows
Not yet.
