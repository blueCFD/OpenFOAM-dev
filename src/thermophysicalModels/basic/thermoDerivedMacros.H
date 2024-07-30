/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2020 OpenFOAM Foundation
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

#ifndef thermoDerivedMacros_H
#define thermoDerivedMacros_H

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define defineDerivedThermoName(className, classNameString)               \
    const Foam::word Foam::className::derivedThermoName(classNameString); \
    const Foam::word Foam::className::derivedThermoName_()                \
    {                                                                     \
        return classNameString;                                           \
    }

#define DerivedThermoName                       \
        static const word derivedThermoName_(); \
        static const word derivedThermoName;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
