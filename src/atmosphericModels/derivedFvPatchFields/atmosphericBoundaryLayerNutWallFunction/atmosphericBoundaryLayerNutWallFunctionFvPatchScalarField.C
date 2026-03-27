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

#include "atmosphericBoundaryLayerNutWallFunctionFvPatchScalarField.H"
#include "momentumTransportModel.H"
#include "atmosphericBoundaryLayer.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

tmp<scalarField>
atmosphericBoundaryLayerNutWallFunctionFvPatchScalarField::nut() const
{
    const label patchi = patch().index();

    const momentumTransportModel& turbModel =
        db().lookupType<momentumTransportModel>(internalField().group());

    const scalarField& y = turbModel.yb()[patchi];
    const tmp<volScalarField> tk = turbModel.k();
    const volScalarField& k = tk();
    const tmp<scalarField> tnuw = turbModel.nu(patchi);
    const scalarField& nuw = tnuw();

    const atmosphericBoundaryLayer& abl =
        atmosphericBoundaryLayer::New(patch().db());

    const scalar Cmu25 = pow025(abl.Cmu());
    const scalar kappa = abl.kappa();

    const scalarField z0(abl.z0(patch().Cf()));

    tmp<scalarField> tnutw(new scalarField(*this));
    scalarField& nutw = tnutw.ref();

    forAll(nutw, facei)
    {
        label celli = patch().faceCells()[facei];

        scalar uStar = Cmu25*sqrt(k[celli]);
        scalar yPlus = uStar*y[facei]/nuw[facei];

        scalar Edash = (y[facei] + z0[facei])/z0[facei];

        nutw[facei] =
            nuw[facei]*(yPlus*kappa/log(max(Edash, 1+1e-4)) - 1);

        if (debug)
        {
            Info<< "yPlus = " << yPlus
                << ", Edash = " << Edash
                << ", nutw = " << nutw[facei]
                << endl;
        }
    }

    return tnutw;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

atmosphericBoundaryLayerNutWallFunctionFvPatchScalarField::
atmosphericBoundaryLayerNutWallFunctionFvPatchScalarField
(
    const fvPatch& p,
    const DimensionedField<scalar, fvMesh>& iF,
    const dictionary& dict
)
:
    nutkWallFunctionFvPatchScalarField(p, iF, dict)
{}


atmosphericBoundaryLayerNutWallFunctionFvPatchScalarField::
atmosphericBoundaryLayerNutWallFunctionFvPatchScalarField
(
    const atmosphericBoundaryLayerNutWallFunctionFvPatchScalarField& ptf,
    const fvPatch& p,
    const DimensionedField<scalar, fvMesh>& iF,
    const fieldMapper& mapper
)
:
    nutkWallFunctionFvPatchScalarField(ptf, p, iF, mapper)
{}


atmosphericBoundaryLayerNutWallFunctionFvPatchScalarField::
atmosphericBoundaryLayerNutWallFunctionFvPatchScalarField
(
    const atmosphericBoundaryLayerNutWallFunctionFvPatchScalarField& rwfpsf,
    const DimensionedField<scalar, fvMesh>& iF
)
:
    nutkWallFunctionFvPatchScalarField(rwfpsf, iF)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void atmosphericBoundaryLayerNutWallFunctionFvPatchScalarField::write
(
    Ostream& os
) const
{
    fvPatchField<scalar>::write(os);
    writeLocalEntries(os);
    writeEntry(os, "value", *this);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

makePatchTypeField
(
    fvPatchScalarField,
    atmosphericBoundaryLayerNutWallFunctionFvPatchScalarField
);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
