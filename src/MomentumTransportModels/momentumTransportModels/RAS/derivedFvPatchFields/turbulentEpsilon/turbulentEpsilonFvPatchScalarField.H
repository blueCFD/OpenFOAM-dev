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
    Foam::turbulentEpsilonFvPatchScalarField

Description
    This boundary condition provides a turbulence dissipation, \f$\epsilon\f$
    (epsilon) inlet condition based on a specified mixing length.  The patch
    values are calculated using:

        \f[
            \epsilon_p = \frac{C_{\mu}^{0.75} k^{1.5}}{L}
        \f]

    where

    \vartable
        \epsilon_p | patch epsilon values
        C_{\mu} | Model coefficient, set to 0.09
        k       | turbulence kinetic energy
        L       | length scale
    \endvartable

Usage
    \table
        Property     | Description             | Required    | Default value
        mixingLength | Length scale [m]        | yes         |
        phi          | flux field name         | no          | phi
        k            | turbulence kinetic energy field name | no | k
        Cmu          | Turbulence model coefficient | no | 0.09
    \endtable

    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type            turbulentEpsilon;
        mixingLength    0.005;
        value           uniform 200;   // placeholder
    }
    \endverbatim

    Note:
        In the event of reverse flow, a zero-gradient condition is applied

See also
    Foam::inletOutletFvPatchField

SourceFiles
    turbulentEpsilonFvPatchScalarField.C

\*---------------------------------------------------------------------------*/

#ifndef turbulentEpsilonFvPatchScalarField_H
#define turbulentEpsilonFvPatchScalarField_H

#include "inletOutletFvPatchFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
 Class turbulentEpsilonFvPatchScalarField Declaration
\*---------------------------------------------------------------------------*/

class turbulentEpsilonFvPatchScalarField
:
    public inletOutletFvPatchScalarField
{
    // Private Data

        //- Turbulent length scale
        scalar mixingLength_;

        //- Name of the turbulent kinetic energy field
        word kName_;

        //- Turbulence model coefficient
        scalar Cmu_;


public:

    //- Runtime type information
    TypeName("turbulentEpsilon");


    // Constructors

        //- Construct from patch, internal field and dictionary
        turbulentEpsilonFvPatchScalarField
        (
            const fvPatch&,
            const DimensionedField<scalar, fvMesh>&,
            const dictionary&
        );

        //- Construct by mapping given
        //  turbulentEpsilonFvPatchScalarField
        //  onto a new patch
        turbulentEpsilonFvPatchScalarField
        (
            const turbulentEpsilonFvPatchScalarField&,
            const fvPatch&,
            const DimensionedField<scalar, fvMesh>&,
            const fieldMapper&
        );

        //- Disallow copy without setting internal field reference
        turbulentEpsilonFvPatchScalarField
        (
            const turbulentEpsilonFvPatchScalarField&
        ) = delete;

        //- Copy constructor setting internal field reference
        turbulentEpsilonFvPatchScalarField
        (
            const turbulentEpsilonFvPatchScalarField&,
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
                new turbulentEpsilonFvPatchScalarField
                (
                    *this,
                    iF
                )
            );
        }


    // Member Functions

        //- Update the coefficients associated with the patch field
        virtual void updateCoeffs();

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
