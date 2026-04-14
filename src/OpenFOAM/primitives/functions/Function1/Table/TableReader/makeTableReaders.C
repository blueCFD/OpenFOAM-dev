/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2011-2026 OpenFOAM Foundation
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

#include "TableReader.T.H"

#include "EmbeddedTableReader.T.H"
#include "FoamTableReader.T.H"
#include "CsvTableReader.T.H"

#include "fieldTypes.H"

#include "addToRunTimeSelectionTable.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Permute the order of the macro arguments so that the Value type can be
// iterated over by FOR_ALL_FIELD_TYPES

#define defineTableReaderValueFirst(Value, Coordinate) \
    defineTableReader(Coordinate, Value)

#define addTableReaderValueFirst(Value, TableReaderType, Coordinate) \
    addTableReader(TableReaderType, Coordinate, Value)

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
    FOR_ALL_FIELD_TYPES(defineTableReaderValueFirst, scalar);
    defineTableReader(vector, scalar)
    defineTableReader(scalar, vector2D)

    FOR_ALL_FIELD_TYPES(addTableReaderValueFirst, Embedded, scalar)
    addTableReader(Embedded, scalar, vector2D)
    addTableReader(Embedded, vector, scalar)
    FOR_ALL_FIELD_TYPES(addTableReaderValueFirst, Foam, scalar)
    addTableReader(Foam, scalar, vector2D)
    addTableReader(Foam, vector, scalar)
    FOR_ALL_FIELD_TYPES(addTableReaderValueFirst, Csv, scalar)
    addTableReader(Csv, scalar, vector2D)
    addTableReader(Csv, vector, scalar)
}

// ************************************************************************* //
