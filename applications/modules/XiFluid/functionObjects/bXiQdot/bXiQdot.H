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
    Foam::functionObjects::bXiQdot

Description
    Calculates and caches the heat release rate bXiQdot
    of the Weller b-Xi combustion model.

    Example of function object specification:
    \verbatim
    bXiQdot
    {
        type        bXiQdot;
    }
    \endverbatim

Usage
    \table
        Property  | Description                 | Required  | Default value
        type      | type name: bXiQdot          | yes       |
    \endtable

    Or, using the standard configuration:
    \verbatim
    #includeFunc bXiQdot
    \endverbatim

See also
    Foam::functionObjects::fvMeshFunctionObject

SourceFiles
    bXiQdot.C

\*---------------------------------------------------------------------------*/

#ifndef bXiQdot_functionObject_H
#define bXiQdot_functionObject_H

#include "fvMeshFunctionObject.H"
#include "writeLocalObjects.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                       Class bXiQdot Declaration
\*---------------------------------------------------------------------------*/

class bXiQdot
:
    public fvMeshFunctionObject,
    public writeLocalObjects
{

public:

    //- Runtime type information
    TypeName("bXiQdot");


    // Constructors

        //- Construct from Time and dictionary
        bXiQdot
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );

        //- Disallow default bitwise copy construction
        bXiQdot(const bXiQdot&) = delete;


    //- Destructor
    virtual ~bXiQdot();


    // Member Functions

        //- Read the controls
        virtual bool read(const dictionary&);

        //- Return the list of fields required
        virtual wordList fields() const
        {
            return wordList::null();
        }

        //- Do not execute at the start of the run
        virtual bool executeAtStart() const
        {
            return false;
        }

        //- Do nothing
        virtual bool execute();

        //- Write the cell-centre fields
        virtual bool write();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace functionObjects
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
