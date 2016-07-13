#----------------------------------*-sh-*--------------------------------------
# License
#
#     Copyright (C) 2013 blueCAPE Lda
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
#     etc/config/zoltan.sh
#
# Description
#     Add the environment variables for the Zoltan library,
#     Sourced from OpenFOAM-<VERSION>/etc/config/settings.sh
#
#------------------------------------------------------------------------------

export ZOLTAN_VERSION=3.6
export ZOLTAN_SOURCE_DIR=$WM_THIRD_PARTY_DIR/Zoltan_v${ZOLTAN_VERSION}
export ZOLTAN_ARCH_DIR=$WM_THIRD_PARTY_DIR/platforms/$WM_ARCH$WM_COMPILER/Zoltan_${ZOLTAN_VERSION}

# ----------------------------------------------------------------- end-of-file
