@echo off
REM ======================================================
rem
rem Root of ARM250, GNUWIN32, and Perl
rem

set MAKEMODE=unix
set ROOT_DRIVE=c:
set APPS_DIR=apps
set APPS_PATH=%ROOT_DRIVE%/%APPS_DIR%
set CYGNUS=gnuwin32
set CYGREL=b18

set ARMROOT=%APPS_PATH%/arm250
set GNUROOT=%APPS_PATH%/%CYGNUS%/%CYGREL%
set PERLROOT=%APPS_PATH%/perl
set UTILROOT=%ROOT_DRIVE%/util

rem
rem ARM Include and Path Variables
rem
set ARMLIB=%ARMROOT%/lib
set ARMINCLUDE=%ARMROOT%/include
set ARMINC=%ARMROOT%/include

rem
rem Path for ARM250, GNUWIN32, Perl tools
rem
set path=%path%;%GNUROOT%/H-i386-cygwin32/bin
set path=%ARMROOT%/bin;%path%
set path=%path%;%PERLROOT%/bin
set path=%path%;%GNUROOT%/tcl/bin
set path=%path%;%UTILROOT%
set path=%path%;%UTILROOT%/gnu/bin
REM ======================================================




