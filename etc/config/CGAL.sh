#----------------------------------*-sh-*--------------------------------------
# =========                 |
# \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
#  \\    /   O peration     |
#   \\  /    A nd           | Copyright (C) 2014 OpenFOAM Foundation
#    \\/     M anipulation  |
#------------------------------------------------------------------------------
# 2014-02-21 blueCAPE Lda: Modifications for blueCFD-Core 2.3
# 2016-03-15 blueCAPE Lda: Modifications for blueCFD-Core 2.4 and 3.0
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
#        - Added environment settings for the MinGW based Gcc cross-compilers.
#        - Revamped to using MSYS2.
#
# File
#     config/CGAL.sh
#
# Description
#     Setup file for CGAL (& boost) include/libraries.
#     Sourced from OpenFOAM-<VERSION>/etc/bashrc
#------------------------------------------------------------------------------

boost_version=boost-system
cgal_version=CGAL-system

if [ "$FOAM_VERBOSE" -a "$PS1" ]
then
    echo "Using CGAL and boost"
    echo "    $cgal_version at $CGAL_ARCH_PATH"
    echo "    $boost_version at $BOOST_ARCH_PATH"
fi

if [ "$cgal_version" != "CGAL-system" ]
then

    export CGAL_ARCH_PATH=$WM_THIRD_PARTY_DIR/platforms/$WM_ARCH$WM_COMPILER/$cgal_version

    if [ -d "$CGAL_ARCH_PATH" ]
    then
        _foamAddPath $CGAL_ARCH_PATH/bin
        _foamAddLib $CGAL_ARCH_PATH/lib
    fi

else

    export CGAL_ARCH_PATH=system

fi

if [ "$boost_version" != "boost-system" ]
then

    export BOOST_ARCH_PATH=$WM_THIRD_PARTY_DIR/platforms/$WM_ARCH$WM_COMPILER/$boost_version

    if [ -d "$BOOST_ARCH_PATH" ]
    then
        _foamAddPath $BOOST_ARCH_PATH/bin
        _foamAddLib $BOOST_ARCH_PATH/lib
    fi

else

    export BOOST_ARCH_PATH=system

fi

unset boost_version cgal_version

# -----------------------------------------------------------------------------
