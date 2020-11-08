# Convex Polygon calculator Makefile.

# Defines the flags for compiling with C++.
CXXFLAGS = -Wall -std=c++11 -O2 -DNO_FREETYPE -I $(HOME)/libs/include 

# Rule to compile everything (make all).
all: main.exe

# Rule to clean object and executable files (make clean).
clean:
	rm -f main.exe *.o


main.exe: main.o Point.o Polygon.o
	$(CXX) $^ -o $@ -L $(HOME)/libs/lib -l PNGwriter -l png


# Dependencies between files.

main.o: main.cc Point.hh Polygon.hh Color.hh

Point.o: Point.cc Point.hh

Polygon.o: Polygon.cc Polygon.hh Point.hh Color.hh

