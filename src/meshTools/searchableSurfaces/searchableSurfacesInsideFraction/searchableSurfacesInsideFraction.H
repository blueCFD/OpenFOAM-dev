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

Namespace
    Foam::searchableSurfaces

Description
    Functions to generate a field of the fraction of space (volume, area,
    etc ...) inside a given searchable surface. Cuts the mesh where elements
    span the surface.

SourceFiles
    searchableSurfacesInsideFraction.C

\*---------------------------------------------------------------------------*/

#ifndef searchableSurfacesInsideFraction_H
#define searchableSurfacesInsideFraction_H

#include "searchableSurface.H"
#include "polyMesh.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace searchableSurfaces
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//- Return a cell-field of the volume fraction inside the given surface
tmp<scalarField> insideFraction
(
    const searchableSurface& surface,
    const polyMesh& mesh
);

//- Return a patch-face-field of the area fraction inside the given surface
tmp<scalarField> insideFraction
(
    const searchableSurface& surface,
    const polyPatch& patch
);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace searchableSurfaces
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
