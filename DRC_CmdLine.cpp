//------------------------------------------------------------------------------
// $Header: D:\\Repository\\D\\Dev\\Psych499\\DRC_CmdLine.cpp,v 1.22 2011-03-14 23:56:25-04 alan Exp alan $
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
// Routines that take care of Command Line parameter parsing.
//
// $Log: DRC_CmdLine.cpp,v $
// Revision 1.22  2011-03-14 23:56:25-04  alan
// Fixed bug in Set_SetParameters() that didn't display the change in Boolean type properly.
//
// Revision 1.21  2011-03-14 16:57:28-04  alan
// Modified to allow compilation with GNUC.
// Fixed comment on #endif line
// Fixed format for debug.h include.
// Converted MAXINT to MAXLINES (more descriptive)
// Fixed stop line for CmdLineArgs[].
//
// Revision 1.0  2011-03-13 23:18:00-04  alan
// Initial revision
//
// Revision 1.20  2011-02-18 15:52:59-05  alan
// Updated files with license information.
//
// Revision 1.19  2011-02-10 16:30:17-05  alan
// Marking in help message which parameters work and which don't.
// Adding comments.
// Changing error messages to print OpenDRC rather than DRC.
//
// Revision 1.18  2011-01-25 14:25:37-05  alan
// Changed all the fprintf() calls to pfprintf() calls (protected against file handle being NULL)
//
// Revision 1.17  2011-01-16 20:36:08-05  alan
// Added new command line command "--neighbourhood" that will display the neighbourhood
// of the given nonwords (batch or command line).
//
// Revision 1.16  2011-01-12 22:44:49-05  alan
// Added new flag to parameters table.
//
// Revision 1.15  2010-12-05 22:27:58-05  alan
// Converting memset()'s to use a better protected size parameter.
//
// Revision 1.14  2010-11-24 10:38:27-05  alan
// Corrected a title of program.
//
// Revision 1.13  2010-11-22 17:27:34-05  alan
// Corrected a parameter comment.
//
// Revision 1.12  2010-11-03 17:42:44-04  alan
// Changed the name of the hdrstop.h file (to DRC_HdrStop.h) to distinguish it from the one
// used for DRCTest.
//
// Revision 1.11  2010-10-28 17:15:52-04  alan
// Changed SeparateHomographemes back to false (as testing today indicated).
//
// Revision 1.10  2010-10-26 22:32:22-04  alan
// Change the number of MAXERR from 5 to 10 because I ran into this 1/2 way through
// processing on "bigbatch" file.
// DRC1.2 seems to be using separate entries for homographs so I changed the default
// for SeparateHmographemes parameter to TRUE.
// Added Debug and Warnings parameters.
// Removed output file name creation and open with OpenOutputFile() call that deals with
// stupid Windows flaws like not being able to create files like con.txt
// Added code to display the DRC header and statistics on a batch run.
//
// Revision 1.9  2010-10-25 11:25:56-04  alan
// Added the CFSPOLEqualOIL parameter to trigger making POL.CFS=OIL.CFS.
// Added code to create output directory and RT and STATS files.
//
// Revision 1.8  2010-10-20 12:29:04-04  alan
// Added the SeparateHomographemes parameter to trigger (or not) Homographeme
// separation and changed the values for this and SeparateHomophonemes to false
// as a default.
// Added two switches (--homophonemes and --homographemes) and corresponding
// command list, help message and routines to implement dumping these types of words.
//
// Revision 1.7  2010-10-20 09:54:18-04  alan
// Re-added the PhonemeUnsupportedDecay parameter for compatibility with the DRC1.2 parameter file.  However this parameter is not implemented.
// Changed the PhonemeDecay parameter value 0.050 - >0.000.  This incorrect value was
// created because of a sloppy edit from the PhonemeUnsupportedDecay parameter.
//
// Revision 1.6  2010-10-20 09:40:01-04  alan
// Changed the order of the parameter entries in their respective tables and added a
// comment field to describe the parameters use.
// Removed parameter PhonemeUnsupportedDecay -- didn't know what it is or where I found it originally.
// Changed the OutputDirectory parameter to use a buffer so that the user could change it.
// Added 41 new flags to control verbose output to the ACTS file (initial debugging dumps).
// Made FailHard sections of Find?Param() routines only print an error message if FailHard
// is true.
// Added FindLanguageParam() to check for the language parameters.  Returns an actual
// char* to the value string instead of a structure pointer.
// Added flag -f to toggle the output of debug information.
// Converted the CmdLineArgs[] array to change the Level parameter to NeedsDB.
// Change ProcessCmdLineArgs() routine to deal with above and to require whitespace separated parameters: Loads DB if needed.
// Change command flag check to use entire flag string (so substring flags don't match).
// Added Set_OutputFlag()
//
// Revision 1.5  2010-08-31 17:27:09-04  alan
// Just made a inconsequential change so additional comments could be made for the last
// revision:
// Modifed the ReadCmdLineOptionsFile() callback to read through the
// parameter file and build an argv[] array structure that could be
// passed to the command line processing routines.
//
// Revision 1.4  2010-08-31 17:14:22-04  alan
// Replaced all the model parameters with tables of parameters and routines to find the
// parameters in each of those files: Find{FBIS}Param() and a general routine FindParam()
// that searches for a general parameter.
// Corrected the help file for the "-T" command (Test batch file without executing).
// Updated the command routine processing array (t_CmdLineArgs CmdLineArgs[] to
// reflect new format.
// Rewrote routine ProcessCmdLineArgs() to reflect new data structure format.
// Fixed TestAndCall() routine to update the argument pointer correctly based on the
// return value from each command processing routine callback.
// Changed signature of callbacks to return an int (number of arguments used).
// Added calls in the command line processing callbacks to set their respective parameters.
// Added return() statement in all the callbacks to return the number of parameters used.
// Added code to set each of the four parameter types in the "set parameter" callback.
//
// Revision 1.3  2010-07-20 20:38:42-04  alan
// Naming changes.
// Added a -t parameter to test a batch file without executing it.
// Fixed a bug in TestAndCall() routine that passed off the wrong argument list.
// Fixed up the Max Batch Errors parameter setting routine.
// Added code to print the module versions.
//
// Revision 1.2  2010-07-16 22:45:49-04  alan
// Added RCS Keywords.
// Cleaned up formating of help message.
//
// Revision 1.1  2010-06-06 00:07:50-04  alan
// Change header files to use only hdrstop.h
//
// Revision 1.0  2010-05-20 01:07:18-04  alan
// Initial revision
//
//------------------------------------------------------------------------------

#include "DRC_HdrStop.h"

//#define DEBUG
#include "debug.h"

//------------------------------------------------------------------------------
// Defines and Global variables for this system.
//------------------------------------------------------------------------------

#define MAXERR 10

static FILE* fh_BatchFile=stdin;

static void* FindParam(char* Name);
static bool MatchArg(const char *flag,const char *arg);
static void TestAndCall(int &idx,int argc,char **argv,int (*func)(char **argv));
static void ErrCmdSwt(char *str);

