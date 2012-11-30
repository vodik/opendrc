//------------------------------------------------------------------------------
// $Header: D:\\Repository\\D\\Dev\\Psych499\\DRCTest.cpp,v 1.9 2011-03-14 17:09:41-04 alan Exp alan $
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
// Program to help with determining where two output DRC file systems differ.
//
// NOTE: See the Wikipedia topic "Longest common subsequence problem" at:
//       http://en.wikipedia.org/wiki/Longest_common_subsequence_problem
//
// $Log: DRCTest.cpp,v $
// Revision 1.9  2011-03-14 17:09:41-04  alan
// Correct for differences in BORLANDC and GNUC.
// Fixed include for debug.h
// Fixed Cmd_SetDirectories() routine to get rid of trailing '/' or '\' on directory names.
// Changed all findfirst/findnext to scandir calls.  This required changing the
// "struct ffblk ffblk" line to "struct dirent **namelist" and changing the findfirst to scandir(),
// eliminating the findnext and adding code to deal with the namelist structures.
// Added code to SaveACTSTotals() to deal with comments and short lines which would
// cause a segment fault.
//
// Revision 1.0  2011-03-13 23:18:02-04  alan
// Initial revision
//
// Revision 1.8  2011-02-18 15:52:59-05  alan
// Updated files with license information.
//
// Revision 1.7  2011-01-16 20:33:45-05  alan
// Save the last Total activations from all ACTS files in a file DRCTotals.txt that has the
// totals listed across the page.  In format suitable to readinto Excel etc.
//
// Revision 1.6  2010-12-07 18:55:48-05  alan
// Get program to exit after displaying the help message.
//
// Revision 1.5  2010-11-28 15:39:25-05  alan
// Instituted a maximum number of lines to compare (def: 1000 - excluding matching lines
// at the beginning).
// Added -ml command line argument to change the max lines to compare.
// Added extra condition to test for an activation line so we don't print activation values
// of zero out when the line naturally contains a '#' character.
//
// Revision 1.4  2010-11-28 00:06:59-05  alan
// Put in check for null string in BuildCRCStr.
// Rearranged CmpCRCStr() to test the string part first.  Also copied the Old string into a
// larger buffer to test the name.  Strncmp()'d the string to test for total line names.
//
// Revision 1.3  2010-11-26 00:08:17-05  alan
// Corrected an overflow bug when trimming strings that were empty.
// Tried to correct Total line thresholding problem.
//
// Revision 1.2  2010-11-04 10:37:26-04  alan
// Cleaned up output.
// Added code to implement processing of RT, STATS and Parameter files.
//
// Revision 1.1  2010-11-03 17:36:58-04  alan
// Got the ACTS file checking working.
// Started to clean up the code.  Commented all the subroutines.
// Made program generate useful output.
//
// Revision 1.0  2010-11-03 00:56:24-04  alan
// Initial revision
//
//
//------------------------------------------------------------------------------

#include "DRCTest_hdrstop.h"


//#define DEBUG
//#include "debug.h"

const char* DRC_CmdLine[]={
	"$Author: alan $",
	"$Date: 2011-03-14 17:09:41-04 $",
	"$Revision: 1.9 $",
	"$RCSfile: DRCTest.cpp,v $",
	"$Source: D:\\Repository\\D\\Dev\\Psych499\\DRCTest.cpp,v $"
};

const char* FN_OutputFile="DRCTest.txt";
const char* FN_ACTSTotals="DRCTotals.txt";
FILE* fhACTSTotals=NULL;

char* DIR_Old=".";
char* DIR_New=".";

// Maximum number of lines to compare between files.
int MaxCmpLines = 1000;


// Global variables.
float ErrThreshold   = 0.0000015;
float ErrTtlMult     = 2.5;
long  ErrorCount     = 0;

// Global flags.
bool FLG_CompComm    = false;

bool FLG_ACTSFiles   = true;
bool FLG_RTFiles     = true;
bool FLG_STATSFiles  = true;
bool FLG_ParamFiles  = true;

// General defines.
#define COMMENT ';'

//------------------------------------------------------------------------------
// Usage message.
//------------------------------------------------------------------------------
char *HelpMessage[]={
    "Usage: DRCTest {-acts|-rt|-stats|-{et|em|ml} <num>|-o <ofile>} <NewDir> {<OldDir>}",
    "  With respect to the files in the <OldDir> and <NewDir>",
    "  <OldDir> and <NewDir> default to the current directory",
    "    -acts     Toggle the ACTS file testing (Default: On),",
    "    -rt       Toggle the RT file testing (Default: On),",
    "    -stats    Toggle the STATS file testing (Default: On),",
    "    -param    Toggle the Parameter file testing (Default: On),",
    "    -et<num>  Set the error threshold to <num> (Default: 0.0000015)",
    "    -em<num>  Set the totals error multiplier (Default: 2.5)",
    "    -ml<num>  Set maximum lines to compare (Default: 1000)",
    "    -o <file> Send output report to <ofile> (Default: DRCTest.txt),",
    "    -h        This help message",
    "",
    NULL
};
int HelpMessageSz=sizeof(HelpMessage)/sizeof(char*);

//------------------------------------------------------------------------------
// Structure and data for command line arguments.
//------------------------------------------------------------------------------
typedef struct{
    char*   Flag;             // Command line flag for option.
    int     (*Func)(char**);  // Function pointer to process this option.
}t_CmdLineArgs;
t_CmdLineArgs CmdLineArgs[]={
    {"-acts"   ,Cmd_CheckACTSFiles},
    {"-rt"     ,Cmd_CheckRTFiles},
    {"-stats"  ,Cmd_CheckSTATSFiles},
    {"-param"  ,Cmd_CheckParamFiles},
    {"-et"     ,Cmd_SetErrThreshold},
    {"-em"     ,Cmd_SetErrTtlMult},
    {"-ml"     ,Cmd_SetMaxLines},
    {"-h"      ,Cmd_DisplayHelp},
    {"-o"      ,Cmd_OutputFile},
    {""        ,Cmd_SetDirectories},

    {NULL,NULL}  // Always leave this line at end of list (flags the end of list).
};


//------------------------------------------------------------------------------
// Routine to check to see if the command line flag matches a CmdLineArgs[] table flag.
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
ProtoType
bool MatchArg(const char *flag,char *arg){
    bool rtn=false;

    if(strcmp(flag,arg)==0){
        rtn=true;
    }
    return(rtn);
}

//------------------------------------------------------------------------------
// Routine to make sure we have enough arguments following the command line option
// and if we do then call the given option processing routine.
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
ProtoType
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

//---------------------------------------------------------------------------
// Routine: Cmd_SetMaxLines - Sets the maximum number of lines to compare.
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
#pragma argsused arglist
ProtoType
int Cmd_SetMaxLines(char** arglist)
{
    int rtn=2;
    int MCL=0;

    sscanf(arglist[1],"%d",&MCL);
    fprintf(stdout,"Changing 'Max Lines to Compare' from %d to %d\n",MaxCmpLines,MCL);
    MaxCmpLines=MCL;

    return(rtn);
}

//---------------------------------------------------------------------------
// Routine: Cmd_SetErrTtlMult - Sets the Error Total Multiplier (how much bigger
//          the errors for totals can be from those for simple activations -- due
//          to fact that the totals sum the errors from the simple activations).
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
#pragma argsused arglist
ProtoType
int Cmd_SetErrTtlMult(char** arglist)
{
    int rtn=2;
    float ETM;

    sscanf(arglist[1],"%f",&ETM);
    fprintf(stdout,"Changing Totals Error Multiplier from %10.8f to %10.8f\n",ErrTtlMult,ETM);
    ErrTtlMult=ETM;

    return(rtn);
}

//---------------------------------------------------------------------------
// Routine: Cmd_SetErrThreshold - Set the simple activation error threshold.
//          Anything larger than this amount of error is flaged.
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
#pragma argsused arglist
ProtoType
int Cmd_SetErrThreshold(char** arglist)
{
    int rtn=2;
    float ErrThresh;

    sscanf(arglist[1],"%f",&ErrThresh);
    fprintf(stdout,"Changing Error Threshold from %10.8f to %10.8f\n",ErrThreshold,ErrThresh);
    ErrThreshold=ErrThresh;

    return(rtn);
}

//---------------------------------------------------------------------------
// Routine: Cmd_CheckACTSFiles - Toggle the FLG_ACTSFiles flag turning on/off
//          the testing of ACTS files.
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
#pragma argsused arglist
ProtoType
int Cmd_CheckACTSFiles(char** arglist)
{
    int rtn=1;

    FLG_ACTSFiles = !FLG_ACTSFiles;

    return(rtn);
}

//---------------------------------------------------------------------------
// Routine: Cmd_CheckRTFiles - Toggle the FLG_RTFiles flag turning on/off
//          the testing of RT files.
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
#pragma argsused arglist
ProtoType
int Cmd_CheckRTFiles(char** arglist)
{
    int rtn=1;

    FLG_RTFiles = !FLG_RTFiles;

    return(rtn);
}

