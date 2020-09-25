/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2011-2018 OpenFOAM Foundation
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

    This file has been created by blueCAPE's unofficial mingw patches for
    OpenFOAM.
    For more information about these patches, visit:
        http://bluecfd.com/Core

    Modifications made:
      - Derived from the patches for blueCFD 2.1 and 2.2.
      - Adjusted the code to OpenFOAM 2.2.

Class
    fileMonitor

\*----------------------------------------------------------------------------*/

#include "fileMonitor.H"
#include "IOstreams.H"
#include "Pstream.T.H"
#include "PackedList.T.H"
#include "PstreamReduceOps.T.H"
#include "OSspecific.H"
#include "regIOobject.H"     // for fileModificationSkew symbol

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

const Foam::NamedEnum<Foam::fileMonitor::fileState, 3>
    Foam::fileMonitor::fileStateNames_;

namespace Foam
{
    defineTypeNameAndDebug(fileMonitor, 0);

    template<>
    const char* Foam::NamedEnum
    <
        Foam::fileMonitor::fileState,
        3
    >::names[] =
    {
        "unmodified",
        "modified",
        "deleted"
    };

    //- Reduction operator for PackedList of fileState
    class reduceFileStates
    {
        public:
        unsigned int operator()(const unsigned int x, const unsigned int y)
        const
        {
            // x,y are sets of 2bits representing fileState

            unsigned int mask = 3u;
            unsigned int shift = 0;
            unsigned int result = 0;

            while (mask)
            {
                // Combine state
                unsigned int xState = (x & mask) >> shift;
                unsigned int yState = (y & mask) >> shift;

                // Combine and add to result. Combine is such that UNMODIFIED
                // wins.
                unsigned int state = min(xState, yState);
                result |= (state << shift);

                shift += 2;
                mask <<= 2;
            }
            return result;
        }
    };

    //- Combine operator for PackedList of fileState
    class combineReduceFileStates
    {
        public:
        void operator()(unsigned int& x, const unsigned int y) const
        {
            x = reduceFileStates()(x, y);
        }
    };



    //-  Internal tracking via stat(3p) or inotify(7)
    class fileMonitorWatcher
    {
    public:

        const bool useInotify_;

        // For inotify

            //- File descriptor for the inotify instance
            int inotifyFd_;

            //- Current watchIDs and corresponding directory id
            DynamicList<label> dirWatches_;
            DynamicList<fileName> dirFiles_;

        // For stat

            //- From watch descriptor to modified time
            DynamicList<double> lastMod_;



        //- Initialise inotify
        inline fileMonitorWatcher(const bool useInotify, const label sz = 20)
        :
            useInotify_(useInotify)
        {
            if (useInotify_)
            {
                FatalErrorInFunction
                    << "You selected inotify, but there isn't such an"
                    << " option in Windows."
                    << "Please select another fileModification test method"
                    << exit(FatalError);
            }
            else
            {
                lastMod_.setCapacity(sz);
            }
        }

        //- Remove all watches
        inline ~fileMonitorWatcher()
        {
        }

        inline bool addWatch(const label watchFd, const fileName& fName)
        {
            if (useInotify_)
            {
                if (inotifyFd_ < 0)
                {
                    return false;
                }
            }
            else
            {
                if (watchFd < lastMod_.size() && lastMod_[watchFd] != 0)
                {
                    // Reuse of watchFd : should have lastMod set to 0.
                    FatalErrorInFunction
                        << "Problem adding watch " << watchFd
                        << " to file " << fName
                        << abort(FatalError);
                }

                lastMod_(watchFd) = highResLastModified(fName);
            }

            return true;
        }

