/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2011-2020 OpenFOAM Foundation
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

#ifndef makeParcelForces_H
#define makeParcelForces_H

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "SphereDragForce.T.H"
#include "NonSphereDragForce.T.H"
#include "SchillerNaumannDragForce.T.H"
#include "WenYuDragForce.T.H"
#include "ErgunWenYuDragForce.T.H"
#include "PlessisMasliyahDragForce.T.H"

#include "SaffmanMeiLiftForce.T.H"
#include "TomiyamaLiftForce.T.H"

#include "GravityForce.T.H"
#include "NonInertialFrameForce.T.H"
#include "ParamagneticForce.T.H"
#include "PressureGradientForce.T.H"
#include "SRFForce.T.H"
#include "VirtualMassForce.T.H"

#include "ScaledForce.T.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeParcelForces(CloudType)                                            \
                                                                               \
    makeParticleForceModel(CloudType);                                         \
    makeParticleForceModelType(SphereDragForce, CloudType);                    \
    makeParticleForceModelType(NonSphereDragForce, CloudType);                 \
    makeParticleForceModelType(SchillerNaumannDragForce, CloudType);           \
    makeParticleForceModelType(WenYuDragForce, CloudType);                     \
    makeParticleForceModelType(ErgunWenYuDragForce, CloudType);                \
    makeParticleForceModelType(PlessisMasliyahDragForce, CloudType);           \
    makeParticleForceModelType(SaffmanMeiLiftForce, CloudType);                \
    makeParticleForceModelType(TomiyamaLiftForce, CloudType);                  \
    makeParticleForceModelType(GravityForce, CloudType);                       \
    makeParticleForceModelType(NonInertialFrameForce, CloudType);              \
    makeParticleForceModelType(ParamagneticForce, CloudType);                  \
    makeParticleForceModelType(PressureGradientForce, CloudType);              \
    makeParticleForceModelType(SRFForce, CloudType);                           \
    makeParticleForceModelType(VirtualMassForce, CloudType);                   \
    makeParticleForceModelType(ScaledForce, CloudType);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
