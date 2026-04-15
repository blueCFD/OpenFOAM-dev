/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2022-2026 OpenFOAM Foundation
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
     Foam::sampledSurfaces::sampledIsoSurfaceSurface

Description
    A base class for sampled surfaces constructed from iso-surfaces

SourceFiles
    sampledIsoSurfaceSurface.C

\*---------------------------------------------------------------------------*/

#ifndef sampledIsoSurfaceSurface_H
#define sampledIsoSurfaceSurface_H

#include "sampledSurface.H"
#include "generatedCellZone.H"
#include "cutPolyIsoSurface.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace sampledSurfaces
{

/*---------------------------------------------------------------------------*\
                       Class sampledIsoSurfaceSurface Declaration
\*---------------------------------------------------------------------------*/

class sampledIsoSurfaceSurface
:
    public sampledSurface
{
private:

    // Private Data

        //- Optional cellZone in which to compute the iso-surface
        mutable generatedCellZone zone_;

        //- Constructed iso-surface
        mutable autoPtr<cutPolyIsoSurface> isoSurfPtr_;

        //- Time index at last surface generation
        mutable label isoSurfTimeIndex_;


    // Private Member Functions

        //- Generate the iso-surface
        virtual autoPtr<cutPolyIsoSurface> calcIsoSurf() const = 0;

        //- Sample field on the surface's faces
        template<class Type>
        tmp<Field<Type>> sampleField
        (
            const VolField<Type>& vField
        ) const;

        //- Interpolate field to the surface's points
        template<class Type>
        tmp<Field<Type>> interpolateField(const interpolation<Type>&) const;


protected:

    // Protected Member Functions

        //- Generate the iso-surface for given point and iso-values
        autoPtr<cutPolyIsoSurface> calcIsoSurf
        (
            const scalarField& pointValues,
            const scalar isoValue
        ) const;

        //- Access the time index
        inline const label& timeIndex() const
        {
            return isoSurfTimeIndex_;
        }


public:

    //- Runtime type information
    TypeName("sampledIsoSurfaceSurface");


    // Constructors

        //- Construct from dictionary
        sampledIsoSurfaceSurface
        (
            const word& name,
            const polyMesh& mesh,
            const dictionary& dict
        );


    //- Destructor
    virtual ~sampledIsoSurfaceSurface();


    // Member Functions

        //- Update the surface as required.
        //  Do nothing (and return false) if no update was needed
        virtual bool update();

        //- Update the surface as required.
        //  Do nothing (and return false) if no update was needed
        virtual bool update() const;

        //- Points of surface
        virtual const pointField& points() const
        {
            return isoSurfPtr_->points();
        }

        //- Faces of surface
        virtual const faceList& faces() const
        {
            return isoSurfPtr_->faces();
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


        // Mesh changes

            //- Update for mesh point-motion
            virtual void movePoints();

            //- Update topology using the given map
            virtual void topoChange(const polyTopoChangeMap&);

            //- Update from another mesh using the given map
            virtual void mapMesh(const polyMeshMap&);

            //- Redistribute or update using the given distribution map
            virtual void distribute(const polyDistributionMap&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace sampledSurfaces
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "sampledIsoSurfaceSurfaceTemplates.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
