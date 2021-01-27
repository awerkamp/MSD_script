INCS = cmdline.h
OBJS = main.o use_arguments.o expr.o
CXXFLAGS = --std=c++14 -O2
executable: $(OBJS) ; $(CXX) $(CXXFLAGS) -o executable $(OBJS)
main.o: main.cpp $(INCS); $(CXX) $(CXXFLAGS) -c main.cpp
expr.o: expr.cpp $(INCS); $(CXX) $(CXXFLAGS) -c expr.cpp
use_arguments.o: use_arguments.cpp $(INCS); $(CXX) $(CXXFLAGS) -c use_arguments.cpp


