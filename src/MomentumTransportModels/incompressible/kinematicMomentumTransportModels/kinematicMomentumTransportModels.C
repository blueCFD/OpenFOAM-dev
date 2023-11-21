/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2013-2021 OpenFOAM Foundation
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

#include "kinematicMomentumTransportModels.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

makeBaseMomentumTransportModel
(
    geometricOneField,
    geometricOneField,
    incompressibleMomentumTransportModel,
    IncompressibleMomentumTransportModel,
    kinematicTransportModel
);


// -------------------------------------------------------------------------- //
// Laminar models
// -------------------------------------------------------------------------- //

#include "Stokes.T.H"
makeLaminarModel(Stokes);

#include "generalisedNewtonian.H"
makeLaminarModel(generalisedNewtonian);

#include "lambdaThixotropic.H"
makeLaminarModel(lambdaThixotropic);

#include "Maxwell.T.H"
makeLaminarModel(Maxwell);

#include "Giesekus.T.H"
makeLaminarModel(Giesekus);

#include "PTT.T.H"
makeLaminarModel(PTT);


// -------------------------------------------------------------------------- //
// RAS models
// -------------------------------------------------------------------------- //

#include "SpalartAllmaras.T.H"
makeRASModel(SpalartAllmaras);

#include "kEpsilon.H"
makeRASModel(kEpsilon);

#include "RNGkEpsilon.T.H"
makeRASModel(RNGkEpsilon);

#include "realizableKE.H"
makeRASModel(realizableKE);

#include "LaunderSharmaKE.T.H"
makeRASModel(LaunderSharmaKE);

#include "kOmega.H"
makeRASModel(kOmega);

#include "kOmegaSST.H"
makeRASModel(kOmegaSST);

#include "kOmegaSSTSAS.H"
makeRASModel(kOmegaSSTSAS);

#include "kOmegaSSTLM.H"
makeRASModel(kOmegaSSTLM);

#include "v2f.H"
makeRASModel(v2f);

#include "LRR.T.H"
makeRASModel(LRR);

#include "SSG.T.H"
makeRASModel(SSG);


// -------------------------------------------------------------------------- //
// LES models
// -------------------------------------------------------------------------- //

#include "Smagorinsky.T.H"
makeLESModel(Smagorinsky);

#include "WALE.T.H"
makeLESModel(WALE);

#include "kEqn.H"
makeLESModel(kEqn);

#include "dynamicKEqn.H"
makeLESModel(dynamicKEqn);

#include "dynamicLagrangian.H"
makeLESModel(dynamicLagrangian);

#include "kOmegaSSTDES.H"
makeLESModel(kOmegaSSTDES);

#include "SpalartAllmarasDES.T.H"
makeLESModel(SpalartAllmarasDES);

#include "SpalartAllmarasDDES.T.H"
makeLESModel(SpalartAllmarasDDES);

#include "SpalartAllmarasIDDES.T.H"
makeLESModel(SpalartAllmarasIDDES);

#include "DeardorffDiffStress.T.H"
makeLESModel(DeardorffDiffStress);


// ************************************************************************* //
