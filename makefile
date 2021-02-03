INCS = cmdline.h
OBJS = main.o use_arguments.o expr.o tests.o
CXXFLAGS = --std=c++14 -O2
executable: $(OBJS) ; $(CXX) $(CXXFLAGS) -o executable $(OBJS)
.PHONY: test
test: executable; ./executable --test
main.o: main.cpp $(INCS); $(CXX) $(CXXFLAGS) -c main.cpp
expr.o: expr.cpp $(INCS); $(CXX) $(CXXFLAGS) -c expr.cpp
tests.o: tests.cpp $(INCS); $(CXX) $(CXXFLAGS) -c tests.cpp
use_arguments.o: use_arguments.cpp $(INCS); $(CXX) $(CXXFLAGS) -c use_arguments.cpp



