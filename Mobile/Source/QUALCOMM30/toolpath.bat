@echo off
REM ======================================================
rem
rem Root of ARM211 and Perl
rem
REM ======================================================

set ROOT_DRIVE=c:
set APPS_DIR=apps
set APPS_PATH=%ROOT_DRIVE%\%APPS_DIR%

set ARMROOT=%APPS_PATH%\arm250
set PERLROOT=%APPS_PATH%\perl

rem
rem ARM Include and Path Variables
rem
set ARMLIB=%ARMROOT%\lib
set ARMINCLUDE=%ARMROOT%\include

REM ======================================================
rem
rem Path for ARM211 and Perl
rem
REM ======================================================

set path=%path%;%ARMROOT%\bin
set path=%path%;%PERLROOT%\bin






