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
    Foam::atmosphericBoundaryLayerTurbulentDissipationRateFvPatchScalarField

Description
    This boundary condition specifies an inlet value for the turbulence
    dissipation, \f$\epsilon\f$, appropriate for atmospheric boundary layers.

    See Foam::atmosphericBoundaryLayer for details.

See also
    Foam::atmosphericBoundaryLayer
    Foam::atmosphericBoundaryLayerVelocityFvPatchVectorField
    Foam::atmosphericBoundaryLayerTurbulentKineticEnergyFvPatchScalarField

SourceFiles
    atmosphericBoundaryLayerTurbulentDissipationRateFvPatchScalarField.C

\*---------------------------------------------------------------------------*/

#ifndef atmosphericBoundaryLayerTurbulentDissipationRateFvPatchScalarField_H
#define atmosphericBoundaryLayerTurbulentDissipationRateFvPatchScalarField_H

#include "fvPatchFields.H"
#include "inletOutletFvPatchFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
   Class atmosphericBoundaryLayerTurbulentDissipationRateFvPatchScalarField
\*---------------------------------------------------------------------------*/

class atmosphericBoundaryLayerTurbulentDissipationRateFvPatchScalarField
:
    public inletOutletFvPatchScalarField
{

public:

    //- Runtime type information
    TypeName("atmosphericBoundaryLayerTurbulentDissipationRate");


    // Constructors

        //- Construct from patch, internal field and dictionary
        atmosphericBoundaryLayerTurbulentDissipationRateFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, fvMesh>&,
            const dictionary&
        );

        //- Construct by mapping given
        //  atmosphericBoundaryLayerTurbulentDissipationRateFvPatchScalarField
        // onto a new patch
        atmosphericBoundaryLayerTurbulentDissipationRateFvPatchScalarField
        (
            const
            atmosphericBoundaryLayerTurbulentDissipationRateFvPatchScalarField&,
            const fvPatch&,
            const DimensionedField<scalar, fvMesh>&,
            const fieldMapper&
        );

        //- Construct as copy setting internal field reference
        atmosphericBoundaryLayerTurbulentDissipationRateFvPatchScalarField
        (
            const
            atmosphericBoundaryLayerTurbulentDissipationRateFvPatchScalarField&,
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
              atmosphericBoundaryLayerTurbulentDissipationRateFvPatchScalarField
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
