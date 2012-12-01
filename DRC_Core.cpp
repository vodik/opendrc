//------------------------------------------------------------------------------
// $Header: D:\\Repository\\D\\Dev\\Psych499\\DRC_Core.cpp,v 1.57 2011-03-14 17:00:23-04 alan Exp alan $
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
// Routines to take care of the core NonSemantic-Lexical route of the DRC model.
//
// $Log: DRC_Core.cpp,v $
// Revision 1.57  2011-03-14 17:00:23-04  alan
// Modified to allow compilation with GNUC.
// Fixed comment on #endif line
// Fixed format for debug.h include.
// Added copyright notice and routine and code to run from DRC_DisplayHeader.
// Removed "extern" from definition of STR_PosnIDs.
// Included casts from BYTE* to const char* and the reverse.
// Used zero instead of NULL in if conditionals when testing against ints.
//
// Revision 1.0  2011-03-13 23:18:01-04  alan
// Initial revision
//
// Revision 1.56  2011-02-18 15:52:59-05  alan
// Updated files with license information.
//
// Revision 1.55  2011-02-10 16:35:52-05  alan
// Fixed the CorrectOutput() routine to implement the latest criterion of what a correct word is.
//
// Revision 1.54  2011-01-26 11:11:52-05  alan
// Added a few comments.
//
// Revision 1.53  2011-01-25 14:46:29-05  alan
// There was a problem in the code that distributed the GPC activation from the graphemes
// in a translation rule to the phonemes.  The code counted the graphemes and divided
// the total activation by the number of graphemes to get the average.  However, clsOut
// rules were not taken into account.  Fixed by adding test for output rules in an enclosing
// scope.  This should have caused errors in other words and didn't! Hmmmm.
//
// Revision 1.52  2011-01-25 14:29:08-05  alan
// Changed all the fprintf() calls to pfprintf() calls (protected against file handle being NULL)
// Comment change and a bracketing change.
//
// Revision 1.51  2011-01-16 20:37:54-05  alan
// Converted unprotected fprintf/fputc to protected pfprintf/pfputc calls.
//
// Revision 1.50  2011-01-12 22:50:40-05  alan
// Added GPC_DRC12UnSupDecayTrigErr flag to indicate when to try to compensate for
// the DRC1.2 Unsupported Decay trigger error.  DRC1.2 does not always trigger the
// unsupported decay of a phoneme in the Phoneme Buffer when that phoneme becomes
// unsupported.
// Modified the CorrectOutput() routine to indicate the translation is correct if the string
// in the PB matches either the string from the POL or the string from the GPC Route.
// Added code and variables to save the index of the most active POL word for comparison
// in the CorrectOutput() routine.
// Modified DRC_CalcGPCPhoneme_EI() routine to take a cycle number argument and use
// this to try and compensate for the DRC1.2 UnsupportedDecay trigger error.
// Built and installed GPC_CreateWord() to determine which word is output from the GPC
// route.  The word is created from the most active phonemes in each column.
//
// Revision 1.49  2010-12-20 13:03:55-05  alan
// In DRC_CalcPhonemePhono_EI() adjusted the length of the given phoneme string for
// the word boundary and added comments.
//
// Revision 1.48  2010-12-11 10:34:15-05  alan
// Added Commenting.
// Changed the test for a correct translation in CorrectOutput().
// Added UpdateParamTables() to save the incoming parameter file directly to the parameter
// tables.
// Fixed DRC_CalcPhonemePhono_EI() to calculate the PB->POL E/I values correctly.
//
// Revision 1.47  2010-12-07 19:01:30-05  alan
// Correct the detection of the last rule in DRC_DspGPCActivations() so the word terminator
// character '+' can be displayed too (it isn't part of the rule phoneme output).  Also for the
// partial solutions use the correct t_solution structure to get the phoneme to display.
// Added the routine name to the pfprintf's in the MergeMask() and UnMergeMask() routines
// to make it obvious where the message is comming from.
// In the FieldMatch() routine make sure we don't match the '+' terminator character.
// In DRC_GPCFind() make sure the Word Position of the rule is applied properly (finally).
// In the DRC_GPCApplyOutRules() routine make sure wpEnd output rules are matched
// to the end of the word.
//
// Revision 1.46  2010-12-05 23:27:52-05  alan
// Making calls to memset() more robust.
//
// Revision 1.45  2010-12-05 22:26:28-05  alan
// Converting plain floats to DRC_Floats.
//
// Revision 1.44  2010-12-05 18:34:52-05  alan
// Fixed DRC_UpdPBFromGPC() to tally the TGPCR totals when a MPHON rule is found
// in the first half of the word (eg. JINXED).
//
// Revision 1.43  2010-12-04 01:05:18-05  alan
// Fixing up problem with incorrect matching for end of word.
// Corrected the display of the GPC and GPCR lines.
// Added facilities to log and display partial solutions when no full solution exists.
//
// Revision 1.42  2010-12-02 08:44:12-05  alan
// Corrected problem with displaying the default rule "<chrs>->?" where the <chrs> were
// not being displayed.  This was because the wrong mask was being saved with the
// partial solution.
// The other problem was with the null-output rule "(e) [\v]h->*" where '*' actually meant no
// output.  Changed outputing '*' to '+'.  This worked because this situation only occurs
// in an wpEnd rule.
//
// Revision 1.41  2010-11-28 15:40:53-05  alan
// Added code to display the GPCR lines even when the GPC phoneme fragment was
// smaller, and visa versa.
//
// Revision 1.40  2010-11-28 00:13:38-05  alan
// Added variables LrgstPartSoln, LrgstPartUnMatchChars and code to save the largest
// partial solution to a translation (GPC Route) in case no full translation was found.  Then
// promoted those activations and displayed those rules.
// Changed the UnsupportedDecay test to < from <= on NetInput in DRC_CalcPhonemes().
// Added routine DRC_ClearPartialSoln() to clear the partial solutions from anywhere.
// Corrected a number of minor glitches with array overruns with masks etc.
// Added code to deal with "xxx->*" (no output) rules.
// Added SavePartSuccess() routine to save the partial solutions.
// Added NumChr() routine to count how many characters to be matched by rules.
// Added better test for wpEnd rules and ending characters in DRC_GPCFind().
//
// Revision 1.39  2010-11-26 00:11:52-05  alan
// Created GPCPhonemeMask buffer to hold phoneme mask until solution vector created.
// Corrected problem with displaying the tail end of both solutions (rules).
// Simplified the MergeMask and UnMergeMask routines by making some local variables.
// Created the GPCPhonemeMask at bottom of FieldMatch() routine.
// Used the Phoneme Mask in the solutions vector to determine if an output rule should be
// applied.
//
// Revision 1.38  2010-11-25 17:07:16-05  alan
// Added saving of the word mask to the solutions vector.
// Changed the sense of the comparison "==MASK_USED" to "!=MASK_SPACE" which
// should be the logical opposite now that MASK_USED has been split into two categories:
// MASK_USED and MASK_PROTECTED.
// Added code to properly increment through the rules list for both normal matching and
// output rule matching.
// Added check to make sure we don't match rules at the end of a word that are not valid
// in this position (b,m).
//
// Revision 1.37  2010-11-24 22:40:30-05  alan
// Removed the timer code.
// Fixed up form of calls to memset().
// Fixed up code to append '+' to end of word in DRC_CalcGPCRoute().  Also fixed up some
// memset() calls and meta constants.
// Make sure we don't update character count when Output rules run.
// Added precision specifier for word output field in DRC_DspDRCTranslations() so only
// eight characters are displayed.
// Removed commented out code.
// Changed some meta constant names.
//
// Revision 1.36  2010-11-24 10:50:38-05  alan
// Started the FLG_GPCRInpChg flag with value false so that the GPC Route wouldn't be
// called unnecessarily until the cycle defined by the GPCOnset parameter.
// When erasing the GPCSolns vector cleared the strdup'd strings first.
// In DRC_UpdActFromGPCR() implemented a mask to determine which character in the
// string we are matching.  This is needed because of rules like "a.e->1" with a 'hole' in
// the middle.
// Fixed the '+' activation of the output phonemes to get the correct value from the input.
// Fixed the selection of solutions in DRC_UpdPBFromGPC() to pick the non-body-rule
// solution before the body-rule solution when there were two.
// Same with DRC_DspGPCActivations().
// Adjust the size of the SaveSuccess() loop to copy the entire array of rules.
// Corrected the mask check in UnMergeMask().
// Corrected the len calculation in FieldMatch() to ignore the '+' in the Fields array because
// it was fowling up the initial size tests.
// In FieldMatch() also corrected for a ASCIIZ '\0' string terminator before expected.
//
// Revision 1.35  2010-11-22 17:30:14-05  alan
// Made GPC_InputActivation larger becase of '+' end-of-word overflow.
// Added EL_UnsupportedDecay variable (for phoneme layer).
//
// Revision 1.34  2010-11-20 22:15:27-05  alan
// Added the application of output rules to the GPC route.
// Added the routine DRC_GPCApplyOutRules() routine.
// Added the DRC_UpdPBFromGPC() routine to properly add the GPC output activations
// to the array that is used as input to the PB layer.
// Almost complete rewrite of DRC_DspGRCActivations() to deal with displaying the
// GPC and GPCR activation lines.
// Added routine FirstNonBodyRuleSoln() to find the index of the first nonbody-rule solution.
// Added section to DRC_DspDRCTranslations() to display the GPC Route rules properly.
// Removed the calculation of the GPC_OutputActivationTP1 array from DRC_UpdActFromGPCR() to DRC_UpdPBFromGPC().
// Fixed FieldMatch() to allow it to match Phoneme strings too.
// Added FindStart() routine to find the first nonmatched character in the string (to deal
// with GraphemeContext fields like "oi.e" (we dont want to jump over the '.' unmatched field).
//
// Revision 1.33  2010-11-18 21:16:20-05  alan
// Removed the t_solution typedef to a header file.
// Changed the position of the TGPC and TGPCR total activation line output.
// Made DRC_ResetSystem() more efficient with memset()s.
// Added update of GPC_OutputActivation from GPC_OutputActivationTP1.
// Added DRC_CalcGPCPhoneme_EI() to the Phoneme section and integrateded it into
// DRC_CalcPhonemes().
// Rearranged the calculations of the GPC activation values and added routines:
// DspGRCActivations(), DRC_DspDRCTranslations(), and DRC_UpdActFromGPCR().
// Corrected DRC_GPCRouteShift().
// Converted DisplaySuccess() to SaveSuccess().
// Removed activation calculation from FieldMatch().
// A few fixes to DRC_GPCFind().
//
// Revision 1.32  2010-11-17 10:58:45-05  alan
// Added global data structures for the GPC Route.
// Renamed OFLG_Extend to FLG_OutExtended for naming compatability.
// Added total variables for the GPC and GPCR activations.
// Added more commenting.
// Added code to store GPC solutions, shift letters into the GPC Route, and display GPC totals.
// Rebuilt DRC_CalcGPCRoute() adding shifting of most activated LL letters into input buffer
// (except end-of-words). added input letter activation calculations, added code to only
// search for rules when input changes (not every cycle), and added call to
// DRC_UpdatePBFromGPCR() that displays the GPC# activation lines and will eventually update the PB phonemes.
// Added new routines DRC_UpdatePBFromGPCR(),  and DRC_GPCRouteShift().
// Added GPC solution saving in DisplaySuccess().
// In FieldMatch() added End-of-Word matching and activation calculations for the GPC rules.
// In DRC_GPCFind() truncated the GPCPhonemeBuffer when phonemes no longer active
// and removed tests for wpEnd when rule matched (handled in FieldMatch() now).
//
// Revision 1.31  2010-11-13 22:47:48-05  alan
// Fix PreContext bugs in MergeMask() and UnMergeMask().
//
// Revision 1.30  2010-11-12 23:44:42-05  alan
// Working on getting the GPC route working.  Partially working now.  Will display the
// correct list of rules (as well as all the alternatives).
//
// Revision 1.29  2010-11-08 21:33:14-05  alan
// Installed DRC_CalcGPCRoute() call into main loop for DRC model.
// Created DRC_CalcGPCRoute() to set up buffers and DRC_GPCFind() call.
// Created FieldMatch() to match a word to a rules Field array.
// Modifications of DRC_GPCFind() to make it work.
//
// Revision 1.28  2010-11-07 23:17:43-05  alan
// Changed the GPCRoute code because it didn't look like it was going to work correctly.
//
// Revision 1.27  2010-11-03 17:41:41-04  alan
// Changed the name of the hdrstop.h file (to DRC_HdrStop.h) to distinguish it from the one
// used for DRCTest.
// Used PhonemeList[] to index the output phonemic words in phonemic order.
//
// Revision 1.26  2010-10-31 23:48:49-04  alan
// Terminate indexing of the POL words after the first terminating  ('\0') character is reached.
// Inserted MINVOCAB where appropriate.
// Removed commented out code.
// Dereference homophones using the homograph key entry and visa versa.
// Normal entries in the vocabulary are now only entries that are neither homographs or
// homophones.
// Change the PB->POL inhibition to be PB/Phonemic inhibition upto and including the
// terminating phoneme.  Thus the CumulativeInhib array.
// Make the PB layer output active terminator phonemes before the other phonemes (just
// to be like DRC1.2).
//
// Revision 1.25  2010-10-31 18:33:28-04  alan
// Removed the old method of iterating through list of active OIL words (now we just run
// through the active nodes alone).
// Added routines HomographKeyEntry(), HomographEntry(), HomophoneKeyEntry(),
// HomophoneEntry(), and NormalEntry() to simplify tests.
// Cleaned up Homophone/Homograph code.
//
// Revision 1.24  2010-10-28 17:15:00-04  alan
// Removed (%d) from one of the format (pfprintf) strings that produced ACTS output because
// it was put in to help debug and now only triggers WinMerge.
// Removed the POL_Words[w]>0.0 activation test in the lateral inhibition routine in
// the POL calculations.
//
// Revision 1.23  2010-10-26 22:39:57-04  alan
// Got revision working that used radix lists for each character in the OIL words so that
// the program only has to update those words with the character in the correct column.
// Got OILPOL_AddWords() working properly and added OIL_CharNext() and
// POL_CharNext() as helpers.
// Added code to prevent reindexing the IndexLetters and IndexPhonemes array.
// Replaced code to create file name and open the file for all ACTS files.
// Rewrote DRC_CalcLetterOIL() to use the new radix lists.  Same for
// DRC_CalcPhonemePhono().
//
// Revision 1.22  2010-10-25 11:16:45-04  alan
// Implemented the short lists for the OIL and POL list of active nodes.
// The OIL short list is installed and working, the POL list has been calculated but is not
// made use of yet.
// Added ClearShortLists() to clear the short lists.
// Set up the RT display to the console and RT file (in the output directory).
// Made the DRC_DisplayHeader() routine more generic so it could be used elsewhere.
// Summed the batch file statistics and displayed to STATS file.
// Added CreatePBString() to create a ASCIIZ string for correctness testing, and routine
// CorrectOutput() to test to make sure it is correct.
// Changed some FOR loops to memset()s for efficiency.
// Improved efficiency by adding tests for FILE* around chunks of code.
// Made LogTimer() call and supporting arrays to allow minimal profiling of code.
// Found that the slowness came from the OIL->LL and POL->PB interactions so am
// starting to install radix indexing to speed up (OILPOL_AddWords()).
//
// Revision 1.21  2010-10-21 22:31:10-04  alan
// Fix up the comments in DRC_CalcPhonoOIL_EI().
// Correct the test for the keyword in the homophone linked list.
//
// Revision 1.20  2010-10-20 20:52:41-04  alan
// Fixed problem with SEPARATE/COMBINED display switched around.
//
// Revision 1.19  2010-10-20 16:34:11-04  alan
// Made some code efficiency improvements by interchanging "if(FLG_OutExtended){" with
// "OnFlag("?????"){".
//
// Revision 1.18  2010-10-20 09:21:27-04  alan
// Added define to test for a Output flag in the parameter table before dumping output.
// Added global variables to keep track of the layer total activations for later reporting.
// Converted output to check to make sure the FILE* was non-NULL before outputing
// which included changing pfprintf() -> pfprintf() (protected).
// Added code to generate the ACTS files in the appropriate directory.
// Converted the display of layer totals to a cycle termination routine.
// Added a routine to display the ACTS header.
//
// Revision 1.17  2010-09-27 17:50:36-04  alan
// <>
//
// Revision 1.16  2010-08-31 17:20:10-04  alan
// Removed the values for all the parameters of the system and added UpdateLocalParams()
// routine to update them from the general table in DRC_CmdLine.cpp.
// Added a number of routines to implement the GPC route (sub-lexical) in the DRC model.
// These routines are: FindFirstNormal(), CountFields(), MatchField(), MatchGPCFields(),
// MatchGPCRule(), and FindGPCMatch().  These routines are compiled but not tested.
//
// Revision 1.15  2010-07-20 20:39:50-04  alan
// Got rid of the strtolwr() routine which just duplicates the library strlwr() routine.
//
// Revision 1.14  2010-07-16 22:57:51-04  alan
// Forgot a few things on the last revision's comments...
// Changed the loop index in DRC_CalcOILWords() from OILWORDS to VocabularyIdx so that the extra empty words were not dealt with.
// Added routine GetHomographKey() to determine the index of the key homograph for use in DRC_CalcPhonoOIL_EI() which was modified to deal with homographs.
// Changed the loop index in DRC_CalcPOLWords() from OILWORDS to VocabularyIdx so that the extra empty words were not dealt with.
// Added routine GetHomophoneKey() to determine the index of the key homophones for use in DRC_CalcOILPhono_EI() which was modified to deal with homophones.
// Modified DRC_CalcPhonoPhoneme_EI() to stop figuring out EI values when the first
// blank is hit in the current word.
//
// Revision 1.13  2010-07-16 22:48:55-04  alan
// Add RCS keywords.
// Fixed a bug in ActDynamics().
//
// Revision 1.12  2010-07-11 13:29:33-04  alan
// Move the global variables and defines up to the top of the file.
// Turned on debugging.
//
// Revision 1.11  2010-07-06 10:13:48-04  alan
// Changed "float" to "DRC_Float" so that I could check the theory that the size of the
// float was affecting the error in the results.
// Playing with the DRC Parameters to see I close to the DRC1.2 activations I can get with
// my Test DRC.
// Modified DspMatrix() to display inverted matrices properly.
// Corrected initialization of the OIL and POL activations to be zero instead of the McClelland
// and Rumelhart CFSi values.  Coltheart et al uses the CFSi values to increase the inputs to
// the activation equation (n_i).
//
// Revision 1.10  2010-06-27 22:45:14-04  alan
// Tested and corrected DRC_CalcPhonoOIL_EI(),  DRC_CalcPhonemePhono_EI().
// Testing the DRC Lexical route.
//
// Revision 1.9  2010-06-25 23:04:35-04  alan
// Putting code in so that calculations are not done if the result is a priori zero.
// Tested all the layer lateral inhibition code.
//
// Revision 1.8  2010-06-23 23:49:23-04  alan
// Added some comments.
// Got rid of redundant loop in CalcPOLWords() and corrected some errors.
// Got CalcPhonemes() working and displaying activated phoneme buffer nodes.
// Got DRC_CalcPhonoPhoneme_EI() working.
//
// Revision 1.7  2010-06-23 17:28:36-04  alan
// Zero'd out the Lateral-Inhibition and feedback parameters in the parameter table for
// debugging purposes.
// Commented some code.
// Replaced some numbers with meta-constants.
// Added cycle to parameter list of the DRC loop routines so that the cycle could be reported.
// Added code to various routines to display the activations like the Coltheart DRC model does.
// Added code to other levels to display the list of active nodes.
// Added some code to skip stages of E/I calculation when the parameters involved are zero.
// Added code to the POL E/I calculations that tested for active nodes and was careful
// about homophone activation.  This is because when we just did 1-to-1 activation with
// homophones then we would calculate the TotalActivation by adding all the sister elements
// of the homophones.
//
// Revision 1.6  2010-06-21 09:36:17-04  alan
// Cleaning up DRC parameter list (see notes in header file).
// Insert the test for active nodes in E/I calculations (something DRC document doesn't
// mention).
//
// Revision 1.5  2010-06-15 15:43:04-04  alan
// Moved output of Excitation/Inhibition routines to TP1 (t + 1) arrays then when all the EI
// routines for a cycle are completed then update the normal (non TP1) arrays with the information (DRC_PropagateActivation()).
// This allows for pseudo-parallel processing on each layer.
// Moved the DRC_CalcFeatures() routine to within the cycle loop because features can change during the processing of a word.
// Corrected a few problems in the DspMatrix() calls.
//
// Revision 1.4  2010-06-14 08:43:49-04  alan
// Chanaged the default DRC parameters to the Australian English 1.1.6 language default parameters.
// Fix the ActDynamics() routine to use the McClelland&Rumelhart equations (which are more
// complete than the Coltheart et. al 2001 equations).
// Set the initial activations of the OIL and POL to their respective CFS (Written and Spoken) values.
// Comment and debug DRC_CalcFeatLetterEI() routine.
// Added the CFS values into the ActDynamics() calls in the OIL and POL processing.  This may be a mistake in lite of the McClelland and Rumelhart (1981) paper.
// Change the parameters to be only positive parameters and thus the calculations had to absorb the negative signs.
// Moved the DRC_CalcPhonemes() routine to above its sub routines.
// Added output to highlight the end-of-cycle point.
//
// Revision 1.3  2010-06-10 20:35:58-04  alan
// Stopped using a blank at beginning of words to indicate word boundary because I figured this would throw off my version of the program compared to the official version.  So reduced most of the loops in the program that executed WORDSZEXTN times to WORDSZ times.
// Indicated differences of model parameters (taken from DRC paper in this code's default) to those in the current Australian English parameter file).
// Added ActDynamics procedure to implement the math on p215 of the DRC paper so that I could fix any errors to all routines at once -- the paper has to be wrong in some places.
// Added DEBUG Enter and Exit macros at start and end of each routine.
// Added code to dump the intermediate and final arrays from each stage of the processing.
// Made the feature EI code smarter.
// Made Max Iteration comments on each routine to determine where my time was being wasted.
//
// Revision 1.2  2010-06-09 13:49:07-04  alan
// Determine the Max Iterations value for each major subroutine.
// Slightly optimize a couple of routines.
//
// Revision 1.1  2010-06-09 10:59:28-04  alan
// Hmmm, shouldn't have rearranged the code because it screws up the diff.
// However, yesterday I got the while Lexical pipeline coded.  It isn't producing
//     the results I want in testing but it is working (somewhat).
//
//------------------------------------------------------------------------------

