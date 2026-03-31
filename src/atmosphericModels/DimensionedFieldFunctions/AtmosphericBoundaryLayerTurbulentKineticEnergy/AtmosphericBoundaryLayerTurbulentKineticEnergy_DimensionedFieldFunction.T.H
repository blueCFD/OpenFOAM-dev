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
    Foam::DimensionedFieldFunctions::AtmosphericBoundaryLayerTurbulentKineticEnergy

Description
    Runtime selectable DimensionedFieldFunction to set the turbulent kinetic
    energy, \f$k\f$, for an atmospheric boundary layer.

    See Foam::atmosphericBoundaryLayer for details.

Usage
    Example to set the k internal field:
    \verbatim
    internalField
    {
        type            atmosphericBoundaryLayerTurbulentKineticEnergy;
        libs            ("libatmosphericModels.so");
    }
    \endverbatim

See also
    Foam::atmosphericBoundaryLayer
    Foam::atmosphericBoundaryLayerTurbulentKineticEnergyFvPatchScalarField

SourceFiles
    AtmosphericBoundaryLayerTurbulentKineticEnergy_DimensionedFieldFunction.C

\*---------------------------------------------------------------------------*/

#ifndef AtmosphericBoundaryLayerTurbulentKineticEnergy_DimensionedFieldFunction_H
#define AtmosphericBoundaryLayerTurbulentKineticEnergy_DimensionedFieldFunction_H

#include "DimensionedFieldFunction.T.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace DimensionedFieldFunctions
{

/*---------------------------------------------------------------------------*\
       Class AtmosphericBoundaryLayerTurbulentKineticEnergy Declaration
\*---------------------------------------------------------------------------*/

template<class DimensionedFieldType>
class AtmosphericBoundaryLayerTurbulentKineticEnergy
:
    public DimensionedFieldFunction<DimensionedFieldType>
{
    typedef typename DimensionedFieldType::Type_ Type;
    typedef typename DimensionedFieldType::GeoMesh_::Zone Zone;


public:

    //- Runtime type information
    TypeName("atmosphericBoundaryLayerTurbulentKineticEnergy");


    // Constructors

        //- Construct with dictionary to initialise given field
        AtmosphericBoundaryLayerTurbulentKineticEnergy
        (
            const dictionary& dict,
            DimensionedFieldType& field
        );

        //- Construct a copy for the given field
        AtmosphericBoundaryLayerTurbulentKineticEnergy
        (
            const AtmosphericBoundaryLayerTurbulentKineticEnergy& dff,
            DimensionedFieldType& field
        );

        //- Construct and return a clone for the specified field
        virtual autoPtr<DimensionedFieldFunction<DimensionedFieldType>>
        clone(DimensionedFieldType& field) const;


    //- Destructor
    virtual ~AtmosphericBoundaryLayerTurbulentKineticEnergy()
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

#ifdef NoRepository
#include "AtmosphericBoundaryLayerTurbulentKineticEnergy_DimensionedFieldFunction.T.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
