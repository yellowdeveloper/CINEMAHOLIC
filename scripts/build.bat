@echo off

setlocal

call "C:/Program Files/Microsoft Visual Studio/2022/Community/Common7/Tools/VsDevCmd.bat"

if not exist build mkdir build

if "%1"=="debug" goto DEBUG
if "%1"=="release" goto RELEASE

:DEBUG

if not exist build\debug mkdir build\debug

cl /Zi /Od /c ^
    /I incl /I src/custom_scripts /Fo"build\\debug\\" /Fd"build\\debug\\" ^
    main.cpp src\*.cpp src\*.c src\custom_scripts\*.cpp src\custom_scripts\*.c

link build\debug\*.obj ^
    User32.lib Gdi32.lib d2d1.lib ^
    /DEBUG ^
    /ILK:build\debug\CINEMA_HOLIC.ilk ^
    /OUT:build\debug\CINEMA_HOLIC.exe ^
    /SUBSYSTEM:WINDOWS

goto END

:RELEASE

if not exist build\release mkdir build\release

cl /c /I incl /I src/custom_scripts /Fo"build\\release\\" ^
    main.cpp src\*.cpp src\*.c src\custom_scripts\*.cpp src\custom_scripts\*.c

link build\release\*.obj ^
    User32.lib Gdi32.lib d2d1.lib ^
    /OUT:build\release\CINEMA_HOLIC.exe ^
    /SUBSYSTEM:WINDOWS

:END
endlocal