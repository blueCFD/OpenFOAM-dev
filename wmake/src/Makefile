#------------------------------------------------------------------------------
# =========                 |
# \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
#  \\    /   O peration     | Website:  https://openfoam.org
#   \\  /    A nd           | Copyright (C) 2011-2021 OpenFOAM Foundation
#    \\/     M anipulation  |
#------------------------------------------------------------------------------
# 2014-02-21 blueCAPE Lda: Modifications for blueCFD-Core 2.3
# 2016-07-13&26 blueCAPE Lda: Modifications for blueCFD-Core 2016-1
# 2020-09-16 FSD blueCAPE Lda: Modifications for blueCFD-Core 2020-1
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
# Unset suffices list (suffix rules are not used)
#------------------------------------------------------------------------------

.SUFFIXES:


#------------------------------------------------------------------------------
# Set compilation and dependency building rules
#------------------------------------------------------------------------------

# Locally set optimised compilation
WM_COMPILE_OPTION = Opt

GENERAL_RULES   = $(WM_DIR)/rules/General
include $(GENERAL_RULES)/general

# Executable extension - independent of the global one
EXE_EXTENSION ?=

#------------------------------------------------------------------------------
# Targets
#------------------------------------------------------------------------------

all:  $(WMAKE_BIN)/dirToString$(EXE_EXTENSION) \
      $(WMAKE_BIN)/wmkdep$(EXE_EXTENSION)

clean:
	@E rm -f $(WMAKE_BIN)/* 2>/dev/null

$(WMAKE_BIN)/dirToString$(EXE_EXTENSION): dirToString.c
	@mkdir -p $(WMAKE_BIN)
	$(call QUIET_MESSAGE,compile,$<)
	$E $(cc) $(cFLAGS) dirToString.c -o $(WMAKE_BIN)/dirToString$(EXE_EXTENSION)

$(WMAKE_BIN)/wmkdep$(EXE_EXTENSION): wmkdep.l
	@mkdir -p $(WMAKE_BIN)
	$(call QUIET_MESSAGE,lex,$<)
	$E flex wmkdep.l; \
	$(cc) $(cFLAGS) lex.yy.c -o $(WMAKE_BIN)/wmkdep$(EXE_EXTENSION)
	@rm -f lex.yy.c 2>/dev/null


#------------------------------------------------------------------------------
