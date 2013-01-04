# "Every Makefile should contain this line..."
SHELL := /bin/sh # - section 7.2.1 of the GNU Coding Standards

# Clear the suffix list; no suffix rules in this makefile. See section 7.2.1 of
# the GNU Coding Standards.
.SUFFICES:

CXX      = g++ -std=c++11
CPPFLAGS =
CXXFLAGS = -Wall -Wextra -Wold-style-cast -pedantic -g -O
LDFLAGS  =
LDLIBS   =

################################################################################

sources          := humble.cpp
objects          := $(sources:.cpp=.o)
prereq_files     := $(sources:.cpp=.d)
existant_prereqs := $(filter $(prereq_files),$(shell ls))
program          := humble
include_dirs     := ../../
ld_dirs          := nutshell_dynamics/
libraries        := nutshell_dynamics/libnut.a

ALL_CPPFLAGS := $(CPPFLAGS) $(addprefix -I,$(include_dirs))
ALL_CXXFLAGS := $(CXXFLAGS) -c
ALL_LDFLAGS  := $(addprefix -L,$(ld_dirs))
ALL_LDLIBS   := -lGL -lGLU -lglut \
                $(patsubst lib%.a,-l%,$(notdir $(libraries))) $(LDLIBS)

.PHONY: all clean

all: $(program)

ifneq ($(filter-out clean,$(or $(MAKECMDGOALS),all)),)
    include $(existant_prereqs)
endif

$(program): $(objects) $(libraries)
	$(CXX) $(ALL_LDFLAGS) $(objects) $(ALL_LDLIBS) -o $(program)

clean:
	$(RM) $(program) $(existant_prereqs)

$(prereq_files):

.SECONDEXPANSION:

$(objects): $$(subst .o,.d,$$@)
	$(CXX) -MMD $(ALL_CPPFLAGS) $(ALL_CXXFLAGS) $(subst .o,.cpp,$@) -o $@
