/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2011-2018 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
 2011 Symscape: Added math definitions for 'j0f' and so on.
 2014-02-21 blueCAPE Lda: Modifications for blueCFD-Core 2.3
 2016-07-27 blueCAPE Lda: Workaround Strict ANSI for still including
                          transcendental functions, such as jnf
------------------------------------------------------------------------------
License
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
    This file has been modified by blueCAPE's unofficial mingw patches for
    OpenFOAM.
    For more information about these patches, visit:
        http://bluecfd.com/Core

\*---------------------------------------------------------------------------*/

#ifndef doubleFloat_H
#define doubleFloat_H

#include "label.H"
#include "products.H"

#if (defined( WIN32 ) || defined( WIN64 )) && defined(__STRICT_ANSI__)
#undef __STRICT_ANSI__
#define REDEFINE_SA__
#endif

#include <cmath>

#if defined( WIN32 ) || defined( WIN64 )
#ifdef REDEFINE_SA__
#define __STRICT_ANSI__ 1
#endif

#define j0f _j0
#define j1f _j1
#define jnf _jn
#define y0f _y0
#define y1f _y1
#define ynf _yn
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Cmpt>
class typeOfRank<Cmpt, 0>
{
public:

    typedef Cmpt type;
};


template<class Cmpt>
class symmTypeOfRank<Cmpt, 0>
{
public:

    typedef Cmpt type;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class T>
inline bool equal(const T& s1, const T& s2)
{
    return s1 == s2;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
