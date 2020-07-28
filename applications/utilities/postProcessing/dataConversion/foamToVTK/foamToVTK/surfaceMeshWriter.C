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
      - Derived from the patches for blueCFD 2.1 and 2.2. which in turn were derived
        from 2.0 and 1.7.
      - Always open the files in binary mode, because of how things work on 
        Windows.
        - Note: This modification is hard to stipulate who implemented this
                first. Symscape's port only began integrating this fix after
                blueCFD 1.7-2 was released with this sort of specific fix.
                But it was Symscape that first implemented this kind of
                "binary" fix in OpenFOAM's core code.

\*---------------------------------------------------------------------------*/

#include "surfaceMeshWriter.H"
#include "writeFuns.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::surfaceMeshWriter::surfaceMeshWriter
(
    const bool binary,
    const indirectPrimitivePatch& pp,
    const word& name,
    const fileName& fName
)
:
    binary_(binary),
    pp_(pp),
    fName_(fName),
    // Use binary mode in case we write binary.
    // Causes windows reading to fail if we don't
    os_(fName.c_str(), 
        ios_base::out|ios_base::binary)
{
    // Write header
    writeFuns::writeHeader(os_, binary_, name);

    os_ << "DATASET POLYDATA" << std::endl;

    // Write topology
    label nFaceVerts = 0;

    forAll(pp, facei)
    {
        nFaceVerts += pp[facei].size() + 1;
    }

    os_ << "POINTS " << pp.nPoints() << " float" << std::endl;

    DynamicList<floatScalar> ptField(3*pp.nPoints());
    writeFuns::insert(pp.localPoints(), ptField);
    writeFuns::write(os_, binary, ptField);


    os_ << "POLYGONS " << pp.size() << ' ' << nFaceVerts << std::endl;

    DynamicList<label> vertLabels(nFaceVerts);

    forAll(pp, facei)
    {
        const face& f = pp.localFaces()[facei];

        vertLabels.append(f.size());
        writeFuns::insert(f, vertLabels);
    }
    writeFuns::write(os_, binary_, vertLabels);
}


// ************************************************************************* //
