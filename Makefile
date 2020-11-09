# the compiler: gcc for C program, define as g++ for C++
CC = g++

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CXXFLAGS= -std=c++11 -g -Wall

OBJECTS = main.o Simulation.o
HEADERS := $(shell find . -path ./test -prune -o -name "*.hpp" -print)

main: main.o $(OBJECTS)
	$(CC) $(CXXFLAGS) -o $@ $^

$(OBJECTS): $(HEADERS)




clean:
	$(RM) *.o *.gch core main 