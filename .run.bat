@echo off 
setlocal

set EXENAME=%1

cd Build
start %EXENAME%
cd ..