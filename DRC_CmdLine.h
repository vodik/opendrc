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
//  The author can be contacted at:
//      Alan.Angold@gmail.com
//  or
//      Alan Angold
//      549 Sandbrooke Court
//      Waterloo, Ontario
//      Canada N2T2H4
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

t_iparam* FindIParam(const char* Name,bool FailHard=true);
t_fparam* FindFParam(const char* Name,bool FailHard=true);
t_sparam* FindSParam(const char* Name,bool FailHard=true);
t_bparam* FindBParam(const char* Name,bool FailHard=true);

char* FindLanguageParam(const char* Name,bool FailHard=true);

/* XXX: From precompiled header */
void* FindParam(char* Name);
void ProcessCmdLineArgs(int argc,char **argv);
void WriteRTFileHeader(FILE* fh,bool batch,const char* word);
bool MatchArg(const char *flag,const char *arg);
void TestAndCall(int &idx,int argc,const char **argv,int (*func)(const char **argv));
void ErrCmdSwt(char *str);
int Set_Language(const char** arglist);
int Set_VisualFeatureLetterUnits(const char** arglist);
int Set_PhonemeUnits(const char** arglist);
int Set_SetParameter(const char** arglist);
int Set_ReadParameterFile(const char** arglist);
int Set_SeparateHomophoneFlag(const char** arglist);
int Set_ContinueToMaxCycles(const char** arglist);
int Set_ProcessBatchFile(const char** arglist);
int Set_TestBatchOnly(const char** arglist);
int Set_BatchProcessLimits(const char** arglist);
int Set_MaxIncorrectResults(const char** arglist);
int Set_RepeatSimulation(const char** arglist);
int Set_ActivationFileExt(const char** arglist);
int Set_SaveActivationLvls(const char** arglist);
int Set_ReportExtraInfo(const char** arglist);
int Set_OutputFlags(const char** arglist);
int Set_ReportActGreater(const char** arglist);
int Set_NoOutputFiles(const char** arglist);
int Set_RTFileExtn(const char** arglist);
int Set_OutputDirectory(const char** arglist);
int Set_ReportSimDuration(const char** arglist);
int Set_Verbalize(const char** arglist);
int ListIrregularWords(const char** arglist);
int ListIrregularStressWords(const char** arglist);
int ApplyGPCRules(const char** arglist);
int ListRegularWords(const char** arglist);
int ApplyGPCStressRules(const char** arglist);
int ListWhammies(const char** arglist);
int DisplayHelp(const char** arglist);
int DisplayVersion(const char** arglist);
int ReadCmdLineOptionsFile(const char** arglist);
int Cmd_DspNeighbourhood(const char** arglist);
int Cmd_DspHomographs(const char** arglist);
int Cmd_DspHomophones(const char** arglist);

#endif // _DRC_CmdLine_h


//------------------------------------------------------------------------------
// End of file $RCSfile: DRC_CmdLine.h,v $
//------------------------------------------------------------------------------
