//---------------------------------------------------------------------------
// $Header: D:\\Repository\\D\\Dev\\Psych499\\DRC_Main.cpp,v 1.25 2011-03-14 17:04:07-04 alan Exp alan $
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
// Main program for DRC (Dual Route Cascaded model).
//
// $Log: DRC_Main.cpp,v $
// Revision 1.25  2011-03-14 17:04:07-04  alan
// Modified to allow compilation with GNUC.
// Fixed comment on #endif line
// Fixed format for debug.h include.
// Corrected version number to 1.0 from 1.2.
// Corrected missing type for static definition of FLG_DatabaseLoaded in Load_Database().
//
// Revision 1.0  2011-03-13 23:18:01-04  alan
// Initial revision
//
// Revision 1.24  2011-02-18 15:52:58-05  alan
// Updated files with license information.
//
// Revision 1.23  2011-01-25 14:26:10-05  alan
// Changed all the fprintf() calls to pfprintf() calls (protected against file handle being NULL)
//
// Revision 1.22  2011-01-16 20:37:00-05  alan
// Added pfputc() function as a complement to the pfprintf() funtion. (Protected from fh=null)
//
// Revision 1.21  2011-01-12 22:43:46-05  alan
// Corrected the atob() routine for false values.
//
// Revision 1.20  2010-12-11 10:34:53-05  alan
// Temp write of vocabulary file so I can check calculations of POL E/I.
//
// Revision 1.19  2010-12-05 22:29:21-05  alan
// Converting plain floats to DRC_Floats.
//
// Revision 1.18  2010-11-24 10:38:11-05  alan
// Corrected a title of program.
//
// Revision 1.17  2010-11-03 17:42:56-04  alan
// Changed the name of the hdrstop.h file (to DRC_HdrStop.h) to distinguish it from the one
// used for DRCTest.
//
// Revision 1.16  2010-10-20 09:26:14-04  alan
// Added variable to hold the actual display version of the program.
// Added comments.
// Fixed the order of the database file loading in Load_Database() and added a load once
// test so any routine that needs the database can call this routine to make sure the DB
// is read in and ready.
// Added an Init() routine to do program level initialization (in particular the output parent
// directory is initiailized).
// Added a protected pfprintf() function that tests its FILE* argument for NULL before using.
// Changed the processing of command line arguments to be a one-pass process.
//
// Revision 1.15  2010-09-27 17:50:55-04  alan
// <>
//
// Revision 1.14  2010-08-31 17:33:42-04  alan
// Added code to test the batch file only without running it.
// Added code to load the database only if we are going to use it.
// Added atob() function to test for boolean paramter values.
//
// Revision 1.13  2010-07-20 20:31:49-04  alan
// Added facilities to print each modules RCS version number out.
// Added subroutine to check to see if two strings are the same except for case.
// Added routine to process commands from the Batch.y parser.  This allows me to build
// the command in a union and then pass the union around.  When doing it the process
// in place way there was the problem of execution on error.
// Re-arrange the mainline so that the vocabulary files were read in before the command-
// line arguments were processed (which might change the data structures of the
// language files).
//
// Revision 1.12  2010-07-16 22:59:07-04  alan
// Added RCS keywords.
// Added PrtVersion() routine to print all the systems version information out.
//
// Revision 1.11  2010-07-11 13:24:07-04  alan
// <>
//
// Revision 1.10  2010-07-06 10:03:36-04  alan
// Minor change.
//
// Revision 1.9  2010-06-25 23:04:59-04  alan
// Changed to a new word for testing purposes.
//
// Revision 1.8  2010-06-15 15:39:19-04  alan
// Minor change after installing Clean-Data-Structure calls in all file reading routines.
//
// Revision 1.7  2010-06-14 08:44:32-04  alan
// Minor changes.
//
// Revision 1.6  2010-06-10 20:24:03-04  alan
// Added the DBGOpen() and DBGClose() calls for the debug output.
//
// Revision 1.5  2010-06-09 11:05:06-04  alan
// Changed the test word to 'back' from 'hello' because 'hello' isn't in the vocabulary file.
//
// Revision 1.4  2010-06-06 00:12:24-04  alan
// Change header files to just one hdrstop.h
// Change main to test input words.
//
// Revision 1.3  2010-05-23 09:51:01-04  alan
// Minor fix.
//
// Revision 1.2  2010-05-20 16:18:00-04  alan
// Added some comments/RCS keywords.
// Added calls to periphial I/O routines.
//
//---------------------------------------------------------------------------

#include "DRC_HdrStop.h"

#include <stdint.h>

//#define DEBUG
#define DBGvars
#include "debug.h"


const char* DRC_Model_Version="1.0 (DRC1.2 Equivalent)";

const char* DRC_Main[]={
    "$Author: alan $",
    "$Date: 2011-03-14 17:04:07-04 $",
    "$Revision: 1.25 $",
    "$RCSfile: DRC_Main.cpp,v $",
    "$Source: D:\\Repository\\D\\Dev\\Psych499\\DRC_Main.cpp,v $"
};