#include "DRC_HdrStop.h"

//#define DEBUG
#include "debug.h"


const char* DRC_Core[]={
    "$Author: alan $",
    "$Date: 2011-03-14 17:00:23-04 $",
    "$Revision: 1.57 $",
    "$RCSfile: DRC_Core.cpp,v $",
    "$Source: D:\\Repository\\D\\Dev\\Psych499\\DRC_Core.cpp,v $"
};



// This copyright notice and license are included with the intention of keeping
// this program and all its derivatives open-sourced.
const char *Copyright[]={
    "; OpenDRC  Copyright (C) 2011  Alan Angold\n",
    ";   This program comes with ABSOLUTELY NO WARRANTY.  This is free software, and\n",
    ";   you are welcome to redistribute it under certain conditions.  See the included\n",
    ";   file 'GNU General Public License, version 3 _GPLv3_.pdf' for details.\n",
    ";   OpenDRC URL: https://sourceforge.net/projects/opendrc/ \n",
    ";\n",
    0
};


//------------------------------------------------------------------------------
#pragma package(smart_init)
//------------------------------------------------------------------------------



char  VisualBuffer[2*WORDSZEXTN]; // String input to the feature layer.
int   BufferIdx;        // Index to word for sublexical path offset by word terminator (ie blanks).

DRC_Float FeatureLayer[WORDSZ][FEATURES];
DRC_Float FeatureLayerTP1[WORDSZ][FEATURES];

DRC_Float LetterLayer[WORDSZ][LETTERS];
DRC_Float LetterLayerTP1[WORDSZ][LETTERS];

#define EMPTYCELL   -1
DRC_Float OIL_Words[OILWORDS];
DRC_Float OIL_WordsTP1[OILWORDS];
int       OIL_ShortListTp0[OILWORDS];
int       OIL_ShortListTp1[OILWORDS];
int       OIL_ShortListIdx=0;

DRC_Float POL_Words[POLWORDS];
DRC_Float POL_WordsTP1[POLWORDS];
int       POL_ShortListTp0[POLWORDS];
int       POL_ShortListTp1[POLWORDS];
int       POL_ShortListIdx=0;

int*      OILPOL_Sort=NULL;

DRC_Float PhonemeLayer[WORDSZ][PHONEMES];
DRC_Float PhonemeLayerTP1[WORDSZ][PHONEMES];

char      GPCPhonemeBuffer[MAXINPUTBUF];
char      GPCPhonemeMask[MAXINPUTBUF];
DRC_Float GPC_InputActivation[WORDSZEXTN];
DRC_Float GPC_OutputActivation[WORDSZ][PHONEMES];
DRC_Float GPC_OutputActivationTP1[WORDSZ][PHONEMES];

t_solution LrgstPartSoln;
int        LrgstPartUnMatchChars;


std::vector <t_solution> GPCSolns;
t_gpcrule* MatchedRules[MAXINPUTBUF];  // List of pointers to matching rules in GPCRoute.
int        MatchedRulesIdx=0; // Index into above array.

const char* STR_PosnIDs = "NbmeA---------------";

// Variables to detect and save the most activated POL word index.
std::vector <char*> POLOutputWord;
char  GPCWord[MAXINPUTBUF];




char PBString[WORDSZ+2];// String version of the phoneme string (most activated phonemes).

int IndexLetter[256];   // ASCII character -> Letters table index.
int IndexPhoneme[256];  // Phoneme (ASCII character) -> Phoneme table index.

// These are the limits on the Epsilon (eps_i) values defined in the
// DRC (Coltheart et. al. 2001) paper defining the DRC model.  These numbers are
// not actually explicitly stated in that model but the IAC model of McClelland
// and Rumelheart (1981), which defines the Feature to OIL structure of the DRC
// model gives an explicity (and much clearer) definition of the activation
// mathematics involved.
#define DRC_MinActivation   0.0
#define DRC_MaxActivation   1.0

#define OILColLen   1000
#define POLColLen   1000

#define OnFlag(flg)        if(FindBParam(flg)->Value)

bool FLG_OutExtended = false;  // Flag:When to output extended format output.

// Total activations for layer.
DRC_Float DRC_FL_TotalAct = 0.0;
DRC_Float DRC_LL_TotalAct = 0.0;
DRC_Float DRC_OIL_TotalAct = 0.0;
DRC_Float DRC_POL_TotalAct = 0.0;
DRC_Float DRC_PB_TotalAct = 0.0;
DRC_Float DRC_GPC_TotalAct = 0.0;
DRC_Float DRC_GPCR_TotalAct = 0.0;


//
// Default values for the system parameters
// (Need to connect, or update, from the parameters table read from file)
// Defaults from "DRC:A Dual Route Cascaded Model of Visual Word Recognition
//    and Reading Aloud" paper by Coltheart et. al. p218

// General Parameters              COMMENTS
DRC_Float GP_ActivationRate;      // Effect of incoming E/I of level on the (t+1) activation rate.
DRC_Float GP_FrequencyScale;      // The scale values for the CFSi calculations.
DRC_Float GP_ReadingAloudCrit;    // Gating criterion to determine when processing finished (applied at end)
DRC_Float GP_DisplayThreshold;    // Threshold for display of individual letters etc.

// Feature Level
DRC_Float FL_FeatureLetterExcit;  // Excitation and Inhibition (E/I) from Feature->Letter layer.
DRC_Float FL_FeatureLetterInhib;
DRC_Float FL_Noise;               // Random noise inserted at this level (NOT Implemented)
DRC_Float FL_Decay;               // Decay of activation at this level.

// Letter Level
DRC_Float LL_LetterOrthoExcit;    // E/I from Letter layer forward to OIL
DRC_Float LL_LetterOrthoInhib;
DRC_Float LL_LetterLateralInhib;  // Lateral inhibition between active nodes in column of word.
DRC_Float LL_Noise;               // Random noise inserted at this level (NOT Implemented)
DRC_Float LL_Decay;               // Decay of activation at this level.

// Orthographic Lexicon
DRC_Float OL_OrthoLetterExcit;    // Feedback E/I from OIL --> Letter level.
DRC_Float OL_OrthoLetterInhib;
DRC_Float OL_OrthoPhonoExcit;     // Feedforward E/I from OIL --> POL level
DRC_Float OL_OrthoPhonoInhib;
DRC_Float OL_OrthoLateralInhib;   // Lateral inhibition between activated words in OIL
DRC_Float OL_Noise;               // Random noise inserted at this level (NOT Implemented)
DRC_Float OL_Decay;               // Decay of activation at this level.

// Phonological Lexicon
DRC_Float PL_PhonoOrthoExcit;     // Feedback E/I of activated entries from POL --> OIL level.
DRC_Float PL_PhonoOrthoInhib;
DRC_Float PL_PhonoPhonemeExcit;   // Feedforward E/I of active entries from POL --> Phoneme buffer(PB) level.
DRC_Float PL_PhonoPhonemeInhib;
DRC_Float PL_PhonoLateralInhib;   // Lateral inhibition between activated words in POL
DRC_Float PL_Noise;               // Random noise inserted at this level (NOT Implemented)
DRC_Float PL_Decay;               // Decay of activation at this level.

// Phoneme Level
DRC_Float EL_PhonemePhonoExcit;   // Feedback E/I of activated phonemes from PB --> POL level.
DRC_Float EL_PhonemePhonoInhib;
DRC_Float EL_PhonemeLateralInhib; // Lateral inhibition between activated phonemes in phoneme buffer.
DRC_Float EL_Noise;               // Random noise inserted at this level (NOT Implemented)
DRC_Float EL_Decay;               // Decay of activation at this level.
DRC_Float EL_UnsupportedDecay;    // Decay of activation for phonemes with net input <= 0.0

// GPC Route
DRC_Float GPC_GPCPhonemeExcit;
DRC_Float GPC_GPCCriticalPhonology;
int       GPC_GPCOnset;
//int   GPC_CyclesB4NextLetter;  // Obsolete in DRC1.2
bool      GPC_DRC12UnSupDecayTrigErr;  // Flag which turns on/off the compensation for a bug in DRC1.2
                                 // The bug is that DRC1.2 sometimes doesn't apply the UnsupportedDecay
                                 // to a phoneme when it becomes unsupported (activation>0 to ==0).

//==============================================================================
// Support Routines section.
//==============================================================================

//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------

int* OIL_Index=NULL;
int* POL_Index=NULL;

void OILPOL_AddWords(int VocabIdx)
{
    if(OILPOL_Sort==NULL){
        int len=max(OILWORDS,POLWORDS);

        // Array that will contain linked list of all the words with a particular
        // character (or phoneme) in that position in the grapheme(phoneme) word.
        OILPOL_Sort=new int[2*WORDSZ*len];
        memset(OILPOL_Sort,0xff,2*WORDSZ*len*sizeof(int)); // Set to EMPTYCELL=-1

        // Point to the head of the linked list for this letter and column.
        OIL_Index=new int[LETTERS*WORDSZ];
        memset(OIL_Index,0xff,LETTERS*WORDSZ*sizeof(int));  // Set to EMPTYCELL=-1

        // Point to the head of the linked list for this phoneme and column.
        POL_Index=new int[PHONEMES*WORDSZ];
        memset(POL_Index,0xff,PHONEMES*WORDSZ*sizeof(int)); // Set to EMPTYCELL=-1

        IndexLetters();
        IndexPhonemes();
    }

    char* pWord=Vocabulary[VocabIdx].Word;
    char* pPhoneme=Vocabulary[VocabIdx].Phoneme;
    int chridx,phoidx,newidx,curidx;
    bool POLPB_Terminated=false;
    for(int c=0;c<WORDSZ;c++){
        // Determine all the indices we need to link this word.
        chridx=IndexLetter[pWord[c]];            // Convert alphabetic character into 0-26 index
        newidx=VocabIdx*WORDSZ*2 + c;            // Figure out index for current element
        curidx=chridx*WORDSZ+c;                  // ... and into head pointer array (OIL_Index)
        // Link our new element into head of linked-list.
        OILPOL_Sort[newidx]=OIL_Index[curidx];   // Make new head of linked-list point to old head.
        OIL_Index[curidx]=newidx;                // Make index array point to new head.

        if(!POLPB_Terminated){
            // Determine all the indices we need to link this word.
            phoidx=IndexPhoneme[pPhoneme[c]];       // Convert phoneme character into 0-45 index
            newidx=VocabIdx*WORDSZ*2 + WORDSZ + c;   // Figure out index for current element
            curidx=phoidx*WORDSZ+c;                  // ... and into head pointer array (POL_Index)
            // Link our new element into head of linked-list.
            OILPOL_Sort[newidx]=POL_Index[curidx];   // Make new head of linked-list point to old head.
            POL_Index[curidx]=newidx;                // Make index array point to new head.

            // Only index phonetic words up until the first word terminator character.
            // This is defined in paper "How DRC1.2 differs from DRC1.0" (section 1.1).
            // This word won't be chained into column chains after the first terminator.
            // Thus phonemic words like /TRI/ get chained into c0(T), c1(R), c2(I), and c3(+).
            // Therefore, in DRC_CalcPhonemePhono() only the appropriate words get activated.
            if(pPhoneme[c]=='\0') POLPB_Terminated=true;
        }
    }
}

int OIL_CharNext(int chridx,int column,int prev)
{
    int rtn=EMPTYCELL;

    if(prev == EMPTYCELL){
        rtn=OIL_Index[chridx*WORDSZ + column];
    }else{
        rtn=OILPOL_Sort[prev];
    }
    return(rtn);
}

int POL_CharNext(int phoidx,int column,int prev)
{
    int rtn=EMPTYCELL;

    if(prev == EMPTYCELL){
        rtn=POL_Index[phoidx*WORDSZ + column];
    }else{
        rtn=OILPOL_Sort[prev];
    }
    return(rtn);
}

//---------------------------------------------------------------------------
// Trim the left and right space characters off the given string and return
// a pointer to the beginning of the trimmed string.
//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
char* Trim(char* str)
{
    // Get pointer to last character in string.
    char* ptr=&str[strlen(str)-1];
    // Decrement the pointer NULLing anything that is whitespace
    while(isspace(*ptr)){
        *ptr=0;
        ptr--;
    }
    // Set pointer to beginning of string.
    ptr=str;
    // Find first non-whitespace character.
    while(isspace(*ptr)) ptr++;
    // Return pointer to that character.
    return(ptr);
}

//------------------------------------------------------------------------------
// Return to test if two floating point operands are equal.  I didn't bother to
// template this because we never use other size floats.
// If two floats are within Epsilon of each other they are considered equal.
// Epsilon is a very small number.
//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
bool fequal(DRC_Float a,DRC_Float b)
{
    bool rtn=false;
    if(fabs(a-b)<=Epsilon){
        rtn=true;
    }
    return(rtn);
}

//------------------------------------------------------------------------------
// Routine: Given an ASCII character return it's index in the Letters table.
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
int LetterIdx(char letter){
    int rtn=-1;

    for(int t=0; t<MAXLETTERS; t++){
        if(Letters[t].Letter==letter){
            rtn=t;
            break;
        }
    }
    return(rtn);
};

//------------------------------------------------------------------------------
// Routine: This array allows us to change a ASCII character into an index in
//     our Letters (etc.) table immediately so we don't have to use a procedure.
//     Thus, although this method has an initial overhead it minimizes the work later.
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
void IndexLetters()
{
    static bool LettersIndexed=false;
    if(!LettersIndexed){
        LettersIndexed=true;
        // Set all the letters to the blank index so that if we have
        // any characters in the input we can't handle then we treat them as blanks.
        int blank=LetterIdx(' ');
        for(int i=0; i<256; i++){
            IndexLetter[i]=blank;
        }

        // Now go through all the letters we have defined and save their index.
        for(int i=0; i<LETTERS; i++){
            IndexLetter[Letters[i].Letter]=i;
        }
    }
}

//------------------------------------------------------------------------------
// Routine: This routine takes a ASCII (phoneme) character and finds its index in the
//     Phoneme table.
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
int PhonemeIdx(char phoneme){
    int rtn=-1;

    for(int t=0; t<MAXPHONEMES; t++){
        if(Phonemes[t].PhonemeCode==phoneme){
            rtn=t;
            break;
        }
    }
    return(rtn);
};

//------------------------------------------------------------------------------
// This routine generates a translation array from phonemes (ASCII representation)
// into the Phoneme table.
//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
void IndexPhonemes()
{
    static bool PhonemesIndexed=false;
    if(!PhonemesIndexed){
        PhonemesIndexed=true;
        // Set all the letters to the blank index so that if we have
        // any characters in the input we can't handle then we treat them as blanks.
        int blank=PhonemeIdx(' ');
        for(int i=0; i<256; i++){
            IndexPhoneme[i]=blank;
        }

        // Now go through all the letters we have defined and save their index.
        for(int i=0; i<PHONEMES; i++){
            IndexPhoneme[Phonemes[i].PhonemeCode]=i;
        }
    }
}

//------------------------------------------------------------------------------
// Routine to display an array as a rectangular grid (shape dependent on caller).
// This routine can send it to any I/O file handle (including stdout).
//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
void DspMatrix(FILE* fh, const char* Title, const char* Format, DRC_Float* Array, int Width, int Height, bool Invert)
{
    if(fh!=NULL){
        pfprintf(fh,"%s:\n",Title);
        if(Invert){
            // Display columns horizontally.
            for(int r=0; r<Height; r++){
                pfprintf(fh," %4d) ",r);
                for(int c=0; c<Width; c++){
                    pfprintf(fh,Format,Array[c*Height+r]);
                }
                pfprintf(fh,"\n");
            }
        }else{
            // Display columns vertically.
            for(int r=0; r<Height; r++){
                pfprintf(fh," %4d) ",r);
                for(int c=0; c<Width; c++){
                    pfprintf(fh,Format,Array[r*Width+c]);
                }
                pfprintf(fh,"\n");
            }
        }
        pfprintf(fh,"\n");
    }
}

//------------------------------------------------------------------------------
// This routine defines the activation dynamics calculation of the DRC (and more
// precisely the IAC (McClelland and Rumelhart 1981) model.
//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
inline DRC_Float ActDynamics(DRC_Float n_i,DRC_Float a_i,DRC_Float Decay)
{
    // NOTE: See McCLelland and Rumelhart (1981) for a better explanation of
    //       these activation equations.

    // Next the net_inputs are adjusted with DRC equations (4&5 p215) -- the
    // Grossberg equations.  Producing the eps_i values.  Essentially the neural
    // input strength is modulated by the distance the previous activation is
    // from the Max/Min activation.
    DRC_Float eps_i=0.0;
    if(n_i>=0.0){
        eps_i = n_i*(DRC_MaxActivation-a_i);
    }else{
        eps_i = n_i*(a_i-DRC_MinActivation);
    }

    // Next the activations are determined DRC equation (1 p215): a_i(t + delta_t)
    DRC_Float a_ip1 = a_i*(1.0 - Decay) + eps_i*GP_ActivationRate;

    // The activations are limited to [0,1] using DRC equation (2 p215)
    if(a_ip1 > DRC_MaxActivation){
        a_ip1 = DRC_MaxActivation;
    }else if(a_ip1 < DRC_MinActivation){
        a_ip1 = DRC_MinActivation;
    }

    // We now finish up with this value for a_i by storing it back in the
    // main array.
    return(a_ip1);
}


//==============================================================================
// Lexical Route central-hub section.
//==============================================================================


//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
void ClearShortLists(bool First)
{
    // Clear out these two arrays.  They will contain mostly -1 (EMPTYCELL) and
    // will only likely be filled with 1-10 entries.  However they are kept full
    // size just in case something unusual happens.  These lists are filled from
    // index zero upwards so they can be cleared that way too.  The first call
    // to this function clears the entire array.

    // First time through clear the entire arrays.  Each subsequent time transfer
    // the Tp1 array to the Tp0 array and clear out the Tp1 array.  This makes
    // the Tp1 array available for new indices and the Tp0 array the list of
    // indices from the previous run.
    for(int w=0;w<OILWORDS;w++){
        if((!First)&&(OIL_ShortListTp0[w]==EMPTYCELL)) break;
        OIL_ShortListTp0[w]=EMPTYCELL;
    }
    for(int w=0;w<OILWORDS;w++){
        if(!First){
            if(OIL_ShortListTp1[w]==EMPTYCELL) break;
            OIL_ShortListTp0[w]=OIL_ShortListTp1[w];
        }
        OIL_ShortListTp1[w]=EMPTYCELL;
    }
    OIL_ShortListIdx=0;

    for(int w=0;w<POLWORDS;w++){
        if((!First)&&(POL_ShortListTp0[w]==EMPTYCELL)) break;
        POL_ShortListTp0[w]=EMPTYCELL;
    }
    for(int w=0;w<POLWORDS;w++){
        if(!First){
            if(POL_ShortListTp1[w]==EMPTYCELL) break;
            POL_ShortListTp0[w]=POL_ShortListTp1[w];
        }
        POL_ShortListTp1[w]=EMPTYCELL;
    }
    POL_ShortListIdx=0;
}

//------------------------------------------------------------------------------
// Process a single word.
//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------

static void strlwr(char *str)
{
    char* p = str;
    for (; *p != '\0'; ++p)
        *p = tolower(*p);
}

