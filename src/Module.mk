local_library := $(subdirectory)/libnut.a
local_sources := $(shell find $(subdirectory) -maxdepth 1 -name '*.cpp')

sources   += $(local_sources)
libraries += $(local_library)

$(local_library): $(local_sources:.cpp=.o)
	$(AR) $(all_arflags) $@ $^

# vim: tw=90 ts=8 sts=-1 sw=3 noet