const char* DRC_CmdLine[]={
    "$Author: alan $",
    "$Date: 2011-03-14 23:56:25-04 $",
    "$Revision: 1.22 $",
    "$RCSfile: DRC_CmdLine.cpp,v $",
    "$Source: D:\\Repository\\D\\Dev\\Psych499\\DRC_CmdLine.cpp,v $"
};

const char* DRC_FileIO[]={
    "$Author: alan $",
    "$Date: 2011-03-14 17:02:30-04 $",
    "$Revision: 1.48 $",
    "$RCSfile: DRC_FileIO.cpp,v $",
    "$Source: D:\\Repository\\D\\Dev\\Psych499\\DRC_FileIO.cpp,v $"
};

const char* DRC_Core[]={
    "$Author: alan $",
    "$Date: 2011-03-14 17:00:23-04 $",
    "$Revision: 1.57 $",
    "$RCSfile: DRC_Core.cpp,v $",
    "$Source: D:\\Repository\\D\\Dev\\Psych499\\DRC_Core.cpp,v $"
};

const char* Batch_l[]={
    "$Author: alan $",
    "$Date: 2011-03-14 16:46:17-04 $",
    "$Revision: 1.13 $",
    "$RCSfile: Batch_l.cpp,v $",
    "$Source: D:\\Repository\\D\\Dev\\Psych499\\Batch_l.cpp,v $"
};

const char* Batch_y[]={
    "$Author: alan $",
    "$Date: 2011-03-14 16:46:18-04 $",
    "$Revision: 1.13 $",
    "$RCSfile: Batch_y.cpp,v $",
    "$Source: D:\\Repository\\D\\Dev\\Psych499\\Batch_y.cpp,v $"
};

const char* Generic_l[]={
    "$Author: alan $",
    "$Date: 2011-03-14 16:46:17-04 $",
    "$Revision: 1.12 $",
    "$RCSfile: Generic_l.cpp,v $",
    "$Source: D:\\Repository\\D\\Dev\\Psych499\\Generic_l.cpp,v $"
};

//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
const char* Terminate(const char* start, __attribute__((unused)) const char* fence)
{
    /* if((uintptr_t)fence<1000){ */
    /*     start[(uintptr_t)fence]=0; */
    /* }else{ */
    /*     char* ps=strstr(start,fence); */
    /*     if(ps!=NULL){ */
    /*         *ps=0; */
    /*     } */
    /* } */
    return start;
}


//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
void PrtVersion(FILE* fh)
{
    pfprintf(fh,"DRC UofW 1.2\n");
    pfprintf(fh,"Modules:\n");
    pfprintf(fh,"   %-20s %6s %20s\n",
        Terminate(DRC_Main[3]+10,",v $"),
        Terminate(DRC_Main[2]+11," $"),
        Terminate(DRC_Main[1]+7,(char*)19));
    pfprintf(fh,"   %-20s %6s %20s\n",
        Terminate(DRC_CmdLine[3]+10,",v $"),
        Terminate(DRC_CmdLine[2]+11," $"),
        Terminate(DRC_CmdLine[1]+7,(char*)19));
    pfprintf(fh,"   %-20s %6s %20s\n",
        Terminate(DRC_FileIO[3]+10,",v $"),
        Terminate(DRC_FileIO[2]+11," $"),
        Terminate(DRC_FileIO[1]+7,(char*)19));
    pfprintf(fh,"   %-20s %6s %20s\n",
        Terminate(DRC_Core[3]+10,",v $"),
        Terminate(DRC_Core[2]+11," $"),
        Terminate(DRC_Core[1]+7,(char*)19));
    pfprintf(fh,"   %-20s %6s %20s\n",
        Terminate(Batch_l[3]+10,",v $"),
        Terminate(Batch_l[2]+11," $"),
        Terminate(Batch_l[1]+7,(char*)19));
    pfprintf(fh,"   %-20s %6s %20s\n",
        Terminate(Batch_y[3]+10,",v $"),
        Terminate(Batch_y[2]+11," $"),
        Terminate(Batch_y[1]+7,(char*)19));
    pfprintf(fh,"   %-20s %6s %20s\n",
        Terminate(Generic_l[3]+10,",v $"),
        Terminate(Generic_l[2]+11," $"),
        Terminate(Generic_l[1]+7,(char*)19));
}

//------------------------------------------------------------------------------
// Routine to check to see if two strings are identical except for case.
//------------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
/* ProtoType */
/* bool MatchStr(char *str1,char *str2){ */
/*     bool rtn=false; */
/*     char buf1[500]; */
/*     char buf2[500]; */

/*     strcpy(buf1,str1); */
/*     strlwr(buf1); */
/*     strcpy(buf2,str2); */
/*     strlwr(buf2); */

/*     if(strcmp(buf1,buf2)==0){ */
/*         rtn=true; */
/*     } */
/*     return(rtn); */
/* } */

