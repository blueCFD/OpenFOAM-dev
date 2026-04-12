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
    Foam::DimensionedFieldFunctions::DistanceFunction

Description
    Distance function internal and patch field initialisation function

    The field is initialised by evaluating the specified
    Foam::Function1 of distance in the given spatial direction.

Usage
    To set a field with a linear ramp from 0 to 1 over the distance 1 starting
    at 0 in the x-direction:
    \verbatim
    internalField
    {
        type        distanceFunction;

        direction   (1 0 0);

        function
        {
            type        linearRamp;
            duration    1;
        }
    }
    \endverbatim

SourceFiles
    DistanceFunction_DimensionedFieldFunction.C

\*---------------------------------------------------------------------------*/

#ifndef DistanceFunction_DimensionedFieldFunction_H
#define DistanceFunction_DimensionedFieldFunction_H

#include "DimensionedFieldFunction.T.H"
#include "Function1.T.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace DimensionedFieldFunctions
{

/*---------------------------------------------------------------------------*\
                            Class DistanceFunction Declaration
\*---------------------------------------------------------------------------*/

template<class DimensionedFieldType>
class DistanceFunction
:
    public DimensionedFieldFunction<DimensionedFieldType>
{
    // Private Data

        //- Distance DistanceFunction pointer
        autoPtr<Foam::Function1<typename DimensionedFieldType::Type_>> funcPtr_;

        //- Direction the DistanceFunction is evaluated for
        vector direction_;


public:

    //- Runtime type information
    TypeName("distanceFunction");


    // Constructors

        //- Construct with dictionary to initialise given field
        DistanceFunction
        (
            const dictionary& dict,
            DimensionedFieldType& field
        );

        //- Construct a copy for the given field
        DistanceFunction
        (
            const DistanceFunction& dff,
            DimensionedFieldType& field
        );

        //- Construct and return a clone for the specified field
        virtual autoPtr<DimensionedFieldFunction<DimensionedFieldType>>
        clone(DimensionedFieldType& field) const;


    //- Destructor
    virtual ~DistanceFunction()
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
#include "DistanceFunction_DimensionedFieldFunction.T.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
