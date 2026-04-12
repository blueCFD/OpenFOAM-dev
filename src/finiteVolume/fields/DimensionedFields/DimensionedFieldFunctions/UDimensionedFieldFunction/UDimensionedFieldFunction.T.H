/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2026 OpenFOAM Foundation
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
    Foam::UDimensionedFieldFunction

Description
    Combined class containing a field reference, a dimensionedField wrapper
    providing the corresponding mesh and dimensions and a run-time selected
    function to evaluate and update the field.

    Particularly applicable to time and/or space varying boundary conditions.

See also
    Foam::DimensionedFieldFunction

SourceFiles
    UDimensionedFieldFunction.C

\*---------------------------------------------------------------------------*/

#ifndef UDimensionedFieldFunction_H
#define UDimensionedFieldFunction_H

#include "SlicedDimensionedField.T.H"
#include "DimensionedFieldFunction.T.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward declaration of friend functions
template<class Type, class GeoMesh>
class UDimensionedFieldFunction;

template<class Type, class GeoMesh>
void writeEntry
(
    Ostream& os,
    const UDimensionedFieldFunction<Type, GeoMesh>& udff
);

/*---------------------------------------------------------------------------*\
                    Class UDimensionedFieldFunction Declaration
\*---------------------------------------------------------------------------*/

template<class Type, class GeoMesh>
class UDimensionedFieldFunction
{
    // Private Data

        //- Reference to field
        Field<Type>& field_;

        //- Name of the function entry/sub dictionary
        const word funcName_;

        //- Dimensioned field holding a pointer to the List in field_
        SlicedDimensionedField<Type, GeoMesh> dimensionedField_;

        //- Optional function pointer
        autoPtr<DimensionedFieldFunction<DimensionedField<Type, GeoMesh>>>
            funcPtr_;


public:

    // Constructors

        //- Construct from name, mesh, dimensions, field and dictionary
        UDimensionedFieldFunction
        (
            const word& name,
            const word& funcName,
            const GeoMesh&,
            const dimensionSet& dimensions,
            Field<Type>&,
            const dictionary&
        );

        //- Construct from name, mesh, dimensions, field, dictionary
        //  and default value
        UDimensionedFieldFunction
        (
            const word& name,
            const word& funcName,
            const GeoMesh&,
            const dimensionSet& dimensions,
            Field<Type>&,
            const dictionary&,
            const Type& defaultValue
        );

        //- Construct from given UDimensionedFieldFunction
        //  with new mesh and field
        UDimensionedFieldFunction
        (
            const UDimensionedFieldFunction<Type, GeoMesh>&,
            const GeoMesh&,
            Field<Type>&
        );

        //- Disallow copy
        UDimensionedFieldFunction
        (
            const UDimensionedFieldFunction<Type, GeoMesh>&
        ) = delete;

        //- Copy constructor setting field reference
        UDimensionedFieldFunction
        (
            const UDimensionedFieldFunction<Type, GeoMesh>&,
            Field<Type>&
        );


    // Member Functions

        //- Return the dimensioned field
        const DimensionedField<Type, GeoMesh>& field() const
        {
            return dimensionedField_;
        }

        //- Map and optionally evaluate
        void map(const bool evaluate);

        //- Reset
        void reset();

        //- Update
        bool update();

        //- Write
        void write(Ostream&) const;


    // Global functions

        friend void writeEntry <Type, GeoMesh>
        (
            Ostream& os,
            const UDimensionedFieldFunction<Type, GeoMesh>& udff
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "UDimensionedFieldFunction.T.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
