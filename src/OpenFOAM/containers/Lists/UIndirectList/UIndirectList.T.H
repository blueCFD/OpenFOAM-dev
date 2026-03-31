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
    Foam::UIndirectList

Description
    A List with indirect addressing.

    Like IndirectList but does not store addressing.

    Note the const_cast of the completeList. This is so we can use it both
    on const and non-const lists. Alternative would be to have a const_
    variant etc.

SourceFiles
    UIndirectListI.H

\*---------------------------------------------------------------------------*/

#ifndef UIndirectList_H
#define UIndirectList_H

#include "List.T.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// Forward declaration of friend functions and operators
template<class T> class UIndirectList;
template<class T> void writeEntry(Ostream& os, const UIndirectList<T>&);
template<class T> Ostream& operator<<(Ostream&, const UIndirectList<T>&);

/*---------------------------------------------------------------------------*\
                        Class UIndirectList Declaration
\*---------------------------------------------------------------------------*/

template<class T>
class UIndirectList
{
    // Private Data

        //- Reference to the complete list
        UList<T>& completeList_;

        //- Indices into the complete list
        const UList<label>& addressing_;


public:

    // Constructors

        //- Construct given the complete list and the addressing array
        inline UIndirectList(const UList<T>&, const UList<label>&);

        //- Copy constructor
        UIndirectList(const UIndirectList<T>&) = default;


    // Member Functions

        // Access

            //- Return the number of elements in the list
            inline label size() const;

            //- Return true if the list is empty (ie, size() is zero).
            inline bool empty() const;

            //- Return the first element of the list.
            inline T& first();

            //- Return first element of the list.
            inline const T& first() const;

            //- Return the last element of the list.
            inline T& last();

            //- Return the last element of the list.
            inline const T& last() const;

            //- Return the forward circular index, i.e. the next index
            //  which returns to the first at the end of the list
            inline label fcIndex(const label i) const;

            //- Return the reverse circular index, i.e. the previous index
            //  which returns to the last at the beginning of the list
            inline label rcIndex(const label i) const;

            //- Return the complete list
            inline const UList<T>& completeList() const;

            //- Return the list addressing
            inline const List<label>& addressing() const;


        // Member Operators

            //- Return the addressed elements as a List
            inline List<T> operator()() const;

            //- Return non-const access to an element
            inline T& operator[](const label);

            //- Return const access to an element
            inline const T& operator[](const label) const;

            //- Assignment to UList of addressed elements
            inline void operator=(const UList<T>&);

            //- Assignment to UIndirectList of addressed elements
            inline void operator=(const UIndirectList<T>&);

            //- Assignment of all entries to the given value
            inline void operator=(const T&);

            //- Equality comparison. Compares element-by-element.
            inline bool operator==(const UIndirectList<T>&) const;

            //- Inequality comparison. Compares element-by-element.
            inline bool operator!=(const UIndirectList<T>&) const;


    // STL type definitions

        //- Type of values the UList contains.
        typedef T value_type;

        //- Type that can be used for storing into
        //  UList::value_type objects.
        typedef T& reference;

        //- Type that can be used for storing into
        //  constant UList::value_type objects
        typedef const T& const_reference;

        //- The type that can represent the difference between any two
        //  UList iterator objects.
        typedef label difference_type;

        //- The type that can represent the size of a UList.
        typedef label size_type;


    // STL iterator
    // Random access iterator for traversing UIndirectList

        //- An STL-conforming iterator
        class iterator
        {
            UList<T>& l_;

            UList<label>::const_iterator i_;

        public:

            friend class const_iterator;

            //- Construct for a given UList and addressing iterator
            inline iterator
            (
                UList<T>&,
                const UList<label>::const_iterator&
            );

            // Member Operators

                inline bool operator==(const iterator&) const;
                inline bool operator!=(const iterator&) const;

                inline T& operator*() const;
                inline T& operator()() const;

                inline iterator operator++();
                inline iterator operator++(const int);

                inline iterator operator--();
                inline iterator operator--(const int);

                inline iterator operator+=(const label);
                inline iterator operator-=(const label);

                inline iterator operator+(const label) const;
                inline iterator operator-(const label) const;

                inline label operator-(const iterator&) const;

                inline T& operator[](const label);

                inline bool operator<(const iterator&) const;
                inline bool operator>(const iterator&) const;

                inline bool operator<=(const iterator&) const;
                inline bool operator>=(const iterator&) const;
        };

        //- Return an iterator to begin traversing the UIndirectList
        inline iterator begin();

        //- Return an iterator to end traversing the UIndirectList
        inline iterator end();


    // STL const_iterator
    // Random access iterator for traversing UIndirectList

        //- An STL-conforming const_iterator
        class const_iterator
        {
            const UList<T>& l_;

            UList<label>::const_iterator i_;

        public:

            //- Construct for a given UList and addressing iterator
            inline const_iterator
            (
                const UList<T>&,
                const UList<label>::const_iterator&
            );

            // Member Operators

                inline bool operator==(const const_iterator&) const;
                inline bool operator!=(const const_iterator&) const;

                inline const T& operator*() const;
                inline const T& operator()() const;

                inline const_iterator operator++();
                inline const_iterator operator++(const int);

                inline const_iterator operator--();
                inline const_iterator operator--(const int);

                inline const_iterator operator+=(const label);
                inline const_iterator operator-=(const label);

                inline const_iterator operator+(const label) const;
                inline const_iterator operator-(const label) const;

                inline label operator-(const const_iterator&) const;

                inline const T& operator[](const label);

                inline bool operator<(const const_iterator&) const;
                inline bool operator>(const const_iterator&) const;

                inline bool operator<=(const const_iterator&) const;
                inline bool operator>=(const const_iterator&) const;
        };

        //- Return an const_iterator to begin traversing the UIndirectList
        inline const_iterator cbegin() const;

        //- Return an const_iterator to end traversing the UIndirectList
        inline const_iterator cend() const;

        //- Return an const_iterator to begin traversing the UIndirectList
        inline const_iterator begin() const;

        //- Return an const_iterator to end traversing the UIndirectList
        inline const_iterator end() const;


    // Ostream operator

        //- Write UIndirectList to Ostream
        //  Binary output is currently still a bit of a problem
        friend Ostream& operator<< <T>
        (
            Ostream&,
            const UIndirectList<T>&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "UIndirectListI.T.H"

#ifdef NoRepository
    #include "UIndirectListIO.T.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
