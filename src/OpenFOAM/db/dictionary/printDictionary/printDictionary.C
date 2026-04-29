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
#include "stringOps.H"


// * * * * * * * * * * * Private Static Member Functions * * * * * * * * * * //

void Foam::printDictionary::setDefaults(const dictionary& dict)
{
    dictPtrToDefaults_.set
    (
        &dict,
        tmpNrc<dictionary>(new dictionary(dict.parent(), dictionary()))
    );

    setSubDefaults(dict, printDictionary::defaults(dict));
}


void Foam::printDictionary::setSubDefaults
(
    const dictionary& dict,
    dictionary& defaults
)
{
    forAllConstIter(dictionary, dict, iter)
    {
        if (!iter().isDict()) continue;

        const dictionary& subDict = iter().dict();

        defaults.set(iter().keyword(), dictionary());

        dictionary& subDefaults = defaults.subDict(iter().keyword());

        if (!dictPtrToDefaults_.found(&subDict))
        {
            dictPtrToDefaults_.set
            (
                &subDict,
                tmpNrc<dictionary>(subDefaults)
            );
        }

        setSubDefaults(subDict, subDefaults);
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
        else if (defaults.isDict(iter().keyword()))
        {
            Info<< indent << iter().keyword();
            print(iter().dict(), defaults.subDict(iter().keyword()));
        }
        else
        {
            Info<< indent << iter().keyword()
                << nl << indent << token::BEGIN_BLOCK << nl << incrIndent;

            iter().dict().write(Info, false);

            OStringStream oss;
            oss << "/* #print must be specified after the " << iter().keyword()
                << " sub-dictionary in order for its defaults to be printed */";

            Info<<
                stringOps::breakIntoIndentedLines
                (
                    oss.str(),
                    80,
                    Info().indentSize()
                ).c_str() << endl
                << decrIndent << indent << token::END_BLOCK << nl;
        }
    }

    label nDefaultsEntries = 0;
    forAllConstIter(dictionary, defaults, iter)
    {
        nDefaultsEntries += !iter().isDict();
    }

    if (nDefaultsEntries) Info<< indent << "/* defaults */" << endl;

    forAllConstIter(dictionary, defaults, iter)
    {
        if (!iter().isDict())
        {
            Info<< iter();
        }
    }

    Info<< decrIndent << indent << token::END_BLOCK << nl;
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void Foam::printDictionary::add(const dictionary& dict)
{
    if (findIndex(dicts_, &dict) != -1) return;

    dicts_.append(&dict);
    dictNames_.append(fileName::null);

    if
    (
        dictNameToDictPtrs_.found(dict.name())
     && dictNameToDictPtrs_[dict.name()] != nullptr
    )
    {
        setDefaults(dict);
    }
}


void Foam::printDictionary::add(const fileName& dictName)
{
    if (findIndex(dictNames_, dictName) != -1) return;

    dicts_.append(nullptr);
    dictNames_.append(dictName);

    dictNameToDictPtrs_.set(dictName, nullptr);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::printDictionary::printDictionary()
:
    dicts_(),
    dictNames_()
{
    Info<< incrIndent;
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::printDictionary::~printDictionary()
{
    forAll(dicts_, i)
    {
        const dictionary* dictPtr =
            dicts_.set(i)
         && dictPtrToDefaults_.found(&dicts_[i])
          ? &dicts_[i]
          : dictNames_[i] != fileName::null
         && dictNameToDictPtrs_.found(dictNames_[i])
         && dictPtrToDefaults_.found(dictNameToDictPtrs_[dictNames_[i]])
          ? dictNameToDictPtrs_[dictNames_[i]]
          : nullptr;

        if (dictPtr && dictPtrToDefaults_[dictPtr].isTmp())
        {
            Info<< indent << dictPtr->name().relativePath().c_str();

            print(*dictPtr, dictPtrToDefaults_[dictPtr]());

            dictPtrToDefaults_.erase(dictPtr);
            dictNameToDictPtrs_.erase(dictPtr->name());
        }
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
