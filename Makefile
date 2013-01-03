# Top-level makefile of the Nutshell Dynamics project, recursively invoking make
# in each of the subdirectories. Written for GNU Make.

# Parameters controlling inplicit rules (as listed in section 10.3 of the GNU
# Make Manual) or that the user can override with command options use upper case
# letters. Those serving internal purposes in this makefile use lower case
# letters. This practice is recommended in chapter 6 of the GNU Make Manual.

# "Every Makefile should contain this line..."
SHELL := /bin/sh # - section 7.2.1 of the GNU Coding Standards

# Clear the suffix list; no suffix rules in this makefile. See section 7.2.1 of
# the GNU Coding Standards.
.SUFFICES:

################################################################################

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

.PHONY: all clean snapshot

all clean:
	$(MAKE) -C src/ $@
	$(MAKE) -C examples/ $@

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
