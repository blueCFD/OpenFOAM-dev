/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2011-2023 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
 2014-02-21 blueCAPE Lda: Modifications for blueCFD-Core 2.3
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

Modifications
    This file has been modified by blueCAPE's unofficial mingw patches for
    OpenFOAM.
    For more information about these patches, visit:
        http://bluecfd.com/Core

    Modifications made:
      - These changes are technically from Symscape's own patches for Windows,
        circa 2011.
      - The adaptation derived from the patches for blueCFD 2.1, adjusted to
        OpenFOAM 2.2.


\*---------------------------------------------------------------------------*/

#include "mathematicalConstants.H"
#include "universalConstants.H"
#include "electromagneticConstants.H"
#include "physicoChemicalConstants.H"
#include "dimensionedConstants.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace constant
{

const char* const physicoChemical::group = "physicoChemical";


// Note: cannot use dimless etc. as they may not have been constructed yet

const Foam::dimensionedScalar physicoChemical::RR
(
    dimensionedConstant
    (
        physicoChemical::group,
        "RR",
        physicoChemical::NA*physicoChemical::k
    )
);


const Foam::dimensionedScalar physicoChemical::F
(
    dimensionedConstant
    (
        physicoChemical::group,
        "F",
        physicoChemical::NA*electromagnetic::e
    )
#ifdef WM_SP
       .dimensions(),
       // Assuming this is the Stefan-Boltzmann constant
       // http://en.wikipedia.org/wiki/Stefan%E2%80%93Boltzmann_law
       // Single precision can't handle the pow4(k), 
       // where k is Boltzmann constant = 1.3806488e-23
       5.6704e-8f
#endif     
);


const Foam::dimensionedScalar physicoChemical::sigma
(
    dimensionedConstant
    (
        physicoChemical::group,
        "sigma",
        (sqr(mathematical::pi)/60.0)
       *pow4(physicoChemical::k)/(pow3(universal::hr)*sqr(universal::c))
    )
);


const Foam::dimensionedScalar physicoChemical::b
(
    dimensionedConstant
    (
        physicoChemical::group,
        "b",
        (universal::h*universal::c/physicoChemical::k)/4.965114231
    )
);


const Foam::dimensionedScalar physicoChemical::c1
(
    dimensionedConstant
    (
        physicoChemical::group,
        "c1",
        mathematical::twoPi*universal::h*sqr(universal::c)
    )
);


const Foam::dimensionedScalar physicoChemical::c2
(
    dimensionedConstant
    (
        physicoChemical::group,
        "c2",
        universal::h*universal::c/physicoChemical::k
    )
);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace constant
} // End namespace Foam

// ************************************************************************* //
