//------------------------------------------------------------------------------
// $Header: D:\\Repository\\D\\Dev\\Psych499\\DRC_FileIO.cpp,v 1.48 2011-03-14 17:02:30-04 alan Exp alan $
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
// Routines that take care of File reading and writing.
//
// $Log: DRC_FileIO.cpp,v $
// Revision 1.48  2011-03-14 17:02:30-04  alan
// Modified to allow compilation with GNUC.
// Fixed comment on #endif line
// Fixed format for debug.h include.
// Included casts from BYTE* to const char* and the reverse.
// Used zero instead of NULL in if conditionals when testing against ints.
// Added code so that we don't try and create files "con.acts" and "con.drc".
// Fixed up mkdir() call and to do findfirst() function.
//
// Revision 1.0  2011-03-13 23:18:01-04  alan
// Initial revision
//
// Revision 1.47  2011-02-18 15:52:58-05  alan
// Updated files with license information.
//
// Revision 1.46  2011-01-25 14:25:12-05  alan
// Changed all the fprintf() calls to pfprintf() calls (protected against file handle being NULL)
//
// Revision 1.45  2011-01-16 20:34:36-05  alan
// Added code to display the Neighbourhood of a nonword.
//
// Revision 1.44  2010-12-11 10:31:34-05  alan
// Changed ReadParamFile() to read directly into the parameter tables.
// Changed PrtVocabLine() to display CFS values in greater precision.
// Added title to WriteVocabularyFile().
// Removed BUFFERSZ to drc.h.
//
// Revision 1.43  2010-12-05 23:27:10-05  alan
// Making calls to memset() more robust.
//
// Revision 1.42  2010-12-05 22:22:29-05  alan
// Converting all the plain floats to DRC_Floats
//
// Revision 1.41  2010-12-04 01:01:52-05  alan
// Commented out the code that appended the FLD_ENDOFWORD marker to the .Fields
// string.
//
// Revision 1.40  2010-11-26 00:07:06-05  alan
// Removing some incorrect code dealing with protected phonemes.
//
// Revision 1.39  2010-11-25 17:03:13-05  alan
// Added code to change the masks for each rule that is protected to MASK_PROTECTED
// from MASK_USED.
// Added code to increment through the rules list for both the ordinary GPC rules and the
// output rules as well.
// Added all the new start and end pointers for the GPCRules file sections.
// Added code to find the end of each section of the GPCRules.
//
// Revision 1.38  2010-11-24 22:34:41-05  alan
// Added section headers.
// Moved the GPCFieldStore variables up to top of file.
// Cleaned up code in CleanGPCRulesFile().
// Added code to output mask in WriteGPCRulesFile().
// Moved some META constants to header.
// Changed some names of meta constants and commented DRC_GPCSetField().
// Changed out magic numbers for meta constant BUFFERSZ in DisplayHomographs().
// Correcting form of some memset() calls.
//
// Revision 1.37  2010-11-24 10:39:57-05  alan
// Corrected a title of program.
// Commented out sort (and fixed the order) because the GPCRULES file is in an arbitrary
// order and sorting screws up the rules chosen in the GPC Route.
//
// Revision 1.36  2010-11-22 17:29:03-05  alan
// Changed the position of DRC_GPCSetField() to after completion of record parsing.
//
// Revision 1.35  2010-11-20 22:05:07-05  alan
// Fixing the gpcrules sort to sort simple and complete field lenghts correctly.
// Added additional sort by alphabetical order for the gpcrules.
//
// Revision 1.34  2010-11-18 21:18:50-05  alan
// Changed the GPCRules field Unknown2(char) to Protected (bool).
// Purpose still unknown.
// Installed a boundary check on the GPCRules table.
// Installed a Qsort for the GPCRules table (class, simple fields count, all fields count).
//
// Revision 1.33  2010-11-17 10:46:08-05  alan
// Added the end-of-word field to all the wpEnd GPC Rules in routine GPC_SetField().
//
// Revision 1.32  2010-11-12 23:46:58-05  alan
// Calculated a mask field for the t_gpcrules structure so we didn't have to keep
// recalculating it every time a match was found.
//
// Revision 1.31  2010-11-08 21:30:48-05  alan
// Added code to clear, set and display the t_gpcrule struct field PreContext.
// Mods to DRC_GPCSetField() to get it working properly and setting PreContext.
//
// Revision 1.30  2010-11-08 11:06:11-05  alan
// Added some comments.
// Put a test for the 'Warnings' parameter before outputing warnings about Homophonic or
// Homographic combining of corpus frequencies.
//
// Revision 1.29  2010-11-07 23:19:22-05  alan
// Got rid of some obsolete character strings.
// Added code to deal with new GPC field "Fields".
// Added code to translate the input grapheme/phoneme(clsOut) string to single character
// field codes.
//
// Revision 1.28  2010-11-03 17:42:32-04  alan
// Changed the name of the hdrstop.h file (to DRC_HdrStop.h) to distinguish it from the one
// used for DRCTest.
//
// Revision 1.27  2010-11-01 11:45:20-04  alan
// Got a little over zealous with the MINVOCAB edit so had to back some of them out
// because they were interfering with indexing the Phonemelist array.
//
// Revision 1.26  2010-10-31 23:42:40-04  alan
// Adjusted all the Vocabulary table indicies to take into account that index zero in the
// table is unused.
// Installed MINVOCAB where appropriate.
//
// Revision 1.25  2010-10-31 18:30:28-04  alan
// Continuing from previous revision notes...
// Removed routine shiftright() and removed \0 prefixing of names to indicate homophone
// or homograph removed entry (use the linked list pointers instead).
// Fixup code that reports when combined Homograph/phone entries exceed the MaxFreq.
// Added code to promote the SumFreq for all homophone/graphs and normal entries.
// Make CFS values depend on SumFreq.
// Fix code that depended on \0 prefixed names.
//
// Revision 1.24  2010-10-31 18:25:43-04  alan
// Added SumFreq to the t_vocabulary structure to keep track of the cumulative freq.
// for the homographs and homophones.
//
// Revision 1.23  2010-10-26 22:28:21-04  alan
// Output converted to depend on DEBUG and WARNINGS switches.
// Built OpenOutputFile() routine to deal with file names that can't be created under
// Windows (like con.acts)  and replaced individual file name creation with it.
//
// Revision 1.22  2010-10-25 11:22:44-04  alan
// Added the RT and STATS file generation code.
// Started adding in the Grapheme/Phoneme radix code to speed up the OIL->LL and
// POL->PB interactions.
// Reading the DRC defining paper Coltheart,et.al.2001, which states that the CFS values
// (OIL and POL) are both OIL CFS values, install switch to make it so (CFSPOLEqualIOIL).
//
// Revision 1.21  2010-10-21 22:30:02-04  alan
// Fixed up DisplayHomographs and DisplayHomophones to print out zero prefixed words
// (i.e. words that have been deactivated) as well as printing the key word from the homo-
// phoneme/grapheme linked lists.
//
// Revision 1.20  2010-10-20 20:56:34-04  alan
// Had test for SeparateHomophones/Homographs switched around.  Fixed it.
// Fixed the off by one error in calculating the CFS values for homophones.
// Extracted MaxWrittenWordFreq and MaxSpokenWordFreq to global position so that they
// could be used in Homophone and Homograph display routines.
// Fixed up DisplayHomographs() and DisplayHomophones() routines to give more infomation.
//
// Revision 1.19  2010-10-20 12:26:07-04  alan
// Added code to turn on/off separation of homographs.
// Converted dynamic allocation PhonemeList array to static allocation.
// Fixed up output for homophoneme and homographeme calculations to use pfprintf() and
// to do calculations with abs() to clean up the flagged Homophone/Homograph entries.
// Added the DisplayHomographs() and DisplayHomophones() routines as well as the
// supporting SpcStrNCmp() routine.
//
// Revision 1.18  2010-10-20 09:45:21-04  alan
// Added variable to count stress rules in GPC file (not implemented).
// Added buffers to keep track of the parent and run output directories.
// Added variable to keep track of count of GPC rule types.
// Added extra code to help debug CFSCalc() routine.
// Change the vocabulary file input routine to calculate CFS values after homographemes
// and homophonemes entries are folded (combined).
// Added code to ReadBatchFile() to create the output run directory name.
// Added CreateOutDir() routine to change the base run directory name to a
// <basename>-<index>.drc name.
//
// Revision 1.17  2010-08-31 17:02:25-04  alan
// Added subroutine comments.
// Added new variables for GPC file reading and GPC route processing.
// Changed the reading of a GPCRules class column into a enum instead of a string.
// Changed the Unknown1 field to be Word Position and encoded it in an enum.
// Added routines GPCNormalIdx() and  GPCEncode().
// Changed ReadGPCRulesFile() to init the GPC Rule Section pointers with InitGPCSearch().
// Added Routine InitGPCSearch() to init section pointers.
//
// Revision 1.16  2010-07-20 20:41:10-04  alan
// Change the Generic Token names.
// Set the batch lex line display to output to stdout.
//
// Revision 1.15  2010-07-16 18:08:23-04  alan
// Fix the RCS keywords again.
//
// Revision 1.14  2010-07-16 18:07:21-04  alan
// Correct the RCS keywords.
//
// Revision 1.12  2010-07-11 13:28:24-04  alan
// Figured out way to display, or not, the input file lines as they are being read.
// Separated out OutputParamFile() from WriteParamFile() so that output could be to other
// than files.
// Corrected the output of the Vocabulary file to include the Homophone/Homograph
// variables.  And for the dislay of a KeyEntry line from the Vocabulary file.
// Set up printing of Homographs and Homophones.
//
// Revision 1.11  2010-07-06 10:07:25-04  alan
// Minor printf format change (to see extra accuracy in floats)
// Dumb change to Log10() from Log() (base e).  Didn't clue in until afterwards that the
// change cancels out in the formula that was being used to calculate CFSi.
//
// Revision 1.10  2010-06-23 23:50:40-04  alan
// Changed the default .Homophone and .Homograph indices to their own index from -1 and
// corrected and commented the code that relied on these variables.
//
// Revision 1.9  2010-06-23 17:18:31-04  alan
// Added code for homophone and homograph linking.
// Two subroutines were employed to sort the words and phonemes (WordSort, and PhonemeSort).
// Added a strtolwr() call to make sure the Vocabulary words were lower case.
// Fixed a bug in the ReadVocabularyFile() subroutine.  When we got a too long word and
// threw it away we forgot to throw away the rest of the tokens on the line.
// Added code at the bottom of the ReadVocabularyFile() subroutine to sort and link all the
// homophones and homographs.
//
// Revision 1.8  2010-06-15 15:45:51-04  alan
// Added 'clean' calls to each file reading routine (to clean out the data structures that the
// program will write to when reading the file.
// When reading in a parameter file make sure all the values are positive (the inhibition
// is now due to negatives in the code).
// Change the vocabulary word length test to use WORDSZ instead of '8'.
//
// Revision 1.7  2010-06-14 08:28:35-04  alan
// Added code to deal with the Written and Spoken word frequencies and the calculations
// of the respective CFS values.
// Added code to discard words that are too long.
//
// Revision 1.6  2010-06-10 20:29:17-04  alan
// Stopped using a blank at beginning of words to indicate word boundary because I figured this would throw off my version of the program compared to the official version.  So reduced most of the loops in the program that executed WORDSZEXTN times to WORDSZ times.
//
// Revision 1.5  2010-06-09 11:04:33-04  alan
// Modified the 'phoneme' file reading routines to ignore the example words that define what
//     the phoneme is supposed to sound like.
//
// Revision 1.4  2010-06-06 00:11:34-04  alan
// Change header files to use only hdrstop.h
// Changed Parameter file read to ignore extra fields on line.
// Changed Letter file reading routines to read in feature array as float and to add a line for
//    a missing space character in the fiile (with no features).
// Changed Phonemes file read to ignore extra fields on line.
// Change Vocabulary file data structure to use static character arrays in structure instead
//    of dynamically allocated strings and pointers.
//
// Revision 1.3  2010-05-23 09:52:38-04  alan
// Added tests to check that reading in files doesn't overload their respective tables.
//
// Revision 1.2  2010-05-20 16:17:15-04  alan
// Added Read, Write, and Clean routines for all input file types.
// Moved the strdup() calls from the Batch/Generic lexor files into this cpp file
// to minimize redundant strdup/frees.
//
// Revision 1.1  2010-05-20 02:25:26-04  alan
// Got the vocabulary "letters" file input accomplished using the "Generic" lexor.
// Cleaned up some naming for the Batch file processing.
//
// Revision 1.0  2010-05-20 01:07:03-04  alan
// Initial revision
//
//------------------------------------------------------------------------------

#include "DRC_HdrStop.h"

//#define DEBUG
#include "debug.h"

#include <unistd.h>

const char* DRC_FileIO[]={
    "$Author: alan $",
    "$Date: 2011-03-14 17:02:30-04 $",
    "$Revision: 1.48 $",
    "$RCSfile: DRC_FileIO.cpp,v $",
    "$Source: D:\\Repository\\D\\Dev\\Psych499\\DRC_FileIO.cpp,v $"
};

//------------------------------------------------------------------------------

// File discriptor pointers.  Used to control debugging output.
FILE* fh_lexlinedsp=NULL;
FILE* fh_homophone=NULL;
FILE* fh_homograph=NULL;

FILE* fhRTFile=NULL;      // File handle for output RT file.

int StressIdx=MAXSTRESS;

// Name of parent directory to place the DRC result files.
char OutParentDirBuf[MAXPATH];
// Name of individual run directory within OutParentDirBuf to place output.
char OutRunDirBuf[MAXPATH];

int           File_Line_Num;
char          File_Line[MAXLINE];
t_CommentType CommentType;

int PhonemeList[MAXVOCAB];

int MaxWrittenWordFreq=0;
int MaxSpokenWordFreq=0;

t_Stats Stats;

//---------------------------------------------------------------------------
// Routine: File_Line_Display() - Used to display the input line for the Batch.l
//          and Generic.l grammars.
// Input:  File_Line_Num and File_Line set in the above lexors.
// Output: To the fh_lexlinedsp FILE* a line of text for the file.
// SideEffects: Output done to the above FILE*
// Errors: None.
//---------------------------------------------------------------------------
void File_Line_Display(void)
{
    if((fh_lexlinedsp!=NULL)&&(FindBParam("Debug")->Value)){
        pfprintf(fh_lexlinedsp,"%5d %s\n",File_Line_Num,File_Line);
    }
}


