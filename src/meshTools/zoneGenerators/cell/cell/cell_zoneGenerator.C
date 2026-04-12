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

\*---------------------------------------------------------------------------*/

#include "cell_zoneGenerator.H"
#include "polyMesh.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    namespace zoneGenerators
    {
        defineTypeNameAndDebug(cell, 0);
        addToRunTimeSelectionTable
        (
            zoneGenerator,
            cell,
            dictionary
        );
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::zoneGenerators::cell::cell
(
    const word& name,
    const polyMesh& mesh,
    const dictionary& dict
)
:
    zoneGenerator(name, mesh, dict),
    zoneGenerators_(mesh, dict)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::zoneGenerators::cell::~cell()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

Foam::zoneSet Foam::zoneGenerators::cell::generate() const
{
    boolList selectedCells(mesh_.nCells(), false);

    forAll(zoneGenerators_, i)
    {
        zoneSet zs(zoneGenerators_[i].generate());

        if (zs.pValid())
        {
            const labelList& zonePoints = zs.pZone();

            forAll(zonePoints, pi)
            {
                const labelList& pCells = mesh_.pointCells()[zonePoints[pi]];

                forAll(pCells, pci)
                {
                    selectedCells[pCells[pci]] = true;
                }
            }
        }

        if (zs.cValid() && zs.cZone().name() != zoneName_)
        {
            const labelList& zoneCells = zs.cZone();

            forAll(zoneCells, zci)
            {
                selectedCells[zoneCells[zci]] = true;
            }
        }

        if (zs.fValid())
        {
            const labelList& zoneFaces = zs.fZone();

            const labelList& own = mesh_.faceOwner();
            const labelList& nei = mesh_.faceNeighbour();

            forAll(zoneFaces, zfi)
            {
                const label facei = zoneFaces[zfi];

                selectedCells[own[facei]] = true;
                if (mesh_.isInternalFace(facei))
                {
                    selectedCells[nei[facei]] = true;
                }
            }
        }
    }

    moveUpdate_ = zoneGenerators_.moveUpdate();

    return zoneSet
    (
        new cellZone
        (
            zoneName_,
            indices(selectedCells),
            mesh_.cellZones(),
            moveUpdate_,
            true
        )
    );
}


// ************************************************************************* //
