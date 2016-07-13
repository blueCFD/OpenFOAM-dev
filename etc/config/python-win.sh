#----------------------------------*-sh-*--------------------------------------
# License
#     Copyright (C) 2011-2014 blueCAPE Lda
#
#     This file is part of blueCAPE's unofficial mingw patches for OpenFOAM.
#     For more information about these patches, visit:
#         http://bluecfd.com/Core
#
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
# File
#     config/python-win.sh
#
# Description
#     Setup file for python include/libraries for a Windows installation.
#     Sourced during wmake process only.
#
# Note
#     A csh version is not needed, since the values here are only sourced
#     during the wmake process
#------------------------------------------------------------------------------

export PYTHON_MAJOR_VERSION=2
export PYTHON_MINOR_VERSION=7
export PYTHON_PATCH_VERSION=3

export PYTHON_VERSION=$PYTHON_MAJOR_VERSION.$PYTHON_MINOR_VERSION.$PYTHON_PATCH_VERSION
export PYTHON_ARCH_PATH=$WM_THIRD_PARTY_DIR/platforms/$WM_ARCH$WM_COMPILER/python-$PYTHON_VERSION
export PYTHON_ARCH_INCLUDE=$PYTHON_ARCH_PATH/include
export PYTHON_ARCH_LIB=$PYTHON_ARCH_PATH/lib

# -----------------------------------------------------------------------------