static int Set_Language(char **arglist);
static int Set_VisualFeatureLetterUnits(char **arglist);
static int Set_PhonemeUnits(char **arglist);
static int Set_SetParameter(char **arglist);
static int Set_ReadParameterFile(char **arglist);
static int Set_SeparateHomophoneFlag(char **arglist);
static int Set_ContinueToMaxCycles(char **arglist);
static int Set_ProcessBatchFile(char **arglist);
static int Set_TestBatchOnly(char **arglist);
static int Set_BatchProcessLimits(char **arglist);
static int Set_MaxIncorrectResults(char **arglist);
static int Set_RepeatSimulation(char **arglist);
static int Set_ActivationFileExt(char **arglist);
static int Set_SaveActivationLvls(char **arglist);
static int Set_ReportExtraInfo(char **arglist);
static int Set_OutputFlags(char **arglist);
static int Set_ReportActGreater(char **arglist);
static int Set_NoOutputFiles(char **arglist);
static int Set_RTFileExtn(char **arglist);
static int Set_OutputDirectory(char **arglist);
static int Set_ReportSimDuration(char **arglist);
static int Set_Verbalize(char **arglist);
static int ListIrregularWords(char **arglist);
static int ListIrregularStressWords(char **arglist);
static int ApplyGPCRules(char **arglist);
static int ListRegularWords(char **arglist);
static int ApplyGPCStressRules(char **arglist);
static int ListWhammies(char **arglist);
static int DisplayHelp(char **arglist);
static int DisplayVersion(char **arglist);
static int ReadCmdLineOptionsFile(char **arglist);
static int Cmd_DspNeighbourhood(char **arglist);
static int Cmd_DspHomographs(char **arglist);
static int Cmd_DspHomophones(char **arglist);

//------------------------------------------------------------------------------
// Default DRC Parameters
//------------------------------------------------------------------------------
static t_iparam DRC_IParams[]={
    // GPC Route Parameters
    {"GPCOnset",                      26,      26, "Cycles before first letter available to GPCRoute"},
    // Other parameters.
    {"SimNumSteps",                    1,       1, "SimNumSteps"},
    {"MaxErrors",                 MAXERR,  MAXERR, "Maximum number of batch file errors before failure"},
    {"BatchFileStart",                 0,       0, "Start line in batch file"},
    {"BatchFileLines",          MAXLINES,MAXLINES, "Number of batch file lines to execute"},
    {"MaxCycles",                   1000,    1000, "Maximum number of cycles for simulation"},
    {"PhonemeUnits",                   8,       8, "Width in characters of output DRC Phoneme Buffer"},
    {"VisualFeatureLetterUnits",       8,       8, "Width in characters of input DRC Feature Layer"},


    {NULL,0,0,NULL}
};

t_fparam DRC_FParams[]={
    // General Parameters
    {"ActivationRate",              0.200, 0.200, "Scaling of input excitation/inhibition/CFS values (Epsilon)"},
    {"FrequencyScale",              0.050, 0.050, "Scaling for written and spoken CFS values"},
    {"MinReadingPhonology",         0.400, 0.400, "Word recognition threshold value"},

    // Feature Level Parameters
    {"FeatureLetterExcitation",     0.005, 0.005, "FL->LL Excitation parameter"},
    {"FeatureLetterInhibition",     0.150, 0.150, "FL->LL Inhibition parameter"},
    {"FeatureNoise",                0.000, 0.000, "FL Noise (not implemented)"},
    {"FeatureDecay",                0.000, 0.000, "FL Decay (Theta)"},

    // Letter Level Parameters
    {"LetterOrthlexExcitation",     0.070, 0.070, "LL->OIL Excitation parameter"},
    {"LetterOrthlexInhibition",     0.480, 0.480, "LL->OIL Inhibition parameter"},
    {"LetterLateralInhibition",     0.000, 0.000, "LL->LL Inhibition parameter"},
    {"LetterNoise",                 0.000, 0.000, "LL Noise (not implemented)"},
    {"LetterDecay",                 0.000, 0.000, "LL Decay (Theta)"},

    // Orthographic Lexicon (Orthlex) Parameters
    {"OrthlexPhonlexExcitation",    0.250, 0.250, "OIL->POL Excitation parameter"},
    {"OrthlexPhonlexInhibition",    0.000, 0.000, "OIL->POL Inhibition parameter"},
    {"OrthlexLetterExcitation",     0.300, 0.300, "OIL->LL Excitation parameter"},
    {"OrthlexLetterInhibition",     0.000, 0.000, "OIL->LL Inhibition parameter"},
    {"OrthlexLateralInhibition",    0.060, 0.060, "OIL->OIL Inhibition parameter"},
    {"OILNoise",                    0.000, 0.000, "OIL Noise (not implemented)"},
    {"OILDecay",                    0.000, 0.000, "OIL Decay (Theta)"},
    {"OILCFSInhibition",            1.000, 1.000, "OIL Specific freqency scaling"},   // Modulate the input of the CFS into POL calc

    // Phonological Lexicon (Phonlex) Parameters
    {"PhonlexPhonemeExcitation",    0.090, 0.090, "POL->PB Excitation parameter"},
    {"PhonlexPhonemeInhibition",    0.000, 0.000, "POL->PB Inhibition parameter"},
    {"PhonlexOrthlexExcitation",    0.250, 0.250, "POL->OIL Excitation parameter"},
    {"PhonlexOrthlexInhibition",    0.000, 0.000, "POL->OIL Inhibition parameter"},
    {"PhonlexLateralInhibition",    0.070, 0.070, "POL->POL Inhibition parameter"},
    {"POLNoise",                    0.000, 0.000, "POL Noise (not implemented)"},
    {"POLDecay",                    0.000, 0.000, "POL Decay (Theta)"},
    {"POLCFSInhibition",            1.000, 1.000, "POL Specific freqency scaling"},   // Modulate the input of the CFS into POL calc

    // Phoneme Level Parameters
    {"PhonemePhonlexExcitation",    0.040, 0.040, "PB->POL Excitation parameter"},
    {"PhonemePhonlexInhibition",    0.160, 0.160, "PB->POL Inhibition parameter"},
    {"PhonemeLateralInhibition",    0.147, 0.147, "PB->PB Inhibition parameter"},
    {"PhonemeUnsupportedDecay",     0.050, 0.050, "PB activation decay (after activation determined)"},
    {"PhonemeNoise",                0.000, 0.000, "PB Noise (not implemented)"},
    {"PhonemeDecay",                0.000, 0.000, "PB Decay (Theta)"},

    // GPC Route Parameters
    {"GPCPhonemeExcitation",        0.051, 0.051, "GPC" },
    {"GPCCriticalPhonology",        0.050, 0.050, "GPC" },

    {"ReportActGreater",            0.02,  0.02,  "Display threshold for reporting activations"},
    {"SimStartVal",                 0.0,   0.0,   "Simulation start-range value"},
    {"SimEndVal",                   1.0,   1.0,   "Simulation end-range value"},

    {NULL,0.0,0.0,NULL}
};

