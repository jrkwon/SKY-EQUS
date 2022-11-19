@echo off

REM If T32 is installed in /apps
set T32DIR=c:/apps/t32

REM If T32 is installed in /apps/arm211a
REM set T32DIR=c:/apps/arm211a/t32

@start %T32DIR%/t32w95 -c %T32DIR%/config.t32
