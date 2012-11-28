@echo off
REM
REM Small file to automate the running of Bison and the conversion
REM of the file names (.c -> .cpp) in the C file generated. This is to 
REM allow us to compile the output file in C++.
REM
REM %1 - Filename (no extension) for *.y file to be converted.
REM
REM Bison can be found at:
REM    http://gnuwin32.sourceforge.net/packages/bison.htm
REM
REM	Sed can be found at:
REM	   http://gnuwin32.sourceforge.net/packages.html
REM
@echo on
bison -p%1 -v -d -l --debug -o%1_y.c %1.y 
sed "s/%1_y.c/%1_y.cpp/g" <%1_y.c >%1_y.cpp
del %1_y.c
