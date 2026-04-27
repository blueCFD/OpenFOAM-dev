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


// * * * * * * * * * * * Private Static Member Functions * * * * * * * * * * //

void Foam::printDictionary::setDefaults(const dictionary& dict)
{
    dictPtrToDefaults_.set
    (
        &dict,
        tmpNrc<dictionary>(new dictionary(dict.parent(), dictionary()))
    );

    setSubDefaults(dict);
}


void Foam::printDictionary::setSubDefaults(const dictionary& dict)
{
    dictionary& defaults = printDictionary::defaults(dict);

    forAllConstIter(dictionary, dict, iter)
    {
        if (!iter().isDict()) continue;

        const dictionary& subDict = iter().dict();

        defaults.set(iter().keyword(), dictionary());

        const dictionary& subDefaults = defaults.subDict(iter().keyword());

        dictPtrToDefaults_.set
        (
            &subDict,
            tmpNrc<dictionary>(subDefaults)
        );

        setSubDefaults(subDict);
    }
}


void Foam::printDictionary::print
(
    const dictionary& dict,
    const dictionary& defaults
)
{
    Info<< nl << indent << token::BEGIN_BLOCK << nl << incrIndent;

    forAllConstIter(dictionary, dict, iter)
    {
        if (!iter().isDict())
        {
            Info<< iter();
        }
        else
        {
            Info<< indent << iter().keyword();
            print(iter().dict(), defaults.subDict(iter().keyword()));
        }
    }

    label nDefaultsEntries = 0;
    forAllConstIter(dictionary, defaults, iter)
    {
        nDefaultsEntries += !iter().isDict();
    }

    if (nDefaultsEntries) Info<< indent << "/* Defaults */" << endl;

    forAllConstIter(dictionary, defaults, iter)
    {
        if (!iter().isDict())
        {
            Info<< iter();
        }
    }

    Info<< decrIndent << indent << token::END_BLOCK << nl;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::printDictionary::printDictionary(const dictionary& dict)
:
    dictPtr_(&dict),
    dictName_(fileName::null)
{
    if (dictNameToDictPtrs_.found(dict.name()))
    {
        setDefaults(dict);
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

    if (dictPtr && dictPtrToDefaults_[dictPtr].isTmp())
    {
        Info<< indent << dictPtr->name().relativePath().c_str();

        print(*dictPtr, dictPtrToDefaults_[dictPtr]());

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
        setDefaults(dict);
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
