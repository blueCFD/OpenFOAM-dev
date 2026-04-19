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
    Foam::printDefaults

Description
    Enables the printing of a dictionary
    and subsequently looked-up defaulted entries

SourceFiles
    printDefaults.C

\*---------------------------------------------------------------------------*/

#ifndef printDefaults_H
#define printDefaults_H

#include "HashPtrTable.T.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

class dictionary;

/*---------------------------------------------------------------------------*\
                          Class printDefaults Declaration
\*---------------------------------------------------------------------------*/

class printDefaults
{
public:

    // Public Typedefs

        typedef HashPtrTable<dictionary, const dictionary*, Hash<void*>>
            dictHashTable;


    // Private Static Data

        //- Pointer to most recently constructed printDefaults
        static printDefaults* printDefaults_;

        //- Table of dictionaries containing all entries including defaults
        static dictHashTable dicts_;

        //- Pointer to the associated dictionary
        //  Set by printDefaults::set called by entry #printDefaults
        const dictionary* dictPtr_;


public:

    // Constructors

        //- Construct null
        printDefaults();


    //- Destructor
    ~printDefaults();


    // Member Functions

        //- Set the dictionary to be printed
        static void set(const dictionary& dict);

        //- Return true if printing of specified dictionary is enabled
        static bool print(const dictionary& dict)
        {
            return dicts_.found(&dict);
        }

        //- Return true if printing of specified dictionary is enabled
        static dictionary& add(const dictionary& dict)
        {
            return *dicts_[&dict];
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