static t_sparam DRC_SParams[]={
    {"LanguageDirectory",        "english1.1.6","english1.1.6", ""},
    {"OutputDirectory",          OutParentDirBuf,".",           ""},
    {"OutputRTFileExt",          "rt","rt",                     ""},
    {"OutputActivationFileExt",  "acts","acts",                 ""},
    {"CmdFile",                  "","",                         ""},  // For use with the "-o <file>" empty file name means no file.
    {"SimParam",                 NULL,NULL,                     ""},
    {"FileNameBatchFile",        NULL,NULL,                     ""},
    {"TestWord",                 NULL,NULL,                     ""},

    {NULL,NULL,NULL,NULL}
};

static t_bparam DRC_BParams[]={
    {"AutoReset",                 true,  true,  ""},
    {"ApplyGPCRules",             false, false, ""},
    {"ApplyGPCStressRules",       false, false, ""},
    {"ListIrregularStressWords",  false, false, ""},
    {"ListIrregularWords",        false, false, ""},
    {"ListRegularWords",          false, false, ""},
    {"ListWhammies",              false, false, ""},
    {"NoOutputFiles",             false, false, ""},
    {"ReportExtraInfo",           false, false, ""},
    {"ReportSimDuration",         false, false, ""},
    {"RunForMaxCycles",           false, false, ""},
    {"SaveActivationLvls",        false, false, ""},
    {"SeparateHomographemes",     false, false, ""},
    {"SeparateHomophones",        false, false, ""},
    {"TestBatchOnly",             false, false, ""},
    {"Verbalize",                 false, false, ""},
    {"Neighbourhood",             false, false, ""},   // Display the neighbourhood of a nonword and size.

    // Flag(s) to turn on DRC1.2 bug effects.
    {"DRC12UnSupDecayTrigErr",    false, false, "Turn on compensation for the DRC1.2 Unsupported Decay Trigger Error"},

    // This flag implements the CFS(POL)=CFS(OIL)=CFS(Written) as indicated in
    // Coltheart et. al. (2001) p216 col_1 para_2 "Activation of a unit...".  This
    // document is incorrect: CFS(POL)=CFS(Spoken Word Freq).  This last equality
    // was tested and shown to be true in DRC1.2.
    {"CFSPOLEqualOIL",            false, false, "Set the POL CFSi value to the corresponding OIL CFSi value"},

    // The following flags control the output information.
    {"Debug",                     false, false, "Sets debug mode for display of extra information"},
    {"Warnings",                  false, false, "Display warnings."},
    {"Extended",                  false, false, "Extended format on output"},

    // Output flags for the Feature Level (FL)
    {"FLDRC",                     true,  true , ""},  // Standard DRC feature level output (VF0-7 values)
    {"FLDRCE",                    false, false, ""},  // Standard DRC feature level output (float output i.e. "1"->"1.0 "
    {"FLDRCT",                    false, false, ""},  // Totals both per letter and per feature (TF value).
    {"FLTimeT",                   false, false, ""},  // Display the previous time values
    {"FLTimeTp1",                 false, false, ""},  // Display the newly ulated values.

    // Output flags for the Letter Level (LL)
    {"LLDRC",                     true,  true , ""},  // Display the DRC Letter Level activations (L0-7 values).
    {"LLDRCT",                    true,  true , ""},  // Display the DRC Totals for the LL (TL line).
    {"LLFromFL",                  false, false, ""},
    {"LLFromLL",                  false, false, ""},
    {"LLFromOIL",                 false, false, ""},
    {"LLTimeT",                   false, false, ""},
    {"LLTimeTp1",                 false, false, ""},

    // Output flags for the Orthographic Input Lexicon (OIL)
    {"OILDRC",                    true,  true , ""},  // Display the DRC OIL activations (Orth values).
    {"OILDRCT",                   true,  true , ""},  // Display the DRC Totals for the OIL (TO line).
    {"OILFromLL",                 false, false, ""},
    {"OILFromOIL",                false, false, ""},
    {"OILFromPOL",                false, false, ""},
    {"OILTimeT",                  false, false, ""},
    {"OILTimeTp1",                false, false, ""},

    // Output flags for the Phonological Output Lexicon (POL)
    {"POLDRC",                    true,  true , ""},  // Display the DRC POL activations (Phon values).
    {"POLDRCT",                   true,  true , ""},  // Display the DRC Totals for the OIL (TP line).
    {"POLFromOIL",                false, false, ""},
    {"POLFromPOL",                false, false, ""},
    {"POLFromPB",                 false, false, ""},
    {"POLTimeT",                  false, false, ""},
    {"POLTimeTp1",                false, false, ""},

    // Output flags for the Phoneme Buffer (PB)
    {"PBDRC",                     true,  true , ""},  // Display the DRC PB activations (P0-7 values).
    {"PBDRCT",                    true,  true , ""},  // Display the DRC Totals for the PB (TPh line).
    {"PBFromPOL",                 false, false, ""},
    {"PBFromPB",                  false, false, ""},
    {"PBTimeT",                   false, false, ""},
    {"PBTimeTp1",                 false, false, ""},

    // Output flags for the GPCRoute (GPC)
    {"GPCDRC",                    true,  true , ""},  // Display the DRC GPC activations (GPC0-7 values).
    {"GPCDRCT",                   true,  true , ""},  // Display the DRC Totals for the GPC (TGPC line).
    {"GPCRDRC",                   true,  true , ""},  // Display the DRC GPCR activations (GPCR0-7 values).
    {"GPCRDRCT",                  true,  true , ""},  // Display the DRC Totals for the GPC (TGPCR line).
    {"GPCFromLL",                 false, false, ""},
    {"GPCToPB",                   false, false, ""},
    {"GPCTimeT",                  false, false, ""},
    {"GPCTimeTp1",                false, false, ""},

    {NULL,false,false,NULL}
};

//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
t_iparam* FindIParam(const char* Name,bool FailHard)
{
    bool NotFound=true;
    t_iparam* ip=DRC_IParams;
    t_iparam* rtn=NULL;
    while(NotFound&&(ip->Name!=NULL)){
        if(strcmp(ip->Name,Name)==0){
            NotFound=false;
            rtn=ip;
            break;
        }
        ip++;
    }
    if(rtn==NULL){
        if(FailHard) {
            pfprintf(stderr,"ERROR: FindIParam(%s,%s) failure!\n",Name,FailHard?"true":"false");
            exit(1);
        }
    }
    return(rtn);
}

//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
t_fparam* FindFParam(const char* Name,bool FailHard)
{
    bool NotFound=true;
    t_fparam* fp=DRC_FParams;
    t_fparam* rtn=NULL;
    while(NotFound&&(fp->Name!=NULL)){
        if(strcmp(fp->Name,Name)==0){
            NotFound=false;
            rtn=fp;
            break;
        }
        fp++;
    }
    if(rtn==NULL){
        if(FailHard) {
            pfprintf(stderr,"ERROR: FindFParam(%s,%s) failure!\n",Name,FailHard?"true":"false");
            exit(1);
        }
    }
    return(rtn);
}

