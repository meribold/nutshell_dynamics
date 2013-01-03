SHELL := /bin/sh

CXX := g++
CXXFLAGS := -std=c++11 -Wall -Wextra -Wold-style-cast -pedantic -g -c
CPPFLAGS := -std=c++11
SRCS := advanceState.cpp body.cpp rigidBody.cpp
OBJS := $(SRCS:.cpp=.o)
PREREQS := $(SRCS:.cpp=.d)

.PHONY: all clean

# We don't use any old-fashioned suffix rules, and clear the list of known suffixes.
.SUFFICES:

all: $(OBJS)

ifneq ($(filter-out clean,$(or $(MAKECMDGOALS),all)),) # any real goals?
   # don't emit a warning if files are missing; their existence is ensured upon building
   -include $(PREREQS) # their respective object file (thus the leading hyphen-minus)
endif

clean:
	$(RM) $(OBJS) $(PREREQS)

$(PREREQS):
	@set -e; echo "building $@"; \
	rule=$$($(CXX) -MM $(CPPFLAGS) $(patsubst %.d,%.cpp,$@)); \
	rule=$@' '$${rule}; \
	echo "$$rule" > $@

.SECONDEXPANSION:

$(OBJS): $$(subst .o,.d,$$@)
	$(CXX) $(CXXFLAGS) $(patsubst %.o,%.cpp,$@) -o $@
