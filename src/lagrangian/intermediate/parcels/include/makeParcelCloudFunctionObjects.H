/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2011-2018 OpenFOAM Foundation
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

#ifndef makeParcelCloudFunctionObjects_H
#define makeParcelCloudFunctionObjects_H

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "FacePostProcessing.T.H"
#include "ParticleCollector.T.H"
#include "ParticleErosion.T.H"
#include "ParticleTracks.T.H"
#include "ParticleTrap.T.H"
#include "PatchCollisionDensity.T.H"
#include "PatchPostProcessing.T.H"
#include "VoidFraction.T.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeParcelCloudFunctionObjects(CloudType)                              \
                                                                               \
    makeCloudFunctionObject(CloudType);                                        \
                                                                               \
    makeCloudFunctionObjectType(FacePostProcessing, CloudType);                \
    makeCloudFunctionObjectType(ParticleCollector, CloudType);                 \
    makeCloudFunctionObjectType(ParticleErosion, CloudType);                   \
    makeCloudFunctionObjectType(ParticleTracks, CloudType);                    \
    makeCloudFunctionObjectType(ParticleTrap, CloudType);                      \
    makeCloudFunctionObjectType(PatchCollisionDensity, CloudType);             \
    makeCloudFunctionObjectType(PatchPostProcessing, CloudType);               \
    makeCloudFunctionObjectType(VoidFraction, CloudType);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
