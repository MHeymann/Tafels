#
# Makefile for Tafels
#

# compiler flags
DEBUG    = -ggdb
OPTIMISE = -O0
WARNINGS = -Wall -Wextra -Wno-variadic-macros -pedantic
CFLAGS   = $(DEBUG) $(OPTIMISE) $(WARNINGS)
DFLAGS   = #-DDEBUG_SYMBOL_TABLE

# commands
CC       = gcc
RM       = rm -f
COMPILE  = $(CC) $(CFLAGS) $(DFLAGS)
INSTALL  = install

# files
EXES     = tafels testhashtable

# directories
LOCALBIN = ~/bin

### RULES ######################################################################

# executables

tafels: tafels.c hashtable.o
	$(COMPILE) -o $@ $^

testhashtable: testhashtable.c hashtable.o
	$(COMPILE) -o $@ $^

# units

hashtable.o: hashtable.c hashtable.h
	$(COMPILE) -c $<

### PHONY TARGETS ##############################################################

.PHONY: all clean install uninstall types

all: tafels

clean:
	$(RM) $(EXES)
	$(RM) *.o
	$(RM) -rf *.dSYM

# XXX: For your program to be in your PATH, ensure that the following is
# somewhere near the end of your ~/.profile (for Mac OS, this might actually be
# ~/.bash_profile) file:
#
# if [ -d "$HOME/bin" ] ; then
#     PATH="$HOME/bin:$PATH"
# fi
#
# If this is not set, the install rule will copy the executable to ~/bin, but
# it will not be resolved automatically by the shell.  If you update the
# profile, either close and reopen the terminal, or use the "source" command to
# reload the profile.
#
# Also, to enable bash completion (of filenames with .simpl extensions), add
# the following at the end of ~/.bashrc:
#
# # bash completion for SIMPL
# complete -f -X '!*.simpl' simplc

# Install all compiler-related binaries in the local bin.
install:
	mkdir -p $(LOCALBIN)
	$(INSTALL) $(foreach EXEFILE, $(EXES), $(wildcard $(EXEFILE))) $(LOCALBIN)

# Remove all compiler-related binaryes from the local bin.
uninstall:
	$(RM) $(foreach EXEFILE, $(EXES), $(LOCALBIN)/$(EXEFILE))
