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
     Foam::sampledSurfaces::distanceSurface

Description
    A sampledSurface defined by a distance to a surface.

    Example:
    \verbatim
    {
        type        distanceSurface;
        surfaceType searchableBox;
        min         (-1 -1 -1);
        max         (1 1 1);
        distance    0.1;
        signed      yes;
        interpolate yes;
    }
    \endverbatim

Usage
    \table
        Property    | Description             | Required    | Default value
        surfaceType | The type of surface to sample from | yes |
        distance    | The distance from which to sample the surface | yes |
        signed      | Sample only on one side of the surface? | yes |
        interpolate | Interpolate values to the surface points? | no | no
        cellZone    | Zone containing cells to include | no | all
    \endtable

See also
    Foam::generatedCellZone

SourceFiles
    sampledDistanceSurface.C

\*---------------------------------------------------------------------------*/

#ifndef sampledDistanceSurface_H
#define sampledDistanceSurface_H

#include "sampledIsoSurfaceSurface.H"
#include "searchableSurface.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace sampledSurfaces
{

/*---------------------------------------------------------------------------*\
                       Class distanceSurface Declaration
\*---------------------------------------------------------------------------*/

class distanceSurface
:
    public sampledIsoSurfaceSurface
{
    // Private Data

        //- Surface
        const autoPtr<searchableSurface> surfPtr_;

        //- Distance value
        const scalar distance_;

        //- Is the distance signed? The surface must be closed if so.
        const bool signed_;


    // Private Member Functions

        //- Generate the iso surface
        virtual autoPtr<cutPolyIsoSurface> calcIsoSurf() const;


public:

    //- Runtime type information
    TypeName("distanceSurface");


    // Constructors

        //- Construct from dictionary
        distanceSurface
        (
            const word& name,
            const polyMesh& mesh,
            const dictionary& dict
        );


    //- Destructor
    virtual ~distanceSurface();


    // Member Functions

        //- Does the surface need an update?
        virtual bool needsUpdate() const;

        //- Write
        virtual void print(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace sampledSurfaces
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
