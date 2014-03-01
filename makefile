BLDDIR="./build"
SRCDIR="./"
CFLAGS=-Wpedantic -Wall -g $(shell sdl2-config --cflags --libs)
LFLAGS=-lm -lGL -lGLEW
CC=clang

all : $(BLDDIR)
	$(CC) $(LFLAGS) $(CFLAGS) -o $(BLDDIR)/swiftsure $(SRCDIR)/*.c

$(BLDDIR) :
	mkdir -p $(BLDDIR)
