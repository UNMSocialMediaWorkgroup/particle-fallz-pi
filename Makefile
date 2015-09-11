CC=gcc
CFLAGS:=-Wall -Werror -pedantic -ansi
LINKS=
SRCDIR=./src
INCDIR=$(SRCDIR)/inc
BUILDDIR=./build
BINDIR=./bin
OUTDIRS=$(BINDIR) $(BUILDDIR)
SOURCES=$(shell find $(SRCDIR) -type f -name "*.c")
HEADERS=$(shell find $(INCDIR) -type f -name "*.h")
OBJECTS=$(subst $(SRCDIR),$(BUILDDIR),$(subst .c,.o,$(SOURCES)))
EXE=$(BINDIR)/pfp
ALLDEPS=$(OUTDIRS) $(OBJECTS) $(EXE)

ifdef DEBUG
CFLAGS:=$(CFLAGS) -g
else
CFLAGS:=$(CFLAGS) -O99
endif

all:	$(ALLDEPS)

$(OUTDIRS):
	mkdir -p $@

$(BUILDDIR)/%.o:	$(SRCDIR)/%.c
	$(CC) $(LINKS) $(CFLAGS) -I$(INCDIR) -c -o $@ $^

$(EXE):	$(OBJECTS)
	$(CC) $(LINKS) $(CFLAGS) -I$(INCDIR) -o $@ $^

clean:
	rm -rf $(ALLDEPS)

.PHONY:	clean