//------------------------------------------------------------------------------
// Defines and Global variables for this system.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  XXXXXXx                                             .
//  X     X.                                           .x
//  X     .x                                           .x
//  X     .x  .XXXX  .xxXx .XXXX  .x.XXX .XXX    XXX. .XXX   XXX.  .xxXx .XXXX
//  X     X. .X   xx .X.  .X   xx .X.  xX.  xx  X.  X. .x   X.  X. .X.   X   xx
//  XXXXXX.       .x .X        .x .X   .X   .x .x   .x .x  .x   .x .X   .x
//  X          xXXXx .x     xXXXx .x   .x   .x xXXXXXX .x  xXXXXXX .x    xXX
//  X        .Xx  .x .x   .Xx  .x .x   .x   .x xx      .x  xx      .x      .XX.
//  X        x.   xx .x   x.   xx .x   .x   .x .x      .x  .x      .x        .x
//  X        xx  .Xx .x   xx  .Xx .x   .x   .x  X.  xx .X   X.  xx .x   .x   xx
//  X         XXXx.X .x    XXXx.X .x   .x   .x   XXXx   XX.  XXXx  .x    .XXXx
//
//
// This is an example 'default.parameters' file.  This file can contain comments
// which are prefixed by a '#' and end at the EOL (newline).  For the data lines
// there are two fields:
//      1) Parameter Name - Which can consist of [a-zA-Z0-9_]
//      2) Value field    - Consists of a real or integer field.

//    #
//    # Default DRC Parameters for Australian English
//    #
//
//    # General Parameters
//    ActivationRate 0.2
//    FrequencyScale 0.05
//    MinReadingPhonology 0.4
//
//    # Feature Level Parameters
//    FeatureLetterExcitation 0.005
//    FeatureLetterInhibition 0.15
//
//    # Letter Level Parameters
//    LetterOrthlexExcitation 0.07
//    LetterOrthlexInhibition 0.48
//    LetterLateralInhibition 0
//
//    # Orthographic Lexicon (Orthlex) Parameters
//    OrthlexPhonlexExcitation 0.25
//    OrthlexPhonlexInhibition 0
//    OrthlexLetterExcitation 0.3
//    OrthlexLetterInhibition 0
//    OrthlexLateralInhibition 0.06
//
//    # Phonological Lexicon (Phonlex) Parameters
//    PhonlexPhonemeExcitation 0.09
//    PhonlexPhonemeInhibition 0
//    PhonlexOrthlexExcitation 0.25
//    PhonlexOrthlexInhibition 0
//    PhonlexLateralInhibition 0.07
//
//    # Phoneme Level Parameters
//    PhonemePhonlexExcitation 0.04
//    PhonemePhonlexInhibition 0.16
//    PhonemeLateralInhibition 0.147
//    PhonemeUnsupportedDecay 0.05
//
//    # GPC Route Parameters
//    GPCPhonemeExcitation 0.051
//    GPCCriticalPhonology 0.05
//    GPCOnset 26
//------------------------------------------------------------------------------
t_parameter Parameters[MAXPARAM];
int         ParametersIdx=0;

//---------------------------------------------------------------------------
// Routine: CleanParamFile() - Routine clean up the parameters structure.
// Input:   Parameters[] array.
// Output:  None.
// SideEffects: Parameters[] array modified.
// Errors:  None.
//---------------------------------------------------------------------------
void CleanParamFile(void)
{
    for(int i=0;i<ParametersIdx;i++){
        if(Parameters[i].Name!=NULL) {
            free(Parameters[i].Name);
            Parameters[i].Name=NULL;
        };
        Parameters[i].Value=0.0;
    }
    for(int i=ParametersIdx;i<MAXPARAM;i++){
        Parameters[i].Name=NULL;
        Parameters[i].Value=0.0;
    }
}


//---------------------------------------------------------------------------
// Routine: WriteParamFile() - Write the parameters list out to a file.
// Input:   FileName - Pointer to name of output file.
// Output:  Error message if fopen() fails.
// SideEffects: Output done to stdout and file written to.
// Errors:  Fopen() failure -- results in message to stdout.
//---------------------------------------------------------------------------
void WriteParamFile(char* FileName)
{
    // NOTE: We might want to format the output based on the type of
    //       parameter (to make it look more like the input file).
    FILE* fh=NULL;

    if((fh=fopen(FileName,"w"))!=NULL){
        OutputParamFile(fh);
        fclose(fh);
    }else{
        perror("ERROR: Language-Letters File");
    }
}

//---------------------------------------------------------------------------
// Routine: OutputParamFile() - Routine to do the actual output of the parameter
//          file lines.
// Input:   FILE* for the output.
// Output:  The ASCII version of the Parameters[] table to the FILE* handle.
// SideEffects: Output done.
// Errors:  None.
//---------------------------------------------------------------------------
void OutputParamFile(FILE* fh)
{
    for(int p=0;p<ParametersIdx;p++){
        pfprintf(fh,"%-30s %8.6f\n",
            Parameters[p].Name,
            Parameters[p].Value);
    }
}


//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
int ReadParamFile(const char* FileName)
{
    FILE* fh=NULL;
    int   rtn=1;
    char  Name[BUFFERSZ];
    char  Value[BUFFERSZ];

    if(strcmp("--",FileName)==0){
        // Told to take from stdin ...
        fh=stdin;
    }else{
        // Else open up a file.
        fh=fopen(FileName,"r");
        if(fh==NULL){
            perror("ERROR: Language-Parameters File");
            rtn=0;
        }
    }
    if(fh!=NULL){
        CleanParamFile();
        CommentType = COM_EveryWhere;
        File_Line_Num = 0;
        // If from stdin or the file open worked then get the first line
        // into our display buffer.  Then seek back to the beginning to start
        // the parsing of the file.  Strangely, the fseek() works on both disk
        // files and stdin streams!
        if(fgets(File_Line,MAXLINE,fh)!=NULL){
            File_Line[strlen(File_Line)-1]=0;
            fseek(fh,0,SEEK_SET);
        }
        // Point the parser input to our open file stream and parse it.
        Genericin=fh;

        ParametersIdx=0;
        int Token=Genericlex();
        while(Token!=0){
            debug1("Token(%d)\n",Token);
            switch(Token){
                case TKG_Comment:
                    debug1("  Comment='%s'\n",Genericlval.stype);
                    Token=Genericlex();
                    break;
                case TKG_Token:
                    if(ParametersIdx>=MAXPARAM){
                        pfprintf(stderr,"ERROR: Number of parameters in Language-Parameter file exceeds table size.\n");
                        Genericrestart(fh);
                        if(fh!=stdin) fclose(fh);
                        return(0);
                    }
                    debug1("  Token='%s'\n",Genericlval.stype);

                    // Save the Name field
                    strcpy(Name,Genericlval.stype);
                    //Parameters[ParametersIdx].Name=strdup(ptr);

                    // Save the Value field.
                    Genericlex();
                    strcpy(Value,Genericlval.stype);
                    // Convert all the input numbers into positive numbers because
                    // the name of the parameter actually conveys it's effect (that
                    // is the +/- effect is now part of the code.
                    //Parameters[ParametersIdx].Value=fabs(atof(ptr));
                    UpdateParamTables(stderr,Name,Value);

                    // Suck up all the tail end tokens that we don't read in yet.  This
                    // allows for enhanced files.
                    while((Token=Genericlex())!=TKG_EOL);

                    ParametersIdx++;
                    break;
                case TKG_EOL:
                    debug0("  End-of-Line\n");
                    Token=Genericlex();
                    break;
                default:
                    debug1("  ERROR:Unknown token=%d\n",Token);
                    Token=Genericlex();
                    break;
            }
        }



        // Close our file stream as long as it isn't stdin.
        if(fh!=stdin) fclose(fh);
    }

    // Now update the core variables.
    UpdateLocalParams();

    return(rtn);
}
















//------------------------------------------------------------------------------
//    XXXXx    XXXXXXx     xXXXx    XXXXXXX           .x
//  .X.   xX   X     X.   X.   xx   X     .X          .x
//  X.     x.  X     .x  X.     X.  X      X.         .x
// .X          X     .x .X          X      X. .x   .x .x   XXX.   .XXXX
// xx          X     X. .x          X     xX  .x   .x .x  X.  X.  X   xx
// xx   .XXXx  XXXXXX.  .x          XXXXXXX   .x   .x .x .x   .x .x
// xx      .x  X        .x          X   .X    .x   .x .x xXXXXXX  xXX
// .X      .x  X        .X      .x  X    xX   .x   .x .x xx         .XX.
//  X.     .x  X         x.     X.  X     Xx  .x   .x .x .x           .x
//  .X.   xX.  X         .X.   xX   X     .X. .X   Xx .x  X.  xx .x   xx
//    xXXXx    X           XXXXx    X      xX  .XXX.x .x   XXXx   .XXXx
//
//
// The following segments are from an example 'gpcrules' file and are representative
// of each different portion.  The file consists of one line records with six fields:
//      1) A single character (e,A,m,b).  Unknown purpose.
//      2) A alphabetic string.  Purpose unknown (perhaps: Type of Rule).
//          The values for this field are:
//              a) body
//              b) cs
//              c) mphon
//              d) multi
//              e) out
//              f) sing
//              g) two
//      3) Vocabulary word (alphabetic string).  Ordered longest to shortest in file.
//          These seem to correspond to words directly or via context rules of the form:
//              a) Word - Requires no context to translate (eg. arched).
//              b) Separated Letters - Two letters separated by an unspecified third (eg. a.e)
//              c) Vowel/Consonant prefix/suffix - Letters followed by Vowel/Consonant - (eg. gu[\V])
//              d) Prefix/suffix set - Letters preceded by, or followed by set (eg. [#123456789DbNdgilmnuv]s).
//              e) Possibly others.
//      4) Phonemes (one or two characters from the 'phoneme' file.).
//      5) Unknown - Single character. Always seems to be 'u'.  Purpose unknown.
//      6) Unknown - Float. Always seems to be '1.0'.

//    e body anched #nJt u 1.0
//    e body arched  #Jt u 1.0
//    e body atched  {Jt u 1.0
//    e body eached  iJt u 1.0
//    e body earned  3nd u 1.0
//    e body eathed  iDd u 1.0
//    e body eathes  iDz u 1.0
//    e body eighed   1d u 1.0
//    e body eights  1ts u 1.0
//    e body eigned  1nd u 1.0
//    e body enched EnJt u 1.0
//
//      ...
//
//    A multi                     eigh    1 u 1.0
//    A multi                     eare    7 u 1.0
//    A multi                     augh    9 u 1.0
//    m multi                     arre    # u 1.0
//    m multi                     eere    7 u 1.0
//    m multi                     urre    3 u 1.0
//    A multi                     ough    9 u 1.0
//    e multi                     tsch    J u 1.0
//    e multi                      tch    J u 1.0
//    m multi                      tch    J u 1.0
//    A multi                      air    8 u 1.0
//
//      ...
//
//    A   two                      a.e    1 u 1.0
//    A   two                      i.e    2 u 1.0
//    A   two                       oo    u u 1.0
//    A   two                      o.e    5 u 1.0
//    m   two                      y.e    2 u 1.0
//    e   two                      y.e    2 u 1.0
//    A   two                      u.e    W u 1.0
//    A   two                      e.e    i u 1.0
//    A   two                       ck    k u 1.0
//    A   two                       ee    i u 1.0
//    A   two                       ea    i u 1.0
//    A   two                       ar    # u 1.0
//    A   two                       sh    S u 1.0
//    A   two                       ai    1 u 1.0
//    A   two                       ff    f u 1.0
//    A   two                       oa    5 u 1.0
//    e   two                       ll    l u 1.0
//    e   two                       ge    _ u 1.0
//    m   two                       ge    _ u 1.0
//
//      ...
//
//    m mphon                        x   ks u 1.0
//    e mphon                        x   ks u 1.0
//    b    cs                   gu[\V]    g u 1.0
//    b    cs                   ng[~e]    N u 1.0
//    m    cs                   ng[~e]    N u 1.0
//    e    cs                   ng[~e]    N u 1.0
//    m    cs                     [q]u    w u 1.0
//    A    cs                     g[e]    _ u 1.0
//    b    cs                  sc[eiy]    s p 1.0
//    A    cs                     c[e]    s p 1.0
//    m    cs                     [t]h    T u 1.0
//    b    cs                     c[i]    s u 1.0
//    b    cs                     c[y]    s u 1.0
//    e    cs               [\V][\C]ed    d u 1.0
//    e    cs           [\V][\C][\C]ed    d u 1.0
//    e    cs       [\V][\C][\C][\C]ed    d u 1.0
//
//      ...
//
//    A  sing                        s    s u 1.0
//    A  sing                        t    t u 1.0
//    A  sing                        p    p u 1.0
//    A  sing                        l    l u 1.0
//    m  sing                        i    I u 1.0
//    m  sing                        a    { u 1.0
//    A  sing                        d    d u 1.0
//    A  sing                        n    n u 1.0
//    A  sing                        r    r u 1.0
//    m  sing                        u    V u 1.0
//    b  sing                        u    V u 1.0
//    A  sing                        m    m u 1.0
//    A  sing                        b    b u 1.0
//
//      ...
//
//    e   out [#123456789DbNdgilmnuv]s    z u 1.0
//    A   out                     d[T]    t u 1.0
//    A   out                     n[k]    N u 1.0
//    A   out                    [pk]d    t u 1.0
//    e   out                    [SJ]d    t u 1.0
//    A   out                     [s]d    t u 1.0
//    A   out                     [t]z    s u 1.0
//    A   out                    [Tf]d    t u 1.0
//------------------------------------------------------------------------------



// Class names of the GPCRules file rules: Body, Multi(ple),Two,M(ulti)Phon(ic),
// C(ontext)S(ensitive), Sing(le), Out(put).
const char*     GPCRuleClassNames[]={"?","body","multi","two","mphon","cs","sing","out"};
// Storage for the GPCRules file.
t_gpcrule GPCRules[MAXGPCRULES];
// Index into GPCRules[] array. After loaded its size.
int       GPCRulesIdx=0;