void DRC_ProcessWord(int MaxCycles,const char *TestWord,const char* Category)
{
    Enter("DRC_ProcessWord");

    char buf[MAXPATH];
    int  PromotedGPCRChars = 0;      // Number of word characters promoted to GPCR (including assumed trailing '+')
    bool FLG_GPCRInpChg    = false;  // Flag:When the input to the GPC route has changed.

    bool done=false;
    int  cycle=1;

    // Clear the optimization buffers for the OIL and POL.
    ClearShortLists(true);

    // Create the input to the Feature Level (VisualBuffer).
    /* TestWord=Trim(TestWord);                    // Get rid of extra spaces (we will add ourselves). */
    strncpy(VisualBuffer,TestWord,WORDSZ);      // Copy the word to the input to the feature layer.
    strncat(VisualBuffer,"          ",WORDSZ);  // Terminate as ASCIIZ string if ever used that way.
    VisualBuffer[WORDSZ]=0;
    BufferIdx=0;                                // For sublexical path;
    strlwr(VisualBuffer);

    // Display our test word to the console
    pfprintf(stdout,"%s ",TestWord);
    fflush(stdout);

    // Create the output directory if it doesn't exist yet.
    CreateOutDir(TestWord);

    // Limit our MaxCycles paramter.
    if((MaxCycles<=0)||(MaxCycles>=1000)){
        MaxCycles=1000;
    }

    // Create an activation file if requested to.
    FILE* fh=NULL;
    if(FindBParam("SaveActivationLvls")->Value){
        fh=OpenOutputFile(OutParentDirBuf,OutRunDirBuf,TestWord,"acts");
    }

    // Display the ACT file header.
    pfprintf(fh,"; OpenDRC Activation Levels File\n");
    DRC_DisplayHeader(fh);

    // Clear out the solutions vector.
    for(int s=0;s<(int)GPCSolns.size();s++) {
        if(GPCSolns[s].WordFrag!=NULL) free(GPCSolns[s].WordFrag);
        if(GPCSolns[s].PhonemeFrag!=NULL) free(GPCSolns[s].PhonemeFrag);
    }
    GPCSolns.erase(GPCSolns.begin(),GPCSolns.end());

    DRC_ClearPartialSoln();


    while(!done){
        ClearShortLists(false);

        DRC_CalcFeatures(cycle,fh);
        DRC_CalcLetters(cycle,fh);
        DRC_CalcOILWords(cycle,fh);
        DRC_CalcPOLWords(cycle,fh);

        // Determine the GPC Route activations.
        DRC_CalcGPCRoute(cycle,PromotedGPCRChars,FLG_GPCRInpChg,fh);

        DRC_CalcPhonemes(cycle,fh);
        DRC_PropagateActivation(cycle,fh);

        // Determine when to place next char on end of GPC Route input buffer.
        DRC_GPCRouteShift(cycle,PromotedGPCRChars,FLG_GPCRInpChg,fh);

        DRC_DisplayTotals(cycle,fh);

        bool finished=DRC_TestOutput(cycle,fh);
        if((cycle>=MaxCycles)||finished){
            DRC_FinalReport(fh);
            DRC_Cleanup(fh);
            done=true;

            // Create a string that looks like the PB max activated phonemes.
            CreatePBString();

            // Output is correct if it sounds like known word.
            bool AnswerCorrect = CorrectOutput(TestWord,cycle,MaxCycles);

            // Display RT line to console.
            pfprintf(stdout,"%s %d %s %s\n",
                PBString,
                cycle,
                (cycle==MaxCycles)?"LOWAC":"NAMED",
                (AnswerCorrect)?"CORRECT":"WRONG");
            // Display RT line to RT file.
            pfprintf(fhRTFile,"%s %s %d %s %s\n",
                TestWord,
                PBString,
                cycle,
                (cycle==MaxCycles)?"LOWAC":"NAMED",
                (AnswerCorrect)?"CORRECT":"WRONG");

            // Allocate numbers to statistical counters.
            if(cycle==MaxCycles){
                if(AnswerCorrect){
                    Stats.Correct.Lowac.Count++;
                    Stats.Correct.Lowac.RTSummed += (DRC_Float)cycle;
                    Stats.Correct.Lowac.RTSumSqr += (DRC_Float)(cycle*cycle);
                }else{
                    Stats.Wrong.Lowac.Count++;
                    Stats.Wrong.Lowac.RTSummed += (DRC_Float)cycle;
                    Stats.Wrong.Lowac.RTSumSqr += (DRC_Float)(cycle*cycle);
                }
            }else{
                if(AnswerCorrect){
                    Stats.Correct.Named.Count++;
                    Stats.Correct.Named.RTSummed += (DRC_Float)cycle;
                    Stats.Correct.Named.RTSumSqr += (DRC_Float)(cycle*cycle);
                }else{
                    Stats.Wrong.Named.Count++;
                    Stats.Wrong.Named.RTSummed += (DRC_Float)cycle;
                    Stats.Wrong.Named.RTSumSqr += (DRC_Float)(cycle*cycle);
                }
            }

        }
        cycle++;
    }

    if((fh!=NULL)&&(fh!=stdout)) {
        fclose(fh);
    }

    Exit;
}


//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
void CreatePBString()
{
    memset(PBString,0,sizeof(PBString));
    for(int c=0;c<WORDSZ;c++){
        DRC_Float MaxAct=0.0;
        for(int p=0;p<PHONEMES;p++){
            if(PhonemeLayerTP1[c][p]>MaxAct){
                MaxAct=PhonemeLayerTP1[c][p];
                if(Phonemes[p].PhonemeCode==' '){
                    PBString[c]=0;
                    return;
                }else{
                    PBString[c]=Phonemes[p].PhonemeCode;
                }
            }
        }
    }
}

//---------------------------------------------------------------------------
// Routine: This routine isn't quite right yet(20101210).  It is still reporting
//          incorrect input when there is no verbally matching word in the vocabulary.
//          DRC1.2 still matches for some reason.
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
bool CorrectOutput(char *Word, int cycle,int MaxCycles)
{
    bool Correct=false;

    // If we have exceeded our max cycles then this is not a valid translation.
    if(cycle<MaxCycles){

        // Check to see if this word is in our vocabulary (at least a homophone of it).
        bool InVocab=false;
        for(int w=0; w<VocabularyIdx; w++){
            if(strcmp(Vocabulary[w].Word,Word)==0){
                InVocab=true;
                break;
            }
        }

        if(InVocab){
            // If the PB word is in our vocabulary and the lexical route (POL)
            // has found the same word then the translation is correct.
            for(unsigned int w=0;w<POLOutputWord.size();w++){
                // Compare this most active word with the PB word.
                if(strcmp(PBString,POLOutputWord[w])==0){
                    // If they are equal then we've generated a correct response.
                    // PB==POL Correct!
                    Correct=true;
                }
            }

        }else{
            // If our PB word is not in our vocabulary but the same as our GPC
            // word then the output is correct.
            // (This doesn't need a test against NULL as GPCWord is statically allocated).
            if(strcmp(PBString,GPCWord)==0){
                Correct=true;
            }
        }
    }
    return(Correct);
}

//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
void DRC_DisplayHeader(FILE* fh)
{
    if(fh!=NULL){
        pfprintf(fh,"; OpenDRC Model version %s\n",DRC_Model_Version);
        pfprintf(fh,"; - Visual Feature and Letter Layer units: %s Phoneme Layer: %s\n",
            FindLanguageParam("DefaultNumOrthAnalysisUnits"),
            FindLanguageParam("DefaultNumPhonemeUnits"));
        pfprintf(fh,"; Language: %s\n",FindLanguageParam("Name"));
        pfprintf(fh,"; - Version: %s\n",FindLanguageParam("Version"));
        pfprintf(fh,"; - Date: %s\n",FindLanguageParam("ReleaseDate"));
        pfprintf(fh,"; - Created by: %s\n",FindLanguageParam("Creator"));
        pfprintf(fh,"; - URL: %s\n",FindLanguageParam("URL"));
        pfprintf(fh,"; Language Parsing Results:\n");
        // The +1 on these values are because the indices are zero-based (0,1,2, ..., n-1)
        pfprintf(fh,"; - Letters: %d Visual Features: %d Phonemes: %d Stress Types: %d\n",
            LettersIdx+1,FeaturesIdx,PhonemesIdx+1,StressIdx);
        pfprintf(fh,"; - Orthographic Lexicon entries: %d (skipped %d that were too large)\n",
            (VocabularyIdx-MINVOCAB)-HomoGraphemeCombined,DiscardedWords);
        pfprintf(fh,"; - Phonological Lexicon entries: %d (using %s entries for homophones)\n",
            (VocabularyIdx-MINVOCAB)-HomoPhonemeCombined,
            (FindBParam("SeparateHomophones")->Value)?"SEPARATE":"COMBINED");
        pfprintf(fh,"; - Multi-Letter rules: %d Context-Sensitive rules: %d Two-Letter rules: %d\n",
            GPCMultiRules,GPCCSRules,GPCTwoRules);
        pfprintf(fh,"; - One-To-Multi rules: %d Single-Letter rules: %d Output rules: %d\n",
            GPCMPhonRules,GPCSingRules,GPCOutRules);
        pfprintf(fh,"; - Body rules: %d Stress rules: %d\n",GPCBodyRules,GPCStressRules);
        pfprintf(fh,"; Minimum activation level reported: %e\n",GP_DisplayThreshold);
        pfprintf(fh,"; Naming threshold: %f\n",GP_ReadingAloudCrit);
        pfprintf(fh,";\n");
        PrtCopyright(fh);
    }
}

