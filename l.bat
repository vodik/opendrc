@echo off
REM
REM Program to convert a given FLEX file into a CPP file.  This
REM program will also do some fixups to the file to convert the
REM yy prefix so that we can use multiple lexors.  It will also
REM fix a isatty warning that is a nuisance.  This compiles with
REM the old LEX compatibility, otherwise the yyback() call fails
REM in a non-obvious way.
REM
REM Flex can be found at:
REM	   http://flex.sourceforge.net/
REM
REM	Sed can be found at:
REM	   http://gnuwin32.sourceforge.net/packages.html
REM
set TMP=c:\tmp\
@echo on
flex -P%1 -i -l -L -o%1_l.c %1.l
sed "s/%1_l.c/%1_l.cpp/g" <%1_l.c >%TMP%a
sed "s/yytext_ptr/%1_text_ptr/g" <%TMP%a >%TMP%b
sed "s/isatty(/std::isatty(/g" <%TMP%b >%1_l.cpp
del %1_l.c %TMP%a