//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
t_sparam* FindSParam(const char* Name,bool FailHard)
{
    bool NotFound=true;
    t_sparam* sp=DRC_SParams;
    t_sparam* rtn=NULL;
    while(NotFound&&(sp->Name!=NULL)){
        if(strcmp(sp->Name,Name)==0){
            NotFound=false;
            rtn=sp;
            break;
        }
        sp++;
    }
    if(rtn==NULL){
        if(FailHard) {
            pfprintf(stderr,"ERROR: FindSParam(%s,%s) failure!\n",Name,FailHard?"true":"false");
            exit(1);
        }
    }
    return(rtn);
}

//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
t_bparam* FindBParam(const char* Name,bool FailHard)
{
    bool NotFound=true;
    t_bparam* bp=DRC_BParams;
    t_bparam* rtn=NULL;
    while(NotFound&&(bp->Name!=NULL)){
        if(strcmp(bp->Name,Name)==0){
            NotFound=false;
            rtn=bp;
            break;
        }
        bp++;
    }
    if(rtn==NULL){
        if(FailHard) {
            pfprintf(stderr,"ERROR: FindBParam(%s,%s) failure!\n",Name,FailHard?"true":"false");
            exit(1);
        }
    }
    return(rtn);
}


//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
void* FindParam(const char* Name)
{
    void *rtn=NULL;

    rtn=(void*)FindIParam(Name,false);
    if(rtn==NULL){
        rtn=(void*)FindFParam(Name,false);
    }
    if(rtn==NULL){
        rtn=(void*)FindSParam(Name,false);
    }
    if(rtn==NULL){
        rtn=(void*)FindBParam(Name,false);
    }

    return(rtn);
}


//---------------------------------------------------------------------------
// Routine: Return value of given named parameter.
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
char* FindLanguageParam(const char* Name,bool FailHard)
{
    bool NotFound=true;
    t_Properties* lp=Properties;
    t_Properties* rtn=NULL;

    // Search for the named string in the parameter list.
    for(int idx=0; NotFound&&(idx<ParametersIdx); idx++){
        if(strcasecmp(lp->Name,Name)==0){
            // If we find it then use the corresponding pointer.
            NotFound=false;
            rtn=lp;
            break;
        }
        lp++;
    }

    // If we didn't find the name then potentially report and exit program
    if((rtn==NULL)&&(FailHard)) {
        pfprintf(stderr,"ERROR: FindLanguageParam(%s,%s) failure!\n",Name,FailHard?"true":"false");
        exit(1);
    }

    // Otherwise, return NULL or the pointer to the value string.
    if(rtn==NULL){
        return(NULL);
    }else{
        return(rtn->Value);
    }
}


//------------------------------------------------------------------------------
// Usage message.
//------------------------------------------------------------------------------
const char *HelpMessage[]={
    "Usage: drc [OPTIONS]... [WORD] [MAXCYCLES]",
    "",
    "Model Alteration Options:",
    "  -l LANGUAGE - Use the language specified (default english1.1.6). (N/W)",
    "  -no VALUE   - Set the number of units in the visual feature & letter layers. (N/W)",
    "  -np VALUE   - Set the number of units in the phoneme layer",
    "                (for -no & -np the default values depend on the language used). (N/W)",
    "  -P PRM VAL  - Set parameter PRM to value VAL.",
    "  -p FILE     - Read parameters from FILE. (N/W)",
    "  -s          - Give homophones separate entries in the phonological lexicon.",
    "",
    "Simulation Options:",
    "  -c          - Continue running cycles until MAXCYCLES is reached, even if the",
    "                stimuli is named before then.",
    "",
    "Batch File Options:",
    "  -b FILE     - Process batch file (use '-b --' for stdin).",
    "  --bpart S N - Process at most N lines of the batch file, starting at line S",
    "                (blank & comment lines are ignored for line-counting purposes).",
    "  -x NUM      - Abort batch jobs when NUM incorrect results have occurred.",
    "  -T          - Test the batch file for errors (do not execute) (NEW).",
    "",
    "Stepping Through Parameter Values:",
    "  -S PRM STARTVAL ENDVAL NUMSTEPS",
    "              - Run this simulation NUMSTEPS times, starting with parameter",
    "                PRM set to value STARTVAL and increasing it incrementally so",
    "                that PRM will be set to ENDVAL on the final step.",
    "",
    "                This argument may be used multiple times, for example:",
    "                  drc -S GPCOnset 5 7 3 -S ActivationRate 0.2 0.5 4 word",
    "",
    "Simulation Output Options:",
    "  -A EXT      - Set the filename extension for activation files (sets -a).",
    "  -a          - Save activation levels.",
    "  -e          - Report extra information in the screen output & RT/stats files.",
    "  -m VALUE    - Report only activation levels >= VALUE (default 0.02).",
    "  --nofiles   - Suppress creation of all output files.",
    "  -r EXT      - Set the filename extension for the RT file.",
    "  --rdir DIR  - Create output files in the directory named DIR.",
    "                DIR will be created if it doesn't exist.",
    "  -t          - Report simulation duration(s) in ms.(N/A)",
    "  -v          - Pronounce the model's output audibly (Mac OS X only) (N/A).",
    "  -f FLAG     - Toggle an output flag (FLAG), use -f for list. (NEW)",
    "",
    "Utility Options:",
    "  Simulations are not run when using these options. The described behaviour is",
    "  performed instead. For --reg, --regcheck, --stress and --whammies, multiple",
    "  inputs can be run using I/O redirection. For example:",
    "",
    "     drc --reg < inputfilename   > outputfilename",
    "",
    "  --irreglist - List all the words in the selected language which are irregular",
    "                (the pronunciation produced by the GPC rules does not match the",
    "                vocabulary) (N/A)",
    "  --irregstress"
    "              - List all the words in the selected language which have",
    "                irregular stress (the stress pattern selected by the GPC stress",
    "                rules does not match the stress pattern in the vocabulary) (N/A)",
    "  --reg       - read words from stdin and apply GPC rules to them (N/A)",
    "  --reglist   - list all the words in the selected language which are regular (N/A)",
    "  --stress    - read words from stdin and apply GPC stress rules to them (N/A)",
    "  --whammies  - read words from stdin and list whammies for them (N/A)",
    "  --homophones- list all words in vocabulary that are homophones (NEW)",
    "  --homographs- list all words in vocabulary that are homographs (NEW)",
    "  --neighbourhood",
    "              - list of neighbourhood of nonword and size (NEW)",
    "",
    "Other Options:",
    "  -h, --help  - Display this help and exit",
    "  --version   - Display version numbers and exit",
    "  -o FILE     - Read command-line options from FILE",
    ""
};

int HelpMessageSz=sizeof(HelpMessage)/sizeof(HelpMessage[0]);

