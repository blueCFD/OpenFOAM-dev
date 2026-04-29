/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2021-2026 OpenFOAM Foundation
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

#include "momentumTransportModel.H"
#include "printDictionary.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class MomentumTransportModel>
inline Foam::autoPtr<MomentumTransportModel> Foam::momentumTransportModel::New
(
    const typename MomentumTransportModel::alphaField& alpha,
    const typename MomentumTransportModel::rhoField& rho,
    const volVectorField& U,
    const surfaceScalarField& alphaRhoPhi,
    const surfaceScalarField& phi,
    const viscosity& viscosity
)
{
    IOobject modelDictIO =
        momentumTransportModel::readModelDict
        (
            U.db(),
            alphaRhoPhi.group()
        );

    const word modelType =
        IOdictionary(modelDictIO).lookup<word>("simulationType");

    Info<< indentOrNl << "Selecting momentum transport model type "
        << modelType << endl;

    typename MomentumTransportModel::dictionaryConstructorTable::iterator
        cstrIter =
        MomentumTransportModel::dictionaryConstructorTablePtr_->find(modelType);

    if
    (
        cstrIter
     == MomentumTransportModel::dictionaryConstructorTablePtr_->end()
    )
    {
        FatalErrorInFunction
            << "Unknown " << MomentumTransportModel::typeName << " type "
            << modelType << nl << nl
            << "Valid " << MomentumTransportModel::typeName << " types:" << endl
            << MomentumTransportModel::dictionaryConstructorTablePtr_
               ->sortedToc()
            << exit(FatalError);
    }

    printDictionary print(modelDictIO.objectPath(true));

    return cstrIter()(alpha, rho, U, alphaRhoPhi, phi, viscosity);
}


// ************************************************************************* //
