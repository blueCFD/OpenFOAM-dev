/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2011-2024 OpenFOAM Foundation
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
    Foam::Function1

Description
    Macros for creating standard Function1-s

\*---------------------------------------------------------------------------*/

#ifndef makeFunction1s_H
#define makeFunction1s_H

#include "None.T.H"
#include "Constant.T.H"
#include "Uniform.T.H"
#include "ZeroConstant.T.H"
#include "OneConstant.T.H"
#include "Polynomial1.T.H"
#include "Add.T.H"
#include "Repeat.T.H"
#include "Scale.T.H"
#include "Shift.T.H"
#include "Sine.T.H"
#include "Square.T.H"
#include "Table.T.H"
#include "UniformTable1.T.H"
#include "NonUniformTable1.T.H"
#include "CodedFunction1.T.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeFunction1s(Type, nullArg)                                          \
                                                                               \
    template<>                                                                 \
    const char* const Foam::Tuple2<Foam::scalar, Type>::typeName               \
    (                                                                          \
        "Tuple2<scalar," #Type ">"                                             \
    );                                                                         \
                                                                               \
    defineFunction1(Type);                                                     \
                                                                               \
    namespace Function1s                                                       \
    {                                                                          \
        addFunction1(None, Type);                                              \
        addStreamConstructableFunction1(Constant, Type);                       \
        addStreamConstructableFunction1(Uniform, Type);                        \
        addFunction1(ZeroConstant, Type);                                      \
        addFunction1(OneConstant, Type);                                       \
        addStreamConstructableFunction1(Polynomial, Type);                     \
        addFunction1(Add, Type);                                               \
        addFunction1(Repeat, Type);                                            \
        addFunction1(Scale, Type);                                             \
        addFunction1(Shift, Type);                                             \
        addFunction1(Sine, Type);                                              \
        addFunction1(Square, Type);                                            \
        addStreamConstructableFunction1(Table, Type);                          \
        addNamedFunction1(Table, Type, tableFile);                             \
        addFunction1(UniformTable, Type);                                      \
        addFunction1(NonUniformTable, Type);                                   \
        addFunction1(Coded, Type);                                             \
    }

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
