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

#include "atmosphericBoundaryLayerVelocityFvPatchVectorField.H"
#include "atmosphericBoundaryLayer.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

atmosphericBoundaryLayerVelocityFvPatchVectorField::
atmosphericBoundaryLayerVelocityFvPatchVectorField
(
    const fvPatch& p,
    const DimensionedField<vector, fvMesh>& iF,
    const dictionary& dict
)
:
    inletOutletFvPatchVectorField(p, iF, dict, false)
{
    phiName_ = dict.lookupOrDefault<word>("phi", "phi");

    const atmosphericBoundaryLayer& abl =
        atmosphericBoundaryLayer::New(patch().db());

    refValue() = abl.U(patch().Cf());
    refGrad() = Zero;
    valueFraction() = 1;

    if (dict.found("value"))
    {
        vectorField::operator=
        (
            vectorField("value", iF.dimensions(), dict, p.size())
        );
    }
    else
    {
        vectorField::operator=(refValue());
    }
}


atmosphericBoundaryLayerVelocityFvPatchVectorField::
atmosphericBoundaryLayerVelocityFvPatchVectorField
(
    const atmosphericBoundaryLayerVelocityFvPatchVectorField& pvf,
    const fvPatch& p,
    const DimensionedField<vector, fvMesh>& iF,
    const fieldMapper& mapper
)
:
    inletOutletFvPatchVectorField(pvf, p, iF, mapper)
{}


atmosphericBoundaryLayerVelocityFvPatchVectorField::
atmosphericBoundaryLayerVelocityFvPatchVectorField
(
    const atmosphericBoundaryLayerVelocityFvPatchVectorField& pvf,
    const DimensionedField<vector, fvMesh>& iF
)
:
    inletOutletFvPatchVectorField(pvf, iF)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void atmosphericBoundaryLayerVelocityFvPatchVectorField::write
(
    Ostream& os
) const
{
    fvPatchVectorField::write(os);
    writeEntryIfDifferent<word>(os, "phi", "phi", phiName_);
    writeEntry(os, "value", *this);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

makePatchTypeField
(
    fvPatchVectorField,
    atmosphericBoundaryLayerVelocityFvPatchVectorField
);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
