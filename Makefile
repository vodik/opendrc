VERSION = $(shell git describe --tags)

CXXFLAGS := -std=c++11 \
	-Wall -Wextra -pedant \
	-DDRC_VERSION=\"${VERSION}\" \

SRC = ${wildcard *.cpp}
OBJ = ${SRC:.cpp=.o}

drc: ${OBJ}
	${CXX} ${CXXFLAGS} ${LDFLAGS} ${OBJ} ${LDLIBS} -o $@

clean:
	${RM} drc ${OBJ}

.PHONY: clean install uninstall
