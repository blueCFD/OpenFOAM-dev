/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2011-2022 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
 2014-02-21 blueCAPE Lda: Modifications for blueCFD-Core 2.3
 2017-09-xx FSD blueCFD Lda: Modifications for blueCFD-Core 2017-1
 2016-2024 FS Dynamics Portugal: Changes are tracked at:
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

Modifications
    This file has been modified by blueCAPE's unofficial mingw patches for
    OpenFOAM.
    For more information about these patches, visit:
        http://bluecfd.com/Core

    Modifications made:
      - These changes are technically from Symscape's own patches for Windows,
        circa 2009.
      - The adaptation derived from the patches for blueCFD 2.1, adjusted to
        OpenFOAM 2.2.
      - Further adaptation was done for re-adapting to OpenFOAM 5 collated file
        management.
      - Moved out from fileName main source file to a separate one.
      - Then move to fileName for generalizing file and folder renaming.

\*---------------------------------------------------------------------------*/

#include "fileName.H"
#include "OSspecific.H"
#include "wordRe.H"
#include "objectRegistry.H"
#include <regExp.H>
#include <regex>

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    static List<Pair<wordRe>> replacedFileNames_;
}

// * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * * //

void Foam::fileName::addFileNameFilter
(
    const Foam::wordRe & from, 
    const Foam::wordRe & to
)
{
    if (debug)
    {
        InfoInFunction
            << "Adding renaming pattern '" << from
            << "' to '"<< to << "'"
            << endl;
    }

    Pair<wordRe> newItem(from, to);
    newItem.first().compile();

    replacedFileNames_.append(newItem);
}


bool Foam::fileName::filterName(const bool absoluteOnly)
{
    fileName newName(*this);

    forAll (replacedFileNames_, index)
    {
        const Pair<wordRe> & rfn = replacedFileNames_[index];

        if(rfn.first().match(diskFileName))
        {
            diskFileName = std::regex_replace
            (
                *this,
                std::regex(rfn.first()),
                rfn.second()
            );
            break;
        }
        else if(regExp(rfn.first()).search(diskFileName))
        {
            diskFileName.replace(rfn.first(), rfn.second());
            break;
        }
    }

    if (debug)
    {
        InfoInFunction
            << "Applying renaming pattern '" << *this
            << "' to '"<< diskFileName << "'"
            << endl;
    }

    *this = diskFileName;
}

// ************************************************************************* //