//---------------------------------------------------------------------------
// Routine: Cmd_CheckSTATSFiles - Toggle the FLG_STATSFiles flag turning on/off
//          the testing of STATS files.
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
#pragma argsused arglist
ProtoType
int Cmd_CheckSTATSFiles(char** arglist)
{
    int rtn=1;

    FLG_STATSFiles = !FLG_STATSFiles;

    return(rtn);
}

//---------------------------------------------------------------------------
// Routine: Cmd_CheckParamFiles - Toggle the FLG_ParamFiles flag turning on/off
//          the testing of Parameter files.
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
#pragma argsused arglist
ProtoType
int Cmd_CheckParamFiles(char** arglist)
{
    int rtn=1;

    FLG_ParamFiles = !FLG_ParamFiles;

    return(rtn);
}

//---------------------------------------------------------------------------
// Routine: Cmd_OutputFile - Will allow user to set their own output file name.
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
#pragma argsused arglist
ProtoType
int Cmd_OutputFile(char** arglist)
{
    int rtn=1;

    FN_OutputFile=arglist[1];
    rtn++;

    return(rtn);
}

//---------------------------------------------------------------------------
// Routine: Cmd_DisplayHelp - Will display the Help messages.
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
#pragma argsused arglist
ProtoType
int Cmd_DisplayHelp(char** arglist)
{
    int rtn=1;

    int i=0;
    while(HelpMessage[i]!=NULL){
        fprintf(stdout,"%s\n",HelpMessage[i]);
        i++;
    }
    exit(0);
    return(rtn);
}

//---------------------------------------------------------------------------
// Routine: Cmd_SetDirectories - Will set new names for the Old and New directories
//          to compare.
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
#pragma argsused arglist
ProtoType
int Cmd_SetDirectories(char** arglist)
{
    int rtn=1;
    static bool FirstDir=true;
	char* str=arglist[0];
	if(str!=NULL){
		// Always get rid of trailing '\' or '/'
		int strsz=strlen(str);
		if((strsz>0)&(str[strsz-1]=='/')) str[strsz-1]='\0';
		if((strsz>0)&(str[strsz-1]=='\\')) str[strsz-1]='\0';

		// The first directory is the New directory.  This is to allow the user to
		// enter only one directory (New) the default old directory being '.'.
		if(FirstDir){
			DIR_New = arglist[0];
			FirstDir=false;
		}else{
			DIR_Old = arglist[0];
		}
	}

    return(rtn);
}



//------------------------------------------------------------------------------
// Function to run through command line argument list and test against our known
// flags in the CmdLineArgs[] table.  If it finds a match then calls the respective
// option processing routine.
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
ProtoType
void ProcessCmdLineArgs(int argc,char **argv)
{
    int t=-1;
    char buf[MAXPATH];

    // For each command line argument...
    // TestAndCall() will increment 'a' when a switch is found.
    for(int a=1;a<argc;){
        bool ArgFound=false;
        // Search through entire argument list...
        for(t=0;(CmdLineArgs[t].Flag!=NULL);t++){
            // Find the matching flag.
            if(MatchArg(CmdLineArgs[t].Flag,argv[a])){
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
                fprintf(stderr,"ERROR: Switch '%s' is illegal. Use \"DRCTest -h\".\n",argv[a]);
                break;
            }
        }

        // Couldn't find argument in list of arguments so this must be directory
        if(!ArgFound){
            // Fell off end of command switch array which means we have a
            // directory to compare.
            for(t=0;(CmdLineArgs[t].Flag!=NULL);t++){
                // Find the matching NULL argument.
                if(MatchArg(CmdLineArgs[t].Flag,"")){
                    TestAndCall(a,argc,argv,CmdLineArgs[t].Func);
                    ArgFound=true;
                    break;
                }
            }
        }
    }
}

//---------------------------------------------------------------------------
// Routine: SortFileNamesPred - Routine to implement the sort direction for
//          file names in the vector<char*> arrays.
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
bool SortFileNamesPred(const char* d1, const char* d2)
{
    char buf1[MAXPATH];
    char buf2[MAXPATH];

    // Get rid of the extensions so we can compare file names.  This is important
    // so that file 'xxxx' gets sorted before 'xxxx-1' etc.
    strcpy(buf1,d1);
    strtok(buf1,".");  // Set the '.' separating the filename and fileextn to '\0' (terminating string)
    strcpy(buf2,d2);
    strtok(buf2,".");  // Set the '.' separating the filename and fileextn to '\0' (terminating string)

    // To change direction of sort change comparison operator.
    bool rtn=(strcmp(buf1,buf2)<=0);
    return(rtn);
}

//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------

#ifdef __GNUC__
int ExtnActs(const struct dirent *pe)
{
    int rtn=0;
    if(pe!=NULL){
        int len=strlen(pe->d_name);
        if(strncmpi(pe->d_name+len-5,".acts",5)==0){
	    rtn=1;
        }
    }
    return(rtn);
}
#endif


ProtoType
void ProcessACTSFiles(FILE* fh)
{

#ifdef __BORLANDC__
    struct ffblk ffblk;
#elif __GNUC__
	struct dirent **namelist;
#endif
    vector<char*> DirOldFiles;
    vector<char*> DirNewFiles;
    int done;
    char buf[MAXPATH];

    // Clear the file name vectors.
    DirOldFiles.erase(DirOldFiles.begin(),DirOldFiles.end());
    DirNewFiles.erase(DirNewFiles.begin(),DirNewFiles.end());

    fprintf(fh,"Processing ACTS files:\n");
    fprintf(stdout,"Processing ACTS files:\n");

    // Read in all the ACTS file names from the OLD directory.
#ifdef __BORLANDC__
    sprintf(buf,"%s\\*.acts",DIR_Old);
    done = findfirst(buf,&ffblk,0);
    while(!done){
        DirOldFiles.push_back(strdup(ffblk.ff_name));
        done = findnext(&ffblk);
    }
#elif __GNUC__
    //sprintf(buf,"%s/*.acts",DIR_Old);
    done=scandir(DIR_Old,&namelist,ExtnActs,alphasort);
    for(int n=0;n<done;n++){
        DirOldFiles.push_back(strdup(namelist[n]->d_name));
	free(namelist[n]);
    }
    if(done>0)free(namelist);
#endif
    sort(DirOldFiles.begin(),DirOldFiles.end(),SortFileNamesPred);

    // Read in all the ACTS file names from the NEW directory.
#ifdef __BORLANDC__
    sprintf(buf,"%s\\*.acts",DIR_New);
    done = findfirst(buf,&ffblk,0);
    while(!done){
        DirNewFiles.push_back(strdup(ffblk.ff_name));
        done = findnext(&ffblk);
    }
#elif __GNUC__
    //sprintf(buf,"%s/*.acts",DIR_New);
    done=scandir(DIR_New,&namelist,ExtnActs,alphasort);
    for(int n=0;n<done;n++){
        DirNewFiles.push_back(strdup(namelist[n]->d_name));
	free(namelist[n]);
    }
    if(done>0)free(namelist);
#endif
    sort(DirNewFiles.begin(),DirNewFiles.end(),SortFileNamesPred);

    t_LCS* lcs=LCSFileNames(DirOldFiles,DirNewFiles);

/*
    // This code prints the LCS-Table and the resulting list of file names (if used).
    PrintLCSTable(fh,lcs);

    // Print the list of file names and differences.
    for(int i=0;i<lcs->start;i++) fprintf(fh,"  %s\n",DirOldFiles[i]);
    PrintLCSDiff(fh,lcs,DirOldFiles,DirNewFiles,lcs->NumRow,lcs->NumCol,lcs->start);
    for(int i=lcs->c_end+1;i<(int)DirOldFiles.size();i++) fprintf(fh,"  %s\n",DirOldFiles[i]);
*/

    ErrorCount=0;

    // Execute the ACTS file compare on each file.  First display the initial list
    // of identical files, then the LCS of files that differ, then the final list of identical files.
    for(int i=0;i<lcs->start;i++) ExecACTSCompare(fh,DirOldFiles[i],DirOldFiles[i]);
    ExecLCSDiff(fh,lcs,DirOldFiles,DirNewFiles,lcs->NumRow,lcs->NumCol,lcs->start);
    for(int i=lcs->c_end+1;i<(int)DirOldFiles.size();i++) ExecACTSCompare(fh,DirOldFiles[i],DirOldFiles[i]);

    fprintf(fh,"------\n");
    fprintf(fh,"Total Errors=%d\n\n",ErrorCount);
    fprintf(stdout,"------\n");
    fprintf(stdout,"Total Errors=%d\n\n",ErrorCount);

    // Release all the strdup strings.
    for(int i=0;i<(int)DirOldFiles.size();i++) free(DirOldFiles[i]);
    for(int i=0;i<(int)DirNewFiles.size();i++) free(DirNewFiles[i]);

    // Clear the file name vectors.
    DirOldFiles.erase(DirOldFiles.begin(),DirOldFiles.end());
    DirNewFiles.erase(DirNewFiles.begin(),DirNewFiles.end());

    // Release the allocated table
    delete[] lcs->Table;
    lcs->Table=NULL;

}

