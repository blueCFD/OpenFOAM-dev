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
#     etc/config/openmpi.sh
#
# Description
#     Fine tuning openmpi settings for OpenFOAM, because there isn't Open-MPI
#     1.6.3 for Windows, only 1.6.2.
#     Sourced from OpenFOAM-<VERSION>/etc/config/settings.sh
#
#------------------------------------------------------------------------------

# 
case "$WM_COMPILER" in
mingw32 | mingw-w32 | mingw-w64 | i686-w64-mingw32 | x86_64-w64-mingw32)
    export FOAM_MPI=openmpi-1.6.2
    ;;
esac

# ----------------------------------------------------------------- end-of-file
