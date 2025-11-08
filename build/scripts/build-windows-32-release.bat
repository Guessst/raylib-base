@echo off
set W64DEVKIT_X86_BIN_PATH=C:\progs\w64devkit-x86\bin
set PATH=%W64DEVKIT_X86_BIN_PATH%;%PATH%

@echo on

set SRC_PATH=%~dp0\..\..
set BUILD_OUT_PATH=%~dp0\..\windows\release-32
if not exist %BUILD_OUT_PATH% mkdir %BUILD_OUT_PATH%

gcc %SRC_PATH%\main.c ^
    -m32 ^
    -O2 ^
-o %BUILD_OUT_PATH%\game.exe ^
-I C:\dev\raylib-base\raylib-5.5\src ^
-L C:\dev\raylib-base\raylib-5.5\release\windows-32 -lraylib -lgdi32 -lwinmm -lopengl32

if %ERRORLEVEL% neq 0 (
    echo Build failed.
) else (
    echo Build succeeded.
)