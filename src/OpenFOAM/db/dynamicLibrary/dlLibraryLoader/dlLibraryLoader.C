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

\*---------------------------------------------------------------------------*/

#include "dlLibraryLoader.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(dlLibraryLoader, 0);
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::dlLibraryLoader::dlLibraryLoader(const fileName& libraryList)
{
    string libsToLoad(libraryList);

    libsToLoad.removeTrailing(' '); //removes spaces from both ends
    libsToLoad.removeRepeated(',');
    libsToLoad.removeTrailing(',');
    libsToLoad += ','; //so that we know where to stop

    if (debug)
    {
        InfoInFunction
            << "Libraries to be loaded: " <<  libsToLoad << endl;
    }

    //parse the comma separated list
    size_t stposstr=0, found=libsToLoad.find_first_of(',');
    while (found!=string::npos)
    {
        string libToLoad = libsToLoad.substr(stposstr,found-stposstr);

        if (libs.open(libToLoad) && debug)
        {
            InfoInFunction
                << "Library loaded with success: " <<  libToLoad << endl;
        }

        stposstr=found+1; found=libsToLoad.find_first_of(',',stposstr);
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::dlLibraryLoader::~dlLibraryLoader()
{
}

// ************************************************************************* //
