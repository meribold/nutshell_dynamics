# Top-level makefile of Nutshell Dynamics; includes all Module.mk files it can
# find in any of the subdirectories. Written for GNU Make.

# Parameters controlling inplicit rules (as listed in section 10.3 of the GNU
# Make Manual) or that the user can override with command options use upper case
# letters. Those serving internal purposes in this makefile use lower case
# letters. This practice is recommended in chapter 6 of the GNU Make Manual.

# "Every Makefile should contain this line..."
SHELL := /bin/sh # - section 7.2.1 of the GNU Coding Standards

# Clear the suffix list; no suffix rules in this makefile. See section 7.2.1 of
# the GNU Coding Standards.
.SUFFICES:

CXX      = g++
CPPFLAGS = -Wall -Wextra -pedantic -g -O
CXXFLAGS = -std=c++11 -Wold-style-cast
LDFLAGS  = -g -O
LDLIBS   =
ARFLAGS  = cs

################################################################################

# See section 7.2.3 'Varialbes for Specifying Commands' of the GNU Coding
# Standards.
all_cppflags = $(CPPFLAGS)
all_cxxflags = $(CXXFLAGS) -c
all_ldflags  = $(LDFLAGS)
all_ldlibs   = $(LDLIBS)
all_arflags  = r$(ARFLAGS)

sources      :=
prereq_files := $(sources:.cpp=.d)
objects      := $(sources:.cpp=.o)
libraries    :=
programs     :=

all:

include $(shell find -name 'Module.mk')

# All whitespace-separated words in the working directory and its subdirectories
# that do match any of the pattern words $(prereq_files). file names shall not
# contain the '%' character.
existant_prereqs := \
   $(filter $(prereq_files),$(shell find -regex '.*\.d$$' -printf '%P\n'))

# Was any goal (other than 'clean') specified on the command line? None counts
# as 'all'.
ifneq ($(filter-out clean,$(or $(MAKECMDGOALS),all)),)
   # Include existant makefiles of prerequisite . After reading in all those
   # files none of them will have to be updated. Non-existant prerequisite files
   # will be build along with their respective object files.
   include $(existant_prereqs)
endif

.PHONY: all

all: $(programs) $(libraries)

.PHONY: clean

clean:
	$(RM) $(objects) $(prereq_files) $(libraries)

# Directories that will be included in development snapshot archives built by
# the snapshot target.
snapdirs := examples/ examples/humble/ src/

# Files included in snapshot archives. I think using the find program is okay,
# in that it doesn't further reduce the portability of this makefile.
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

# Create hard links to all files that are required to build a source archive.
# Build $(snapdirs) first but don't rebuild $@ when any of those direcories has
# a later timestamp than $@. See section 4.3 of the GNU Make Manual.
$(snaplinks): | $(snapdirs)
	@ln $(subst $(snaproot)/,,$@) $@

# Build the directory tree needed for the snapshot archive (as far as it doesn't
# exist already).
$(snapdirs):
	mkdir -p $@

# If the target is not an existant file, then Make will imagine it to have been
# updated whenever this rule is run - and the rule should only be run when the
# target is not an existant file.
# This way it is ensured that an object file corresponding to any of the targets
# of this rule (a makefile of prereq_files) will be updated, if the target does
# not exist. That file will be created along with the object file and included
# during the next invocation of make.
$(prereq_files):

.SECONDEXPANSION:

# A call $(subst foo,bar,text) replaces each occurence of 'foo' by 'bar' and
# does not substitute 'foo' for 'bar' as I tend to misunderstand it recurringly.
$(objects): $$(subst .o,.d,$$@)
	$(CXX) -MMD $(all_cppflags) $(all_cxxflags) $(subst .o,.cpp,$@) -o $@

# vim: tw=90 ts=8 sts=-1 sw=3 noet
