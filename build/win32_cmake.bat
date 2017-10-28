@echo on
cd /d %~dp0

mkdir win32
cd win32
cmake -G "Visual Studio 15 2017" ../..

pause
