/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2011-2022 OpenFOAM Foundation
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
    Foam::wallPoint

Description
    Holds information regarding nearest wall point. Used in wall distance
    calculation.

SourceFiles
    wallPointI.H

\*---------------------------------------------------------------------------*/

#ifndef wallPoint_H
#define wallPoint_H

#include "pointField.H"
#include "face.T.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward declaration of classes
class polyPatch;
class polyMesh;
class transformer;

// Forward declaration of friend functions and operators
template<class Derived>
class WallPointBase;
template<class Derived>
Ostream& operator<<(Ostream&, const WallPointBase<Derived>&);
template<class Derived>
Istream& operator>>(Istream&, WallPointBase<Derived>&);

/*---------------------------------------------------------------------------*\
                          Class WallPointBase Declaration
\*---------------------------------------------------------------------------*/

template<class Derived>
class WallPointBase
{
    // Private Data

        //- Position of nearest wall center
        point origin_;

        //- Normal distance (squared) from cellcenter to origin
        scalar distSqr_;


protected:

    // Protected Member Functions

        //- Evaluate distance to point. Update distSqr, origin from whomever
        //  is nearer pt. Return true if w2 is closer to point,
        //  false otherwise.
        template<class TrackingData>
        inline bool update
        (
            const point&,
            const WallPointBase<Derived>& w2,
            const scalar tol,
            TrackingData& td
        );


public:

    // Constructors

        //- Construct null
        inline WallPointBase();

        //- Construct from origin, distance
        inline WallPointBase(const point& origin, const scalar distSqr);

        //- Construct from face, distance
        inline WallPointBase
        (
            const face& f,
            const pointField& ps,
            const point& centre,
            const scalar distSqr
        );


    // Member Functions

        // Access

            inline const point& origin() const;

            inline point& origin();

            inline scalar distSqr() const;

            inline scalar& distSqr();

            template<class TrackingData>
            inline scalar dist(TrackingData& td) const;


        // Needed by FaceCellWave

            //- Check whether origin has been changed at all or
            //  still contains original (invalid) value.
            template<class TrackingData>
            inline bool valid(TrackingData& td) const;

            //- Check for identical geometrical data. Used for cyclics checking.
            template<class TrackingData>
            inline bool sameGeometry
            (
                const polyMesh&,
                const WallPointBase<Derived>&,
                const scalar,
                TrackingData& td
            ) const;

            //- Transform across an interface
            template<class TrackingData>
            inline void transform
            (
                const polyPatch& patch,
                const label patchFacei,
                const transformer& transform,
                TrackingData& td
            );

            //- Influence of neighbouring face.
            template<class TrackingData>
            inline bool updateCell
            (
                const polyMesh&,
                const label thisCelli,
                const label neighbourFacei,
                const WallPointBase<Derived>& neighbourInfo,
                const scalar tol,
                TrackingData& td
            );

            //- Influence of neighbouring cell.
            template<class TrackingData>
            inline bool updateFace
            (
                const polyMesh&,
                const label thisFacei,
                const label neighbourCelli,
                const WallPointBase<Derived>& neighbourInfo,
                const scalar tol,
                TrackingData& td
            );

            //- Influence of different value on same face.
            template<class TrackingData>
            inline bool updateFace
            (
                const polyMesh&,
                const label thisFacei,
                const WallPointBase<Derived>& neighbourInfo,
                const scalar tol,
                TrackingData& td
            );

            //- Same (like operator==)
            template<class TrackingData>
            inline bool equal
            (
                const WallPointBase<Derived>&,
                TrackingData& td
            ) const;


    // Member Operators

        inline bool operator==(const WallPointBase<Derived>&) const;
        inline bool operator!=(const WallPointBase<Derived>&) const;


    // IOstream Operators

        friend Ostream& operator<< <Derived>
        (
            Ostream&,
            const WallPointBase<Derived>&
        );

        friend Istream& operator>> <Derived>
        (
            Istream&,
            WallPointBase<Derived>&
        );
};


/*---------------------------------------------------------------------------*\
                          Class wallPoint Declaration
\*---------------------------------------------------------------------------*/

class wallPoint
:
    public WallPointBase<wallPoint>
{
    public:

        using WallPointBase<wallPoint>::WallPointBase;

        template<class Derived> using type = WallPointBase<Derived>;
};


//- Data associated with wallPoint type are contiguous
template<>
inline bool contiguous<wallPoint>()
{
    return true;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "wallPointI.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
