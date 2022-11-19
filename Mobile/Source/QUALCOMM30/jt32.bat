@echo off

REM If T32JTG is installed in /apps
set JT32DIR=c:/apps/t32jtagpp

@start %JT32DIR%/t32marm -c %JT32DIR%/config.t32
