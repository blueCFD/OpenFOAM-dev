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

#include "atmosphericBoundaryLayerTurbulentDissipationRateFvPatchScalarField.H"
#include "atmosphericBoundaryLayer.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

atmosphericBoundaryLayerTurbulentDissipationRateFvPatchScalarField::
atmosphericBoundaryLayerTurbulentDissipationRateFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, fvMesh>& iF,
    const dictionary& dict
)
:
    inletOutletFvPatchScalarField(p, iF, dict, false)
{
    phiName_ = dict.lookupOrDefault<word>("phi", "phi");

    const atmosphericBoundaryLayer& abl =
        atmosphericBoundaryLayer::New(patch().db());

    refValue() = abl.epsilon(patch().Cf());
    refGrad() = 0;
    valueFraction() = 1;

    if (dict.found("value"))
    {
        scalarField::operator=
        (
            scalarField("value", iF.dimensions(), dict, p.size())
        );
    }
    else
    {
        scalarField::operator=(refValue());
    }
}


atmosphericBoundaryLayerTurbulentDissipationRateFvPatchScalarField::
atmosphericBoundaryLayerTurbulentDissipationRateFvPatchScalarField
(
    const atmosphericBoundaryLayerTurbulentDissipationRateFvPatchScalarField&
    psf,
    const fvPatch& p,
    const DimensionedField<scalar, fvMesh>& iF,
    const fieldMapper& mapper
)
:
    inletOutletFvPatchScalarField(psf, p, iF, mapper)
{}


atmosphericBoundaryLayerTurbulentDissipationRateFvPatchScalarField::
atmosphericBoundaryLayerTurbulentDissipationRateFvPatchScalarField
(
    const atmosphericBoundaryLayerTurbulentDissipationRateFvPatchScalarField&
    psf,
    const DimensionedField<scalar, fvMesh>& iF
)
:
    inletOutletFvPatchScalarField(psf, iF)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void atmosphericBoundaryLayerTurbulentDissipationRateFvPatchScalarField::write
(
    Ostream& os
) const
{
    fvPatchScalarField::write(os);
    writeEntryIfDifferent<word>(os, "phi", "phi", phiName_);
    writeEntry(os, "value", *this);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

makePatchTypeField
(
    fvPatchScalarField,
    atmosphericBoundaryLayerTurbulentDissipationRateFvPatchScalarField
);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