//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
bool atob(const char* str)
{
    switch (*str) {
    case 't':
    case 'T':
    case 'y':
    case 'Y':
        return true;
    default:
        return false;
    }

    /* char buf[10]; */
    /* bool rtn=false; */

    /* strncpy(buf,str,9); */
    /* strlwr(buf); */
    /* if((strcmp(buf,"true")==0)|| */
    /*    (strcmp(buf,"t")==0)|| */
    /*    (strcmp(buf,"yes")==0)|| */
    /*    (strcmp(buf,"y")==0)) */
    /* { */
    /*     rtn=true; */
    /* } */
    /* return(rtn); */
}



//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
void ProcessDRCCmd(t_DRCCmd* Cmd)
{
    if(!FindBParam("TestBatchOnly")->Value){
        // Local variables.
        char* Prm=NULL;
        DRC_Float Val=0.0;
        t_fparam* fp=NULL;

        switch(Cmd->Type){
            case CMD_Data:
                // Process a data command (i.e. {<MaxCycles>} <TestWord> {<Category>}).
                DRC_ResetSystem();
                DRC_ProcessWord(Cmd->Data.MaxCycles,Cmd->Data.TestWord,Cmd->Data.Category);
                break;

            case CMD_Param:
                // Process a set parameter command (i.e. ! <param-name> <param-value>).
                Prm=Cmd->Param.Name;
                Val=Cmd->Param.Value;

                // Search for this parameter (Prm) in out list and set the current
                // value to the given value(Val).
                if((fp=FindFParam(Prm,true))!=NULL){
                    printf("Setting parameter '%s':\n",fp->Name);
                    printf("    Old Value=%f\n",fp->Value);
                    printf("    New Value=%f\n",Val);
                    fp->Value=Val;
                }
                if(fp==NULL){
                    printf("Setting parameter '%s' to %f failed! Unknown parameter.\n",Prm,Val);
                    printf("Available parameters:\n");
                    printf("    Parameter Name                  Value\n");
                    printf("    ------------------------- -----------\n");
                    t_fparam* fp=DRC_FParams;
                    while(fp->Name!=NULL){
                        printf("    %-25s %11.6f\n",fp->Name,fp->Value);
                        fp++;
                    }
                }
                // Passing up an allocated string with a name string in it.
                if(Prm!=NULL) free(Prm);
                break;

            case CMD_Quit:
                // Stop processing the batch file.
                printf("Stopping processing of Batch file.\n");
                break;

            case CMD_Reset:
                // Reset the DRC core.
                printf("Reseting the DRC core.\n");
                DRC_ResetSystem();
                break;

            case CMD_AutoReset:
                printf("Changing AutoReset to %s.\n",(Cmd->AutoReset.Value)?"ON":"OFF");
                // Change the value of the AutoReset flag.
                break;

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
void Load_Database()
{
    static bool FLG_DatabaseLoaded=false;

    // Load the database if it isn't already.
    if(!FLG_DatabaseLoaded){

        ReadParamFile("default.parameters");
        //WriteParamFile("default.parameters.out");

        ReadPropertiesFile("properties");
        //WritePropertiesFile("properties.out");

        ReadLettersFile("letters");
        //WriteLettersFile("letters.out");

        ReadPhonemesFile("phonemes");
        //WritePhonemesFile("phonemes.out");

        ReadVocabularyFile("vocabulary");
        //WriteVocabularyFile("vocabulary.out");

        ReadGPCRulesFile("gpcrules");
        //WriteGPCRulesFile("gpcrules.out");

        FLG_DatabaseLoaded=true;
    }
}

//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
void Init()
{
    // Set the default (parent) output directory to the current variable.
    // Having this in a buffer allows the user to change it.
    strcpy(OutParentDirBuf,FindSParam("OutputDirectory")->Default);
}

//---------------------------------------------------------------------------
// Routine: This is just a protected version of the normal pfprintf().  The purpose
//          is to protect pfprintf() calls from a NULL file pointer.  We can turn
//          the output off by NULLing the pinter.
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
int pfprintf(FILE* fh,const char *fmt, ...)
{
    va_list argptr;
    int cnt=0;

    if(fh!=NULL){
        va_start(argptr, fmt);
        cnt = vfprintf(fh, fmt, argptr);
        va_end(argptr);
    }
    return(cnt);
}

void pfputc(char chr,FILE *fh)
{
    if(fh!=NULL){
        fputc(chr,fh);
    }
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
    DBGOpen("DRC.dbg");

    Init();

    ProcessCmdLineArgs(argc,argv);

    WriteVocabularyFile("vocabulary.dump.txt");

    // Clean up time.
    CleanPropertiesFile();
    CleanLettersFile();
    CleanPhonemesFile();
    CleanVocabularyFile();
    CleanParamFile();
    CleanGPCRulesFile();

    DBGClose();
    return 0;
}
//---------------------------------------------------------------------------
