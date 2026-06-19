@echo off

:: erase all contents in buid dir
if exist build (
    rmdir /s /q build
)

:: erase all exe files in base dir
if exist *.exe (
    del *.exe
)

echo Clean completed!