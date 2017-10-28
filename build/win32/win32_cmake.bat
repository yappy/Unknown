@echo on
cd /d %~dp0

mkdir build
cd build
cmake -G "Visual Studio 15 2017" ../../..

pause
