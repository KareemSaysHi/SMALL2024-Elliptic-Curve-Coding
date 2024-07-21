# Makefile for Writing Make Files Example
 
# *****************************************************
# Variables to control Makefile operation
 
CC = g++
CFLAGS = -Wall -g
 
# ****************************************************
# Targets needed to bring the executable up to date
 
main: main.o Point.o Square.o
    $(CC) $(CFLAGS) -o main main.o Point.o Square.o
 
# The main.o target can be written more simply
 
main.o: main.cpp Point.h Square.h
    $(CC) $(CFLAGS) -c main.cpp
 
Point.o: Point.h
 
Square.o: Square.h Point.h