/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2012-2026 OpenFOAM Foundation
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

#include "fixedMeanFvPatchField.H"
#include "volFields.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
Foam::fixedMeanFvPatchField<Type>::fixedMeanFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, fvMesh>& iF,
    const dictionary& dict
)
:
    fixedValueFvPatchField<Type>
    (
        p,
        iF,
        dict,
        !p.time().completeCase()
    ),
    meanValue_
    (
        Function1<Type>::New
        (
            "meanValue",
            this->time().userUnits(),
            iF.dimensions(),
            dict
        )
    )
{
    if (p.time().completeCase())
    {
        this->evaluate();
    }
}


template<class Type>
Foam::fixedMeanFvPatchField<Type>::fixedMeanFvPatchField
(
    const fixedMeanFvPatchField<Type>& ptf,
    const fvPatch& p,
    const DimensionedField<Type, fvMesh>& iF,
    const fieldMapper& mapper
)
:
    fixedValueFvPatchField<Type>(ptf, p, iF, mapper),
    meanValue_(ptf.meanValue_, false)
{}


template<class Type>
Foam::fixedMeanFvPatchField<Type>::fixedMeanFvPatchField
(
    const fixedMeanFvPatchField<Type>& ptf,
    const DimensionedField<Type, fvMesh>& iF
)
:
    fixedValueFvPatchField<Type>(ptf, iF),
    meanValue_(ptf.meanValue_, false)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void Foam::fixedMeanFvPatchField<Type>::updateCoeffs()
{
    if (this->updated())
    {
        return;
    }

    Type meanValue = meanValue_->value(this->time().value());

    Field<Type> newValues(this->patchInternalField());

    Type meanValuePsi =
        gSum(this->patch().magSf()*newValues)
       /gSum(this->patch().magSf());

    if (mag(meanValue) > small && mag(meanValuePsi)/mag(meanValue) > 0.5)
    {
        newValues *= mag(meanValue)/mag(meanValuePsi);
    }
    else
    {
        newValues += (meanValue - meanValuePsi);
    }

    this->operator==(newValues);

    fixedValueFvPatchField<Type>::updateCoeffs();
}


template<class Type>
void Foam::fixedMeanFvPatchField<Type>::write(Ostream& os) const
{
    fvPatchField<Type>::write(os);
    writeEntry
    (
        os,
        this->time().userUnits(),
        this->internalField().dimensions(),
        meanValue_()
    );
    writeEntry(os, "value", *this);
}


// ************************************************************************* //
