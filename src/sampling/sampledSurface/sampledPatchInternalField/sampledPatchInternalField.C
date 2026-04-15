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

#include "sampledPatchInternalField.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace sampledSurfaces
{
    defineTypeNameAndDebug(patchInternalField, 0);
    addToRunTimeSelectionTable(sampledSurface, patchInternalField, word);
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::sampledSurfaces::patchInternalField::patchInternalField
(
    const word& name,
    const polyMesh& mesh,
    const dictionary& dict
)
:
    patch(name, mesh, dict),
    mappers_(patchIndices().size())
{
    // Negate the distance so that we sample cells inside the patch
    dictionary mappersDict(dict);
    if (dict.found("distance"))
    {
        mappersDict.set("distance", -mappersDict.lookup<scalar>("distance"));
    }

    forAll(patchIndices(), i)
    {
        mappers_.set
        (
            i,
            new mappedInternalPatchBase
            (
                mesh.boundary()[patchIndices()[i]],
                mappersDict
            )
        );
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::sampledSurfaces::patchInternalField::~patchInternalField()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

#define IMPLEMENT_SAMPLE(Type, nullArg)                                        \
    Foam::tmp<Foam::Field<Foam::Type>>                                         \
    Foam::sampledSurfaces::patchInternalField::sample                          \
    (                                                                          \
        const VolField<Type>& vField                                           \
    ) const                                                                    \
    {                                                                          \
        return sampleField(vField);                                            \
    }
FOR_ALL_FIELD_TYPES(IMPLEMENT_SAMPLE);
#undef IMPLEMENT_SAMPLE


#define IMPLEMENT_INTERPOLATE(Type, nullArg)                                   \
    Foam::tmp<Foam::Field<Foam::Type>>                                         \
    Foam::sampledSurfaces::patchInternalField::interpolate                     \
    (                                                                          \
        const interpolation<Type>& interpolator                                \
    ) const                                                                    \
    {                                                                          \
        return interpolateField(interpolator);                                 \
    }
FOR_ALL_FIELD_TYPES(IMPLEMENT_INTERPOLATE);
#undef IMPLEMENT_INTERPOLATE


void Foam::sampledSurfaces::patchInternalField::print(Ostream& os) const
{
    os  << "patchInternalField: " << name() << " :"
        << "  patches:" << patchNames()
        << "  faces:" << faces().size()
        << "  points:" << points().size();
}


// ************************************************************************* //
