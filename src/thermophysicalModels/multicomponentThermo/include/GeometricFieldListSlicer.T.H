/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2023-2026 OpenFOAM Foundation
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
    Foam::GeometricFieldListSlicer

Description
    Class to provide list slices to different parts of a geometric field

See also
    Foam::FieldListSlice

\*---------------------------------------------------------------------------*/

#ifndef GeometricFieldListSlicer_H
#define GeometricFieldListSlicer_H

#include "GeometricField.T.H"
#include "FieldListSlice.T.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
/*---------------------------------------------------------------------------*\
                  Class GeometricFieldListSlicer Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class GeometricFieldListSlicer
{
    // Private Data

        //- Pointers to the primitive fields
        UPtrList<const Field<Type>> primitiveFields_;

        //- Pointers to the patch fields. Indexed by patch, then by field.
        List<UPtrList<const Field<Type>>> patchFields_;


public:

    // Constructors

        //- Construct null. Set later.
        inline GeometricFieldListSlicer()
        {}

        //- Construct from a mesh and a list of fields
        template<class GeoMesh, template<class> class PrimitiveField>
        inline GeometricFieldListSlicer
        (
            const GeoMesh& mesh,
            const UPtrList<GeometricField<Type, GeoMesh, PrimitiveField>>&
                geoFields
        )
        {
            set(mesh, geoFields);
        }


    // Member Functions

        //- Set the field pointers
        template<class GeoMesh, template<class> class PrimitiveField>
        inline void set
        (
            const GeoMesh& mesh,
            const UPtrList<GeometricField<Type, GeoMesh, PrimitiveField>>&
                geoFields
        )
        {
            primitiveFields_.resize(geoFields.size());

            patchFields_.resize(mesh.boundary().size(), primitiveFields_);

            forAll(geoFields, fieldi)
            {
                primitiveFields_.set
                (
                    fieldi,
                    &geoFields[fieldi].primitiveField()
                );

                forAll(geoFields[fieldi].boundaryField(), patchi)
                {
                    patchFields_[patchi].set
                    (
                        fieldi,
                        &geoFields[fieldi].boundaryField()[patchi]
                    );
                }
            }
        }

        //- Return a slice for an internal element
        inline FieldListSlice<Type> slice(const label elementi) const
        {
            return FieldListSlice<Type>(primitiveFields_, elementi);
        }

        //- Return a slice for a patch element
        inline FieldListSlice<Type> patchSlice
        (
            const label patchi,
            const label patchElementi
        ) const
        {
            return FieldListSlice<Type>(patchFields_[patchi], patchElementi);
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
