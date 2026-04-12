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

#include "faceZoneToCell.H"
#include "polyMesh.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(faceZoneToCell, 0);
    addToRunTimeSelectionTable(topoSetSource, faceZoneToCell, word);
}

const Foam::NamedEnum<Foam::faceZoneToCell::faceAction, 2>
Foam::faceZoneToCell::faceActionNames_
{
    "master",
    "slave"
};


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void Foam::faceZoneToCell::combine(topoSet& set, const bool add) const
{
    forAll(mesh_.faceZones(), i)
    {
        const faceZone& fZone = mesh_.faceZones()[i];

        if (zoneName_.match(fZone.name()))
        {
            Info<< "    Found matching zone " << fZone.name() << endl;

            const labelList& faces = fZone;
            const labelList& own = mesh_.faceOwner();
            const labelList& nei = mesh_.faceNeighbour();

            if (fZone.oriented())
            {
                const boolList& flipMap = fZone.flipMap();

                forAll(faces, facei)
                {
                    if
                    (
                        option_ == MASTER
                     && !flipMap[facei]
                     && mesh_.isInternalFace(faces[facei]))
                    {
                        addOrDelete(set, nei[faces[facei]], add);
                    }
                    else
                    {
                        addOrDelete(set, own[faces[facei]], add);
                    }
                }
            }
            else
            {
                forAll(faces, facei)
                {
                    if (option_ == MASTER && mesh_.isInternalFace(faces[facei]))
                    {
                        addOrDelete(set, nei[faces[facei]], add);
                    }
                    else
                    {
                        addOrDelete(set, own[faces[facei]], add);
                    }
                }
            }
        }
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::faceZoneToCell::faceZoneToCell
(
    const polyMesh& mesh,
    const word& zoneName,
    const faceAction option
)
:
    topoSetSource(mesh),
    zoneName_(zoneName),
    option_(option)
{}


Foam::faceZoneToCell::faceZoneToCell
(
    const polyMesh& mesh,
    const dictionary& dict
)
:
    topoSetSource(mesh),
    zoneName_(dict.lookupBackwardsCompatible({"zone", "name"})),
    option_(faceActionNames_.read(dict.lookup("option")))
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::faceZoneToCell::~faceZoneToCell()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::faceZoneToCell::applyToSet
(
    const topoSetSource::setAction action,
    topoSet& set
) const
{
    if ((action == topoSetSource::NEW) || (action == topoSetSource::ADD))
    {
        Info<< "    Adding all " << faceActionNames_[option_]
            << " cells of faceZone " << zoneName_ << " ..." << endl;

        combine(set, true);
    }
    else if (action == topoSetSource::DELETE)
    {
        Info<< "    Removing all " << faceActionNames_[option_]
            << " cells of faceZone " << zoneName_ << " ..." << endl;

        combine(set, false);
    }
}


// ************************************************************************* //
