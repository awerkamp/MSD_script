INCS = cmdline.h
OBJS = main.o use_arguments.o expr.o tests.o val.o env.o step.o cont.o
CXXFLAGS = --std=c++14 -O2 
msd_script: $(OBJS) ; $(CXX) $(CXXFLAGS) -o msd_script $(OBJS)
.PHONY: test
test: msd_script ; ./msd_script --test
.PHONY: inter
interp: msd_script ; ./msd_script --interp
.PHONY: print
print: mss_script ; ./msd_script --print
.PHONY: step
step: msd_script ; ./msd_script --step
main.o: main.cpp $(INCS); $(CXX) $(CXXFLAGS) -c main.cpp
val.o: val.cpp $(INCS); $(CXX) $(CXXFLAGS) -c val.cpp
expr.o: expr.cpp $(INCS); $(CXX) $(CXXFLAGS) -c expr.cpp
tests.o: tests.cpp $(INCS); $(CXX) $(CXXFLAGS) -c tests.cpp
step.o: step.cpp $(INCS); $(CXX) $(CXXFLAGS) -c step.cpp
cont.o: cont.cpp $(INCS); $(CXX) $(CXXFLAGS) -c cont.cpp
env.o: env.cpp $(INCS); $(CXX) $(CXXFLAGS) -c env.cpp
use_arguments.o: use_arguments.cpp $(INCS); $(CXX) $(CXXFLAGS) -c use_arguments.cpp