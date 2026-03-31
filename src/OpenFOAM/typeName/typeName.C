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

\*---------------------------------------------------------------------------*/

#include "typeName.H"

#ifdef __GNUC__

#include <cxxabi.h>

#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<>
Foam::string Foam::typeName<Foam::string>(const std::type_info& info)
{
    #ifdef __GNUC__

    // Get the un-mangled name
    int status;
    char* nameCharStar =
        abi::__cxa_demangle(info.name(), nullptr, nullptr, &status);
    string nameString(nameCharStar);
    std::free(nameCharStar);

    // Remove 'Foam::' namespace
    nameString.replaceAll("Foam::", "");

    return nameString;

    #else

    return info.name();

    #endif
}


template<>
Foam::word Foam::typeName<Foam::word>(const std::type_info& info)
{
    string nameString = typeName<string>(info);

    // Remove all white-space and camel-case as needed
    size_t i0 = 0, i1 = 0;
    bool camel = false;
    while (nameString[i0] == ' ') ++ i0;
    for (; i0 < nameString.size(); ++ i0)
    {
        if (nameString[i0] != ' ')
        {
            nameString[i1] = camel ? toupper(nameString[i0]) : nameString[i0];
            i1 ++;
            camel = false;
        }
        else if (i1 != 0 && isalnum(nameString[i1 - 1]))
        {
            camel = true;
        }
    }
    nameString.resize(i1);

    return nameString;
}


// ************************************************************************* //