//------------------------------------------------------------------------------
// Structure and data for command line arguments.
//------------------------------------------------------------------------------
typedef struct{
    const char*   Flag;             // Command line flag for option.
    bool    NeedsDB;          // Point in execution of program that this option should be processed.
    int     (*Func)(char**);  // Function pointer to process this option.
}t_CmdLineArgs;
t_CmdLineArgs CmdLineArgs[]={
    {"--neighbourhood" ,true    ,Cmd_DspNeighbourhood},
    {"--homophones"    ,true    ,Cmd_DspHomophones},
    {"--homographs"    ,true    ,Cmd_DspHomographs},
    {"--bpart"         ,true    ,Set_BatchProcessLimits},
    {"--help"          ,false   ,DisplayHelp},
    {"--irreglist"     ,true    ,ListIrregularWords},
    {"--irregstress"   ,true    ,ListIrregularStressWords},
    {"--nofiles"       ,false   ,Set_NoOutputFiles},
    {"--rdir"          ,false   ,Set_OutputDirectory},
    {"--reg"           ,true    ,ApplyGPCRules},
    {"--reglist"       ,true    ,ListRegularWords},
    {"--stress"        ,true    ,ApplyGPCStressRules},
    {"--version"       ,false   ,DisplayVersion},
    {"--whammies"      ,true    ,ListWhammies},
    {"-A"              ,false   ,Set_ActivationFileExt},
    {"-P"              ,false   ,Set_SetParameter},
    {"-S"              ,true    ,Set_RepeatSimulation},
    {"-a"              ,false   ,Set_SaveActivationLvls},
    {"-b"              ,true    ,Set_ProcessBatchFile},
    {"-c"              ,false   ,Set_ContinueToMaxCycles},
    {"-e"              ,false   ,Set_ReportExtraInfo},
    {"-f"              ,false   ,Set_OutputFlags},
    {"-h"              ,false   ,DisplayHelp},
    {"-l"              ,false   ,Set_Language},
    {"-m"              ,false   ,Set_ReportActGreater},
    {"-no"             ,false   ,Set_VisualFeatureLetterUnits},
    {"-np"             ,false   ,Set_PhonemeUnits},
    {"-o"              ,false   ,ReadCmdLineOptionsFile},
    {"-p"              ,false   ,Set_ReadParameterFile},
    {"-r"              ,false   ,Set_RTFileExtn},
    {"-s"              ,false   ,Set_SeparateHomophoneFlag},
    {"-t"              ,false   ,Set_ReportSimDuration},
    {"-T"              ,false   ,Set_TestBatchOnly},
    {"-v"              ,false   ,Set_Verbalize},
    {"-x"              ,false   ,Set_MaxIncorrectResults},

    {NULL,false,NULL}  // Always leave this line at end of list (flags the end of list).

};


bool DispdNbhdTtl=false;

//------------------------------------------------------------------------------
// Routine: ProcessCmdLineArgs - Function to run through command line argument
//              list and test against our known flags in the CmdLineArgs[] table.
//              If it finds a match then calls the respective option processing routine.
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
void ProcessCmdLineArgs(int argc,char **argv)
{
    int t=-1;
    int i=-1;
    int MaxCycles=FindIParam("MaxCycles")->Value;
    const char* TestWord=NULL;
    char buf[MAXPATH];

    // For each command line argument...
    // TestAndCall() will increment 'a' when a switch is found.
    for(int a=1;a<argc;){
        bool ArgFound=false;
        // Search through entire argument list...
        for(t=0;(CmdLineArgs[t].Flag!=NULL);t++){
            // Find the matching flag.
            if(MatchArg(CmdLineArgs[t].Flag,argv[a])){
                // If it needs the Database(DB) then load it.
                if(CmdLineArgs[t].NeedsDB){
                    Load_Database();
                }
                // If we find a matching argument, check to see we have enough
                // command line elements left and call the respective subroutine.
                TestAndCall(a,argc,argv,CmdLineArgs[t].Func);
                ArgFound=true;
                break;
            }
        }

        // Check to see if we got an illegal switch.
        if(argv[a]!=NULL){
            char swtchr=argv[a][0];
            if((!ArgFound)&&((swtchr=='-')||(swtchr=='+'))){
                pfprintf(stderr,"ERROR: Switch '%s' is illegal. Use \"OpenDRC -h\".\n",argv[a]);
                break;
            }
        }

        // Couldn't find argument in list of arguments so this must be either
        // a test WORD or the MAXCYCLES parameter.
        if(!ArgFound){
            bool IsNumber=true;
            // Fell off end of command switch array which means we have either a
            // WORD to process or MAXCYCLES to process.
            for(i=0;i<(int)strlen(argv[a]);i++){
                if(!isdigit(argv[a][i])){
                    IsNumber=false;
                    break;
                }
            }
            if(IsNumber){
                // We got MAXCYCLES
                FindIParam("MaxCycles")->Value=atoi(argv[a]);

            }else{
                // We got a TEST word, make sure database is loaded.
                Load_Database();
                // ... then setup the test word.
                TestWord=argv[a];
                FindSParam("TestWord")->Value=TestWord;

                if(FindBParam("Neighbourhood")->Value){
                    // User just wants the neighbourhood of the word.
                    if(!DispdNbhdTtl) {
                        DspNeighbourTtl(stdout);
                        DispdNbhdTtl=true;
                    }
                    DspNeighbourhood(stdout,TestWord);
                }else{
                    // User wants the system run on the word...

                    // Reset the system.
                    DRC_ResetSystem();

                    CreateOutDir(TestWord);

                    // Open the RT file for this test word.
                    fhRTFile=OpenOutputFile(OutParentDirBuf,OutRunDirBuf,TestWord,"RT");
                    if(fhRTFile!=NULL){

                        // Display the ACT file header.
                        DRC_DisplayHeader(stdout);

                        WriteRTFileHeader(fhRTFile,false,TestWord);

                        // Do the processing on the test word.
                        MaxCycles=FindIParam("MaxCycles")->Value;
                        DRC_ProcessWord(MaxCycles,TestWord,"none");
                        // Close our RT file.
                        fclose(fhRTFile);
                    }
                }
            }
            a++;
        }
    }
}

//---------------------------------------------------------------------------
// Routine: WriteRTFileHeader - Writes out the information at the top of the RT Files
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
void WriteRTFileHeader(FILE* fh,bool batch,const char* word){
    pfprintf(fh,"; OpenDRC Reaction Time Results File\n");
    DRC_DisplayHeader(fh);
    pfprintf(fh,"; Results for %s \"%s\"\n",(batch)?"BATCH":"WORD",word);
}


//------------------------------------------------------------------------------
// Routine to check to see if the command line flag matches a CmdLineArgs[] table flag.
//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
bool MatchArg(const char *flag,const char *arg){
    bool rtn=false;

    if(strcmp(flag,arg)==0){
        rtn=true;
    }
    return(rtn);
}

