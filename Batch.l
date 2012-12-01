%{
/* 
 * $Header: D:\\Repository\\D\\Dev\\Psych499\\Batch.l,v 1.12 2011-03-14 16:50:47-04 alan Exp alan $
 *
 *    OpenDRC is an open-source implementation of the DRC Dual Route Cascaded Model 
 *        of Visual Word Recognition and Reading Aloud.
 *    Copyright (C) 2011  Alan Angold
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *	The author can be contacted at:
 *		Alan.Angold@gmail.com
 *	or
 *		Alan Angold
 *		549 Sandbrooke Court
 *		Waterloo, Ontario 
 *		Canada N2T2H4
 *
 *
 * Lexical analyser to read in DRC batch files.
 *
 * By Alan Angold
 *    May 18,2010
 *
 * NOTES: 
 * 
 * $Log: Batch.l,v $
 * Revision 1.12  2011-03-14 16:50:47-04  alan
 * Corrected problem with include (debug.h)
 *
 * Revision 1.0  2011-03-13 23:17:52-04  alan
 * Initial revision
 *
 * Revision 1.11  2011-02-18 15:52:55-05  alan
 * Updated files with license information.
 *
 * Revision 1.10  2011-01-25 14:24:08-05  alan
 * Changed all the fprintf() calls to pfprintf() calls (protected against file handle being NULL)
 *
 * Revision 1.9  2010-11-03 17:40:09-04  alan
 * Changed the name of the hdrstop.h file (to DRC_HdrStop.h) to distinguish it from the one
 * used for DRCTest.
 *
 * Revision 1.8  2010-10-26 22:35:08-04  alan
 * Added a new CMDS state that takes over when we get a ! on the batch line.
 * Reverts back to DATA when the end-of-line is reached.
 * Changed tokens to be only matchable in CMDS state.
 * Changed the Name field to accept names down to one character long (previously 2).
 *
 * Revision 1.7  2010-08-31 17:29:07-04  alan
 * Changed the max errors parameter to get its value from the general parameters table.
 *
 * Revision 1.6  2010-07-20 20:43:56-04  alan
 * When passing back strings they need to be strdup()'ed first so that multiple calls will not
 * overrun the yytext buffer.
 * Moved the MaxErrors out of this module so that it could be taken care of by the
 * parameter setting routines of the DRC_CmdLine package.
 *
 * Revision 1.5  2010-07-20 16:34:01-04  alan
 * Got rid of some pointless debug?() calls.
 * Made the token names unique by prefixing with TKB_ to distinguish them from those of
 * the Generic(.l) lexor.  When you have overlapping tokens in a multiple lexor/parser system
 * you will get unexplainable parse errors.
 *
 * Revision 1.4  2010-07-16 22:59:48-04  alan
 * Added RCS Keywords.
 *
 * Revision 1.3  2010-06-06 00:14:56-04  alan
 * Change header files to just one hdrstop.h
 *
 * Revision 1.2  2010-05-20 16:20:10-04  alan
 * Remove line buffering from this file so that it can be dealt with from DRC_FileIO.cpp.
 *
 * Revision 1.1  2010-05-20 02:22:25-04  alan
 * Converted the names of the local variables to start with "Batch".
 * Cleaned out yyerror() routine.
 *
 * Revision 1.0  2010-05-20 01:07:15-04  alan
 * Initial revision
 *
 *
 */

#include "DRC_HdrStop.h"

//#define DEBUG
#include "debug.h"

char* Batch_l[]={
	"$Author: alan $",
	"$Date: 2011-03-14 16:50:47-04 $",
	"$Revision: 1.12 $",
	"$RCSfile: Batch.l,v $",
	"$Source: D:\\Repository\\D\\Dev\\Psych499\\Batch.l,v $"
};

#define yylval	Batchlval
#define yyerror Batcherror
#define yywrap  Batchwrap



int     Batch_Column_Num=0;
%}

d		[0-9]

%s DATA CMDS



%%

%{
	if(File_Line_Num==0){
		File_Line_Num++;
		File_Line_Display();
	}
    BEGIN DATA; /* Start in DATA input state */
%}

\n.*				{
						File_Line_Num++;
						Batch_Column_Num=0; 
						strcpy(File_Line,yytext+1);
						File_Line[strlen(yytext)-1]=0;
						File_Line_Display();
						yyless(1);
						debug0("(LEX)TKB_EndLine\n");
                        BEGIN DATA;
						return(TKB_EndLine);
					}
#[^\n]* 			{
						debug1("(LEX)TKB_Comment=%s\n",yytext);
						Batch_Column_Num=1;
                        yylval.stype=yytext;
                        BEGIN DATA;
						return(TKB_Comment);
                    }
