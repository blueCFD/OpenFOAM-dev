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

#include "units.H"
#include "demandDrivenData.H"
#include "dictionary.H"

using namespace Foam::constant::mathematical;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

dictionary* unitsDictPtr_(nullptr);

const dictionary& unitsDict()
{
    if (!unitsDictPtr_)
    {
        dictionary* cachedPtr = nullptr;

        unitsDictPtr_ = new dictionary
        (
            debug::switchSet
            (
                debug::configDict().found("UnitSets")
              ? "UnitSets"
              : debug::configDict().found("DimensionSets")
              ? "DimensionSets"
              : "UnitSets",
                cachedPtr
            )
        );
    }

    return *unitsDictPtr_;
}

HashTable<unitSet>* addedUnitsPtr_(nullptr);
HashTable<unitSet>* unitsPtr_(nullptr);

// Delete the above data at the end of the run
struct deleteUnitsPtr
{
    ~deleteUnitsPtr()
    {
        deleteDemandDrivenData(unitsDictPtr_);
        deleteDemandDrivenData(addedUnitsPtr_);
        deleteDemandDrivenData(unitsPtr_);
    }
};

deleteUnitsPtr deleteUnitsPtr_;

}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

dimensionSet makeDimless()
{
    return dimensionSet(0, 0, 0, 0, 0);
}

unitSet makeUnitless()
{
    return unitSet(makeDimless(), 0, 0, 1);
}

unitSet makeUnitAny()
{
    return unitSet(makeDimless(), 0, 0, 0);
}
unitSet makeUnitNone()
{
    return unitSet(makeDimless(), 0, 0, -1);
}

unitSet makeUnitFraction()
{
    return unitSet(makeDimless(), 1, 0, 1);
}
unitSet makeUnitPercent()
{
    return unitSet(makeDimless(), 1, 0, 0.01);
}

unitSet makeUnitRadians()
{
    return unitSet(makeDimless(), 0, 1, 1);
}
unitSet makeUnitRotations()
{
    return unitSet(makeDimless(), 0, 1, 2*pi);
}
unitSet makeUnitDegrees()
{
    return unitSet(makeDimless(), 0, 1, pi/180);
}

}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

const Foam::unitSet Foam::units::unitless(makeUnitless());

const Foam::unitSet Foam::units::any(makeUnitAny());
const Foam::unitSet Foam::units::none(makeUnitNone());

const Foam::unitSet Foam::units::fraction(makeUnitFraction());
const Foam::unitSet Foam::units::percent(makeUnitPercent());

const Foam::unitSet Foam::units::radians(makeUnitRadians());
const Foam::unitSet Foam::units::rotations(makeUnitRotations());
const Foam::unitSet Foam::units::degrees(makeUnitDegrees());


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

const Foam::HashTable<Foam::unitSet>& Foam::units::table()
{
    if (!unitsPtr_)
    {
        unitsPtr_ = new HashTable<unitSet>();

        unitsPtr_->insert("%", makeUnitPercent());

        unitsPtr_->insert("rad", makeUnitRadians());
        unitsPtr_->insert("rot", makeUnitRotations());
        unitsPtr_->insert("deg", makeUnitDegrees());

        // Get the relevant part of the control dictionary
        const dictionary& unitSetDict =
            unitsDict().subDict(unitsDict().lookup<word>("unitSet") + "Coeffs");

        // Add units from the control dictionary
        forAllConstIter(dictionary, unitSetDict, iter)
        {
            ITstream& is = iter().stream();

            const unitSet units(is);
            const scalar multiplier = pTraits<scalar>(is);

            const bool ok =
                unitsPtr_->insert
                (
                    iter().keyword(),
                    units*unitSet(dimless, 0, 0, multiplier)
                );

            if (!ok)
            {
                FatalIOErrorInFunction(unitsDict())
                    << "Duplicate unit " << iter().keyword()
                    << " read from dictionary"
                    << exit(FatalIOError);
            }
        }

        // Add programmatically defined units
        if (addedUnitsPtr_)
        {
            forAllConstIter(HashTable<unitSet>, *addedUnitsPtr_, iter)
            {
                const bool ok = unitsPtr_->insert(iter.key(), iter());

                if (!ok)
                {
                    FatalIOErrorInFunction(unitsDict())
                        << "Duplicate unit " << iter.key()
                        << " added to dictionary"
                        << exit(FatalIOError);
                }
            }
        }
    }

    return *unitsPtr_;
}


void Foam::units::add(const word& name, const unitSet& units)
{
    deleteDemandDrivenData(unitsDictPtr_);

    if (!addedUnitsPtr_)
    {
        addedUnitsPtr_ = new HashTable<unitSet>();
    }

    addedUnitsPtr_->insert(name, units);

    deleteDemandDrivenData(unitsPtr_);
}


Foam::scalar Foam::degToRad(const scalar deg)
{
    return units::degrees.toStandard(deg);
}


Foam::scalar Foam::radToDeg(const scalar rad)
{
    return units::degrees.toUser(rad);
}


// ************************************************************************* //
