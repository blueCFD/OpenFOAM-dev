/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2014-2020 OpenFOAM Foundation
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

#include "CompressibleMomentumTransportModel.T.H"
#include "incompressibleTwoPhaseInteractingMixture.H"
#include "addToRunTimeSelectionTable.H"
#include "makeMomentumTransportModel.H"

#include "laminarModel.H"
#include "RASModel.T.H"
#include "LESModel.T.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

makeMomentumTransportModelTypes
(
    geometricOneField,
    volScalarField,
    compressibleMomentumTransportModel,
    CompressibleMomentumTransportModel,
    incompressibleTwoPhaseInteractingMixture
);

makeBaseMomentumTransportModel
(
    geometricOneField,
    volScalarField,
    compressibleMomentumTransportModel,
    CompressibleMomentumTransportModel,
    incompressibleTwoPhaseInteractingMixture
);

#define makeLaminarModel(Type)                                                 \
    makeTemplatedMomentumTransportModel                                        \
    (                                                                          \
        incompressibleTwoPhaseInteractingMixture                               \
        ##CompressibleMomentumTransportModel,                                  \
        laminar,                                                               \
        Type                                                                   \
    )

#define makeRASModel(Type)                                                     \
    makeTemplatedMomentumTransportModel                                        \
    (                                                                          \
        incompressibleTwoPhaseInteractingMixture                               \
        ##CompressibleMomentumTransportModel,                                  \
        RAS,                                                                   \
        Type                                                                   \
    )

#define makeLESModel(Type)                                                     \
    makeTemplatedMomentumTransportModel                                        \
    (                                                                          \
        incompressibleTwoPhaseInteractingMixture                               \
        ##CompressibleMomentumTransportModel,                                  \
        LES,                                                                   \
        Type                                                                   \
    )

#include "Stokes.T.H"
makeLaminarModel(Stokes);

#include "kEpsilon.H"
makeRASModel(kEpsilon);

#include "buoyantKEpsilon.H"
makeRASModel(buoyantKEpsilon);

#include "Smagorinsky.T.H"
makeLESModel(Smagorinsky);

#include "kEqn.H"
makeLESModel(kEqn);

#include "LRR.T.H"
makeRASModel(LRR);

#include "SSG.T.H"
makeRASModel(SSG);


// ************************************************************************* //
