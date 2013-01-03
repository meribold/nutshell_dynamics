SHELL := /bin/sh

CXX := g++
IDIRS := ../../
CPPFLAGS := -std=c++11 $(addprefix -I, $(IDIRS))
CXXFLAGS := -Wall -Wextra -Wold-style-cast -pedantic -g -c
LDIRS := ../../nutshell_dynamics/
LDLIBS := -lGL -lGLU -lglut -lnut
LDFLAGS := $(addprefix -L, $(LDIRS))
PROG := humble
SRCS := humble.cpp
#OBJS := $(addprefix nutshell_dynamics/,advanceState.o body.o rigidBody.o) $(SRCS:.cpp=.o)
OBJS := $(SRCS:.cpp=.o)
PREREQS := $(SRCS:.cpp=.d)

.SUFFICES:

.PHONY: all clean

all: humble

ifneq ($(filter-out clean,$(or $(MAKECMDGOALS),all)),)
    -include $(PREREQS)
endif

$(PROG): $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $(PROG)

# never attempt to built nutshell_dynamics object files from here
$(filter nutshell_dynamics/%,$(OBJS)): ;

clean:
	$(RM) $(PROG) $(filter-out nutshell_dynamics/%,$(OBJS)) $(PREREQS)

$(PREREQS):
	@set -e; echo "(re)building $@"; \
	rule=$$($(CXX) $(CPPFLAGS) -MM $(patsubst %.d,%.cpp,$@)); \
	rule=$@' '$${rule}; \
	echo "$$rule" > $@

.SECONDEXPANSION:

$(filter-out nutshell_dynamics/%,$(OBJS)): $$(subst .o,.d,$$@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(patsubst %.o,%.cpp,$@) -o $@
