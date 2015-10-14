local_directory := examples/humble

local_sources  := humble.cpp
local_program  := humble

local_sources := $(addprefix $(local_directory)/,$(local_sources))
local_prereqs := $(local_sources:.cpp=.d)
local_objects := $(local_sources:.cpp=.o)
local_program := $(local_directory)/$(local_program)

sources      += $(local_sources)
prereq_files += $(local_prereqs)
objects      += $(local_objects)
programs     += $(local_program)

$(local_program) : ld_dirs      = src

$(local_program) : all_ldflags  = $(addprefix -L,$(ld_dirs)) $(LDFLAGS)
$(local_program) : all_ldlibs   = -lGL -lGLU -lglut \
                   $(patsubst lib%.a,-l%,$(notdir $(libraries))) $(LDLIBS)

$(local_program): $(local_objects) $(libraries)
	$(CXX) $(all_ldflags) $(local_objects) $(all_ldlibs) -o $(local_program)

# vim: tw=90 ts=8 sts=-1 sw=3 noet