//---------------------------------------------------------------------------
// Routine: Trim - Trim a string of whitespace and newlines at ends of string.
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
ProtoType
char* Trim(char* str)
{
    // Working from back of string forward zero out any newlines or space characters.
    char* pStr=str+strlen(str)-1;
    while(((*pStr=='\n')||isspace(*pStr))&&(pStr!=str)) *pStr--=0;
    // Now working forward do the same.
    pStr=str;
    while(isspace(*pStr)) *pStr++==0;
    // Return pointer to first non-blank character in string.
    return(pStr);
}

//---------------------------------------------------------------------------
// Routine: CalcCRC - Placeholder.  This was here to be a placeholder for a
//          CRC generated from a line of input.  The purponse of generating these
//          numbers was to allow us to do relatively fast INT compares instead
//          of strcmp() calls.
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
ProtoType
unsigned int CalcCRC(char* str)
{
    return(0);
}

//---------------------------------------------------------------------------
// Routine: BuildCRCStr - Routine is responsible for making the structure
//          and allocating a file line string to it.  Because we tolerate no
//          differences in the body of the string except for the activation
//          value which is limited by a default or user supplied parameter.
//          To accomplish this we remove the activation value to its own float
//          and replace it with a placeholde character '#' which marks where in
//          the string the activation was.
//          The CRC value was installed to allow us to do a speed up comparing
//          two CRC-32 values(fast) rather than two strings(slow).
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
ProtoType
void BuildCRCStr(t_CRCStr& CRCStr,char* str)
{
    // Find the location of period in activation floating point number.
    char* pStr=strchr(str,'.');
    // If we found it then copy out the activation.
    if(pStr!=NULL){
        // First go to the character before the period and decrement our pointer
        // until we get to character before activation.
        char* pPeriod=pStr;
        pStr--;
        while(isdigit(*pStr)) pStr--;
        // Oops, went to far ...
        pStr++;

        if(pStr==pPeriod){
            // We have no activation so just fake it (ie. GPCRoute line).
            CRCStr.Act=0.0;
        }else{
            // Now scan in the activaiton value.
            sscanf(pStr,"%f",&CRCStr.Act);

            // Insert a flag character where activation removed (for display purposes).
            *pStr++='#';

            // pStr is pointing at beginning of activation number
            char* pEndAct=pStr;
            // Find the end of the activation number.
            while(isdigit(*pEndAct)||(*pEndAct=='.')) pEndAct++;
            // Find the length of string after activation number, and ...
            int len=strlen(pEndAct);
            // Copy that string forward over the activation number so when we do
            // a CRC on the string we don't take into account the activation.  This
            // is compared separately because we are looking to ignore small errors
            // that will be displayed (ie. 0.014333 vs. 0.014332, or 0.014400 vs. 0.014399).
            for(int i=0;i<len+1;i++) pStr[i]=pEndAct[i];
        }
    }else{
        CRCStr.Act=0.0;
    }
    // Copy the string.
    CRCStr.Str=strdup(str);
    // Calculate the CRC
    CRCStr.CRC=CalcCRC(str);
}

//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------

FILE *fh_acts=stdout;

typedef struct {
    struct {
        char* fn;
        bool  got;
        int   cycles;
        float TL;
        float TO;
        float TP;
        float TGPC;
        float TGPCR;
        float TPh;
    }Old,New;
}t_acts;

t_acts ACTS_Totals;
char *ACTSDelim=" ";

//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
ProtoType
void ClearACTSTotals(void)
{
    t_acts* pt=&ACTS_Totals;
    pt->Old.fn=NULL;
    pt->Old.got=false;
    pt->Old.cycles=0;
    pt->Old.TL=0.0;
    pt->Old.TO=0.0;
    pt->Old.TP=0.0;
    pt->Old.TGPC=0.0;
    pt->Old.TGPCR=0.0;
    pt->Old.TPh=0.0;

    pt->New.fn=NULL;
    pt->New.cycles=0;
    pt->Old.got=false;
    pt->New.TL=0.0;
    pt->New.TO=0.0;
    pt->New.TP=0.0;
    pt->New.TGPC=0.0;
    pt->New.TGPCR=0.0;
    pt->New.TPh=0.0;
}

//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
ProtoType
void SaveACTSTotals(char *fn,char *str,bool Old)
{
    t_acts* pt=&ACTS_Totals;
    char *token,buf[2000];
    int   cycles;
    float value;

	if(str[0]!=';'){
		// Copy string to new buffer to protect original string and get info.
		strcpy(buf,str);
		token=strtok(buf,ACTSDelim);
		token=strtok(NULL,ACTSDelim);
		if(token!=NULL){
			cycles=atoi(token);
		}
		token=strtok(NULL,ACTSDelim);
		if(token!=NULL){
			value=atof(token);
		}

		// Save the file name pointer and number of cycles.
		if(Old){
			pt->Old.cycles=cycles;
			pt->Old.fn=fn;
			pt->Old.got=true;
		}else{
			pt->New.cycles=cycles;
			pt->New.fn=fn;
			pt->New.got=true;
		}

		// Save the cycles and
		if(token==NULL){
			// Ignore this line.
	    }else if(strcmp(token,"TL")==0){
			// Got TL line save cycles and activation
			((Old)?pt->Old.TL:pt->New.TL)=value;

		}else if(strcmp(token,"TO")==0){
			// Got TO line save cycles and activation
			((Old)?pt->Old.TO:pt->New.TO)=value;

		}else if(strcmp(token,"TP")==0){
			// Got TP line save cycles and activation
			((Old)?pt->Old.TP:pt->New.TP)=value;

		}else if(strcmp(token,"TGPC")==0){
			// Got TGPC line save cycles and activation
			((Old)?pt->Old.TGPC:pt->New.TGPC)=value;

		}else if(strcmp(token,"TGPCR")==0){
			// Got TGPCR line save cycles and activation
			((Old)?pt->Old.TGPCR:pt->New.TGPCR)=value;

		}else if(strcmp(token,"TPh")==0){
			// Got TPh line save cycles and activation
			((Old)?pt->Old.TPh:pt->New.TPh)=value;
		}
    }
}

//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
ProtoType
void PrtACTSTotalHdr(void)
{
    if(fhACTSTotals!=NULL){
        //fprintf(fhACTSTotals,"  Old Directory=%s\n  New Directory=%s\n\n",DIR_Old,DIR_New);
        fprintf(fhACTSTotals,"                Old       Old       Old       Old     Old       Old        Old        New       New       New       New     New       New        New    \n");
        fprintf(fhACTSTotals,"Filename        Cycles    TL        TO        TP      TGPC      TGPCR      TPh        Cycles    TL        TO        TP      TGPC      TGPCR      TPh    \n");
        //fprintf(fhACTSTotals,"=============== ====== ========= ========= ========= ========= ========= =========    ====== ========= ========= ========= ========= ========= =========\n");
    }
}

//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
ProtoType
void DumpACTSTotals(void)
{
    t_acts* pt=&ACTS_Totals;

    if(fhACTSTotals!=NULL){

        // Make sure the two file names are identical and that they aren't repeats.
        if((strcmp(pt->New.fn,pt->Old.fn)==0)&&(strchr(pt->Old.fn,'-')==NULL)){
            fprintf(fhACTSTotals,"%-16s",pt->Old.fn);
            fprintf(fhACTSTotals,"%6d %9.6f %9.6f %9.6f %9.6f %9.6f %9.6f",
                pt->Old.cycles,
                pt->Old.TL,
                pt->Old.TO,
                pt->Old.TP,
                pt->Old.TGPC,
                pt->Old.TGPCR,
                pt->Old.TPh
            );
            fprintf(fhACTSTotals,"    ");

            fprintf(fhACTSTotals,"%6d %9.6f %9.6f %9.6f %9.6f %9.6f %9.6f",
                pt->New.cycles,
                pt->New.TL,
                pt->New.TO,
                pt->New.TP,
                pt->New.TGPC,
                pt->New.TGPCR,
                pt->New.TPh
            );
            fprintf(fhACTSTotals,"\n");
        }
    }
}



