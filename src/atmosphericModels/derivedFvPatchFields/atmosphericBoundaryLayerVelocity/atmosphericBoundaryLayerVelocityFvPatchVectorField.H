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

Class
    Foam::atmosphericBoundaryLayerVelocityFvPatchVectorField

Description
    This boundary condition specifies a velocity inlet profile appropriate
    for an atmospheric boundary layer.

    See Foam::atmosphericBoundaryLayer for details.

See also
    Foam::atmosphericBoundaryLayer
    Foam::atmosphericBoundaryLayerTurbulentKineticEnergyFvPatchScalarField
    Foam::atmosphericBoundaryLayerTurbulentEpsilonFvPatchScalarField

SourceFiles
    atmosphericBoundaryLayerVelocityFvPatchVectorField.C

\*---------------------------------------------------------------------------*/

#ifndef atmosphericBoundaryLayerVelocityFvPatchVectorField_H
#define atmosphericBoundaryLayerVelocityFvPatchVectorField_H

#include "fvPatchFields.H"
#include "inletOutletFvPatchFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
       Class atmosphericBoundaryLayerVelocityFvPatchVectorField Declaration
\*---------------------------------------------------------------------------*/

class atmosphericBoundaryLayerVelocityFvPatchVectorField
:
    public inletOutletFvPatchVectorField
{

public:

    //- Runtime type information
    TypeName("atmosphericBoundaryLayerVelocity");


    // Constructors

        //- Construct from patch, internal field and dictionary
        atmosphericBoundaryLayerVelocityFvPatchVectorField
        (
            const fvPatch&,
            const DimensionedField<vector, fvMesh>&,
            const dictionary&
        );

        //- Construct by mapping given
        //  atmosphericBoundaryLayerVelocityFvPatchVectorField onto a new patch
        atmosphericBoundaryLayerVelocityFvPatchVectorField
        (
            const atmosphericBoundaryLayerVelocityFvPatchVectorField&,
            const fvPatch&,
            const DimensionedField<vector, fvMesh>&,
            const fieldMapper&
        );

        //- Construct as copy setting internal field reference
        atmosphericBoundaryLayerVelocityFvPatchVectorField
        (
            const atmosphericBoundaryLayerVelocityFvPatchVectorField&,
            const DimensionedField<vector, fvMesh>&
        );

        //- Construct and return a clone setting internal field reference
        virtual tmp<fvPatchVectorField> clone
        (
            const DimensionedField<vector, fvMesh>& iF
        ) const
        {
            return tmp<fvPatchVectorField>
            (
                new atmosphericBoundaryLayerVelocityFvPatchVectorField
                (
                    *this,
                    iF
                )
            );
        }


    // Member Functions

        //- Write
        virtual void write(Ostream&) const;


    // Member Operators

        //- Inherit assignment
        using inletOutletFvPatchVectorField::operator=;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
