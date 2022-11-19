@echo off

if exist build4pe.log del /f /q build4pe.old.log && move build4pe.log build4pe.old.log > nul

nmake /f surf4pe.mak %1 2>&1 | perl tee.pl build4pe.log
