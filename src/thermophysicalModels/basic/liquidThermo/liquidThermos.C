/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2017-2023 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
2024 FS Dynamics Portugal: Changes are tracked at:
                 https://github.com/blueCFD/OpenFOAM-dev
------------------------------------------------------------------------------
License
    This file is a derivative work of OpenFOAM.

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

#include "liquidThermo.H"

#include "pureMixture.H"

#include "liquidPropertiesSelector.H"
#include "sensibleInternalEnergy.H"
#include "sensibleEnthalpy.H"
#include "thermo.H"

#include "makeThermo.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeLiquidThermo(ThermoPhysics)                                        \
                                                                               \
    defineThermo(liquidThermo, pureMixture, ThermoPhysics);                    \
                                                                               \
    addThermo(basicThermo, liquidThermo, pureMixture, ThermoPhysics);          \
    addThermo(fluidThermo, liquidThermo, pureMixture, ThermoPhysics);          \
    addThermo(rhoFluidThermo, liquidThermo, pureMixture, ThermoPhysics);       \
    addThermo(liquidThermo, liquidThermo, pureMixture, ThermoPhysics)

namespace Foam
{
    #if defined( WIN32 ) || defined( WIN64 )
    #define derivedThermoName derivedThermoName_()
    #endif

    typedef
        species::thermo<liquidPropertiesSelector, sensibleInternalEnergy>
        liquidSensibleInternalEnergy;

    makeLiquidThermo(liquidSensibleInternalEnergy);

    typedef
        species::thermo<liquidPropertiesSelector, sensibleEnthalpy>
        liquidSensibleEnthalpy;

    makeLiquidThermo(liquidSensibleEnthalpy);
}

// ************************************************************************* //
