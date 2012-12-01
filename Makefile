CCXFLAGS := -std=c++11 -Wall -Wextra -pedant

SRC = ${wildcard *.cpp}
OBJ = ${SRC:.cpp=.o}

drc: ${OBJ}
	${CXX} ${CXXFLAGS} ${LDFLAGS} ${OBJ} ${LDLIBS} -o $@

clean:
	${RM} drc ${OBJ}

.PHONY: clean install uninstall

# all : DRCTest OpenD

# DRCTest : $(DRCTestObj
# 	g++ -g3 $(DRCTestObjs) -o DRCTe

# DRCTest.o : DRCTest_hdrstop.h DRCTest
# 	g++ -c -g3 DRCTest.c


# OpenDRC : $(OpenDRCObj
# 	g++ -g3 $(OpenDRCObjs) -o OpenD

# $(OpenDRCObjs) : DRC.h DRC_CmdLine.h DRC_Core.h DRC_FileIO.h Batch_y.h
# 		         DRC_Main.p DRC_CmdLine.p DRC_Core.p DRC_FileIO

# Batch_l.cpp : Batch
# 	flex -PBatch -i -l -L -oBatch_l.c Batch
# 	sed "s/Batch_l.c/Batch_l.cpp/g" <Batch_l.c 
# 	sed "s/yytext_ptr/Batch_text_ptr/g" <a >Batch_l.c
# #	sed "s/isatty(/std::isatty(/g" <b >Batch_l.c
# 	-rm Batch_l.c 

# Batch_y.cpp : Batch
# 	bison -pBatch -v -d -l --debug -oBatch_y.c Batch.
# 	sed "s/Batch_y.c/Batch_y.cpp/g" <Batch_y.c >Batch_y.c
# 	-rm Batch_y

# Generic_l.cpp : Generic
# 	flex -PGeneric -i -l -L -oGeneric_l.c Generic
# 	sed "s/Generic_l.c/Generic_l.cpp/g" <Generic_l.c 
# 	sed "s/yytext_ptr/Generic_text_ptr/g" <a >Generic_l.c
# #	sed "s/isatty(/std::isatty(/g" <b >Generic_l.c
# 	-rm Generic_l.c 

# .PHONY : cle
# clean
# 	-rm $(DRCTestObjs) $(OpenDRCObj

