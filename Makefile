# Top-level makefile; includes all Module.mk files it can find in any of the
# subdirectories.  Partly based on the non-recursive make from "Managing Projects With GNU
# Make".  Written for GNU Make and not expected to be portable.

# Parameters controlling implicit rules (as listed in section 10.3 of the GNU Make Manual)
# or that the user can override with command options use upper case letters.  Those
# serving internal purposes in this makefile use lower case letters.  This practice is
# recommended in chapter 6 of the GNU Make Manual.

# "Every Makefile should contain this line..." - section 7.2.1 of the GNU Coding Standards
SHELL := /bin/sh

# Disable all built-in rules.  See http://stackoverflow.com/q/4122831 and
# http://gnu.org/software/make/manual/html_node/Catalogue-of-Rules.html.
MAKEFLAGS += --no-builtin-rules

# Clear the suffix list; no suffix rules in this makefile.  See section 7.2.1 of the GNU
# Coding Standards.
.SUFFIXES:

CXX      ?= g++
CPPFLAGS += -Wall -Wextra -pedantic -g -O
CXXFLAGS += -std=c++14 -Wold-style-cast
LDFLAGS  += -g -O
LDLIBS   +=
ARFLAGS  := cs

##########################################################################################

# Taken from "Managing Projects With GNU Make".
subdirectory = $(patsubst %/Module.mk,%, \
   $(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST)))

# See section 7.2.3 'Variables for Specifying Commands' of the GNU Coding Standards.
all_cppflags := $(CPPFLAGS)
all_cxxflags := $(CXXFLAGS) -c
all_ldflags  := $(LDFLAGS)
all_ldlibs   := $(LDLIBS)
all_arflags  := r$(ARFLAGS)

# Collect information from each module in these variables.  Explicitly initialize as
# simple variables as recursive ones are the default.
sources   :=
libraries :=
programs  :=

# Set the default target.
all:

include $(shell find -name 'Module.mk')

prereq_files := $(sources:.cpp=.d)
objects      := $(sources:.cpp=.o)

# All whitespace-separated words in the working directory and its subdirectories that do
# match any of the pattern words $(prereq_files).  File names shall not contain the "%"
# character.
existent_prereqs := \
   $(filter $(prereq_files),$(shell find -regex '.*\.d$$' -printf '%P\n'))

# Was any goal (other than `clean`) specified on the command line? None counts as `all`.
ifneq ($(filter-out clean,$(or $(MAKECMDGOALS),all)),)
   # Include existent makefiles of prerequisites.  After reading in all those files none
   # of them will have to be updated.  Non-existent prerequisite files will be build along
   # with their respective object files.
   include $(existent_prereqs)
endif

.PHONY: all

all: $(programs) $(libraries)

.PHONY: clean

clean:
	$(RM) $(prereq_files) $(objects) $(libraries) $(programs)

# Directories that will be included in development snapshot archives built by the snapshot
# target.
snapdirs := examples/ examples/humble/ src/

# Files included in snapshot archives.
snapfiles := COPYING INSTALL README Makefile
snapfiles += $(shell find $(snapdirs) -maxdepth 1 -regextype posix-extended \
                          -regex '.*\.(h|cpp)')
snapfiles += $(shell find $(snapdirs) -maxdepth 1 -name Makefile)

# Yields e.g. 19901229T1337Z.
date := $(shell date -u +%Y%m%dT%H%MZ)
snaproot := nutshell_dynamics-$(date)

snapdirs  := $(snaproot) $(addprefix $(snaproot)/,$(snapdirs))
snaplinks := $(addprefix $(snaproot)/,$(snapfiles))

.PHONY: snapshot

# Build a development snapshot archive.
snapshot: $(snaproot).tar

$(snaproot).tar: $(snapdirs) $(snaplinks)
	tar -cvf $(snaproot).tar $(snaproot)
	$(RM) -r $(snaproot)

# Create hard links to all files that are required to build a source archive.  Build
# $(snapdirs) first but don't rebuild $@ when any of those direcories has a later
# timestamp than $@.  See section 4.3 of the GNU Make Manual.
$(snaplinks): | $(snapdirs)
	@ln $(subst $(snaproot)/,,$@) $@

# Build the directory tree needed for the snapshot archive (as far as it doesn't exist
# already).
$(snapdirs):
	mkdir -p $@

# If the target is not an existent file, then Make will imagine it to have been updated
# whenever this rule is run -- and the rule should only be run when the target is not an
# existent file.
# This way it is ensured that an object file corresponding to any of the targets of this
# rule (a makefile of prereq_files) will be updated, if the target does not exist.  That
# file will be created along with the object file and included during the next invocation
# of make.
$(prereq_files):

# Enable the second expansion of prerequisites.
.SECONDEXPANSION:

# A call $(subst foo,bar,text) replaces each occurrence of 'foo' by 'bar' and does not
# substitute 'foo' for 'bar' as I tend to misunderstand it recurringly.
$(objects): $$(subst .o,.d,$$@)
	$(CXX) -MMD $(all_cppflags) $(all_cxxflags) $(subst .o,.cpp,$@) -o $@

# vim: tw=90 ts=8 sts=-1 sw=3 noet