//------------------------------------------------------------------------------
// Routine to make sure we have enough arguments following the command line option
// and if we do then call the given option processing routine.
//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
void TestAndCall(int &idx,int argc,char **argv,int (*func)(char **argv))
{
    int args=0;
    if(idx+args<=argc){
        args=(func)(&argv[idx]);
        idx+=args;
    }
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// List of option processing routines
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void ErrCmdSwt(const char *str)
{
    fprintf(stderr,"ERROR: \"%s\" command switch not implemented yet.\n",str);
    exit(1);
};


//---------------------------------------------------------------------------
// Routine: Set_Language - Retrieve language from given directory. (Not Working).
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
int Set_Language(char **arglist)
{
    // -l LANGUAGE  -Use the language specified (default english1.1.6)
    //char* CmdLineFlag=arglist[0];
    const char* Language=arglist[1];

    // NOTE: Should test to make sure this directory exists and that there
    //       is a trailing slash.
    FindSParam("LanguageDirectory")->Value=Language;
    ErrCmdSwt("-l LANGUAGE");
    return(2);
}

//---------------------------------------------------------------------------
// Routine: Set_VisualFeatureLetterUnits - Set width of input buffer. (Not Working).
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
int Set_VisualFeatureLetterUnits(char **arglist)
{
    // -no VALUE  -Set the number of units in the visual feature & letter layers
    //             (The default value depends on the language used)
    //char* CmdLineFlag=arglist[0];
    const char* Units=arglist[1];

    // NOTE: Should test bounds on this number (do we have min and max?).
    FindIParam("VisualFeatureLetterUnits")->Value=atoi(Units);
    ErrCmdSwt("-no VALUE");
    return(2);
}

//---------------------------------------------------------------------------
// Routine: Set_PhonemeUnits - Set width of output buffer (Not Working).
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
int Set_PhonemeUnits(char **arglist)
{
    // -np VALUE  -Set the number of units in the phoneme layer
    //             (The default value depends on the language used)
    //char* CmdLineFlag=arglist[0];
    const char* Units=arglist[1];

    // NOTE: Should test bounds on this number (do we have min and max?).
    FindIParam("PhonemeUnits")->Value=atoi(Units);
    ErrCmdSwt("-np VALUE");
    return(2);
}


//---------------------------------------------------------------------------
// Routine: Set_SetParameter - Sets the parameter value. (Working).
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
int Set_SetParameter(char **arglist)
{
    // -P PRM VAL  -Set parameter PRM to value VAL
    //char* CmdLineFlag=arglist[0];
    const char* Prm=arglist[1];
    const char* Val=arglist[2];

    // Search for this parameter (Prm) in parameter list and set the current
    // value to the given value(Val).

    t_iparam* ip=NULL;
    t_fparam* fp=NULL;
    t_sparam* sp=NULL;
    t_bparam* bp=NULL;
    if((ip=FindIParam(Prm,false))!=NULL){
        printf("Setting parameter '%s':\n",ip->Name);
        printf("    Old Value=%d\n",ip->Value);
        printf("    New Value=%ld\n",(long)Val); /* XXX: WTF? */
        ip->Value=atoi(Val);
    }else if((fp=FindFParam(Prm,false))!=NULL){
        printf("Setting parameter '%s':\n",fp->Name);
        printf("    Old Value=%f\n",fp->Value);
        printf("    New Value=%s\n","stupid cast from Val is stupid");
        fp->Value=atof(Val);
    }else if((sp=FindSParam(Prm,false))!=NULL){
        printf("Setting parameter '%s':\n",sp->Name);
        printf("    Old Value='%s'\n",sp->Value);
        printf("    New Value='%s'\n",Val);
        sp->Value=Val;
    }else if((bp=FindBParam(Prm,false))!=NULL){
        printf("Setting parameter '%s':\n",bp->Name);
        printf("    Old Value=%s\n",bp->Value?"true":"false");
        /* bp->Value=atob(Val); */
        bp->Value = true;
        printf("    New Value=%s\n",bp->Value?"true":"false");
    }else{
        pfprintf(stderr,"ERROR: Parameter '%s' not found\n",Prm);
    }
    return(3);
}

//---------------------------------------------------------------------------
// Routine: Set_ReadParameterFile - Read command line parameters from file. (Not Working)
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
int Set_ReadParameterFile(char **arglist)
{
    // -p FILE  -Read parameters from FILE
    //char* CmdLineFlag=arglist[0];
    //char* FileName=arglist[1];

    // NOTE: Need to run the parameter processing Yacc file on the given
    //       file if it exists and update our current parameter values in
    //       the table above.
    ErrCmdSwt("-p FILE");
    return(2);
}

//---------------------------------------------------------------------------
// Routine: Set_SeparateHomophoneFlag - Give homophones separate entries in POL. (Working)
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
int Set_SeparateHomophoneFlag(char **arglist)
{
    // -s  -Give homophones separate entries in the phonological lexicon
    //char* CmdLineFlag=arglist[0];

    FindBParam("SeparateHomophones")->Value=true;
    return(1);
}

//---------------------------------------------------------------------------
// Routine: Set_ContinueToMaxCycles - Set limit for number of cycles (Working).
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
int Set_ContinueToMaxCycles(char **arglist)
{
    // -c  -Continue running cycles until MAXCYCLES is reached, even if the
    //      stimuli is named before then.
    //char* CmdLineFlag=arglist[0];

    FindIParam("MaxCycles")->Value=true;
    return(1);
}

//---------------------------------------------------------------------------
// Routine: Set_ProcessBatchFile - Read input in from batch file (working).
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
int Set_ProcessBatchFile(char **arglist)
{
    // -b FILE  -Process batch file (use '-b --' for stdin)
    //char* CmdLineFlag=arglist[0];
    const char* FileName=arglist[1];

    if(FindBParam("Neighbourhood")->Value){
        // User only wants neighbourhood of words.
        ReadBatchFile(FileName);
    }else{
        // Display the ACT file header.
        DRC_DisplayHeader(stdout);

        ReadBatchFile(FileName);

        // Write out the statistics info.
        WriteBatchStats(stdout);
    }
    return(2);
}

//---------------------------------------------------------------------------
// Routine: Set_TestBatchOnly - Test the batch file without executing it (Working).
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
int Set_TestBatchOnly(char **arglist)
{
    // -t   - Test batch file only (no execution done).
    FindBParam("TestBatchOnly")->Value=true;
    return(1);
}

//---------------------------------------------------------------------------
// Routine: Set_BatchProcessLimits - Execute a subsection of the batch file (Working)
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
int Set_BatchProcessLimits(char **arglist)
{
    // --bpart S N  process at most N lines of the batch file, starting at line S
    //              (blank & comment lines are ignored for line-counting purposes)
    //char* CmdLineFlag=arglist[0];
    const char* Start=arglist[1];
    const char* Lines=arglist[2];

    // NOTE: If these values are within reason (they have been checked) then
    FindIParam("BatchFileStart")->Value=atoi(Start);
    FindIParam("BatchFileLines")->Value=atoi(Lines);
    return(3);
}

//---------------------------------------------------------------------------
// Routine: Set_MaxIncorrectResults - Abort processing of the batch file when
//              the given maximum number of errors has occured (Working).
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
int Set_MaxIncorrectResults(char **arglist)
{
    //  -x NUM  -Abort batch jobs when NUM incorrect results have occurred
    //char* CmdLineFlag=arglist[0];
    const char* Num=arglist[1];

    // NOTE: Assumed ranged checked.
    int value=atoi(Num);
    if(value>0){
        FindIParam("MaxErrors")->Value=value;
    }
    return(2);
}

//---------------------------------------------------------------------------
// Routine: Set_RepeatSimulation - Repeatedly run the simulation for each word
//              for all the values of the parameters. (Not Working).
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
int Set_RepeatSimulation(char **arglist)
{
    //   -S PRM STARTVAL ENDVAL NUMSTEPS
    //      -Run this simulation NUMSTEPS times, starting with parameter
    //       PRM set to value STARTVAL and increasing it incrementally so
    //       that PRM will be set to ENDVAL on the final step.
    //
    //       This argument may be used multiple times, for example:
    //           drc -S GPCOnset 5 7 3 -S ActivationRate 0.2 0.5 4 word
    //char* CmdLineFlag=arglist[0];
    const char* Prm=arglist[1];
    const char* StartVal=arglist[2];
    const char* EndVal=arglist[3];
    const char* NumSteps=arglist[4];

    // NOTE: Assumed range checked.
    FindSParam("SimParam")->Value=Prm;
    FindFParam("SimStartVal")->Value=atof(StartVal);
    FindFParam("SimEndVal")->Value=atof(EndVal);
    FindIParam("SimNumSteps")->Value=atoi(NumSteps);
    ErrCmdSwt("-S PRM STARTVAL ENDVAL NUMSTEPS");
    return(5);
}

//---------------------------------------------------------------------------
// Routine: Set_ActivationFileExt - Set the extension of the activation files. (Not Working).
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
int Set_ActivationFileExt(char **arglist)
{
    // -A EXT   -Set the filename extension for activation files (sets -a)
    //           (NOTE: Not sure what "sets -a" means for the program.
    //char* CmdLineFlag=arglist[0];
    const char* Ext=arglist[1];

    // NOTE: Should probably check that the extension is a valid extension (alphabetics).
    FindSParam("OutputActivationFileExt")->Value=Ext;
    ErrCmdSwt("-A EXT");
    return(2);
}

//---------------------------------------------------------------------------
// Routine: Set_SaveActivationLvls - Generate ACTS files with all the activation
//              levels worth reporting. (Working).
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
int Set_SaveActivationLvls(char **arglist)
{
    // -a  -Save activation levels
    //char* CmdLineFlag=arglist[0];

    FindBParam("SaveActivationLvls")->Value=true;
    return(1);
}

//---------------------------------------------------------------------------
// Routine: Set_ReportExtraInfo - Report extra information to the RT line. (Not Working).
//              Should work with the -t switch.
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
int Set_ReportExtraInfo(char **arglist)
{
    // -e  -Report extra information in the screen output & RT/stats files
    //char* CmdLineFlag=arglist[0];
    // Example:
    //    "czars NONWORD kz#z PNONWORD 141 NAMED CORRECT kz#z 4 4 0"

    FindBParam("ReportExtraInfo")->Value=true;
    ErrCmdSwt("-e");
    return(1);
}

//---------------------------------------------------------------------------
// Routine: Set_OutputFlags - Toggle the boolean flag value. (Working).
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
int Set_OutputFlags(char **arglist)
{
    //  -f FLAG    - Toggle an output flag (FLAG), use -f for list.",
    int rtn=1;  // How many arguments we used.

    t_bparam *pflag=NULL;  // Default is no argument.
    if(arglist[1]!=NULL){
        // If the next string is not NULL then assume it is a flag name.
        pflag=FindBParam(arglist[1],false);
    }
    if(pflag!=NULL){
        // If we found a valid flag then change it's value.
        pflag->Value=!pflag->Value;
        printf("FLAG: %s = %s\n",pflag->Name,(pflag->Value)?"true":"false");
        rtn=2;
    }else{
        // If user didn't enter a flag or gave an invalid flag name...
        if((arglist[1]!=NULL)&&(arglist[1][0]!='-')) {
            // User *did* give an argument.
            pfprintf(stderr,"WARNING: Flag '%s' is not valid. Use -f for list.\n",arglist[1]);
            rtn=2;
        }else{
            // If user gave no argument display the flag list.
            t_bparam *start=FindBParam("FLDRC");
            t_bparam *end  =FindBParam("GPCTimeTp1");
            printf("DRC Output Flags:\n");
            for(;start<=end; start++){
                printf("    %s\n",start->Name);
            }
        }
    }
    return(rtn);
}

//---------------------------------------------------------------------------
// Routine: Set_ReportActGreater - Set the reporting threshold for display of
//              the activation levesl in the ACTS files. (Working).
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
int Set_ReportActGreater(char **arglist)
{
    // -m VALUE  -Report only activation levels >= VALUE (default 0.02)
    //char* CmdLineFlag=arglist[0];
    const char* Value=arglist[1];

    FindFParam("ReportActGreater")->Value=atof(Value);
    return(2);
}

//---------------------------------------------------------------------------
// Routine: Set_NoOutputFiles - Do not generate output files (Not Working).
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
int Set_NoOutputFiles(char **arglist)
{
    // --nofiles  -Suppress creation of all output files
    //char* CmdLineFlag=arglist[0];

    FindBParam("NoOutputFiles")->Value=true;
    ErrCmdSwt("--nofiles");
    return(1);
}

//---------------------------------------------------------------------------
// Routine: Set_RTFileExtn - Set the RT file extensions (Not Working).
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
int Set_RTFileExtn(char **arglist)
{
    // -r EXT  -Set the filename extension for the RT file
    //char* CmdLineFlag=arglist[0];
    const char* Ext=arglist[1];

    FindSParam("OutputRTFileExt")->Value=Ext;
    ErrCmdSwt("-r EXT");
    return(2);
}

//---------------------------------------------------------------------------
// Routine: Set_OutputDirectory - Sets the output directory for the files. (Working)
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
int Set_OutputDirectory(char **arglist)
{
    // --rdir DIR  -Create output files in the directory named DIR
    //              (DIR will be created if it doesn't exist)
    //char* CmdLineFlag=arglist[0];
    const char* Dir=arglist[1];

    // NOTE: This should probably be format checked first (so we don't get
    //       directories the user can't access.
    // NOTE: Should also check to see if the directory exists and create it if it doesn't.
    strcpy(OutParentDirBuf,Dir);
    return(2);
}

//---------------------------------------------------------------------------
// Routine: Set_ReportSimDuration - Report the simulation duration. (Not Working).
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
int Set_ReportSimDuration(char **arglist)
{
    // -t  -Report simulation duration(s) in ms
    //char* CmdLineFlag=arglist[0];

    // This routine should take the final RT value for the run of a word and
    // massage this with some formula in RT to produce a number of millisecons.
    // Example RT=64, Func=Init+10*RT+Term. Init=5, Term=7, resulting MS=5+10*64+7 = 652
    // The MS time is then presented on the RT line as such:
    //     "help  hElp  64  NAMED  CORRECT  (~652ms)"

    FindBParam("ReportSimDuration")->Value=true;
    ErrCmdSwt("-t");
    return(1);
}

//---------------------------------------------------------------------------
// Routine: Set_Verbalize - Actually convert the phonemic output to sounds. (Not Working)
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
int Set_Verbalize(char **arglist)
{
    // -v  -Pronounce the model's output audibly (Mac OS X only)
    //char* CmdLineFlag=arglist[0];

    // NOTE: Should see if we can find a program for Linux and Windows that will
    //       verbalize a string of phonemes.  Which program on Mac OS X does this?

    FindBParam("Verbalize")->Value=true;
    ErrCmdSwt("-v");
    return(1);
}

//---------------------------------------------------------------------------
// Routine: ListIrregularWords - Generate a list of irregular words
//              from the vocabulary (Not Working).
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
int ListIrregularWords(char **arglist)
{
    // --irreglist  -List all the words in the selected language which are irregular
    //               (the pronunciation produced by the GPC rules does not match the vocabulary)
    //char* CmdLineFlag=arglist[0];

    // NOTE: Need to generate a function to do this.
    FindBParam("ListIrregularWords")->Value=true;
    ErrCmdSwt("--irreglist");
    return(1);
}

//---------------------------------------------------------------------------
// Routine: ListIrregularStressWords - Generate a list of irregular stress words
//              from the vocabulary (Not Working).
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
int ListIrregularStressWords(char **arglist)
{
    // --irregstress  -List all the words in the selected language which have
    //                 irregular stress (the stress pattern selected by the GPC stress
    //                 rules does not match the stress pattern in the vocabulary)
    //char* CmdLineFlag=arglist[0];

    // NOTE: Need to generate a function to do this.
    FindBParam("ListIrregularStressWords")->Value=true;
    ErrCmdSwt("--irregstress");
    return(1);
}

//---------------------------------------------------------------------------
// Routine: ApplyGPCRules - Only apply the GPC rules to the input list of words.
//              (Not Working).
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
int ApplyGPCRules(char **arglist)
{
    // --reg  -Read words from stdin and apply GPC rules to them
    //char* CmdLineFlag=arglist[0];

    // NOTE: Need to generate a function to do this.
    FindBParam("ApplyGPCRules")->Value=true;
    ErrCmdSwt("--reg");
    return(1);
}

//---------------------------------------------------------------------------
// Routine: ListRegularWords - Generate a list of regular vocabulary words.
//              (Not Working).
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
int ListRegularWords(char **arglist)
{
    // --reglist  -List all the words in the selected language which are regular
    //char* CmdLineFlag=arglist[0];

    // NOTE: Need to generate a function to do this.
    FindBParam("ListRegularWords")->Value=true;
    ErrCmdSwt("--reglist");
    return(1);
}

//---------------------------------------------------------------------------
// Routine: ApplyGPCStressRules - Apply stress rules to given words (Not Working).
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
int ApplyGPCStressRules(char **arglist)
{
    // --stress  -Read words from stdin and apply GPC stress rules to them
    //char* CmdLineFlag=arglist[0];

    // NOTE: Need to generate a function to do this.
    FindBParam("ApplyGPCStressRules")->Value=true;
    ErrCmdSwt("--stress");
    return(1);
}

//---------------------------------------------------------------------------
// Routine: ListWhammies - Generate a list of Whammies (Not Working).
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
int ListWhammies(char **arglist)
{
    // --whammies  -Read words from stdin and list whammies for them
    //char* CmdLineFlag=arglist[0];

    // NOTE: Need to generate a function to do this.
    FindBParam("ListWhammies")->Value=true;
    ErrCmdSwt("--whammies");
    return(1);
}

//---------------------------------------------------------------------------
// Routine: DisplayHelp - Routine to display the Help message to the console. (Working)
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
int DisplayHelp(char **arglist)
{
    // Display the help message as it appears in the text array above.
    // Insert terminating newlines on each line (requires that blank lines
    // are represented in the HelpMessage array (ie. "",).
    // NOTE: Currently the DRC version numbers are just hardcoded in and need
    //       to be programatically inserted.
    // NOTE: The version number for each file could be generated by a single
    //       subroutine and left out everywhere else.
    //char* CmdLineFlag=arglist[0];

    for(int line=0;line<HelpMessageSz;line++){
        pfprintf(stdout,"%s\n",HelpMessage[line]);
    }
    return(1);
}

//---------------------------------------------------------------------------
// Routine: DisplayVersion - Command line wrapper for the routine that displays
//              the versioning information.
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
int DisplayVersion(char **arglist)
{
    // --version  -Display version numbers and exit
    //char* CmdLineFlag=arglist[0];

    // NOTE: Need procedure to list the version numbers of everything.
    PrtVersion(stdout);
    exit(1);
    return(0);
}

const char* Delimiters=" \t\n";

//---------------------------------------------------------------------------
// Routine: ReadCmdLineOptionsFile - Reads and processes the command line options
//              given in the input file.
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
int ReadCmdLineOptionsFile(char **arglist)
{
    // -o FILE  -Read command-line options from FILE
    //char* CmdLineFlag=arglist[0];
    const char* File=arglist[1];

    char* ArgsArray[MAXARGS];
    int   NumArgs=0;
    char  buf[MAXLINE];
    FILE* fh=NULL;
    char* token=NULL;

    // NOTE: Need to create a procedure to read the command line options in from
    //       the given file (one or more per line) and store them in an argc/argv
    //       arrangement and then pass them to ProcessCmdLineArgs().

    // Clean up for the first line to be processed.
    memset(ArgsArray,0,sizeof(ArgsArray));

    if((fh=fopen(File,"r"))==NULL){
        perror("ERROR: Options File");
        exit(1);
    }else{
        // The file opened correctly so lets read it line by line.
        while(fgets(buf,MAXLINE,fh)!=NULL){
            // Now tokenize the line based on the Delimiters.
            NumArgs=1;
            ArgsArray[0]=strdup("DRC.exe"); /* XXX: strdup */
            token=strtok(buf,Delimiters);
            while(token!=NULL){
                // Save the pointers to the tokens (just like argv).
                ArgsArray[NumArgs++]=token;
                token=strtok(NULL,Delimiters);
            }
            // Now that we have parsed the line process it.
            ProcessCmdLineArgs(NumArgs,ArgsArray);
            // Clean up for the next line.
            memset(ArgsArray,0,sizeof(ArgsArray));
        }
        // We're finished, so close the file.
        fclose(fh);
    }
    return(2);
}

//---------------------------------------------------------------------------
// Routine: Cmd_DspNeighbourhood - Command line wrapper for the Orthographic
//              Neighbourhood display routine. (Working).
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
int Cmd_DspNeighbourhood(char **arglist)
{
    FindBParam("Neighbourhood")->Value=true;
    return(1);
}

//---------------------------------------------------------------------------
// Routine: Cmd_DspHomographs - Command line wrapper for the Homograph display
//              rtn. (Working).
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
int Cmd_DspHomographs(char **arglist)
{
    DisplayHomographs(stdout);
    return(1);
}

//---------------------------------------------------------------------------
// Routine: Cmd_DspHomophones - Command line wrapper for the Homophone Display
//              rtn (Working).
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
int Cmd_DspHomophones(char **arglist)
{
    DisplayHomophones(stdout);
    return(1);
}

//------------------------------------------------------------------------------
// End of $RCSfile: DRC_CmdLine.cpp,v $
//------------------------------------------------------------------------------

