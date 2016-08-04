/*---------------------------------------------------------------------------*\
License
    Copyright            : (C) 2016 blueCAPE Lda
    Website              : www.bluecape.com.pt

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

Description
    This file is used for ensuring we are able to use transcendental
    mathematical functions, as well as use the CGAL version that MSys2
    provides.

Details
    This file has been created by blueCAPE's unofficial mingw patches for
    OpenFOAM.
    For more information about these patches, visit:
        http://bluecfd.com/Core

\*---------------------------------------------------------------------------*/

#ifndef workaroundMathAndANSIStrict_H
#define workaroundMathAndANSIStrict_H

#if (defined( WIN32 ) || defined( WIN64 )) && defined(__STRICT_ANSI__)
#undef __STRICT_ANSI__
#include <cmath>
#define __STRICT_ANSI__ 1
#endif

#endif //workaroundMathAndANSIStrict_H
