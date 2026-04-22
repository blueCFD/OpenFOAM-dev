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

#include "printDictionary.H"
#include "dictionary.H"

/* * * * * * * * * * * * * * * Static Member Data  * * * * * * * * * * * * * */

namespace Foam
{
    HashTable<const dictionary*, fileName, Hash<fileName>>
        printDictionary::dictNameToDictPtrs_;

    HashPtrTable<dictionary, const dictionary*, Hash<void*>>
        printDictionary::dictPtrToDefaults_;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::printDictionary::printDictionary(const dictionary& dict)
:
    dictPtr_(&dict),
    dictName_(fileName::null)
{
    if (dictNameToDictPtrs_.found(dict.name()))
    {
        dictPtrToDefaults_.set
        (
            &dict,
            new dictionary(dict.parent(), dictionary())
        );
    }

    Info<< incrIndent;
}


Foam::printDictionary::printDictionary(const fileName& dictName)
:
    dictPtr_(nullptr),
    dictName_(dictName)
{
    dictNameToDictPtrs_.set(dictName, nullptr);

    Info<< incrIndent;
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::printDictionary::~printDictionary()
{
    const dictionary* dictPtr =
        dictPtr_ && dictPtrToDefaults_.found(dictPtr_)
      ? dictPtr_
      : dictName_ != fileName::null && dictNameToDictPtrs_[dictName_]
      ? dictNameToDictPtrs_[dictName_]
      : nullptr;

    if (dictPtr)
    {
        Info<< indent << dictPtr->dictName() << endl
            << indent << token::BEGIN_BLOCK << nl << incrIndent;

        auto print = [](const dictionary& dict)
        {
            forAllConstIter(dictionary, dict, iter)
            {
                if (iter().isDict())
                {
                    writeKeyword(Info, iter().keyword());
                    Info<< "{ ... }" << nl;
                }
                else
                {
                    Info<< iter();
                }
            }
        };

        Info<< indent << "// Specified" << endl;
        print(*dictPtr);
        Info<< indent << "// Defaulted" << endl;
        print(dictPtrToDefaults_[dictPtr]);

        Info<< decrIndent << indent << token::END_BLOCK << nl;

        dictPtrToDefaults_.erase(dictPtr);
        dictNameToDictPtrs_.erase(dictPtr->name());
    }

    Info<< decrIndent;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::printDictionary::set(const dictionary& dict)
{
    if
    (
        dictNameToDictPtrs_.found(dict.name())
     && dictNameToDictPtrs_[dict.name()] != &dict
    )
    {
        dictPtrToDefaults_.set
        (
            &dict,
            new dictionary(dict.parent(), dictionary())
        );
    }

    dictNameToDictPtrs_.set(dict.name(), &dict);
}


void Foam::printDictionary::unset(const dictionary& dict)
{
    if
    (
        dictNameToDictPtrs_.found(dict.name())
     && dictNameToDictPtrs_[dict.name()] == &dict
    )
    {
        dictNameToDictPtrs_.erase(dict.name());
    }

    if (dictPtrToDefaults_.found(&dict))
    {
        dictPtrToDefaults_.erase(&dict);
    }
}


// ************************************************************************* //
