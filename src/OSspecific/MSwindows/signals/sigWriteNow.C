/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2016 OpenFOAM Foundation
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
        Copyright            : (C) 2012 Symscape
        Website              : www.symscape.com

    This copy of this file has been created by blueCAPE's unofficial mingw
    patches for OpenFOAM.
    For more information about these patches, visit:
        http://bluecfd.com/Core

    Modifications made:
      - Derived from the patches for blueCFD 2.1 and 2.2.

\*---------------------------------------------------------------------------*/

#include "sigWriteNow.H"
#include "error.H"
#include "jobInfo.H"
#include "IOstreams.H"
#include "Time.T.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{

// Signal number to catch
int sigWriteNow::signal_
(
    debug::optimisationSwitch("writeNowSignal", -1)
);

} // End namespace Foam


Foam::Time* Foam::sigWriteNow::runTimePtr_ = nullptr;


__p_sig_fn_t Foam::sigWriteNow::oldAction_ = SIG_DFL;


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void Foam::sigWriteNow::sigHandler(int)
{
    // Reset old handling
    const __p_sig_fn_t success = ::signal(signal_, oldAction_);
    oldAction_ = SIG_DFL;
    
    if (SIG_ERR == success)
    {
        FatalErrorIn
        (
            "Foam::sigWriteNow::sigHandler(int)"
        )   << "Cannot reset " << signal_ << " trapping"
            << abort(FatalError);
    }

    Info<< "sigWriteNow :"
        << " setting up write at end of the next iteration" << nl << endl;
    runTimePtr_->writeOnce();

    //// Throw signal (to old handler)
    //raise(signal_);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::sigWriteNow::sigWriteNow()
{}


Foam::sigWriteNow::sigWriteNow(const bool verbose, Time& runTime)
{
    // Store runTime
    runTimePtr_ = &runTime;

    set(verbose);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::sigWriteNow::~sigWriteNow()
{
    // Reset old handling
    if (signal_ > 0)
    {
        const __p_sig_fn_t success = ::signal(signal_, oldAction_);
        oldAction_ = SIG_DFL;

        if (SIG_ERR == success)
        {
            FatalErrorIn
            (
                "Foam::sigWriteNow::~sigWriteNow()"
            )   << "Cannot reset " << signal_ << " trapping"
                << abort(FatalError);
        }
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void Foam::sigWriteNow::set(const bool verbose)
{
    if (signal_ >= 0)
    {
        oldAction_ = ::signal(signal_, &Foam::sigWriteNow::sigHandler);        

        if (SIG_ERR == oldAction_)
        {
            FatalErrorIn
            (
                "Foam::sigWriteNow::sigWriteNow(const bool, const Time&)"
            )   << "Cannot set " << signal_ << " trapping"
                << abort(FatalError);
        }

        if (verbose)
        {
            Info<< "sigWriteNow :"
                << " Enabling writing upon signal " << signal_
                << endl;
        }
    }
}


bool Foam::sigWriteNow::active() const
{
    return signal_ > 0;
}


// ************************************************************************* //
