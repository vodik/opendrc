VERSION = $(shell git describe --tags)

CXXFLAGS := -std=c++11 \
	-Wall -Wextra -pedantic \
	-Winit-self \
	-Wshadow \
	-Wformat=2 \
	-Wmissing-declarations \
	-Wstrict-overflow=5 \
	-Wcast-align \
	-Wcast-qual \
	-Wconversion \
	-Wunused-macros \
	-Wwrite-strings \
	-DDRC_VERSION=\"${VERSION}\"

SRC = ${wildcard *.cpp}
OBJ = ${SRC:.cpp=.o}

drc: ${OBJ}
	${CXX} ${CXXFLAGS} ${LDFLAGS} ${OBJ} ${LDLIBS} -o $@

clean:
	${RM} drc ${OBJ}

.PHONY: clean install uninstall
