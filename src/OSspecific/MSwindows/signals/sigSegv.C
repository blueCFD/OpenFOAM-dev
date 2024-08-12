/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
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

Modifications
    This file is based on the original version for POSIX:
        OpenFOAM/src/OSspecific/POSIX/

    This file was developed for Windows by:
        Copyright            : (C) 2011 Symscape
        Website              : www.symscape.com

    This copy of this file has been created by blueCAPE's unofficial mingw
    patches for OpenFOAM.
    For more information about these patches, visit:
        http://bluecfd.com/Core

    Modifications made:
      - Derived from the patches for blueCFD 2.1 and 2.2.

Class
    sigSegv

\*---------------------------------------------------------------------------*/

#include "error.T.H"
#include "sigSegv.H"
#include "jobInfo.H"
#include "IOstreams.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

__p_sig_fn_t Foam::sigSegv::oldAction_ = SIG_DFL;

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void Foam::sigSegv::sigSegvHandler(int)
{
    // Reset old handling
    const __p_sig_fn_t success = ::signal(SIGSEGV, oldAction_);

    if (SIG_ERR == success)
    {
         FatalErrorIn
        (
            "Foam::sigSegv::sigSegvHandler()"
        )   << "Cannot reset SIGSEGV trapping"
            << abort(FatalError);    
    }

    // Update jobInfo file
    jobInfo_.signalEnd();

    error::printStack(Perr);

    // Throw signal (to old handler)
    ::raise(SIGSEGV);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::sigSegv::sigSegv()
{
    oldAction_ = SIG_DFL;
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::sigSegv::~sigSegv()
{
    // Reset old handling
    const __p_sig_fn_t success = ::signal(SIGSEGV, oldAction_);
    oldAction_ = SIG_DFL;

    if (SIG_ERR == success)
    {
        FatalErrorIn
        (
            "Foam::sigSegv::~sigSegv()"
        )   << "Cannot reset SIGSEGV trapping"
            << abort(FatalError);    
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::sigSegv::set(const bool verbose)
{
    if (SIG_DFL != oldAction_)
    {
        FatalErrorIn
        (
            "Foam::sigSegv::set()"
        )   << "Cannot call sigSegv::set() more than once"
            << abort(FatalError);
    }

    oldAction_ = ::signal(SIGSEGV, &Foam::sigSegv::sigSegvHandler);        

    if (SIG_ERR == oldAction_)
    {
        oldAction_ = SIG_DFL;

        FatalErrorIn
        (
            "Foam::sigSegv::set()"
        )   << "Cannot set SIGSEGV trapping"
            << abort(FatalError);    
    }
}


// ************************************************************************* //
