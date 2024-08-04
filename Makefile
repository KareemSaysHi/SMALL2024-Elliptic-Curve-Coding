# MACROS
CC = g++
CFLAGS = -fopenmp -O3

# Dependency rules for non-file targets
all: computegarbage 
#graphgarbage familygarbage

clobber: clean
	rm -f *~ \#*\# 

clean: 
	del /f /q *.o computegarbage graphgarbage familygarbage

# Dependency rules for file targets
computegarbage: quarticclasses.o helperfunctions.o
	$(CC) $(CFLAGS) quarticclasses.o helperfunctions.o -o computegarbage

graphgarbage: quarticgrapher.o helperfunctions.o
	$(CC) $(CFLAGS) quarticgrapher.o helperfunctions.o -o graphgarbage

familygarbage: quarticgrapherfamilysearch.o helperfunctions.o
	$(CC) $(CFLAGS) quarticgrapherfamilysearch.o helperfunctions.o -o familygarbage

quarticclasses.o: quarticclasses.cpp helperfunctions.h
	$(CC) $(CFLAGS) -c quarticclasses.cpp

quarticgrapher.o: quarticgrapher.cpp helperfunctions.h
	$(CC) $(CFLAGS) -c quarticgrapher.cpp

quarticgrapherfamilysearch.o: quarticgrapherfamilysearch.cpp helperfunctions.h
	$(CC) $(CFLAGS) -c quarticgrapherfamilysearch.cpp

helperfunctions.o: helperfunctions.cpp helperfunctions.h
	$(CC) $(CFLAGS) -c helperfunctions.cpp

helperfunctions.h:
	touch helperfunctions.h
