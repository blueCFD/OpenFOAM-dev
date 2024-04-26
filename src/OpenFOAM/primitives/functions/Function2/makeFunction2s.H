/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2020-2024 OpenFOAM Foundation
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

Class
    Foam::Function2

Description
    Macros for creating standard Function2-s

\*---------------------------------------------------------------------------*/

#ifndef makeFunction2s_H
#define makeFunction2s_H

#include "None2.T.H"
#include "Constant2.T.H"
#include "ZeroConstant2.T.H"
#include "OneConstant2.T.H"
#include "Scale2.T.H"
#include "Function12.T.H"
#include "Product2.T.H"
#include "Radial2.T.H"
#include "UniformTable2.T.H"
#include "CodedFunction2.T.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeFunction2s(Type, nullArg)                                          \
                                                                               \
    defineFunction2(Type);                                                     \
                                                                               \
    namespace Function2s                                                       \
    {                                                                          \
        addFunction2(None, Type);                                              \
        addStreamConstructableFunction2(Constant, Type);                       \
        addFunction2(ZeroConstant, Type);                                      \
        addFunction2(OneConstant, Type);                                       \
        addFunction2(Scale, Type);                                             \
        addFunction2(Function12, Type);                                        \
        addFunction2(Product, Type);                                           \
        addFunction2(Radial, Type);                                            \
        addFunction2(UniformTable, Type);                                      \
        addFunction2(Coded, Type);                                             \
    }

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
