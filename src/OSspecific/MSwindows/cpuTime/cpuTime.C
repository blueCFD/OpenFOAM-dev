/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of blueCAPE's unofficial mingw patches for OpenFOAM.
    For more information about these patches, visit:
         http://bluecfd.com/Core

    This file is a derivative work of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Modifications
    This file is based on the original version for POSIX:
        OpenFOAM/src/OSspecific/POSIX/

    This file was developed for Windows by:
        Copyright            : (C) 2011 Symscape
        Website              : www.symscape.com

    This copy of this file has been created by blueCAPE's unofficial mingw
    patches for OpenFOAM.
    For more information about these patches, visit:
        http://bluecfd.com/Core

    Modifications made:
      - Derived from the patches for blueCFD 2.1 and 2.2.

Description
    Starts timing CPU usage and return elapsed time from start.

\*---------------------------------------------------------------------------*/

#include "cpuTime.H"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

inline
double cpuTime::timeDifference
(
    const std::clock_t& start,
    const std::clock_t& end
)
{
    return std::difftime(end, start)/CLOCKS_PER_SEC;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

cpuTime::cpuTime()
:
    startTime_(std::clock()),
    prevTime_(startTime_),
    curTime_(startTime_)
{
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

double cpuTime::elapsedCpuTime() const
{
    curTime_ = std::clock();
    return timeDifference(startTime_, curTime_);
}


double cpuTime::cpuTimeIncrement() const
{
    prevTime_ = curTime_;
    curTime_ = std::clock();
    return timeDifference(prevTime_, curTime_);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
