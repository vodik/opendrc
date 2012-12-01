%{
/* 
 * $Header: D:\\Repository\\D\\Dev\\Psych499\\Generic.l,v 1.10 2011-03-14 16:50:31-04 alan Exp alan $
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
 *    along with this program.  If not, see <http: *www.gnu.org/licenses/>.
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
 * Generic Lexical analyser to read in DRC language files.
 *
 * By Alan Angold
 *    May 18,2010
 *
 * NOTES: 
 * 
 * $Log: Generic.l,v $
 * Revision 1.10  2011-03-14 16:50:31-04  alan
 * Corrected problem with include (debug.h)
 *
 * Revision 1.0  2011-03-13 23:17:53-04  alan
 * Initial revision
 *
 * Revision 1.9  2011-02-18 15:52:55-05  alan
 * Updated files with license information.
 *
 * Revision 1.8  2011-01-25 14:24:25-05  alan
 * Changed all the fprintf() calls to pfprintf() calls (protected against file handle being NULL)
 *
 * Revision 1.7  2010-11-07 23:15:32-05  alan
 * Added lexing of comments strings starting with PC(%) symbol instead of "#" which interfers
 * with the phoneme "#".
 *
 * Revision 1.6  2010-11-03 17:40:20-04  alan
 * Changed the name of the hdrstop.h file (to DRC_HdrStop.h) to distinguish it from the one
 * used for DRCTest.
 *
 * Revision 1.5  2010-07-20 16:36:05-04  alan
 * Made the token names unique by prefixing with TKG_ to distinguish them from those of
 * the Batch(.l) lexor.  When you have overlapping tokens in a multiple lexor/parser system
 * you will get unexplainable parse errors.
 *
 * Revision 1.4  2010-07-16 23:01:11-04  alan
 * Added RCS Keywords.
 *
 * Revision 1.3  2010-06-06 00:15:08-04  alan
 * Change header files to just one hdrstop.h
 *
 * Revision 1.2  2010-05-20 16:21:34-04  alan
 * Remove file line buffer to DRC_FileIO.cpp to make consistent with Batch.l.
 * Also added switch code for Comments so that the same Generic.l lexor can
 * be used with multiple file types.
 *
 * Revision 1.1  2010-05-20 02:28:53-04  alan
 * Completely revised this from the "Batch" file version.
 *
 * Revision 1.0  2010-05-20 01:07:03-04  alan
 * Initial revision
 *
 *
 */


#include "DRC_HdrStop.h"

//#define DEBUG
#include "debug.h"

char* Generic_l[]={
	"$Author: alan $",
	"$Date: 2011-03-14 16:50:31-04 $",
	"$Revision: 1.10 $",
	"$RCSfile: Generic.l,v $",
	"$Source: D:\\Repository\\D\\Dev\\Psych499\\Generic.l,v $"
};

#define yylval	Genericlval
#define yyerror Genericerror
#define yywrap  Genericwrap

int     Generic_Column_Num=0;

GENERICSTYPE Genericlval;


%}
%x  NOCOM

d	[0-9]

%%

%{
	if(File_Line_Num==0){
		File_Line_Num++;
		debug1("\n\nCapturing line %d! a\n",File_Line_Num);
		File_Line_Display();
	}
%}

\n.*				{
						File_Line_Num++;
						Generic_Column_Num=0; 
						debug1("\n\nCapturing line %d!\n",File_Line_Num);
						strcpy(File_Line,yytext+1);
						File_Line[strlen(yytext)-1]=0;
						File_Line_Display();
						yyless(1);
						debug0("(LEX)TKG_EOL\n");
						return(TKG_EOL);
					}
[ \t]*#[^\n]* 		{
						debug1("(LEX)TKG_Comment=%s\n",yytext);
						debug1("     Column=%d\n",Generic_Column_Num);
						if(CommentType==COM_EveryWhere){
							// Comments can occur anywhere on the line.
							Generic_Column_Num+=yyleng;
							yylval.stype=yytext;
							return(TKG_Comment);
						}else if((CommentType==COM_BeginOfLine)&&(Generic_Column_Num==0)){
							// Comments have to be the only thing on the line, but they
							// may have leading spaces.
							Generic_Column_Num+=yyleng;
							yylval.stype=yytext;
							return(TKG_Comment);
						}else{
							// Comments are not allowed at all.
							// Supposed comment didn't start a column 0 so must be
							// a valid field.  So push back everything and let some
							// other rule capture this.
							yyless(0);
							BEGIN(NOCOM);
						}
                    }
[ \t]*%[^\n]* 		{
						debug1("(LEX)TKG_Comment=%s\n",yytext);
						debug1("     Column=%d\n",Generic_Column_Num);
						// PC (%) Comments can occur anywhere on the line in any file.
						Generic_Column_Num+=yyleng;
						yylval.stype=yytext;
						return(TKG_Comment);
                    }
[ \t]+				{   // Ignore whitespace
						debug0("(LEX)WhiteSpace\n");
						Generic_Column_Num+=yyleng;
						debug1("(LEX) Column=%d\n",Generic_Column_Num);
					}; 
<NOCOM>[ \t]+		{   // Ignore whitespace after we found non(^#)-comment
						debug0("(LEX)WhiteSpace\n");
						Generic_Column_Num+=yyleng;
						debug1("(LEX) Column=%d\n",Generic_Column_Num);
					}; 
[^\n\t ]+			{	// Get a non-whitespace token.
						debug1("(LEX)TKG_Token=%s\n",yytext);
						Generic_Column_Num+=yyleng;
                        yylval.stype=yytext;
						debug1("(LEX) Column=%d\n",Generic_Column_Num);
						return(TKG_Token);
					}
<NOCOM>[^\n\t ]+	{	// Get a non-whitespace token after we found non(^#)-comment.
						debug1("(LEX)TKG_Token=%s\n",yytext);
						Generic_Column_Num+=yyleng;
                        yylval.stype=yytext
						debug1("(LEX) Column=%d\n",Generic_Column_Num);
						// Now that we've eaten the problem '#' we can return to normal mode.
						BEGIN(0);
						return(TKG_Token);
					}
%%

char GenericParseErrMsg[]={"ERROR: (%s) at line=%d col=%d (token='%s').\n"};

#define MAXERR 5
int GenericSyntaxErrs=0;



int yyerror(char *str){
	int rtn=1;

	pfprintf(stdout,"%*s\n",Generic_Column_Num-yyleng+6,"|");
	pfprintf(stdout,GenericParseErrMsg,str,File_Line_Num,Generic_Column_Num-yyleng+1,yytext);

	if(++GenericSyntaxErrs>MAXERR) {
		pfprintf(stdout,"ERROR: Maximum syntax errors have been reached!\n");
		exit(1);
	}
	return rtn;
}

int yywrap(void){
	return 1;
}


/*
 * End of $RCSfile: Generic.l,v $
 */
