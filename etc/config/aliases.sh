#----------------------------------*-sh-*--------------------------------------
# =========                 |
# \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
#  \\    /   O peration     |
#   \\  /    A nd           | Copyright (C) 2011-2015 OpenFOAM Foundation
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
#        - Added sourcing of the "*.sh" scripts from the folder
#          "etc/config.d/".
#        - Added aliases for the following items:
#            wmSC, wmMC, wmNONSTOP, wmSTOPON1st, user
#
# File
#     etc/config/aliases.sh
#
# Description
#     Aliases for working with OpenFOAM
#     Sourced from OpenFOAM-<VERSION>/etc/bashrc and/or ~/.bashrc
#
#------------------------------------------------------------------------------

# Change compiled version aliases
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
alias wmSET='. $WM_PROJECT_DIR/etc/bashrc'
alias wm64='wmSET WM_ARCH_OPTION=64'
alias wm32='wmSET WM_ARCH_OPTION=32'
alias wmSP='wmSET WM_PRECISION_OPTION=SP'
alias wmDP='wmSET WM_PRECISION_OPTION=DP'

# refresh the environment
alias wmREFRESH='wmSET $FOAM_SETTINGS'

# clear env
alias wmUNSET='. $WM_PROJECT_DIR/etc/config/unset.sh'

# Toggle wmakeScheduler on/off
#  - also need to set WM_HOSTS
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
alias wmSchedON='export WM_SCHEDULER=$WM_PROJECT_DIR/wmake/wmakeScheduler'
alias wmSchedOFF='unset WM_SCHEDULER'

# Toggle Multi-Core On/Off
# Proposed the following bug report: http://www.openfoam.com/mantisbt/view.php?id=211
#wmSC - single core machine
#wmMC - multi-core machine
alias wmSC='unset WM_NCOMPPROCS && echo "Building enabled for a single core"'
alias wmMC='export WM_NCOMPPROCS=${NUMBER_OF_PROCESSORS:-1}; test -r /proc/cpuinfo && export WM_NCOMPPROCS=$(egrep "^processor" /proc/cpuinfo | wc -l); echo "Building enabled for $WM_NCOMPPROCS cores"'

# Toggle WM_CONTINUE_ON_ERROR on/off
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
alias wmNONSTOP='export WM_CONTINUE_ON_ERROR=1'
alias wmSTOPON1st='unset WM_CONTINUE_ON_ERROR'

# Change ParaView version
# ~~~~~~~~~~~~~~~~~~~~~~~
unset foamPV
foamPV()
{
    . $WM_PROJECT_DIR/etc/config/paraview.sh ParaView_VERSION=$1
    echo "paraview-$ParaView_VERSION  (major: $ParaView_MAJOR)" 1>&2
}


# Change directory aliases
# ~~~~~~~~~~~~~~~~~~~~~~~~
alias src='cd $FOAM_SRC'
alias lib='cd $FOAM_LIBBIN'
alias run='cd $FOAM_RUN'
alias user='cd ${FOAM_RUN%/*}'
alias foam='cd $WM_PROJECT_DIR'
alias foamsrc='cd $FOAM_SRC/$WM_PROJECT'
alias foamfv='cd $FOAM_SRC/finiteVolume'
alias app='cd $FOAM_APP'
alias util='cd $FOAM_UTILITIES'
alias sol='cd $FOAM_SOLVERS'
alias tut='cd $FOAM_TUTORIALS'

alias foamApps='cd $FOAM_APP'
alias foamSol='cd $FOAM_SOLVERS'
alias foamTuts='cd $FOAM_TUTORIALS'
alias foamUtils='cd $FOAM_UTILITIES'
alias foam3rdParty='cd $WM_THIRD_PARTY_DIR'

if [ -n "$WM_PROJECT_SITE" ]
then
    alias foamSite='cd $WM_PROJECT_SITE'
else
    alias foamSite='cd $WM_PROJECT_INST_DIR/site'
fi

# -----------------------------------------------------------------------------
