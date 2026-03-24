/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2024-2026 OpenFOAM Foundation
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
    Foam::unitSet

Description
    Unit conversion structure. Contains the associated dimensions and the
    multiplier with which to convert values.

SourceFiles
    unitSet.C
    unitSetIO.C
    unitSetI.H

\*---------------------------------------------------------------------------*/

#ifndef unitSet_H
#define unitSet_H

#include "dimensionSet.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward declaration of classes
class Istream;
class Ostream;
template<class Type> class List;
template<class Type> class Field;

// Forward declaration of friend functions and operators
class unitSet;
unitSet pow(const unitSet&, const scalar);
const unitSet& operator+(const unitSet&, const unitSet&);
unitSet operator*(const unitSet&, const unitSet&);
unitSet operator/(const unitSet&, const unitSet&);
Istream& operator>>(Istream&, unitSet&);
Ostream& operator<<(Ostream&, const unitSet&);
Ostream& operator<<(Ostream&, const InfoProxy<unitSet>&);

/*---------------------------------------------------------------------------*\
                          Class unitSet Declaration
\*---------------------------------------------------------------------------*/

class unitSet
{
public:

    // Member constants

        //- Define an enumeration for the number of dimensionless units
        enum
        {
            nDimlessUnits = 2
        };

        //- Define an enumeration for the names of the dimensionless unit
        //  exponents
        enum dimlessUnitType
        {
            FRACTION,           // fraction   %
            ANGLE               // angle      rad, rot, deg
        };

        //- Names of the dimensionless units
        static const NamedEnum<dimlessUnitType, 2> dimlessUnitTypeNames_;


    // Static Data Members

        //- A small exponent with which to perform inexact comparisons
        static const scalar smallExponent;


private:

    // Private Data

        //- The dimensions
        dimensionSet dimensions_;

        //- Array of dimensionless unit exponents
        scalar exponents_[nDimlessUnits];

        //- The conversion multiplier with which to multiply quantities
        //  in this unit in order to recover the quantity in standard units
        scalar multiplier_;


    // Private Member Functions

        //- Compare two unit conversions and return if they are the same.
        //  An additional control determines whether the multiplier is compared
        //  as well as the dimensions and dimensionless units.
        static bool compare
        (
            const unitSet&,
            const unitSet&,
            const bool compareMultiplier
        );


public:

    // Static Data Members

        //- Run time type information
        ClassName("unitSet");


    // Constructors

        //- Construct from components
        unitSet
        (
            const dimensionSet&,
            const scalar fraction,
            const scalar angle,
            const scalar multiplier
        );

        //- Construct from a dimension set. No dimensionless units. Unity
        //  multiplier.
        unitSet(const dimensionSet&);

        //- Copy constructor
        unitSet(const unitSet&) = default;

        //- Move constructor
        unitSet(unitSet&&) = default;

        //- Construct from stream
        unitSet(Istream& is);


    // Member Functions

        //- Access the dimensions
        inline const dimensionSet& dimensions() const;

        //- Convert a value to standard units
        template<class T>
        T toStandard(const T&) const;

        //- Convert a pair of values to standard units
        template<class T>
        Pair<T> toStandard(const Pair<T>&) const;

        //- Convert a list of values to standard units
        template<class T>
        List<T> toStandard(const List<T>&) const;

        //- Convert a field of values to standard units
        template<class T>
        tmp<Field<T>> toStandard(const Field<T>&) const;

        //- Convert a tmp field of values to standard units
        template<class T>
        tmp<Field<T>> toStandard(const tmp<Field<T>>&) const;

        //- Convert a value to standard units
        template<class T>
        void makeStandard(T&) const;

        //- Convert a pair of values to standard units
        template<class T>
        void makeStandard(Pair<T>&) const;

        //- Convert a list of values to standard units
        template<class T>
        void makeStandard(List<T>&) const;

        //- Convert a value to user units
        template<class T>
        T toUser(const T&) const;

        //- Convert a pair of values to user units
        template<class T>
        Pair<T> toUser(const Pair<T>&) const;

        //- Convert a list of values to user units
        template<class T>
        List<T> toUser(const List<T>&) const;

        //- Convert a field of values to user units
        template<class T>
        tmp<Field<T>> toUser(const Field<T>&) const;

        //- Convert a tmp field of values to user units
        template<class T>
        tmp<Field<T>> toUser(const tmp<Field<T>>&) const;

        //- Return whether this is the "any" unit. I.e., the case where
        //  dimensions and dimensionless units are not checked, and any
        //  conversion is permitted.
        inline bool any() const;

        //- Return whether this is the "none" unit. I.e., the case where unit
        //  conversions are prohibited.
        inline bool none() const;

        //- Return whether this unit is standard. I.e., is its multiplier one?
        inline bool standard() const;

        //- Reset the unit conversion
        void reset(const unitSet&);

        //- Update
        void read(const word& keyword, const dictionary&);

        //- Update
        void read(Istream& is);

        //- Update
        void read(const word& keyword, const dictionary&, Istream& is);

        //- Update if found in the dictionary
        bool readIfPresent(const word& keyword, const dictionary&);

        //- Update if found on the stream
        bool readIfPresent(Istream& is);

        //- Update if found on the dictionary stream
        bool readIfPresent(const word& keyword, const dictionary&, Istream& is);

        //- Return info proxy
        inline InfoProxy<unitSet> info() const
        {
            return *this;
        }


    // Member Operators

        //- Access the dimension exponents
        inline scalar operator[](const dimensionSet::dimensionType type) const;

        //- Access the dimensionless unit exponents
        inline scalar operator[](const dimlessUnitType type) const;

        //- Disallow default bitwise assignment
        void operator=(const unitSet&) = delete;

        //- Disallow default bitwise move assignment
        void operator=(const unitSet&&) = delete;


    // Friend Functions

        //- Raise to a power
        friend unitSet pow(const unitSet&, const scalar);


    // Friend Operators

        //- Combine
        friend const unitSet& operator+
        (
            const unitSet&,
            const unitSet&
        );

        //- Multiply
        friend unitSet operator*
        (
            const unitSet&,
            const unitSet&
        );

        //- Divide
        friend unitSet operator/
        (
            const unitSet&,
            const unitSet&
        );


    // IOstream Operators

        //- Read from stream
        friend Istream& operator>>(Istream&, unitSet&);

        //- Write to stream
        friend Ostream& operator<<(Ostream&, const unitSet&);

        //- Write info to stream
        friend Ostream& operator<<(Ostream&, const InfoProxy<unitSet>&);
};


// Global Functions

//- Write a type with a given unit conversion
template<class Type>
void writeEntry(Ostream& os, const unitSet&, const Type& t);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "unitSetI.H"

#ifdef NoRepository
    #include "unitSetTemplates.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
