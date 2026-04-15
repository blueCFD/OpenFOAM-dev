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
    Foam::sampledSurfaces::thresholdCellFaces

Description
    A sampledSurface defined by the cell faces corresponding to a threshold
    value.

    Example:
    \verbatim
    {
        type        thresholdCellFaces;
        field       p;
        lowerLimit  5;
        upperLimit  10;
        interpolate yes;
    }
    \endverbatim

Usage
    \table
        Property     | Description             | Required    | Default value
        field        | the field to threshold  | yes         |
        lowerLimit   | the limit below which cells are omitted \\
                     | if no upperLimit | -vGreat
        upperLimit   | the limit above which cells are omitted \\
                     | if no lowerLimit | +vGreat
        interpolate  | interpolate values to the surface points | no | no
    \endtable

SourceFiles
    sampledThresholdCellFaces.C

\*---------------------------------------------------------------------------*/

#ifndef sampledThresholdCellFaces_H
#define sampledThresholdCellFaces_H

#include "sampledSurface.H"
#include "MeshedSurface.T.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace sampledSurfaces
{

/*---------------------------------------------------------------------------*\
                     Class thresholdCellFaces Declaration
\*---------------------------------------------------------------------------*/

class thresholdCellFaces
:
    public sampledSurface,
    public MeshedSurface<face>
{
    // Private Data

        //- Field to get threshold
        const word fieldName_;

        //- Threshold value
        const scalar lowerThreshold_;

        //- Threshold value
        const scalar upperThreshold_;

        //- Triangulated faces or keep faces as is
        bool triangulate_;

        //- Time at last call, also track it surface needs an update
        mutable label prevTimeIndex_;

        //- For every face the original cell in mesh
        mutable labelList meshCells_;


    // Private Member Functions

        //- Create surface (if time has changed)
        //  Do nothing (and return false) if no update was needed
        bool updateGeometry() const;

        //- Sample field on faces
        template<class Type>
        tmp<Field<Type>> sampleField
        (
            const VolField<Type>& vField
        ) const;


        template<class Type>
        tmp<Field<Type>>
        interpolateField(const interpolation<Type>&) const;


public:

    //- Runtime type information
    TypeName("thresholdCellFaces");


    // Constructors

        //- Construct from dictionary
        thresholdCellFaces
        (
            const word& name,
            const polyMesh&,
            const dictionary&
        );


    //- Destructor
    virtual ~thresholdCellFaces();


    // Member Functions

        //- Return the list of fields required
        virtual wordList fields() const;

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
    #include "sampledThresholdCellFacesTemplates.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
