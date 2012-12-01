//--------------------------------------------------------------------------------------
// $Header: D:\\Repository\\D\\Dev\\Psych499\\DRC.h,v 1.10 2011-03-14 16:55:29-04 alan Exp alan $
//
//    OpenDRC is an open-source implementation of the DRC Dual Route Cascaded Model 
//        of Visual Word Recognition and Reading Aloud.
//    Copyright (C) 2011  Alan Angold
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//	The author can be contacted at:
//		Alan.Angold@gmail.com
//	or
//		Alan Angold
//		549 Sandbrooke Court
//		Waterloo, Ontario 
//		Canada N2T2H4
//
//
//
// $Log: DRC.h,v $
// Revision 1.10  2011-03-14 16:55:29-04  alan
// Modified to allow compilation with GNUC.
// Fixed comment on #endif line
// Added defines for MAXLINES and MAXPATH.
//
// Revision 1.0  2011-03-13 23:17:58-04  alan
// Initial revision
//
// Revision 1.9  2011-02-18 15:52:57-05  alan
// Updated files with license information.
//
//--------------------------------------------------------------------------------------

#ifndef DRC_H
#define DRC_H

typedef double DRC_Float;

#include "DRC_CmdLine.h"

#define Epsilon (float)(1.5e-45)
#define MAX_FILE_ID       250
#define BUFFERSZ          200
#define MAXLINES      1000000

#ifdef __GNUC__
#define MAXPATH	1000
#endif


typedef unsigned char BYTE;

/* Tokens for the Generic Lexor.  */
#define TKG_Comment 260
#define TKG_Token   261
#define TKG_EOL     262

typedef enum{
    COM_None=1,
    COM_BeginOfLine=2,
    COM_EveryWhere=3
}t_CommentType;
extern t_CommentType CommentType;

typedef union GENERICSTYPE
{
    int         itype;
    float       ftype;
    char       *stype;
    char        ctype;
} GENERICSTYPE;
extern GENERICSTYPE Genericlval;


extern const char* DRC_Model_Version;





#define CMD_Data      100
#define CMD_Param     101
#define CMD_Quit      102
#define CMD_Reset     103
#define CMD_AutoReset 104

typedef struct DRC_DATA_CMD {
    int   MaxCycles;
    char* TestWord;
    char* Category;
}t_DRCData;

typedef struct DRC_PARAM_CMD {
    char* Name;
    float Value;
}t_DRCParam;

typedef struct DRC_AUTORESET_CMD {
    bool  Value;
}t_DRCAutoReset;

typedef struct DRC_COMMAND {
    int Type;
    union {
        t_DRCData      Data;
        t_DRCParam     Param;
        t_DRCAutoReset AutoReset;
    };
}t_DRCCmd;





// All the version information arrays.
extern const char* DRC_Main[];
extern const char* DRC_CmdLine[];
extern const char* DRC_FileIO[];
extern const char* DRC_Core[];
extern const char* Batch_l[];
extern const char* Batch_y[];
extern const char* Generic_l[];



extern char File_Line[MAXLINE];
extern void File_Line_Display();
extern int	File_Line_Num;

extern void Genericrestart(FILE*);
extern void Batchrestart(FILE*);

extern bool FLG_LoadDatabase;
#endif // DRC_H

//--------------------------------------------------------------------------------------
// End of $RCSfile: DRC.h,v $
//--------------------------------------------------------------------------------------

