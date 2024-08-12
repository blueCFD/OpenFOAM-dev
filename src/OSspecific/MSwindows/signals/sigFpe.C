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
    sigFpe

\*---------------------------------------------------------------------------*/

#include "error.T.H"
#include "sigFpe.H"

#include "jobInfo.H"
#include "OSspecific.H"
#include "IOstreams.H"

// We need to unset the strict ANSI marker, so that we can use the special
// signal functions
#ifdef __STRICT_ANSI__
#undef __STRICT_ANSI__
#endif

#include <float.h> // *fp functions
#include <limits>


// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

__p_sig_fn_t Foam::sigFpe::oldAction_ = SIG_DFL;

static unsigned int fpOld_ = 0;

void Foam::sigFpe::fillNan(UList<scalar>& lst)
{
    lst = std::numeric_limits<scalar>::signaling_NaN();
}

static void clearFpe()
{
    _clearfp ();
    _controlfp (fpOld_, 0xFFFFFFFF);
}


void Foam::sigFpe::sigFpeHandler(int)
{
    const __p_sig_fn_t success = ::signal(SIGFPE, oldAction_);

    // Reset old handling
    if (SIG_ERR == success)
    {
        FatalErrorIn
        (
            "Foam::sigSegv::sigFpeHandler()"
        )   << "Cannot reset SIGFPE trapping"
            << abort(FatalError);    
    }

    // Update jobInfo file
    jobInfo_.signalEnd();

    error::printStack(Perr);

    clearFpe();

    // Throw signal (to old handler)
    ::raise(SIGFPE);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::sigFpe::sigFpe()
{
    oldAction_ = SIG_DFL;
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::sigFpe::~sigFpe()
{
    if (env("FOAM_SIGFPE"))
    {
        clearFpe();

        // Reset signal
        const __p_sig_fn_t success = ::signal(SIGFPE, oldAction_);
        oldAction_ = SIG_DFL;

        if (SIG_ERR == success)
        {
            FatalErrorIn
            (
                "Foam::sigFpe::~sigFpe()"
            )   << "Cannot reset SIGFPE trapping"
                << abort(FatalError);    
        }
    }

    if (env("FOAM_SETNAN"))
    {
        WarningIn("Foam::sigFpe::~sigFpe()")
            << "FOAM_SETNAN not supported under MSwindows "
            << endl;
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::sigFpe::set(const bool verbose)
{
    if (SIG_DFL != oldAction_)
    {
        FatalErrorIn
        (
            "Foam::sigFpe::set()"
        )   << "Cannot call sigFpe::set() more than once"
            << abort(FatalError);
    }

    if (env("FOAM_SIGFPE"))
    {
        if (verbose)
        {
            Info<< "SigFpe : Enabling floating point exception trapping"
                << " (FOAM_SIGFPE)." << endl;
        }

        fpOld_ = _controlfp(0, 0);
        const unsigned int fpNew = 
          fpOld_ & ~(_EM_ZERODIVIDE | _EM_INVALID | _EM_OVERFLOW);
        _controlfp(fpNew, _MCW_EM);

        oldAction_ = ::signal(SIGFPE, &Foam::sigFpe::sigFpeHandler);        

        if (SIG_ERR == oldAction_)
        {
            oldAction_ = SIG_DFL;

            FatalErrorIn
            (
                "Foam::sigFpe::set()"
            )   << "Cannot set SIGFPE trapping"
                << abort(FatalError);    
        }
    }


    if (env("FOAM_SETNAN"))
    {
        if (verbose)
        {
            WarningIn("Foam::sigFpe::set()")
              << "FOAM_SETNAN not supported under MSwindows "
              << endl;
        }
    }
}


// ************************************************************************* //
