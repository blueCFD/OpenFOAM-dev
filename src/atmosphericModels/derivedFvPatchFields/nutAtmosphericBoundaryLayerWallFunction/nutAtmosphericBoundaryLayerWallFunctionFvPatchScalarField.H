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
    Foam::nutAtmosphericBoundaryLayerWallFunctionFvPatchScalarField

Description
    This boundary condition provides a turbulent kinematic viscosity for
    atmospheric velocity profiles.  It is designed to be used in conjunction
    with the atmosphericBoundaryLayerVelocity boundary condition.  The values
    are calculated using:

        \f[
            U = frac{U_f}{\kappa} ln(\frac{z + z_0}{z_0})
        \f]

    where

    \vartable
        U_f    | frictional velocity
        \kappa | Von Karman's constant
        z_0    | surface roughness length
        z      | vertical co-ordinate
    \endvartable

See also
    Foam::atmosphericBoundaryLayer
    Foam::atmosphericBoundaryLayerVelocityFvPatchVectorField
    Foam::nutkWallFunctionFvPatchField

SourceFiles
    nutAtmosphericBoundaryLayerWallFunctionFvPatchScalarField.C

\*---------------------------------------------------------------------------*/

#ifndef nutAtmosphericBoundaryLayerWallFunctionFvPatchScalarField_H
#define nutAtmosphericBoundaryLayerWallFunctionFvPatchScalarField_H

#include "nutkWallFunctionFvPatchScalarField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
         Class nutAtmosphericBoundaryLayerWallFunctionFvPatchScalarField
\*---------------------------------------------------------------------------*/

class nutAtmosphericBoundaryLayerWallFunctionFvPatchScalarField
:
    public nutkWallFunctionFvPatchScalarField
{
protected:

    // Protected Member Functions

        //- Calculate the turbulence viscosity
        virtual tmp<scalarField> nut() const;


public:

    //- Runtime type information
    TypeName("nutAtmosphericBoundaryLayerWallFunction");


    // Constructors

        //- Construct from patch, internal field and dictionary
        nutAtmosphericBoundaryLayerWallFunctionFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, fvMesh>&,
            const dictionary&
        );

        //- Construct by mapping given
        //  nutAtmosphericBoundaryLayerWallFunctionFvPatchScalarField
        //  onto a new patch
        nutAtmosphericBoundaryLayerWallFunctionFvPatchScalarField
        (
            const nutAtmosphericBoundaryLayerWallFunctionFvPatchScalarField&,
            const fvPatch&,
            const DimensionedField<scalar, fvMesh>&,
            const fieldMapper&
        );

        //- Disallow copy without setting internal field reference
        nutAtmosphericBoundaryLayerWallFunctionFvPatchScalarField
        (
            const nutAtmosphericBoundaryLayerWallFunctionFvPatchScalarField&
        ) = delete;

        //- Copy constructor setting internal field reference
        nutAtmosphericBoundaryLayerWallFunctionFvPatchScalarField
        (
            const nutAtmosphericBoundaryLayerWallFunctionFvPatchScalarField&,
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
                new nutAtmosphericBoundaryLayerWallFunctionFvPatchScalarField
                (
                    *this,
                    iF
                )
            );
        }


    // Member Functions

        // I-O

            //- Write
            virtual void write(Ostream&) const;


    // Member Operators

        //- Inherit assignment
        using nutkWallFunctionFvPatchScalarField::operator=;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
