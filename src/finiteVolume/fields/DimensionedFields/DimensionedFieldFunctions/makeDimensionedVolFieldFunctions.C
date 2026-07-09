/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2026 OpenFOAM Foundation
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

#include "DimensionedFieldFunction.T.H"
#include "volFields.H"
#include "fieldTypes.H"
#include "addToRunTimeSelectionTable.H"

#include "TimeFunction_DimensionedFieldFunction.T.H"
#include "Zonal_DimensionedFieldFunction.T.H"
#include "DistanceFunction_DimensionedFieldFunction.T.H"
#include "Coded_DimensionedFieldFunction.T.H"
#include "Surfaces_DimensionedFieldFunction.T.H"
#include "FieldFunction_DimensionedFieldFunction.T.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeDimensionedVolFieldFunctions(Type, nullArg)                        \
                                                                               \
    typedef DimensionedField<Type, fvMesh, Field> Type##FvMesh;                \
    defineDimensionedFieldFunction(Type##FvMesh);                              \
    namespace DimensionedFieldFunctions                                        \
    {                                                                          \
        addDimensionedFieldFunction(TimeFunction, Type##FvMesh);               \
        addDimensionedFieldFunction(Zonal, Type##FvMesh);                      \
        addDimensionedFieldFunction(DistanceFunction, Type##FvMesh);           \
        addDimensionedFieldFunction(Coded, Type##FvMesh);                      \
        addDimensionedFieldFunction(Surfaces, Type##FvMesh);                   \
        addDimensionedFieldFunction(FieldFunction, Type##FvMesh);              \
    }

namespace Foam
{
    FOR_ALL_FIELD_TYPES(makeDimensionedVolFieldFunctions);
}


// ************************************************************************* //
