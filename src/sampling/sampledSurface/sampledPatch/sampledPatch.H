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
    Foam::sampledSurfaces::patch

Description
    A sampledSurface on patches. Non-triangulated by default.

    Example:
    \verbatim
    {
        type        patch;
        patches     (walls);
        triangulate no;
        interpolate yes;
    }
    \endverbatim

Usage
    \table
        Property     | Description             | Required    | Default value
        patches      | the names of patches on which to sample | yes |
        triangulate  | triangulate the output  | no          | no
        interpolate  | interpolate values to the surface points | no | no
    \endtable

SourceFiles
    sampledPatch.C

\*---------------------------------------------------------------------------*/

#ifndef sampledPatch_H
#define sampledPatch_H

#include "sampledSurface.H"
#include "MeshedSurface.T.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace sampledSurfaces
{

/*---------------------------------------------------------------------------*\
                            Class patch Declaration
\*---------------------------------------------------------------------------*/

class patch
:
    public MeshedSurface<face>,
    public sampledSurface
{
    // Private Data

        //- Name of patches
        const wordReList patchNames_;

        //- Corresponding patchIDs
        mutable labelList patchIndices_;

        //- Triangulated faces or keep faces as is
        bool triangulate_;

        //- Track if the surface needs an update
        mutable bool needsUpdate_;

        //- For every face (or triangle) the originating patch
        labelList patchIndex_;

        //- For every face (or triangle) the index in the originating patch
        labelList patchFaceLabels_;

        //- Start indices (in patchFaceLabels_) of patches
        labelList patchStart_;


    // Private Member Functions

        //- Sample field on faces
        template<class Type>
        tmp<Field<Type>> sampleField
        (
            const VolField<Type>& vField
        ) const;

        //- Sample surface field on faces
        template<class Type>
        tmp<Field<Type>> sampleField
        (
            const SurfaceField<Type>& sField
        ) const;

        template<class Type>
        tmp<Field<Type>> interpolateField(const interpolation<Type>&) const;

        //- Re-map action on triangulation or cleanup
        virtual void remapFaces(const labelUList& faceMap);


protected:

        const wordReList& patchNames() const
        {
            return patchNames_;
        }

        const labelList& patchIndices() const;

        const labelList& patchStart() const
        {
            return patchStart_;
        }

        const labelList& patchFaceLabels() const
        {
            return patchFaceLabels_;
        }


public:

    //- Runtime type information
    TypeName("patch");


    // Constructors

        //- Construct from components
        patch
        (
            const word& name,
            const polyMesh& mesh,
            const wordReList& patchNames,
            const bool triangulate = false
        );

        //- Construct from dictionary
        patch
        (
            const word& name,
            const polyMesh& mesh,
            const dictionary& dict
        );


    //- Destructor
    virtual ~patch();


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
            ) const;                                                           \
                                                                               \
            virtual tmp<Field<Type>> sample                                    \
            (                                                                  \
                const SurfaceField<Type>&                                      \
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
    #include "sampledPatchTemplates.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
