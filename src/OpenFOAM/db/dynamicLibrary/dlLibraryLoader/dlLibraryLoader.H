/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2011-2020 OpenFOAM Foundation
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
    Foam::dlLibraryLoader

Description
    Slightly based on dlLibraryLoader, this class is meant to be used for
    forcefully loading libraries needed on Windows.

SourceFiles
    dlLibraryLoader.C

\*---------------------------------------------------------------------------*/

#ifndef dlLibraryLoader_H
#define dlLibraryLoader_H

#include "dlLibraryTable.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                       Class dlLibraryLoader Declaration
\*---------------------------------------------------------------------------*/

class dlLibraryLoader
{
public:

    // Declare name of the class and its debug switch
    ClassName("dlLibraryLoader");


    // Constructors

        //- Construct from comma separated string list of libraries
        dlLibraryLoader(const fileName& libraryList);

        //- Disallow default bitwise copy construction
        dlLibraryLoader(const dlLibraryLoader&) = delete;


    //- Destructor
    ~dlLibraryLoader();
    
    // Methos
        // Dummy method for skipping warning about unused variable
        void dummy() {}
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
