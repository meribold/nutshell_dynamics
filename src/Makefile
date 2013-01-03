# "Every Makefile should contain this line..."
SHELL := /bin/sh # - section 7.2.1 of the GNU Coding Standards

# Clear the suffix list; no suffix rules in this makefile. See section 7.2.1 of
# the GNU Coding Standards.
.SUFFICES:
 
################################################################################

CXX := g++
CXXFLAGS := -std=c++11 -Wall -Wextra -Wold-style-cast -pedantic -g -c
CPPFLAGS := -std=c++11
SRCS := advanceState.cpp body.cpp rigidBody.cpp
OBJS := $(SRCS:.cpp=.o)
PREREQS := $(SRCS:.cpp=.d)

.PHONY: all clean

all: libnut.a

# Was any goal (other than 'clean') specified on the command line?
ifneq ($(filter-out clean,$(or $(MAKECMDGOALS),all)),) # None counts as 'all'.
   include $(PREREQS) # If so, we'll need to include prerequisites files.
endif

# After reading in all makefiles, make will consider each as a goal target and
# attempt to update it. If a makefile has a rule which says how to update it,
# it will be updated if necessary. After all makefiles have been checked, if any
# have actually been changed, make starts with a clean slate and reads all the
# makefiles over again.

libnut.a: $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

clean:
	$(RM) $(OBJS) $(PREREQS)

.SECONDEXPANSION:

# The subst function replaces each occurence of .d by .cpp and does not
# substitute .d for .cpp as I tend to misunderstand it recurringly.
$(PREREQS): $$(subst .d,.cpp,$$@)
	@set -e; echo "building $@"; \
	rule=$$($(CXX) -MM $(CPPFLAGS) $(patsubst %.d,%.cpp,$@)); \
	rule=$@' '$${rule}; \
	echo "$$rule" > $@

$(OBJS): $$(subst .o,.d,$$@)
	$(CXX) $(CXXFLAGS) $(patsubst %.o,%.cpp,$@) -o $@
