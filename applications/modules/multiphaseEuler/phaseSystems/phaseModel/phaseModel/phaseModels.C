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

#include "addToRunTimeSelectionTable.H"

#include "rhoFluidThermo.H"
#include "rhoFluidMulticomponentThermo.H"

#include "combustionModel.H"

#include "phaseModel.H"
#include "ThermoPhaseModel.T.H"
#include "IsothermalPhaseModel.T.H"
#include "AnisothermalPhaseModel.T.H"
#include "PurePhaseModel.T.H"
#include "MulticomponentPhaseModel.T.H"
#include "InertPhaseModel.T.H"
#include "ReactingPhaseModel.T.H"
#include "MovingPhaseModel.T.H"
#include "StationaryPhaseModel.T.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
    typedef
        AnisothermalPhaseModel
        <
            PurePhaseModel
            <
                InertPhaseModel
                <
                    MovingPhaseModel
                    <
                        ThermoPhaseModel
                        <
                            phaseModel,
                            rhoFluidThermo
                        >
                    >
                >
            >
        >
        purePhaseModel;

    addNamedToRunTimeSelectionTable
    (
        phaseModel,
        purePhaseModel,
        phaseSystem,
        purePhaseModel
    );

    typedef
        AnisothermalPhaseModel
        <
            PurePhaseModel
            <
                InertPhaseModel
                <
                    StationaryPhaseModel
                    <
                        ThermoPhaseModel
                        <
                            phaseModel,
                            rhoFluidThermo
                        >
                    >
                >
            >
        >
        pureStationaryPhaseModel;

    addNamedToRunTimeSelectionTable
    (
        phaseModel,
        pureStationaryPhaseModel,
        phaseSystem,
        pureStationaryPhaseModel
    );

    typedef
        IsothermalPhaseModel
        <
            PurePhaseModel
            <
                InertPhaseModel
                <
                    MovingPhaseModel
                    <
                        ThermoPhaseModel
                        <
                            phaseModel,
                            rhoFluidThermo
                        >
                    >
                >
            >
        >
        pureIsothermalPhaseModel;

    addNamedToRunTimeSelectionTable
    (
        phaseModel,
        pureIsothermalPhaseModel,
        phaseSystem,
        pureIsothermalPhaseModel
    );

    typedef
        IsothermalPhaseModel
        <
            PurePhaseModel
            <
                InertPhaseModel
                <
                    StationaryPhaseModel
                    <
                        ThermoPhaseModel
                        <
                            phaseModel,
                            rhoFluidThermo
                        >
                    >
                >
            >
        >
        pureStationaryIsothermalPhaseModel;

    addNamedToRunTimeSelectionTable
    (
        phaseModel,
        pureStationaryIsothermalPhaseModel,
        phaseSystem,
        pureStationaryIsothermalPhaseModel
    );

    typedef
        AnisothermalPhaseModel
        <
            MulticomponentPhaseModel
            <
                InertPhaseModel
                <
                    MovingPhaseModel
                    <
                        ThermoPhaseModel
                        <
                            phaseModel,
                            rhoFluidMulticomponentThermo
                        >
                    >
                >
            >
        >
        multicomponentPhaseModel;

    addNamedToRunTimeSelectionTable
    (
        phaseModel,
        multicomponentPhaseModel,
        phaseSystem,
        multicomponentPhaseModel
    );

    // Also add multicomponentPhaseModel with the name multiComponentPhaseModel
    // for backward-compatibility
    addNamedToRunTimeSelectionTable
    (
        phaseModel,
        multicomponentPhaseModel,
        phaseSystem,
        multiComponentPhaseModel
    );

    typedef
        IsothermalPhaseModel
        <
            MulticomponentPhaseModel
            <
                InertPhaseModel
                <
                    MovingPhaseModel
                    <
                        ThermoPhaseModel
                        <
                            phaseModel,
                            rhoFluidMulticomponentThermo
                        >
                    >
                >
            >
        >
        multicomponentIsothermalPhaseModel;

    addNamedToRunTimeSelectionTable
    (
        phaseModel,
        multicomponentIsothermalPhaseModel,
        phaseSystem,
        multicomponentIsothermalPhaseModel
    );

    typedef
        AnisothermalPhaseModel
        <
            MulticomponentPhaseModel
            <
                ReactingPhaseModel
                <
                    MovingPhaseModel
                    <
                        ThermoPhaseModel
                        <
                            phaseModel,
                            rhoFluidMulticomponentThermo
                        >
                    >
                >
            >
        >
        reactingPhaseModel;

    addNamedToRunTimeSelectionTable
    (
        phaseModel,
        reactingPhaseModel,
        phaseSystem,
        reactingPhaseModel
    );
}

// ************************************************************************* //
