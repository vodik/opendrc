@echo off
REM
REM Program to convert both the BISON and FLEX files to CPP files.
REM
REM Bison can be found at:
REM    http://gnuwin32.sourceforge.net/packages/bison.htm
REM
REM Flex can be found at:
REM	   http://flex.sourceforge.net/
REM
REM	The y.bat and l.bat MS Dos programs are included.
REM
@echo on
call y Batch 
call l Batch 
call l Generic
