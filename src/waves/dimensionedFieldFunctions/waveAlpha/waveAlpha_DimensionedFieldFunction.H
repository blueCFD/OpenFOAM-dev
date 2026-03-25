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
    Foam::DimensionedFieldFunctions::waveAlpha

Description
    Initialises the phase fraction to that specified by a superposition of wave
    models. All the wave modelling parameters are obtained from a centrally
    registered waveSuperposition class.

Usage
    \table
        Property  | Description                            | Required? | Default
        liquid    | Is the alpha field that of the liquid? | no        | true
    \endtable

    Example of the boundary condition specification:
    \verbatim
    internalField
    {
        type        waveAlpha;
        libs        ("libwaves.so");
    }
    \endverbatim

See also
    Foam::waveAlphaFvPatchScalarField
    Foam::waveSuperposition

SourceFiles
    waveAlpha_DimensionedFieldFunction.C

\*---------------------------------------------------------------------------*/

#ifndef waveAlpha_DimensionedFieldFunction_H
#define waveAlpha_DimensionedFieldFunction_H

#include "DimensionedFieldFunction.T.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

class fvMesh;

namespace DimensionedFieldFunctions
{

/*---------------------------------------------------------------------------*\
                            Class waveAlpha Declaration
\*---------------------------------------------------------------------------*/

class waveAlpha
:
    public DimensionedFieldFunction<DimensionedField<scalar, fvMesh>>
{
    // Private Data

        //- Is this alpha field that of the liquid under the wave?
        const Switch liquid_;


public:

    //- Runtime type information
    TypeName("waveAlpha");


    // Constructors

        //- Construct with dictionary to initialise given field
        waveAlpha
        (
            const dictionary& dict,
            DimensionedField<scalar, fvMesh>& field
        );

        //- Construct a copy for the given field
        waveAlpha
        (
            const waveAlpha& dff,
            DimensionedField<scalar, fvMesh>& field
        );

        //- Construct and return a clone for the specified field
        virtual autoPtr
        <
            DimensionedFieldFunction<DimensionedField<scalar, fvMesh>>
        > clone(DimensionedField<scalar, fvMesh>& field) const;


    //- Destructor
    virtual ~waveAlpha()
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
