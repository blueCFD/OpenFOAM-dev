/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2011-2023 OpenFOAM Foundation
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
    Foam::FvWallInfo

Description
    Holds information regarding nearest wall point. Used in wall distance
    calculation.

SourceFiles
    FvWallInfoI.H

\*---------------------------------------------------------------------------*/

#ifndef FvWallInfo_H
#define FvWallInfo_H

#include "fieldTypes.H"
#include "labelPair.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward declaration of classes
class fvPatch;
class fvMesh;
class transformer;

/*---------------------------------------------------------------------------*\
                          Class FvWallInfo Declaration
\*---------------------------------------------------------------------------*/

template<class WallLocation>
class FvWallInfo
:
    public WallLocation
{
public:

    // Constructors

        //- Inherit constructors
        using WallLocation::WallLocation;


    // Member Functions

        // Needed by FvFaceCellWave

            //- Check for identical geometrical data. Used for cyclics checking.
            template<class TrackingData>
            inline bool sameGeometry
            (
                const fvMesh& mesh,
                const FvWallInfo<WallLocation>& w2,
                const scalar tol,
                TrackingData& td
            ) const;

            //- Transform across an interface
            template<class TrackingData>
            inline void transform
            (
                const fvPatch& patch,
                const label patchFacei,
                const transformer& transform,
                TrackingData& td
            );

            //- Influence of neighbouring face.
            template<class TrackingData>
            inline bool updateCell
            (
                const fvMesh&,
                const label thisCelli,
                const labelPair& neighbourPatchAndFacei,
                const FvWallInfo<WallLocation>& neighbourInfo,
                const scalar tol,
                TrackingData& td
            );

            //- Influence of neighbouring cell.
            template<class TrackingData>
            inline bool updateFace
            (
                const fvMesh&,
                const labelPair& thisPatchAndFacei,
                const label neighbourCelli,
                const FvWallInfo<WallLocation>& neighbourInfo,
                const scalar tol,
                TrackingData& td
            );

            //- Influence of different value on same face.
            template<class TrackingData>
            inline bool updateFace
            (
                const fvMesh&,
                const labelPair& thisPatchAndFacei,
                const FvWallInfo<WallLocation>& neighbourInfo,
                const scalar tol,
                TrackingData& td
            );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

class wallPoint;

template<>
inline bool contiguous<FvWallInfo<wallPoint>>()
{
    return true;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class WallLocation, class Type>
class WallLocationData;

#define DefineContiguousFvWallLocationDataType(Type, nullArg)                 \
    template<>                                                                \
    inline bool contiguous<FvWallInfo<WallLocationData<wallPoint, Type>>>()   \
    {                                                                         \
        return true;                                                          \
    }

DefineContiguousFvWallLocationDataType(bool, );
DefineContiguousFvWallLocationDataType(label, );
FOR_ALL_FIELD_TYPES(DefineContiguousFvWallLocationDataType);

#undef DefineContiguousFvWallLocationDataType


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "FvWallInfoI.T.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
