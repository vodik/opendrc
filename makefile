DRCTestObjs = DRCTest.o
OpenDRCObjs = DRC_Main.o DRC_CmdLine.o DRC_Core.o DRC_FileIO.o \
			  Batch_l.o Batch_y.o Generic_l.o

all : DRCTest OpenDRC

DRCTest : $(DRCTestObjs)
	g++ -g3 $(DRCTestObjs) -o DRCTest

DRCTest.o : DRCTest_hdrstop.h DRCTest.p
	g++ -c -g3 DRCTest.cpp


OpenDRC : $(OpenDRCObjs)
	g++ -g3 $(OpenDRCObjs) -o OpenDRC

$(OpenDRCObjs) : DRC.h DRC_CmdLine.h DRC_Core.h DRC_FileIO.h Batch_y.h \
		         DRC_Main.p DRC_CmdLine.p DRC_Core.p DRC_FileIO.p

Batch_l.cpp : Batch.l
	flex -PBatch -i -l -L -oBatch_l.c Batch.l
	sed "s/Batch_l.c/Batch_l.cpp/g" <Batch_l.c >a
	sed "s/yytext_ptr/Batch_text_ptr/g" <a >Batch_l.cpp
#	sed "s/isatty(/std::isatty(/g" <b >Batch_l.cpp
	-rm Batch_l.c a 

Batch_y.cpp : Batch.y
	bison -pBatch -v -d -l --debug -oBatch_y.c Batch.y 
	sed "s/Batch_y.c/Batch_y.cpp/g" <Batch_y.c >Batch_y.cpp
	-rm Batch_y.c

Generic_l.cpp : Generic.l
	flex -PGeneric -i -l -L -oGeneric_l.c Generic.l
	sed "s/Generic_l.c/Generic_l.cpp/g" <Generic_l.c >a
	sed "s/yytext_ptr/Generic_text_ptr/g" <a >Generic_l.cpp
#	sed "s/isatty(/std::isatty(/g" <b >Generic_l.cpp
	-rm Generic_l.c a 

.PHONY : clean
clean :
	-rm $(DRCTestObjs) $(OpenDRCObjs)

