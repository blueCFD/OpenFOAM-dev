/*---------------------------------------------------------------------------*\
 License

    Copyright (C) 2011-2014 blueCAPE Lda

    This file is part of blueCAPE's unofficial mingw patches for OpenFOAM.
    For more information about these patches, visit:
         http://bluecfd.com/Core

    This file is a derivative work of OpenFOAM.

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

Application
    Test-Random

Description
    Test application for Random number generation.

    Derived from the patches for blueCFD 2.1 and 2.2.

\*---------------------------------------------------------------------------*/

#include "Random.H"
#include "cpuTime.H"

using namespace Foam;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    Info<< "Initialize Random object..." << endl;
    Random rnd(123456);
    int maxIter=100, maxInteger=100000;

    Info<< "Initialization complete." << endl;

    Info<< "Do "<< maxIter <<" scalar01, integer(0,"<<maxInteger<<") and bit()"<< endl;
    Info<< "Iter    scalar01     integer     bit"<< endl;
    for(int i=0; i<maxIter; i++) {
      Info<< i << "\t " << rnd.scalar01() << "\t " << rnd.integer(0,maxInteger) << "\t " << rnd.bit() << endl;
    }
    Info<< nl << "Done." << endl;

    return 0;
}


// ************************************************************************* //
