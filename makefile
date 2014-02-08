BLDDIR="./build"
SRCDIR="./"
CFLAGS="-Wall"

all : $(BLDDIR)
	gcc -lncurses $(CFLAGS) -o $(BLDDIR)/mario $(SRCDIR)/*.c

$(BLDDIR) :
	mkdir -p $(BLDDIR)
