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
      - Adapted for blueCFD-Core 2017.

\*---------------------------------------------------------------------------*/

#include "error.H"
#include "OSspecific.H"

//Undefine this macro, otherwise it will collide with Windows' definitions
#undef DebugInfo

#include "stack_trace.h"
#include <sstream>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void error::safePrintStack(std::ostream& os)
{
    std::stringstream callstacktext
    (
        std::stringstream::in | std::stringstream::out
    );

    os << "Generating stack trace..." << std::endl;

    StackTrace *traceUs = new StackTrace();

    if(traceUs!=NULL)
    {
        traceUs->OutputToStream(&callstacktext);

        delete traceUs;
        traceUs=NULL;

        os << callstacktext.str().data();
    }
    else
    {
        os
            << "We are sorry, but the application crashed and stack tracing"
            << " did not work in this specific situation."
            << std::endl;
    }
}

void error::printStack(Ostream& os)
{
    std::stringstream callstacktext
    (
        std::stringstream::in | std::stringstream::out
    );

    os << "Generating stack trace..." << endl;

    StackTrace *traceUs = new StackTrace();

    if(traceUs!=NULL)
    {
        traceUs->OutputToStream(&callstacktext);

        delete traceUs;
        traceUs=NULL;

        os << callstacktext.str().data();
    }
    else
    {
        os
            << "We are sorry, but the application crashed and stack tracing"
            << " did not work in this specific situation."
            << endl;
    }
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
