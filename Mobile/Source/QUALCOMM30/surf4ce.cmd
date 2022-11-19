@echo off

if exist build4ce.log del /f /q build4ce.old.log && move build4ce.log build4ce.old.log > nul

nmake /f surf4ce.mak %1 2>&1 | perl tee.pl build4ce.log
