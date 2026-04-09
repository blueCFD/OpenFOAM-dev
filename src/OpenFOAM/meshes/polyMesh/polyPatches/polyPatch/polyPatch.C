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

#include "polyPatch.H"
#include "addToRunTimeSelectionTable.H"
#include "polyBoundaryMesh.H"
#include "polyMesh.H"
#include "primitiveMesh.H"
#include "SubField.T.H"
#include "entry.H"
#include "dictionary.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
    defineTypeNameAndDebug(polyPatch, 0);

    int polyPatch::disallowGenericPolyPatch
    (
        debug::debugSwitch("disallowGenericPolyPatch", 0)
    );

    defineRunTimeSelectionTable(polyPatch, word);
    defineRunTimeSelectionTable(polyPatch, dictionary);

    addToRunTimeSelectionTable(polyPatch, polyPatch, word);
    addToRunTimeSelectionTable(polyPatch, polyPatch, dictionary);
}


// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

void Foam::polyPatch::movePoints(const pointField& p)
{
    primitivePatch::clearGeom();
}


void Foam::polyPatch::movePoints(PstreamBuffers&, const pointField& p)
{
    primitivePatch::clearGeom();
}


void Foam::polyPatch::topoChange(PstreamBuffers&)
{
    primitivePatch::clearGeom();
    clearAddressing();
}


void Foam::polyPatch::clearGeom()
{
    primitivePatch::clearGeom();
}


void Foam::polyPatch::rename(const wordList& newNames)
{
    name_ = newNames[index_];
}


void Foam::polyPatch::reorder(const labelUList& newToOldIndex)
{
    index_ = findIndex(newToOldIndex, index_);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::polyPatch::polyPatch
(
    const word& name,
    const label size,
    const label start,
    const label index,
    const polyBoundaryMesh& bm,
    const word& patchType
)
:
    patchIdentifier(name, index),
    primitivePatch
    (
        faceSubList(bm.mesh().faces(), size, start),
        bm.mesh().points()
    ),
    start_(start),
    boundaryMesh_(bm),
    faceCellsPtr_(nullptr),
    mePtr_(nullptr)
{}


Foam::polyPatch::polyPatch
(
    const word& name,
    const dictionary& dict,
    const label index,
    const polyBoundaryMesh& bm,
    const word& patchType
)
:
    patchIdentifier(name, dict, index),
    primitivePatch
    (
        faceSubList
        (
            bm.mesh().faces(),
            dict.lookup<label>("nFaces"),
            dict.lookup<label>("startFace")
        ),
        bm.mesh().points()
    ),
    start_(dict.lookup<label>("startFace")),
    boundaryMesh_(bm),
    faceCellsPtr_(nullptr),
    mePtr_(nullptr)
{}


Foam::polyPatch::polyPatch
(
    const polyPatch& pp,
    const polyBoundaryMesh& bm
)
:
    patchIdentifier(pp),
    primitivePatch
    (
        faceSubList
        (
            bm.mesh().faces(),
            pp.size(),
            pp.start()
        ),
        bm.mesh().points()
    ),
    start_(pp.start()),
    boundaryMesh_(bm),
    faceCellsPtr_(nullptr),
    mePtr_(nullptr)
{}


Foam::polyPatch::polyPatch
(
    const polyPatch& pp,
    const polyBoundaryMesh& bm,
    const label index,
    const label newSize,
    const label newStart
)
:
    patchIdentifier(pp, index),
    primitivePatch
    (
        faceSubList
        (
            bm.mesh().faces(),
            newSize,
            newStart
        ),
        bm.mesh().points()
    ),
    start_(newStart),
    boundaryMesh_(bm),
    faceCellsPtr_(nullptr),
    mePtr_(nullptr)
{}


Foam::polyPatch::polyPatch(const polyPatch& p)
:
    patchIdentifier(p),
    primitivePatch(p),
    start_(p.start_),
    boundaryMesh_(p.boundaryMesh_),
    faceCellsPtr_(nullptr),
    mePtr_(nullptr)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::polyPatch::~polyPatch()
{
    clearAddressing();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const Foam::polyBoundaryMesh& Foam::polyPatch::boundaryMesh() const
{
    return boundaryMesh_;
}


const Foam::polyMesh& Foam::polyPatch::mesh() const
{
    return boundaryMesh_.mesh();
}


const Foam::vectorField::subField Foam::polyPatch::faceCentres() const
{
    return patchSlice(mesh().faceCentres());
}


const Foam::vectorField::subField Foam::polyPatch::faceAreas() const
{
    return patchSlice(mesh().faceAreas());
}


const Foam::scalarField::subField Foam::polyPatch::magFaceAreas() const
{
    return patchSlice(mesh().magFaceAreas());
}


Foam::tmp<Foam::vectorField> Foam::polyPatch::faceCellCentres() const
{
    tmp<vectorField> tcc(new vectorField(size()));
    vectorField& cc = tcc.ref();

    // get reference to global cell centres
    const vectorField& gcc = mesh().cellCentres();

    const labelUList& faceCells = this->faceCells();

    forAll(faceCells, facei)
    {
        cc[facei] = gcc[faceCells[facei]];
    }

    return tcc;
}


const Foam::labelUList& Foam::polyPatch::faceCells() const
{
    if (!faceCellsPtr_)
    {
        faceCellsPtr_ = new labelList::subList
        (
            patchSlice(mesh().faceOwner())
        );
    }

    return *faceCellsPtr_;
}


const Foam::labelList& Foam::polyPatch::meshEdges() const
{
    if (!mePtr_)
    {
        mePtr_ =
            new labelList
            (
                primitivePatch::meshEdges
                (
                    mesh().edges(),
                    mesh().pointEdges()
                )
            );
    }

    return *mePtr_;
}


void Foam::polyPatch::clearAddressing()
{
    primitivePatch::clearTopology();
    primitivePatch::clearPatchMeshAddr();
    deleteDemandDrivenData(faceCellsPtr_);
    deleteDemandDrivenData(mePtr_);
}


void Foam::polyPatch::write(Ostream& os) const
{
    writeEntry(os, "type", type());
    patchIdentifier::write(os);
    writeEntry(os, "nFaces", size());
    writeEntry(os, "startFace", start());
}


void Foam::polyPatch::initOrder(PstreamBuffers&, const primitivePatch&) const
{}


bool Foam::polyPatch::order
(
    PstreamBuffers&,
    const primitivePatch&,
    labelList& faceMap,
    labelList& rotation
) const
{
    // Nothing changed.
    return false;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

void Foam::polyPatch::operator=(const polyPatch& p)
{
    clearAddressing();

    patchIdentifier::operator=(p);
    primitivePatch::operator=(p);
    start_ = p.start_;
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

Foam::Ostream& Foam::operator<<(Ostream& os, const polyPatch& p)
{
    p.write(os);
    os.check("Ostream& operator<<(Ostream& os, const polyPatch& p");
    return os;
}


// ************************************************************************* //
