INCS = cmdline.h

OBJS = main.cpp use_arguments.cpp
CXXFLAGS = --std=c++14 -O2

Executable: $(OBJS) ; $(CXX) $(CXXFLAGS) -o executable $(OBJS)

Main.o: $(INCS)

Sum.o: $(INCS)