//---------------------------------------------------------------------------
// Routine: CompareACTS - This routine is the root routine to compare two ACTS
//          files it will take care of opening and reading in the two files into
//          local vectors then calling the routines to generate a LCS-Table and
//          the 'diff' output for the files.  Finally it will clean up any
//          allocated memory.
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
ProtoType
void CompareACTS(FILE* fh,char* OldName,char* NewName)
{
    // Read in DIR_Old/OldName file into vector, possibly discarding comments.
    // Read in DIR_Old/OldName file into vector, possibly discarding comments.
    // Do LCS on both vectors, using special comparison that ignores errs<Param.
    // Use t_LCS structure to print out the file differences found.
    // Release the t_LCS structure and empty vectors.

    vector<t_CRCStr> OldACTSFile;
    vector<t_CRCStr> NewACTSFile;
    int done;
    char buf[MAXPATH];
    FILE* fhOld=NULL;
    FILE* fhNew=NULL;


    // Clear the ACTS file string arrays.
    OldACTSFile.erase(OldACTSFile.begin(),OldACTSFile.end());
    NewACTSFile.erase(NewACTSFile.begin(),NewACTSFile.end());

    // Open the ACTS file in the OLD directory first.
#ifdef __BORLANDC__
    sprintf(buf,"%s\\%s",DIR_Old,OldName);
#elif __GNUC__
    sprintf(buf,"%s/%s",DIR_Old,OldName);
#endif
    if((fhOld=fopen(buf,"r"))==NULL){
        perror("ERROR: Opening Old ACTS file.");
        return;
    }

    // Open the ACTS file in the NEW directory next.
#ifdef __BORLANDC__
    sprintf(buf,"%s\\%s",DIR_New,NewName);
#elif __GNUC__
    sprintf(buf,"%s/%s",DIR_New,NewName);
#endif
    if((fhNew=fopen(buf,"r"))==NULL){
        perror("ERROR: Opening New ACTS file.");
        return;
    }

    // Prepare to save our ACTS total information
    ClearACTSTotals();

    // Read in the strings from the OLD file, calculate the CRCs and store them.
    while(fgets(buf,MAXPATH,fhOld)!=NULL){
        char* pStr=Trim(buf);
        SaveACTSTotals(OldName,pStr,true);
        if(((pStr[0]==COMMENT)&&FLG_CompComm)||(pStr[0]!=COMMENT)){
            t_CRCStr CRCStr;
            BuildCRCStr(CRCStr,pStr);
            OldACTSFile.push_back(CRCStr);
        }
    }

    // Read in the strings from the NEW file, calculate the CRCs and store them.
    while(fgets(buf,MAXPATH,fhNew)!=NULL){
        char* pStr=Trim(buf);
        SaveACTSTotals(NewName,pStr,false);
        if(((pStr[0]==COMMENT)&&FLG_CompComm)||(pStr[0]!=COMMENT)){
            t_CRCStr CRCStr;
            BuildCRCStr(CRCStr,pStr);
            NewACTSFile.push_back(CRCStr);
        }
    }

    // Pump out ACTS total information to file.
    DumpACTSTotals();

    // Generate the LCS-Table.
    t_LCS* lcs=LCSFileLines(OldACTSFile,NewACTSFile,CmpCRCStr);

    // Display a file groupt title and the 'diff' listing.
    if((lcs->NumRow!=0)||(lcs->NumCol!=0)){
        fprintf(fh,"    Comparing:\n");
#ifdef __BORLANDC__
        fprintf(fh,"        NEW: %s\\%s (%d)\n",DIR_New,NewName,NewACTSFile.size());
        fprintf(fh,"        OLD: %s\\%s (%d)\n",DIR_Old,OldName,OldACTSFile.size());
#elif __GNUC__
        fprintf(fh,"        NEW: %s/%s (%d)\n",DIR_New,NewName,NewACTSFile.size());
        fprintf(fh,"        OLD: %s/%s (%d)\n",DIR_Old,OldName,OldACTSFile.size());
#endif
        fprintf(fh,"        ---\n");

        // Display the 'diff' listing.
        PrintACTSDiff(fh,lcs,OldACTSFile,NewACTSFile,lcs->NumRow,lcs->NumCol,lcs->start,CmpCRCStr);
    }

    // Release the allocated table
    delete[] lcs->Table;
    lcs->Table=NULL;

    // Close the two open files.
    fclose(fhOld);
    fclose(fhNew);

    // Release all the strdup strings.
    for(int i=0;i<(int)OldACTSFile.size();i++) free(OldACTSFile[i].Str);
    for(int i=0;i<(int)NewACTSFile.size();i++) free(NewACTSFile[i].Str);

    // Clear the ACTS file string arrays.
    OldACTSFile.erase(OldACTSFile.begin(),OldACTSFile.end());
    NewACTSFile.erase(NewACTSFile.begin(),NewACTSFile.end());

}

//---------------------------------------------------------------------------
// Routine: PrintACTSDiff - Run through the LCS-Table from the end to the start
//          reporting on any file lines that are different from one file to the other.
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
ProtoType
void PrintACTSDiff(FILE* fh,t_LCS* lcs,vector<t_CRCStr> &Old,vector<t_CRCStr> &New,
                   int i,int j,int start,bool (*CmpFnc)(t_CRCStr&,t_CRCStr&))
{
    static bool FLG_Divided=false;

    if((i>0)&&(j>0)&&(CmpFnc(Old[i+start-1],New[j+start-1]))){
        // If we got a match this isn't very interesting for us so just print
        // out a spacer line once between successive errors.
        PrintACTSDiff(fh,lcs,Old,New,i-1,j-1,start,CmpFnc);
        if(!FLG_Divided) fprintf(fh,"        ---\n");
        FLG_Divided=true;
    }else{
        if((j>0)&&
           ((i==0)||
            (lcs->Table[(i)*(lcs->NumCol+1)+(j-1)] >=
             lcs->Table[(i-1)*(lcs->NumCol+1)+(j)]))){

            // Recurse back up the list.
            PrintACTSDiff(fh,lcs,Old,New,i,j-1,start,CmpFnc);
            // Display the ACTS file line with/without Activation values depending
            // on whether the original line had and activation value in it.
            if((strchr(New[j+start-1].Str,'#')==NULL)||(New[j+start-1].Act==0.0)){
                fprintf(fh,"        New: '%s' (%d)\n",
                    New[j+start-1].Str,
                    j+start-1);
            }else{
                fprintf(fh,"        New: '%s'  #=%8.6f (%d)\n",
                    New[j+start-1].Str,
                    New[j+start-1].Act,
                    j+start-1);
            }
            // Increment the error count after intervening good lines.
            if(FLG_Divided) ErrorCount++;
            FLG_Divided=false;

        }else if((i>0)&&
           ((j==0)||
            (lcs->Table[(i)*(lcs->NumCol+1)+(j-1)] <
             lcs->Table[(i-1)*(lcs->NumCol+1)+(j)]))){

            // Recurse back up the list.
            PrintACTSDiff(fh,lcs,Old,New,i-1,j,start,CmpFnc);
            // Display the ACTS file line with/without Activation values depending
            // on whether the original line had and activation value in it.
            if(strchr(Old[i+start-1].Str,'#')==NULL){
                fprintf(fh,"        Old: '%s' (%d)\n",
                    Old[i+start-1].Str,
                    i+start-1);
            }else{
                fprintf(fh,"        Old: '%s'  #=%8.6f (%d)\n",
                    Old[i+start-1].Str,
                    Old[i+start-1].Act,
                    i+start-1);
            }
            // Increment the error count after intervening good lines.
            if(FLG_Divided) ErrorCount++;
            FLG_Divided=false;
        }
    }
}



//---------------------------------------------------------------------------
// Routine: CmpCRCStr - Callback routine that flags lines that are not similar
//          up to activations that fall within the ErrThreshold of one another.
//          The error threshold is larger for Total lines by a factor of ErrTtlMult.
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
ProtoType
bool CmpCRCStr(t_CRCStr& Old,t_CRCStr& New)
{
    bool rtn=true;
    if(strcmp(Old.Str,New.Str)!=0) {
        // If the string is different then the lines are different.
        rtn=false;
        
    }else if(fabs(Old.Act - New.Act)>ErrThreshold) {
        // Activation are different.

        // Test the file line string to see if it is a total line.
        char buf[105];
        strncpy(buf,Old.Str,100);
        if((strncmpi(buf,"TL",2)==0) ||
           (strncmpi(buf,"TO",2)==0) ||
           (strncmpi(buf,"TP",2)==0) ||
           (strncmpi(buf,"TPh",3)==0) ||
           (strncmpi(buf,"TGPC",4)==0) ||
           (strncmpi(buf,"TGPCR",5)==0)){

            // If it is a total line then be a little looser in error sensitivity.
            if(fabs(Old.Act - New.Act)>(ErrTtlMult*ErrThreshold)){
                rtn=false;
            }
        }else{
            // Non-total lines are governed by the raw ErrThreshold.
            rtn=false;
        }

    }
    return(rtn);
}


//---------------------------------------------------------------------------
// Routine: LCSFileLines - Run through the two ACTS file line vectors and
//          determine which are unmatched by producing a LCS-table.  This will
//          be used later to display the faulty lines.
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
ProtoType
t_LCS* LCSFileLines(vector<t_CRCStr> &Old,vector<t_CRCStr> &New,bool (*CmpFnc)(t_CRCStr&,t_CRCStr&))
{
    int start = 0;
    int c_end = Old.size()-1;
    int r_end = New.size()-1;
    static t_LCS lcs;

    // Trim off the matching lines at the beginning of these two files. 'start'
    // will be the index of the first different record in each vector.
    while((start<=r_end)&&(start<=c_end)){
        if(!CmpFnc(Old[start],New[start])) break;
        start++;
    }

    // Trim off the matching lines at the end. 'r_end' and 'c_end' will be the
    // last different element in the Old and New vector respectively.
    while((start<=r_end)&&(start<=c_end)){
        if(!CmpFnc(Old[c_end],New[r_end])) break;
        r_end--;
        c_end--;
    }

    // Do a maximum of 1000 lines of comparison.
    c_end = min(start+MaxCmpLines,c_end);
    r_end = min(start+MaxCmpLines,r_end);

    // Save information in structure for return to the caller.
    lcs.start=start;
    lcs.r_end=r_end;
    lcs.c_end=c_end;
    lcs.NumCol=(r_end-start+1); // Number of 'active' rows and columns ...
    lcs.NumRow=(c_end-start+1); // ... there are also a zero row and column at (0,0).
    lcs.Table=NULL;

    // Allocate our LCS-Table.  We have to include another row and column for
    // the zero first row and col.  Clean out the array.
    int* Table = new int[(lcs.NumRow+1)*(lcs.NumCol+1)];
    memset(Table,0,(lcs.NumRow+1)*(lcs.NumCol+1)*sizeof(int));
    lcs.Table=Table;

    // If there are lines left then we create the entries for the table.
    if((lcs.NumRow+lcs.NumCol)!=0){
        // This is the heart of the LCS algorithm (check out the Wikipedia page).

        // Only loop over the items that have changed start ... {r_end,c_end}
        for(int i=1; i<=lcs.NumRow; i++){
            for(int j=1; j<=lcs.NumCol; j++){
                if(CmpFnc(Old[i+start-1],New[j+start-1])){
                    *INDEX(Table,i,j,lcs.NumCol+1) = *INDEX(Table,i-1,j-1,lcs.NumCol+1) + 1;
                }else{
                    *INDEX(Table,i,j,lcs.NumCol+1) = max(*INDEX(Table,i,j-1,lcs.NumCol+1), *INDEX(Table,i-1,j,lcs.NumCol+1));
                }
            }
        }
    }
    return(&lcs);
}



