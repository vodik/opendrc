Feb 18,2011

    Any scholarly or other publications that rely on the software whether in its original 
	form or modified by others, should cite the work as:

	Angold, Alan (2011). OpenDRC (Version 1.01)[Computer Software]. Waterloo, Canada. 
		Retrieved from https://sourceforge.net/projects/opendrc/


Contents:
	Building the projects (OpenDRC and DRCTest).
	Input and Output files.
	Program Parameters.
	Running the programs.
	DRCTest Program.	
	Contact Info.

Building the projects (OpenDRC and DRCTest):
	Windows/Borland C:
		[ A free command line version of Borland C++Builder v6.0 that was used to develop this 
		  project is available at: https://downloads.embarcadero.com/free/c_builder ]

        These are the basic rules to MAKE the OpenDRC and DRCTest projects on Windows XP.
    
    	These projects have been developed with Borland's C++Builder v6 and as such
    	are best recompiled with that platform.  Just open the OpenDRC.bpg file in
    	C++Builder.  No other compilers have been used to compile this program.
    	
    	However, C++Builder will generate .mak files to use with the command line. 
    	The two make files are 'OpenDRC.mak', for the DRC model, and 'DRCTest.mak'
    	for the supporting test program.  To make the projects from the command line
    	just issue the commands (Windows XP):
    
    		set MAKEDIR="c:\Program Files\Borland\CBuilder6\Bin\"
    
        The actual directory may vary depending on where your Borland directory resides.
        Then to compile the projects:
    
    		make -fOpenDRC
    	
    	and
    
    	    make -fDRCTest
    	
    	There should be just three warnings for OpenDRC:
    
    		[C++ Warning] debug.h(58): W8058 Cannot create pre-compiled header: 
    			initialized data in header
    		[C++ Warning] Batch_y.cpp(1749): W8008 Condition is always false
    		[C++ Warning] Batch_y.cpp(1750): W8066 Unreachable code
    
    	For DRCTest there are no warnings.
    
    	In the event that the .MAK files become out of synch with the .BPR files these
    	can be regenerated using Borland's toolset as follows (with the MAKEDIR as above):
    
    		bpr2mak OpenDRC.bpr		and
    		bpr2mak DRCTest.bpr
	
	Windows/Cygwin:
		[ Cygwin is a free linux like environment for Windows: http://www.cygwin.com/ ]

		There is now (v1.01) a supplied make file that will allow you to compile
		the OpenDRC and DRCTest programs by typing the "make" command to the 
		Cygwin command line.

		The make file is rather rudimentary at the moment but works fine.

Input and Output files:

	Unlike Coltheart, et. al. (2001)'s DRC1.2 all the language files have to be in the
	current directory to be found by OpenDRC.  These files are:

		default.parameters
		gpcrules
		letters
		phonemes
		properties
		vocabulary

	These files may be obtained from (they come as part of the model):

		http://www.maccs.mq.edu.au/~ssaunder/DRC/

    When you unpack the ZIP file just extract these files to the OpenDRC directory.
	
	The output RT, STATS and ACTS files should appear identical to the DRC1.2 model's
	output files with the exception of the header information that is specific to
	the OpenDRC model itself.

Program Parameters:
	
	The OpenDRC parameters are derived from the DRC1.2 parameters and can by seen
	by issuing the command:

		OpenDRC -h

	As can be seen in the output of this command only the core functionality if DRC1.2
	has been implemented, however, there are a few new functions as well.  The core
	functionality will allow the use of the model for testing run times (RT) and 
	activations to use like those of DRC1.2.  Priming, masking and noise has not been
	implemented yet.

	Similarly DRCTest, a program to compare RT, STATS and ACTS files from two runs 
	of either OpenDRC or DRC1.2, will describe its parameters with the command:

		DRCTest -h

Running the programs:

	A typical test run of OpenDRC during the development of the program was:

		OpenDRC -m 0.0000001 -a -b <batch-file>

	Where <batch-file> is just a text file of words/non-words (one per line).  This will
	produce a directory named (something like) <batch-file>.drc on the first run and other
	directories such as <batch-file>-#.drc on subsequent runs (where '#' is some number).
	Within this output directory will be the <batch-file>.RT and <batch-file>.STATS files
	which will summarize the run performance.  There will also be a number of <name>.ACTS
	files (where <name> is each word/non-word from the <batch-file>).

	Once two of these runs have been completed (either OpenDRC and DRC1.2 or two runs 
	of the same program) they may be compared with the DRCTest program.  The easiest
	way to do this is to move into the 'base' directory and issue the command:

		DRCTest <destination-directory>

	This will compare the files in the current directory and those in <destination-directory>.
	During the testing of OpenDRC a single run of DRC1.2 was generated and this became
	the 'base' directory to test against.  OpenDRC was then run and generated its own
	files that could be compared to the 'base' run.  DRCTest will generate the DRCTest.txt
	and DRCTotals.txt files in the 'base' directory.


DRCTest Program:

	This program is designed (and used in OpenDRC development) to highlight the difference 
	between two runs of either program (OpenDRC or DRC1.2) on the same input word or batch 
	word lists.  Its advantage is significant when testing the DRC vocabulary and other 
	such large batches of test words/non-words.  DRCTest can read all ~8000 files generated 
	by OpenDRC (and DRC1.2) in a few minutes and will generate two report files: DRCTest.txt
	and DRCTotals.txt.

	The DRCTest.txt file will highlight any differences between the output files of two 
	runs of the OpenDRC or DRC1.2 programs.  An example of this file (taken from the final
	test run of OpenDRC (and DRC1.2) on the DRC1.2/English1.1.6 vocabulary) is as follows:

		DRCTest comparison of directories:
		  Old: '.'
		  New: 'allwords-11.drc'
		  Error Threshold(A):          0.00000150
		  Error Totals Multiplier(B):        2.50
		  Error Totals Threshold(A*B): 0.00000375

		Processing RT files:
			Comparing:
				NEW: allwords-11.drc\allwords.RT (8031)
				OLD: .\allwords.RT (8031)
				---
				Old: 'erg 3g 73 NAMED CORRECT' (1921)
				New: 'erg 3g 72 NAMED CORRECT' (1921)
				---
				Old: 'pooh phu 101 NAMED CORRECT' (4665)
				New: 'pooh phu 100 NAMED CORRECT' (4665)
		Processing STATS files:
			Comparing:
				NEW: allwords-11.drc\allwords.stats (8)
				OLD: .\allwords.stats (7)
				---
				Old: '                   CORRECT                        WRONG' (1)
				Old: '           Named    Lowac    Total       Named    Lowac    Total' (2)
				Old: 'Count:      7999        0     7999          25        7       32' (3)
				Old: 'Mean:     75.939        -   75.939     103.840 1000.000  299.875' (4)
				Old: 'StdDev:   10.814        -   10.814      10.212    0.000  370.581' (5)
				Old: 'Total Count: 8031 Mean: 76.831 StdDev: 29.372' (6)
				New: '
		' (1)
				New: '                   CORRECT                       WRONG' (2)
				New: '           Named    Lowac    Total      Named    Lowac    Total' (3)
				New: 'Count:      7999        0     7999         25        7       32' (4)
				New: 'Mean:     75.938        -   75.938    103.840  1000.000  299.875' (5)
				New: 'StdDev:   10.814        -   10.814     10.212    0.000  370.581' (6)
				New: 'Total Count: 8031 Mean:  76.831 StdDev:  29.372' (7)
		Processing Parameter files:
			New: allwords-11.drc\parameters
			WARNING: New Param file cannot be found.
		Processing ACTS files:
			Comparing:
				NEW: allwords-11.drc\awls.acts (2105)
				OLD: .\awls.acts (2105)
				---
				Old: 'Cycle38 P0 # {'  #=0.053943 (827)
				New: 'Cycle38 P0 # {'  #=0.051246 (827)
				---
				Old: 'TPh Cycle38 #'  #=0.080606 (835)
				New: 'TPh Cycle38 #'  #=0.077909 (835)
				---
				Old: 'Cycle39 Phon # 9lz'  #=0.112794 (853)
				New: 'Cycle39 Phon # 9lz'  #=0.112871 (853)
				---
				Old: 'Cycle39 P0 # 9'  #=0.012059 (857)
				Old: 'Cycle39 P0 # {'  #=0.051237 (858)
				New: 'Cycle39 P0 # 9'  #=0.012138 (857)
				New: 'Cycle39 P0 # {'  #=0.048675 (858)
				...

	The numbers in ()'s are the line numbers from the original files.
	
	The first section describes the running parameters of the program.  The second section 
	compares the two RT files.  In this case, of the ~8000 vocabulary words, the words ERG 
	and POOH have been found to have a difference in their RT lines.  In this case the 
	RT Cycle numbers differ (73 vs 72 for ERG and 101 vs 100 for POOH).  The third section
	compares the two STATS files.  In this case the 'Mean:' line has a difference in the
	activations for the CORRECT entries.  The fourth section compares the parameter files.
	Since OpenDRC does not currently produce one of these files so this section states that.
	The last section compares each individual ACTS file pair.  In this case the word AWLS
	has been found to have different values in its ACTS file pair.  At cycle 38 the phonemic
	buffer has had an activation difference in the first phoneme (P0).  The activations are
	0.0053943 vs 0.051246. These activations would normally appear where the initial '#' 
	appears.  This format is a result of the coding of DRCTest and will likely change to
	present the ACTS file lines as they appear in the files.

	The DRCTotals.txt file lists all the final total lines in each ACTS file pair for each
	level of the model: FL, LL, OIL, POL, GPC, GPCR and PB.  These RT Cycle and activation
	numbers are presented in columnar format so they can be read into programs such as 
	Matlab and Excel.  The top few lines of this file (DRC1.2/English1.1.6 vocabulary) for
	a OpenDRC and DRC1.2 run is as follows (note there are 15 columns):

						Old       Old       Old       Old     Old       Old        Old        New       New       New       New     New       New        New    
		Filename        Cycles    TL        TO        TP      TGPC      TGPCR      TPh        Cycles    TL        TO        TP      TGPC      TGPCR      TPh    
		a.acts              83  7.918721  0.999773  0.941465  1.978057  0.000000  1.198325        83  7.918721  0.999773  0.941465  1.978057  0.000000  1.198325
		ace.acts            75  7.774066  0.995438  0.784286  2.908535  0.000000  1.330626        75  7.774065  0.995438  0.784286  2.908535  0.000000  1.330626
		ache.acts          104  7.975804  0.999948  0.892192  2.990202  0.000000  1.831712       104  7.975805  0.999948  0.892192  2.990202  0.000000  1.831713
		ached.acts         105  7.977427  0.999955  0.903696  3.987812  2.990859  2.590605       105  7.977427  0.999955  0.903696  3.987811  2.990859  2.590605
		aches.acts         115  7.989333  0.999989  0.859814  4.992800  0.000000  2.731116       115  7.989332  0.999989  0.859814  4.992799  0.000000  2.731116
		act.acts            65  7.589221  0.989569  0.799373  3.778371  0.000000  1.712995        65  7.589221  0.989569  0.799373  3.778371  0.000000  1.712994
		acts.acts           67  7.623394  0.990398  0.804004  4.745996  0.000000  2.142757        67  7.623394  0.990398  0.804004  4.745995  0.000000  2.142757
		ad.acts             66  7.630669  1.291871  0.805170  2.842752  0.000000  1.277251        66  7.630669  1.291872  0.805170  2.842752  0.000000  1.277251
		add.acts            65  7.627223  1.259994  0.804648  2.846117  0.000000  1.276476        65  7.627221  1.259994  0.804648  2.846117  0.000000  1.276476
		adds.acts           70  7.687630  0.992794  0.802572  3.831425  0.000000  1.779910        70  7.687630  0.992794  0.802572  3.831425  0.000000  1.779910


Contact Info:

	If you need help running these programs or other help with the OpenDRC project I can be 
	contacted at:

		Alan.Angold@gmail.com 
	
	or

		alanangold@alumni.uwaterloo.ca



