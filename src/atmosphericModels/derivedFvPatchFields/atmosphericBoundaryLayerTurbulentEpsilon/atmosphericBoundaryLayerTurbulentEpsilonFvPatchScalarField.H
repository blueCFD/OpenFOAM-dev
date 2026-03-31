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
    Foam::atmosphericBoundaryLayerTurbulentEpsilonFvPatchScalarField

Description
    This boundary condition specifies an inlet value for the turbulence
    dissipation rate, \f$\epsilon\f$, for an atmospheric boundary layer.

    See Foam::atmosphericBoundaryLayer for details.

See also
    Foam::atmosphericBoundaryLayer
    Foam::atmosphericBoundaryLayerVelocityFvPatchVectorField
    Foam::atmosphericBoundaryLayerTurbulentKineticEnergyFvPatchScalarField

SourceFiles
    atmosphericBoundaryLayerTurbulentEpsilonFvPatchScalarField.C

\*---------------------------------------------------------------------------*/

#ifndef atmosphericBoundaryLayerTurbulentEpsilonFvPatchScalarField_H
#define atmosphericBoundaryLayerTurbulentEpsilonFvPatchScalarField_H

#include "fvPatchFields.H"
#include "inletOutletFvPatchFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
   Class atmosphericBoundaryLayerTurbulentEpsilonFvPatchScalarField
\*---------------------------------------------------------------------------*/

class atmosphericBoundaryLayerTurbulentEpsilonFvPatchScalarField
:
    public inletOutletFvPatchScalarField
{

public:

    //- Runtime type information
    TypeName("atmosphericBoundaryLayerTurbulentEpsilon");


    // Constructors

        //- Construct from patch, internal field and dictionary
        atmosphericBoundaryLayerTurbulentEpsilonFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, fvMesh>&,
            const dictionary&
        );

        //- Construct by mapping given
        //  atmosphericBoundaryLayerTurbulentEpsilonFvPatchScalarField
        // onto a new patch
        atmosphericBoundaryLayerTurbulentEpsilonFvPatchScalarField
        (
            const
            atmosphericBoundaryLayerTurbulentEpsilonFvPatchScalarField&,
            const fvPatch&,
            const DimensionedField<scalar, fvMesh>&,
            const fieldMapper&
        );

        //- Construct as copy setting internal field reference
        atmosphericBoundaryLayerTurbulentEpsilonFvPatchScalarField
        (
            const
            atmosphericBoundaryLayerTurbulentEpsilonFvPatchScalarField&,
            const DimensionedField<scalar, fvMesh>&
        );

        //- Construct and return a clone setting internal field reference
        virtual tmp<fvPatchScalarField> clone
        (
            const DimensionedField<scalar, fvMesh>& iF
        ) const
        {
            return tmp<fvPatchScalarField>
            (
                new
              atmosphericBoundaryLayerTurbulentEpsilonFvPatchScalarField
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
        using inletOutletFvPatchScalarField::operator=;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
