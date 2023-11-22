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

\*---------------------------------------------------------------------------*/

#include "wallPoint.H"
#include "polyMesh.H"
#include "transformer.H"
#include "SubField.T.H"

// * * * * * * * * * * * *  Protected Member Functions * * * * * * * * * * * //

template<class Derived>
template<class TrackingData>
inline bool Foam::WallPointBase<Derived>::update
(
    const point& pt,
    const WallPointBase<Derived>& w2,
    const scalar tol,
    TrackingData& td
)
{
    const scalar dist2 = magSqr(pt - w2.origin());

    if (valid(td))
    {
        const scalar diff = distSqr() - dist2;

        if (diff < 0)
        {
            // already nearer to pt
            return false;
        }

        if ((diff < small) || ((distSqr() > small) && (diff/distSqr() < tol)))
        {
            // don't propagate small changes
            return false;
        }
    }

    // Either *this is not yet valid or w2 is closer
    distSqr() = dist2;
    origin() = w2.origin();

    return true;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Derived>
inline Foam::WallPointBase<Derived>::WallPointBase()
:
    origin_(point::max),
    distSqr_(-great)
{}


template<class Derived>
inline Foam::WallPointBase<Derived>::WallPointBase
(
    const point& origin,
    const scalar distSqr
)
:
    origin_(origin),
    distSqr_(distSqr)
{}


template<class Derived>
inline Foam::WallPointBase<Derived>::WallPointBase
(
    const face& f,
    const pointField& points,
    const point& centre,
    const scalar distSqr
)
:
    origin_(centre),
    distSqr_(distSqr)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Derived>
inline const Foam::point& Foam::WallPointBase<Derived>::origin() const
{
    return origin_;
}


template<class Derived>
inline Foam::point& Foam::WallPointBase<Derived>::origin()
{
    return origin_;
}


template<class Derived>
inline Foam::scalar Foam::WallPointBase<Derived>::distSqr() const
{
    return distSqr_;
}


template<class Derived>
inline Foam::scalar& Foam::WallPointBase<Derived>::distSqr()
{
    return distSqr_;
}


template<class Derived>
template<class TrackingData>
inline Foam::scalar Foam::WallPointBase<Derived>::dist(TrackingData& td) const
{
    return valid(td) ? sqrt(distSqr_) : great;
}


template<class Derived>
template<class TrackingData>
inline bool Foam::WallPointBase<Derived>::valid(TrackingData& td) const
{
    return distSqr_ > -small;
}


template<class Derived>
template<class TrackingData>
inline bool Foam::WallPointBase<Derived>::sameGeometry
(
    const polyMesh&,
    const WallPointBase<Derived>& w2,
    const scalar tol,
    TrackingData& td
) const
{
    scalar diff = mag(distSqr() - w2.distSqr());

    if (diff < small)
    {
        return true;
    }
    else
    {
        if ((distSqr() > small) && ((diff/distSqr()) < tol))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}


template<class Derived>
template<class TrackingData>
inline void Foam::WallPointBase<Derived>::transform
(
    const polyPatch& patch,
    const label patchFacei,
    const transformer& transform,
    TrackingData& td
)
{
    // Note that distSqr_ is not affected by crossing an interface
    origin_ = transform.transformPosition(origin_);
}


template<class Derived>
template<class TrackingData>
inline bool Foam::WallPointBase<Derived>::updateCell
(
    const polyMesh& mesh,
    const label thisCelli,
    const label neighbourFacei,
    const WallPointBase<Derived>& neighbourWallInfo,
    const scalar tol,
    TrackingData& td
)
{
    return
        static_cast<Derived&>(*this).update
        (
            mesh.cellCentres()[thisCelli],
            neighbourWallInfo,
            tol,
            td
        );
}


template<class Derived>
template<class TrackingData>
inline bool Foam::WallPointBase<Derived>::updateFace
(
    const polyMesh& mesh,
    const label thisFacei,
    const label neighbourCelli,
    const WallPointBase<Derived>& neighbourWallInfo,
    const scalar tol,
    TrackingData& td
)
{
    return
        static_cast<Derived&>(*this).update
        (
            mesh.faceCentres()[thisFacei],
            neighbourWallInfo,
            tol,
            td
        );
}


template<class Derived>
template<class TrackingData>
inline bool Foam::WallPointBase<Derived>::updateFace
(
    const polyMesh& mesh,
    const label thisFacei,
    const WallPointBase<Derived>& neighbourWallInfo,
    const scalar tol,
    TrackingData& td
)
{
    return
        static_cast<Derived&>(*this).update
        (
            mesh.faceCentres()[thisFacei],
            neighbourWallInfo,
            tol,
            td
        );
}


template<class Derived>
template<class TrackingData>
inline bool Foam::WallPointBase<Derived>::equal
(
    const WallPointBase<Derived>& rhs,
    TrackingData& td
) const
{
    return operator==(rhs);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Derived>
inline bool Foam::WallPointBase<Derived>::operator==
(
    const Foam::WallPointBase<Derived>& rhs
) const
{
    return origin() == rhs.origin();
}


template<class Derived>
inline bool Foam::WallPointBase<Derived>::operator!=
(
    const Foam::WallPointBase<Derived>& rhs
) const
{
    return !(*this == rhs);
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

template<class Derived>
Foam::Ostream& Foam::operator<<(Ostream& os, const WallPointBase<Derived>& w)
{
    return os << w.origin() << token::SPACE << w.distSqr();
}


template<class Derived>
Foam::Istream& Foam::operator>>(Istream& is, WallPointBase<Derived>& w)
{
    return is >> w.origin() >> w.distSqr();
}


// ************************************************************************* //
