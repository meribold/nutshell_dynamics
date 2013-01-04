# "Every Makefile should contain this line..."
SHELL := /bin/sh # - section 7.2.1 of the GNU Coding Standards

# Clear the suffix list; no suffix rules in this makefile. See section 7.2.1 of
# the GNU Coding Standards.
.SUFFICES:
 
CXX      = g++ -std=c++11
CPPFLAGS =
CXXFLAGS = -Wall -Wextra -Wold-style-cast -pedantic -g -O
ARFLAGS  = cs

################################################################################

# See section 7.2.3 'Varialbes for Specifying Commands' of the GNU Coding
# Standards
ALL_CPPFLAGS := $(CPPFLAGS)
ALL_CXXFLAGS := $(CXXFLAGS) -c
ALL_ARFLAGS  := r$(ARFLAGS)

sources      := advanceState.cpp body.cpp rigidBody.cpp
objects      := $(sources:.cpp=.o)
prereq_files := $(sources:.cpp=.d)

# All whitespace-separated words in the working directory that do match any of
# the pattern words $(prereq_files). Beware of file names containing the '%'
# character.
existant_prereqs := $(filter $(prereq_files),$(shell ls))

.PHONY: all clean

all: libnut.a

# Was any goal (other than 'clean') specified on the command line? None counts
# as 'all'.
ifneq ($(filter-out clean,$(or $(MAKECMDGOALS),all)),)
   # Include existant makefiles of prerequisite . After reading in all those
   # files none of them will have to be updated. Non-existant prerequisite files
   # will be build along with their respective object files.
   include $(existant_prereqs)
endif

clean:
	$(RM) $(objects) $(existant_prereqs) libnut.a

libnut.a: $(objects)
	$(AR) $(ALL_ARFLAGS) $@ $^

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
	$(CXX) -MMD $(ALL_CPPFLAGS) $(ALL_CXXFLAGS) $(subst .o,.cpp,$@) -o $@
