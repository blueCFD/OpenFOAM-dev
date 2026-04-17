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

\*---------------------------------------------------------------------------*/

#include "printDefaults.H"
#include "dictionary.H"
#include "Pstream.T.H"

/* * * * * * * * * * * * * * * Static Member Data  * * * * * * * * * * * * * */

bool Foam::printDefaults::active_(false);
const Foam::dictionary* Foam::printDefaults::dictPtr_(nullptr);


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::printDefaults::printDefaults()
{
    if (Pstream::master())
    {
        active_ = true;
        Info<< incrIndent;
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::printDefaults::~printDefaults()
{
    if (Pstream::master())
    {
        active_ = false;
        dictPtr_ = nullptr;
        Info<< decrIndent;
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::printDefaults::set(const dictionary& dict)
{
    if (active_ && !printDefaults::dictPtr_)
    {
        dict.write(Info, false);
        printDefaults::dictPtr_ = &dict;
    }
}


// ************************************************************************* //
