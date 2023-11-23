/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2011-2023 OpenFOAM Foundation
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

#include "None.T.H"
#include "Constant.T.H"
#include "Uniform.T.H"
#include "ZeroConstant.T.H"
#include "OneConstant.T.H"
#include "Polynomial1.T.H"
#include "Sine.T.H"
#include "Square.T.H"
#include "Table.T.H"
#include "UniformTable1.T.H"
#include "NonUniformTable1.T.H"
#include "Scale.T.H"
#include "CodedFunction1.T.H"

#include "fieldTypes.H"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
    makeFunction1(label);

    namespace Function1s
    {
        makeFunction1Type(None, label);
        makeFunction1Type(Constant, label);
    }

    FOR_ALL_FIELD_TYPES(makeFunction1s);
}


// ************************************************************************* //
