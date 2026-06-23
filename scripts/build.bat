@echo off

setlocal

call "C:/Program Files/Microsoft Visual Studio/2022/Community/Common7/Tools/VsDevCmd.bat"

if not exist build mkdir build

if "%1"=="debug" goto DEBUG
if "%1"=="release" goto RELEASE

:DEBUG

if not exist build\debug mkdir build\debug

cl /Zi /Od /c ^
    /I incl /Fo"build\\debug\\" /Fd"build\\debug\\" ^
    main.cpp src\ImgManager.cpp src\InteractiveManager.cpp

link build\debug\main.obj build\debug\ImgManager.obj build\debug\InteractiveManager.obj ^
    User32.lib Gdi32.lib d2d1.lib ^
    /DEBUG ^
    /ILK:build\debug\CINEMA_HOLIC.ilk ^
    /OUT:build\debug\CINEMA_HOLIC.exe ^
    /SUBSYSTEM:WINDOWS

goto END

:RELEASE

if not exist build\release mkdir build\release

cl /c /I incl /Fo"build\\release\\" ^
    main.cpp src\ImgManager.cpp src\InteractiveManager.cpp

link build\release\main.obj build\release\ImgManager.obj build\release\InteractiveManager.obj ^
    User32.lib Gdi32.lib d2d1.lib ^
    /OUT:build\release\CINEMA_HOLIC.exe ^
    /SUBSYSTEM:WINDOWS

:END
endlocal