/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2011-2018 OpenFOAM Foundation
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

defineDimensionedConstantWithDefault
(
    physicoChemical::group,
    physicoChemical::R,
    dimensionedScalar
    (
        "R",
        physicoChemical::NA*physicoChemical::k
    ),
    constantphysicoChemicalR,
    "R"
);


defineDimensionedConstantWithDefault
(
    physicoChemical::group,
    physicoChemical::F,
    dimensionedScalar
    (
        "F",
        physicoChemical::NA*electromagnetic::e
    ),
    constantphysicoChemicalF,
    "F"
);


// Note: cannot use dimless etc. since not guaranteed to be constructed
defineDimensionedConstantWithDefault
(
    physicoChemical::group,
    physicoChemical::sigma,
    dimensionedScalar
    (
        "sigma",
        (
        Foam::dimensionedScalar
        (
            "C",
            dimensionSet(0, 0, 0, 0, 0),    // Foam::dimless,
            Foam::sqr(mathematical::pi)/60.0
        )
       *Foam::pow4(physicoChemical::k)
       /(pow3(universal::hr)*sqr(universal::c))
       )
#ifdef WM_SP
       .dimensions(),
       // Assuming this is the Stefan-Boltzmann constant
       // http://en.wikipedia.org/wiki/Stefan%E2%80%93Boltzmann_law
       // Single precision can't handle the pow4(k), 
       // where k is Boltzmann constant = 1.3806488e-23
       5.6704e-8f
#endif     
    ),
    constantphysicoChemicalsigma,
    "sigma"
);


defineDimensionedConstantWithDefault
(
    physicoChemical::group,
    physicoChemical::b,
    dimensionedScalar
    (
        "b",
        (universal::h*universal::c/physicoChemical::k)
       /Foam::dimensionedScalar
        (
            "C",
            dimensionSet(0, 0, 0, 0, 0),    // Foam::dimless
            4.965114231
        )
    ),
    constantphysicoChemicalb,
    "b"
);


defineDimensionedConstantWithDefault
(
    physicoChemical::group,
    physicoChemical::c1,
    dimensionedScalar
    (
        "c1",
        Foam::dimensionedScalar
        (
            "C",
            dimensionSet(0, 0, 0, 0, 0),    // Foam::dimless,
            mathematical::twoPi
        )
       *universal::h*Foam::sqr(universal::c)
    ),
    constantphysicoChemicalc1,
    "c1"
);


defineDimensionedConstantWithDefault
(
    physicoChemical::group,
    physicoChemical::c2,
    dimensionedScalar
    (
        "c2",
        universal::h*universal::c/physicoChemical::k
    ),
    constantphysicoChemicalc2,
    "c2"
);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace constant
} // End namespace Foam

// ************************************************************************* //
