/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2024-2026 OpenFOAM Foundation
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

\*---------------------------------------------------------------------------*/

#include "unitSet.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(unitSet, 0);
}


const Foam::NamedEnum<Foam::unitSet::dimlessUnitType, 2>
Foam::unitSet::dimlessUnitTypeNames_
{
    "fraction",
    "angle"
};


namespace Foam
{
    const scalar unitSet::smallExponent = rootSmall;
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

bool Foam::unitSet::compare
(
    const unitSet& a,
    const unitSet& b,
    const bool compareMultiplier
)
{
    if (a.any()) return true;
    if (b.any()) return true;
    if (a.none()) return false;
    if (b.none()) return false;

    // Check the dimensions are the same
    if (a.dimensions_ != b.dimensions_) return false;

    // Check the dimensionless units are the same
    for (int i = 0; i < unitSet::nDimlessUnits; ++ i)
    {
        if
        (
            mag(a.exponents_[i] - b.exponents_[i])
          > unitSet::smallExponent
        )
        {
            return false;
        }
    }

    // If specified, check the unit conversion factors are the same
    return !compareMultiplier || a.multiplier_ == b.multiplier_;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::unitSet::unitSet
(
    const dimensionSet& dimensions,
    const scalar fraction,
    const scalar angle,
    const scalar multiplier
)
:
    dimensions_(dimensions),
    multiplier_(multiplier)
{
    exponents_[FRACTION] = fraction;
    exponents_[ANGLE] = angle;
}


Foam::unitSet::unitSet(const dimensionSet& dimensions)
:
    dimensions_(dimensions),
    multiplier_(1)
{
    exponents_[FRACTION] = 0;
    exponents_[ANGLE] = 0;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::unitSet::reset(const unitSet& units)
{
    dimensions_.reset(units.dimensions_);
    for (int i = 0; i < unitSet::nDimlessUnits; ++ i)
    {
        exponents_[i] = units.exponents_[i];
    }
    multiplier_ = units.multiplier_;
}


// * * * * * * * * * * * * * * * Friend Functions  * * * * * * * * * * * * * //

Foam::unitSet Foam::pow(const unitSet& units, const scalar exp)
{
    if (units.any()) return units;
    if (units.none()) return units::none;

    return
        unitSet
        (
            pow(units.dimensions_, exp),
            units.exponents_[0]*exp,
            units.exponents_[1]*exp,
            pow(units.multiplier_, exp)
        );
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

const Foam::unitSet& Foam::operator+
(
    const unitSet& a,
    const unitSet& b
)
{
    if (a.any()) return b;
    if (b.any()) return a;
    if (a.none()) return units::none;
    if (b.none()) return units::none;

    if (!unitSet::compare(a, b, true))
    {
        FatalErrorInFunction
            << "Different units for +" << endl
            << "     units : " << a << " + " << b << endl
            << abort(FatalError);
    }

    return a;
}


Foam::unitSet Foam::operator*
(
    const unitSet& a,
    const unitSet& b
)
{
    if (a.any()) return a;
    if (b.any()) return b;
    if (a.none()) return units::none;
    if (b.none()) return units::none;

    return
        unitSet
        (
            a.dimensions_*b.dimensions_,
            a.exponents_[0] + b.exponents_[0],
            a.exponents_[1] + b.exponents_[1],
            a.multiplier_*b.multiplier_
        );
}


Foam::unitSet Foam::operator/
(
    const unitSet& a,
    const unitSet& b
)
{
    if (a.any()) return a;
    if (b.any()) return b;
    if (a.none()) return units::none;
    if (b.none()) return units::none;

    return
        unitSet
        (
            a.dimensions_/b.dimensions_,
            a.exponents_[0] - b.exponents_[0],
            a.exponents_[1] - b.exponents_[1],
            a.multiplier_/b.multiplier_
        );
}


// ************************************************************************* //