//---------------------------------------------------------------------------
// Routine: ExecACTSCompare - This is the routine that handles the missing ACTS
//          file error messages and handing a good set of files to  CompareACTS.
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
ProtoType
void ExecACTSCompare(FILE* fh,char* OldName,char* NewName)
{
    if((OldName==NULL)&&(NewName==NULL)){
        fprintf(fh,"NOTE: ExecACTSCompare() given two NULL strings\n");
    }else if(OldName==NULL){
        fprintf(stdout,"  -N %s\n",NewName);
        fprintf(fh,"    ERROR: File '%s' does NOT exist in OLD directory!\n",NewName);
        ErrorCount++;
    }else if(NewName==NULL){
        fprintf(stdout,"  O- %s\n",OldName);
        fprintf(fh,"    ERROR: File '%s' does NOT exist in NEW directory!\n",OldName);
        ErrorCount++;
    }else{
        fprintf(stdout,"  ON %s\n",NewName);
        CompareACTS(fh,OldName,NewName);
    }
}

//---------------------------------------------------------------------------
// Routine: ExecLCSDiff - This file looks for a difference in the two lists of
//          ACTS files in the Old and New directories.  It must be given a
//          pre-created LCS-Table.
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
ProtoType
void ExecLCSDiff(FILE* fh,t_LCS* lcs,vector<char*> &Old,vector<char*> &New, int i,int j,int start)
{
    if((i>0)&&(j>0)&&(strcmp(Old[i+start-1],New[j+start-1])==0)){
        ExecLCSDiff(fh,lcs,Old,New,i-1,j-1,start);
        ExecACTSCompare(fh,Old[i+start-1],New[j+start-1]);
    }else{
        if((j>0)&&
           ((i==0)||
            (lcs->Table[(i)*(lcs->NumCol+1)+(j-1)] >=
             lcs->Table[(i-1)*(lcs->NumCol+1)+(j)]))){

            ExecLCSDiff(fh,lcs,Old,New,i,j-1,start);
            ExecACTSCompare(fh,NULL,New[j+start-1]);

        }else if((i>0)&&
           ((j==0)||
            (lcs->Table[(i)*(lcs->NumCol+1)+(j-1)] <
             lcs->Table[(i-1)*(lcs->NumCol+1)+(j)]))){

            ExecLCSDiff(fh,lcs,Old,New,i-1,j,start);
            ExecACTSCompare(fh,Old[i+start-1],NULL);

        }
    }
}

//---------------------------------------------------------------------------
// Routine: PrintLCSDiff - This routine displays a 'diff' list of ACTS file names.
//          This function is almost identical to ExecLCSDiff() above.
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
ProtoType
void PrintLCSDiff(FILE* fh,t_LCS* lcs,vector<char*> &Old,vector<char*> &New, int i,int j,int start)
{
    if((i>0)&&(j>0)&&(strcmp(Old[i+start-1],New[j+start-1])==0)){
        PrintLCSDiff(fh,lcs,Old,New,i-1,j-1,start);
        fprintf(fh,"  %s\n",Old[i+start-1]);
    }else{
        if((j>0)&&
           ((i==0)||
            (lcs->Table[(i)*(lcs->NumCol+1)+(j-1)] >=
             lcs->Table[(i-1)*(lcs->NumCol+1)+(j)]))){

            PrintLCSDiff(fh,lcs,Old,New,i,j-1,start);
            fprintf(fh,"+ %s\n",New[j+start-1]);

        }else if((i>0)&&
           ((j==0)||
            (lcs->Table[(i)*(lcs->NumCol+1)+(j-1)] <
             lcs->Table[(i-1)*(lcs->NumCol+1)+(j)]))){

            PrintLCSDiff(fh,lcs,Old,New,i-1,j,start);
            fprintf(fh,"- %s\n",Old[i+start-1]);

        }
    }
}

//---------------------------------------------------------------------------
// Routine: PrintLCS - This function will display the LCS-Table.
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
ProtoType
void PrintLCSTable(FILE* fh,t_LCS* lcs)
{
    // Some general information.
    fprintf(fh,"Active Rows=%d\n",lcs->NumRow);
    fprintf(fh,"Active Cols=%d\n",lcs->NumCol);

    // Print the header (X-axis title) of the table.
    char* Title="New Directory";
    for(int i=0;i<8+((lcs->NumCol+1)*3 - (int)strlen(Title))/2; i++) fprintf(fh," ");
    fprintf(fh,"%s\n       ",Title);
    for(int i=0;i<=lcs->NumCol;i++) fprintf(fh,"%2d ",i); fprintf(fh,"\n       ");
    for(int i=0;i<=lcs->NumCol;i++) fprintf(fh,"---",i); fprintf(fh,"\n");

    // Print the Y-axis title and the table.
    Title="Old Directory";
    int start=(lcs->NumRow - strlen(Title))/2;
    for(int r=0; r<=lcs->NumRow; r++){
        int p=r-start;
        // Print a character from the Y-axis title.
        fprintf(fh," %c %2d |",((p>=0)&&(p<(int)strlen(Title)))?Title[p]:' ',r);
        // Print a row from the table.
        for(int c=0; c<=lcs->NumCol; c++){
            fprintf(fh,"%2d ",lcs->Table[r*(lcs->NumCol+1)+c]);
        }
        fprintf(fh,"\n");
    }
}

//---------------------------------------------------------------------------
// Routine: INDEX - Helper routine to make the LCSFileNames() code a little
//          easier to read (since the LCS-Table is a linear vector disguised as
//          a 2D array.
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
ProtoType
int* INDEX(int* Table,int r,int c,int RowSz){
    // Convert the Table adress, row and column into a element address.
    int* rtn=Table + r*RowSz + c;
    return(rtn);
}


//---------------------------------------------------------------------------
// Routine: LCSFileNames - Run the LCS algorithm on the two lists of directory
//          ACTS file names.
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
ProtoType
t_LCS* LCSFileNames(vector<char*> &Old,vector<char*> &New)
{
    int start = 0;
    int c_end = Old.size()-1;
    int r_end = New.size()-1;
    static t_LCS lcs;

    // Trim off the matching items at the beginning. 'start' will be the index
    // of the first different record in each vector.
    while((start<=r_end)&&(start<=c_end)){
        if(strcmp(Old[start],New[start])!=0) break;
        start++;
    }

    // Trim off the matching items at the end. 'r_end' and 'c_end' will be the
    // last different element in the OldRow and NewCol vector respectively.
    while((start<=r_end)&&(start<=c_end)){
        if(strcmp(Old[c_end],New[r_end])!=0) break;
        r_end--;
        c_end--;
    }


    lcs.start=start;
    lcs.r_end=r_end;
    lcs.c_end=c_end;
    lcs.NumCol=(r_end-start+1); // Number of 'active' rows and columns
    lcs.NumRow=(c_end-start+1);
    lcs.Table=NULL;

    // We have to include another row and column for the zero first row and col.
    int* Table = new int[(lcs.NumRow+1)*(lcs.NumCol+1)];
    memset(Table,0,(lcs.NumRow+1)*(lcs.NumCol+1)*sizeof(int));
    lcs.Table=Table;

    // Create the LCS-Table if we actually have some differences in the lists.
    if((lcs.NumRow+lcs.NumCol)!=0){

        // Only loop over the items that have changed
        for(int i=1; i<=lcs.NumRow; i++){
            for(int j=1; j<=lcs.NumCol; j++){
                if(strcmp(Old[i+start-1],New[j+start-1])==0){
                    *INDEX(Table,i,j,lcs.NumCol+1) = *INDEX(Table,i-1,j-1,lcs.NumCol+1) + 1;
                }else{
                    *INDEX(Table,i,j,lcs.NumCol+1) = max(*INDEX(Table,i,j-1,lcs.NumCol+1), *INDEX(Table,i-1,j,lcs.NumCol+1));
                }
            }
        }
    }
    return(&lcs);
}