// Field store is an malloc'ed array that stores the individual specific context
// fields defined in the GPC rules file (i.e. "[rls]").
bool *GPCFieldStore=NULL;
int   GPCFieldStoreIdx=-1;


// Section pointers for each section of GPCRules.
t_gpcrule* GPCList_BodyStart=NULL;
t_gpcrule* GPCList_BodyEnd=NULL;
t_gpcrule* GPCList_MultiplesStart=NULL;
t_gpcrule* GPCList_MultiplesEnd=NULL;
t_gpcrule* GPCList_DoublesStart=NULL;
t_gpcrule* GPCList_DoublesEnd=NULL;
t_gpcrule* GPCList_MultiPhonemesStart=NULL;
t_gpcrule* GPCList_MultiPhonemesEnd=NULL;
t_gpcrule* GPCList_ContextSensitiveStart=NULL;
t_gpcrule* GPCList_ContextSensitiveEnd=NULL;
t_gpcrule* GPCList_SinglesStart=NULL;
t_gpcrule* GPCList_SinglesEnd=NULL;
t_gpcrule* GPCList_OutputStart=NULL;
t_gpcrule* GPCList_OutputEnd=NULL;
t_gpcrule* GPCList_End=NULL;

int        GPCBodyRules=0;   // Number of body rules.
int        GPCMultiRules=0;  // Number of multi-letter rules.                   #1
int        GPCTwoRules=0;    // Number of two-letter rules.                     #3
int        GPCMPhonRules=0;  // Number of Multi-phoneme (one-to-multi) rules.   #4
int        GPCCSRules=0;     // Number of context-sensitive rules.              #2
int        GPCSingRules=0;   // Number of single-letter rules.                  #5
int        GPCOutRules=0;    // Number of output rules.
int        GPCStressRules=0; // Number of stress rules (not implemented).

//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
void CleanGPCRulesFile(void)
{
    for(int i=0;i<MAXGPCRULES;i++){
        if(i>=GPCRulesIdx){
            // If we've already filled the array be sure to deallocate
            // the strdup'ed memory first.
            if(GPCRules[i].GraphemeContext!=NULL) {
                free(GPCRules[i].GraphemeContext);
            };
            if(GPCRules[i].Phonemes!=NULL) {
                free(GPCRules[i].Phonemes);
            };
        }
        GPCRules[i].WrdPosn=wpNone;
        GPCRules[i].Class=clsNone;
        GPCRules[i].GraphemeContext=NULL;
        memset(GPCRules[i].Fields,0,sizeof(GPCRules[i].Fields));
        memset(GPCRules[i].Mask,0,sizeof(GPCRules[i].Mask));
        GPCRules[i].PreContext=-1;
        GPCRules[i].Phonemes=NULL;
        GPCRules[i].Protected=false;
        GPCRules[i].Unknown3=0.0;
    }
}


//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
void WriteGPCRulesFile(char* FileName)
{
    // Special parsing.
    FILE* fh=NULL;

    if((fh=fopen(FileName,"w"))!=NULL){
        for(int i=0;i<GPCRulesIdx;i++){
            pfprintf(fh,"%c %-6s %-26s %2d %-6s %c %3.1f ",
                "?bmeA"[GPCRules[i].WrdPosn],
                GPCRuleClassNames[GPCRules[i].Class],
                GPCRules[i].GraphemeContext,
                GPCRules[i].PreContext,
                GPCRules[i].Phonemes,
                (GPCRules[i].Protected)?"p":"u",
                GPCRules[i].Unknown3);
            for(int f=0;f<MAXINPUTBUF;f++){
                pfprintf(fh," %02x", GPCRules[i].Fields[f]);
            }
            pfprintf(fh," ");
            for(int f=0;f<MAXINPUTBUF;f++){
                pfprintf(fh," %02x", GPCRules[i].Mask[f]);
            }
            pfprintf(fh,"\n");
        }
        fclose(fh);
    }else{
        perror("ERROR: Language-Letters File");
    }
}


//---------------------------------------------------------------------------
// Routine: DRC_GPCSetField - Take our string of characters (either letters defined
//          in the 'letter' file, or phonemes defined in the 'phoneme' file) and
//          convert it into an array of FLD_ codes which is stored in the BYTE
//          array Fields which is a member of the t_gpcrules structure. This routine
//          is called from the ReadGPCRulesFile() routine while reading in the
//          GPCRules file. The input string is a phonemic string when dealing
//          with clsOut class GPCRules.
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
void DRC_GPCSetField(t_gpcrule* rule)
{
    int FieldIdx=0;
    int FieldState=FLDStateNormal;

    char* word=(char*)rule->GraphemeContext;
    eGPCClass WrdClass=rule->Class;
    BYTE* Fields=rule->Fields;
    BYTE* Mask=(BYTE*)rule->Mask;

    // Initialize the fixed elements of the field structure and allocate the array if not done already.
    if(GPCFieldStore==NULL){
        // Define the array to have the number of rows defined by the number of
        // special characters to define a context field. Each row will be the
        // largest number of characters to support for a field.  Since we use
        // letters and phonemes it is MAXPHONEMES.
        GPCFieldStore = new bool[MAXFIELDS*MAXFLDCHAR];
        memset(GPCFieldStore,0,MAXFIELDS*MAXFLDCHAR*sizeof(bool));

        // WARNING: The 'letters' and 'phonemes' files must be loaded at this point.
        for(int i=0;i<MAXFLDCHAR;i++){

            // Set the Grapheme vowel and consonants ('letter' file). These are
            // two predefined context fields.  These two are for letters the next
            // two are for phonemes.
            if(i<MAXLETTERS){
                if(Letters[i].VC=='V'){
                    // Define the accepatble letters for the Vowels field.
                    GPCFieldStore[FLD_GRAPHVOWELS*MAXFLDCHAR+i]=true;
                    GPCFieldStore[FLD_GRAPHCONSONANTS*MAXFLDCHAR+i]=false;
                }else{
                    // Define the accepatble letters for the Consonants field.
                    GPCFieldStore[FLD_GRAPHVOWELS*MAXFLDCHAR+i]=false;
                    GPCFieldStore[FLD_GRAPHCONSONANTS*MAXFLDCHAR+i]=true;
                }
            }

            // Set the Phoneme vowels and consonants ('phoneme' file) different from
            // the Grapheme vowels and consonants.
            if(Phonemes[i].VC=='V'){
                // Define the accepatble letters for the Vowels field.
                GPCFieldStore[FLD_PHONEVOWELS*MAXFLDCHAR+i]=true;
                GPCFieldStore[FLD_PHONECONSONANTS*MAXFLDCHAR+i]=false;
            }else{
                // Define the accepatble letters for the Consonants field.
                GPCFieldStore[FLD_PHONEVOWELS*MAXFLDCHAR+i]=false;
                GPCFieldStore[FLD_PHONECONSONANTS*MAXFLDCHAR+i]=true;
            }

        }
        GPCFieldStoreIdx = -1;
    }

    // Clear our return Fields array.
    memset(Fields,0,MAXINPUTBUF*sizeof(BYTE));

    // Convert character string to FLD_ code string.
    int chr,idx;
    for(int c=0;c<(int)strlen(word);c++){
        switch(FieldState){
            case FLDStateNormal:
                chr=word[c];
                if((WrdClass!=clsOut)&&((idx=IndexLetter[tolower(chr)])!=EOW_LETTER)){  // ie. we got a..z
                    // These field numbers correspond to FLD_A to FLD_Z
                   if(rule->PreContext==-1){
                        rule->PreContext=FieldIdx;
                    }
                    Fields[FieldIdx]=idx+1;
                    Mask[FieldIdx]=MASK_USED;
                    FieldIdx++;

                }else if((WrdClass==clsOut)&&((idx=IndexPhoneme[chr])!=EOW_PHONEME)){  // ie. we got valid phoneme code.
                    // These field numbers correspond to the phonemes
                   if(rule->PreContext==-1){
                        rule->PreContext=FieldIdx;
                    }
                    Fields[FieldIdx]=idx+1;
                    Mask[FieldIdx]=MASK_USED;
                    FieldIdx++;

                }else if(chr=='['){
                    FieldState=FLDStateContext;
                    GPCFieldStoreIdx++;
                    // Then save the FieldStore index to our fields array.
                    Fields[FieldIdx]=GPCFieldStoreIdx+FLD_USERMIN;
                    Mask[FieldIdx]=MASK_SPACE;
                    FieldIdx++;

                }else if(chr=='.'){
                    Fields[FieldIdx]=FLD_ANY;
                    Mask[FieldIdx]=MASK_SPACE;
                    FieldIdx++;

                }else{
                    pfprintf(stderr,"ERROR: Illegal syntax GPCRule word='%s' at '%c' state=%d\n",word,chr,FieldState);
                }
                break;

            case FLDStateContext:
                chr=word[c];
                if((WrdClass!=clsOut)&&((idx=IndexLetter[tolower(chr)])!=EOW_LETTER)){  // ie. we got valid letter code.
                    // These field numbers correspond to FLD_A to FLD_Z
                    GPCFieldStore[GPCFieldStoreIdx*MAXFLDCHAR + idx]=true;

                }else if((WrdClass==clsOut)&&((idx=IndexPhoneme[chr])!=EOW_PHONEME)){  // ie. we got valid phoneme code.
                    // These field numbers correspond to the phonemes
                    GPCFieldStore[GPCFieldStoreIdx*MAXFLDCHAR + idx]=true;

                }else if(chr==']'){
                    FieldState=FLDStateNormal;

                }else if(chr=='\\'){
                    GPCFieldStoreIdx--;
                    FieldIdx--;
                    FieldState=FLDStateMeta;

                }else if(chr=='~'){
                    FieldState=FLDStateNegContext;
                    // Set this field to all true (all characters accepted) and
                    // we will later negate any letters that are not accpeted.
                    for(int i=0;i<MAXFLDCHAR;i++){
                        GPCFieldStore[GPCFieldStoreIdx*MAXFLDCHAR + i]=true;
                    }

                }else{
                    pfprintf(stderr,"ERROR: Illegal syntax GPCRule word='%s' at '%c' state=%d\n",word,chr,FieldState);
                }
                break;

            case FLDStateNegContext:
                chr=word[c];
                if((WrdClass!=clsOut)&&((idx=IndexLetter[tolower(chr)])!=26)){  // ie. we got a..z
                    // These field numbers correspond to FLD_A to FLD_Z
                    GPCFieldStore[GPCFieldStoreIdx*MAXFLDCHAR + idx]=false;

                }else if((WrdClass==clsOut)&&((idx=IndexPhoneme[chr])!=44)){  // ie. we got valid phoneme code.
                    // These field numbers correspond to the phonemes
                    GPCFieldStore[GPCFieldStoreIdx*MAXFLDCHAR + idx]=false;

                }else if(chr==']'){
                    FieldState=FLDStateNormal;

                }else{
                    pfprintf(stderr,"ERROR: Illegal syntax GPCRule word='%s' at '%c' state=%d\n",word,chr,FieldState);
                }
                break;

            case FLDStateMeta:
                chr=toupper(word[c]);
                if(chr=='V'){
                    // Set a Vowel Field (either for phonemes or graphemes).
                    Fields[FieldIdx]=(WrdClass==clsOut)?FLD_PHONEVOWELS:FLD_GRAPHVOWELS;
                    Mask[FieldIdx]=MASK_SPACE;
                    FieldIdx++;

                }else if(chr=='C'){
                    // Set a Consonant Field (either for phonemes or graphemes).
                    Fields[FieldIdx]=(WrdClass==clsOut)?FLD_PHONECONSONANTS:FLD_GRAPHCONSONANTS;
                    Mask[FieldIdx]=MASK_SPACE;
                    FieldIdx++;

                }else if(chr==']'){
                    FieldState=FLDStateNormal;

                }else{
                    pfprintf(stderr,"ERROR: Illegal syntax GPCRule word='%s' at '%c' state=%d\n",word,chr,FieldState);
                }
                break;

            default:
                pfprintf(stderr,"ERROR: Illegal syntax GPCRule word='%s' at '%c' state=%d\n",word,chr,FieldState);
                break;
        }
    }

    // If this rule is an End-of-Word rule then append a End-of-Word field to
    // the Fields array and the phoneme string.
/*    if(rule->WrdPosn==wpEnd){
        Fields[FieldIdx]=FLD_ENDOFWORD;
        Mask[FieldIdx]=MASK_SPACE;
        FieldIdx++;
    }
*/
}

