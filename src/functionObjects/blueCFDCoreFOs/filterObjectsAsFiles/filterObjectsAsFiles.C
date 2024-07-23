/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2011-2023 OpenFOAM Foundation
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

#include "filterObjectsAsFiles.H"
#include "Time.T.H"
#include "polyMesh.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{
    defineTypeNameAndDebug(filterObjectsAsFiles, 0);

    addToRunTimeSelectionTable
    (
        functionObject,
        filterObjectsAsFiles,
        dictionary
    );
}
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void Foam::functionObjects::filterObjectsAsFiles::transferPatterns()
{
    forAll (patterns_, index)
    {
        IOobject::replaceFileName
        (
            patterns_[index].first(), patterns_[index].second()
        );
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::functionObjects::filterObjectsAsFiles::filterObjectsAsFiles
(
    const word& name,
    const Time& runTime,
    const dictionary& dict
)
:
    functionObject(name, runTime),
    patterns_()
{
    read(dict);
    
    transferPatterns();
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::functionObjects::filterObjectsAsFiles::~filterObjectsAsFiles()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool Foam::functionObjects::filterObjectsAsFiles::read(const dictionary& dict)
{

    patterns_ = List<Pair<wordRe>>
    (
        dict.lookup("patterns")
    );

    return functionObject::read(dict);
}


bool Foam::functionObjects::filterObjectsAsFiles::execute()
{
    return true;
}


bool Foam::functionObjects::filterObjectsAsFiles::write()
{
    return true;
}


// ************************************************************************* //
