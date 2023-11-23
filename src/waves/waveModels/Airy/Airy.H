/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2017-2023 OpenFOAM Foundation
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
    Foam::waveModels::Airy

Description
    First-order wave model.

    Reference:
    \verbatim
        Stokes, G.G. (1847)
        On the theory of oscillatory waves.
        Transactions of the Cambridge Philosophical Society, 8, 441.
    \endverbatim

    See the leading terms of equations 18 and 19.

Usage
    \table
        Property  | Description            | Required?         | Default
        depth     | The water depth [m]    | no                | great
        amplitude | The amplitude [m]      | yes               |
        length    | The wave length [m]    | if period not set |
        period    | The wave period [s]    | if length not set |
        phase     | The phase offset [rad] | yes               |
    \endtable

    Example specification in constant/waveProperties:
    \verbatim
    waves
    (
        Airy
        {
            length      40;
            amplitude   0.5;
            phase       0;
        }
    );
    \endverbatim

SourceFiles
    Airy.C

\*---------------------------------------------------------------------------*/

#ifndef Airy_H
#define Airy_H

#include "waveModel.H"
#include "AiryCoeffs.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace waveModels
{

/*---------------------------------------------------------------------------*\
                            Class Airy Declaration
\*---------------------------------------------------------------------------*/

class Airy
:
    public waveModel
{
    // Private Data

        //- Depth [m]
        const scalar depth_;

        //- Amplitude [m]
        autoPtr<Function1<scalar>> amplitude_;

        //- Wavelength [m]
        const scalar length_;

        //- Phase offset [rad]
        const scalar phase_;


    // Private Member Functions

        //- Read and return the wave length from the dictionary. Either reads
        //  the length directly, or reads the period and depth and calculates
        //  the length.
        static scalar readLength
        (
            const dictionary& dict,
            const scalar depth,
            const scalar amplitude,
            const scalar g,
            scalar (*celerityPtr)(const AiryCoeffs&)
        );


protected:

    // Protected Member Functions

        //- Return the wave coefficients
        AiryCoeffs coeffs(const scalar t) const;

        //- Return the wave coefficients at steady state
        AiryCoeffs coeffs() const;

        //- The wave celerity [m/s]
        static scalar celerity(const AiryCoeffs&);


public:

    //- Runtime type information
    TypeName("Airy");


    // Constructors

        //- Construct a copy
        Airy(const Airy& wave);

        //- Construct from a dictionary and gravity
        Airy
        (
            const dictionary& dict,
            const scalar g,
            const word& modelName = Airy::typeName,
            scalar (*celerityPtr)(const AiryCoeffs&) = &AiryCoeffs::celerity
        );

        //- Construct a clone
        virtual autoPtr<waveModel> clone() const
        {
            return autoPtr<waveModel>(new Airy(*this));
        }


    //- Destructor
    virtual ~Airy();


    // Member Functions

        // Access

            //- Get the depth
            scalar depth() const;

            //- Get the amplitude
            scalar amplitude(const scalar t) const;

            //- Get the amplitude at steady state
            scalar amplitude() const;

            //- Get the length
            scalar length() const;

            //- Get the phase
            scalar phase() const;


        //- The wave celerity [m/s]
        virtual scalar celerity() const;

        //- Get the wave elevation at a given time and local coordinates. Local
        //  x is aligned with the direction of propagation.
        virtual tmp<scalarField> elevation
        (
            const scalar t,
            const scalarField& x
        ) const;

        //- Get the wave velocity at a given time and local coordinates. Local
        //  x is aligned with the direction of propagation, and z with negative
        //  gravity.
        virtual tmp<vector2DField> velocity
        (
            const scalar t,
            const vector2DField& xz
        ) const;

        //- Write
        virtual void write(Ostream& os) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace waveModels
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "AiryI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
