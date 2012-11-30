//------------------------------------------------------------------------------
// $Header: D:\\Repository\\D\\Dev\\Psych499\\DRC_CmdLine.h,v 1.6 2011-03-14 16:54:52-04 alan Exp alan $
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
// $Log: DRC_CmdLine.h,v $
// Revision 1.6  2011-03-14 16:54:52-04  alan
// Modified to allow compilation with GNUC.
// Fixed comment on #endif line
//
// Revision 1.0  2011-03-13 23:17:58-04  alan
// Initial revision
//
// Revision 1.5  2011-02-18 15:52:57-05  alan
// Updated files with license information.
//
//------------------------------------------------------------------------------
#ifndef _DRC_CmdLine_h
#define _DRC_CmdLine_h
/* #pragma message in DRC_CmdLine.h */

//------------------------------------------------------------------------------
// Constants defining maximum number of command line arguments we can store and
// the longest line we can read in from a file.
//------------------------------------------------------------------------------
#define MAXARGS     250
#define MAXLINE     250

#define min(a,b)    (((a)<(b))?(a):(b))
#define max(a,b)    (((a)>(b))?(a):(b))

//------------------------------------------------------------------------------
// Parameter table structure definition and extern for
// table located in DRC_CmdLine.cpp
//------------------------------------------------------------------------------

typedef struct{
    const char*  Name;
    int    Value;
    int    Default;
    const char*  Comment;
}t_iparam;
extern t_iparam DRC_IParams[];

typedef struct{
    const char*      Name;
    DRC_Float  Value;
    DRC_Float  Default;
    const char*      Comment;
}t_fparam;
extern t_fparam DRC_FParams[];

typedef struct{
    const char*  Name;
    const char*  Value;
    const char*  Default;
    const char*  Comment;
}t_sparam;
extern t_sparam DRC_SParams[];

typedef struct{
    const char*  Name;
    bool   Value;
    bool   Default;
    const char*  Comment;
}t_bparam;
extern t_bparam DRC_BParams[];

typedef struct{
    t_iparam *IP;
    t_fparam *FP;
    t_sparam *SP;
    t_bparam *BP;
}t_DRCParams;
extern t_DRCParams DRCParams;

extern t_iparam* FindIParam(const char* Name,bool FailHard=true);
extern t_fparam* FindFParam(const char* Name,bool FailHard=true);
extern t_sparam* FindSParam(const char* Name,bool FailHard=true);
extern t_bparam* FindBParam(const char* Name,bool FailHard=true);

extern char* FindLanguageParam(const char* Name,bool FailHard=true);



#endif // _DRC_CmdLine_h


//------------------------------------------------------------------------------
// End of file $RCSfile: DRC_CmdLine.h,v $
//------------------------------------------------------------------------------
