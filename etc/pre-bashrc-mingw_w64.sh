#----------------------------------*-sh-*--------------------------------------
#
# License
#
#     Copyright (C) 2011-2024 blueCAPE Lda
#     blueCFD(R) is a registered trade mark of blueCAPE Lda
#     OpenFOAM(R) is a registered trade mark of ESI-OpenCFD
#
#     This file is part of blueCAPE's blueCFD-Core port of OpenFOAM technology
#     for Windows and it abides to the same GNU GPL v3 license.
#     blueCFD-Core was designed to improve the user experience in using
#     OpenFOAM in Windows via MSYS/MSYS2 and Windows command line.
#     For more information about blueCFD-Core, visit:
#         http://bluecfd.com/Core
#
#     OpenFOAM Foundation website: http://www.openfoam.org
#     OpenFOAM+ website (ESI-OpenCFD): http://www.openfoam.com
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
#     etc/pre-bashrc-mingw-w64.sh
#
# Description
#     Preset variables for the OpenFOAM configuration - POSIX shell syntax.
#
#     This file should be sourced instead of bashrc. Example:
#       source $HOME/OpenFOAM/OpenFOAM-dev/etc/pre-bashrc-mingw-w64.sh
#
#------------------------------------------------------------------------------

# Precursor retrieved from bashrc
[ "$BASH" ] && bashrcFile=${BASH_SOURCE}
[ "$ZSH_NAME" ] && bashrcFile=$0
if [ -n "$bashrcFile" ]
then
    export REF_OF_ETC=$(cd $(dirname $bashrcFile) && pwd -P)
fi
unset bashrcFile

# Source with our own settings
source $REF_OF_ETC/bashrc \
    WM_OSTYPE=MSwindows \
    foamCompiler=ThirdParty \
    WM_MPLIB=OPENMPI \
    WM_TARGET_ARCH=mingw_w64 \
    WM_COMPILER_ARCH= \
    WM_COMPILER_LIB_ARCH= \
    WM_ARCH_OPTION=64 \
    SCOTCH_TYPE=MSYS2 \
    METIS_TYPE=MSYS2 \
    PARMETIS_TYPE=MSYS2 \
    ZOLTAN_TYPE=blueCFDCore \
    $@

unset REF_OF_ETC

# Define the path for the cv2pdb utility, but don't add it to PATH, since this
# is strictly used on Windows only.
export CV2PDB_PATH=$WM_THIRD_PARTY_DIR/cv2pdb

# ----------------------------------------------------------------- end-of-file
