BLDDIR="./build"
SRCDIR="./"
CFLAGS=-Wall -g $(shell sdl2-config --cflags --libs)

all : $(BLDDIR)
	gcc -lm -lGL -lGLEW $(CFLAGS) -o $(BLDDIR)/swiftsure $(SRCDIR)/*.c

$(BLDDIR) :
	mkdir -p $(BLDDIR)
