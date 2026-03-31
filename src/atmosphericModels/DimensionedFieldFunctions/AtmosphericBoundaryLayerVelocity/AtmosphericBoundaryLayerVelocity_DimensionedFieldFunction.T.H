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
    Foam::DimensionedFieldFunctions::AtmosphericBoundaryLayerVelocity

Description
    Runtime selectable DimensionedFieldFunction to set the velocity
    for an atmospheric boundary layer.

    See Foam::atmosphericBoundaryLayer for details.

Usage
    Example to set the U internal field:
    \verbatim
    internalField
    {
        type            atmosphericBoundaryLayerVelocity;
        libs            ("libatmosphericModels.so");
    }
    \endverbatim


SourceFiles
    AtmosphericBoundaryLayerVelocity_DimensionedFieldFunction.C

\*---------------------------------------------------------------------------*/

#ifndef AtmosphericBoundaryLayerVelocity_DimensionedFieldFunction_H
#define AtmosphericBoundaryLayerVelocity_DimensionedFieldFunction_H

#include "DimensionedFieldFunction.T.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace DimensionedFieldFunctions
{

/*---------------------------------------------------------------------------*\
                            Class AtmosphericBoundaryLayerVelocity Declaration
\*---------------------------------------------------------------------------*/

template<class DimensionedFieldType>
class AtmosphericBoundaryLayerVelocity
:
    public DimensionedFieldFunction<DimensionedFieldType>
{
    typedef typename DimensionedFieldType::Type_ Type;
    typedef typename DimensionedFieldType::GeoMesh_::Zone Zone;


public:

    //- Runtime type information
    TypeName("atmosphericBoundaryLayerVelocity");


    // Constructors

        //- Construct with dictionary to initialise given field
        AtmosphericBoundaryLayerVelocity
        (
            const dictionary& dict,
            DimensionedFieldType& field
        );

        //- Construct a copy for the given field
        AtmosphericBoundaryLayerVelocity
        (
            const AtmosphericBoundaryLayerVelocity& dff,
            DimensionedFieldType& field
        );

        //- Construct and return a clone for the specified field
        virtual autoPtr<DimensionedFieldFunction<DimensionedFieldType>>
        clone(DimensionedFieldType& field) const;


    //- Destructor
    virtual ~AtmosphericBoundaryLayerVelocity()
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
#include "AtmosphericBoundaryLayerVelocity_DimensionedFieldFunction.T.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
