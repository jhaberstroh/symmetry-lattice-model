# Based on Peter Miller's "Recursive Make Considered Harmful", 1997, AUUGN Journal of AUUG Inc., 19(1), pp. 14-25
MODULES:= MetropolisGrowth #name the subfolders to compile
#CXXTEST:= cxxtest
CPPFLAGS += $(patsubst %,-I%/inc,$(MODULES))
CPPFLAGS += $(patsubst %,-I%/test,$(MODULES))
# C Compiler not passed to ./depend.sh
CC=g++
CPPFLAGS += --std=c++11


# Generate the source & required libraries in each $(module)/module.mk file through the include
# Outputs
MAIN :=
# Inputs
LIBS :=
SRC := 
include $(patsubst %,%/module.mk,$(MODULES))
OBJ := $(patsubst %.cpp,%.o,$(filter %.cpp,$(SRC)))

# Generate the source for testing
TESTSRC :=
include $(patsubst %,%/module.mk,$(TESTING))
TESTOBJ := $(patsubst %.cpp,%.o,$(filter %.cpp,$(TESTSRC)))



.PHONY: clean sortaclean all cxxtests mains library
all: mains library $(TESTOBJ:.o=.t) 

sortaclean:
	rm $(OBJ) $(OBJ:.o=.d) $(TESTOBJ) $(TESTOBJ:.o=.d)

clean:
	rm $(OBJ) $(OBJ:.o=.d) $(TESTOBJ) $(TESTOBJ:.o=.d) TissueCells $(TESTOBJ:.o=)

# Iterate through building all of the .cpp files in $(MAIN)
mains: $(MAIN:.cpp=.main) $(OBJ)

# Checks all dependencies of $(MAIN.cpp), and runs the build with all objects
# Not all objects are guaranteed to be up to date, but all objects that are dependencies
#   are guaranteed to be up to date.
%.main: %.o
	$(CC) -o `basename $*` $< $(OBJ) $(LIBS)

# .o dependencies will build automatically
$(TESTING)/%.t: $(TESTING)/%.o $(OBJ)
	$(CC) -o $@ $^ $(LIBS)

include $(OBJ:.o=.d)

# Does not pass $(CC) to ./depend.sh 
%.d: %.cpp
	./depend.sh `dirname $<` $(CFLAGS) $< > $@
