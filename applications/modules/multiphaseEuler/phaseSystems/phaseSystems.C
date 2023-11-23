/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2015-2023 OpenFOAM Foundation
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

#include "phaseSystem.H"
#include "MomentumTransferPhaseSystem.T.H"
#include "OneResistanceHeatTransferPhaseSystem.T.H"
#include "TwoResistanceHeatTransferPhaseSystem.T.H"
#include "PhaseTransferPhaseSystem.T.H"
#include "InterfaceCompositionPhaseChangePhaseSystem.T.H"
#include "ThermalPhaseChangePhaseSystem.T.H"
#include "PopulationBalancePhaseSystem.T.H"
#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
    typedef
        PhaseTransferPhaseSystem
        <
            OneResistanceHeatTransferPhaseSystem
            <
                MomentumTransferPhaseSystem<phaseSystem>
            >
        >
        basicMultiphaseSystem;

    addNamedToRunTimeSelectionTable
    (
        phaseSystem,
        basicMultiphaseSystem,
        dictionary,
        basicMultiphaseSystem
    );

    typedef
        InterfaceCompositionPhaseChangePhaseSystem
        <
            PhaseTransferPhaseSystem
            <
                TwoResistanceHeatTransferPhaseSystem
                <
                    MomentumTransferPhaseSystem<phaseSystem>
                >
            >
        >
        interfaceCompositionPhaseChangeMultiphaseSystem;

    addNamedToRunTimeSelectionTable
    (
        phaseSystem,
        interfaceCompositionPhaseChangeMultiphaseSystem,
        dictionary,
        interfaceCompositionPhaseChangeMultiphaseSystem
    );

    typedef
        ThermalPhaseChangePhaseSystem
        <
            PhaseTransferPhaseSystem
            <
                TwoResistanceHeatTransferPhaseSystem
                <
                    MomentumTransferPhaseSystem<phaseSystem>
                >
            >
        >
        thermalPhaseChangeMultiphaseSystem;

    addNamedToRunTimeSelectionTable
    (
        phaseSystem,
        thermalPhaseChangeMultiphaseSystem,
        dictionary,
        thermalPhaseChangeMultiphaseSystem
    );

    typedef
        PopulationBalancePhaseSystem
        <
            PhaseTransferPhaseSystem
            <
                OneResistanceHeatTransferPhaseSystem
                <
                    MomentumTransferPhaseSystem<phaseSystem>
                >
            >
        >
        populationBalanceMultiphaseSystem;

    addNamedToRunTimeSelectionTable
    (
        phaseSystem,
        populationBalanceMultiphaseSystem,
        dictionary,
        populationBalanceMultiphaseSystem
    );

    typedef
        InterfaceCompositionPhaseChangePhaseSystem
        <
            PopulationBalancePhaseSystem
            <
                PhaseTransferPhaseSystem
                <
                    TwoResistanceHeatTransferPhaseSystem
                    <
                        MomentumTransferPhaseSystem<phaseSystem>
                    >
                >
            >
        >
        interfaceCompositionPhaseChangePopulationBalanceMultiphaseSystem;

    addNamedToRunTimeSelectionTable
    (
        phaseSystem,
        interfaceCompositionPhaseChangePopulationBalanceMultiphaseSystem,
        dictionary,
        interfaceCompositionPhaseChangePopulationBalanceMultiphaseSystem
    );

    typedef
        ThermalPhaseChangePhaseSystem
        <
            PopulationBalancePhaseSystem
            <
                PhaseTransferPhaseSystem
                <
                    TwoResistanceHeatTransferPhaseSystem
                    <
                        MomentumTransferPhaseSystem<phaseSystem>
                    >
                >
            >
        >
        thermalPhaseChangePopulationBalanceMultiphaseSystem;

    addNamedToRunTimeSelectionTable
    (
        phaseSystem,
        thermalPhaseChangePopulationBalanceMultiphaseSystem,
        dictionary,
        thermalPhaseChangePopulationBalanceMultiphaseSystem
    );
}


// ************************************************************************* //
