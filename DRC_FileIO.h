//------------------------------------------------------------------------------
// $Header: D:\\Repository\\D\\Dev\\Psych499\\DRC_FileIO.h,v 1.23 2011-03-14 16:54:23-04 alan Exp alan $
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
//
// $Log: DRC_FileIO.h,v $
// Revision 1.23  2011-03-14 16:54:23-04  alan
// Modified to allow compilation with GNUC.
// Fixed comment on #endif line
//
// Revision 1.0  2011-03-13 23:17:59-04  alan
// Initial revision
//
// Revision 1.22  2011-02-18 15:52:56-05  alan
// Updated files with license information.
//
//------------------------------------------------------------------------------
#ifndef _DRC_FileIO_h
#define _DRC_FileIO_h
/* #pragma message in DRC_FileIO.h */


//------------------------------------------------------------------------------
// Globals located in DRC_FileIO.cpp
//------------------------------------------------------------------------------

// Properties file structure
#define MAXPROPROPERTIES 50
typedef struct{
    char* Name;
    char* Value;
}t_Properties;
extern t_Properties Properties[MAXPROPROPERTIES];
extern int          PropertiesIdx;


#define MINVOCAB     1
#define MAXVOCAB 10000
typedef struct{
    char    Word[WORDSZEXTN];
    char    Phoneme[WORDSZEXTN];
    char    Unknown1[4];
    struct{
        int         Freq;    // This designation for the field is an assumption.
        int         SumFreq; // Summed frequency over all homophones/graphs in chain.
        DRC_Float   CFS;     // This Constant Frequency Scaling variable is based on the above WOrdFreq.
    }Written,Spoken;
    // When Homophone points to itself it means this word isn't a homophone
    // When negative this means that this entry is a Homophone keyword.
    int     Homophone; // Linked list of homophones.
    // When Homograph points to itself it means this word isn't a homophone
    // When negative this means that this entry is a Homophone keyword.
    int     Homograph; // Linked list of homographs.
}t_vocabulary;
extern t_vocabulary Vocabulary[MAXVOCAB];
extern int          VocabularyIdx;
extern int          DiscardedWords;
extern int          HomoGraphemeCombined;
extern int          HomoPhonemeCombined;


#define MAXPHONEMES 50
typedef struct{
    char    PhonemeCode;
    char    VC;
}t_phoneme;
extern t_phoneme Phonemes[MAXPHONEMES];
extern int       PhonemesIdx;


#define MAXFEATURES  14
#define MAXLETTERS 30
typedef struct{
    char        Letter;
    char        VC;
    char        Unknown1;
    DRC_Float   Features[14];
}t_letter;
extern t_letter Letters[MAXLETTERS];
extern int      LettersIdx;
extern int      FeaturesIdx;


// Pointer to array GPCFields[MAXFIELDS][MAXFLDCHAR] that hold the characters
// allowable in any system or user defined field.  For instance MAXFLDCHAR will
// be MAXLETTERS or a little larger to allow all characters to be defined:
// a-z:0-25. FLD_A to FLD_Z and FLD_ANY won't be represented in this array, because
// they define specific rudamentary information, only from FLD_VOWELS and FLD_CONSONANTS
// (which are defined by the 'letters' file and

// GraphemeContext parse states
#define FLDStateNormal      0
#define FLDStateContext     1
#define FLDStateNegContext  2
#define FLDStateMeta        3

extern bool *GPCFieldStore;
extern int   GPCFieldStoreIdx;



#define MAXPARAM 50
typedef struct{
    char*       Name;    // Name of parameter.
    DRC_Float   Value;   // Value of parameter
}t_parameter;
extern t_parameter Parameters[MAXPARAM];
extern int         ParametersIdx;


#define MAXDRIVER 200
#define MAXSTRESS 0

extern int StressIdx;


extern FILE* Batchin;
extern int   Batchparse();

extern FILE* Genericin;
extern int   Genericlex();

extern FILE* fhRTFile;

typedef struct {
    int       Count;      // Number of entries.
    DRC_Float RTSummed;   // Sum of entries.
    DRC_Float RTSumSqr;   // Sum of squares of entries.
}t_StatList;

typedef struct STATISTICS{
    struct {
        t_StatList Named;
        t_StatList Lowac;
    }Correct,Wrong;
}t_Stats;
extern t_Stats Stats;



extern int File_Line_Num;
extern char File_Line[MAXLINE];

extern char*     GPCIdxValues;
extern char*     VowelConsonant;
extern int       GPCIdxValuesSz;
extern const char*     GPCRuleClassNames[];
extern t_gpcrule GPCRules[MAXGPCRULES];
extern int       GPCRulesIdx;
// Section pointers;
extern t_gpcrule* GPCList_BodyStart;
extern t_gpcrule* GPCList_BodyEnd;
extern t_gpcrule* GPCList_MultiplesStart;
extern t_gpcrule* GPCList_MultiplesEnd;
extern t_gpcrule* GPCList_DoublesStart;
extern t_gpcrule* GPCList_DoublesEnd;
extern t_gpcrule* GPCList_MultiPhonemesStart;
extern t_gpcrule* GPCList_MultiPhonemesEnd;
extern t_gpcrule* GPCList_ContextSensitiveStart;
extern t_gpcrule* GPCList_ContextSensitiveEnd;
extern t_gpcrule* GPCList_SinglesStart;
extern t_gpcrule* GPCList_SinglesEnd;
extern t_gpcrule* GPCList_OutputStart;
extern t_gpcrule* GPCList_OutputEnd;
extern t_gpcrule* GPCList_End;

extern char OutParentDirBuf[];
extern char OutRunDirBuf[];

// Vocabulary index with phonemic sort.
extern int PhonemeList[MAXVOCAB];


#endif // _DRC_FileIO_h

//------------------------------------------------------------------------------
// End of file $RCSfile: DRC_FileIO.h,v $
//------------------------------------------------------------------------------
