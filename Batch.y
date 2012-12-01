/*
 * $Header: D:\\Repository\\D\\Dev\\Psych499\\Batch.y,v 1.9 2011-03-14 16:49:58-04 alan Exp alan $
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
 * Written by:
 *      Alan Angold
 *      May 17,2010
 *
 * $Log: Batch.y,v $
 * Revision 1.9  2011-03-14 16:49:58-04  alan
 * Corrected an include file (debug.h) error.
 *
 * Revision 1.0  2011-03-13 23:17:53-04  alan
 * Initial revision
 *
 * Revision 1.8  2011-02-18 15:52:55-05  alan
 * Updated files with license information.
 *
 * Revision 1.7  2011-02-10 16:34:06-05  alan
 * Fixed so that MaxCycles on batch line actually works.
 *
 * Revision 1.6  2010-11-03 17:39:57-04  alan
 * Changed the name of the hdrstop.h file (to DRC_HdrStop.h) to distinguish it from the one
 * used for DRCTest.
 *
 * Revision 1.5  2010-08-31 17:30:48-04  alan
 * Updated for Test only of batch file.
 *
 * Revision 1.4  2010-07-20 20:35:50-04  alan
 * Added some more types for the Yacc type union and some of the rule types to help
 * with the implementation of the command passing structures in the rule actions.
 * Implemented some of the rule actions.
 *
 * Revision 1.3  2010-07-20 16:35:16-04  alan
 * Made the token names unique by prefixing with TKB_ to distinguish them from those of
 * the Generic(.l) lexor.  When you have overlapping tokens in a multiple lexor/parser system
 * you will get unexplainable parse errors.
 * Also added the string "(PARSE)" at the beginning of all the parser debug?() calls (to
 * distinguish them from the LEX entries).
 *
 * Revision 1.2  2010-07-16 23:00:37-04  alan
 * Added RCS Keywords.
 *
 * Revision 1.1  2010-06-06 00:15:27-04  alan
 * Change header files to just one hdrstop.h
 *
 * Revision 1.0  2010-05-20 01:07:15-04  alan
 * Initial revision
 *
 *
 */

/*
 * NOTES: 
 *
 */

%{

#include "DRC_HdrStop.h"

//#define DEBUG
#include "debug.h"

char* Batch_y[]={
	"$Author: alan $",
	"$Date: 2011-03-14 16:49:58-04 $",
	"$Revision: 1.9 $",
	"$RCSfile: Batch.y,v $",
	"$Source: D:\\Repository\\D\\Dev\\Psych499\\Batch.y,v $"
};

extern char linebuf[1024];

/*
 * Any global variables go here
 */
char err[80];
char buf[132];
%}


%union {
    int         itype;
    float       ftype;
    char*       stype;
    char        ctype;
    bool        btype;
    t_DRCCmd*   Mtype;
}


%token <itype>		TKB_AutoReset
%token <itype>		TKB_Command
%token <stype>		TKB_Comment
%token <itype>		TKB_Consonant
%token <itype>		TKB_Decay
%token <itype>		TKB_EndLine
%token <ftype>		TKB_Float
%token <itype>		TKB_Integer
%token <stype>		TKB_Name
%token <itype>		TKB_NoInput
%token <stype>		TKB_Off
%token <stype>		TKB_On
%token <itype>		TKB_Prime
%token <itype>		TKB_Quit
%token <itype>		TKB_Reset
%token <itype>		TKB_Then
%token <itype>		TKB_Vowel



%type <itype>		OptThenPart
%type <stype>		OptCategory
%type <itype>		OptMaxCycles
%type <itype>		PrimeableCmd
%type <itype>		OptDecayParams
%type <Mtype>		OptCmdData
%type <btype>       ARValue

%{
extern int yyerror(char *str);
extern int yylex();
extern int line_num;



%}



%%

start:	line_list
		{ 
		}

line_list:
	line
		{ 
		}
	| line_list line			
		{ 
		}

line:
	OptCmdData TKB_EndLine
		{
			// Each line is followed by an End-of-line.  Once we get here we know
            // that the command has terminated correctly.
            debug0("(PARSE) Got a command and an EOL\n");
            if($1!=NULL) ProcessDRCCmd($1);
		}

