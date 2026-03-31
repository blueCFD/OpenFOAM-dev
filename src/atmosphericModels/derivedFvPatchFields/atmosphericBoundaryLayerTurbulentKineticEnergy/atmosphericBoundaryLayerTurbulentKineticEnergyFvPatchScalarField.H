/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2014-2026 OpenFOAM Foundation
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
    Foam::atmosphericBoundaryLayerTurbulentKineticEnergyFvPatchScalarField

Description
    This boundary condition specifies an inlet value for the turbulent
    kinetic energy, \f$k\f$, for an atmospheric boundary layer.

    See Foam::atmosphericBoundaryLayer for details.

See also
    Foam::atmosphericBoundaryLayer
    Foam::atmosphericBoundaryLayerVelocityFvPatchVectorField
    Foam::atmosphericBoundaryLayerTurbulentEpsilonFvPatchScalarField

SourceFiles
    atmosphericBoundaryLayerTurbulentKineticEnergyFvPatchScalarField.C

\*---------------------------------------------------------------------------*/

#ifndef atmosphericBoundaryLayerTurbulentKineticEnergyFvPatchScalarField_H
#define atmosphericBoundaryLayerTurbulentKineticEnergyFvPatchScalarField_H

#include "fvPatchFields.H"
#include "inletOutletFvPatchFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
    Class atmosphericBoundaryLayerTurbulentKineticEnergyFvPatchScalarField
\*---------------------------------------------------------------------------*/

class atmosphericBoundaryLayerTurbulentKineticEnergyFvPatchScalarField
:
    public inletOutletFvPatchScalarField
{

public:

    //- Runtime type information
    TypeName("atmosphericBoundaryLayerTurbulentKineticEnergy");


    // Constructors

        //- Construct from patch, internal field and dictionary
        atmosphericBoundaryLayerTurbulentKineticEnergyFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, fvMesh>&,
            const dictionary&
        );

        //- Construct by mapping given
        //  atmosphericBoundaryLayerTurbulentKineticEnergyFvPatchScalarField
        //  onto a new patch
        atmosphericBoundaryLayerTurbulentKineticEnergyFvPatchScalarField
        (
            const
            atmosphericBoundaryLayerTurbulentKineticEnergyFvPatchScalarField&,
            const fvPatch&,
            const DimensionedField<scalar, fvMesh>&,
            const fieldMapper&
        );

        //- Construct as copy setting internal field reference
        atmosphericBoundaryLayerTurbulentKineticEnergyFvPatchScalarField
        (
            const
            atmosphericBoundaryLayerTurbulentKineticEnergyFvPatchScalarField&,
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
                atmosphericBoundaryLayerTurbulentKineticEnergyFvPatchScalarField
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
