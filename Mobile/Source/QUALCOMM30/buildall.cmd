@echo off

if exist build4c.log del /f /q build4c.old.log && move build4c.log build4c.old.log > nul

nmake /f surf4c.mak  2>&1 | perl tee.pl build4c.log

if exist build4ce.log del /f /q build4ce.old.log && move build4ce.log build4ce.old.log > nul

nmake /f surf4ce.mak  2>&1 | perl tee.pl build4ce.log

if exist build4p.log del /f /q build4p.old.log && move build4p.log build4p.old.log > nul

nmake /f surf4p.mak  2>&1 | perl tee.pl build4p.log

if exist build4pe.log del /f /q build4pe.old.log && move build4pe.log build4pe.old.log > nul

nmake /f surf4pe.mak  2>&1 | perl tee.pl build4pe.log





