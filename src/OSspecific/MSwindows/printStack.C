/*---------------------------------------------------------------------------*\

License
    This file is part of blueCAPE's unofficial mingw patches for OpenFOAM.
    For more information about these patches, visit:
         http://bluecfd.com/Core

    This file is a derivative work of OpenFOAM.

    OpenFOAM is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Modifications
    This file is based on the original version for POSIX:
        OpenFOAM/src/OSspecific/POSIX/

    This file has been created by blueCAPE's unofficial mingw patches for
    OpenFOAM.
    For more information about these patches, visit:
        http://bluecfd.com/Core

    Modifications made:
      - Derived from the patches for blueCFD 2.1 and 2.2.
      - Adapted to OpenFOAM 2.2.

\*---------------------------------------------------------------------------*/

#include "error.H"
#include "OSspecific.H"
#include <sstream>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void error::safePrintStack(std::ostream& os)
{
    os << "We're sorry, but the application crashed and safe stack tracing"
          " isn't available in this current implementation of blueCFD-Core"
          " patches for OpenFOAM."
       << std::endl
       << "For more details on when this is fixed, please visit: "
       << "https://github.com/blueCFD/Core/issues/7"
       << std::endl;
}

void error::printStack(Ostream& os)
{
    os << "We're sorry, but the application crashed and safe stack tracing"
          " isn't available in this current implementation of blueCFD-Core"
          " patches for OpenFOAM."
       << nl
       << "For more details on when this is fixed, please visit: "
       << "https://github.com/blueCFD/Core/issues/7"
       << endl;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
