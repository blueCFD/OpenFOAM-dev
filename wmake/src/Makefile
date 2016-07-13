#------------------------------------------------------------------------------
# =========                 |
# \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
#  \\    /   O peration     |
#   \\  /    A nd           | Copyright (C) 2011 OpenFOAM Foundation
#    \\/     M anipulation  |
#------------------------------------------------------------------------------
# 2014-02-21 blueCAPE Lda: Modifications for blueCFD-Core 2.3
#------------------------------------------------------------------------------
# License
#     This file is a derivative work of OpenFOAM.
#
#     OpenFOAM is free software: you can redistribute it and/or modify it
#     under the terms of the GNU General Public License as published by
#     the Free Software Foundation, either version 3 of the License, or
#     (at your option) any later version.
#
#     OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
#     ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
#     FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
#     for more details.
#
#     You should have received a copy of the GNU General Public License
#     along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.
#
# Modifications
#     This file has been modified by blueCAPE's unofficial mingw patches for
#     OpenFOAM.
#     For more information about these patches, visit:
#         http://bluecfd.com/Core
#
#     Modifications made:
#        - Derived from the patches for blueCFD 2.1 and 2.2.
#        - Added the ability to cross-compile the wmake related binaries for
#          running on Windows.
#        - Added the macros ADDITIONAL_*FLAGS, so that we can turn on a
#          few hacks.
#
# File
#     Makefile
#
# Description
#     Makefile used to compile wmake utilities.
#
#------------------------------------------------------------------------------

#------------------------------------------------------------------------------
# The Makefile use a POSIX shell
#------------------------------------------------------------------------------

SHELL      = /bin/sh

#------------------------------------------------------------------------------
# Initialise suffices list
#------------------------------------------------------------------------------

.SUFFIXES:
.SUFFIXES: .o

#------------------------------------------------------------------------------
# set compilation and dependency building rules
#------------------------------------------------------------------------------

GENERAL_RULES = $(WM_DIR)/rules/General
RULES         = $(WM_DIR)/rules/$(WM_ARCH)$(WM_COMPILER)
WMAKE_BIN    ?= $(WM_DIR)/platforms/$(WM_ARCH)$(WM_COMPILER)

# Executable extension - independent of the global one
EXE_EXTENSION ?=

# Additional flags, usually for adding more macros
ADDITIONAL_CFLAGS   ?=
ADDITIONAL_CXXFLAGS ?=

include $(RULES)/general
include $(RULES)/$(WM_LINK_LANGUAGE)


#------------------------------------------------------------------------------
# targets
#------------------------------------------------------------------------------

all:  $(WMAKE_BIN)/dirToString$(EXE_EXTENSION) $(WMAKE_BIN)/wmkdep$(EXE_EXTENSION) $(WMAKE_BIN)/wmkdepend$(EXE_EXTENSION)

clean:
	rm -f $(WMAKE_BIN)/* 2>/dev/null


$(WMAKE_BIN)/dirToString$(EXE_EXTENSION): dirToString.c
	@mkdir -p $(WMAKE_BIN)
	$(cc) $(cFLAGS) dirToString.c -o $(WMAKE_BIN)/dirToString$(EXE_EXTENSION)


$(WMAKE_BIN)/wmkdep$(EXE_EXTENSION): wmkdep.l
	@mkdir -p $(WMAKE_BIN)
	flex wmkdep.l
	$(cc) $(cFLAGS) $(ADDITIONAL_CFLAGS) lex.yy.c -o $(WMAKE_BIN)/wmkdep$(EXE_EXTENSION)
	@rm -f lex.yy.c 2>/dev/null


# for bootstrapping - use generated files directly (instead of from .atg file)
$(WMAKE_BIN)/wmkdepend$(EXE_EXTENSION): wmkdepend.cpp \
    wmkdependParser.cpp wmkdependScanner.cpp \
    wmkdependParser.h   wmkdependScanner.h
	@mkdir -p $(WMAKE_BIN)
	$(CC) $(c++FLAGS) $(ADDITIONAL_CXXFLAGS) \
	     wmkdepend.cpp wmkdependParser.cpp wmkdependScanner.cpp \
	    -o $(WMAKE_BIN)/wmkdepend$(EXE_EXTENSION)

#------------------------------------------------------------------------------
