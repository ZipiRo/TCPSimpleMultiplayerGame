@echo off 
setlocal

set EXENAME=%1

cd Build
start cmd /k %EXENAME%
cd ..