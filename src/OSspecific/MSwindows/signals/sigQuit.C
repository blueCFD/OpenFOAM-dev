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
    sigQuit

\*---------------------------------------------------------------------------*/

#include "error.H"
#include "sigQuit.H"
#include "jobInfo.H"
#include "IOstreams.H"

// SIGBREAK is best alternative to SIGQUIT on windows

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

__p_sig_fn_t Foam::sigQuit::oldAction_ = SIG_DFL;

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void Foam::sigQuit::sigQuitHandler(int)
{
    // Reset old handling
    const __p_sig_fn_t success = ::signal(SIGBREAK, oldAction_);

    if (SIG_ERR == success)
    {
        FatalErrorIn("Foam::sigQuit::sigQuitHandler()")   
            << "Cannot reset SIGBREAK trapping"
            << abort(FatalError);    
    }

    // Update jobInfo file
    jobInfo_.signalEnd();

    error::printStack(Perr);

    // Throw signal (to old handler)
    ::raise(SIGBREAK);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::sigQuit::sigQuit()
{
    oldAction_ = SIG_DFL;
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::sigQuit::~sigQuit()
{
    // Reset old handling
    if (SIG_DFL != oldAction_)
    {
        const __p_sig_fn_t success = ::signal(SIGBREAK, oldAction_);
        oldAction_ = SIG_DFL;

        if (SIG_ERR == success)
        {
            FatalErrorIn("Foam::sigQuit::~sigQuit()")
                << "Cannot reset SIGBREAK trapping"
                << abort(FatalError);    
        }
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::sigQuit::set(const bool verbose)
{
    if (SIG_DFL != oldAction_)
    {
        FatalErrorIn("Foam::sigQuit::set()")   
            << "Cannot call sigQuit::set() more than once"
            << abort(FatalError);
    }

    oldAction_ = ::signal(SIGBREAK, &Foam::sigQuit::sigQuitHandler);        

    if (SIG_ERR == oldAction_)
    {
        oldAction_ = SIG_DFL;

        // Not a FatalErrorIn or abort because fails under wine
        WarningIn("Foam::sigQuit::set()")
            << "Cannot set SIGBREAK trapping" << endl;
    }
}


// ************************************************************************* //
