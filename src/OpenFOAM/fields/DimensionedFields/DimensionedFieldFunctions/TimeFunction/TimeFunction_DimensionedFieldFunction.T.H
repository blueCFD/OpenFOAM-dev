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
    Foam::DimensionedFieldFunctions::TimeFunction

Description
    Time dependent internal and patch field initialisation and update function

    The field is initialised by evaluating the specified Foam::Function1
    for the current time.

Usage
    To coded pulsed inlet using the \c functionalFixedValue boundary condition:
    \verbatim
    inlet
    {
        type            functionalFixedValue;

        value
        {
            type        timeFunction;

            function
            {
                type        coded;

                name        pulse;

                codeInclude
                #{
                    #include "mathematicalConstants.H"
                #};

                code
                #{
                    return vector
                    (
                        0.5
                       *(1 - cos(constant::mathematical::twoPi*min(x/0.3, 1))),
                        0,
                        0
                    );
                #};
            }
        }
    }
    \endverbatim

SourceFiles
    TimeFunction_DimensionedFieldFunction.C

\*---------------------------------------------------------------------------*/

#ifndef TimeFunction_DimensionedFieldFunction_H
#define TimeFunction_DimensionedFieldFunction_H

#include "DimensionedFieldFunction.T.H"
#include "Function1.T.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace DimensionedFieldFunctions
{

/*---------------------------------------------------------------------------*\
                            Class TimeFunction Declaration
\*---------------------------------------------------------------------------*/

template<class DimensionedFieldType>
class TimeFunction
:
    public DimensionedFieldFunction<DimensionedFieldType>
{
    // Private Data

        //- Time Function1 pointer
        autoPtr<Foam::Function1<typename DimensionedFieldType::Type_>> funcPtr_;


public:

    //- Runtime type information
    TypeName("timeFunction");


    // Constructors

        //- Construct with dictionary to initialise given field
        TimeFunction
        (
            const dictionary& dict,
            DimensionedFieldType& field
        );

        //- Construct a copy for the given field
        TimeFunction
        (
            const TimeFunction& dff,
            DimensionedFieldType& field
        );

        //- Construct and return a clone for the specified field
        virtual autoPtr<DimensionedFieldFunction<DimensionedFieldType>>
        clone(DimensionedFieldType& field) const;


    //- Destructor
    virtual ~TimeFunction()
    {}


    // Member Functions

        //- Evaluate the function and set the field
        virtual void evaluate();

        //- Update the field for the current time
        virtual bool update();

        //- Write data to dictionary stream
        virtual void write(Ostream& os) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace DimensionedFieldFunctions
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#include "TimeFunction_DimensionedFieldFunction.T.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
