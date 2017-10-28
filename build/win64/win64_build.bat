@echo on
cd /d %~dp0

cmake --build build --config Debug
cmake --build build --config Release

pause