OptCmdData:
		{
			// Blank Line
			debug0("(PARSE) Blank line:\n");
            $$=NULL;
		}
	| TKB_Comment
		{
			// Comment Line
			debug1("(PARSE) Comment Line: '%s'\n",$1);
            $$=NULL;
        }
	| TKB_Command TKB_Quit
		{
			// Exit DRC immediately.
			debug0("(PARSE) Batch Command: QUIT\n");
            $$=new(t_DRCCmd);
            $$->Type=CMD_Quit;
		}
	| TKB_Command TKB_AutoReset ARValue 
		{
			// Turn ON/OFF the autoreset feature.
			debug1("(PARSE) Batch Command: AUTORESET '%s'\n",(($1==0)?"OFF":"ON"));
            $$=new(t_DRCCmd);
            $$->Type=CMD_AutoReset;
            $$->AutoReset.Value=$3;
		}
	| TKB_Command PrimeableCmd 
		{
			// One of the DECAY/NOINPUT commands.
			debug0("(PARSE) Primeable Command\n");
            $$=NULL;
		}
	| TKB_Command TKB_Prime TKB_Integer TKB_Name OptThenPart 
		{
			// Prime command with/without subcommand
			debug2("(PARSE) Batch Command: PRIME maxcycles=%d prime='%s'\n",$3,$4);
            $$=NULL;
		}
	| TKB_Command TKB_Reset
		{
			// Reset the DRC model.
			debug0("(PARSE) Batch Command: RESET\n");
            $$=new(t_DRCCmd);
            $$->Type=CMD_Reset;
		}
	| TKB_Command TKB_Name TKB_Float
		{
			// Set a parameter value
			debug2("(PARSE) Batch Command: Set Parameter Value - '%s=%f'\n",$2,$3);
            $$=new(t_DRCCmd);
            $$->Type=CMD_Param;
            $$->Param.Name=$2;
            $$->Param.Value=$3;
        }
	| OptMaxCycles TKB_Name OptCategory
		{
			// Stimulus line with only WORD
			debug3("(PARSE) Batch Command: Stimulus '%d %s %s'\n",$1,$2,$3);
            $$=new(t_DRCCmd);
            $$->Type=CMD_Data;
            $$->Data.MaxCycles=$1;
            $$->Data.TestWord=$2;
            $$->Data.Category=$3;
		}
	| error
		{
			debug0("(PARSE) Got an error token.\n");
            $$=NULL;
		}

OptThenPart:
		{
			// No then-part
		}
	| TKB_Then PrimeableCmd
		{
			// Optional part of PRIME command
			debug0("(PARSE) Batch Command: Then Part\n");
		}

ARValue:
	  TKB_On
		{
			// AutoReset value.
			debug0("(PARSE) AutoReset value ON\n");
            $$=true;
		}
	| TKB_Off
		{
			// AutoReset value.
			debug0("(PARSE) AutoReset value OFF\n");
            $$=false;
		}

OptCategory:
		{
			// No Category-part
			$$="";
		}
	| TKB_Name
		{
			// AutoReset values.
			debug1("(PARSE) Command: Category Name=%s\n",$1);
			$$=$1;
		}

OptMaxCycles:
		{
			// No MaxCycles-part (Set to what system says).
			$$=FindIParam("MaxCycles")->Value;
		}
	| TKB_Integer
		{
			// Maximum cycles for a stimulus command.
			debug1("(PARSE) Max Cycles=%d\n",$1);
			$$=$1;
		}

PrimeableCmd:
	  TKB_Decay TKB_Integer OptDecayParams
		{
			// Command to set decay-only cycles 
			// (With optional decay parameters).
			debug1("(PARSE) Batch Command: DECAY num-cycles=%d\n",$2);
		}
	| TKB_NoInput TKB_Integer OptDecayParams
		{
			// Command to turn off the input for a number of cycles.
			// (With optional decay parameters).
			debug1("(PARSE) Batch Command: NOINPUT num-cycles=%d\n",$2);
		}

OptDecayParams:
		{
			// No DecayParams-part
		}
	| TKB_Float
		{
			// Single universal decay parameter
			debug1("(PARSE) Decay Parameter: value=%f\n",$1);
		}
	| TKB_Float TKB_Float TKB_Float TKB_Float
		{
			debug4("(PARSE) Decay Parameter: values=%f %f %f %f\n",$1,$2,$3,$4);
			// Optional decay parameters. There can be:
			//    1. No decay parameters (parameters are not changed).
			//	  2. One decay value (float) meaning that all the decay parameters get
			//       set to the same value:
			//          LetterDecay
			//          OrthlexDecay
			//			PhonlexDecay
			//			PhonemeDecay
			//    3. Each of the four decay values can be set individually.
		}
%%

/*
 * End of $RCSfile: Batch.y,v $
 */
