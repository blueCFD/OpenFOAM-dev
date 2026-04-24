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
    Foam::DimensionedFieldFunctions::externalWallLayersHeatTransferCoefficient

Description
    Runtime selectable DimensionedFieldFunction providing the heat transfer
    coefficient for a number of layers of conductive materials.

    Typically used to provide the thermal resistance or insulation layers with
    the Foam::externalTemperatureFvPatchScalarField boundary condition.

Usage
    \table
    Property        | Description               | Required    | Default value
    h               | Heat transfer coefficient [W/m^2/K] | yes |
    thicknessLayers | list of thicknesses per layer [m]   | yes |
    kappaLayers     | list of thermal conductivities per layer [W/m/K] | yes |
    \endtable

    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type            externalTemperature;

        Ta              constant 300.0;

        h
        {
            type              externalWallLayersHeatTransferCoefficient;

            h                 10.0;
            thicknessLayers   (0.1 0.2 0.3 0.4);
            kappaLayers       (1 2 3 4);
        }

        value           $internalField;
    }
    \endverbatim

SourceFiles
    externalWallLayersHeatTransferCoefficient_DimensionedFieldFunction.C

\*---------------------------------------------------------------------------*/

#ifndef externalWallLayersHeatTransferCoefficient_DimensionedFieldFunction_H
#define externalWallLayersHeatTransferCoefficient_DimensionedFieldFunction_H

#include "wallLayersHeatTransferCoefficient_DimensionedFieldFunction.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace DimensionedFieldFunctions
{

/*---------------------------------------------------------------------------*\
          Class externalWallLayersHeatTransferCoefficient Declaration
\*---------------------------------------------------------------------------*/

class externalWallLayersHeatTransferCoefficient
:
    public wallLayersHeatTransferCoefficient
{
    // Private Data

        //- External heat transfer coefficient [W/m^2K]
        scalar h_;


public:

    //- Runtime type information
    TypeName("externalWallLayersHeatTransferCoefficient");


    // Constructors

        //- Construct with dictionary to initialise given field
        externalWallLayersHeatTransferCoefficient
        (
            const dictionary& dict,
            DimensionedField<scalar, fvPatch>& field
        );

        //- Construct a copy for the given field
        externalWallLayersHeatTransferCoefficient
        (
            const externalWallLayersHeatTransferCoefficient& dff,
            DimensionedField<scalar, fvPatch>& field
        );

        //- Construct and return a clone for the specified field
        virtual
        autoPtr<DimensionedFieldFunction<DimensionedField<scalar, fvPatch>>>
        clone(DimensionedField<scalar, fvPatch>& field) const;


    //- Destructor
    virtual ~externalWallLayersHeatTransferCoefficient()
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