!					{
						debug0("(LEX)TKB_Command\n");
						Batch_Column_Num+=yyleng;
						debug1("(LEX) Column=%d\n",Batch_Column_Num);
                        BEGIN CMDS;
						return(TKB_Command);
					}
[ \t]+				{   // Ignore whitespace
						debug0("(LEX)WhiteSpace\n");
						Batch_Column_Num+=yyleng;
						debug1("(LEX) Column=%d\n",Batch_Column_Num);
					}; 
{d}*\.{d}+			{
						debug1("(LEX)TKB_Float=%s\n",yytext);
						Batch_Column_Num+=yyleng;
                        yylval.ftype=atof(yytext);
						debug1("(LEX) Column=%d\n",Batch_Column_Num);
						return(TKB_Float);
					}
{d}+			    {
						debug1("(LEX)TKB_Integer=%s\n",yytext);
						Batch_Column_Num+=yyleng;
                        yylval.itype=atoi(yytext);
						debug1("(LEX) Column=%d\n",Batch_Column_Num);
						return(TKB_Integer);
					}
\\[Vv]				{
						Batch_Column_Num+=yyleng;
						debug0("(LEX)TKB_Vowel\n");
						return(TKB_Vowel);
					}
\\[Cc]				{
						Batch_Column_Num+=yyleng;
						debug0("(LEX)TKB_Consonant\n");
						return(TKB_Consonant);
					}
						
<CMDS>"quit"				{
						debug1("(LEX)TKB_Quit='%s'\n",yytext);
						Batch_Column_Num+=yyleng;
						return(TKB_Quit);
					}
<CMDS>"autoreset"			{
						debug1("(LEX)TKB_AutoReset='%s'\n",yytext);
						Batch_Column_Num+=yyleng;
						return(TKB_AutoReset);
					}
<CMDS>"reset"				{
						debug1("(LEX)TKB_Reset='%s'\n",yytext);
						Batch_Column_Num+=yyleng;
						return(TKB_Reset);
					}
<CMDS>"decay"				{
						debug1("(LEX)TKB_Decay='%s'\n",yytext);
						Batch_Column_Num+=yyleng;
						debug1("(LEX) Column=%d\n",Batch_Column_Num);
						return(TKB_Decay);
					}
<CMDS>"noinput"			{
						debug1("(LEX)TKB_NoInput='%s'\n",yytext);
						Batch_Column_Num+=yyleng;
						return(TKB_NoInput);
					}
<CMDS>"prime"				{
						debug1("(LEX)TKB_Prime='%s'\n",yytext);
						Batch_Column_Num+=yyleng;
						return(TKB_Prime);
					}
<CMDS>"on"				{
						debug1("(LEX)TKB_On='%s'\n",yytext);
						Batch_Column_Num+=yyleng;
						return(TKB_On);
					}
<CMDS>"off"				{
						debug1("(LEX)TKB_Off='%s'\n",yytext);
						Batch_Column_Num+=yyleng;
						return(TKB_Off);
					}
<CMDS>"then"				{
						debug1("(LEX)TKB_Then='%s'\n",yytext);
						Batch_Column_Num+=yyleng;
						return(TKB_Then);
					}
[a-z][a-z0-9_]*		{	// This has to appear at the end to avoid capturing keywords.
						Batch_Column_Num+=yyleng;
						debug1("(LEX)TKB_Name=%s\n",yytext);
                        yylval.stype=strdup(yytext);
						return(TKB_Name);
					}
.					{
						debug3("(LEX)NOTYPE<INITIAL>(.)=(%d)0x%02d'%c'\n",yyleng,yytext[0],yytext[0]);
						Batch_Column_Num+=yyleng;
						return(yytext[0]); 
					}
%%

char BatchParseErrMsg[]={"ERROR: (%s) at line=%d col=%d (token='%s').\n"};

int BatchSyntaxErrs=0;

int yyerror(char *str){
	int rtn=0;

	pfprintf(stdout,"%*s\n",Batch_Column_Num-yyleng+6,"|");
	pfprintf(stdout,BatchParseErrMsg,str,File_Line_Num,Batch_Column_Num-yyleng+1,yytext);

	if(++BatchSyntaxErrs>=FindIParam("MaxErrors")->Value) {
		pfprintf(stdout,"ERROR: Maximum syntax errors have been reached!\n");
		exit(1);
	}
	return rtn;
}

int yywrap(void){
	return 1;
}

/*
 * End of $RCSfile: Batch.l,v $
 */
