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
    Foam::DimensionedFieldFunctions::wallLayersHeatTransferCoefficient

Description
    Runtime selectable DimensionedFieldFunction providing the heat transfer
    coefficient for a number of layers of conductive materials.

    Typically used to provide thermal resistance between regions with the
    Foam::coupledTemperatureFvPatchScalarField boundary condition.

Usage
    \table
        Property        | Description             | Required    | Default value
        thicknessLayers | list of thicknesses per layer [m] | yes |
        kappaLayers | list of thermal conductivities per layer [W/m/K] | yes |
    \endtable

    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type            coupledTemperature;

        h
        {
            type              wallLayersHeatTransferCoefficient;

            thicknessLayers   (0.1 0.2 0.3 0.4);
            kappaLayers       (1 2 3 4);
        }

        value           $internalField;
    }
    \endverbatim

SourceFiles
    wallLayersHeatTransferCoefficient_DimensionedFieldFunction.C

\*---------------------------------------------------------------------------*/

#ifndef wallLayersHeatTransferCoefficient_DimensionedFieldFunction_H
#define wallLayersHeatTransferCoefficient_DimensionedFieldFunction_H

#include "DimensionedFieldFunction.T.H"
#include "fvPatch.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace DimensionedFieldFunctions
{

/*---------------------------------------------------------------------------*\
            Class wallLayersHeatTransferCoefficient Declaration
\*---------------------------------------------------------------------------*/

class wallLayersHeatTransferCoefficient
:
    public DimensionedFieldFunction<DimensionedField<scalar, fvPatch>>
{
    // Private Data

        //- Thickness of wall layers
        scalarList thicknessLayers_;

        //- Conductivity of wall layers
        scalarList kappaLayers_;


public:

    //- Runtime type information
    TypeName("wallLayersHeatTransferCoefficient");


    // Constructors

        //- Construct with dictionary to initialise given field
        wallLayersHeatTransferCoefficient
        (
            const dictionary& dict,
            DimensionedField<scalar, fvPatch>& field
        );

        //- Construct a copy for the given field
        wallLayersHeatTransferCoefficient
        (
            const wallLayersHeatTransferCoefficient& dff,
            DimensionedField<scalar, fvPatch>& field
        );

        //- Construct and return a clone for the specified field
        virtual
        autoPtr<DimensionedFieldFunction<DimensionedField<scalar, fvPatch>>>
        clone(DimensionedField<scalar, fvPatch>& field) const;


    //- Destructor
    virtual ~wallLayersHeatTransferCoefficient()
    {}


    // Member Functions

        //- Evaluate the function and set the field
        virtual void evaluate();

        //- Write data to dictionary stream
        virtual void write(Ostream& os) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace DimensionedFieldFunctions
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
