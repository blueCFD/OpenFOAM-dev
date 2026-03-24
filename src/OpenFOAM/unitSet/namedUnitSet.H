/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2024-2026 OpenFOAM Foundation
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
    Foam::namedUnitSet

Description
    ...

SourceFiles
    ...

\*---------------------------------------------------------------------------*/

#ifndef namedUnitSet_H
#define namedUnitSet_H

#include "unitSet.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward declaration of classes
class Istream;
class Ostream;
class entry;

// Forward declaration of friend functions and operators
class namedUnitSet;
Istream& operator>>(Istream&, namedUnitSet&);
Ostream& operator<<(Ostream&, const namedUnitSet&);

/*---------------------------------------------------------------------------*\
                          Class namedUnitSet Declaration
\*---------------------------------------------------------------------------*/

class namedUnitSet
:
    public unitSet
{
private:

    // Private Data

        //- The name
        string name_;


    // Private Member Functions

        //- Read the name
        void readName(const entry& e);

        //- Read the name
        void readName(const word& keyword, const dictionary&);


public:

    // Static Data Members

        //- Run time type information
        ClassName("namedUnitSet");


    // Constructors

        //- Construct from name
        namedUnitSet(const string&);

        //- Copy constructor
        namedUnitSet(const namedUnitSet&) = default;

        //- Move constructor
        namedUnitSet(namedUnitSet&&) = default;


    // Member Functions

        //- Update
        void read(const word& keyword, const dictionary&);

        //- Update if found in the dictionary
        bool readIfPresent(const word& keyword, const dictionary&);


    // IOstream Operators

        //- Read from stream
        friend Istream& operator>>(Istream&, namedUnitSet&);

        //- Write to stream
        friend Ostream& operator<<(Ostream&, const namedUnitSet&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
