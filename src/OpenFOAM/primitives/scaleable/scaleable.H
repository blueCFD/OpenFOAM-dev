/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2026 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

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

Struct
    Foam::scaleable

Description
    Trait to identify types which are "scaleable"; i.e., that can be
    multiply-equals-d with a scalar. This has to be done explicitly, rather
    than by identifying the existence of the operator with decltype, in order
    to exclude bool and int and similar.

\*---------------------------------------------------------------------------*/

#ifndef scaleable_H
#define scaleable_H

#include <type_traits>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                          Class scaleable Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
struct scaleable : public std::false_type {};

template<class Type, class Void = void>
using enableIfScaleable = std::enable_if_t<scaleable<Type>::value, Void>;

template<class Type, class Void = void>
using enableIfNotScaleable = std::enable_if_t<!scaleable<Type>::value, Void>;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
