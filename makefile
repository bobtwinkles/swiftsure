BLDDIR="./build"
SRCDIR="./"
CFLAGS=-Wall -g $(shell sdl2-config --cflags --libs)
CC=clang

all : $(BLDDIR)
	$(CC) -lm -lGL -lGLEW $(CFLAGS) -o $(BLDDIR)/swiftsure $(SRCDIR)/*.c

$(BLDDIR) :
	mkdir -p $(BLDDIR)