//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
int ReadGPCRulesFile(char* FileName)
{
    FILE* fh=NULL;
    int   rtn=1;
    char* ptr;

    // Determine where to get the input batch of words.
    if(strcmp("--",FileName)==0){
        // Told to take from stdin ...
        fh=stdin;
    }else{
        // Else open up a file.
        fh=fopen(FileName,"r");
        if(fh==NULL){
            perror("ERROR: Language-GPCRules File");
            rtn=0;
        }
    }

    // ...and if we figure it out read in the file.
    if(fh!=NULL){
        CleanGPCRulesFile();
        CommentType = COM_BeginOfLine;
        File_Line_Num = 0;
        // If from stdin or the file open worked then get the first line
        // into our display buffer.  Then seek back to the beginning to start
        // the parsing of the file.  Strangely, the fseek() works on both disk
        // files and stdin streams!
        if(fgets(File_Line,MAXLINE,fh)!=NULL){
            File_Line[strlen(File_Line)-1]=0;
            fseek(fh,0,SEEK_SET);
        }
        // Point the parser input to our open file stream and parse it.
        Genericin=fh;

        GPCRulesIdx=0;
        int Token=0;
        while((Token=Genericlex())!=0){
            debug1("Token(%d)\n",Token);
            switch(Token){
                case TKG_Comment:
                    debug1("  Comment='%s'\n",Genericlval.stype);
                    break;
                case TKG_Token:
                    if(GPCRulesIdx>=MAXGPCRULES){
                        pfprintf(stderr,"ERROR: Number of rules in Language-GPCRules file exceeds table size.\n");
                        Genericrestart(fh);
                        if(fh!=stdin) fclose(fh);
                        return(0);
                    }
                    debug1("  Token='%s'\n",Genericlval.stype);

                    // Save the initial letter
                    ptr=Genericlval.stype;
                    switch (*ptr){
                        case 'b': GPCRules[GPCRulesIdx].WrdPosn=wpBegin; break;
                        case 'm': GPCRules[GPCRulesIdx].WrdPosn=wpMedial; break;
                        case 'e': GPCRules[GPCRulesIdx].WrdPosn=wpEnd; break;
                        case 'A': GPCRules[GPCRulesIdx].WrdPosn=wpAny; break;
                        default:
                            pfprintf(stderr,"ERROR: GPCRules file: Illegal Word Position='%c'\n",ptr);
                            break;
                    }


                    // Save the Class.
                    Genericlex();
                    ptr=Genericlval.stype;
                    if(strcmp(ptr,"body")==0){
                        GPCRules[GPCRulesIdx].Class=clsBody;
                        GPCBodyRules++;
                    }else if(strcmp(ptr,"multi")==0){
                        GPCRules[GPCRulesIdx].Class=clsMulti;
                        GPCMultiRules++;
                    }else if(strcmp(ptr,"two")==0){
                        GPCRules[GPCRulesIdx].Class=clsTwo;
                        GPCTwoRules++;
                    }else if(strcmp(ptr,"mphon")==0){
                        GPCRules[GPCRulesIdx].Class=clsMphon;
                        GPCMPhonRules++;
                    }else if(strcmp(ptr,"cs")==0){
                        GPCRules[GPCRulesIdx].Class=clsCS;
                        GPCCSRules++;
                    }else if(strcmp(ptr,"sing")==0){
                        GPCRules[GPCRulesIdx].Class=clsSing;
                        GPCSingRules++;
                    }else if(strcmp(ptr,"out")==0){
                        GPCRules[GPCRulesIdx].Class=clsOut;
                        GPCOutRules++;
                    }else{
                        pfprintf(stderr,"ERROR: GPCRules file: Illegal class='%s'\n",ptr);
                    }

                    // Save the Grapheme field.
                    Genericlex();
                    ptr=Genericlval.stype;
                    GPCRules[GPCRulesIdx].GraphemeContext=(BYTE*)strdup(ptr);

                    // Save the Phoneme field
                    Genericlex();
                    ptr=Genericlval.stype;
                    GPCRules[GPCRulesIdx].Phonemes=(BYTE*)strdup(ptr);

                    // Save the Protected/Unprotected field (mostly 'u' but a few 'p's)
                    Genericlex();
                    ptr=Genericlval.stype;
                    GPCRules[GPCRulesIdx].Protected=(tolower(*ptr)=='p')?true:false;

                    // Save the Unknown3 field (always '1.0');
                    Genericlex();
                    ptr=Genericlval.stype;
                    GPCRules[GPCRulesIdx].Unknown3=atof(ptr);

                    // Generate the corresponding .Fields array.
                    DRC_GPCSetField(&GPCRules[GPCRulesIdx]);

                    GPCRulesIdx++;
                    if(GPCRulesIdx>=MAXGPCRULES){
                        pfprintf(stderr,"ERROR: Number of GPCRules exceed MAXGPCRULES.\n");
                        exit(1);
                    }
                    break;
                case TKG_EOL:
                    debug0("  End-of-Line\n");
                    break;
                default:
                    debug1("  ERROR:Unknown token=%d\n",Token);
                    break;
            }
        }



        // Close our file stream as long as it isn't stdin.
        if(fh!=stdin) fclose(fh);
    }

    SortGPCRules();
    InitGPCSearch();

    return(rtn);
}