//---------------------------------------------------------------------------
// Routine: ProcessRTFiles - Placeholder routine.
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------

#ifdef __GNUC__
int ExtnRT(const struct dirent *pe)
{
    int rtn=0;
    if(pe!=NULL){
        int len=strlen(pe->d_name);
        if(strncmpi(pe->d_name+len-3,".RT",3)==0){
	        rtn=1;
        }
    }
    return(rtn);
}
#endif



ProtoType
void ProcessRTFiles(FILE* fh)
{
    // Read in DIR_Old/OldName file into vector, possibly discarding comments.
    // Read in DIR_Old/OldName file into vector, possibly discarding comments.
    // Do LCS on both vectors, using string comparison.
    // Use t_LCS structure to print out the file differences found.
    // Release the t_LCS structure and empty vectors.

#ifdef __BORLANDC__
    struct ffblk ffnew;
    struct ffblk ffold;
#elif __GNUC__
	struct dirent **nlnew;
	struct dirent **nlold;
#endif

    vector<char*> OldRTFile;
    vector<char*> NewRTFile;
    int done;
    char buf[MAXPATH];
    char fn[MAXPATH];
    FILE* fhOld=NULL;
    FILE* fhNew=NULL;

    dualfpf(fh,"Processing RT files:\n");

    // Clear the RT file string arrays.
    OldRTFile.erase(OldRTFile.begin(),OldRTFile.end());
    NewRTFile.erase(NewRTFile.begin(),NewRTFile.end());

#ifdef __BORLANDC__
    // Read in the RT file name from the OLD directory.
    sprintf(fn,"%s\\*.RT",DIR_Old);
    done=findfirst(fn,&ffold,0);
    if(done<0){
        dualfpf(fh,"    Old: %s\n",fn);
        dualfpf(fh,"    WARNING: Old RT file cannot be found.\n");
        return;
    }
    fprintf(stdout,"    Old: %s\\%s\n",DIR_Old,ffold.ff_name);

    // Open the RT file in the OLD directory first.
    sprintf(buf,"%s\\%s",DIR_Old,ffold.ff_name);
#elif __GNUC__
    // Read in the RT file name from the OLD directory.
    done=scandir(DIR_Old,&nlold,ExtnRT,alphasort);
    if(done<=0){
        dualfpf(fh,"    Old: %s/*.RT\n",DIR_Old);
        dualfpf(fh,"    WARNING: Old RT file cannot be found.\n");
        return;
    }
    fprintf(stdout,"    Old: %s/%s\n",DIR_Old,nlold[0]->d_name);

    // Open the RT file in the OLD directory first.
    sprintf(buf,"%s/%s",DIR_Old,nlold[0]->d_name);
#endif

    if((fhOld=fopen(buf,"r"))==NULL){
        dualfpf(fh,"    ERROR: Opening Old RT file.\n");
        return;
    }

#ifdef __BORLANDC__
    // Read in the RT file name from the NEW directory.
    sprintf(fn,"%s\\*.RT",DIR_New);
    done=findfirst(fn,&ffnew,0);
    if(done<0){
        dualfpf(fh,"    New: %s\n",fn);
        dualfpf(fh,"    WARNING: New RT file cannot be found.\n");
        return;
    }

    fprintf(stdout,"    New: %s\\%s\n",DIR_New,ffnew.ff_name);

    // Open the RT file in the NEW directory next.
    sprintf(buf,"%s\\%s",DIR_New,ffnew.ff_name);
#elif __GNUC__
    // Read in the RT file name from the NEW directory.
    done=scandir(DIR_New,&nlnew,ExtnRT,alphasort);
    if(done<=0){
        dualfpf(fh,"    New: %s/*.RT\n",DIR_New);
        dualfpf(fh,"    WARNING: New RT file cannot be found.\n");
        return;
    }

    fprintf(stdout,"    New: %s/%s\n",DIR_New,nlnew[0]->d_name);

    // Open the RT file in the NEW directory next.
    sprintf(buf,"%s/%s",DIR_New,nlnew[0]->d_name);
#endif

    if((fhNew=fopen(buf,"r"))==NULL){
        dualfpf(fh,"    ERROR: Opening New RT file.\n");
        return;
    }

    // Read in the strings from the OLD file and store them.
    while(fgets(buf,MAXPATH,fhOld)!=NULL){
        char* pStr=Trim(buf);
        if(((pStr[0]==COMMENT)&&FLG_CompComm)||(pStr[0]!=COMMENT)){
            OldRTFile.push_back(strdup(buf));
        }
    }

    // Read in the strings from the NEW file and store them.
    while(fgets(buf,MAXPATH,fhNew)!=NULL){
        char* pStr=Trim(buf);
        if(((pStr[0]==COMMENT)&&FLG_CompComm)||(pStr[0]!=COMMENT)){
            NewRTFile.push_back(strdup(buf));
        }
    }

    // Generate the LCS-Table.
    t_LCS* lcs=LCSStrLines(OldRTFile,NewRTFile,CmpStr);

    // Display a file groupt title and the 'diff' listing.
    fprintf(fh,"    Comparing:\n");
#ifdef __BORLANDC__
    fprintf(fh,"        NEW: %s\\%s (%d)\n",DIR_New,ffnew.ff_name,NewRTFile.size());
    fprintf(fh,"        OLD: %s\\%s (%d)\n",DIR_Old,ffold.ff_name,OldRTFile.size());
#elif __GNUC__
    fprintf(fh,"        NEW: %s/%s (%d)\n",DIR_New,nlnew[0]->d_name,NewRTFile.size());
    fprintf(fh,"        OLD: %s/%s (%d)\n",DIR_Old,nlold[0]->d_name,OldRTFile.size());
#endif
    if((lcs->NumRow!=0)||(lcs->NumCol!=0)){
        fprintf(fh,"        ---\n");

        // Display the 'diff' listing.
        PrintFileDiff(fh,lcs,OldRTFile,NewRTFile,lcs->NumRow,lcs->NumCol,lcs->start,CmpStr);
    }else{
        fprintf(fh,"        Identical!\n");
    }

    // Release the allocated table
    delete[] lcs->Table;
    lcs->Table=NULL;

    // Close the two open files.
    fclose(fhOld);
    fclose(fhNew);

    // Release all the strdup strings.
    for(int i=0;i<(int)OldRTFile.size();i++) free(OldRTFile[i]);
    for(int i=0;i<(int)NewRTFile.size();i++) free(NewRTFile[i]);

    // Clear the RT file string arrays.
    OldRTFile.erase(OldRTFile.begin(),OldRTFile.end());
    NewRTFile.erase(NewRTFile.begin(),NewRTFile.end());

}

//---------------------------------------------------------------------------
// Routine: PrintACTSDiff - Run through the LCS-Table from the end to the start
//          reporting on any file lines that are different from one file to the other.
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
ProtoType
void PrintFileDiff(FILE* fh,t_LCS* lcs,vector<char*> &Old,vector<char*> &New,
                   int i,int j,int start,bool (*CmpFnc)(char*,char*))
{
    static bool FLG_Divided=false;

    if((i>0)&&(j>0)&&(CmpFnc(Old[i+start-1],New[j+start-1]))){
        // If we got a match this isn't very interesting for us so just print
        // out a spacer line once between successive errors.
        PrintFileDiff(fh,lcs,Old,New,i-1,j-1,start,CmpFnc);
        if(!FLG_Divided) fprintf(fh,"        ---\n");
        FLG_Divided=true;
    }else{
        if((j>0)&&
           ((i==0)||
            (lcs->Table[(i)*(lcs->NumCol+1)+(j-1)] >=
             lcs->Table[(i-1)*(lcs->NumCol+1)+(j)]))){

            // Recurse back up the list.
            PrintFileDiff(fh,lcs,Old,New,i,j-1,start,CmpFnc);
            // Display the RT file line
            fprintf(fh,"        New: '%s' (%d)\n",
                New[j+start-1],
                j+start-1);
            // Increment the error count after intervening good lines.
            if(FLG_Divided) ErrorCount++;
            FLG_Divided=false;

        }else if((i>0)&&
           ((j==0)||
            (lcs->Table[(i)*(lcs->NumCol+1)+(j-1)] <
             lcs->Table[(i-1)*(lcs->NumCol+1)+(j)]))){

            // Recurse back up the list.
            PrintFileDiff(fh,lcs,Old,New,i-1,j,start,CmpFnc);
            // Display the ACTS file line
            fprintf(fh,"        Old: '%s' (%d)\n",
                Old[i+start-1],
                i+start-1);
            // Increment the error count after intervening good lines.
            if(FLG_Divided) ErrorCount++;
            FLG_Divided=false;
        }
    }
}

