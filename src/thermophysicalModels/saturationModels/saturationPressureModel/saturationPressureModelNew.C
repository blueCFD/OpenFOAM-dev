/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2015-2026 OpenFOAM Foundation
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

#include "constantPressure.H"

// * * * * * * * * * * * * * * * * Selector  * * * * * * * * * * * * * * * * //

Foam::autoPtr<Foam::saturationPressureModel>
Foam::saturationPressureModel::New
(
    const word& name,
    const dictionary& dict
)
{
    auto unknown = [&](const word& modelTypeName)
    {
        FatalIOErrorInFunction(dict)
            << "Unknown " << typeName << " type "
            << modelTypeName << endl << endl
            << "Valid " << typeName << " types are : " << endl
            << dictionaryConstructorTablePtr_->sortedToc()
            << exit(FatalIOError);
    };

    const bool isDict = dict.isDict(name);

    if (!isDict)
    {
        token firstToken(dict.lookup(name, false));

        if (!firstToken.isWord())
        {
            return autoPtr<saturationPressureModel>
            (
                new saturationModels::constantPressure
                (
                    dimensionedScalar(name, dimPressure, dict)
                )
            );
        }

        const word modelTypeName = firstToken.wordToken();

        // !!! Assume all models are on the dictionary selection table
        dictionaryConstructorTable::iterator dictCstrIter =
            dictionaryConstructorTablePtr_->find(modelTypeName);
        if (dictCstrIter == dictionaryConstructorTablePtr_->end())
        {
            unknown(modelTypeName);
        }

        ConstructorTable::iterator cstrIter =
            ConstructorTablePtr_->find(modelTypeName);
        if (cstrIter == ConstructorTablePtr_->end())
        {
            FatalIOErrorInFunction(dict)
                << name << " must be a sub-dictionary in order to select a "
                << typeName << " of type " << modelTypeName
                << exit(FatalIOError);
        }

        return cstrIter()();
    }

    const dictionary& modelDict = dict.subDict(name);

    const word modelTypeName = modelDict.lookup<word>("type");

    const dictionary& modelCoeffsDict =
        modelDict.optionalTypeDict(modelTypeName);

    Info<< indentOrNl
        << "Selecting " << typeName << " " << modelTypeName << endl;

    dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(modelTypeName);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        unknown(modelTypeName);
    }

    printDictionary print(modelDict, modelCoeffsDict);

    return cstrIter()(modelCoeffsDict);
}


// ************************************************************************* //