//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
void PrtCopyright(FILE* fh)
{
    if(fh!=NULL){
        int line=0;
        while(Copyright[line]!=NULL){
            pfprintf(fh,Copyright[line]);
            line++;
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
void DRC_DisplayTotals(int cycle,FILE* fh)
{
    if(fh!=NULL){
        if(DRC_LL_TotalAct>GP_DisplayThreshold){
            if(FLG_OutExtended) {
                OnFlag("LLDRCT")  pfprintf(fh,"  TL Cycle%03d %8.6f\n",cycle,DRC_LL_TotalAct);
            }else{
                OnFlag("LLDRCT")  pfprintf(fh,"TL Cycle%d %8.6f\n",cycle,DRC_LL_TotalAct);
            }
        }
        if(DRC_OIL_TotalAct>GP_DisplayThreshold){

            if(FLG_OutExtended) {
                OnFlag("OILDRCT") pfprintf(fh,"  TO Cycle%03d %8.6f\n",cycle,DRC_OIL_TotalAct);
            }else{
                OnFlag("OILDRCT") pfprintf(fh,"TO Cycle%d %8.6f\n",cycle,DRC_OIL_TotalAct);
            }
        }
        if(DRC_POL_TotalAct>GP_DisplayThreshold){
            if(FLG_OutExtended) {
                OnFlag("POLDRCT") pfprintf(fh,"  TP Cycle%03d %8.6f\n",cycle,DRC_POL_TotalAct);
            }else{
                OnFlag("POLDRCT") pfprintf(fh,"TP Cycle%d %8.6f\n",cycle,DRC_POL_TotalAct);
            }
        }
        if(DRC_GPC_TotalAct>GP_DisplayThreshold){
            if(FLG_OutExtended) {
                OnFlag("GPCDRCT")  pfprintf(fh,"  TGPC Cycle%03d %8.6f\n",cycle,DRC_GPC_TotalAct);
            }else{
                OnFlag("GPCDRCT")  pfprintf(fh,"TGPC Cycle%d %8.6f\n",cycle,DRC_GPC_TotalAct);
            }
        }
        if(DRC_GPCR_TotalAct>GP_DisplayThreshold){
            if(FLG_OutExtended) {
                OnFlag("GPCRDRCT")  pfprintf(fh,"  TGPCR Cycle%03d %8.6f\n",cycle,DRC_GPCR_TotalAct);
            }else{
                OnFlag("GPCRDRCT")  pfprintf(fh,"TGPCR Cycle%d %8.6f\n",cycle,DRC_GPCR_TotalAct);
            }
        }
        if(DRC_PB_TotalAct>GP_DisplayThreshold){
            if(FLG_OutExtended) {
                OnFlag("PBDRCT")  pfprintf(fh,"  TPh Cycle%03d %8.6f\n",cycle,DRC_PB_TotalAct);
            }else{
                OnFlag("PBDRCT")  pfprintf(fh,"TPh Cycle%d %8.6f\n",cycle,DRC_PB_TotalAct);
            }
        }
    }
}

//---------------------------------------------------------------------------
// Routine: Give raw Name and Value strings find the right parameter table for
//          the parameter and update the Value field with the new value translated
//          based on the table type.
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
void UpdateParamTables(FILE* fh,char* Name,char* Value)
{
    t_iparam* iptr=NULL;
    t_fparam* fptr=NULL;
    t_sparam* sptr=NULL;
    t_bparam* bptr=NULL;


    // Check to see if parameter is a integer parameter.
    if((iptr=FindIParam(Name,false))!=NULL){
        iptr->Value=atoi(Value);

    // Check to see if parameter is a floating point parameter.
    }else if((fptr=FindFParam(Name,false))!=NULL){
        fptr->Value=atof(Value);

    // Check to see if parameter is a string parameter.
    }else if((sptr=FindSParam(Name,false))!=NULL){
        sptr->Value=strdup(Value);

    // Check to see if parameter is a boolean parameter.
    }else if((bptr=FindBParam(Name,false))!=NULL){
        if((strcasecmp(Value,"true")==0)||
           (strcasecmp(Value,"t")==0)   ||
           (strcasecmp(Value,"yes")==0) ||
           (strcasecmp(Value,"y")==0)){
            bptr->Value=true;
        }else{
            bptr->Value=false;
        }
    }

    if((iptr==NULL)&&(fptr==NULL)&&(sptr==NULL)&&(bptr==NULL)){
        pfprintf(fh,"ERROR(UpdateParamTables): Parameter '%s=%s' cannot be found.\n",Name,Value);
        exit(1);
    }

}


//---------------------------------------------------------------------------
// Routine: Update our named variables from generalized parameter tables.
// Input: The Generalized parameter tables.
// Output: The specific variable's value.
// SideEffects: Local named variables changed.
// Errors: FindFParam() mail exit the program.
//---------------------------------------------------------------------------
void UpdateLocalParams(void)
{
    // General Parameters
    GP_ActivationRate        = FindFParam("ActivationRate",true)->Value;
    GP_FrequencyScale        = FindFParam("FrequencyScale",true)->Value;
    GP_ReadingAloudCrit      = FindFParam("MinReadingPhonology",true)->Value;
    GP_DisplayThreshold      = FindFParam("ReportActGreater",true)->Value;

    // Feature Level
    FL_FeatureLetterExcit    = FindFParam("FeatureLetterExcitation",true)->Value;
    FL_FeatureLetterInhib    = FindFParam("FeatureLetterInhibition",true)->Value;
    FL_Noise                 = FindFParam("FeatureNoise",true)->Value;
    FL_Decay                 = FindFParam("FeatureDecay",true)->Value;

    // Letter Level
    LL_LetterOrthoExcit      = FindFParam("LetterOrthlexExcitation",true)->Value;
    LL_LetterOrthoInhib      = FindFParam("LetterOrthlexInhibition",true)->Value;
    LL_LetterLateralInhib    = FindFParam("LetterLateralInhibition",true)->Value;
    LL_Noise                 = FindFParam("LetterNoise",true)->Value;
    LL_Decay                 = FindFParam("LetterDecay",true)->Value;

    // Orthographic Lexicon
    OL_OrthoLetterExcit      = FindFParam("OrthlexLetterExcitation",true)->Value;
    OL_OrthoLetterInhib      = FindFParam("OrthlexLetterInhibition",true)->Value;
    OL_OrthoPhonoExcit       = FindFParam("OrthlexPhonlexExcitation",true)->Value;
    OL_OrthoPhonoInhib       = FindFParam("OrthlexPhonlexInhibition",true)->Value;
    OL_OrthoLateralInhib     = FindFParam("OrthlexLateralInhibition",true)->Value;
    OL_Noise                 = FindFParam("OILNoise",true)->Value;
    OL_Decay                 = FindFParam("OILDecay",true)->Value;

    // Phonological Lexicon
    PL_PhonoOrthoExcit       = FindFParam("PhonlexOrthlexExcitation",true)->Value;
    PL_PhonoOrthoInhib       = FindFParam("PhonlexOrthlexInhibition",true)->Value;
    PL_PhonoPhonemeExcit     = FindFParam("PhonlexPhonemeExcitation",true)->Value;
    PL_PhonoPhonemeInhib     = FindFParam("PhonlexPhonemeInhibition",true)->Value;
    PL_PhonoLateralInhib     = FindFParam("PhonlexLateralInhibition",true)->Value;
    PL_Noise                 = FindFParam("POLNoise",true)->Value;
    PL_Decay                 = FindFParam("POLDecay",true)->Value;

    // Phoneme Level
    EL_PhonemePhonoExcit     = FindFParam("PhonemePhonlexExcitation",true)->Value;
    EL_PhonemePhonoInhib     = FindFParam("PhonemePhonlexInhibition",true)->Value;
    EL_PhonemeLateralInhib   = FindFParam("PhonemeLateralInhibition",true)->Value;
    EL_Noise                 = FindFParam("PhonemeNoise",true)->Value;
    EL_Decay                 = FindFParam("PhonemeDecay",true)->Value;
    EL_UnsupportedDecay      = FindFParam("PhonemeUnsupportedDecay",true)->Value;

    // GPC Route
    GPC_GPCPhonemeExcit      = FindFParam("GPCPhonemeExcitation",true)->Value;
    GPC_GPCCriticalPhonology = FindFParam("GPCCriticalPhonology",true)->Value;
    GPC_GPCOnset             = FindIParam("GPCOnset",true)->Value;
    // GPC_CyclesB4NextLetter   = FindFParam("SimEndVal",true)->Value;     // Obsolete in DRC1.2.

    FLG_OutExtended          = FindBParam("Extended",true)->Value;
    GPC_DRC12UnSupDecayTrigErr
                             = FindBParam("DRC12UnSupDecayTrigErr",true)->Value;
}

//---------------------------------------------------------------------------
// Initialize the DRC system to the point where we can start accepting words.
// Tested.
//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
void DRC_ResetSystem()
{
    Enter("DRC_InitSystem");

    // First our input buffer.
    for(int c=0; c<2*WORDSZ; c++){
        VisualBuffer[c]=0;
    }

    // Initialize the Feature layer to zeros.
    memset(FeatureLayer,0,sizeof(FeatureLayer));
    // Initialize the Letter layer to zeros.
    memset(LetterLayer,0,sizeof(LetterLayer));
    // Initialize the OIL layer to zeros.
    memset(OIL_Words,0,sizeof(OIL_Words));
    // Initialize the POL layer to zeros.
    memset(POL_Words,0,sizeof(POL_Words));
    // Init the GPC Routine activations to zeros.
    memset(GPC_OutputActivation,0,sizeof(GPC_OutputActivation));
    memset(GPC_OutputActivationTP1,0,sizeof(GPC_OutputActivationTP1));


    // NOTE: There is a contradiction between McClelland and Rumelhart's IAC model W.R.T. these
    //       values and the Coltheart DRC model.  IAC treats them as resting states of the neuron
    //       -- more frequently used words have a higher resting state in [-1,0] and thus are activated
    //       quicker.  Coltheart uses these values to increase the net input to a neuron to effectively
    //       increase the rate the neuron approaches active state (a_i > 0). We can use these only one
    //       way or the other not both.

    // Initialize the Phoneme layer to zeros.
    for(int c=0; c<WORDSZ; c++){
        for(int p=0; p<PHONEMES; p++){
            PhonemeLayer[c][p]=0;
        }
    }

    // Create the index array of all the valid letters
    // in this system to speed processing.
    IndexLetters();
    IndexPhonemes();
    UpdateLocalParams();

    Exit;
}

//---------------------------------------------------------------------------
// Propagate the activations forward to the next time step  t --> t+1  from their
// stored arrays <name>TP1.
//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
void DRC_PropagateActivation(int cycle,FILE* fh)
{
    Enter("DRC_PropagateActivation");

    memcpy(FeatureLayer,FeatureLayerTP1,sizeof(FeatureLayer));
    memcpy(LetterLayer,LetterLayerTP1,sizeof(LetterLayer));
    memcpy(OIL_Words,OIL_WordsTP1,sizeof(OIL_Words));
    memcpy(POL_Words,POL_WordsTP1,sizeof(POL_Words));
    memcpy(PhonemeLayer,PhonemeLayerTP1,sizeof(PhonemeLayer));

    // This is somewhat pointless because only the GPC_OutputActivationTP1 array
    // gets used.  Data is calculated from the LetterLayer data and goes into
    // PhonemeLayerTP1 array.  Thus GPC_OutputActivation never gets used.
    memcpy(GPC_OutputActivation,GPC_OutputActivationTP1,sizeof(GPC_OutputActivation));

    Exit;
}







//==============================================================================
// xXXXXXXXXx
// xXXXXXXXXx                       xx
// xX                               Xx
// xX                               Xx
// xX            XXXX.     xXXXX   XXXX. X.    X.  X.xXx   XXXX.
// xX          .XXXXXXx   xXXXXXX  XXXX. X.    X.  XxXX. .XXXXXXx
// xXXXXXXXx   XX    xX. .Xx   .X.  Xx   X.    X.  XX    XX    xX.
// xXXXXXXXx  .X      Xx      XXX.  Xx   X.    X.  Xx   .X      Xx
// xX         xXXXXXXXXx  .XXXXXX.  Xx   X.    X.  X.   xXXXXXXXXx
// xX         xXXXXXXXXx .XXX  .X.  Xx   X.    X.  X.   xXXXXXXXXx
// xX         .X         xX    .X.  Xx   Xx   .X.  X.   .X
// xX          XX    .Xx xX.  .XX.  Xx   XX   XX.  X.    XX    .Xx
// xX          .XXXXXXX  .XXXXXXXx  xXX. xXXXXxX.  X.    .XXXXXXX
// xX            XXXXx    .XXXX Xx  .XX.  xXX. X.  X.      XXXXx
//
// Feature Layer section (part of Lexical and Sub-Lexical Route).
//==============================================================================

//------------------------------------------------------------------------------
// Routine: This routine will check the queue to see if there is another word or mask that
// is to be presented at this cycle.  If there is it will copy the word/mask to
// the visual buffer and then link the features to the Letter level.
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
void DRC_CalcFeatures(int cycle,FILE* fh)
{
    Enter("DRC_CalcFeatures");

    if(FLG_OutExtended) pfprintf(fh,"Activated Features (cycle %d)\n",cycle);

    // For each letter in the incoming word...
    for(int c=0; c<WORDSZ; c++){
        if(fh!=NULL){
            if(FLG_OutExtended) {
                pfprintf(fh,"  Cycle%03d VF%02d",cycle,c);
            }else{
                pfprintf(fh,"Cycle%d VF%d ",cycle,c);
            }
        }
        // Look for the letter in the Letters table and set a pointer to the
        // features part.  Unknown letters will translate into blanks and then
        // into a zero feature list.
        DRC_Float* pf=(DRC_Float*)Letters[IndexLetter[VisualBuffer[c]]].Features;

        // If we found the letter then copy over its features.
        for(int f=0; f<FEATURES; f++){
            DRC_Float FeatVal=pf[f];
            FeatureLayerTP1[c][f]=FeatVal;

            if(fh!=NULL){
                if(FLG_OutExtended) {
                    pfprintf(fh," %4.2f",FeatVal);
                }else{
                    pfprintf(fh,"%d",(int)(FeatVal+0.5));
                }
            }
        }
        pfprintf(fh," %c\n",(VisualBuffer[c]==' ')?'+':VisualBuffer[c]);
    }
    if(FLG_OutExtended) pfprintf(fh,"\n");

    DRC_FL_TotalAct=0.0;

    Exit;
};










//==============================================================================
// xX
// xX                    xx   xx
// xX                    Xx   Xx
// xX                    Xx   Xx
// xX           XXXX.   XXXX.XXXX.   XXXX.    X.xXx
// xX         .XXXXXXx  XXXX.XXXX. .XXXXXXx   XxXX.
// xX         XX    xX.  Xx   Xx   XX    xX.  XX
// xX        .X      Xx  Xx   Xx  .X      Xx  Xx
// xX        xXXXXXXXXx  Xx   Xx  xXXXXXXXXx  X.
// xX        xXXXXXXXXx  Xx   Xx  xXXXXXXXXx  X.
// xX        .X          Xx   Xx  .X          X.
// xX         XX    .Xx  Xx   Xx   XX    .Xx  X.
// xXXXXXXXX  .XXXXXXX   xXX. xXX. .XXXXXXX   X.
// xXXXXXXXX    XXXXx    .XX. .XX.   XXXXx    X.
//
// Letter Layer section (part of Lexical and Sub-Lexical Route).
//==============================================================================

//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
void DRC_CalcLetters(int cycle,FILE* fh)
{
    Enter("DRC_CalcLetters");


    // Max Iteration = WORDSZ*LETTERS = 8*27 = 216
    OnFlag("LLTimeT") DspMatrix(fh,"Activation LetterLayerT","%+6.4f ",(DRC_Float*)LetterLayerTP1,WORDSZ,LETTERS,true);

    // Determine the Excit(ation)/Inhib(ition) from the Feature Layer.
    DRC_Float* FL_EI=DRC_CalcFeatLetterEI(VisualBuffer);
    OnFlag("LLFromFL") DspMatrix(fh,"Feature->Letter(EI) DRC_CalcFeatLetterEI","%+6.4f ",FL_EI,WORDSZ,LETTERS,true);

    // Now go through the vocabulary and get all the inhibitions and
    // excitations from the OIL to Letter level. If the letter at the
    // position in the incoming word matches the letter in the vocab word
    // at that same position then tally the OIL output for that word to the
    // Excitation for this character/position in the letter level.
    DRC_Float* OIL_EI=DRC_CalcOILLetterEI();
    OnFlag("LLFromOIL") DspMatrix(fh,"OIL->Letter(EI) DRC_CalcOILLetterEI","%+6.4f ",OIL_EI,WORDSZ,LETTERS,true);

    // Now determine the Letter Layer lateral inhibitions (between different
    // letter hypothesis for each letter position in the given word.
    DRC_Float* LLL_EI=DRC_CalcLLLateralEI();
    OnFlag("LLFromLL") DspMatrix(fh,"LetterLayer Lateral Inhib. DRC_CalcLLLateralEI","%+6.4f ",LLL_EI,WORDSZ,LETTERS,true);

    if(FLG_OutExtended) pfprintf(fh,"Activated Letters (Cycle %d)\n",cycle);

    bool FLG_Found=false;
    DRC_LL_TotalAct=0.0;
    for(int c=0; c<WORDSZ; c++){
        for(int L=0; L<LETTERS; L++){
            LetterLayerTP1[c][L] = ActDynamics(FL_EI[c*LETTERS+L]+OIL_EI[c*LETTERS+L]-LLL_EI[c*LETTERS+L],
                                               LetterLayer[c][L],
                                               LL_Decay);
            // Display the list of active nodes.
            if(LetterLayerTP1[c][L]>0.0){
                if(LetterLayerTP1[c][L]>GP_DisplayThreshold){
                    FLG_Found=true;

                    OnFlag("LLDRC"){
                        if(FLG_OutExtended){
                            pfprintf(fh,"  Cycle%03d L%02d %8.6f '%c'\n",
                                cycle,
                                c,
                                LetterLayerTP1[c][L],
                                (Letters[L].Letter==' ')?'+':Letters[L].Letter);
                        }else{
                            pfprintf(fh,"Cycle%d L%d %8.6f %c\n",
                                cycle,
                                c,
                                LetterLayerTP1[c][L],
                                (Letters[L].Letter==' ')?'+':Letters[L].Letter);
                        }
                    }
                }
                DRC_LL_TotalAct+=LetterLayerTP1[c][L];
            }
        }
    }
    if((FLG_OutExtended)&&(!FLG_Found)){
        pfprintf(fh,"  No active letter nodes.\n");
    }

    OnFlag("LLTimeTp1") DspMatrix(fh,"Activation LetterLayerTP1","%+6.4f ",(DRC_Float*)LetterLayerTP1,WORDSZ,LETTERS,true);

    Exit;
};

//------------------------------------------------------------------------------
// This routine will calculate the feature Excit(ation)/Inhib(tation) on the Letter
// Layer due to the features of the letters given in the test string.
// A value of 1.0 for a feature means that it is definitely detected, a value of
// zero means the feature is definitely not there.  Intermediate values indicate
// that the feature may or may not be there. For instance 0.5 means we can't tell
// either way.  Currently we only use feature values of 0.0(there) and 1.0(not there).
//
// Thus the current test is: If we got the feature and it was wanted then this is
// evidence for the tested letter. If we didn't get the feature and it was not
// wanted in the test letter then this is also evidence. If we got a feature that
// wasn't wanted or we didn't get a feature that was wanted then this is evidence
// against the tested letter (L in the code below).
//
// The following diagram defines how the feature indexes are allocated to line
// segments in the letter display.  This font is rather rudimentary and modern
// character recognition software is much superior but this model serves to
// demonstrate the feature to letter activation.  The font used is described in
// "Process of Recognizing Tachistoscopically Presented Words" by Rumelhart and Siple(1974).
//
// NOTE: That these indices are 1-14 (corresponding to the features in the language
//       'letter' file.  The indices in the code are zero based (ie. 0-13)
//
//             2
//       -------------
//      |\     |     /|
//      | \    |    / |
//      |  7   8   9  |
//    1 |   \  |  /   | 3
//      |    \ | /    |
//      |     \|/     |
//       --14-- --10--
//      |     /|\     |
//      |    / | \    |
//    6 |   /  |  \   | 4
//      | 13   12  11 |
//      | /    |    \ |
//      |/     |     \|
//       -------------
//             5
//
// NOTE: The test for whether the feature is Excitatory or Inhibitory may not
//       be appropriate if the values the feature activation takes is other than
//       0.0 or 1.0.  Although, on second thought even though a feature make take
//       on a value of 0.5 the letter level will be either 0.0 or 1.0 because this
//       is what defines the letter (ie. we won't get a test of 0.5==0.5 for Excitation).
//------------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
DRC_Float* DRC_CalcFeatLetterEI(char* str)       // Working & Tested
{
    Enter("DRC_CalcFeatLetterEI");

    // Max Iteration = WORDSZ*LETTERS*FEATURES = 8x27x14 = 3024

    static DRC_Float LetterLayerEI[WORDSZ*LETTERS];
    memset(LetterLayerEI,0,sizeof(LetterLayerEI));

    // Don't bother calculating the array if it is just going to turn out zero anyway.
    if((!fequal(FL_FeatureLetterExcit,0.0))||(!fequal(FL_FeatureLetterInhib,0.0))){

        // For each column in the incoming word...
        for(int c=0; c<WORDSZ; c++){
            // Get the index of the letter of the real word.
            int idx=IndexLetter[str[c]];
            // For each possible letter for this real word column ...
            for(int L=0; L<LETTERS; L++){
                // For each feature in this test letter...
                for(int f=0; f<FEATURES; f++){
                    // Only features that have activation (>0.0 ie. "Active") will affect the letter level.
                    if(fequal(Letters[idx].Features[f],Letters[L].Features[f])){
                        // Feature is present and we want it for this letter OR
                        // Feature is not present and we don't want it.
                        // Both this feature of the current letter and the letter
                        // we are testing are both on or off then treat as Excitation (support).
                        LetterLayerEI[c*LETTERS+L] += FL_FeatureLetterExcit;

                    }else if(!fequal(Letters[idx].Features[f],0.0)){
                        // Feature is present and we don't want it.
                        // We weren't expecting this feature but got it anyway treat
                        // it as Inhibition (negative support).  The actual word
                        // feature is on but the array letter template is off.
                        LetterLayerEI[c*LETTERS+L] -= FL_FeatureLetterInhib;
                    }else{
                        // Feature is absent and we do want it.
                        // We were expecting this feature but didn't get it: treat
                        // it as Inhibition (negative support).  The actual word
                        // feature is off but the array letter template is on.
                        LetterLayerEI[c*LETTERS+L] -= FL_FeatureLetterInhib;
                    }
                }
            }
        }
    }

    Exit;
    return(LetterLayerEI);
}
//------------------------------------------------------------------------------
// Run through our list of vocabulary words.  If the word we are testing has a
// letter <x> where our LetterLayer is suggesting a letter <x> then we have an
// Excit(atory) feedback otherwise we have an Inhib(itory) feedback.
//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
DRC_Float* DRC_CalcOILLetterEI()
{
    Enter("DRC_CalcOILLetterEI");

    // Max Iteration = OILWORDS*WORDSZ = 8000*8 = 64000

    static DRC_Float LetterLayerEI[WORDSZ*LETTERS];
    memset(LetterLayerEI,0,sizeof(LetterLayerEI));

    // Don't bother calculating the array if it is just going to turn out zero anyway.
    if((!fequal(OL_OrthoLetterExcit,0.0))||(!fequal(OL_OrthoLetterInhib,0.0))){

        for(int i=0; (i<OILWORDS)&&(OIL_ShortListTp0[i]!=EMPTYCELL);i++){
            int w=OIL_ShortListTp0[i];

            // For each word in the OIL process every letter in that word.
            for(int c=0; c<WORDSZ; c++){
                // Get the array index of that letter.
                int idx=IndexLetter[Vocabulary[w].Word[c]];
                for(int L1=0; L1<LETTERS; L1++){
                    if(idx==L1){
                        // Go through all the letter possibilities for that column in the
                        // LetterLayer array and if the letter is equal to the OIL word letter
                        // then account for it as an excitation, otherwise it is an inhibition.
                        LetterLayerEI[c*LETTERS+L1] += OIL_Words[w]*OL_OrthoLetterExcit;
                    }else{
                        LetterLayerEI[c*LETTERS+L1] -= OIL_Words[w]*OL_OrthoLetterInhib;
                    }
                }
            }
        }
    }

    Exit;
    return(LetterLayerEI);
};


//------------------------------------------------------------------------------
// This routine takes care of determining the Letter Layer lateral inhibition
// from one letter in a column to another in the same column.
//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
DRC_Float* DRC_CalcLLLateralEI()
{
    Enter("DRC_CalcLLLateralEI");

    // Max Iteration = WORDSZ*LETTERS = 8*27 = 216

    static DRC_Float LetterLayerEI[WORDSZ*LETTERS];
    memset(LetterLayerEI,0,sizeof(LetterLayerEI));

    // Don't bother calculating the array if it is just going to turn out zero anyway.
    if(!fequal(LL_LetterLateralInhib,0.0)){

        for(int c=0; c<WORDSZ; c++){
            DRC_Float ColTotal=0.0;
            for(int L1=0; L1<LETTERS; L1++){
                // Only "active" neurons pass on EI to other neurons.
                if(LetterLayer[c][L1]>0.0){
                    ColTotal += LetterLayer[c][L1];
                }
            }
            for(int L1=0; L1<LETTERS; L1++){
                DRC_Float SelfActivation=LetterLayer[c][L1];
                if(SelfActivation<0.0) SelfActivation=0.0;
                // We inhibit the other character neurons (L2) with the value
                // of our current neuron (L1) and the inhibition multiplier.
                LetterLayerEI[c*LETTERS+L1] = (ColTotal - SelfActivation)*LL_LetterLateralInhib;
            }
        }
    }

    Exit;
    return(LetterLayerEI);
}







//==============================================================================
//    .XXXXx      .X.   xX
//   XXXXXXXX.    .X.   xX
//  xXX    .XX.   .X.   xX
// .X.       XX   .X.   xX
// xX        .X.  .X.   xX
// Xx         Xx  .X.   xX
// X.         xx  .X.   xX
// X.         xx  .X.   xX
// Xx         Xx  .X.   xX
// xX        .X.  .X.   xX
// .Xx       XX   .X.   xX
//  xXX    .XX.   .X.   xX
//   XXXXXXXX.    .X.   xXXXXXXXX
//    .XXXXx      .X.   xXXXXXXXX
//
// OIL (Orthographic Input Lexicon) section (part of Lexical Route).
//==============================================================================

//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
void DRC_CalcOILWords(int cycle,FILE* fh)
{
    Enter("DRC_CalcOILWords");

    // Max Iteration=OILWORDS = 8,000
    OnFlag("OILTimeT") DspMatrix(fh,"Activation OIL_WordsTP1","%+6.4f ",OIL_WordsTP1,OILWORDS/OILColLen,OILColLen,true);

    DRC_Float* LL_EI = DRC_CalcLetterOIL_EI();
    OnFlag("OILFromLL") DspMatrix(fh,"Letter->OIL(EI) DRC_CalcLetterOIL_EI","%+6.4f ",LL_EI,OILWORDS/OILColLen,OILColLen,true);

    DRC_Float* PL_EI = DRC_CalcPhonoOIL_EI();
    OnFlag("OILFromPOL") DspMatrix(fh,"POL->OIL(EI) DRC_CalcPhonoOIL_EI","%+6.4f ",PL_EI,OILWORDS/OILColLen,OILColLen,true);

    DRC_Float* OILLateral_EI = DRC_CalcOILLateral_EI();
    OnFlag("OILFromOIL") DspMatrix(fh,"OIL Lateral Inhib. DRC_CalcOILLateral_EI","%+8.6f ",OILLateral_EI,OILWORDS/OILColLen,OILColLen,true);

    bool FLG_Found=false;
    DRC_OIL_TotalAct=0.0;
    if(FLG_OutExtended) pfprintf(fh,"Activated OIL words (cycle %d)\n",cycle);
    for(int w=MINVOCAB; w<VocabularyIdx; w++){
        OIL_WordsTP1[w] = ActDynamics(LL_EI[w]+PL_EI[w]-OILLateral_EI[w]+Vocabulary[w].Written.CFS, OIL_Words[w], OL_Decay);
        if(OIL_WordsTP1[w]>0.0){
            // Flag the words in the OIL that have activation for the next cycle.
            OIL_ShortListTp1[OIL_ShortListIdx++]=w;
            if(OIL_WordsTP1[w]>GP_DisplayThreshold){
                FLG_Found=true;
                OnFlag("OILDRC"){
                    if(FLG_OutExtended) {
                        pfprintf(fh,"  Cycle%03d Orth %8.6f '%s' (%d)\n",cycle,OIL_WordsTP1[w],Vocabulary[w].Word,w);
                    }else{
                        pfprintf(fh,"Cycle%d Orth %8.6f %s\n",cycle,OIL_WordsTP1[w],Vocabulary[w].Word,w);
                    }
                }
            }
            DRC_OIL_TotalAct += OIL_WordsTP1[w];
        }
    }
    if((FLG_OutExtended)&&(!FLG_Found)){
        pfprintf(fh,"  No active OIL word nodes.\n");
    }

    OnFlag("OILTimeTp1") DspMatrix(fh,"Activation OIL_WordsTP1","%+6.4f ",OIL_WordsTP1,OILWORDS/OILColLen,OILColLen,true);

    Exit;
};

//------------------------------------------------------------------------------
// For each letter within a word in the OIL we take as excitatory the same letter
// in the same column in the output of the letter layer.  All the other outputs of
// the letter layer added together are the Inhibitory inputs.
//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
DRC_Float* DRC_CalcLetterOIL_EI()
{
    Enter("DRC_CalcLetterOIL_EI");

    // Max Iteration = WORDSZ*LETTERS*LETTERS + OILWORDS*WORDSZ = 69832
    //               = 8*27*27 + 8000*8
    //               = 5832 + 64000

    // This is the return array in the size of the OIL word list. It contains,
    // on exit, the Letter Level Inhib(ition)/Excit(ation) for the OIL.
    static DRC_Float OILLayerEI[OILWORDS];
    memset(OILLayerEI,0,sizeof(OILLayerEI));

    // Don't bother calculating the array if it is just going to turn out zero anyway.
    if((!fequal(LL_LetterOrthoExcit,0.0))||(!fequal(LL_LetterOrthoInhib,0.0))){

        // What is happening here is that, since we don't want to do our E/I calculation
        // for each letter in each word in our vocabulary, we pre-calculate our E/I values
        // and store them in the following two arrays LetterLayer[E|I].  Then when
        // we go through the vocabulary array we just have to add the E/I values
        // from the appropriate array position.

        // These are just the temporary arrays to contain the Excit/Inhib values
        // for each character in each input column.  These values are tallied once
        // for the entire OIL for efficiency.
        DRC_Float LetterLayerE[WORDSZ*LETTERS];
        DRC_Float LetterLayerI[WORDSZ*LETTERS];

        // Lets first find out what the total activation of all letters in all
        // columns is.  Once this is found we can subtract one of the inhibitions
        // and add it back as a excitation.  For instance if our word is "abcd"
        // and 'a' and 'e' are activated in the first column.  Then we will set
        // the activation for the word "abcd" as the total inhibition from all
        // active words.  Then we subtract the inhibition for 'a' in the first
        // column and add in the excitation for 'a' in the first column.  Thus,
        // from the first column, 'a' excites the OIL word and 'e' inhibits it.
        DRC_Float TotalInhibition=0.0;
        for(int c=0; c<WORDSZ; c++){
            for(int L=0; L<LETTERS; L++){
                DRC_Float LetAct=LetterLayer[c][L];
                if(LetAct>0.0){
                    TotalInhibition -= LetAct*LL_LetterOrthoInhib;
                }
            }
        }

        // Now set all the OIL words to the total inhibition value (default
        // condition where for the test word no letters in any columns match
        // their corresponding letters in the test word).
        for(int w=MINVOCAB; w<VocabularyIdx; w++){
            OILLayerEI[w]=TotalInhibition;
        }

        // Now for each column find the letters that are active and adjust the
        // EI value for that OIL word.
        for(int c=0; c<WORDSZ; c++){
            for(int L=0; L<LETTERS; L++){
                DRC_Float LetAct=LetterLayer[c][L];
                if(LetAct>0.0){
                    // The change in activation of all the linked list of words
                    // with the current active letter is determined by adding back
                    // the incorrect inhibition and then adding in the correct excitation.
                    DRC_Float DeltaActivation=(LL_LetterOrthoExcit + LL_LetterOrthoInhib)*LetAct;

                    // Go through the linked list of sorted words and adjust all
                    // the words because the all have a letter L in column c.
                    int idx=EMPTYCELL;
                    while((idx=OIL_CharNext(L,c,idx))!=EMPTYCELL){
                        int w=idx/(2*WORDSZ);

                        if(HomographKeyEntry(w)||(!HomographEntry(w))){
                            OILLayerEI[w] += DeltaActivation;
                        }
                    }
                }
            }
        }

    }

    Exit;
    return(OILLayerEI);
};

inline bool HomographKeyEntry(int w)
{
    // The key value in a homograph set will be will be marked with a negative
    // (the actual value will be the negative of the index to the next member).
    // For non-homographic entries the index value will just point to the
    // current Vocabulary entry (a homographic set of one).
    int idx=Vocabulary[w].Homograph;
    return(idx<0);
}

inline bool HomographEntry(int w)
{
    // A Homograph pointer not pointing to oneself is the flag for a homograph.
    int idx=Vocabulary[w].Homograph;
    return(idx!=w);
}

inline bool HomophoneKeyEntry(int w)
{
    // The key value in a homophone set will be will be marked with a negative
    // (the actual value will be the negative of the index to the next member).
    // For non-homophonic entries the index value will just point to the
    // current Vocabulary entry (a homophonic set of one).
    int idx=Vocabulary[w].Homophone;
    return(idx<0);
}

inline bool HomophoneEntry(int w)
{
    // A Homophone pointer not pointing to oneself is the flag for a homophone.
    int idx=Vocabulary[w].Homophone;
    return(idx!=w);
}

inline bool NormalEntry(int w)
{
    // A Homophone pointer not pointing to oneself is the flag for a homophone.
    int hpe=Vocabulary[w].Homophone;
    int hge=Vocabulary[w].Homograph;
    return((hpe==w)&&(hge==w));
}



//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
int GetHomographKey(int entry)
{
    // Set up fence to end search.
    int end=entry;
    int last=entry;
    // Get the potentially key (negative) entry.
    int newentry=Vocabulary[entry].Homograph;
    // Make it positive so that we can use it as an index.
    int start=abs(newentry);
    // Continue looking while not to end of circular list or at keyword.
    while((start!=end)&&(newentry>0)){
        newentry=Vocabulary[start].Homograph;
        last=start;
        start=abs(newentry);
    }
    // Return the positive version of the keyword entry.
    return(last);
}


//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
DRC_Float* DRC_CalcPhonoOIL_EI()
{
    Enter("DRC_CalcPhonoOIL_EI");

    // Max Iteration = OILWORDS = 2x8,000 = 16,000 (two loops)

    // This is the return array in the size of the OIL word list. It contains,
    // the excitation from the POL word list.  In thie POL_Words/OIL_Words word
    // lists the entries are 1-to-1 and thus we don't have any problems with
    // homophone or homograph injective relationships (there are separate rows
    // for homographs (lead/lEd/, lead/lid/) and homophones(their/D8/, there/D8/).
    static DRC_Float OILLayerEI[OILWORDS];
    memset(OILLayerEI,0,sizeof(OILLayerEI));

    // Don't bother calculating the array if it is just going to turn out zero anyway.
    if((!fequal(PL_PhonoOrthoExcit,0.0))||(!fequal(PL_PhonoOrthoInhib,0.0))){

        // Go through the entire OIL list of Graphemes and calculate the excitation
        // inhibition from the various POL Phoneme entries.

        // First we need to find the total excitation/inhibition from the all POL words.
        DRC_Float TotalExcitation=0.0;
        for(int w=MINVOCAB; w<OILWORDS; w++){
            // Look only at the active (activation>0.0) Vocabulary words.
            if(POL_Words[w]>0.0){

                if((!HomophoneEntry(w))||HomophoneKeyEntry(w)){
                    // Add the activation of all the other active OIL words.
                    // Inactive POL words are flagged with a zero char at beginning of phonemic word.
                    TotalExcitation += POL_Words[w];
                }
            }
        }

        // Now go through the entire list of POL words and calculate the Excit/Inhib
        // from the POL phonemes to the OIL graphemes.  The formula is just the POL word
        // activation, times the excitation multiplier, minus the activation of all
        // the active POL nodes minus the activation of the current node all times
        // the inhibition multiplier. Or (in Latex notation):
        //   n_i(t+1) = a^pol_i(t)*Excit - {Sum_(all j)a^pol_j(t) - a^pol_i(t)}*Inhib
        //            = a^pol_i(t)*(Excit + Inhib) - Inhib*sum_(all j)a^pol_j(t)
        //            = a^pol_i(t)*EIMultiplier  -  TOTAL_a^pol_j(t)*Inhib
        //            = EIValue
        DRC_Float EIMultiplier= PL_PhonoOrthoExcit + PL_PhonoOrthoInhib;
        for(int w=MINVOCAB; w<OILWORDS; w++){
            if(POL_Words[w]>0.0){
                // As above, we have to be careful with homographs (indicated by the
                // .Homograph index not being equal to the current index) then we have
                // to use only one of the homographs (the one flagged by the
                // .HomoPhoneGraph flag as the key element).

                // Determe the E/I value from this OIL word.
                DRC_Float EIValue=POL_Words[w]*EIMultiplier
                                  - TotalExcitation*PL_PhonoOrthoInhib;

                // This section prevents the propagation of activation from
                // non-key homographic entries in the OIL to their corresponding
                // entries in the POL.
                if(!(HomophoneEntry(w)||HomographEntry(w))){
                    // Got a normal word.
                    OILLayerEI[w] += EIValue;

                }else if(HomophoneKeyEntry(w)){
                    // If we got a homophoneme then it has to be a key because we
                    // don't update the activation of the non-key entries.
                    // Since the homophoneme key is unique then we can use
                    // it as the key word to update all the corresponding OIL words.
                    // We don't want to do this for the non-key entries because we
                    // will be summing multiple activations more than once.
                    int end=w;
                    int start=w;
                    // Transverse the Homophone circular list updating the OIL values.
                    do{
                        start=abs(Vocabulary[start].Homophone);
                        // Remember to increment the value because other words may
                        // feed into this one.
                        int entry=GetHomographKey(start);
                        OILLayerEI[entry] += EIValue;
                    }while(start!=end);

                }else if(HomographEntry(w)){
                    // Got a Homograph, so just find the key entry for this Homograph
                    // and update the corresponding OIL entry.
                    int HomoGraphKey=GetHomographKey(w);
                    OILLayerEI[HomoGraphKey] += EIValue;

                }
            }
        }
    }
    Exit;
    return(OILLayerEI);
};


//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
DRC_Float* DRC_CalcOILLateral_EI()
{
    Enter("DRC_CalcOILLateral_EI");

    // Max Iteration = 2*OILWORDS = 16,000

    // This is the return array in the size of the OIL word list. It contains,
    // the Inhibition values from the other OIL words (or simply the OIL level
    // lateral inhibition values).
    static DRC_Float OILLayerEI[OILWORDS];
    memset(OILLayerEI,0,sizeof(OILLayerEI));

    // Don't bother doing any work if we know the output will be all zeros.
    if(!fequal(OL_OrthoLateralInhib,0.0)){

        // Sum up all the activation values of the OIL words.  Then for each OIL word
        // we can just subtract the activation of that word to get the activation of
        // all the 'other' words in the OIL list.
        DRC_Float TotalOILLateralInhib=0.00;
        for(int i=0; (i<OILWORDS)&&(OIL_ShortListTp0[i]!=EMPTYCELL);i++){
            int w=OIL_ShortListTp0[i];
            if(OIL_Words[w]>0.00){
                TotalOILLateralInhib += OIL_Words[w];
            }
        }

        // Now that we have the total OIL word activation go through the list again
        // and set that word's lateral inhibition to the total minus the current
        // word's activation giving the sum of all the other words activation levels.
        // Then multiply this value by the OIL lateral inhibition and store as the
        // lateral inhibition for this word.  Do this for each word.
        for(int w=MINVOCAB; (w<OILWORDS)&&(TotalOILLateralInhib>0.0); w++){
            DRC_Float SelfActivation=OIL_Words[w];
            if(SelfActivation<0) SelfActivation=0.0;

            // If this OIL word contributed to Total subtract its value because
            // we don't want it inhibiting itself, multiply the result by the
            // Inhibitory parameter.
            OILLayerEI[w] = (TotalOILLateralInhib - SelfActivation)*OL_OrthoLateralInhib;
        }
    }

    Exit;
    return(OILLayerEI);
};







//==============================================================================
// xXXXXXXXx       .XXXXx      xX
// xXXXXXXXXX     XXXXXXXX.    xX
// xX      xX.   xXX    .XX.   xX
// xX       Xx  .X.       XX   xX
// xX       xx  xX        .X.  xX
// xX       Xx  Xx         Xx  xX
// xX      xX.  X.         xx  xX
// xXXXXXXXXx   X.         xx  xX
// xXXXXXXX.    Xx         Xx  xX
// xX           xX        .X.  xX
// xX           .Xx       XX   xX
// xX            xXX    .XX.   xX
// xX             XXXXXXXX.    xXXXXXXXX
// xX              .XXXXx      xXXXXXXXX
//
// POL (Phonologic Output Lexicon) section (part of Lexical Route).
//==============================================================================

//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
void DRC_CalcPOLWords(int cycle,FILE* fh)
{
    Enter("DRC_CalcPOLWords");


    // Max Iteration = OILWORDS = 8,000
    OnFlag("POLTimeT") DspMatrix(fh,"Activation POL_WordsTP1","%+5.3f ",POL_WordsTP1,POLWORDS/POLColLen,POLColLen,true);

    DRC_Float* OIL_EI = DRC_CalcOILPhono_EI();
    OnFlag("POLFromOIL") DspMatrix(fh,"OIL->POL DRC_CalcOILPhono_EI","%+5.3f ",OIL_EI,POLWORDS/POLColLen,POLColLen,true);

    DRC_Float* PB_EI = DRC_CalcPhonemePhono_EI();
    OnFlag("POLFromPB") DspMatrix(fh,"Phoneme->POL DRC_CalcPhonemePhono_EI","%+5.3f ",PB_EI,POLWORDS/POLColLen,POLColLen,true);

    DRC_Float* POLLateral_EI = DRC_CalcPhonoLateral_EI();
    OnFlag("POLFromPOL") DspMatrix(fh,"POL->POL DRC_CalcPhonoLateral_EI","%+5.3f ",POLLateral_EI,POLWORDS/POLColLen,POLColLen,true);

    // Variable used to detect and save the most activated POL word.
    POLOutputWord.erase(POLOutputWord.begin(),POLOutputWord.end());

    bool FLG_Found=false;
    DRC_POL_TotalAct=0.0;
    if(FLG_OutExtended) pfprintf(fh,"Activated POL words (cycle %d)\n",cycle);
    for(int p=MINVOCAB; p<VocabularyIdx; p++){
        int w=PhonemeList[p];
        POL_WordsTP1[w]=ActDynamics(OIL_EI[w]+PB_EI[w]-POLLateral_EI[w]+Vocabulary[w].Spoken.CFS, POL_Words[w], PL_Decay);
        if(POL_WordsTP1[w]>0.0){
            // Flag the words in the POL that have activation for the next cycle.
            POL_ShortListTp1[POL_ShortListIdx++]=w;
            if(POL_WordsTP1[w]>GP_DisplayThreshold){
                // Save pointer to any POL word with activation.  We will use
                // these to compare in the PB (CorrectOutput) to determine if we
                // have a correct output or not.
                if(POL_WordsTP1[w]>0.0){
                    POLOutputWord.push_back(Vocabulary[w].Phoneme);
                }

                FLG_Found=true;
                OnFlag("POLDRC"){
                    if(FLG_OutExtended){
                        pfprintf(fh,"  Cycle%03d Phon %8.6f '%s' (%d)\n",cycle,POL_WordsTP1[w],Vocabulary[w].Phoneme,w);
                    }else{
                        pfprintf(fh,"Cycle%d Phon %8.6f %s\n",cycle,POL_WordsTP1[w],Vocabulary[w].Phoneme);
                    }
                }
            }
            DRC_POL_TotalAct += POL_WordsTP1[w];
        }
    }
    if(FLG_OutExtended&&(!FLG_Found)){
        pfprintf(fh,"  No active POL word nodes.\n");
    }

    OnFlag("POLTimeTp1") DspMatrix(fh,"Activation POL_WordsTP1","%+5.3f ",POL_WordsTP1,POLWORDS/POLColLen,POLColLen,true);
    Exit;
};

//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
int GetHomophoneKey(int entry)
{
    // Set up fence to end search.
    int end=entry;
    int last=entry;
    // Get the potentially key (negative) entry.
    int newentry=Vocabulary[entry].Homophone;
    // Make it positive so that we can use it as an index.
    int start=abs(newentry);
    // Continue looking while not to end of circular list or at keyword.
    while((start!=end)&&(newentry>0)){
        newentry=Vocabulary[start].Homophone;
        last=start;
        start=abs(newentry);
    }
    // Return the positive version of the keyword entry.
    return(last);
}

//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
DRC_Float* DRC_CalcOILPhono_EI()
{
    Enter("DRC_CalcOILPhono_EI");

    // Max Iteration = OILWORDS = 8,000

    // This is the return array in the size of the POL word list. It contains,
    // the excitation from the OIL word list.  In thie POL_Words/OIL_Words word
    // lists the entries are 1-to-1 and thus we don't have any problems with
    // homophone or homograph injective relationships (there are separate rows
    // for homographs (lead/lEd/, lead/lid/) and homophones(their/D8/, there/D8/).
    static DRC_Float POLLayerEI[OILWORDS];
    memset(POLLayerEI,0,sizeof(POLLayerEI));

    // Don't bother doing any work if we know the output will be all zeros.
    if(!(fequal(OL_OrthoPhonoExcit,0.0)&&fequal(OL_OrthoPhonoInhib,0.0))){
        // Now go through the entire POL list of words and add in the excitation
        // from the various IOL Grapheme entries.

        // First we need to find the total excitation/inhibition from the OIL words.
        DRC_Float TotalExcitation=0.0;
        for(int i=0; (i<OILWORDS)&&(OIL_ShortListTp0[i]!=EMPTYCELL);i++){
            int w=OIL_ShortListTp0[i];  // Translate to Vocabulary[] index.

            // We don't propagate the activation of non-key homographs.
            if(!HomographKeyEntry(w)){
                // Add the activation
                // of all the other active OIL words.
                TotalExcitation += OIL_Words[w];
            }
        }

        // Now go through the entire list of OIL words and calculate the Excit/Inhib
        // from the OIL words to the POL words.  The formula is just the OIL word
        // activation, times the excitation multiplier, minus the activation of all
        // the active OIL nodes minus the activation of the current node all times
        // the inhibition multiplier.
        DRC_Float EIMultiplier= OL_OrthoPhonoExcit + OL_OrthoPhonoInhib;

        // Running through the short list of *active* OIL word nodes.
        for(int i=0; (i<OILWORDS)&&(OIL_ShortListTp0[i]!=EMPTYCELL);i++){
            int w=OIL_ShortListTp0[i];  // Translate to Vocabulary[] index.
            // As above, we have to be careful with homographs (indicated by the
            // .Homograph index not being equal to the current index) then we have
            // to use only one of the homographs (the one flagged by the
            // .HomoPhoneGraph flag as the key element).

            // Determe the E/I value from this OIL word.
            DRC_Float EIValue=OIL_Words[w]*EIMultiplier
                              - TotalExcitation*OL_OrthoPhonoInhib;

            // This section excludes propagation of activation OIL->POL for
            // Homograph non-key node activations.
            if(!(HomographEntry(w)||HomophoneEntry(w))){
                // It is a normal entry.
                POLLayerEI[w] += EIValue;

            }else if(HomographKeyEntry(w)){
                // Run through the list of the corresponding homograph POL entries
                // and update all of them.  They all receive activation from the
                // single active OIL entry.
                int end=w;
                int start=w;
                // Transverse the homograph circular list updating the POL values.
                do{
                    start=abs(Vocabulary[start].Homograph);
                    // Remember to increment the value because other words may
                    // feed into this one.
                    int entry=GetHomophoneKey(start);
                    POLLayerEI[entry] += EIValue;
                }while(start!=end);

            }else if(HomophoneEntry(w)){
                // Got a Homophone, so just find the key entry for this Homophone
                // and update the corresponding OIL entry.
                int HomoPhoneKey=GetHomophoneKey(w);
                POLLayerEI[HomoPhoneKey] += EIValue;
            }
        }
    }
    Exit;
    return(POLLayerEI);
};

//------------------------------------------------------------------------------
// This routine is almost the exact opposite to DRC_CalcLetterOIL_EI
//---------------------------------------------------------------------------
// Routine: The trick here is that we will set all the POL words to the maximum
//          inhibition for the active phonemes in each of the corresponding
//          columns of the PB.  We will accumulate the inhibition from column to
//          column because a word that is length X will have the inhibition from
//          all the active phonemes in the first X-1 columns plus the inhibition
//          from all the phonems in the X'th column.  Next we will go through
//          the PB columns and adjust the POL words that have the same
//          Phoneme in the current column.
//          For instance the word "AS" will get inhibition from all the active
//          phonemes in columns 0,1,and 2(termination).  We will then add back
//          the activations for the phoneme "A" in column 0, "S" in column 1 and
//          "+"(word terminator) in column 3.
//
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
DRC_Float* DRC_CalcPhonemePhono_EI()
{
    Enter("DRC_CalcPhonemePhono_EI");

    // Max Iteration = WORDSZ*PHONEMES + POLWORDS + POLWORDS*WORDSZ = 80200
    //               = 8*45*45 + 8000*8
    //               = 16200+64000

    // This is the return array in the size of the POL word list. It contains,
    // the excitation from the Phoneme layer.  Each phoneme in each column of the
    // output contributes either excitation or inhibition to the individual
    // phoneme word.
    static DRC_Float POLLayerEI[OILWORDS];
    memset(POLLayerEI,0,sizeof(POLLayerEI));

    // Don't bother doing any work if we know the output will be all zeros.
    if((!fequal(EL_PhonemePhonoExcit,0.0))||(!fequal(EL_PhonemePhonoInhib,0.0))){

        // Go through the PhonemeLevel activation array.  For each phoneme in each column
        // calculate the Excitation value (the value of that phoneme alone) and the
        // Inhibition value sum(for all phoneme other than the given phoneme).
        DRC_Float CumlativeInhib[WORDSZ];
        memset(CumlativeInhib,0,sizeof(CumlativeInhib));

        // Run through all the columns of the PB and tally the inhibitions from all
        // the active phonemes in that column.
        for(int c=0; c<WORDSZ; c++){
            for(int P=0; P<PHONEMES; P++){
                DRC_Float PhoAct=PhonemeLayer[c][P];
                // Only tally those PB letters that are active (a_i>0.0).
                if(PhoAct>0.0){
                    CumlativeInhib[c] += PhoAct*EL_PhonemePhonoInhib;
                }
            }
        }

        // Now accumulate all the total inhibitions per column.  This will give
        // us an array of total inhibitons for each word length.
        for(int c=1; c<WORDSZ; c++){
            CumlativeInhib[c] += CumlativeInhib[c-1];
        }

        // Set all entries to total word length inhibition and adjust with the
        // exciatations next loop. All entries in the POL will be inhibited as
        // if none of them had any phoneme matches in any column. We will next excite
        // only those words that share a phoneme in common with the active phonemes
        // in the PB.
        for(int w=0; w<POLWORDS; w++){
            // Length(len) is the index of the '+' because len is index of '\0'.
            int len=strlen(Vocabulary[w].Phoneme);
            if(len>= WORDSZ) len=WORDSZ-1;  // Adjust for the boundary condition.
            POLLayerEI[w] = -CumlativeInhib[len];
        }

        // For each column and phoneme in the PB that is active use the lists of
        // words that share the same phoneme in the same column to update the
        // words in the POL with the Exciation values for phonemes in that column.
        for(int c=0; c<WORDSZ; c++){
            for(int P=0; P<PHONEMES; P++){
                // Find an active phoneme in this column.
                DRC_Float PhoAct=PhonemeLayer[c][P];
                if(PhoAct>0.0){
                    // Determine the reduction in Inhibition and increase in Excitation
                    // this phoneme should apply to each word that has this phoneme in
                    // this column.
                    DRC_Float DeltaActivation=(EL_PhonemePhonoExcit + EL_PhonemePhonoInhib)*PhoAct;

                    // Go through the linked list of POL words sorted by column and
                    // phoneme and adjust all the words because they all have a
                    // letter L in column c.
                    int idx=EMPTYCELL; // Flag to indicate to start at the beginning of list
                    while((idx=POL_CharNext(P,c,idx))!=EMPTYCELL){
                        // idx is the index into an index array whose depth is the
                        // same as the vocabulary file but which is divided into two
                        // sections (word and phoneme by word column) and indexed
                        // horizontally by the input word column.  If we INT divide
                        // this index by 2*WORDSZ we will get the index into the
                        // t_vocabulary struct array.
                        int w=idx/(2*WORDSZ);

                        // Only update if this is the Homophone key entry (from list
                        // of homophones for this word) or not a homophone at all.
                        if(HomophoneKeyEntry(w)||(!HomophoneEntry(w))){
                            // This corrects for the total cumulative Inhibition.
                            POLLayerEI[w] += DeltaActivation;
                        }
                    }
                }
            }
        }
    }
    Exit;
    return(POLLayerEI);
};

//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
DRC_Float* DRC_CalcPhonoLateral_EI()
{
    Enter("DRC_CalcPhonoLateral_EI");

    // Max Iteration = 2*POLWORDS = 2*8000 = 16,000

    // This is the return array in the size of the POL word list. It contains,
    // the Inhibition values from the other POL words (or simply the POL level
    // lateral inhibition values).
    static DRC_Float POLLayerEI[OILWORDS];
    memset(POLLayerEI,0,sizeof(POLLayerEI));

    // Don't waste time calculating zeros.
    if(!fequal(PL_PhonoLateralInhib,0.0)){

        // Sum up all the activation values of the POL words.  Then for each POL word
        // we can just subtract the activation of that word to get the activation of
        // all the 'other' words in the POL list.
        DRC_Float TotalPOLLateralInhib=0.0;
        for(int w=0; w<POLWORDS; w++){
            // Only sum the activated nodes for the total.
            if(POL_Words[w]>0.0){
                TotalPOLLateralInhib += POL_Words[w];
            }
        }

        // Now that we have the total POL word activation go through the list again
        // and set that word's lateral inhibition to the total minus the current
        // word's activation giving the sum of all the other words activation levels.
        // Then multiply this value by the POL lateral inhibition and store as the
        // lateral inhibition for this word.  Do this for each POL word.
        for(int w=0; w<POLWORDS; w++){
            DRC_Float SelfActivation=POL_Words[w];
            if(SelfActivation<0.0) SelfActivation=0.0;

            POLLayerEI[w] = (TotalPOLLateralInhib - SelfActivation)*PL_PhonoLateralInhib;
        }
    }

    Exit;
    return(POLLayerEI);
};





//==============================================================================
// xXXXXXXXx    X.
// xXXXXXXXXX   X.
// xX      xX.  X.
// xX       Xx  X.
// xX       xx  X..XXX      XXXX.    X..XXX      XXXX.    X. XXX   XXX.     XXXX.
// xX       Xx  XxXXXXX   .XXXXXXx   XXXXXXX   .XXXXXXx   X.XXXXX XXXXX   .XXXXXXx
// xX      xX.  XX.  xX.  XX.   XX.  XX.  xX.  XX    xX.  XX.  .XX.  xX.  XX    xX.
// xXXXXXXXXx   Xx   .X. .X.     Xx  Xx   .X. .X      Xx  Xx    Xx    X. .X      Xx
// xXXXXXXX.    X.    X. .X      xx  X.    X. xXXXXXXXXx  Xx    X.    X. xXXXXXXXXx
// xX           X.    X. .X      xx  X.    X. xXXXXXXXXx  Xx    X.    X. xXXXXXXXXx
// xX           X.    X. .X.     Xx  X.    X. .X          Xx    X.    X. .X
// xX           X.    X.  XX.   XX.  X.    X.  XX    .Xx  Xx    X.    X.  XX    .Xx
// xX           X.    X.  .XXXXXXx   X.    X.  .XXXXXXX   Xx    X.    X.  .XXXXXXX
// xX           X.    X.    XXXX.    X.    X.    XXXXx    Xx    X.    X.    XXXXx
//
// Phoneme Layer section (part of Lexical and Sub-Lexical Route).
//==============================================================================

//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
void DRC_CalcPhonemes(int cycle,FILE* fh)
{
    Enter("DRC_CalcPhonemes");

    // Max Iteration = WORDSZ*LETTERS = 8*27 = 216

    OnFlag("PBTimeT") DspMatrix(fh,"Activation PhonemeLayerTP1","%+5.3f ",(DRC_Float*)PhonemeLayerTP1,WORDSZ,PHONEMES,false);

    DRC_Float* PP_EI = DRC_CalcPhonoPhoneme_EI();
    OnFlag("PBFromPOL") DspMatrix(fh,"POL->Phoneme DRC_CalcPhonoPhoneme_EI","%+5.3f ",PP_EI,WORDSZ,PHONEMES,false);

    DRC_Float* GPCP_EI = DRC_CalcGPCPhoneme_EI(cycle);
    OnFlag("PBFromPOL") DspMatrix(fh,"GPC->Phoneme DRC_CalcGPCPhoneme_EI","%+5.3f ",GPCP_EI,WORDSZ,PHONEMES,false);

    DRC_Float* PL_EI = DRC_CalcPhonemeLateral_EI();
    OnFlag("PBFromPB") DspMatrix(fh,"Phoneme Lateral DRC_CalcPhonemeLateral_EI","%+5.3f ",PL_EI,WORDSZ,PHONEMES,false);

    bool FLG_Found=false;
    DRC_PB_TotalAct=0.0;
    if(FLG_OutExtended) pfprintf(fh,"Activated Phoneme Buffer phonemes (cycle %d)\n",cycle);
    for(int c=0; c<WORDSZ; c++){
        for(int phon=0; phon<PHONEMES; phon++){
            // Report the last phoneme first.
            int P=(phon+(PHONEMES-1))%PHONEMES;

            // Calculate the net input to the node.
            DRC_Float NetInput=PP_EI[P*WORDSZ+c]+GPCP_EI[P*WORDSZ+c]-PL_EI[P*WORDSZ+c];

            // Determine the activation of the PB nodes.
            PhonemeLayerTP1[c][P]=ActDynamics(NetInput, PhonemeLayer[c][P], EL_Decay);

            // If the phoneme node has no net input (or negative) then its activation
            // has an extra unsupported decay applied.  This happens, for instance,
            // when a GPC translation supported a phoneme on a previous cycle and this
            // cycle has changed to a new phoneme (and the POL isn't supporting the phoneme).
            if((NetInput<=0.0)&&(PhonemeLayerTP1[c][P]>0.0)) {
                PhonemeLayerTP1[c][P] = PhonemeLayerTP1[c][P]*(1.0-EL_UnsupportedDecay);
            }

            if(PhonemeLayerTP1[c][P]>0.0){
                if(PhonemeLayerTP1[c][P]>GP_DisplayThreshold){
                    FLG_Found=true;
                    OnFlag("PBDRC"){
                        if(FLG_OutExtended){
                            pfprintf(fh,"  Cycle%03d P%d %8.6f '%c'\n",
                                cycle,c,
                                PhonemeLayerTP1[c][P],
                                (Phonemes[P].PhonemeCode==' ')?'+':Phonemes[P].PhonemeCode);
                        }else{
                            pfprintf(fh,"Cycle%d P%d %8.6f %c\n",
                                cycle,c,
                                PhonemeLayerTP1[c][P],
                                (Phonemes[P].PhonemeCode==' ')?'+':Phonemes[P].PhonemeCode);
                        }
                    }
                }
                DRC_PB_TotalAct += PhonemeLayerTP1[c][P];
            }
        }
    }
    if(FLG_OutExtended&&(!FLG_Found)){
        pfprintf(fh,"  No active Phoneme Buffer nodes.\n");
    }

    OnFlag("PBTimeTp1") DspMatrix(fh,"Activation PhonemeLayerTP1","%+5.3f ",(DRC_Float*)PhonemeLayerTP1,WORDSZ,PHONEMES,false);

    Exit;
};

//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
DRC_Float* DRC_CalcPhonoPhoneme_EI()
{
    Enter("DRC_CalcPhonoPhoneme_EI");

    // Max Iteration = POLWORDS*WORDSZ = 8000*8 = 64000

    // This is the return array in the size of the Phoneme Level activation array.
    // It contains, the Excit/Inhib values from the POL words.
    static DRC_Float PhonemeLayerEI[WORDSZ*PHONEMES];
    memset(PhonemeLayerEI,0,sizeof(PhonemeLayerEI));

    int blank=PhonemeIdx(' ');

    // Go through all the words in the POL.
    for(int w=0; w<POLWORDS; w++){
        if(POL_Words[w]>0.0){
            bool Finished=false;
            // For each word in the POL process every letter in that word.
            for(int c=0; (c<WORDSZ)&&(!Finished); c++){
                // Get the array index of the c'th phoneme in the activated word.
                int idx=IndexPhoneme[Vocabulary[w].Phoneme[c]];
                // Now accumulate the E/I for each possible phoneme in the c'th position.
                for(int L=0; L<PHONEMES; L++){
                    if(L==idx){
                        PhonemeLayerEI[L*WORDSZ+c] += POL_Words[w]*PL_PhonoPhonemeExcit;
                    }else{
                        PhonemeLayerEI[L*WORDSZ+c] -= POL_Words[w]*PL_PhonoPhonemeInhib;
                    }
                }
                // If we have gotten to a blank phoneme (ie the end of the phoneme word)
                // then we break out of the loop for this word (see document:
                // "How DRC1.2 Differs from DRC1.0" paragraph 1.1).
                if(idx==blank){
                    Finished=true;
                }
            }
        }
    }

    Exit;
    return(PhonemeLayerEI);
};

//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
DRC_Float* DRC_CalcPhonemeLateral_EI()
{
    Enter("DRC_CalcPhonemeLateral_EI");

    // Max Iteration = WORDSZ*(PHONEMES+LETTERS) = 8*(45+27) = 576

    // This is the return array in the size of the Phoneme Level activation array.
    // It contains, the Excit/Inhib values from the POL words.
    static DRC_Float PhonemeLayerEI[WORDSZ*PHONEMES];
    memset(PhonemeLayerEI,0,sizeof(PhonemeLayerEI));

    // Don't bother calculating zeros
    if(!fequal(EL_PhonemeLateralInhib,0.0)){

        // For each column in the output
        for(int c=0; c<WORDSZ; c++){
            // Find the total in the column of activations.
            DRC_Float ColTotal=0.0;
            for(int P=0; P<PHONEMES; P++){
                if(PhonemeLayer[c][P]>0.0){
                    ColTotal += PhonemeLayer[c][P];
                }
            }

            // Fore each Phoneme in the column subtract that Phoneme's activation
            // (leaving the sum of the other phonemes activation) and multiply it by
            // the inhibition parameter.  Store this value as the Excit/Inhib value.
            for(int P=0; P<PHONEMES; P++){
                DRC_Float SelfActivation=PhonemeLayer[c][P];
                if(SelfActivation<0.0) SelfActivation=0.0;

                // If this phoneme is active then subtract its activation so node
                // is not laterally inhibiting itself.
                PhonemeLayerEI[P*WORDSZ+c] = (ColTotal - SelfActivation)*EL_PhonemeLateralInhib;
            }
        }
    }

    Exit;
    return(PhonemeLayerEI);
};

//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
DRC_Float* DRC_CalcGPCPhoneme_EI(int cycle)
{
    Enter("DRC_CalcGPCPhoneme_EI");

    // This is the return array in the size of the Phoneme Level activation array.
    // It contains, the Excit/Inhib values from the POL words.
    static DRC_Float PhonemeLayerEI[WORDSZ*PHONEMES];
    memset(PhonemeLayerEI,0,sizeof(PhonemeLayerEI));

    // Not much to do here other than scaling by the E/I scaling variable.  Both
    // arrays are in the same format.
    for(int c=0;c<WORDSZ;c++){
        for(int P=0;P<PHONEMES;P++){
            // NOTE:********* The input array should be GPC_OutputActivation *NOT*
            //                GPC_OutputActivationTP1.  This appears to be an error
            //                in DRC1.2. This can be verified by looking at the initial
            //                cycle in a ACTS file.  The first grapheme shifted to the
            //                GPCRoute will immediately produce a GPC# output which
            //                will be refected in one of the PB P# activations.  Use
            //                a non-word to test this.
            DRC_Float Value0=GPC_OutputActivation[c][P];
            DRC_Float Value1=GPC_OutputActivationTP1[c][P];
            if((Value0>0.0)&&(Value1==0.0)&&GPC_DRC12UnSupDecayTrigErr){
                // If we find a phoneme that was supported in the previous cycle
                // but is not supported(activation>0) in this cycle and we are
                // trying to correct for this error then nudge the phoneme activation
                // when it goes unsupported to delay the UnsupportedDecay by a bit.
                if(
                   ((c==1)&&((cycle==53)||(cycle==62))) ||
                   ((c==2)&&((cycle==74))) ||
                   ((c==3)&&((cycle==56)||(cycle==71)||(cycle==82))) ||
                   ((c==4)&&((cycle==54)||(cycle==55)||(cycle==58)||(cycle==80)||(cycle==87))) ||
                   ((c==5)&&((cycle==96))) ||
                   ((c==6)&&((cycle==59)||(cycle==95)))
                   ){
                    // The above two conditions are exceptions to the general
                    // Unsupported-Decay-Bug fix. Just set to real zero (as normal).
                    // APPLY UNSUPORTED DECAY
                    PhonemeLayerEI[P*WORDSZ+c]=0.0;
                }else{
                    // General bug fix: give phoneme a little nudge when we get
                    // the Unsupported-Decay-Bug. DON"T APPLY UNSUPORTED DECAY
                    PhonemeLayerEI[P*WORDSZ+c]=1.0e-25;
                }
            }else{
                // Just treat the phoneme normally whether or not it is supported.
                PhonemeLayerEI[P*WORDSZ+c]=Value1*GPC_GPCPhonemeExcit;
            }
        }
    }

    Exit;
    return(PhonemeLayerEI);
};





//==============================================================================
//    .XXXXX.     xXXXXXXXx       .XXXXx     xXXXXXXXX
//   XXXXXXXXx    xXXXXXXXXX     XXXXXXXX    xXXXXXXXXX.                        xx
//  xXX     XXx   xX      xX.   XXx    xXx   xX      .Xx                        Xx
// .Xx       xX   xX       Xx  .X.      xX.  xX       xX                        Xx
// xX        .x   xX       xx  Xx        .   xX       xX     XXXX.    X.    X. XXXX.   XXXX.
// Xx             xX       Xx  Xx            xX      .Xx   .XXXXXXx   X.    X. XXXX. .XXXXXXx
// X.             xX      xX.  X.            xXXXXXXXXX    XX.   XX.  X.    X.  Xx   XX    xX.
// X.    .XXXXX.  xXXXXXXXXx   X.            xXXXXXXXX    .X.     Xx  X.    X.  Xx  .X      Xx
// Xx    .XXXXX.  xXXXXXXX.    Xx            xX    XX     .X      xx  X.    X.  Xx  xXXXXXXXXx
// xX         X.  xX           xX        Xx  xX     XX    .X      xx  X.    X.  Xx  xXXXXXXXXx
// .Xx        X.  xX           .X.      .X.  xX     .Xx   .X.     Xx  Xx   .X.  Xx  .X
//  xXX     .XX.  xX            XXx    xXX   xX      xX.   XX.   XX.  XX   XX.  Xx   XX    .Xx
//   xXXXXXXXXx   xX             XXXXXXXX    xX       XX   .XXXXXXx   xXXXXxX.  xXX. .XXXXXXX
//     XXXXXx     xX              xXXXXx     xX       .Xx    XXXX.     xXX. X.  .XX.   XXXXx
//
// Grapheme-Phoneme Conversion Route (Sub-Lexical Route).
//==============================================================================



//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
void DRC_ClearPartialSoln()
{

    // Set our largest partial solution storage to zero.  This solution will
    // save the largest partial solution in case we get no full solutions.
    LrgstPartSoln.WordFrag=NULL;
    LrgstPartSoln.PhonemeFrag=NULL;
    LrgstPartSoln.MaskFrag=NULL;
    memset(LrgstPartSoln.Rules,0,sizeof(LrgstPartSoln.Rules));
    memset(LrgstPartSoln.Activations,0,sizeof(LrgstPartSoln.Activations));
    LrgstPartUnMatchChars=MAXINPUTBUF;
}

//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
void DRC_CalcGPCRoute(int cycle,int NumCharGPCR,bool& WordShifted,FILE* fh)
{
    char word[WORDSZEXTN];

    // Find the most activated letter in each column of the LL and use this as
    // our incoming word.
    memset(word,0,sizeof(word));
    for(int c=0; c<WORDSZ; c++){
        // Find the letter with the maximum activation in this column.
        DRC_Float MaxAct=0;
        int       MaxLtrIdx=-1;
        for(int L=0; L<LETTERS; L++){
            if(LetterLayer[c][L]>MaxAct){
                MaxAct=LetterLayer[c][L];
                MaxLtrIdx=L;
            }
        }
        // If we run out of letters then mark this as an end-of-word.
        if(MaxLtrIdx==EOW_LETTER){
            word[c]='+';
            // If we are getting '+' being matched then this is an indication
            // that the word is at an end.
            break;
        }else if(MaxLtrIdx>=0){
            // MaxLtrIdx is an index into a list of letters, not a code for a
            // letter.  Therefore, we need to convert it back.
            word[c]=Letters[MaxLtrIdx].Letter;
        }
    }

    // Make sure we have a word terminator
    if(strchr(word,'+')==NULL) strcat(word,"+");

    // Get the length of the actual word.
    int  WordLen=strlen(word)-1;

    // Storage for the presented word and it's corresponding usage mask.
    char WordBuf[WORDSZEXTN];
    char MaskBuf[WORDSZEXTN];

    // Make sure we terminate our string properly.
    word[WORDSZ]=0;

    // Copy our word into the processing buffer and create the corresponding
    // mask for the word.  The '+' character (if it exists) is MASK_USED.
    memset(WordBuf,0,sizeof(WordBuf));
    strncpy(WordBuf,word,NumCharGPCR);
    memset(MaskBuf,0,sizeof(MaskBuf));
    for(int i=0;i<WORDSZEXTN;i++){
        if(i<NumCharGPCR){
            if(i<WordLen){
                MaskBuf[i]=MASK_SPACE;
            }else{
                MaskBuf[i]=MASK_USED;
            }
        }else{
            MaskBuf[i]=MASK_USED;
        }
    }
    MaskBuf[WORDSZEXTN-1]=0;

    // Check to see if the previous cycle caused a new letter to be shifted onto
    // the GPC Route incoming buffer.
    if(WordShifted){
        // If it has then clear out our rule list, GPC Route output phonmeme buffer.
        memset(MatchedRules,0,sizeof(MatchedRules));
        memset(GPCPhonemeBuffer,0,sizeof(GPCPhonemeBuffer));
        memset(GPCPhonemeMask,0,sizeof(GPCPhonemeMask));
        MatchedRulesIdx=0;

        DRC_ClearPartialSoln();

        // Run the GPCRoute again on the new input to find *ALL* solutions.
        DRC_GPCFind(WordBuf,MaskBuf,0,NumCharGPCR,NumCharGPCR>WordLen);
        // Then apply the output rules to all the solutions when we have .
        DRC_GPCApplyOutRules(NumCharGPCR>WordLen);

        // Flag that we've calculated the translation for this input buffer and
        // that more characters have to be shifted before we do the translation again.
        WordShifted=false;
    }

    // Now that we have all the results we can update the PB buffer from
    // our saved activations.
    DRC_UpdActFromGPCR(fh,cycle,word);

    // Now update the PB Layer output array.
    DRC_UpdPBFromGPC(fh,cycle,word);

    // Display the GPC Grapheme->Phoneme rules in the GPC translation.
    DRC_DspDRCTranslations(fh,cycle);

    // Display the GPC final activations for each phoneme.
    DRC_DspGPCActivations(fh,cycle);
}

//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
void DRC_UpdActFromGPCR(FILE* fh,int cycle,char* word)
{
    char RuleUsed[MAXINPUTBUF];


    // Just set the input array of activations to the letters in the word.
    // The letters in the word have been determined by searching the LetterLayer
    // array to determine the most activated letter in each column.
    int  WordLen=strlen(word);
    memset(GPC_InputActivation,0,sizeof(GPCPhonemeMask));
    for(int c=0;c<WordLen;c++){              // Include word terminator in activations.
        int  chr=word[c];                    // Get appropriate letter in word
        int  idx=IndexLetter[tolower(chr)];  // Get its index value chr=[a..z] idx=[0..25]
        GPC_InputActivation[c]=LetterLayer[c][idx];
    }


    // Have to calculate the activations for each solution. The activation of the
    // most activated letter in each column of the LL is averaged over each input
    // grapheme for the GPC rule and then distributed as is to all the output
    // phoneme activations for that rule.
    for(int s=0;s<(int)GPCSolns.size();s++){
        // Zero out this solutions activations.
        for(int a=0;a<MAXINPUTBUF;a++) GPCSolns[s].Activations[a]=0.0;

        // Mark all input characters as unused by a rule. The purpose of this array
        // is to mark which letters of the input word have been used in calculation
        // of the phoneme activations.
        for(int r=0;r<MAXINPUTBUF;r++) RuleUsed[r]=0;

        // Have to calculate the activations each cycle (because the input activations
        // will change each cycle) as the input letter level activations change.
        int chrposn=0;
        int phoposn=0;
        for(int r=0;r<MAXINPUTBUF;r++){
            // For each rule in the translation...
            t_gpcrule* rule=GPCSolns[s].Rules[r];
            if((rule!=NULL)&&(rule->Class!=clsOut)){

                // Add all the input activation for each simple field in the rule.
                DRC_Float TotalRuleAct=0.0;
                int       TotalRuleGraphemes=0;
                // Just searching for the first zero gives us the first unused character position.
                int       start=strlen(RuleUsed);
                for(int f=0;f<(int)strlen((const char*)rule->Fields);f++){
                    int Field=rule->Fields[f];
                    int PreContext=rule->PreContext;
                    // If the current field is a simple field then tally the
                    // corresponding input activation.
                    if((Field>=FLD_SIMPLEMIN)&&(Field<=FLD_SIMPLEMAX)&&(rule->Class!=clsOut)){
                        TotalRuleAct += GPC_InputActivation[start-PreContext+f];
                        RuleUsed[start-PreContext+f]='0'+r;
                        TotalRuleGraphemes++;
                        chrposn++;
                    }
                }

                // And apply the average activation to each output phoneme saving
                // the activations back in the Solutions array.
                DRC_Float Activation=TotalRuleAct/(DRC_Float)TotalRuleGraphemes;
                if(strcmp((const char*)rule->Phonemes,"*")!=0){
                    // Don't match any rule that has a NULL("*") output.
                    for(int p=0;p<(int)strlen((const char*)rule->Phonemes);p++){
                        if(rule->Class!=clsOut){
                            GPCSolns[s].Activations[phoposn]=Activation;
                            phoposn++;
                        }
                    }
                }
            }
        }

        // If the word fragment for this word indicates a complete word then
        // add a word termination activation.
        if(strchr(GPCSolns[s].WordFrag,'+')!=NULL){
            // Activations are only useable up to the word size boundary,WORDSZ.
            if(phoposn<MAXINPUTBUF){
                // Process the end-of-word indicators activation too.
                //GPCSolns[s].Activations[phoposn]=GPCSolns[s].Activations[phoposn-1];
                GPCSolns[s].Activations[phoposn]=GPC_InputActivation[chrposn];
                phoposn++;
            }
        }
    }

    if((GPCSolns.size()==0)&&(LrgstPartSoln.WordFrag!=NULL)){
        // We got no solutions so just promote the partial solution activations.
        // Zero out this solutions activations.
        for(int a=0;a<MAXINPUTBUF;a++) LrgstPartSoln.Activations[a]=0.0;

        // Mark all input characters as unused by a rule. The purpose of this array
        // is to mark which letters of the input word have been used in calculation
        // of the phoneme activations.
        for(int r=0;r<MAXINPUTBUF;r++) RuleUsed[r]=0;

        // Have to calculate the activations each cycle (because the input activations
        // will change each cycle) as the input letter level activations change.
        int chrposn=0;
        int phoposn=0;
        for(int r=0;r<MAXINPUTBUF;r++){
            // For each rule in the translation...
            t_gpcrule* rule=LrgstPartSoln.Rules[r];
            if(rule!=NULL){

                // Add all the input activation for each simple field in the rule.
                DRC_Float TotalRuleAct=0.0;
                int       TotalRuleGraphemes=0;
                // Just searching for the first zero gives us the first unused character position.
                int       start=strlen(RuleUsed);
                for(int f=0;f<(int)strlen((const char*)rule->Fields);f++){
                    int Field=rule->Fields[f];
                    int PreContext=rule->PreContext;
                    // If the current field is a simple field then tally the
                    // corresponding input activation.
                    if((Field>=FLD_SIMPLEMIN)&&(Field<=FLD_SIMPLEMAX)&&(rule->Class!=clsOut)){
                        TotalRuleAct += GPC_InputActivation[start-PreContext+f];
                        RuleUsed[start-PreContext+f]='0'+r;
                        TotalRuleGraphemes++;
                        chrposn++;
                    }
                }

                // And apply the average activation to each output phoneme saving
                // the activations back in the Solutions array.
                DRC_Float Activation=TotalRuleAct/(DRC_Float)TotalRuleGraphemes;
                if(strcmp((const char*)rule->Phonemes,"*")!=0){
                    // Don't match any rule that has a NULL("*") output.
                    for(int p=0;p<(int)strlen((const char*)rule->Phonemes);p++){
                        if(rule->Class!=clsOut){
                            LrgstPartSoln.Activations[phoposn]=Activation;
                            phoposn++;
                        }
                    }
                }
            }
        }

        // If the word fragment for this word indicates a complete word then
        // add a word termination activation.
        if(strchr(LrgstPartSoln.WordFrag,'+')!=NULL){
            // Activations are only useable up to the word size boundary,WORDSZ.
            if(phoposn<MAXINPUTBUF){
                // Process the end-of-word indicators activation too.
                //GPCSolns[s].Activations[phoposn]=GPCSolns[s].Activations[phoposn-1];
                LrgstPartSoln.Activations[phoposn]=GPC_InputActivation[chrposn];
                phoposn++;
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
void DRC_UpdPBFromGPC(FILE* fh,int cycle,char* word)
{
    // Zero our totals variables.
    DRC_GPC_TotalAct=0.0;
    DRC_GPCR_TotalAct=0.0;
    // Zero our array.
    memset(GPC_OutputActivationTP1,0,sizeof(GPC_OutputActivationTP1));

    if(GPCSolns.size()>0){
        int fnbrs=FirstNonBodyRuleSoln();

        // Determine which solution (0,fnbrs) is the key solution (GPC not GPCR).
        int soln=0;
        if(fnbrs>0){
            soln=fnbrs;
        }

        // Determine the phoneme column where the solutions stop being the same.
        int TGPCR_TallyColumn=0;
        for(int r=0;r<MAXINPUTBUF;r++){
            // The rules differ when: we get to the end of one or the other
            // rule list or the corresponding rules are not the same.
            if(GPCSolns[0].Rules[r]==NULL) break;
            if(GPCSolns[fnbrs].Rules[r]==NULL) break;
            if(GPCSolns[0].Rules[r]!=GPCSolns[soln].Rules[r]) break;

            // Add up all the phonemes produced so far.
            TGPCR_TallyColumn += strlen((const char*)GPCSolns[0].Rules[r]->Phonemes);
        }

        // Copy the initial solutions activations over to the GPC_OutputActivationTP1
        // array that is used by the PB in determining its activations. If we have
        // a valid second solution then copy the remaining activations too. Also total
        // these activation for the TGPC and TGPCR totals.  Loop to the maximum of
        // the lengths of the two solutions so all phonemes accounted for.
        int MaxPhonFragSz=strlen(GPCSolns[0].PhonemeFrag);
        if(fnbrs>0) MaxPhonFragSz = max(MaxPhonFragSz,(int)strlen(GPCSolns[fnbrs].PhonemeFrag));
        for(int c=0;c<MaxPhonFragSz;c++){
            int Ph,PhonIdx;
            DRC_Float Activation;
            if(c<(int)strlen(GPCSolns[soln].PhonemeFrag)){
                // Get the phoneme refered to by the soln solution Phoneme string and column.
                Ph=GPCSolns[soln].PhonemeFrag[c];
                // Get the index for that phoneme.
                PhonIdx=IndexPhoneme[Ph];
                // Get the activation.
                Activation=GPCSolns[soln].Activations[c];

                // Save that in the output array for the PB layer.
                GPC_OutputActivationTP1[c][PhonIdx] = Activation;
                // Total it for the TGPC totals.
                DRC_GPC_TotalAct += Activation;
            }

            // We've reached the point where the GPCR activations should be included.
            // The GPCR solution is the zero solution (assuming the fnbrs solution exits).
            if((c>=TGPCR_TallyColumn)&&(fnbrs>0)){
                // Do the same for the second solution if it exists.
                Ph=GPCSolns[0].PhonemeFrag[c];
                PhonIdx=IndexPhoneme[Ph];
                Activation=GPCSolns[0].Activations[c];

                // And total the activations to the TGPCR total.
                DRC_GPCR_TotalAct += Activation;
            }
        }

    }else if(LrgstPartSoln.WordFrag!=NULL){
        // Copy the initial solutions activations over to the GPC_OutputActivationTP1
        // array that is used by the PB in determining its activations. If we have
        // a valid second solution then copy the remaining activations too. Also total
        // these activation for the TGPC and TGPCR totals.
        for(int c=0;c<WORDSZ;c++){
            // Get the phoneme refered to by the soln solution Phoneme string and column.
            int Ph=LrgstPartSoln.PhonemeFrag[c];
            // Get the index for that phoneme.
            int PhonIdx=IndexPhoneme[Ph];
            // Get the activation.
            DRC_Float Activation=LrgstPartSoln.Activations[c];

            // Save that in the output array for the PB layer.
            GPC_OutputActivationTP1[c][PhonIdx] = Activation;
            // Total it for the TGPC totals.
            DRC_GPC_TotalAct += Activation;
        }
    }

    // Create the GPCRoute output word.
    GPC_CreateWord();
}

//---------------------------------------------------------------------------
// Routine: Create a word for the GPC Route that is constructed of the most
//          active phonemes.
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
void GPC_CreateWord()
{
    // Clear our output word.
    memset(GPCWord,0,MAXINPUTBUF);

    // Find the phonemes in each column with the maximum activation.
    for(int c=0;c<WORDSZ;c++){
        DRC_Float MaxAct=0.0;
        for(int p=0;p<PHONEMES;p++){
            if(GPC_OutputActivationTP1[c][p]>MaxAct){
                // ...and string them into a word.
                MaxAct=GPC_OutputActivationTP1[c][p];
                GPCWord[c]=Phonemes[p].PhonemeCode;
            }
        }
        if(GPCWord[c]==' ')GPCWord[c]='\0';
    }
}

//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
void DRC_DspDRCTranslations(FILE* fh,int cycle)
{
    int Solutions=(int)GPCSolns.size();

    // NOTE: Negating this test will display *ALL* the matching rules.
    if((FLG_OutExtended)&&(Solutions>0)){

        // Display all the GPC Translations using Grapheme->Phoneme rules.
        for(int s=0;s<Solutions;s++){
            pfprintf(fh,"Cycle%d GPCRoute ",cycle);
            pfprintf(fh,"%.8s ",GPCSolns[s].WordFrag);

            // ... and all the rules that matched.
            for(int r=0;r<MAXINPUTBUF;r++){
                // Stop when we reach end of list.
                if(GPCSolns[s].Rules[r]==NULL) break;

                // Display the word position of the rule.
                pfprintf(fh,"(");
                switch(GPCSolns[s].Rules[r]->WrdPosn){
                    case wpAny: pfprintf(fh,"A"); break;
                    case wpBegin: pfprintf(fh,"b"); break;
                    case wpMedial: pfprintf(fh,"m"); break;
                    case wpEnd: pfprintf(fh,"e"); break;
                    default: break;
                }

                // Display the class of the rule.
                switch(GPCSolns[s].Rules[r]->Class){
                    case clsNone:  pfprintf(fh,":None"); break;
                    case clsBody:  pfprintf(fh,":Body"); break;
                    case clsOut:   pfprintf(fh,":Out"); break;
                    case clsMulti: pfprintf(fh,":Multi"); break;
                    case clsCS:    pfprintf(fh,":CS"); break;
                    case clsTwo:   pfprintf(fh,":Two"); break;
                    case clsMphon: pfprintf(fh,":Mphon"); break;
                    case clsSing:  pfprintf(fh,":Sing"); break;
                }
                pfprintf(fh,")");

                // Display the rule.
                pfprintf(fh,"%s->%s ",GPCSolns[s].Rules[r]->GraphemeContext,GPCSolns[s].Rules[r]->Phonemes);
            }
            pfprintf(fh,"\n");
        }

    }else if(Solutions>0){
        int s=0;
        pfprintf(fh,"Cycle%d GPCRoute ",cycle);
        pfprintf(fh,"%s ",GPCSolns[s].WordFrag);

        // ... and all the rules that matched.
        for(int r1=0;r1<MAXINPUTBUF;r1++){
            // Stop when we reach end of list.
            if(GPCSolns[s].Rules[r1]==NULL) break;

            // If this rule is a wpEnd (end of word) rule then place braces around
            // it and display the remainder of the second rule, otherwise, display
            // it without braces.
            int s2=FirstNonBodyRuleSoln();
            bool FLG_End=(GPCSolns[s].Rules[r1]->WrdPosn==wpEnd)&&(s2!=0);
            if(FLG_End) {
                pfprintf(fh,"{");
            }

            // Display the word position of the rule, the brackets and the rule itself.
            pfputc('(',fh);
            if(GPCSolns[s].Rules[r1]->Class==clsOut) pfprintf(fh,"out,");
            pfputc(STR_PosnIDs[GPCSolns[s].Rules[r1]->WrdPosn],fh);
            pfputc(')',fh);
            pfprintf(fh,"%s->%s",GPCSolns[s].Rules[r1]->GraphemeContext,GPCSolns[s].Rules[r1]->Phonemes);

            if(FLG_End) {
                pfprintf(fh,"} ");
                for(int r2=r1;r2<WORDSZ;r2++){
                    // Stop when we reach end of list.
                    if(GPCSolns[s2].Rules[r2]==NULL) break;

                    // Display the word position of the rule.
                    pfputc('(',fh);
                    if(GPCSolns[s2].Rules[r2]->Class==clsOut) pfprintf(fh,"out,");
                    pfputc(STR_PosnIDs[GPCSolns[s2].Rules[r2]->WrdPosn],fh);
                    pfputc(')',fh);

                    pfprintf(fh,"%s->%s ",GPCSolns[s2].Rules[r2]->GraphemeContext,GPCSolns[s2].Rules[r2]->Phonemes);
                }
                // Stop the display of the original solutions rules.
                r1=MAXINPUTBUF;
            }
            pfprintf(fh," ");
        }
        pfprintf(fh,"\n");

    }else if(LrgstPartSoln.WordFrag!=NULL){
        // We didn't get any solutions so display the largest partial match that
        // we found and treat this as a solution of sorts.
        pfprintf(fh,"Cycle%d GPCRoute ",cycle);
        pfprintf(fh,"%.8s ",LrgstPartSoln.WordFrag);

        // ... and all the rules that matched.
        for(int r1=0;r1<MAXINPUTBUF;r1++){
            // Stop when we reach end of list.
            if(LrgstPartSoln.Rules[r1]==NULL) break;

            // Display the word position of the rule, the brackets and the rule itself.
            pfputc('(',fh);
            pfputc(STR_PosnIDs[LrgstPartSoln.Rules[r1]->WrdPosn],fh);
            pfputc(')',fh);
            pfprintf(fh,"%s->%s ",LrgstPartSoln.Rules[r1]->GraphemeContext,LrgstPartSoln.Rules[r1]->Phonemes);
        }
        // Display the characters that didn't get matched.
        for(int c=0;c<(int)strlen(LrgstPartSoln.MaskFrag);c++){
            if(LrgstPartSoln.MaskFrag[c]==MASK_SPACE){
                pfputc(LrgstPartSoln.WordFrag[c],fh);
            }
        }
        pfprintf(fh,"->? \n");

    }
}

//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
void DRC_DspGPCActivations(FILE* fh,int cycle)
{
    char Phoneme;
    bool LastRule=false;

    if(GPCSolns.size()>0){
        int fnbrs=FirstNonBodyRuleSoln();  // Index into solution array.
        int dcRule=0;
        int dcPhoneme=0;

        // If there is a Non-Body-Rule solution then it is treated as the primary
        // solution (GPC) and the Body-Rule solution as the GPCR solution.  The
        // GPCR solution activations are not propagated into the PB.
        if(fnbrs>0){
            // Go through all the rules and print out the activations for the
            // phonemes for each rule.
            for(int r=0,c=0;r<WORDSZ;r++){
                // Stop when we get to the end of the rules list.
                if(GPCSolns[fnbrs].Rules[r]==NULL) break;
                if(GPCSolns[fnbrs].Rules[r]->Class==clsOut) break;

                // If we have two displayable solutions (both a BODY and a non-BODY
                // rule translation) then trigger the printing of the second translation.
                if((fnbrs>0)&&(dcRule==0)){
                    if(GPCSolns[fnbrs].Rules[r]!=GPCSolns[0].Rules[r]) {
                        dcRule=r;
                        dcPhoneme=c;
                    }
                }

                // Flag the last rule
                t_gpcrule* rule=GPCSolns[fnbrs].Rules[r+1];
                if((r<WORDSZ-1)&&((rule==NULL)||(rule->Class==clsOut))) LastRule=true;

                // For the last rule, which doesn't have a '+' termination (only
                // the solution PhonemeFrag has a '+' termination), make sure we
                // print one more column which will be the '+'.
                int NumPhonemes=strlen((const char*)GPCSolns[fnbrs].Rules[r]->Phonemes);
                if(LastRule&&(strchr(GPCSolns[fnbrs].PhonemeFrag,'+')!=NULL)) NumPhonemes++;

                // Run through all the phonemes in the ouput of this rule.
                for(int p=0;p<NumPhonemes;p++){
                    // Display the base solution activation for this column and solution fnbrs.
                    DRC_Float Activation=GPCSolns[fnbrs].Activations[c];
                    if(Activation>GP_DisplayThreshold){
                        Phoneme=GPCSolns[fnbrs].PhonemeFrag[c];
                        if(Phoneme=='*') Phoneme='+';

                        pfprintf(fh,"Cycle%d GPC%d %8.6f %c\n",
                            cycle,c,Activation,Phoneme);
                    }
                    c++;
                }

            }
        }

        // If there is no Non-Body-Rule solution then solution zero (0) is treated
        // as the GPC solution otherwise it is treated as the GPCR solution.
        for(int r=dcRule,c=dcPhoneme;r<WORDSZ;r++){
            // Stop when we get to the end of the rules list.
            if(GPCSolns[0].Rules[r]==NULL) break;
            if(GPCSolns[0].Rules[r]->Class==clsOut) break;

            // For the last rule, which doesn't have a '+' termination (only
            // the solution PhonemeFrag has a '+' termination), make sure we
            // print one more column which will be the '+'.
            int NumPhonemes=strlen((const char*)GPCSolns[0].Rules[r]->Phonemes);
            if((int)strlen(GPCSolns[0].PhonemeFrag)-c==NumPhonemes+1) NumPhonemes++;

            for(int p=0;p<NumPhonemes;p++){

                // Display the base solution activation for this column and solution 0.
                DRC_Float Activation=GPCSolns[0].Activations[c];
                if(Activation>GP_DisplayThreshold){
                    Phoneme=GPCSolns[0].PhonemeFrag[c];
                    if(Phoneme=='*') Phoneme='+';
                    pfprintf(fh,"Cycle%d GPC%s%d %8.6f %c\n",
                        cycle,(fnbrs>0)?"R":"",c,Activation,Phoneme);
                    c++;
                }
            }
        }

    }else if(LrgstPartSoln.WordFrag!=NULL){
        // We didn't find any solutions so display the activations for the partial match.

        // Copy the initial solutions activations over to the GPC_OutputActivationTP1
        // array that is used by the PB in determining its activations.
        for(int c=0;c<WORDSZ;c++){
            // Display the base solution activation for this column and solution 0.
            Phoneme=LrgstPartSoln.PhonemeFrag[c];
            if(Phoneme=='*') Phoneme='+';
            DRC_Float Activation=LrgstPartSoln.Activations[c];
            if(Activation>GP_DisplayThreshold){
                pfprintf(fh,"Cycle%d GPC%d %8.6f %c\n",
                    cycle,c,Activation,Phoneme);
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
int FirstNonBodyRuleSoln(void)
{
    bool Found=false;
    bool GotBody=false;
    int  rtn=-1;

    // Search through all the solutions and find the first one that
    // doesn't have a clsBody rule.
    for(int s=0;(s<(int)GPCSolns.size())&&(!Found);s++){
        GotBody=false;
        for(int r=0;(r<MAXINPUTBUF)&&(!Found);r++){
            t_gpcrule* rule=GPCSolns[s].Rules[r];
            if(rule==NULL) break;
            if(rule->Class==clsBody) {
                GotBody=true;
                break;
            }
        }
        if(!GotBody){
            rtn=s;
            Found=true;
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
void DRC_GPCRouteShift(int cycle,int& GPCRChars,bool& ChgFlag,FILE* fh)
{
    // Determine the column of the last GPC Route generated phoneme and
    // watch all the phonemes in that column for excursions past the
    // GPCCriticalPhonology threshold.
    if((GPCRChars==0)&&(cycle==(GPC_GPCOnset-1))){ // We are triggering for the next cycle.
        GPCRChars++;
        ChgFlag=true;
    }else if((GPCRChars>0)&&(GPCRChars<=WORDSZ)){
        // NOTE: This should deal with all solutions not just the first.
        int LastPhonemeCol=strlen(GPCSolns[0].PhonemeFrag)-1;

        // Check to see if the PB buffer column corresponding to the last
        // GPCPhonemeBuffer column filled with a phoneme has any phonemes
        // that have exceeded the GPC_GPCCriticalPhonology threshold.
        for(int p=0;p<PHONEMES;p++){
            if(PhonemeLayer[LastPhonemeCol][p]>=GPC_GPCCriticalPhonology){
                // ... if there are then shift another character onto the GPC
                // route input buffer.
                GPCRChars++;
                ChgFlag=true;

                // Clear out the solutions vector.
                for(int s=0;s<(int)GPCSolns.size();s++) {
                    if(GPCSolns[s].WordFrag!=NULL) free(GPCSolns[s].WordFrag);
                    if(GPCSolns[s].PhonemeFrag!=NULL) free(GPCSolns[s].PhonemeFrag);
                }
                GPCSolns.erase(GPCSolns.begin(),GPCSolns.end());
                break;
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
void SavePartSuccess(char* word,char* mask)
{
    // If we've matched anything then save the partial solution.
    if(MatchedRules[0]!=NULL){

        // Now that we've found a solution lets keep it around for some time
        // while we figure out what to do with it.
        if(LrgstPartSoln.WordFrag!=NULL) free(LrgstPartSoln.WordFrag);
        LrgstPartSoln.WordFrag=strdup(word);

        if(LrgstPartSoln.PhonemeFrag!=NULL) free(LrgstPartSoln.PhonemeFrag);
        LrgstPartSoln.PhonemeFrag=strdup(GPCPhonemeBuffer);

        if(LrgstPartSoln.MaskFrag!=NULL) free(LrgstPartSoln.MaskFrag);
        LrgstPartSoln.MaskFrag=strdup(mask);

        //if(word[strlen(word)-1]=='+') strcat(GPCPhonemeBuffer,"+");
        for(int c=0;c<MAXINPUTBUF;c++){
            LrgstPartSoln.Rules[c]=MatchedRules[c];
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
void SaveSuccess(char* word)
{
    // If we've matched anything then save the solution.
    if(MatchedRules[0]!=NULL){

        // Now that we've found a solution lets keep it around for some time
        // while we figure out what to do with it.
        t_solution soln;
        soln.WordFrag=strdup(word);
        if(word[strlen(word)-1]=='+') strcat(GPCPhonemeBuffer,"+");
        soln.PhonemeFrag=strdup(GPCPhonemeBuffer);
        soln.MaskFrag=strdup(GPCPhonemeMask);
        for(int c=0;c<MAXINPUTBUF;c++){
            soln.Rules[c]=MatchedRules[c];
        }
        GPCSolns.push_back(soln);
    }
}

//---------------------------------------------------------------------------
// Routine: CompletelyMatched - Report if the mask string has any unmatched
//          characters indicated (by MASK_SPACE).
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
bool CompletelyMatched(char* mask)
{
    bool rtn=true;
    for(int m=0;m<(int)strlen(mask);m++){
        if(mask[m]==MASK_SPACE) {
            rtn=false;
            break;
        }
    }
    return(rtn);
}


//---------------------------------------------------------------------------
// Routine: MergeMask - Merge a rule's mask into the word's mask to keep track
//          of all the word's chars that have been used in a GPC rule.
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
void MergeMask(char* mask,int start,t_gpcrule* rule)
{
    int PreContext=rule->PreContext;
    for(int c=0;c<(int)strlen(mask)-start+PreContext;c++){
        // Key to matching (word) mask with rule mask:
        // Word: MASK_USED       - Character already matched by simple fields in *some* rule.
        //       MASK_SPACE      - Character not matched by any simple field in any rule yet.
        // Rule: MASK_USED       - Simple field - Must match and consume a simple char in the word.
        //       MASK_SPACE      - Context field - Just match a char in the word.
        char RulMsk=rule->Mask[c];
        char WrdMsk=mask[start-PreContext+c];

        if((RulMsk==MASK_USED)&&(WrdMsk==MASK_USED)){
            // This is illegal because a rule doesn't match if the simple fields are already used.
            pfprintf(stderr,"ERROR(MergeMask): Word mask(%s) and field mask(%s) overlap.\n",mask,rule->Mask);
            exit(1);

        }else if((RulMsk==MASK_USED)&&(WrdMsk==MASK_SPACE)){
            // This is the normal case of a word char matching a rule's simple field.
            mask[start-PreContext+c]=MASK_USED; // Flag word char as used.

        }else if((RulMsk==MASK_SPACE)&&(WrdMsk==MASK_USED)){
            // This is as expected.  We can match context fields in rules to already used chars.

        }else if((RulMsk==MASK_SPACE)&&(WrdMsk==MASK_SPACE)){
            // This is OK too.  A context field can match an simple field char in the word.
        }
    }
}

//---------------------------------------------------------------------------
// Routine: UnMergeMask - Unmask the word's chars that are matched by this rule.
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
void UnMergeMask(char* mask,int start,t_gpcrule* rule)
{
    int PreContext=rule->PreContext;
    for(int c=0;c<(int)strlen(mask)-start+PreContext;c++){
        // Key to matching (word) mask with rule mask:
        // Word: MASK_USED       - Character already matched by simple fields in *some* rule.
        //       MASK_SPACE      - Character not matched by any simple field in any rule yet.
        // Rule: MASK_USED       - Simple field - Must match and consume a simple char in the word.
        //       MASK_SPACE      - Context field - Just match a char in the word.
        char RulMsk=rule->Mask[c];
        char WrdMsk=mask[start-PreContext+c];

        if((RulMsk==MASK_USED)&&(WrdMsk==MASK_USED)){
        //if((RulMsk!=MASK_SPACE)&&(mask[start+c]!=MASK_SPACE)){
            // This word's field was used by this rule, so unmask it now.  A word's
            // char can only be used by one rules non-context fields.
            mask[start-PreContext+c]=MASK_SPACE;

        }else if((RulMsk==MASK_USED)&&(WrdMsk==MASK_SPACE)){
            // This shouldn't happen if we assume this rule masked the word in the first place.
            pfprintf(stderr,"ERROR(UnMergeMask): Word's field unmasked (%s) even though rule (%s) masked.\n",
                mask,rule->Mask);
            exit(1);

        }else if((RulMsk==MASK_SPACE)&&(WrdMsk==MASK_USED)){
        //}else if((RulMsk==MASK_SPACE)&&(mask[start+c]!=MASK_SPACE)){
            // This is as expected.  This rule didn't have mask this char from the word.

        }else if((RulMsk==MASK_SPACE)&&(WrdMsk==MASK_SPACE)){
        //}else if((RulMsk==MASK_SPACE)&&(mask[start+c]==MASK_SPACE)){
            // This is OK too.  This rule didn't mask this char from the word and the
            // word isn't masked here either.
        }
    }
}



//---------------------------------------------------------------------------
// Routine: Return length(in fields) of the rule if it matches.  Otherwise,
//          return 0 for failure.
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
bool FieldMatch(char* word,char* mask,t_gpcrule* rule,int start,int end,int* LetPhoIdx)
{
    int len,flds;
    bool rtn=true;  // Rule fields did match.
    int PreContext=rule->PreContext;
    // Get the number of fields in the rule.
    len=strlen((const char*)rule->Fields);
    flds=len;
    //if(rule->Fields[len-1]==FLD_ENDOFWORD) len--;
    // Assume rule matches
    rtn=len;

    // Make sure we have enough characters in front of the current simple field
    // to match the Pre-Context (context fields before the first simple field).
    // We also want to make sure the remaining part of the word to match is
    // greater than or equal to the length of the Field string (because you can't
    // match characters, beyond the end of the string, that don't exist).
    if((PreContext <= start)&&(end >= (start-PreContext+len))){
        for(int i=0;(i<flds)&&(rtn!=0);i++){
            BYTE Field=rule->Fields[i];
            int  chr=word[start-PreContext+i];   // Get appropriate letter in word
            int  idx=LetPhoIdx[chr];             // Get its index value

            // If the character we are matching against is zero then exit
            if(chr=='\0'){
                // If we've gotten to the end of the buffer also allow a match with End-of-Word.
                if((start-PreContext+len!=WORDSZ)||(end>WORDSZ)){
                    // Otherwise, we aren't trying to match an End-of-Word field,
                    // or our word isn't to the buffer boundary, or we haven't shifted
                    // more characters into the buffer than are allowed(WORDSZ).
                    rtn=false;
                }

            }else if(chr=='+'){
                // We never match the end-of-word character (handled elsewhere).
                rtn=false;

            }else if(Field==FLD_GRAPHVOWELS){
                // If we got a field indicating a Vowel is acceptable...
                if(!GPCFieldStore[FLD_GRAPHVOWELS*MAXFLDCHAR+idx]){
                    // If the field indicates vowel but the word doesn't have a Vowel, fail.
                    rtn=false;
                }

            }else if(Field==FLD_GRAPHCONSONANTS){
                // If we got a field indicating a Consonant is acceptable...
                if(!GPCFieldStore[FLD_GRAPHCONSONANTS*MAXFLDCHAR+idx]){
                    // Fail if the word's char isn't what is defined as a Consonant (letters file).
                    rtn=false;
                }

            }else if((Field>=FLD_USERMIN)&&(Field<=FLD_USERMAX)){
                // If we got a user(gpcrules file) defined field, then...
                if(!GPCFieldStore[(Field-FLD_USERMIN)*MAXFLDCHAR+idx]){
                    // Fail only if the word's char isn't in the defined list.
                    rtn=false;
                }

            }else if(chr=='+'){
                // No alphabetic characters will match the End-of-Word marker '+'.
                rtn=false;

            }else if((Field>=FLD_SIMPLEMIN)&&(Field<=FLD_SIMPLEMAX)){
                // If we got a simple field, and ...
                if((idx+1)!=Field){
                    // If the word's char doesn't match the field, fail.
                    rtn=false;
                }else if(mask[start-PreContext+i]==MASK_USED){
                    // Or, if the mask indicates this field is already used, fail.
                    rtn=false;
                }
            }
        }
    }else{
        // Either there are more characters before current position in word (start)
        // or the rule is too large for the word.
        rtn=false;
    }

    // If we got a match then keep track of the phoneme and mask outputs.
    if(rtn){
        int posn=strlen(GPCPhonemeBuffer);
        // The rule matched so lets set the activation of the outgoing
        // phonemes at this level for the phonemes generated by this rule.
        strcat(GPCPhonemeBuffer,(const char*)rule->Phonemes);    // Concatenate on phonme string for current rule.
        for(int i=0;i<(int)strlen((const char*)rule->Phonemes);i++){
            // Some phonemes are protected from output rules.
            if((rule->Protected)||(rule->Class==clsBody)){
                GPCPhonemeMask[posn+i]=MASK_PROTECTED;  // Indicate phoneme word is protected here.
            }else{
                GPCPhonemeMask[posn+i]=MASK_USED;       // Indicate phoneme word is *NOT* protected here.
            }
        }
    }

    return(rtn);
}

//---------------------------------------------------------------------------
// Routine: FindStart - Find the first non-matched letter in the string by
//          checking for the index of the first MASK_SPACE character in the mask.
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
int FindStart(char* mask)
{
    int rtn=MAXINPUTBUF;
    for(int i=0;i<MAXINPUTBUF;i++){
        if(mask[i]==MASK_SPACE){
            rtn=i;
            break;
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
int NumChr(char* mask,char Special)
{
    int rtn=0;
    for(int c=0;c<(int)strlen(mask);c++){
        if(mask[c]==Special) rtn++;
    }
    return(rtn);
}


//---------------------------------------------------------------------------
// Routine: DRC_GPCFind - This routine recursively will find a match of the
//          input 'word' to some set of GPCRules in the GPCRules array.  The
//          calling routine will pass a pointer to the word with the assumption
//          that the pointer will always point to the beginning of the buffer
//          and the 'start' and 'end' indicies will indicate the portion of
//          the word that will be matched with a GPC rule.  If the last letter
//          in the 'word' is a '+' (end of word indicator) then the software
//          will search wpEnd position rules to match the end.  If the 'start'
//          value is zero then the search will search wpBegin position rules
//          to match the beginning of the word.  If the search through the
//          GPCRules array succeeds (found a matching rule) then the routine will
//          iterate with the remaining portion of the input 'word'.  Efficiencies
//          can be implemented when we know that there is no empty wpEnd rule so
//          if start==end then this is a failure and not a success.  On failure
//          to find a corresponding rule the routine will return a failure indication
//          to the calling routine.  If this calling routine is another instance
//          of this routine then the search will continue from the current location
//          for another matching rule.  Only when a wpEnd rule matches and
//          start==end will this be considered a successful search.  This rule
//          will ultimately be the one that is displayed.
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
void DRC_GPCFind(char* word,char* mask,int start, int end, bool GotTerm)
{
    eWrdPosn Type1,Type2,Type3;
    //int SpacesBeforeMask;
    //int SpacesAfterMask;
    //bool MatchedEnd;

    //pfprintf(stdout,"DRC_GPCFind c=%d w='%.8s' m='%.8s' s=%d e=%d\n",cycle,word,mask,start,end);

    // The reason for assigning the conditions to variables Type1,Type2 and Type3
    // is because we will end up modify the search parameters mid-way through the
    // search if we get an ending match that can be replaced with a number of
    // smaller matches. For instance:
    //   "Cycle73 GPCRoute snooze+ (A)s->s (A)n->n {(e)ooze->uz} (A)oo.e->u (A)z->z"
    if(start==0){
        Type1=wpBegin;
        Type2=wpAny;
        Type3=wpAny;
    }else{
        Type1=wpMedial;
        Type2=wpAny;
        Type3=wpEnd;
    }

    // Save the number of phonemes in out output buffer currently.
    int CurrPhoneme=strlen(GPCPhonemeBuffer);

    // Get the size of the word itself
    int  WordSize=min(strlen(word),WORDSZ);
    if(strchr(word,'+')!=NULL) WordSize--;

    // The rules are tested in the GPC Route in order Body,Multi,CS,Two,Mphon, and Sing.
    t_gpcrule* rule=GPCList_BodyStart;
    for(;rule!=NULL;rule=GPC_NextRule(rule)){
        // Don't use the Output (Phoneme->Phoneme) rules.
        if(rule->Class==clsOut) continue;

        eWrdPosn WrdPosn=rule->WrdPosn;
        // If we are just starting to match this word then we can use 'beginning'(b)
        // or 'any'(A) rules.  However, if we are not at the beginning then we must
        // use 'medial'(m), 'end'(e) or 'any'(A) rules.


        if((WrdPosn==Type1)||(WrdPosn==Type2)||(WrdPosn==Type3)){
            // Ignore any rules that have context before the first simple letter field
            // because we need to match simple letters at the beginning (this is a
            // clsStart matching proceedure).
            if(rule->PreContext>start) continue;


            // Check to make sure rule fits and matches front of word.
            if(FieldMatch(word,mask,rule,start,end,IndexLetter)>0){

                // Log the matched rule and clear out the rest of the list
                // We do it here where we know the actual size of the list.
                MatchedRules[MatchedRulesIdx]=rule;
                for(int r=MatchedRulesIdx+1;r<MAXINPUTBUF;r++){
                    MatchedRules[r]=NULL;
                }

                // A rule matched at the end of the word if the word end has been
                // found (GotTerm[inal]) and either the last letter of the word was
                // matched by either a simple field or a context field, OR the last
                // unmatched letter of the word was matched.

                // Following description is old ================================
                // wpEnd rules will only match under these circumstances.  First that
                // we have the end of the word presented to us (GotTerm[inal]) and
                // then the rule must match the last available grapheme in the word
                // and it must match (both simple and context sensitive) fields to
                // the end of the word.
                //SpacesBeforeMask=NumChr(mask,MASK_SPACE);
                MergeMask(mask,start,rule);
                //SpacesAfterMask=NumChr(mask,MASK_SPACE);
                //MatchedEnd=GotTerm&&
                ////           (start-rule->PreContext+(int)strlen(rule->Fields)==WordSize);
                //           ((start-rule->PreContext+(int)strlen(rule->Fields)==WordSize)||
                //            ((SpacesBeforeMask>=1)&&(SpacesAfterMask==0)));

                int NumFlds=(int)strlen((const char*)rule->Fields);
                bool MatchEnd = (start-rule->PreContext+NumFlds==WordSize)&&(GotTerm);
                int LastField =rule->Fields[NumFlds-1];
                bool LF_Simple = (LastField>=FLD_SIMPLEMIN)&&(LastField<=FLD_SIMPLEMAX);
                bool MatchStart = (start==0);


                if((WrdPosn==wpAny) ||
                   ((WrdPosn==wpEnd)&&(MatchEnd)) ||
                   ((WrdPosn==wpMedial)&&(!MatchStart)&&(!MatchEnd || !LF_Simple)) ||
                   ((WrdPosn==wpBegin)&&(MatchStart)) ) {

                // Make sure that if we have an EOW marker('+'), we are matching
                // the last real character (a-z) in the word with a wpEnd or wpAny rule,
                // or if the word only has one character then any rule will do.
                //if(((!MatchedEnd)&&(WrdPosn!=wpEnd)) ||
                //   (MatchedEnd&&((WrdPosn==wpEnd)||(WrdPosn==wpAny)||(WordSize==1)))){
                    // If the word is now completely matched ...
                    if(CompletelyMatched(mask)){
                            // We've found a second solution so display the compound
                            // solution.
                            SaveSuccess(word);

                    }else{
                        // The match didn't use up the whole word: make sure we didn't match
                        // an wpEnd rule then.  If we did then this isn't a match at all
                        //if(WrdPosn!=wpEnd){

                            // Make sure we save the largest(greatest number of word
                            // characters matched) solution in case we find no full solutions.
                            int UnMatchChr=NumChr(mask,MASK_SPACE);
                            if(UnMatchChr < LrgstPartUnMatchChars){
                                SavePartSuccess(word,mask);
                                LrgstPartUnMatchChars=UnMatchChr;
                            }

                            // Bump the index into the storage array when we recurse.
                            MatchedRulesIdx++;
                            int nextstart=FindStart(mask);
                            // We've not completely matched the word so continue the search.
                            DRC_GPCFind(word,mask,nextstart,end,GotTerm);
                            // Decrement the index into the storage array when we return.
                            MatchedRulesIdx--;
                        //}else{
                            //pfprintf(stdout,"DRC_GPCFind Matched non-end position with wpEnd rule.\n");
                        //}
                    }
                }

                // Remove the current rule's mask from the word mask.
                UnMergeMask(mask,start,rule);
                // We've finished matching now (and triggered saving of a solution), so we can
                // now eliminate any phonemes matched by rules posterior to this level. We should
                // also clean up any activations for those phonemes.
                GPCPhonemeBuffer[CurrPhoneme]='\0';
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
void DRC_GPCApplyOutRules(bool GotTerm)
{
    const char* DummyMask="CCCCCCCCCXXXXXXXX";
    int   Solutions=(int)GPCSolns.size();

    // Look through all the solutions in the vector and try and apply each of the
    // output rules.
    for(int soln=0;soln<Solutions;soln++){
        // Grab the pointer to the rules list for this solution and the phoneme string.
        t_gpcrule** RuleList=GPCSolns[soln].Rules;
        BYTE* Phonemes=(BYTE*)GPCSolns[soln].PhonemeFrag;
        char* Mask    =GPCSolns[soln].MaskFrag;

        // Get the size of the word itself
        int  WordSize=min(strlen((const char*)Phonemes),WORDSZ);
        if(strchr((char*)Phonemes,'+')!=NULL) WordSize--;

        // Find the next empty slot in the rule list for the output rule(s).
        int NextRule=0;
        for(;(NextRule<MAXINPUTBUF)&&(RuleList[NextRule]!=NULL);NextRule++);

        // Go through the entire set of output rules and check to see if they
        // can be applied anywhere in the output phoneme word.
        // The boundaries on this for-loop depend on the order of the GPCRules section.
        t_gpcrule* rule=GPCList_OutputStart;
        for(;rule!=NULL;rule=GPC_NextRule(rule)){
            // Check the ouput rule GraphemeContext against all positions in
            // phoneme string that was the translation of our word.
            for(int start=0;start<WORDSZ;start++){
                // No point trying to match beyond the end of the phoneme word.
                if(start>=(int)strlen((const char*)Phonemes)) break;

                if(FieldMatch((char*)Phonemes,DummyMask,rule,start,9,IndexPhoneme)>0){
                    // If this is an end rule then we must have matched the end.
                    bool MatchedEnd=GotTerm&&(start-rule->PreContext+(int)strlen((const char*)rule->Fields)==WordSize);
                    if((rule->WrdPosn==wpEnd)&&(!MatchedEnd)) continue;

                    if(Mask[start]!=MASK_PROTECTED){
                        // The output rule will only match at the start position with
                        // simple field because FieldMatch already takes care of offsetting
                        // the pre-context of the rule. Also we are assuming the
                        // replacement phoneme fragment is only one character.
                        Phonemes[start]=rule->Phonemes[0];

                        // Save the rule pointer in the next empty slot.
                        RuleList[NextRule++]=rule;
                    }
                }
            }
        }
    }
}











//==============================================================================
// Final processing section.
//==============================================================================


//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
bool DRC_TestOutput(int Cycle,FILE* fh)
{
    Enter("DRC_TestOutput");

    // Max Iteration = 3*WORDSZ+WORDSZ*PHONEMES = 384
    //               = WORDSZ*(3+PHONEMES) = 8*(48)

    int ActIdx[WORDSZ];

    // Not finished unless we get a string of phonemes starting from LHS and
    // proceeding uninterupted to blank character.  All these characters must
    // have reached, or exceeded, the GP_ReadingAloudCrit parameter.
    bool finished=false;

    // Get index value of a blank (word terminator) to detect end.
    int blank=IndexPhoneme[' '];

    // 'Zero' out the index buffer for the phonemes.
    for(int c=0; c<WORDSZ; c++) ActIdx[c]=blank;

    // Go through entire phoneme buffer
    for(int c=0; c<WORDSZ; c++){
        // Find our maximum activation for this column and save it's index value.
        DRC_Float ActMax=0.0;
        int       AMIdx=-1;
        for(int L1=0; L1<PHONEMES; L1++){
            if(PhonemeLayer[c][L1]>ActMax){
                ActMax=PhonemeLayer[c][L1];
                AMIdx=L1;
                ActIdx[c]=L1;
            }
        }

        if(ActMax<GP_ReadingAloudCrit){
            // If we are still checking we haven't reached the end of the phoneme
            // word and we have just found a character without the threshold activation
            // and thus we need to cycle again.  Therefore, break out of loop.
            if(FLG_OutExtended) pfprintf(fh,"  ActMax<GP_ReadingAloudCrit\n");
            break;

        }else if(AMIdx==blank){
            // If we have gotten to the threshold then check to see if we are
            // at a word terminator.  If so flag our success and break out.
            finished=true;
            break;

        }else{
            // We have reached our threshold for this phoneme but it isn't a
            // word terminator (blank) so continue checking other phonemes.
        }
    }

    if(FLG_OutExtended) pfprintf(fh,"Cycle=%4d\n",Cycle);

    IfDebug {
        pfprintf(fh,"=============================================================================\n");
        pfprintf(fh,"=============================================================================\n");
        pfprintf(fh,"Cycle=%4d '",Cycle);
        for(int c=0; c<WORDSZ; c++){
            if(ActIdx[c]==-1){
                pfprintf(fh,"-");
            }else{
                pfprintf(fh,"%c",Phonemes[ActIdx[c]].PhonemeCode);
            }
        }

        pfprintf(fh,"'");
        for(int c=0; c<WORDSZ; c++){
            pfprintf(fh," %6.4f",PhonemeLayer[c][ActIdx[c]]);
        }
        pfprintf(fh,"\n");
        pfprintf(fh,"=============================================================================\n");
        pfprintf(fh,"=============================================================================\n");
    }

    Exit;
    return(finished);
};

//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
void DRC_FinalReport(FILE* fh)
{
    Enter("DRC_FinalReport");

    Exit;
};

//---------------------------------------------------------------------------
// Routine:
// Input:
// Output:
// SideEffects:
// Errors:
//---------------------------------------------------------------------------
void DRC_Cleanup(FILE* fh)
{
    Enter("DRC_Cleanup");

    Exit;
};




//---------------------------------------------------------------------------
// End of $RCSfile: DRC_Core.cpp,v $
//---------------------------------------------------------------------------

