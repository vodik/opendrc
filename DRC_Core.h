//---------------------------------------------------------------------------
// $Header: D:\\Repository\\D\\Dev\\Psych499\\DRC_Core.h,v 1.15 2011-03-14 16:54:40-04 alan Exp alan $
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
//
//
// $Log: DRC_Core.h,v $
// Revision 1.15  2011-03-14 16:54:40-04  alan
// Modified to allow compilation with GNUC.
// Fixed comment on #endif line
//
// Revision 1.0  2011-03-13 23:17:58-04  alan
// Initial revision
//
// Revision 1.14  2011-02-18 15:52:56-05  alan
// Updated files with license information.
//
//---------------------------------------------------------------------------

#ifndef DRC_CoreH
#define DRC_CoreH
//---------------------------------------------------------------------------



#define WORDSZ     8
#define WORDSZEXTN (WORDSZ+2)

#define FEATURES   14

#define LETTERS    27
#define OILWORDS   MAXVOCAB
#define POLWORDS   OILWORDS
#define PHONEMES   45

extern int IndexLetter[256];   // ASCII character -> Letters table index.
extern int IndexPhoneme[256];  // Phoneme (ASCII character) -> Phoneme table index.

// Field definitions for graphemes
#define FLD_NULL             0  // Terminal field (non-field).
#define FLD_SIMPLEMIN        1  // Start of letter/phoneme simple fields.
#define FLD_SIMPLEMAX       44  // End of letter/phoneme simple fields.
// The individual codes correspond to the position of the letters or phonemes
// in their respective file.  For instance Letter_a is 1 and Phoneme_1 is 1. The
// zero field number is reserved for the end of the field string (like ASCIZ).
#define FLD_ANY             45  // All letters are acceptable in this field.
#define FLD_PHONEVOWELS     46  // Only Phoneme Vowels accepatable in this field.
#define FLD_PHONECONSONANTS 47  // Only Phoneme Consonants accepatable in this field.
#define FLD_GRAPHVOWELS     48  // Only Phoneme Vowels accepatable in this field.
#define FLD_GRAPHCONSONANTS 49  // Only Phoneme Consonants accepatable in this field.
#define FLD_USERMIN         50  // Only letters defiend by user accepatable in these fields.
#define FLD_USERMAX        254  // ...
#define FLD_ENDOFWORD      255  // Field indicating that we need to match the end of word character '+'.


#define EOW_PHONEME         44     // End of word indicator index for phonemic words.
#define EOW_LETTER          26     // End of word indicator index for graphemic words.

// GPC Route information.
#define MAXINPUTBUF   20

#define MAXFIELDS  (FLD_USERMAX-FLD_USERMIN+1)  // Array dimensions (maximum number of user defnd fields)
#define MAXFLDCHAR  MAXPHONEMES  // Assumption MAXPHONEMES>=MAXLETTERS (will store one or the other).

#define MASK_USED          'X'  // Char indicating corresponding char in word matched already
#define MASK_SPACE         'C'  // ... or not.
#define MASK_PROTECTED     'P'  // This field has been protected by a protected rule.

typedef enum {wpNone=0,wpBegin=1,wpMedial=2,wpEnd=3,wpAny=4} eWrdPosn;
typedef enum {clsNone=0,clsOut=1,clsBody=2,clsMulti=3,clsCS=4,clsTwo=5,clsMphon=6,clsSing=7} eGPCClass;

#define MAXGPCRULES 1100
typedef struct{
    eWrdPosn  WrdPosn;        // Position in the word to match (b-begin,m-medial,e-end,A-any).
    eGPCClass Class;          // Class of GPC Rule.
    BYTE*     GraphemeContext;// Context string for rule.
    size_t    PreContext;     // Number of context fields before first basic or simple character field.
    BYTE      Fields[MAXINPUTBUF]; // Fields corresponding to GraphemeContext
    char      Mask[MAXINPUTBUF];   // Mask of simple('X') vs. context(' ') fields of Fields[]
    BYTE*     Phonemes;       // Phonemic replacement.
    bool      Protected;      // Unprotected 'u', or Protected 'p'.
    DRC_Float Unknown3;       // Always seems to be 1.0
}t_gpcrule;

/* extern t_gpcrule GPCRules[MAXGPCRULES]; */
/* extern int       GPCRulesIdx; */
extern int       GPCBodyRules;   // Number of body rules.
extern int       GPCMultiRules;  // Number of multi-letter rules.
extern int       GPCTwoRules;    // Number of two-letter rules.
extern int       GPCMPhonRules;  // Number of Multi-phoneme (one-to-multi) rules.
extern int       GPCCSRules;     // Number of context-sensitive rules.
extern int       GPCSingRules;   // Number of single-letter rules.
extern int       GPCOutRules;    // Number of output rules.
extern int       GPCStressRules; // Number of stress rules (not implemented).

// One solution to the GPC translation of the incoming word fragment.
typedef struct {
    char*       WordFrag;            // The part of the word processed by the GPC Route.
    char*       PhonemeFrag;         // The phoneme translation of the above word.
    char*       MaskFrag;            // The masking of the word with protected fields.
    t_gpcrule*  Rules[MAXINPUTBUF];       // The rules going into making the above translation.
    DRC_Float   Activations[MAXINPUTBUF]; // The activations of the phonemes at end of GPC Route.
}t_solution;

// XXX: Fix this nonsense
extern DRC_Float GP_FrequencyScale;

/* XXX: From precompiled header */
void OILPOL_AddWords(int VocabIdx);
void IndexPhonemes(void);
void DRC_ProcessWord(int MaxCycles,const char *TestWord,const char* Category);
void DRC_DisplayHeader(FILE* fh);
void UpdateParamTables(FILE* fh,const char* Name,const char* Value);
void UpdateLocalParams(void);
void DRC_ResetSystem(void);
void DRC_PropagateActivation(int cycle,FILE* fh);

#endif

//---------------------------------------------------------------------------
// End of $RCSfile: DRC_Core.h,v $
//---------------------------------------------------------------------------