//---------------------------------------------------------------------------
// Routine: LCSFileLines - Run through the two ACTS file line vectors and
//          determine which are unmatched by producing a LCS-table.  This will
//          be used later to display the faulty lines.
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
ProtoType
t_LCS* LCSStrLines(vector<char*> &Old,vector<char*> &New,bool (*CmpFnc)(char*,char*))
{
    int start = 0;
    int c_end = Old.size()-1;
    int r_end = New.size()-1;
    static t_LCS lcs;

    // Trim off the matching lines at the beginning of these two files. 'start'
    // will be the index of the first different record in each vector.
    while((start<=r_end)&&(start<=c_end)){
        if(!CmpFnc(Old[start],New[start])) break;
        start++;
    }

    // Trim off the matching lines at the end. 'r_end' and 'c_end' will be the
    // last different element in the Old and New vector respectively.
    while((start<=r_end)&&(start<=c_end)){
        if(!CmpFnc(Old[c_end],New[r_end])) break;
        r_end--;
        c_end--;
    }

    // Save information in structure for return to the caller.
    lcs.start=start;
    lcs.r_end=r_end;
    lcs.c_end=c_end;
    lcs.NumCol=(r_end-start+1); // Number of 'active' rows and columns ...
    lcs.NumRow=(c_end-start+1); // ... there are also a zero row and column at (0,0).
    lcs.Table=NULL;

    // Allocate our LCS-Table.  We have to include another row and column for
    // the zero first row and col.  Clean out the array.
    int* Table = new int[(lcs.NumRow+1)*(lcs.NumCol+1)];
    memset(Table,0,(lcs.NumRow+1)*(lcs.NumCol+1)*sizeof(int));
    lcs.Table=Table;

    // If there are lines left then we create the entries for the table.
    if((lcs.NumRow+lcs.NumCol)!=0){
        // This is the heart of the LCS algorithm (check out the Wikipedia page).

        // Only loop over the items that have changed start ... {r_end,c_end}
        for(int i=1; i<=lcs.NumRow; i++){
            for(int j=1; j<=lcs.NumCol; j++){
                if(CmpFnc(Old[i+start-1],New[j+start-1])){
                    *INDEX(Table,i,j,lcs.NumCol+1) = *INDEX(Table,i-1,j-1,lcs.NumCol+1) + 1;
                }else{
                    *INDEX(Table,i,j,lcs.NumCol+1) = max(*INDEX(Table,i,j-1,lcs.NumCol+1), *INDEX(Table,i-1,j,lcs.NumCol+1));
                }
            }
        }
    }
    return(&lcs);
}


//---------------------------------------------------------------------------
// Routine: CmpStr - Callback routine that flags lines that are not similar
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
ProtoType
bool CmpStr(char* Old,char* New)
{
    bool rtn=true;
    if(strcmp(Old,New)!=0) {
        // If the string is different then the lines are not the same.
        rtn=false;
    }
    return(rtn);
}


//---------------------------------------------------------------------------
// Routine: ProcessSTATSFiles - Placeholder routine.
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
#ifdef __GNUC__
int ExtnStats(const struct dirent *pe)
{
    int rtn=0;
    if(pe!=NULL){
        int len=strlen(pe->d_name);
        if(strncmpi(pe->d_name+len-6,".stats",6)==0){
	    rtn=1;
        }
    }
    return(rtn);
}
#endif


ProtoType
void ProcessSTATSFiles(FILE* fh)
{
    // Read in DIR_Old/OldName file into vector, possibly discarding comments.
    // Read in DIR_Old/OldName file into vector, possibly discarding comments.
    // Do LCS on both vectors, using string comparison.
    // Use t_LCS structure to print out the file differences found.
    // Release the t_LCS structure and empty vectors.

#ifdef __BORLANDC__
    struct ffblk ffnew;
    struct ffblk ffold;
#elif __GNUC__
    struct dirent **nlnew;
    struct dirent **nlold;
#endif
    vector<char*> OldSTATSFile;
    vector<char*> NewSTATSFile;
    int done;
    char buf[MAXPATH];
    char fn[MAXPATH];
    FILE* fhOld=NULL;
    FILE* fhNew=NULL;

    dualfpf(fh,"Processing STATS files:\n");

    // Clear the STATS file string arrays.
    OldSTATSFile.erase(OldSTATSFile.begin(),OldSTATSFile.end());
    NewSTATSFile.erase(NewSTATSFile.begin(),NewSTATSFile.end());

#ifdef __BORLANDC__
    // Read in the STATS file name from the OLD directory.
    sprintf(fn,"%s\\*.stats",DIR_Old);
    done=findfirst(fn,&ffold,0);
    if(done<0){
        dualfpf(fh,"    Old: %s\\*.stats\n",DIR_Old);
        dualfpf(fh,"    WARNING: Old STATS file cannot be found.\n");
        return;
    }
    fprintf(stdout,"    Old: %s\\%s\n",DIR_Old,ffold.ff_name);

    // Open the STATS file in the OLD directory first.
    sprintf(buf,"%s\\%s",DIR_Old,ffold.ff_name);
#elif __GNUC__
    // Read in the STATS file name from the OLD directory.
    done=scandir(DIR_Old,&nlold,ExtnStats,alphasort);
    if(done==-1){
        dualfpf(fh,"    Old: %s/*.stats\n",DIR_Old);
        dualfpf(fh,"    WARNING: Old STATS file cannot be found.\n");
        return;
    }
    fprintf(stdout,"    Old: %s/%s\n",DIR_Old,nlold[0]->d_name);

    // Open the STATS file in the OLD directory first.
    sprintf(buf,"%s/%s",DIR_Old,nlold[0]->d_name);
#endif

    if((fhOld=fopen(buf,"r"))==NULL){
        dualfpf(fh,"    ERROR: Opening Old STATS file");
        return;
    }

#ifdef __BORLANDC__
    // Read in the STATS file name from the NEW directory.
    sprintf(fn,"%s\\*.stats",DIR_New);
    done=findfirst(fn,&ffnew,0);
    if(done<0){
        dualfpf(fh,"    New: %s\\*.stats\n",DIR_New);
        dualfpf(fh,"    WARNING: New STATS file cannot be found.\n");
        return;
    }

    fprintf(stdout,"    New: %s\\%s\n",DIR_New,ffnew.ff_name);

    // Open the STATS file in the NEW directory next.
    sprintf(buf,"%s\\%s",DIR_New,ffnew.ff_name);
#elif __GNUC__
    // Read in the STATS file name from the NEW directory.
    done=scandir(DIR_New,&nlnew,ExtnStats,alphasort);
    if(done==-1){
        dualfpf(fh,"    New: %s/*.stats\n",DIR_New);
        dualfpf(fh,"    WARNING: New STATS file cannot be found.\n");
        return;
    }

    fprintf(stdout,"    New: %s/%s\n",DIR_New,nlnew[0]->d_name);

    // Open the STATS file in the NEW directory next.
    sprintf(buf,"%s/%s",DIR_New,nlnew[0]->d_name);
#endif

    if((fhNew=fopen(buf,"r"))==NULL){
        dualfpf(fh,"    ERROR: Opening New STATS file");
        return;
    }

    // Read in the strings from the OLD file and store them.
    while(fgets(buf,MAXPATH,fhOld)!=NULL){
        char* pStr=Trim(buf);
        if(((pStr[0]==COMMENT)&&FLG_CompComm)||(pStr[0]!=COMMENT)){
            OldSTATSFile.push_back(strdup(buf));
        }
    }

    // Read in the strings from the NEW file and store them.
    while(fgets(buf,MAXPATH,fhNew)!=NULL){
        char* pStr=Trim(buf);
        if(((pStr[0]==COMMENT)&&FLG_CompComm)||(pStr[0]!=COMMENT)){
            NewSTATSFile.push_back(strdup(buf));
        }
    }

    // Generate the LCS-Table.
    t_LCS* lcs=LCSStrLines(OldSTATSFile,NewSTATSFile,CmpStr);

    // Display a file groupt title and the 'diff' listing.
    fprintf(fh,"    Comparing:\n");
#ifdef __BORLANDC__
    fprintf(fh,"        NEW: %s\\%s (%d)\n",DIR_New,ffnew.ff_name,NewSTATSFile.size());
    fprintf(fh,"        OLD: %s\\%s (%d)\n",DIR_Old,ffold.ff_name,OldSTATSFile.size());
#elif __GNUC__
    fprintf(fh,"        NEW: %s/%s (%d)\n",DIR_New,nlnew[0]->d_name,NewSTATSFile.size());
    fprintf(fh,"        OLD: %s/%s (%d)\n",DIR_Old,nlold[0]->d_name,OldSTATSFile.size());
#endif
    if((lcs->NumRow!=0)||(lcs->NumCol!=0)){
        fprintf(fh,"        ---\n");

        // Display the 'diff' listing.
        PrintFileDiff(fh,lcs,OldSTATSFile,NewSTATSFile,lcs->NumRow,lcs->NumCol,lcs->start,CmpStr);
    }else{
        fprintf(fh,"        Identical!\n");
    }

    // Release the allocated table
    delete[] lcs->Table;
    lcs->Table=NULL;

    // Close the two open files.
    fclose(fhOld);
    fclose(fhNew);

    // Release all the strdup strings.
    for(int i=0;i<(int)OldSTATSFile.size();i++) free(OldSTATSFile[i]);
    for(int i=0;i<(int)NewSTATSFile.size();i++) free(NewSTATSFile[i]);

    // Clear the RT file string arrays.
    OldSTATSFile.erase(OldSTATSFile.begin(),OldSTATSFile.end());
    NewSTATSFile.erase(NewSTATSFile.begin(),NewSTATSFile.end());
}

