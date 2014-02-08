BLDDIR="./build"
SRCDIR="./"
CFLAGS="-Wall"

all : $(BLDDIR)
	gcc -lm -lncurses $(CFLAGS) -o $(BLDDIR)/swiftsure $(SRCDIR)/*.c

$(BLDDIR) :
	mkdir -p $(BLDDIR)
