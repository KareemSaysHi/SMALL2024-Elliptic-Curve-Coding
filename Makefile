# MACROS
CC = g++
CFLAGS = -fopenmp

# Dependency rules for non-file targets
all: computegarbage graphgarbage

clobber: clean
	rm -f *~ \#*\# 

clean: 
	del /f /q *.o computegarbage graphgarbage

# Dependency rules for file targets
computegarbage: quarticclasses.o helperfunctions.o
	$(CC) $(CFLAGS) quarticclasses.o helperfunctions.o -o computegarbage

graphgarbage: quarticgrapher.o helperfunctions.o
	$(CC) $(CFLAGS) quarticgrapher.o helperfunctions.o -o graphgarbage

quarticclasses.o: quarticclasses.cpp helperfunctions.h
	$(CC) $(CFLAGS) -c quarticclasses.cpp

quarticgrapher.o: quarticgrapher.cpp helperfunctions.h
	$(CC) $(CFLAGS) -c quarticgrapher.cpp

helperfunctions.o: helperfunctions.cpp helperfunctions.h
	$(CC) $(CFLAGS) -c helperfunctions.cpp

helperfunctions.h:
	touch helperfunctions.h