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

#include "Function2UnitSets.T.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::Function2s::unitSets::unitSets(std::initializer_list<unitSet> l)
:
    x(dimless),
    y(dimless),
    value(dimless)
{
    auto i = l.begin();
    x.reset(*i);
    y.reset(*(++i));
    value.reset(*(++i));
}


Foam::Function2s::unitSets::unitSets(Istream& is)
:
    x(dimless),
    y(dimless),
    value(dimless)
{
    is >> *this;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::Function2s::unitSets::read(Istream& is)
{
    is.readBegin("Function2s::unitSets");
    is.readBegin("Function2s::unitSets");
    x.read(is);
    y.read(is);
    is.readEnd("Function2s::unitSets");
    value.read(is);
    is.readEnd("Function2s::unitSets");
}


// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //

void Foam::assertNoConvertUnits
(
    const word& typeName,
    const Function2s::unitSets& units,
    const dictionary& dict
)
{
    if (!units.x.standard() || !units.y.standard() || !units.value.standard())
    {
        FatalIOErrorInFunction(dict)
            << "Unit conversions are not supported by "
            << typeName << " function2 types" << abort(FatalError);
    }
}


void Foam::writeEntry(Ostream& os, const Function2s::unitSets& units)
{
    os << units;
}


// * * * * * * * * * * * * * *  IOStream operators * * * * * * * * * * * * * //

Foam::Istream& Foam::operator>>
(
    Istream& is,
    Function2s::unitSets& units
)
{
    is.readBegin("Function2s::unitSets");
    is.readBegin("Function2s::unitSets");
    is >> units.x >> units.y;
    is.readEnd("Function2s::unitSets");
    is >> units.value;
    is.readEnd("Function2s::unitSets");

    is.check(FUNCTION_NAME);

    return is;
}


Foam::Ostream& Foam::operator<<
(
    Ostream& os,
    const Function2s::unitSets& units
)
{
    os  << token::BEGIN_LIST
        << token::BEGIN_LIST
        << units.x << token::SPACE << units.y
        << token::END_LIST << token::SPACE
        << units.value
        << token::END_LIST;

    os.check(FUNCTION_NAME);

    return os;
}


// ************************************************************************* //
