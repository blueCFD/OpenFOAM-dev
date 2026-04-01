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

\*---------------------------------------------------------------------------*/

#include "dimensionedConstants.H"
#include "demandDrivenData.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

int dimensionedConstantsDebug(debug::debugSwitch("constants", 0));

dictionary* dimensionedConstantsDictPtr_(nullptr);

dictionary& dimensionedConstantsDict()
{
    if (!dimensionedConstantsDictPtr_)
    {
        dictionary* cachedPtr = nullptr;

        dimensionedConstantsDictPtr_ = new dictionary
        (
            debug::switchSet
            (
                debug::configDict().found("constants")
              ? "constants"
              : debug::configDict().found("DimensionedConstants")
              ? "DimensionedConstants"
              : "constants",
                cachedPtr
            )
        );
    }

    return *dimensionedConstantsDictPtr_;
}

// Delete the above data at the end of the run
struct deleteDimensionedConstantsPtr
{
    ~deleteDimensionedConstantsPtr()
    {
        if (dimensionedConstantsDebug)
        {
            Info<< "constants"
                << dimensionedConstantsDict() << endl;
        }

        deleteDemandDrivenData(dimensionedConstantsDictPtr_);
    }
};

deleteDimensionedConstantsPtr deleteDimensionedConstantsPtr_;

}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

Foam::dimensionedScalar Foam::dimensionedConstant
(
    const char* const group,
    const char* name,
    const dimensionSet& dimensions
)
{
    dictionary& dict = dimensionedConstantsDict();

    const dimensionedScalar dimensionedValue
    (
        name,
        dimensions,
        dict.isDict(group)
     && dict.subDict(group).found(name)
      ? dict.subDict(group).lookup(name)
      : dict.found(name)
      ? dict.lookup(name)
      : dict.isDict(group)
      ? dict.subDict(group).lookup(name)
      : dict.lookup(name)
    );

    if (!dict.found(group))
    {
        dict.add(group, dictionary::null);
    }

    dict.subDict(group).set(name, dimensionedValue);

    if (dict.found(name))
    {
        dict.remove(name);
    }

    return dimensionedValue;
}


Foam::dimensionedScalar Foam::dimensionedConstant
(
    const char* const group,
    const char* name,
    const dimensionedScalar& valueNoName
)
{
    return dimensionedConstant(group, name, name, valueNoName);
}


Foam::dimensionedScalar Foam::dimensionedConstant
(
    const char* const group,
    const char* entryName,
    const char* codeName,
    const dimensionedScalar& valueNoName
)
{
    dictionary& dict = dimensionedConstantsDict();

    const dimensionedScalar dimensionedValue(codeName, valueNoName);

    if (!dict.found(group))
    {
        dict.add(group, dictionary::null);
    }

    dict.subDict(group).add(entryName, dimensionedValue);

    return dimensionedValue;
}


Foam::dimensionedScalar Foam::dimensionedConstant
(
    const char* const group,
    const char* name,
    const unitSet& units,
    const scalar value
)
{
    return dimensionedConstant(group, name, name, units, value);
}


Foam::dimensionedScalar Foam::dimensionedConstant
(
    const char* const group,
    const char* entryName,
    const char* codeName,
    const unitSet& units,
    const scalar value
)
{
    dictionary& dict = dimensionedConstantsDict();

    const dimensionedScalar dimensionedValue
    (
        codeName,
        units.dimensions(),
        units.toStandard(value)
    );

    if (!dict.found(group))
    {
        dict.add(group, dictionary::null);
    }

    dict.subDict(group).add(entryName, dimensionedValue);

    return dimensionedValue;
}


// ************************************************************************* //
