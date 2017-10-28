@echo on
cd /d %~dp0

mkdir win64
cd win64
cmake -G "Visual Studio 15 2017 Win64" ../..

pause
