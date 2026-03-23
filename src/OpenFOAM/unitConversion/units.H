/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2011-2026 OpenFOAM Foundation
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

Global
    unitConversions

Description
    Useful unit conversions

SourceFiles
    unitConversions.C

\*---------------------------------------------------------------------------*/

#ifndef units_H
#define units_H

#include "unitConversion.H"
#include "dimensions.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace units
{
    extern const unitConversion unitless;

    extern const unitConversion any;
    extern const unitConversion none;

    extern const unitConversion fraction;
    extern const unitConversion percent;

    extern const unitConversion radians;
    extern const unitConversion rotations;
    extern const unitConversion degrees;

    //- Get the table of unit conversions
    const HashTable<unitConversion>& table();

    //- Add a unit conversion
    void add(const word& name, const unitConversion& unit);

    //- Lookup and return the named unit from the table
    inline const unitConversion& lookup(const word& unitName)
    {
        return table()[unitName];
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//- Convert degrees to radians
scalar degToRad(const scalar deg);

//- Convert radians to degrees
scalar radToDeg(const scalar rad);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
