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

Class
    Foam::sampledSurfaces::triSurface

Description
    A sampledSurface from a triSurface. It samples on the points/triangles
    of the triSurface. It either samples cells or (non-coupled) boundary faces.

    There are 6 possible modes:

    - source=cells, interpolate=false:
        Finds per triangle centre the nearest cell centre and uses its value.

    - source=cells, interpolate=true
        Finds per triangle centre the nearest cell centre. Per surface point
        checks if this nearest cell is the one containing point; otherwise
        projects the point onto the nearest point on the boundary of the cell
        (to make sure interpolateCellPoint gets a valid location)

    - source=insideCells, interpolate=false:
        Finds per triangle centre the cell containing it and uses its value.
        Trims triangles outside mesh.

    - source=insideCells, interpolate=true
        Per surface point interpolate cell containing it.

    - source=boundaryFaces, interpolate=false:
        Finds per triangle centre the nearest point on the boundary (uncoupled
        faces only) and uses the value (or 0 if the nearest is on an empty
        boundary).

    - source=boundaryFaces, interpolate=true:
        Finds per triangle centre the nearest point on the boundary (uncoupled
        faces only). Per surface point projects the point onto this boundary
        face (to make sure interpolateCellPoint gets a valid location)

    Since it finds a nearest per triangle each triangle is guaranteed to be on
    one processor only. So after stitching (by sampledSurfaces) the original
    surface should be complete.

    Example:
    \verbatim
    {
        type        triSurface;
        surface     "surface.stl";
        source      cells;
        interpolate yes;
    }
    \endverbatim

Usage
    \table
        Property     | Description             | Required    | Default value
        surface      | the surface file (within constant/triSurface) | yes |
        source       | see above               | yes         | no
        interpolate  | see above               | no          | no
    \endtable

SourceFiles
    sampledTriSurface.C

\*---------------------------------------------------------------------------*/

#ifndef sampledTriSurface_H
#define sampledTriSurface_H

#include "sampledSurface.H"
#include "triSurface_searchableSurface.H"
#include "MeshedSurface.T.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

class treeDataFace;

namespace sampledSurfaces
{

/*---------------------------------------------------------------------------*\
                          Class triSurface Declaration
\*---------------------------------------------------------------------------*/

class triSurface
:
    public sampledSurface,
    public MeshedSurface<face>
{
public:

    // Public Typedefs

        //- Types of communications
        enum samplingSource
        {
            cells,
            insideCells,
            boundaryFaces,
        };

        //- Names of communication types
        static const NamedEnum<samplingSource, 3> samplingSourceNames_;


private:

    // Private Data

        //- Surface to sample on
        const searchableSurfaces::triSurface surface_;

        //- Whether to sample internal cell values or boundary values
        const samplingSource sampleSource_;

        //- Track if the surface needs an update
        mutable bool needsUpdate_;

        //- Search tree for all non-coupled boundary faces
        mutable autoPtr<indexedOctree<treeDataFace>> boundaryTreePtr_;

        //- From local surface triangle to mesh cell/face.
        labelList sampleElements_;

        //- Local points to sample per point
        pointField samplePoints_;


    // Private Member Functions

        //- Get tree of all non-coupled boundary faces
        const indexedOctree<treeDataFace>& nonCoupledboundaryTree() const;

        //- Sample field on surface
        template<class Type>
        tmp<Field<Type>> sampleField
        (
            const VolField<Type>& vField
        ) const;

        //- Interpolate field on surface
        template<class Type>
        tmp<Field<Type>>
        interpolateField(const interpolation<Type>&) const;


public:

    //- Runtime type information
    TypeName("triSurface");


    // Constructors

        //- Construct from components
        triSurface
        (
            const word& name,
            const polyMesh& mesh,
            const word& surfaceName,
            const samplingSource sampleSource
        );

        //- Construct from dictionary
        triSurface
        (
            const word& name,
            const polyMesh& mesh,
            const dictionary& dict
        );

        //- Construct from triSurface
        triSurface
        (
            const word& name,
            const polyMesh& mesh,
            const Foam::triSurface& surface,
            const word& sampleSourceName
        );


    //- Destructor
    virtual ~triSurface();


    // Member Functions

        //- Does the surface need an update?
        virtual bool needsUpdate() const;

        //- Update the surface as required.
        //  Do nothing (and return false) if no update was needed
        virtual bool update();

        //- Points of surface
        virtual const pointField& points() const
        {
            return MeshedSurface<face>::points();
        }

        //- Faces of surface
        virtual const faceList& faces() const
        {
            return MeshedSurface<face>::faces();
        }

        //- Sample field on the surface's faces
        #define DEFINE_SAMPLE(Type, nullArg)                                   \
            virtual tmp<Field<Type>> sample                                    \
            (                                                                  \
                const VolField<Type>&                                          \
            ) const;
        FOR_ALL_FIELD_TYPES(DEFINE_SAMPLE);
        #undef DEFINE_SAMPLE

        //- Interpolate field to the surface's points
        #define DEFINE_INTERPOLATE(Type, nullArg)                              \
            virtual tmp<Field<Type>> interpolate                               \
            (                                                                  \
                const interpolation<Type>&                                     \
            ) const;
        FOR_ALL_FIELD_TYPES(DEFINE_INTERPOLATE);
        #undef DEFINE_INTERPOLATE

        //- Update for mesh point-motion
        virtual void movePoints();

        //- Update topology using the given map
        virtual void topoChange(const polyTopoChangeMap&);

        //- Update from another mesh using the given map
        virtual void mapMesh(const polyMeshMap&);

        //- Redistribute or update using the given distribution map
        virtual void distribute(const polyDistributionMap&);

        //- Write
        virtual void print(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace sampledSurfaces
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "sampledTriSurfaceTemplates.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
