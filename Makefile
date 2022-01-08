# source files.
SRC = src/bucket.c src/arrays.c src/linkedList.c

OBJ = $(SRC:.cpp=.o)

OUT = run

# include directories
INCLUDES = -I. 
 
# C compiler flags 
#CCFLAGS = -O0 -Wall
CCFLAGSPAR = -std=c99 -O2 -Wall -fopenmp -I/share/apps/papi/5.5.0/include -L/share/apps/papi/5.5.0/lib
CCFLAGS = -std=c99 -O2 -Wall
#CCFLAGS = -O3 -Wall -msse4.1 -march=i686

# compiler
CCC = gcc 
#CCC = /opt/intel/Compiler/11.1/073/bin/ia32/icpc 
#CCC = g++-4.5

# libraries
LIBSPAR = -lm -lpapi
LIBS = -lm


default:
	$(CCC) -o $(OUT) $(CCFLAGS) $(OBJ) $(LIBS) 

par:
	$(CCC) -o $(OUT) $(CCFLAGSPAR) $(OBJ) $(LIBSPAR) 

clean:
	rm -f *.o .a *~ Makefile.bak $(OUT)
