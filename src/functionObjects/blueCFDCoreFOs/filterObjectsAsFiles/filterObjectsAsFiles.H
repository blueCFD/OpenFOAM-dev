/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2011-2022 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
2024 FS Dynamics Portugal: Changes are tracked at:
                 https://github.com/blueCFD/OpenFOAM-dev
------------------------------------------------------------------------------
License
    This file is a derivative work of OpenFOAM.

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
    Foam::functionObjects::filterObjectsAsFiles

Description
    Allows defining the renaming patterns for files on MS Windows.
    This grants better control over the file names to be used by solvers, e.g.:
      * create files with colons in their names,
      * avoid file collisions between lower and upper case letters, e.g. 'b' vs
        'B'.

    The renaming patterns are set during construction of this function object,
    so that they are ready as soon as possible.

    Example of function object specification:
    \verbatim
    filterObjectsAsFiles1
    {
        type            filterObjectsAsFiles;

        libs            ("libblueCFDCoreFOs.so");

        pattern
        (
            ("B" "upper_B")
            (":" "_collon_")
        );
    }
    \endverbatim

Usage
    \table
        Property     | Description             | Required     | Default value
        type         | type name: filterObjectsAsFiles | yes          |
        pattern      | renaming pattern using regex        | yes          |
    \endtable

    Note: Regular expressions are usable in pattern.

See also
    Foam::functionObject

SourceFiles
    filterObjectsAsFiles.C

\*---------------------------------------------------------------------------*/

#ifndef functionObjects_filterObjectsAsFiles_H
#define functionObjects_filterObjectsAsFiles_H

#include "functionObject.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                    Class filterObjectsAsFiles Declaration
\*---------------------------------------------------------------------------*/

class filterObjectsAsFiles
:
    public functionObject
{
private:

    // Private Data

        //- List of renaming patterns retrieved from dict
        List<Pair<word>> patterns_;


    // Private Member Functions

        //- Transfer the patterns onto the manager
        void transferPatterns();


public:

    //- Runtime type information
    TypeName("filterObjectsAsFiles");


    // Constructors

        //- Construct from Time and dictionary
        filterObjectsAsFiles
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );

        //- Disallow default bitwise copy construction
        filterObjectsAsFiles(const filterObjectsAsFiles&) = delete;


    //- Destructor
    virtual ~filterObjectsAsFiles();


    // Member Functions

        //- Read the filterObjectsAsFiles data
        virtual bool read(const dictionary&);

        //- Return the list of fields required
        virtual wordList fields() const
        {
            return wordList::null();
        }

        //- Do nothing
        virtual bool execute();

        //- Do nothing
        virtual bool write();


    // Member Operators

        //- Disallow default bitwise assignment
        void operator=(const filterObjectsAsFiles&) = delete;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace functionObjects
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
