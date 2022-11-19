@echo off

if exist build4p.log del /f /q build4p.old.log && move build4p.log build4p.old.log > nul

nmake /f surf4p.mak %1 2>&1 | perl tee.pl build4p.log
