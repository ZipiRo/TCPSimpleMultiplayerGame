@echo off
setlocal

set CODEFILE=%1
set EXENAME=%2
set DEPENDENCIES="%3"

g++ --version
g++ %CODEFILE% -o Build/%EXENAME% %DEPENDENCIES%