        inline bool removeWatch(const label watchFd)
        {
            if (useInotify_)
            {
                if (inotifyFd_ < 0)
                {
                    return false;
                }

                dirWatches_[watchFd] = -1;
            }
            else
            {
                lastMod_[watchFd] = 0;
            }
            return true;
        }

    };
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void Foam::fileMonitor::checkFiles() const
{
    if (!useInotify_)
    {
        forAll(watcher_->lastMod_, watchFd)
        {
            double oldTime = watcher_->lastMod_[watchFd];

            if (oldTime != 0)
            {
                const fileName& fName = watchFile_[watchFd];
                double newTime = highResLastModified(fName);

                if (newTime == 0)
                {
                    localState_[watchFd] = DELETED;
                }
                else
                {
                    if (newTime > (oldTime + regIOobject::fileModificationSkew))
                    {
                        localState_[watchFd] = MODIFIED;
                    }
                    else
                    {
                        localState_[watchFd] = UNMODIFIED;
                    }
                }
            }
        }
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


Foam::fileMonitor::fileMonitor(const bool useInotify)
:
    useInotify_(useInotify),
    localState_(20),
    state_(20),
    watchFile_(20),
    freeWatchFds_(2),
    watcher_(new fileMonitorWatcher(useInotify_, 20))
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::fileMonitor::~fileMonitor()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

// Note: fName might not exist (on slaves if in master-only mode for
// regIOobject)
Foam::label Foam::fileMonitor::addWatch(const fileName& fName)
{
    label watchFd;

    label sz = freeWatchFds_.size();

    if (sz)
    {
        watchFd = freeWatchFds_[sz-1];
        freeWatchFds_.setSize(sz-1);
    }
    else
    {
        watchFd = state_.size();
    }

    watcher_->addWatch(watchFd, fName);

    if (debug)
    {
        Pout<< "fileMonitor : added watch " << watchFd << " on file "
            << fName << endl;
    }

    if (watchFd < 0)
    {
        WarningInFunction
            << "could not add watch for file " << fName << endl;
    }
    else
    {
        localState_(watchFd) = UNMODIFIED;
        state_(watchFd) = UNMODIFIED;
        watchFile_(watchFd) = fName;
    }
    return watchFd;
}


bool Foam::fileMonitor::removeWatch(const label watchFd)
{
    if (debug)
    {
        Pout<< "fileMonitor : removing watch " << watchFd << " on file "
            << watchFile_[watchFd] << endl;
    }

    freeWatchFds_.append(watchFd);
    return watcher_->removeWatch(watchFd);
}


const Foam::fileName& Foam::fileMonitor::getFile(const label watchFd) const
{
    return watchFile_[watchFd];
}


Foam::fileMonitor::fileState Foam::fileMonitor::getState(const label watchFd)
const
{
    return state_[watchFd];
}


void Foam::fileMonitor::updateStates
(
    const bool masterOnly,
    const bool syncPar
) const
{
    if (Pstream::master() || !masterOnly)
    {
        // Update the localState_
        checkFiles();
    }

    if (syncPar)
    {
        // Pack local state (might be on master only)
        PackedList<2> stats(state_.size(), MODIFIED);
        if (Pstream::master() || !masterOnly)
        {
            forAll(state_, watchFd)
            {
                stats[watchFd] = static_cast<unsigned int>
                (
                    localState_[watchFd]
                );
            }
        }


        // Scatter or reduce to synchronise state
        if (masterOnly)
        {
            // Scatter
            if (stats.storage().size() == 1)
            {
                Pstream::scatter(stats.storage()[0]);
            }
            else
            {
                Pstream::listCombineScatter(stats.storage());
            }
        }
        else
        {
            // Reduce
            if (stats.storage().size() == 1)
            {
                // Optimisation valid for most cases.
                reduce(stats.storage()[0], reduceFileStates());
            }
            else
            {
                Pstream::listCombineGather
                (
                    stats.storage(),
                    combineReduceFileStates()
                );
            }
        }


        // Update synchronised state
        forAll(state_, watchFd)
        {
            // Assign synchronised state
            unsigned int stat = stats[watchFd];
            state_[watchFd] = fileState(stat);

            if (!masterOnly)
            {
                // Give warning for inconsistent state
                if (state_[watchFd] != localState_[watchFd])
                {
                    if (debug)
                    {
                        Pout<< "fileMonitor : Delaying reading "
                            << watchFile_[watchFd]
                            << " due to inconsistent "
                               "file time-stamps between processors"
                            << endl;
                    }

                    WarningInFunction
                        << "Delaying reading " << watchFile_[watchFd]
                        << " due to inconsistent "
                           "file time-stamps between processors" << endl;
                }
            }
        }
    }
    else
    {
        state_ = localState_;
    }
}


void Foam::fileMonitor::setUnmodified(const label watchFd)
{
    state_[watchFd] = UNMODIFIED;
    localState_[watchFd] = UNMODIFIED;

    if (!useInotify_)
    {
        watcher_->lastMod_[watchFd] = highResLastModified(watchFile_[watchFd]);
    }
}


// ************************************************************************* //
