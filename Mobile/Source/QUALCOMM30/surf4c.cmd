@echo off

if exist build4c.log del /f /q build4c.old.log && move build4c.log build4c.old.log > nul

nmake /f surf4c.mak %1 2>&1 | perl tee.pl build4c.log
