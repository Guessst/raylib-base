:: project setup
set PROJECT_ROOT=C:\dev\raylib-base

:: put gcc in PATH
set PATH_DO_NOT_CHANGE=%PATH%
set W64DEVKIT_X64_BIN_PATH=C:\progs\w64devkit-x64\bin
set W64DEVKIT_X86_BIN_PATH=C:\progs\w64devkit-x86\bin

set BUILD_OUT_PATH_64=%PROJECT_ROOT%\raylib-5.5\release\windows-64
if not exist %BUILD_OUT_PATH_64% mkdir %BUILD_OUT_PATH_64%

set BUILD_OUT_PATH_32=%PROJECT_ROOT%\raylib-5.5\release\windows-32
if not exist %BUILD_OUT_PATH_32% mkdir %BUILD_OUT_PATH_32%

cd %PROJECT_ROOT%\raylib-5.5\src

:: build windows-64
@echo off
set PATH=%W64DEVKIT_X64_BIN_PATH%;%PATH_DO_NOT_CHANGE%
@echo on
make -f Makefile.Windows64 clean
make -f Makefile.Windows64 RAYLIB_RELEASE_PATH=%BUILD_OUT_PATH_64%
if %ERRORLEVEL% neq 0 (
    echo Build 64 failed.
) else (
    echo Build 64 succeeded.
)

:: build windows-32
@echo off
set PATH=%W64DEVKIT_X86_BIN_PATH%;%PATH_DO_NOT_CHANGE%
@echo on
make -f Makefile.Windows32 clean
make.exe -f Makefile.Windows32 RAYLIB_RELEASE_PATH=%BUILD_OUT_PATH_32%
if %ERRORLEVEL% neq 0 (
    echo Build 32 failed.
) else (
    echo Build 32 succeeded.
)

cd %~dp0