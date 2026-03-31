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
    Foam::typeName

Description
    Template function which returns the un-mangled name of a given type. Useful
    for types which do not have a type() function or typeName static data
    defined, and/or for printing information about complex templated object
    structures.

SourceFiles
    typeName.C

\*---------------------------------------------------------------------------*/

#ifndef typeName_H
#define typeName_H

#include "word.H"
#include <typeinfo>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//- Return the un-mangled name given the standard type info
template<class String>
String typeName(const std::type_info& info);

//- Return the un-mangled name as a string, given the standard type info.
//  Returns what CXXABI provides, with the 'Foam::' namespace removed.
template<>
string typeName<string>(const std::type_info& info);

//- Return the un-mangled name as a word, given the standard type info.
//  Returns what CXXABI provides, with the 'Foam::' namespace removed, with
//  white-space removed (to make a valid word), and with names camel-cased
//  where necessary (i.e., where white-space was delimiting).
template<>
word typeName<word>(const std::type_info& info);

//- Return the un-mangled name of the given type
template<class Type, class String = string>
inline word typeName()
{
    return typeName<String>(typeid(Type));
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
