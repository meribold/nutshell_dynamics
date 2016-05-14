local_program := $(subdirectory)/humble

sources  += $(addsuffix .cpp,$(local_program))
programs += $(local_program)

$(local_program) : ld_dirs     = src
$(local_program) : all_ldflags = $(addprefix -L,$(ld_dirs)) $(LDFLAGS)
$(local_program) : all_ldlibs  = -lGL -lGLU -lglut \
                   $(patsubst lib%.a,-l%,$(notdir $(libraries))) $(LDLIBS)

# Enable the second expansion of prerequisites (only).
.SECONDEXPANSION:

$(local_program): $(addsuffix .o,$(local_program)) $$(libraries)
	$(CXX) $(all_ldflags) $^ $(all_ldlibs) -o $@

# vim: tw=90 ts=8 sts=-1 sw=3 noet