//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
int GPCRuleSort(const void *a,const void *b)
{
    t_gpcrule* EntryA=(t_gpcrule*)a;
    t_gpcrule* EntryB=(t_gpcrule*)b;
    int rtn;

    // Check to see which rule has earlier sort order based on the class of the
    // rule.  Rules are sorted by class in the order:
    //     NoClass,Out,Body,Multi,CS,Two,Mphon, and Sing
    // The rules are tested in the GPC Route in order Body,Multi,CS,Two,Mphon, and Sing.
    rtn = EntryA->Class - EntryB->Class;
    if(rtn==0){
        int EntryA_NumSimpleFlds=0;
        for(int i=0;i<(int)strlen((const char*)EntryA->GraphemeContext);i++){
            int Field=EntryA->Fields[i];
            // If the current field is a simple field then tally the
            // corresponding input activation.
            if((Field>=FLD_SIMPLEMIN)&&(Field<=FLD_SIMPLEMAX)){
                EntryA_NumSimpleFlds++;
            }
        }

        int EntryB_NumSimpleFlds=0;
        for(int i=0;i<(int)strlen((const char*)EntryB->GraphemeContext);i++){
            int Field=EntryB->Fields[i];
            // If the current field is a simple field then tally the
            // corresponding input activation.
            if((Field>=FLD_SIMPLEMIN)&&(Field<=FLD_SIMPLEMAX)){
                EntryB_NumSimpleFlds++;
            }
        }

        // Sort on the number of simple fields (larger number first).
        rtn=EntryB_NumSimpleFlds - EntryA_NumSimpleFlds;
        // Sort on the number of fields (non-CS rules first).
        if(rtn==0){
            rtn=strlen((const char*)EntryA->GraphemeContext) - strlen((const char*)EntryB->GraphemeContext);
        }
        // Sort alphabetically on Grapheme string.
        if(rtn==0){
            rtn=strcmp((const char*)EntryA->GraphemeContext,(const char*)EntryB->GraphemeContext);
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
void SortGPCRules(void)
{
    // Sort the entire table by words so we can be sure words are in order by orthography.
    //qsort(GPCRules,GPCRulesIdx,sizeof(t_gpcrule),GPCRuleSort);
}

//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
t_gpcrule* GPC_NextRule(t_gpcrule* rule)
{
    // NOTE: This code depends on the sections of the GPCRules file being contiguous.
    //     Rule order for searching: Body,Multi,CS,Two,Mphon, and Sing

    t_gpcrule* nextrule=NULL;
    if(rule==GPCList_BodyEnd){
        nextrule=GPCList_MultiplesStart;

    }else if(rule==GPCList_MultiplesEnd){
        nextrule=GPCList_ContextSensitiveStart;

    }else if(rule==GPCList_ContextSensitiveEnd){
        nextrule=GPCList_DoublesStart;

    }else if(rule==GPCList_DoublesEnd){
        nextrule=GPCList_MultiPhonemesStart;

    }else if(rule==GPCList_MultiPhonemesEnd){
        nextrule=GPCList_SinglesStart;

    }else if(rule==GPCList_SinglesEnd){
        // We've gotten to end of normal search list.
        nextrule=NULL;

    }else if(rule==GPCList_OutputEnd){
        // We've gotten to end of output rules search list.
        nextrule=NULL;

    }else{
        nextrule= rule+1;
    }
    return(nextrule);
}

//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
bool InitGPCSearch(void)
{
    // GPCList_End is the last valid entry in the list of GPC Rules.
    GPCList_End=&GPCRules[GPCRulesIdx-1];

    // Find the start rule of each class of rules (which are sorted together).
    for(int i=0; i<GPCRulesIdx; i++){
        switch(GPCRules[i].Class){
            case clsBody:
                if(GPCList_BodyStart==NULL){
                    GPCList_BodyStart=&GPCRules[i];
                }
                GPCList_BodyEnd=&GPCRules[i];
                break;

            case clsMulti:
                if(GPCList_MultiplesStart==NULL){
                    GPCList_MultiplesStart=&GPCRules[i];
                }
                GPCList_MultiplesEnd=&GPCRules[i];
                break;

            case clsTwo:
                if(GPCList_DoublesStart==NULL){
                    GPCList_DoublesStart=&GPCRules[i];
                }
                GPCList_DoublesEnd=&GPCRules[i];
                break;

            case clsMphon:
                if(GPCList_MultiPhonemesStart==NULL){
                    GPCList_MultiPhonemesStart=&GPCRules[i];
                }
                GPCList_MultiPhonemesEnd=&GPCRules[i];
                break;

            case clsCS:
                if(GPCList_ContextSensitiveStart==NULL){
                    GPCList_ContextSensitiveStart=&GPCRules[i];
                }
                GPCList_ContextSensitiveEnd=&GPCRules[i];
                break;

            case clsSing:
                if(GPCList_SinglesStart==NULL){
                    GPCList_SinglesStart=&GPCRules[i];
                }
                GPCList_SinglesEnd=&GPCRules[i];
                break;

            case clsOut:
                if(GPCList_OutputStart==NULL){
                    GPCList_OutputStart=&GPCRules[i];
                }
                GPCList_OutputEnd=&GPCRules[i];
                break;
            default:
                break;
        }
    }
    return(false);
}












//------------------------------------------------------------------------------
//  X               .   .
//  X              .x  .x
//  X              .x  .x
//  X        XXX. .XXXxXXX   XXX.  .xxXx .XXXX
//  X       X.  X. .x  .x   X.  X. .X.   X   xx
//  X      .x   .x .x  .x  .x   .x .X   .x
//  X      xXXXXXX .x  .x  xXXXXXX .x    xXX
//  X      xx      .x  .x  xx      .x      .XX.
//  X      .x      .x  .x  .x      .x        .x
//  X       X.  xx .X  .X   X.  xx .x   .x   xx
//  XXXXXX   XXXx   XX. XX.  XXXx  .x    .XXXx
//
//
// This is an example 'letters' file.  It is the translation between letters of
// the incoming word to be converted to phonemes and the letter features. This
// file seems to consist of comment lines started with a '#' character and going
// to the end of the line.  The second column seems to be the common interpretation
// of (V)owel/(C)onsonant (ie. 'a','e','i','o','u' and 'y' are vowels and the other
// letters are consonants.  This correspondence doesn't equate with the phonemic
// Vowel/Consonant correspondence which is predicated on how the sounds are produced.
// The third column is always 'L' which seems to indicate 'Letter' but otherwise
// the purpose of the column is unknown.  The 4th to 17th column appear to be
// boolean values for weather the letter's "feature" is on(1) or off(0).

//    # English letters with vowel/consonant, inclusion and visual feature flags.
//    a V L 1 1 1 1 0 1 0 0 0 1 0 0 0 1
//    b C L 0 1 1 1 1 0 0 1 0 1 0 1 0 0
//    c C L 1 1 0 0 1 1 0 0 0 0 0 0 0 0
//    d C L 0 1 1 1 1 0 0 1 0 0 0 1 0 0
//    e V L 1 1 0 0 1 1 0 0 0 0 0 0 0 1
//    f C L 1 1 0 0 0 1 0 0 0 0 0 0 0 1
//    g C L 1 1 0 1 1 1 0 0 0 1 0 0 0 0
//    h C L 1 0 1 1 0 1 0 0 0 1 0 0 0 1
//    i V L 0 1 0 0 1 0 0 1 0 0 0 1 0 0
//    j C L 0 0 1 1 1 1 0 0 0 0 0 0 0 0
//    k C L 1 0 0 0 0 1 0 0 1 0 1 0 0 1
//    l C L 1 0 0 0 1 1 0 0 0 0 0 0 0 0
//    m C L 1 0 1 1 0 1 1 0 1 0 0 0 0 0
//    n C L 1 0 1 1 0 1 1 0 0 0 1 0 0 0
//    o V L 1 1 1 1 1 1 0 0 0 0 0 0 0 0
//    p C L 1 1 1 0 0 1 0 0 0 1 0 0 0 1
//    q C L 1 1 1 1 1 1 0 0 0 0 1 0 0 0
//    r C L 1 1 1 0 0 1 0 0 0 1 1 0 0 1
//    s C L 1 1 0 1 1 0 0 0 0 1 0 0 0 1
//    t C L 0 1 0 0 0 0 0 1 0 0 0 1 0 0
//    u V L 1 0 1 1 1 1 0 0 0 0 0 0 0 0
//    v C L 1 0 0 0 0 1 0 0 1 0 0 0 1 0
//    w C L 1 0 1 1 0 1 0 0 0 0 1 0 1 0
//    x C L 0 0 0 0 0 0 1 0 1 0 1 0 1 0
//    y V L 0 0 0 0 0 0 1 0 1 0 0 1 0 0
//    z C L 0 1 0 0 1 0 0 0 1 0 0 0 1 0
//
//------------------------------------------------------------------------------

t_letter Letters[MAXLETTERS];
int      LettersIdx=0;
int      FeaturesIdx=MAXFEATURES;

//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
void CleanLettersFile(void)
{
    for(int i=0;i<MAXLETTERS;i++){
        Letters[i].Letter=0;
        Letters[i].VC=0;
        Letters[i].Unknown1=0;
        for(int f=0;f<14;f++){
            Letters[i].Features[f]=0.0;
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
void WriteLettersFile(char* FileName)
{
    // Special parsing.
    FILE* fh=NULL;

    if((fh=fopen(FileName,"w"))!=NULL){
        for(int i=0;i<LettersIdx;i++){
            pfprintf(fh,"%c %c %c",
                Letters[i].Letter,
                Letters[i].VC,
                Letters[i].Unknown1);
            for(int f=0;f<14;f++){
                pfprintf(fh," %8.6f",Letters[i].Features[f]);
            }
            pfprintf(fh,"\n");
        }
        fclose(fh);
    }else{
        perror("ERROR: Language-Letters File");
    }
}




//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
int ReadLettersFile(char* FileName)
{
    FILE* fh=NULL;
    int   rtn=1;
    char* ptr;
    bool  FLG_FoundSpace=false;

    if(strcmp("--",FileName)==0){
        // Told to take from stdin ...
        fh=stdin;
    }else{
        // Else open up a file.
        fh=fopen(FileName,"r");
        if(fh==NULL){
            perror("ERROR: Language-Letter File");
            rtn=0;
        }
    }
    if(fh!=NULL){
        CleanLettersFile();
        CommentType = COM_EveryWhere;
        File_Line_Num = 0;
        // If from stdin or the file open worked then get the first line
        // into our display buffer.  Then seek back to the beginning to start
        // the parsing of the file.  Strangely, the fseek() works on both disk
        // files and stdin streams!
        if(fgets(File_Line,MAXLINE,fh)!=NULL){
            File_Line[strlen(File_Line)-1]=0;
            fseek(fh,0,SEEK_SET);
        }
        // Point the parser input to our open file stream and parse it.
        Genericin=fh;

        LettersIdx=0;
        int Token=0;
        while((Token=Genericlex())!=0){
            debug1("Token(%d)\n",Token);
            switch(Token){
                case TKG_Comment:
                    debug1("  Comment='%s'\n",Genericlval.stype);
                    break;
                case TKG_Token:
                    if(LettersIdx>=MAXLETTERS){
                        pfprintf(stderr,"ERROR: Number of lines in Language-Letters file exceeds table size.\n");
                        Genericrestart(fh);
                        if(fh!=stdin) fclose(fh);
                        return(0);
                    }
                    debug1("  Token='%s'\n",Genericlval.stype);
                    // Save the letter
                    ptr=Genericlval.stype;
                    Letters[LettersIdx].Letter=(*ptr);
                    if((*ptr)==' ') FLG_FoundSpace=true;

                    // Save the VC type.
                    Genericlex();
                    ptr=Genericlval.stype;
                    Letters[LettersIdx].VC=toupper(*ptr);

                    // Save the Unknown field (always 'L');
                    Genericlex();
                    ptr=Genericlval.stype;
                    Letters[LettersIdx].Unknown1=toupper(*ptr);

                    // Save all the 14 letter features.
                    for(int f=0;f<14;f++){
                        Genericlex();
                        ptr=Genericlval.stype;
                        Letters[LettersIdx].Features[f]=atof(ptr);
                    }

                    LettersIdx++;
                    break;
                case TKG_EOL:
                    debug0("  End-of-Line\n");
                    break;
                default:
                    debug1("  ERROR:Unknown token=%d\n",Token);
                    break;
            }
        }



        // Close our file stream as long as it isn't stdin.
        if(fh!=stdin) fclose(fh);
    }

    // If the input file didn't have a space character then lets add it.
    if(!FLG_FoundSpace){
        Letters[LettersIdx].Letter   = ' ';  // Letter is space.
        Letters[LettersIdx].VC       = 'N';  // It is neither a Vowel or Consonant
        Letters[LettersIdx].Unknown1 = 'N';  // It is not a letter (!isalpha())
        // Set all the 14 letter features to zero (the proper value for a space).
        for(int f=0;f<14;f++){
            Letters[LettersIdx].Features[f]=0.0;
        }
    }
    return(rtn);
}










//------------------------------------------------------------------------------
//  XXXXXXx  .x
//  X     X. .x
//  X     .x .x
//  X     .x .xxXXX    XXX.  .x.XXX    XXX.  .x.XXX .XXX    XXX.   .XXXX
//  X     X. .X.  xx  X.  X. .X.  xx  X.  X. .X.  xX.  xx  X.  X.  X   xx
//  XXXXXX.  .X   .x xx   xx .X   .x .x   .x .X   .X   .x .x   .x .x
//  X        .x   .x x.   .x .x   .x xXXXXXX .x   .x   .x xXXXXXX  xXX
//  X        .x   .x x.   .x .x   .x xx      .x   .x   .x xx         .XX.
//  X        .x   .x xx   xx .x   .x .x      .x   .x   .x .x           .x
//  X        .x   .x  X.  X. .x   .x  X.  xx .x   .x   .x  X.  xx .x   xx
//  X        .x   .x   XXX.  .x   .x   XXXx  .x   .x   .x   XXXx   .XXXx
//
//
// This is an example 'phonemes' file.  It keeps the Phoneme <-> Vowel/Consonant
// correspondence.  The first field seems to be a one character encoding of each
// phoneme in the language.  The second field is 'V' or 'C' which would seem to
// be representations of (V)owel and (C)onsonant.

//    1 V
//    2 V
//    3 V
//    4 V
//    5 V
//    6 V
//    7 V
//    8 V
//    9 V
//    D C
//    E V
//    I V
//    J C
//    N C
//    Q V
//    S C
//    T C
//    U V
//    V V
//    Z C
//    b C
//    d C
//    f C
//    g C
//    h C
//    i V
//    j C
//    k C
//    l C
//    m C
//    n C
//    p C
//    r C
//    s C
//    t C
//    u V
//    v C
//    w C
//    x C
//    z C
//    # V
//    { V
//    _ C
//    W V
//
//------------------------------------------------------------------------------

t_phoneme Phonemes[MAXPHONEMES];
int       PhonemesIdx=0;

//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
void CleanPhonemesFile(void)
{
    for(int i=0;i<MAXPHONEMES;i++){
        Phonemes[i].PhonemeCode=0;
        Phonemes[i].VC=0;
    }
}


//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
void WritePhonemesFile(char* FileName)
{
    // Special parsing.
    FILE* fh=NULL;

    if((fh=fopen(FileName,"w"))!=NULL){
        for(int i=0;i<PhonemesIdx;i++){
            pfprintf(fh,"%c %c\n",
                Phonemes[i].PhonemeCode,
                Phonemes[i].VC);
        }
        fclose(fh);
    }else{
        perror("ERROR: Language-Vocabulary File");
    }
}


//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
int ReadPhonemesFile(char* FileName)
{
    FILE* fh=NULL;
    int   rtn=1;
    char* ptr;
    bool  FLG_FoundSpace=false;

    if(strcmp("--",FileName)==0){
        // Told to take from stdin ...
        fh=stdin;
    }else{
        // Else open up a file.
        fh=fopen(FileName,"r");
        if(fh==NULL){
            perror("ERROR: Language-Phoneme File");
            rtn=0;
        }
    }
    if(fh!=NULL){
        CleanPhonemesFile();
        CommentType = COM_None;
        File_Line_Num = 0;
        // If from stdin or the file open worked then get the first line
        // into our display buffer.  Then seek back to the beginning to start
        // the parsing of the file.  Strangely, the fseek() works on both disk
        // files and stdin streams!
        if(fgets(File_Line,MAXLINE,fh)!=NULL){
            File_Line[strlen(File_Line)-1]=0;
            fseek(fh,0,SEEK_SET);
        }
        // Point the parser input to our open file stream and parse it.
        Genericin=fh;

        PhonemesIdx=0;
        int Token=Genericlex();
        while(Token!=0){
            debug1("Token(%d)\n",Token);
            switch(Token){
                case TKG_Comment:
                    debug1("  Comment='%s'\n",Genericlval.stype);
                    Token=Genericlex();
                    break;
                case TKG_Token:
                    if(PhonemesIdx>=MAXPHONEMES){
                        pfprintf(stderr,"ERROR: Number of lines in Language-Phonemes file exceeds table size.\n");
                        Genericrestart(fh);
                        if(fh!=stdin) fclose(fh);
                        return(0);
                    }
                    debug1("  Token='%s'\n",Genericlval.stype);
                    // Save the letter
                    ptr=Genericlval.stype;
                    Phonemes[PhonemesIdx].PhonemeCode=(*ptr);
                    if((*ptr)==' ') FLG_FoundSpace=true;

                    // Save the VC type.
                    Genericlex();
                    ptr=Genericlval.stype;
                    Phonemes[PhonemesIdx].VC=(*ptr);

                    while(Token!=TKG_EOL) Token=Genericlex();

                    PhonemesIdx++;
                    break;
                case TKG_EOL:
                    debug0("  End-of-Line\n");
                    Token=Genericlex();
                    break;
                default:
                    debug1("  ERROR:Unknown token=%d\n",Token);
                    Token=Genericlex();
                    break;
            }
        }



        // Close our file stream as long as it isn't stdin.
        if(fh!=stdin) fclose(fh);
    }
    // If the input file didn't have a space character then lets add it.
    if(!FLG_FoundSpace){
        Phonemes[PhonemesIdx].PhonemeCode   = ' ';  // Phoneme is space.
        Phonemes[PhonemesIdx].VC       = 'N';       // It is neither a Vowel or Consonant
    }
    return(rtn);
}













//------------------------------------------------------------------------------
//  XXXXXXx                                     .  .x
//  X     X.                                   .x
//  X     .x                                   .x
//  X     .x .xxXx  XXX.  .xxXX.    XXX.  .xxXXXXX .x   XXX.   .XXXX
//  X     X. .X.   X.  X. .Xx  X.  X.  X. .X.  .x  .x  X.  X.  X   xx
//  XXXXXX.  .X   xx   xx .X   .x .x   .x .X   .x  .x .x   .x .x
//  X        .x   x.   .x .x   .X xXXXXXX .x   .x  .x xXXXXXX  xXX
//  X        .x   x.   .x .x   .X xx      .x   .x  .x xx         .XX.
//  X        .x   xx   xx .X   .x .x      .x   .x  .x .x           .x
//  X        .x    X.  X. .Xx  X.  X.  xx .x   .X  .x  X.  xx .x   xx
//  X        .x     XXX.  .xxXX.    XXXx  .x    XX..x   XXXx   .XXXx
//                        .x
//
//
// This is an example 'properties' file.  It takes the form <property>=<value>
//    Name=Australian English
//    Version=1.1.6
//    ReleaseDate=April 2nd, 2009
//    Creator=Macquarie Centre for Cognitive Science, Macquarie University, Australia.
//    Url=http://www.maccs.mq.edu.au/~ssaunder/DRC/
//    DefaultNumOrthAnalysisUnits=8
//    DefaultNumPhonemeUnits=8
//
//------------------------------------------------------------------------------
const char* PropDelim="=\n";

t_Properties Properties[MAXPROPROPERTIES];
int          PropertiesIdx=0;

//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
void CleanPropertiesFile(void)
{
    for(int i=0;i<PropertiesIdx;i++){
        if(Properties[i].Name!=NULL) free(Properties[i].Name);
        if(Properties[i].Value!=NULL) free(Properties[i].Value);

        Properties[i].Name=NULL;
        Properties[i].Value=NULL;
    }
    for(int i=PropertiesIdx;i<MAXPROPROPERTIES;i++){
        Properties[i].Name=NULL;
        Properties[i].Value=NULL;
    }
}

//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
char* trim(char *buf)
{
    if(buf!=NULL){
        // Get the length of the string and...
        int len=min(strlen(buf),MAXLINE);
        // Trim the whitespace off the end.
        for(;(len>=0)&&(isspace(buf[len-1]));len--) {
            buf[len-1]=0;
        }
        // Now move the pointer up until we have passed the whitespace.
        for(;isspace(*buf);buf++);
    }
    return(buf);
}

//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
int ReadPropertiesFile(char* FileName)
{
    // Special parsing.
    FILE* fh=NULL;
    char* pToken=NULL;
    int   rtn=1;
    char  buf[MAXLINE];

    PropertiesIdx=0;
    if((fh=fopen(FileName,"r"))!=NULL){
        CleanPropertiesFile();
        while(fgets(buf,MAXLINE,fh)!=NULL){
            pToken=strtok(buf,PropDelim);
            if(pToken!=NULL){
                pToken=trim(pToken);
                if((*pToken!='#')&&(*pToken!='\n')){
                    if(PropertiesIdx>=MAXPROPROPERTIES){
                        pfprintf(stderr,"ERROR: Number of lines in Language-Properties file exceeds table size.\n");
                        Genericrestart(fh);
                        if(fh!=stdin) fclose(fh);
                        return(0);
                    }
                    Properties[PropertiesIdx].Name=strdup(pToken);
                    pToken=strtok(NULL,PropDelim);
                    if(pToken!=NULL){
                        pToken=trim(pToken);
                        Properties[PropertiesIdx].Value=strdup(pToken);
                    }else{
                        Properties[PropertiesIdx].Value=strdup("<missing value>");
                    }
                    PropertiesIdx++;
                }
            }
        }
        fclose(fh);
    }else{
        perror("ERROR: Properties File");
        rtn=0;
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
void WritePropertiesFile(char* FileName)
{
    // Special parsing.
    FILE* fh=NULL;

    if((fh=fopen(FileName,"w"))!=NULL){
        for(int p=0;p<PropertiesIdx;p++){
            pfprintf(fh,"%s = %s\n",Properties[p].Name,Properties[p].Value);
        }
        fclose(fh);
    }else{
        perror("ERROR: Properties File");
    }
}











//------------------------------------------------------------------------------
// X.     x.                        .x              .x
// xx     X                         .x              .x
// .x    .x                         .x              .x
//  X    x.   XXX.    XXX.   .XXXX  .xxXX.  .x   .x .x  .XXXX  .xxXxX.   x.
//  x.   X   X.  X.  X. .X  .X   xx .X.  X. .x   .x .x .X   xx .X.  xx   X
//  .x  .x  xx   xx .x           .x .X   .x .x   .x .x      .x .X   .X  .x
//   X  x.  x.   .x xx        xXXXx .x   .X .x   .x .x   xXXXx .x    x. x.
//   x. X   x.   .x xx      .Xx  .x .x   .X .x   .x .x .Xx  .x .x    .x X
//   .x.x   xx   xx .x      x.   xx .x   .x .x   .x .x x.   xx .x     X.x
//    Xx.    X.  X.  X.  X. xx  .Xx .X.  X. .X   Xx .x xx  .Xx .x     xX.
//    xX      XXX.    XXX.   XXXx.X .xxXX.   .XXX.x .x  XXXx.X .x     .X
//                                                                    .x
//
// This is an example 'vocabulary' file.  The fields are as follows:
//      1) Word        -string of one or more alphabetic characters.
//      2) Phoneme     -string of one or more phonemes corresponding to word in field 1.
//      3) Unknown     -appears to always be the two character symbol 'OP'.
//      4) ?WordFreq?  -This may be the word frequency(occurances) in a particular corpus.
//      5) Unknown     -Unknown integer.

//    a 1 OP 779072 65599
//    ace 1s OP 66 1
//    ache 1k OP 86 0
//    ached 1kt OP 70 0
//    aches 1ks OP 39 1
//    act {kt OP 3159 195
//    acts {kts OP 629 41
//    ad {d OP 102 5
//    add {d OP 1376 132
//    adds {dz OP 239 8
//    ads {dz OP 60 3
//    adze {dz OP 2 0
//    aft #ft OP 17 2
//    age 1_ OP 4159 278
//    aged 1_d OP 584 22
//    ah # OP 937 270
//    aid 1d OP 943 76
//    aide 1d OP 100 0
//    aides 1dz OP 75 1
//    aids 1dz OP 103 14
//    ail 1l OP 136 0
//    ailed 1ld OP 5 0
//    ails 1lz OP 6 0
//    aim 1m OP 697 38
//    aimed 1md OP 330 5
//    aims 1mz OP 378 22
//    air 8 OP 4394 102
//    aired 8d OP 20 0
//    airs 8z OP 35 0
//    aisle 2l OP 128 1
//    aisles 2lz OP 44 1
//    aitch 1J OP 4 2
//    al {l OP 229 39
//    alb {lb OP 0 0
//    albs {lbz OP 1 0
//    ale 1l OP 55 1
//    ales 1lz OP 6 1
//    all 9l OP 58999 5396
//    alms #mz OP 13 0
//    alp {lp OP 4 0
//    alps {lps OP 71 0
//------------------------------------------------------------------------------

t_vocabulary Vocabulary[MAXVOCAB];
int          VocabularyIdx=MINVOCAB;       // Offset by 1(MINVOCAB) because indices need to accept a sign (ie. negative).
int          DiscardedWords=0;
int          HomoGraphemeCombined=0;
int          HomoPhonemeCombined=0;
DRC_Float lfMaxWrittenWordFreq;
DRC_Float lfMaxSpokenWordFreq;


//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
void CleanVocabularyFile(void)
{
    for(int i=MINVOCAB;i<MAXVOCAB;i++){
        memset(Vocabulary[i].Word,0,sizeof(Vocabulary[i].Word));
        memset(Vocabulary[i].Phoneme,0,sizeof(Vocabulary[i].Phoneme));
        memset(Vocabulary[i].Unknown1,0,sizeof(Vocabulary[i].Unknown1));
        Vocabulary[i].Written.Freq=0;
        Vocabulary[i].Written.SumFreq=0;
        Vocabulary[i].Written.CFS=0.0;
        Vocabulary[i].Spoken.Freq=0;
        Vocabulary[i].Spoken.SumFreq=0;
        Vocabulary[i].Spoken.CFS=0.0;
    }
}

//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
void  PrtVocabWord(FILE* fh,char* word)
{
    char buf[WORDSZEXTN];
    buf[0]=0;
    if(word[0]==0){
        strcat(buf,"(");
        strcat(buf,word+1);
        strcat(buf,")");
    }else{
        strcat(buf,word);
    }
    pfprintf(fh,"%-10s  ",buf);
}

//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
void  PrtVocabLine(FILE* fh,int idx)
{
    pfprintf(fh,"%5d) ",idx);
    PrtVocabWord(fh,Vocabulary[idx].Word);
    PrtVocabWord(fh,Vocabulary[idx].Phoneme);
    pfprintf(fh,"%2s %7d(%+8.6f):wrt %7d(%+8.6f):spk HoPh=%5d%c HoGr=%5d%c\n",
        Vocabulary[idx].Unknown1,
        Vocabulary[idx].Written.Freq,
        Vocabulary[idx].Written.CFS,
        Vocabulary[idx].Spoken.Freq,
        Vocabulary[idx].Spoken.CFS,
        abs(Vocabulary[idx].Homophone),
        (Vocabulary[idx].Homophone<0)?'K':' ',
        abs(Vocabulary[idx].Homograph),
        (Vocabulary[idx].Homograph<0)?'K':' '
    );
}

//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
void WriteVocabularyFile(char* FileName)
{
    // Special parsing.
    FILE* fh=NULL;

    if((fh=fopen(FileName,"w"))!=NULL){
        pfprintf(fh,"MaxWrittenWordFreq=%d\n",MaxWrittenWordFreq);
        pfprintf(fh,"MaxSpokenWordFreq=%d\n\n",MaxSpokenWordFreq);
        pfprintf(fh,"       Graphemic   Phonemic           Written                Spoken                     Chain          \n");
        pfprintf(fh,"Index  Word        Word        ??  Freq      CFS          Freq      CFS         Homophone   Homograph  \n");
        pfprintf(fh,"====== =========== =========== == ======= =========      ======= =========      ==========  ========== \n");
        for(int i=MINVOCAB;i<VocabularyIdx;i++){
            PrtVocabLine(fh,i);
        }
        fclose(fh);
    }else{
        perror("ERROR: Language-Vocabulary File");
    }
}

// This is a qsort() helper routine (callback) that sorts two individual words
// based on the grapheme order and it also sorts the empty entries to the end of the array.
int WordSort(const void *a,const void *b)
{
    t_vocabulary* EntryA=(t_vocabulary*)a;
    t_vocabulary* EntryB=(t_vocabulary*)b;
    int rtn=strncmp(EntryA->Word,EntryB->Word,WORDSZ);

    if(rtn==0){
        // If we just tripped across homographs then sort with the phoneme.
        rtn=strncmp(EntryA->Phoneme,EntryB->Phoneme,WORDSZ);
    }else{
        // Sort NULL entries to end;
        if(EntryA->Word[0]==0){
            rtn=+1;
        }else if(EntryB->Word[0]==0){
            rtn=-1;
        }
    }

    return(rtn);
}

// This is a qsort() helper routine (callback) that sorts two individual phoneme
// strings based on the phoneme's grapheme order and it the empty entries are
// sorted to the end of the array.
//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
int PhonemeSort(const void *a,const void *b)
{
    t_vocabulary* EntryA = &Vocabulary[*(int*)a];
    t_vocabulary* EntryB = &Vocabulary[*(int*)b];
    int rtn=strncmp(EntryA->Phoneme,EntryB->Phoneme,WORDSZ);

    if(rtn==0){
        // If we tripped across a homophone then sort with the OIL word.
        rtn=strncmp(EntryA->Word,EntryB->Word,WORDSZ);
    }else{
        // Sort NULL entries to end;
        if(EntryA->Phoneme[0]==0){
            rtn=+1;
        }else if(EntryB->Phoneme[0]==0){
            rtn=-1;
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
DRC_Float CFSCalc(int Freqency,DRC_Float lfMaximum)
{
    DRC_Float lfWord=log10((DRC_Float)(Freqency+1));  // The +1 is a fudge factor because log(0) is undefined
    DRC_Float Rtn=((lfWord/lfMaximum)-1.0)*GP_FrequencyScale;
    return(Rtn);
}

static void strlwr(char *str)
{
    char* p = str;
    for (; *p != '\0'; ++p)
        *p = tolower(*p);
}

//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
int ReadVocabularyFile(char* FileName)
{
    FILE* fh=NULL;
    int   rtn=1;
    char* ptr;

    int WrittenWordFreq=0;
    int SpokenWordFreq=0;

    if(strcmp("--",FileName)==0){
        // Told to take from stdin ...
        fh=stdin;
    }else{
        // Else open up a file.
        fh=fopen(FileName,"r");
        if(fh==NULL){
            perror("ERROR: Language-vocabulary File");
            rtn=0;
        }
    }
    if(fh!=NULL){
        CleanVocabularyFile();
        CommentType = COM_BeginOfLine;
        File_Line_Num = 0;
        // If from stdin or the file open worked then get the first line
        // into our display buffer.  Then seek back to the beginning to start
        // the parsing of the file.  Strangely, the fseek() works on both disk
        // files and stdin streams!
        if(fgets(File_Line,MAXLINE,fh)!=NULL){
            File_Line[strlen(File_Line)-1]=0;
            fseek(fh,0,SEEK_SET);
        }
        // Point the parser input to our open file stream and parse it.
        Genericin=fh;

        VocabularyIdx=MINVOCAB;

        int Token=0;
        while((Token=Genericlex())!=0){
            debug1("Token(%d)\n",Token);
            switch(Token){
                case TKG_Comment:
                    debug1("  Comment='%s'\n",Genericlval.stype);
                    break;
                case TKG_Token:
                    if(VocabularyIdx>=MAXVOCAB){
                        pfprintf(stderr,"ERROR: Number of lines in Language-Vocabulary file exceeds table size.\n");
                        Genericrestart(fh);
                        if(fh!=stdin) fclose(fh);
                        return(0);
                    }
                    debug1("  Token='%s'\n",Genericlval.stype);
                    // Save the word
                    ptr=Genericlval.stype;
                    strlwr(ptr);
                    if(strlen(ptr)<=WORDSZ){
                        strncpy(Vocabulary[VocabularyIdx].Word,ptr,WORDSZ);
                        Vocabulary[VocabularyIdx].Word[WORDSZ]=0;

                        // Save the associated phoneme (string).
                        Genericlex();
                        ptr=Genericlval.stype;
                        strncpy(Vocabulary[VocabularyIdx].Phoneme,ptr,WORDSZ);
                        Vocabulary[VocabularyIdx].Phoneme[WORDSZ]=0;

                        // Save the Unknown1 field
                        Genericlex();
                        ptr=Genericlval.stype;
                        strncpy(Vocabulary[VocabularyIdx].Unknown1,ptr,2);
                        Vocabulary[VocabularyIdx].Unknown1[3]=0;

                        // Save the written word frequency field
                        Genericlex();
                        ptr=Genericlval.stype;
                        WrittenWordFreq=atoi(ptr);
                        Vocabulary[VocabularyIdx].Written.Freq=WrittenWordFreq;
                        Vocabulary[VocabularyIdx].Written.SumFreq=WrittenWordFreq;
                        if(WrittenWordFreq>MaxWrittenWordFreq) {
                            MaxWrittenWordFreq=WrittenWordFreq;
                        }

                        // Save the spoken word frequency field
                        Genericlex();
                        ptr=Genericlval.stype;
                        SpokenWordFreq=atoi(ptr);  // Fudge factor in DRC1.2 (due to log10)
                        Vocabulary[VocabularyIdx].Spoken.Freq=SpokenWordFreq;
                        Vocabulary[VocabularyIdx].Spoken.SumFreq=SpokenWordFreq;
                        if(SpokenWordFreq>MaxSpokenWordFreq) {
                            MaxSpokenWordFreq=SpokenWordFreq;
                        }

                        // Add this entry to our LL/PB index list.
                        OILPOL_AddWords(VocabularyIdx);

                        VocabularyIdx++;
                    }else{
                        if(FindBParam("Warnings")->Value){
                            pfprintf(stderr,"WARNING: Vocabulary word '%s' discarded: too long.\n",ptr);
                        }
                        // Make sure we eat the rest of the line.
                        Genericlex();
                        Genericlex();
                        Genericlex();
                        Genericlex();
                        DiscardedWords++;
                    }
                    break;
                case TKG_EOL:
                    debug0("  End-of-Line\n");
                    break;
                default:
                    debug1("  ERROR:Unknown token=%d\n",Token);
                    break;
            }
        }

        // Close our file stream as long as it isn't stdin.
        if(fh!=stdin) fclose(fh);
    }





    // Sort the entire table by words so we can be sure words are in order by orthography.
    qsort(Vocabulary+1,MAXVOCAB-1,sizeof(t_vocabulary),WordSort);
    // Now the homographs will all be together.  We just need to go through the
    // list linking them together so they might be found later in the layer
    // updating process.  This produces a circular list.
    // Fixup the Homographs.

    // Set all the Homophone/Homograph pointers to point to their own entry (meaning
    // that this entry is neither a Homophone or Homograph.  We need to do this after
    // the qsort because the qsort can change the order.
    for(int w=MINVOCAB; w<VocabularyIdx; w++){
        Vocabulary[w].Homophone=w;
        Vocabulary[w].Homograph=w;
    }

    // The DRC1.2 software has this switch on as a default (it treats the flag as
    // invalid).  However this software will allow you to turn it off.
    if(!FindBParam("SeparateHomographemes")->Value){
        pfprintf(fh_homograph,"Linking Homographs:\n");
        for(int w=MINVOCAB+1; w<VocabularyIdx; w++){
            // If the two words are the same...
            if(strncmp(Vocabulary[w-1].Word,Vocabulary[w].Word,WORDSZ)==0){
                HomoGraphemeCombined++;
                // See if we have indication that we've linked in other words yet.
                if(Vocabulary[w-1].Homograph == (w-1)){
                    // Nope ... then link these together
                    // Link the first two homographs together.
                    Vocabulary[w-1].Homograph=w;
                    Vocabulary[w].Homograph=-(w-1);     // Make current Key word (negative pointer).
                    // Record homographs in a file.
                    if(fh_homograph!=NULL){
                        // Test fh_homograph to avoid calculations in argument list.
                        pfprintf(fh_homograph,"  [%d]%s\\%s <--> [%d]%s\\%s\n",
                            w-1,Vocabulary[w-1].Word,Vocabulary[w-1].Phoneme,
                            w,Vocabulary[w].Word,Vocabulary[w].Phoneme);
                    }
                }else{
                    // For the third, fourth etc homograph in the list link this
                    // homograph to the first one and link the previous to this one.
                    // NOTE: DO NOT interchange next two lines.
                    Vocabulary[w].Homograph=-(abs(Vocabulary[w-1].Homograph)); // Make this one the key word.
                    Vocabulary[w-1].Homograph=w;                               // Previous no longer key word.
                    // Record homographs in a file.
                    if(fh_homograph!=NULL){
                        // Test fh_homograph to avoid calculations in argument list.
                        pfprintf(fh_homograph,"  [%d]%s\\%s --> [%d]%s\\%s --> [%d]%s\\%s (insert)\n",
                            w-1,Vocabulary[w-1].Word,Vocabulary[w-1].Phoneme,
                            w,Vocabulary[w].Word,Vocabulary[w].Phoneme,
                            Vocabulary[w].Homograph,Vocabulary[abs(Vocabulary[w].Homograph)].Word,
                                Vocabulary[abs(Vocabulary[w].Homograph)].Phoneme);
                    }
                }
                // Roll the abandoned word's freqency into the key word Summed frequency.
                // The summed frequency of all words is initially the basic frequency.
                Vocabulary[w].Written.SumFreq =
                    Vocabulary[w-1].Written.SumFreq
                    + Vocabulary[w].Written.Freq;

                if(Vocabulary[w].Written.SumFreq>MaxWrittenWordFreq){
                    // Only give a warning if requested to.
                    if(FindBParam("Warnings")->Value){
                        pfprintf(stdout,"WARNING: Word '%s'/%s/ has increased the max written word freq.\n",
                            Vocabulary[w].Word, Vocabulary[w].Phoneme);
                        pfprintf(stdout,"         Old=%d, New=%d\n",MaxWrittenWordFreq,Vocabulary[w].Written.SumFreq);
                    }
                    MaxWrittenWordFreq=Vocabulary[w].Written.SumFreq;
                }
            }
        }
    }



    // Create phoneme ancillary index array and sort the phonemes without disturbing
    // the sort of the vocabulary table.  The homophone pointers are in the vocabulary
    // array and have already been set to point to their own record.
    for(int w=0; w<MAXVOCAB; w++){
        PhonemeList[w]=w;
    }
    // Now sort ancillary table to get all the homophones grouped together.
    qsort(PhonemeList,MAXVOCAB,sizeof(int),PhonemeSort);

    // The DRC1.2 software has this switch on as a default (it treats the flag as
    // invalid).  However this software will allow you to turn it off.
    if(!FindBParam("SeparateHomophones")->Value){

        // Fixup the Homophones.  This ammounts to producing a circular list in the
        // .Homophone elements.  We will display the likages as they are produced.
        pfprintf(fh_homophone,"Linking Homophones:\n");
        for(int w=1; w<VocabularyIdx; w++){
            // Use the ancillary index array to give us pointers into the Vocabulary
            // array. Since the PhonemeList adjacent pointers point to phonetic words
            // that are alphabetically adjacent we can use them to fixup the Homophonemes.
            int idx1=PhonemeList[w-1];
            int idx2=PhonemeList[w];
            // If the two phonemic words are the same...
            if(strncmp(Vocabulary[idx1].Phoneme,Vocabulary[idx2].Phoneme,WORDSZ)==0){
                HomoPhonemeCombined++;
                // See if we have indication that we've linked in other words yet.
                if(Vocabulary[idx1].Homophone == idx1){
                    // Nope ... these are the first and second entries for this homophone list.
                    // For the first two homographs link them to each other.
                    Vocabulary[idx1].Homophone=idx2;
                    Vocabulary[idx2].Homophone=-idx1;   // Make the last one the key word.
                    // Record homophones in a file.
                    if(fh_homophone!=NULL){
                        // Test fh_homophone to avoid calculations in argument list.
                        pfprintf(fh_homophone,"  [%d]%s\\%s <--> [%d]%s\\%s\n",
                            idx1,Vocabulary[idx1].Word,Vocabulary[idx1].Phoneme,
                            idx2,Vocabulary[idx2].Word,Vocabulary[idx2].Phoneme);
                    }
                }else{
                    // For the third, fourth etc homograph in the list link this
                    // homograph to the first one and link the previous to this one.
                    // NOTE: DO NOT interchange next two lines.
                    Vocabulary[idx2].Homophone=-(abs(Vocabulary[idx1].Homophone));   // Make last in list keyword.
                    Vocabulary[idx1].Homophone=idx2;
                    // Record homophones in a file.
                    if(fh_homophone!=NULL){
                        // Test fh_homophone to avoid calculations in argument list.
                        pfprintf(fh_homophone,"  [%d]%s\\%s --> [%d]%s\\%s --> [%d]%s\\%s (insert)\n",
                            idx1,Vocabulary[idx1].Word,Vocabulary[idx1].Phoneme,
                            idx2,Vocabulary[idx2].Word,Vocabulary[idx2].Phoneme,
                            Vocabulary[idx2].Homophone,Vocabulary[abs(Vocabulary[idx2].Homophone)].Word,
                                Vocabulary[abs(Vocabulary[idx2].Homophone)].Phoneme);
                    }
                }

                // Roll the abandoned word's freqency into the key word.
                Vocabulary[idx2].Spoken.SumFreq =
                    Vocabulary[idx1].Spoken.SumFreq
                    + Vocabulary[idx2].Spoken.Freq;

                if(Vocabulary[idx2].Spoken.SumFreq > MaxSpokenWordFreq){
                    // Only give warnings when requested to.
                    if(FindBParam("Warnings")->Value){
                        pfprintf(stdout,"WARNING: Phoneme '%s'/%s/ has increased the max Spoken word freq.\n",
                            Vocabulary[idx2].Phoneme,Vocabulary[idx2].Word);
                        pfprintf(stdout,"         Old=%d, New=%d\n",MaxSpokenWordFreq,Vocabulary[idx2].Spoken.SumFreq);
                    }
                    MaxSpokenWordFreq=Vocabulary[idx2].Spoken.SumFreq;
                }
            }
        }
    }

    // Maked the summed entries of the homograph and homophones the same as the key element.
    for(int w=MINVOCAB; w<VocabularyIdx; w++){
        int start,end,sumfreq;
        if(Vocabulary[w].Homograph<0){
            end=w;
            start=end;
            sumfreq=Vocabulary[w].Written.SumFreq;
            do{
                start=abs(Vocabulary[start].Homograph);
                Vocabulary[start].Written.SumFreq=sumfreq;
            }while(start!=end);
        }

        if(Vocabulary[w].Homophone<0){
            end=w;
            start=end;
            sumfreq=Vocabulary[w].Spoken.SumFreq;
            do{
                start=abs(Vocabulary[start].Homophone);
                Vocabulary[start].Spoken.SumFreq=sumfreq;
            }while(start!=end);
        }
    }






    // Calculate all the CFS values now that we have the max frequency from
    // the lexicon (or homophone rearrangements) and the homophone freqencies
    // updated.
    lfMaxWrittenWordFreq=log10((DRC_Float)(MaxWrittenWordFreq+1));
    lfMaxSpokenWordFreq=log10((DRC_Float)(MaxSpokenWordFreq+1));
    for(int w=MINVOCAB; w<VocabularyIdx; w++){
        // Determine this word's written frequency scaling resting state
        Vocabulary[w].Written.CFS=CFSCalc(Vocabulary[w].Written.SumFreq,lfMaxWrittenWordFreq);

        // Determine this word's spoken frequency scaling resting state
        if(FindBParam("CFSPOLEqualOIL")->Value){
            Vocabulary[w].Spoken.CFS=Vocabulary[w].Written.CFS;
        }else{
            Vocabulary[w].Spoken.CFS=CFSCalc(Vocabulary[w].Spoken.SumFreq,lfMaxSpokenWordFreq);
        }
    }

    return(rtn);
}

//---------------------------------------------------------------------------
// Routine: Just do a string compare on the two given strings adjusting for
//          the possibility that one or both of the strings might be prefixed
//          with a zero character.
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
int SpcStrNCmp(char* str1, char* str2, int len)
{
    int rtn;
    char* pstr1=str1;
    char* pstr2=str2;
    if(str1[0]==0){
        pstr1++;
    }
    if(str2[0]==0){
        pstr2++;
    }
    rtn=strncmp(pstr1,pstr2,len);
    return(rtn);
}
//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
void DisplayHomographs(FILE* fh)
{
    bool InList=false;
    char buf[BUFFERSZ];
    char buf2[BUFFERSZ];

    pfprintf(fh,"\n\nMax. Freq: Written=%d, Spoken=%d.\n\n",MaxWrittenWordFreq,MaxSpokenWordFreq);
    pfprintf(fh,"Graphemic Word           Corresponding Phonemic Words\n");
    pfprintf(fh,"======================== ============================================\n");
    for(int w=MINVOCAB+1; w<VocabularyIdx; w++){
        int w1=w-1;
        int w2=w;
        if(SpcStrNCmp(Vocabulary[w1].Word,Vocabulary[w2].Word,WORDSZ)==0){
            if(!InList){
                // First pair of homophones detected.
                InList=true;
                memset(buf,' ',sizeof(buf));
                buf[25]=0;
                sprintf(buf2,"'%s'(%d,%8.6f)",
                    Vocabulary[w1].Phoneme,
                    Vocabulary[w1].Spoken.Freq,
                    Vocabulary[w1].Spoken.CFS);
                strcat(buf,buf2);
            }
            if(Vocabulary[w2].Homograph<0){
                sprintf(buf,"%-s (%d,%8.6f)",
                    Vocabulary[w2].Word,
                    Vocabulary[w2].Written.Freq,
                    Vocabulary[w2].Written.CFS);
                buf[strlen(buf)]=' ';
            }
            sprintf(buf2,", '%s'(%d,%8.6f)",
                Vocabulary[w2].Phoneme,
                Vocabulary[w2].Spoken.Freq,
                Vocabulary[w2].Spoken.CFS);
            strcat(buf,buf2);
        }else{
            if(InList){
                pfprintf(fh,"%s\n",buf);
                InList=false;
            }
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
void DisplayHomophones(FILE* fh)
{
    bool InList=false;
    char buf[500];
    char buf2[50];

    pfprintf(fh,"\n\nMax. Freq: Written=%d, Spoken=%d.\n\n",MaxWrittenWordFreq,MaxSpokenWordFreq);
    pfprintf(fh,"Phonemic Word            Corresponding Graphemic Words\n");
    pfprintf(fh,"======================== ============================================\n");
    for(int w=MINVOCAB+1; w<VocabularyIdx; w++){
        int w1=PhonemeList[w-1];
        int w2=PhonemeList[w];
        if(SpcStrNCmp(Vocabulary[w1].Phoneme,Vocabulary[w2].Phoneme,WORDSZ)==0){
            if(!InList){
                // First pair of homophones detected.
                InList=true;
                memset(buf,' ',25);
                buf[25]=0;
                sprintf(buf2,"'%s'(%d,%8.6f)",
                    Vocabulary[w1].Word,
                    Vocabulary[w1].Written.Freq,
                    Vocabulary[w1].Written.CFS);
                strcat(buf,buf2);
            }
            if(Vocabulary[w2].Homophone<0){
                sprintf(buf,"%-s (%d,%8.6f)",
                    Vocabulary[w2].Phoneme,
                    Vocabulary[w2].Spoken.Freq,
                    Vocabulary[w2].Spoken.CFS);
                buf[strlen(buf)]=' ';
            }
            sprintf(buf2,", '%s'(%d,%8.6f)",
                Vocabulary[w2].Word,
                Vocabulary[w2].Written.Freq,
                Vocabulary[w2].Written.CFS);
            strcat(buf,buf2);
        }else{
            if(InList){
                pfprintf(fh,"%s\n",buf);
                InList=false;
            }
        }
    }
}

FILE* OpenOutputFile(const char* ParentDir,const char* RunDir,const char* FileName,const char* Extn)
{
    char buf[MAXPATH];
    FILE* fh=NULL;
    int   file_id=0;

    for(file_id=0;file_id<MAX_FILE_ID;file_id++){
        if(file_id==0){
            // This file cannot be produced in MS Windows so we will
            // always suffix it with a number.
            if(strcasecmp(FileName,"con")==0) continue;

            sprintf(buf,"%s\\%s\\%s.%s",ParentDir,RunDir,FileName,Extn);
        }else{
            sprintf(buf,"%s\\%s\\%s-%d.%s",ParentDir,RunDir,FileName,file_id,Extn);
        }
        // Make sure the file doesn't exist...
        if(access(buf,0)!=0){
            // ... and if it doesn't create it.
            if((fh=fopen(buf,"w"))!=NULL) break;
        }
    }
    if(file_id>=MAX_FILE_ID){
        perror("ERROR: Opening file");
        pfprintf(stderr,"Cannot open any iterated file name:\n");
        pfprintf(stderr,"   From: <file><id>.<ext> where <id> is one of:\n");
        pfprintf(stderr,"         <empty>,\"-1\" ... \"-%d\"(MAX_FILE_ID)\n",MAX_FILE_ID);
        pfprintf(stderr,"LAST FILE='%s'\n",buf);
        exit(1);
    }
    return(fh);
}



//------------------------------------------------------------------------------
//  XXXXXX            .          .x
//  X    .X          .x          .x
//  X     x.         .x          .x
//  X     x.  .XXXX .XXX   XXX.  .xxXXX
//  X    .x  .X   xx .x   X. .X  .X.  xx
//  XXXXXXX       .x .x  .x      .X   .x
//  X     Xx   xXXXx .x  xx      .x   .x
//  X     .x .Xx  .x .x  xx      .x   .x
//  X     .x x.   xx .x  .x      .x   .x
//  X     X. xx  .Xx .X   X.  X. .x   .x
//  XXXXXX.   XXXx.X  XX.  XXX.  .x   .x
//
//
// ReadbatchFile() is a routine that will read and parse a batch file.
//
// It's functionality might be a bit mysterious.  The lexor will capture lines
// for display by using a token string that matches a newline and the following
// characters up to just before the next newline.  It then pushes everything back
// except for the initial newline and then prints this line (excluding the initial
// new line character).  Thus it can match the whole line and each individual
// token in the line.  The drawback to this method is that it requires the file
// to have a newline character at the beginning (which implies a blank line at
// the start of the file).  However, since it only prints the stuff after the
// newline it starts printing the file at the second line.
// To fix this problem we open the file and read the first line in and then
// reset the pointer (fseek).  This allows us to capture the first line and print
// it in a normal log file fashion.  Then we hand the processing back to the
// lexor which tokenizes everything and prints the lines and error statements.
//
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
int ReadBatchFile(char* FileName)
{
    FILE* fhBatch=NULL;
    FILE* fhBatchStats=NULL;
    int   rtn=1;
    char  buf[MAXPATH];
    char* BatchFileName=strdup(FileName);

    fh_lexlinedsp=stdout;
    if(strcmp("--",FileName)==0){
        // Told to take from stdin ...
        fhBatch=stdin;
        free(BatchFileName);
        BatchFileName=strdup("results");
        strcat(OutRunDirBuf,BatchFileName);
    }else{
        // Else open up batch file for reading.
        fhBatch=fopen(FileName,"r");
        if(fhBatch==NULL){
            perror("ERROR: Batch File");
            exit(1);
        }
        // Create the output directory name for the results.  Use the file name
        // without any special characters in it (only alphanumerics).
        for(int i=0,j=0;i<(int)strlen(FileName); i++){
            if(isalnum(FileName[i])){
                OutRunDirBuf[j++]=FileName[i];
            }
        }
    }

    // If we have a source of batch file commands...
    if(fhBatch!=NULL){
        if(FindBParam("Neighbourhood")->Value){
            DspNeighbourTtl(stdout);
            // Determine neighbourhood of each word in turn.
            while(fgets(File_Line,MAXLINE,fhBatch)!=NULL){
                // Ignore comments, blank lines and homographs.
                File_Line[strlen(File_Line)-1]=0;
                if((File_Line[0]!='#')&&(File_Line[0]!='\0')&&(File_Line[0]!=' ')){
                    char *ptr=strchr(File_Line,' ');
                    if(ptr!=NULL) *ptr=0;  // Get rid of stuff past first word.
                    DspNeighbourhood(stdout,File_Line);
                }
            }
        }else{
            // Create the output batch directory (same name as batch file).
            CreateOutDir(OutRunDirBuf);

            // Open the RT file for this batch.
            fhRTFile=OpenOutputFile(OutParentDirBuf,OutRunDirBuf,BatchFileName,"RT");

            ClearBatchStatistics();

            WriteRTFileHeader(fhRTFile,true,BatchFileName);

            File_Line_Num = 0;
            // If from stdin or the file open worked then get the first line
            // into our display buffer.  Then seek back to the beginning to start
            // the parsing of the file.  Strangely, the fseek() works on both disk
            // files and stdin streams!
            if(fgets(File_Line,MAXLINE,fhBatch)!=NULL){
                File_Line[strlen(File_Line)-1]=0;
                fseek(fhBatch,0,SEEK_SET);
            }

            // Point the parser input to our open file stream and parse it.
            Batchin=fhBatch;
            Batchparse();

            // Open the STATS file for this batch.
            fhBatchStats=OpenOutputFile(OutParentDirBuf,OutRunDirBuf,BatchFileName,"stats");
            if(fhBatchStats!=NULL){
                // Save the STATS file information
                WriteBatchStatsHdr(fhBatchStats);
                WriteBatchStats(fhBatchStats);
                fclose(fhBatchStats);
            }

            // Close our RT file stream.
            fclose(fhRTFile);
        }

        // Close our BATCH file stream as long as it isn't stdin.
        if(fhBatch!=stdin) fclose(fhBatch);
    }
    free(BatchFileName);
    fh_lexlinedsp=NULL;
    return(rtn);
}

void DspNeighbourTtl(FILE *fh)
{
    pfprintf(fh,"\n\nNonword      Status  Cnt Neighbouring Words\n");
    pfprintf(fh,"============ ======= === ============================================\n");
}

bool IsNeighbour(char* word, char* nonword)
{
    int len1=strlen(word);
    int len2=strlen(nonword);
    int len=max(len1,len2);
    bool rtn=true;

    if(abs(len2-len1)>1){
        // Neighbourhood words can only differ by one letter.
        rtn=false;
    }else{
        int errcnt=0;
        for(int i=0;i<len;i++){
            if(word[i]!=nonword[i]){
                errcnt++;
            }
        }
        if(errcnt>1) rtn=false;
    }
    return(rtn);
}

void DspNeighbourhood(FILE* fh,char* nonword){
    std::vector<int> VocabIdx;
    bool Word=false;

    // Clear the vector
    VocabIdx.erase(VocabIdx.begin(),VocabIdx.end());
    // Count and keep track of indices to words that are neighbours.
    for(int w=0; w<VocabularyIdx; w++){
        // Check to see if this word is a neighbour.
        if(IsNeighbour(Vocabulary[w].Word,nonword)){
            // Only process homographs if they are the key one (or nonhomographs).
            if((Vocabulary[w].Homograph==w)||(Vocabulary[w].Homograph<0)){
                VocabIdx.push_back(w);
            }
        }

        // Check to see if this word is our given 'nonword'
        if(strcmp(Vocabulary[w].Word,nonword)==0){
            Word=true;
        }
    }
    // Print the 'nonword's leadin.
    pfprintf(fh,"%-12s %7s %3d",nonword,(Word)?"WORD!!":"nonword",VocabIdx.size());
    if(VocabIdx.size()>0){
        // If there are neighbours print them too.
        for(int w=0;w<(int)VocabIdx.size();w++){
            pfprintf(fh," %s",Vocabulary[VocabIdx[w]].Word);
        }
    }
    pfprintf(fh,"\n");
}

void WriteBatchStatsHdr(FILE* fh)
{

    pfprintf(fh,"; OpenDRC Result Statistics File\n");
    DRC_DisplayHeader(fh);
}


void ClearBatchStatistics()
{
    Stats.Correct.Named.Count=0;
    Stats.Correct.Named.RTSummed=0.0;
    Stats.Correct.Named.RTSumSqr=0.0;
    Stats.Correct.Lowac.Count=0;
    Stats.Correct.Lowac.RTSummed=0.0;
    Stats.Correct.Lowac.RTSumSqr=0.0;

    Stats.Wrong.Named.Count=0;
    Stats.Wrong.Named.RTSummed=0.0;
    Stats.Wrong.Named.RTSumSqr=0.0;
    Stats.Wrong.Lowac.Count=0;
    Stats.Wrong.Lowac.RTSummed=0.0;
    Stats.Wrong.Lowac.RTSumSqr=0.0;

}

DRC_Float sqr(DRC_Float num)
{
    return(num*num);
}

void DspMean(FILE* fh,const char* good,const char* bad,t_StatList& Stats)
{
    if(Stats.Count!=0){
        DRC_Float Mean=Stats.RTSummed/Stats.Count;
        pfprintf(fh,good,Mean);
    }else{
        pfprintf(fh,bad);
    }
}

void DspStdDev(FILE* fh,const char* good,const char* bad,t_StatList& Stats)
{
    if(Stats.Count!=0){
        DRC_Float Mean=Stats.RTSummed/Stats.Count;
        DRC_Float StdDev=sqrt((Stats.RTSumSqr/(DRC_Float)Stats.Count) - sqr(Mean));
        pfprintf(fh,good,StdDev);
    }else{
        pfprintf(fh,bad);
    }
}

void RTTotal(t_StatList& Return,t_StatList& Stats1,t_StatList& Stats2)
{
    Return.Count=Stats1.Count + Stats2.Count;
    Return.RTSummed=Stats1.RTSummed + Stats2.RTSummed;
    Return.RTSumSqr=Stats1.RTSumSqr + Stats2.RTSumSqr;
}

void WriteBatchStats(FILE* fh)
{
    if(fh!=NULL){
        int   TotalRTCount=
                    Stats.Correct.Named.Count +
                    Stats.Correct.Lowac.Count +
                    Stats.Wrong.Named.Count +
                    Stats.Wrong.Lowac.Count;

        DRC_Float TotalRTSummed=
                    Stats.Correct.Named.RTSummed +
                    Stats.Correct.Lowac.RTSummed +
                    Stats.Wrong.Named.RTSummed +
                    Stats.Wrong.Lowac.RTSummed;

        DRC_Float TotalRTSumSqr=
                    Stats.Correct.Named.RTSumSqr +
                    Stats.Correct.Lowac.RTSumSqr +
                    Stats.Wrong.Named.RTSumSqr +
                    Stats.Wrong.Lowac.RTSumSqr;

        DRC_Float Mean=0.0;
        DRC_Float StdDev=0.0;
        t_StatList Totals;

        pfprintf(fh,"\n\n");
        pfprintf(fh,"                   CORRECT                       WRONG\n");
        pfprintf(fh,"           Named    Lowac    Total      Named    Lowac    Total\n");

        pfprintf(fh,"Count:     ");
        pfprintf(fh,"%5d    ",   Stats.Correct.Named.Count);
        pfprintf(fh,"%5d    ",   Stats.Correct.Lowac.Count);
        pfprintf(fh,"%5d      ",Stats.Correct.Named.Count + Stats.Correct.Lowac.Count);
        pfprintf(fh,"%5d    ",   Stats.Wrong.Named.Count);
        pfprintf(fh,"%5d    ",   Stats.Wrong.Lowac.Count);
        pfprintf(fh,"%5d\n",     Stats.Wrong.Named.Count + Stats.Wrong.Lowac.Count);

        pfprintf(fh,"Mean:    ");
        RTTotal(Totals,Stats.Correct.Named,Stats.Correct.Lowac);
        DspMean(fh,"%7.3f  ",  "      -  ",  Stats.Correct.Named);
        DspMean(fh,"%7.3f  ",  "      -  ",  Stats.Correct.Lowac);
        DspMean(fh,"%7.3f    ","      -    ",Totals);
        RTTotal(Totals,Stats.Wrong.Named,Stats.Wrong.Lowac);
        DspMean(fh,"%7.3f  ",  "      -  ",  Stats.Wrong.Named);
        DspMean(fh,"%7.3f  ",  "      -  ",  Stats.Wrong.Lowac);
        DspMean(fh,"%7.3f\n",  "      -  \n",Totals);

        pfprintf(fh,"StdDev:  ");
        RTTotal(Totals,Stats.Correct.Named,Stats.Correct.Lowac);
        DspStdDev(fh,"%7.3f  ",  "      -  ",  Stats.Correct.Named);
        DspStdDev(fh,"%7.3f  ",  "      -  ",  Stats.Correct.Lowac);
        DspStdDev(fh,"%7.3f    ","      -    ",Totals);
        RTTotal(Totals,Stats.Wrong.Named,Stats.Wrong.Lowac);
        DspStdDev(fh,"%7.3f  ",  "      -  ",  Stats.Wrong.Named);
        DspStdDev(fh,"%7.3f  ",  "      -  ",  Stats.Wrong.Lowac);
        DspStdDev(fh,"%7.3f\n",  "      -  \n",Totals);

        pfprintf(fh,"Total Count: %d ",TotalRTCount);
        Mean=TotalRTSummed/(DRC_Float)TotalRTCount;
        pfprintf(fh,"Mean: %7.3f ",Mean);
        StdDev=sqrt((TotalRTSumSqr/(DRC_Float)TotalRTCount) - sqr(Mean));
        pfprintf(fh,"StdDev: %7.3f\n",StdDev);

    }
}






//------------------------------------------------------------------------------
//  XXXXXX.        .x
//  X     Xx
//  X      X.
//  X      xx .xxXx.x X    xx  XXX.  .xxXx
//  X      .x .X.  .x x.   X  X.  X. .X.
//  X      .x .X   .x .x  .x .x   .x .X
//  X      .x .x   .x  X  x. xXXXXXX .x
//  X      xx .x   .x  x. X  xx      .x
//  X      X. .x   .x  .x.x  .x      .x
//  X     Xx  .x   .x   XX.   X.  xx .x
//  XXXXXX.   .x   .x   xX     XXXx  .x
//
//
// The DRC program also has the ability to read in and process command line
// arguments from a driver file.  The argument list will appear as in the
// usage help message.  Parameters may be placed by themselves or multiply on
// each input line.  The procedure should be recursive in case it is called by
// a '-p' parameter within the driver file.
//------------------------------------------------------------------------------
char*    Driver[MAXDRIVER];
int      DriverIdx=1; // No data in first entry because it is ignored by ProcessCmdLineArgs().

//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
int ReadDriverFile(char* FileName)
{
    FILE* fh=NULL;
    int   rtn=1;
    char* ptr;

    if(strcmp("--",FileName)==0){
        // Told to take from stdin ...
        fh=stdin;
    }else{
        // Else open up a file.
        fh=fopen(FileName,"r");
        if(fh==NULL){
            perror("ERROR: Driver File");
            rtn=0;
        }
    }
    if(fh!=NULL){
        CommentType = COM_BeginOfLine;
        File_Line_Num = 0;
        // If from stdin or the file open worked then get the first line
        // into our display buffer.  Then seek back to the beginning to start
        // the parsing of the file.  Strangely, the fseek() works on both disk
        // files and stdin streams!
        if(fgets(File_Line,MAXLINE,fh)!=NULL){
            File_Line[strlen(File_Line)-1]=0;
            fseek(fh,0,SEEK_SET);
        }
        // Point the parser input to our open file stream and parse it.
        Genericin=fh;

        DriverIdx=0;
        int Token=0;
        while((Token=Genericlex())!=0){
            debug1("Token(%d)\n",Token);
            switch(Token){
                case TKG_Comment:
                    debug1("  Comment='%s'\n",Genericlval.stype);
                    break;
                case TKG_Token:
                    if(DriverIdx>=MAXDRIVER){
                        pfprintf(stderr,"ERROR: Number of lines in command line file exceeds table size.\n");
                        Genericrestart(fh);
                        if(fh!=stdin) fclose(fh);
                        return(0);
                    }
                    debug1("  Token='%s'\n",Genericlval.stype);
                    // Save the word
                    ptr=Genericlval.stype;
                    Driver[DriverIdx]=strdup(ptr);

                    DriverIdx++;
                    break;
                case TKG_EOL:
                    debug0("  End-of-Line\n");
                    break;
                default:
                    debug1("  ERROR:Unknown token=%d\n",Token);
                    break;
            }
        }

        ProcessCmdLineArgs(DriverIdx,Driver);

        // Close our file stream as long as it isn't stdin.
        if(fh!=stdin) fclose(fh);
    }
    return(rtn);
}

//---------------------------------------------------------------------------
// Routine: Creates a unique output directory given the base name of the form:
//             <basename>.drc  OR
//             <basename>-<index>.drc
//          and places the name in the buffer 'OutRunDirBuf'.  The basename is
//          either the batch file name (compressed) or 'results' (for stdin) or
//          the name of the test string.
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
void CreateOutDir(char* OutDir)
{
    static bool  FLG_OutDirCreated=false;
    char         buf[MAXPATH];
    int          FileIndex=0;

    while(!FLG_OutDirCreated){
        if(FileIndex==0){
            // This directory cannot be produced in MS Windows so we will
            // always suffix it with a number.
            if(strcasecmp(OutDir,"con")==0) {
                FileIndex++;
                continue;
            }

            sprintf(buf,"%s.drc",OutDir);
        }else{
            sprintf(buf,"%s-%d.drc",OutDir,FileIndex);
        }

        // Try to make the directory to see if it already exists.
#ifdef __BORLANDC__
        if(mkdir(buf)>=0){
#elif __GNUC__
        if(mkdir(buf,0xffff)>=0){
#endif
            // Success, so mkdir() created it.
            strcpy(OutRunDirBuf,buf);
            FLG_OutDirCreated=true;
            break;
        }
        FileIndex++;
    }

}


//------------------------------------------------------------------------------
// End of $RCSfile: DRC_FileIO.cpp,v $
//------------------------------------------------------------------------------

















