/*---------------------------------------------------------------------------*\
License
    Copyright            : (C) 2011 Symscape
    Website              : www.symscape.com

    Copyright            : (C) 2013 blueCAPE Lda
    Website              : www.bluecape.com.pt

    This file is part of blueCAPE's unofficial mingw patches for OpenFOAM.
    For more information about these patches, visit:
         http://bluecfd.com/Core

    OpenFOAM's official site: http://www.openfoam.com

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
    MSwindows

Description
    Has some auxiliary definitions of methods/functions.

SourceFiles
    MSwindows.H

\*---------------------------------------------------------------------------*/

#ifndef MSwindows_H
#define MSwindows_H

#include <string>

#include "className.H"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace MSwindows
{
    //- Declare name of the class and it's debug switch
    NamespaceName("MSwindows");

    //- Get last windows api error from GetLastError
    Foam::string getLastError();
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