//---------------------------------------------------------------------------
// Routine: ProcessParamFiles - Placeholder routine.
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------

#ifdef __GNUC__
int ExtnParams(const struct dirent *pe)
{
    int rtn=0;
    if(pe!=NULL){
        if(strcmpi(pe->d_name,"parameters")==0){
	    rtn=1;
        }
    }
    return(rtn);
}
#endif

ProtoType
void ProcessParamFiles(FILE* fh)
{
    // Read in DIR_Old/OldName file into vector, possibly discarding comments.
    // Read in DIR_Old/OldName file into vector, possibly discarding comments.
    // Do LCS on both vectors, using string comparison.
    // Use t_LCS structure to print out the file differences found.
    // Release the t_LCS structure and empty vectors.

#ifdef __BORLANDC__
    struct ffblk ffnew;
    struct ffblk ffold;
#elif __GNUC__
    struct dirent **nlnew;
    struct dirent **nlold;
#endif
    vector<char*> OldParamFile;
    vector<char*> NewParamFile;
    int done;
    char buf[MAXPATH];
    char fn[MAXPATH];
    FILE* fhOld=NULL;
    FILE* fhNew=NULL;

    fprintf(stdout,"Processing Parameter files:\n");
    fprintf(fh,"Processing Parameter files:\n");

    // Clear the Param file string arrays.
    OldParamFile.erase(OldParamFile.begin(),OldParamFile.end());
    NewParamFile.erase(NewParamFile.begin(),NewParamFile.end());

#ifdef __BORLANDC__
    // Read in the Param file name from the OLD directory.
    sprintf(fn,"%s\\parameters",DIR_Old);
    done=findfirst(fn,&ffold,0);
    if(done<0){
        dualfpf(fh,"    Old: %s\\parameters\n",DIR_Old);
        dualfpf(fh,"    WARNING: Old Param file cannot be found.\n");
        return;
    }
    fprintf(stdout,"    Old: %s\\%s\n",DIR_Old,ffold.ff_name);

    // Open the Param file in the OLD directory first.
    sprintf(buf,"%s\\%s",DIR_Old,ffold.ff_name);
#elif __GNUC__
    // Read in the Param file name from the OLD directory.
    //sprintf(fn,"%s/parameters",DIR_Old);
    done=scandir(DIR_Old,&nlold,ExtnParams,alphasort);
    if(done<=0){
        dualfpf(fh,"    Old: %s/parameters\n",DIR_Old);
        dualfpf(fh,"    WARNING: Old Param file cannot be found.\n");
        return;
    }
    fprintf(stdout,"    Old: %s/%s\n",DIR_Old,nlold[0]->d_name);

    // Open the Param file in the OLD directory first.
    sprintf(buf,"%s/%s",DIR_Old,nlold[0]->d_name);
#endif

    if((fhOld=fopen(buf,"r"))==NULL){
        dualfpf(fh,"    ERROR: Opening Old Param file.\n");
        return;
    }

#ifdef __BORLANDC__
    // Read in the Param file name from the NEW directory.
    sprintf(fn,"%s\\parameters",DIR_New);
    done=findfirst(fn,&ffnew,0);
    if(done<0){
        dualfpf(fh,"    New: %s\\parameters\n",DIR_New);
        dualfpf(fh,"    WARNING: New Param file cannot be found.\n");
        return;
    }
    fprintf(stdout,"    New: %s\\%s\n",DIR_New,ffnew.ff_name);

    // Open the Param file in the NEW directory next.
    sprintf(buf,"%s\\%s",DIR_New,ffnew.ff_name);
#elif __GNUC__
    // Read in the Param file name from the NEW directory.
    //sprintf(fn,"%s/parameters",DIR_New);
    done=scandir(DIR_New,&nlnew,ExtnParams,alphasort);
    if(done<=0){
        dualfpf(fh,"    New: %s/parameters\n",DIR_New);
        dualfpf(fh,"    WARNING: New Param file cannot be found.\n");
        return;
    }
    fprintf(stdout,"    New: %s/%s\n",DIR_New,nlnew[0]->d_name);

    // Open the Param file in the NEW directory next.
    sprintf(buf,"%s/%s",DIR_New,nlnew[0]->d_name);
#endif

    if((fhNew=fopen(buf,"r"))==NULL){
        dualfpf(fh,"    ERROR: Opening New Param file.\n");
        return;
    }

    // Read in the strings from the OLD file and store them.
    while(fgets(buf,MAXPATH,fhOld)!=NULL){
        char* pStr=Trim(buf);
        if(((pStr[0]==COMMENT)&&FLG_CompComm)||(pStr[0]!=COMMENT)){
            OldParamFile.push_back(strdup(buf));
        }
    }

    // Read in the strings from the NEW file and store them.
    while(fgets(buf,MAXPATH,fhNew)!=NULL){
        char* pStr=Trim(buf);
        if(((pStr[0]==COMMENT)&&FLG_CompComm)||(pStr[0]!=COMMENT)){
            NewParamFile.push_back(strdup(buf));
        }
    }

    // Generate the LCS-Table.
    t_LCS* lcs=LCSStrLines(OldParamFile,NewParamFile,CmpStr);

    // Display a file groupt title and the 'diff' listing.
    fprintf(fh,"    Comparing:\n");
#ifdef __BORLANDC__
    fprintf(fh,"        NEW: %s\\%s (%d)\n",DIR_New,ffnew.ff_name,NewParamFile.size());
    fprintf(fh,"        OLD: %s\\%s (%d)\n",DIR_Old,ffold.ff_name,OldParamFile.size());
#elif __GNUC__
    fprintf(fh,"        NEW: %s/%s (%d)\n",DIR_New,nlnew[0]->d_name,NewParamFile.size());
    fprintf(fh,"        OLD: %s/%s (%d)\n",DIR_Old,nlold[0]->d_name,OldParamFile.size());
#endif
    if((lcs->NumRow!=0)||(lcs->NumCol!=0)){
        fprintf(fh,"        ---\n");

        // Display the 'diff' listing.
        PrintFileDiff(fh,lcs,OldParamFile,NewParamFile,lcs->NumRow,lcs->NumCol,lcs->start,CmpStr);
    }else{
        fprintf(fh,"        Identical!\n");
    }

    // Release the allocated table
    delete[] lcs->Table;
    lcs->Table=NULL;

    // Close the two open files.
    fclose(fhOld);
    fclose(fhNew);

    // Release all the strdup strings.
    for(int i=0;i<(int)OldParamFile.size();i++) free(OldParamFile[i]);
    for(int i=0;i<(int)NewParamFile.size();i++) free(NewParamFile[i]);

    // Clear the RT file string arrays.
    OldParamFile.erase(OldParamFile.begin(),OldParamFile.end());
    NewParamFile.erase(NewParamFile.begin(),NewParamFile.end());
}

//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
ProtoType
int dualfpf(FILE*fh,char *fmt, ...)
{
   va_list argptr;
   int cnt;

   va_start(argptr, fmt);
   cnt = vfprintf(stdout, fmt, argptr);
   cnt = vfprintf(fh, fmt, argptr);
   va_end(argptr);

   return(cnt);
}


//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
#pragma argsused
int main(int argc, char* argv[])
{
    ProcessCmdLineArgs(argc,argv);

    // Tell the user where the output is going.
    fprintf(stdout,"Output file='%s'\n",FN_OutputFile);

    // Open the output file and ...
    FILE* fh=stdout;
    if((fh=fopen(FN_OutputFile,"w"))==NULL){
        perror("ERROR: Output file");
        exit(1);
    }else{
        fhACTSTotals=fopen("DRCTotals.txt","w");
        PrtACTSTotalHdr();

        // Display a title block.
        // NOTE: Might want to convert the directories to absolute values.
        fprintf(fh,"\n\n");
        fprintf(fh,"DRCTest comparison of directories:\n");
        fprintf(fh,"  Old: '%s'\n",DIR_Old);
        fprintf(fh,"  New: '%s'\n",DIR_New);
        fprintf(fh,"  Error Threshold(A):          %10.8f\n",ErrThreshold);
        fprintf(fh,"  Error Totals Multiplier(B):       %5.2f\n",ErrTtlMult);
        fprintf(fh,"  Error Totals Threshold(A*B): %10.8f\n",ErrTtlMult*ErrThreshold);
        fprintf(fh,"\n\n");

        // Display any differences in the RT file(s).
        if(FLG_RTFiles){
            ProcessRTFiles(fh);
        }

        // Display any differences in the STATS file(s).
        if(FLG_STATSFiles){
            ProcessSTATSFiles(fh);
        }

        // Display any differences in the Parameter file(s).
        if(FLG_ParamFiles){
            ProcessParamFiles(fh);
        }

        // Display any differences in the ACTS file(s).
        if(FLG_ACTSFiles){
            fprintf(stdout,"\n");
            fprintf(stdout,"Lctn Filename\n");
            fprintf(stdout,"---- ---------------\n");
            ProcessACTSFiles(fh);
        }

        fprintf(fh,"\n\n");
        fclose(fhACTSTotals);
    }

    if(fh!=stdout) fclose(fh);
    return 0;
}

//---------------------------------------------------------------------------




