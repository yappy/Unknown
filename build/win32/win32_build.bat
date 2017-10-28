@echo on
cd /d %~dp0

cmake --build win32 --config Debug
cmake --build win32 --config Release

pause
