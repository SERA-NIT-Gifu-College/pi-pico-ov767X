CC:=gcc
CXX:=g++
CFLAGS:=-g -O2 -Wall -Wpedantic
CXXFLAGS:=-g -O2 -Wall -Wpedantic

SRCS:=client.cpp server.cpp
TGTS:=client server

all: ${TGTS}

%: %.cpp
	${CXX} ${CXXFLAGS} ${shell pkg-config --libs --cflags raylib} $^ -o $@
