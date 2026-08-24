/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2025-2026 OpenFOAM Foundation
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
    Foam::DimensionedFieldListSlicer

Description
    Class to provide list slices to different parts of a dimensioned field

See also
    Foam::FieldListSlice

\*---------------------------------------------------------------------------*/

#ifndef DimensionedFieldListSlicer_H
#define DimensionedFieldListSlicer_H

#include "DimensionedField.T.H"
#include "FieldListSlicer.T.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
/*---------------------------------------------------------------------------*\
                  Class DimensionedFieldListSlicer Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class DimensionedFieldListSlicer
:
    public FieldListSlicer<Type>
{
public:

    // Constructors

        //- Construct null. Set later.
        inline DimensionedFieldListSlicer()
        {}

        //- Construct from a list of dimensioned fields
        template<class DimField>
        inline DimensionedFieldListSlicer(const UPtrList<DimField>& dimFields)
        {
            set(dimFields);
        }

        //- Construct from a sub-range of a list of dimensioned fields
        template<class DimField>
        inline DimensionedFieldListSlicer
        (
            const UPtrList<DimField>& dimFields,
            const label size,
            const label start
        )
        {
            set(dimFields, size, start);
        }


    // Member Functions

        //- Set the field pointers
        template<class DimField>
        inline void set(const UPtrList<DimField>& dimFields)
        {
            FieldListSlicer<Type>::set
            (
                [&](const label geoFieldi) -> decltype(auto)
                {
                    return dimFields[geoFieldi].primitiveField();
                },
                dimFields.size()
            );
        }

        //- Set the field pointers to a specified sub-range
        template<class DimField>
        inline void set
        (
            const UPtrList<DimField>& dimFields,
            const label size,
            const label start
        )
        {
            FieldListSlicer<Type>::set
            (
                [&](const label geoFieldi)
                {
                    return
                        SubList<Type>
                        (
                            dimFields[geoFieldi].primitiveField(),
                            size,
                            start
                        );
                },
                dimFields.size()
            );
        }
};


/*---------------------------------------------------------------------------*\
                Class DimensionedFieldListAndSlicer Declaration
\*---------------------------------------------------------------------------*/

template<class Type, class GeoMesh, template<class> class PrimitiveField=Field>
class DimensionedFieldListAndSlicer
:
    public PtrList<DimensionedField<Type, GeoMesh, PrimitiveField>>,
    private DimensionedFieldListSlicer<Type>
{
public:

    // Constructors

        //- Null Constructor
        DimensionedFieldListAndSlicer()
        {}

        //- Construct with size specified
        explicit DimensionedFieldListAndSlicer(const label size)
        :
            PtrList<DimensionedField<Type, GeoMesh, PrimitiveField>>(size)
        {}

        //- Construct from arguments
        template<class ... Args>
        DimensionedFieldListAndSlicer(Args ... args)
        :
            PtrList<DimensionedField<Type, GeoMesh, PrimitiveField>>(args ...)
        {
            set();
        }


    // Member Functions

        //- Inherit the pointer list's set method
        using PtrList<DimensionedField<Type, GeoMesh, PrimitiveField>>::set;

        //- Set the field pointers in the slicer
        inline void set()
        {
            DimensionedFieldListSlicer<Type>::set(*this);
        }

        //- Return a slice for an internal element
        using DimensionedFieldListSlicer<Type>::slice;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
