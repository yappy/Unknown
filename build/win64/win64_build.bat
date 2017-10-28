@echo on
cd /d %~dp0

cmake --build build --config Debug
cmake --build build --config Release

cmake --build build --target INSTALL --config Release

pause
