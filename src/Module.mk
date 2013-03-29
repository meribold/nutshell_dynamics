local_directory := src

local_sources  := advanceState.cpp body.cpp rigidBody.cpp
local_library  := libnut.a

local_sources := $(addprefix $(local_directory)/,$(local_sources))
local_prereqs := $(local_sources:.cpp=.d)
local_objects := $(local_sources:.cpp=.o)
local_library := $(local_directory)/$(local_library)

sources      += $(local_sources)
prereq_files += $(local_prereqs)
objects      += $(local_objects)
libraries    += $(local_library)

$(local_library): $(local_objects)
	$(AR) $(all_arflags) $@ $^
