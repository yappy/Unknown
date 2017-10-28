@echo on
cd /d %~dp0

cmake --build win64 --config Debug
cmake --build win64 --config Release

pause
