@echo off
set W64DEVKIT_X64_BIN_PATH=C:\progs\w64devkit-x64\bin
set PATH=%W64DEVKIT_X64_BIN_PATH%;%PATH%

@echo on

set SRC_PATH=%~dp0\..\..
set BUILD_OUT_PATH=%~dp0\..\windows\debug
if not exist %BUILD_OUT_PATH% mkdir %BUILD_OUT_PATH%

gcc %SRC_PATH%\main.c ^
    -Wall ^
    -Wextra ^
    -Werror ^
    -Wpedantic ^
    -Wshadow ^
    -Wconversion ^
    -Wsign-conversion ^
    -Wnull-dereference ^
    -g ^
    -O0 ^
-o %BUILD_OUT_PATH%\game.exe ^
-I %SRC_PATH%\include ^
-L %SRC_PATH%\lib -lraylib -lgdi32 -lwinmm

if %ERRORLEVEL% neq 0 (
    echo Build failed.
    exit /b %ERRORLEVEL%
) else (
    echo Build succeeded.
    exit 0
)