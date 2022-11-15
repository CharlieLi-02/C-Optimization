CXXFLAGS = -g -std=c++17 -Wall -Wextra -Werror -Wno-unused-parameter
CXX      = g++

# These targets don't create any files:
.PHONY: clean

# Magic Variables!
# https://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html
#  $@ is the name of the target
#  $+ is a list of all the dependencies
#  $< is the first dependency

Dictionary: main.o Dictionary.o Point.o Heap.o
	${CXX} $(CXXFLAGS) -o $@ $+ -g


main.o: main.cpp Dictionary.h Heap.h Point.h
	${CXX} $(CXXFLAGS) -c -o $@ $< -g

Dictionary.o: Dictionary.cpp Dictionary.h Heap.h Point.h
	${CXX} $(CXXFLAGS) -c -o $@ $< -g 

Point.o: Point.cpp Point.h
	${CXX} $(CXXFLAGS) -c -o $@ $< -g 

Heap.o: Heap.cpp Heap.h
	${CXX} $(CXXFLAGS) -c -o $@ $< -g


clean:
	rm -f Dictionary main.o Dictionary.o Point.o Heap.o
