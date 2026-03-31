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

#include "AtmosphericBoundaryLayerTurbulentKineticEnergy_DimensionedFieldFunction.T.H"
#include "DimensionedField.T.H"
#include "atmosphericBoundaryLayer.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class DimensionedFieldType>
Foam::DimensionedFieldFunctions::
AtmosphericBoundaryLayerTurbulentKineticEnergy<DimensionedFieldType>::
AtmosphericBoundaryLayerTurbulentKineticEnergy
(
    const dictionary& dict,
    DimensionedFieldType& field
)
:
    DimensionedFieldFunction<DimensionedFieldType>(dict, field)
{}


template<class DimensionedFieldType>
Foam::DimensionedFieldFunctions::
AtmosphericBoundaryLayerTurbulentKineticEnergy<DimensionedFieldType>::
AtmosphericBoundaryLayerTurbulentKineticEnergy
(
    const AtmosphericBoundaryLayerTurbulentKineticEnergy& dff,
    DimensionedFieldType& field
)
:
    DimensionedFieldFunction<DimensionedFieldType>(dff, field)
{}


template<class DimensionedFieldType>
Foam::autoPtr<Foam::DimensionedFieldFunction<DimensionedFieldType>>
Foam::DimensionedFieldFunctions::
AtmosphericBoundaryLayerTurbulentKineticEnergy<DimensionedFieldType>::clone
(
    DimensionedFieldType& field
) const
{
    return autoPtr<DimensionedFieldFunction<DimensionedFieldType>>
    (
        new AtmosphericBoundaryLayerTurbulentKineticEnergy<DimensionedFieldType>
        (
            *this,
            field
        )
    );
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class DimensionedFieldType>
void Foam::DimensionedFieldFunctions::
AtmosphericBoundaryLayerTurbulentKineticEnergy<DimensionedFieldType>::evaluate()
{
    const atmosphericBoundaryLayer& abl =
        atmosphericBoundaryLayer::New(this->field_.mesh().db());

    this->field_.primitiveFieldRef() =
        abl.k(this->field_.mesh().C().primitiveField());
}


template<class DimensionedFieldType>
void Foam::DimensionedFieldFunctions::
AtmosphericBoundaryLayerTurbulentKineticEnergy<DimensionedFieldType>::write
(
    Ostream& os
) const
{}


// ************************************************************************* //
