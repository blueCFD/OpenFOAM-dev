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
    unitSets

Description
    Useful unit conversions

SourceFiles
    unitSets.C

\*---------------------------------------------------------------------------*/

#ifndef units_H
#define units_H

#include "unitSet.H"
#include "dimensions.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace units
{
    extern const unitSet unitless;

    extern const unitSet any;
    extern const unitSet none;

    extern const unitSet fraction;
    extern const unitSet percent;

    extern const unitSet radians;
    extern const unitSet rotations;
    extern const unitSet degrees;

    extern const unitSet& length;

    //- Get the table of unit conversions
    const HashTable<unitSet>& table();

    //- Add a unit conversion
    void add(const word& name, const unitSet& unit);

    //- Lookup and return the named unit from the table
    const unitSet& lookup(const word& unitName);

    //- Set the length scale to the given unit set
    void setLength(const unitSet& length);

    //- Set the length scale to the given value
    void setLength(const scalar length);
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
