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

Class
    Foam::DimensionedFieldFunctions::displacementGapHeatTransferCoefficient

Description
    Runtime selectable DimensionedFieldFunction providing the heat transfer
    coefficient for a gap caused by surface displacement.

    Typically used to provide thermal resistance between regions with the
    Foam::coupledTemperatureFvPatchScalarField boundary condition where
    one region is deforming.

Usage
    \table
        Property        | Description             | Required    | Default value
        kappa           | Thermal conductivity of the gap [W/m/K] | yes |
    \endtable

    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type            coupledTemperature;

        h
        {
            type              displacementGapHeatTransferCoefficient;
            kappa             0.025;
        }

        value           $internalField;
    }
    \endverbatim

SourceFiles
    displacementGapHeatTransferCoefficient_DimensionedFieldFunction.C

\*---------------------------------------------------------------------------*/

#ifndef displacementGapHeatTransferCoefficient_DimensionedFieldFunction_H
#define displacementGapHeatTransferCoefficient_DimensionedFieldFunction_H

#include "DimensionedFieldFunction.T.H"
#include "fvPatch.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace DimensionedFieldFunctions
{

/*---------------------------------------------------------------------------*\
            Class displacementGapHeatTransferCoefficient Declaration
\*---------------------------------------------------------------------------*/

class displacementGapHeatTransferCoefficient
:
    public DimensionedFieldFunction<DimensionedField<scalar, fvPatch>>
{
    // Private Data

        //- Conductivity of wall layers
        dimensionedScalar kappa_;


public:

    //- Runtime type information
    TypeName("displacementGapHeatTransferCoefficient");


    // Constructors

        //- Construct with dictionary to initialise given field
        displacementGapHeatTransferCoefficient
        (
            const dictionary& dict,
            DimensionedField<scalar, fvPatch>& field
        );

        //- Construct a copy for the given field
        displacementGapHeatTransferCoefficient
        (
            const displacementGapHeatTransferCoefficient& dff,
            DimensionedField<scalar, fvPatch>& field
        );

        //- Construct and return a clone for the specified field
        virtual
        autoPtr<DimensionedFieldFunction<DimensionedField<scalar, fvPatch>>>
        clone(DimensionedField<scalar, fvPatch>& field) const;


    //- Destructor
    virtual ~displacementGapHeatTransferCoefficient()
    {}


    // Member Functions

        //- Evaluate the function and set the field
        virtual void evaluate();

        //- Update the function and set the field
        virtual bool update();

        //- Write data to dictionary stream
        virtual void write(Ostream& os) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace DimensionedFieldFunctions
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
