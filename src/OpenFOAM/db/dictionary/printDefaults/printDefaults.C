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

Foam::printDefaults* Foam::printDefaults::printDefaults_(nullptr);
Foam::printDefaults::dictHashTable Foam::printDefaults::dicts_;


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::printDefaults::printDefaults(const bool parent)
:
    parent_(parent),
    dictPtr_(nullptr)
{
    printDefaults_ = this;

    if (Pstream::master())
    {
        Info<< incrIndent;
    }
}


Foam::printDefaults::printDefaults(const dictionary& subDict)
:
    parent_(false),
    dictPtr_(nullptr)
{
    if (dicts_.found(&subDict.parent()))
    {
        printDefaults_ = this;
        set(subDict);
    }

    if (Pstream::master())
    {
        Info<< incrIndent;
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::printDefaults::~printDefaults()
{
    printDefaults_ = nullptr;

    if (Pstream::master())
    {
        if (dictPtr_)
        {
            if (!parent_)
            {
                dicts_[dictPtr_].write(Info, false);
            }
            dicts_.erase(dictPtr_);
        }
        Info<< decrIndent;
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::printDefaults::set(const dictionary& dict)
{
    if (printDefaults_)
    {
        printDefaults_->dictPtr_ = &dict;
        dicts_.insert
        (
            printDefaults_->dictPtr_,
            new dictionary(dict.parent(), dict)
        );
        printDefaults_ = nullptr;
    }
}


// ************************************************************************* //
