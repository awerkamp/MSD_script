INCS = cmdline.h

OBJS = main.cpp use_arguments.cpp
CXXFLAGS = --std=c++14 -O2

msdscript: $(OBJS) ; $(CXX) $(CXXFLAGS) -o msdscript $(OBJS)

Main.o: $(INCS)

msdcript.o: $(INCS)