/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2017 OpenFOAM Foundation
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

Description
    Wrapper for stat() system call.

\*---------------------------------------------------------------------------*/

#include "fileStat.H"
#include "IOstreams.H"
#include "timer.H"

#include <unistd.h>



#undef major
#undef minor
#undef makedev

# define major(dev) (int(((dev) >> 8) & 0xff))
# define minor(dev) (int((dev) & 0xff))
# define makedev(major, minor) ((((unsigned int)(major)) << 8) \
                                | ((unsigned int)(minor)))


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

fileStat::fileStat()
:
    isValid_(false)
{}


fileStat::fileStat
(
    const fileName& fName,
    const bool followLink,
    const unsigned int maxTime
)
{
    // Work on volatile
    volatile bool locIsValid = false;

    timer myTimer(maxTime);

    if (!timedOut(myTimer))
    {
        if (::stat(fName.c_str(), &status_) != 0)
        {
            locIsValid = false;
        }
        else
        {
            // FIXME: Need to populate the 'st_atim' branch in 'stat_extended'
            // Task assigned to this: https://github.com/blueCFD/Core/issues/65

            status_.st_atim.tv_sec = 0;
            status_.st_atim.tv_nsec = 0;

            locIsValid = true;
        }
    }

    // Copy into (non-volatile, possible register based) member var
    isValid_ = locIsValid;
}


fileStat::fileStat(Istream& is)
{
    is >> *this;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool fileStat::sameDevice(const fileStat& stat2) const
{
    return
        isValid_
     && (
            major(status_.st_dev) == major(stat2.status().st_dev)
         && minor(status_.st_dev) == minor(stat2.status().st_dev)
        );
}

bool fileStat::sameINode(const fileStat& stat2) const
{
    return isValid_ && (status_.st_ino == stat2.status().st_ino);
}

bool fileStat::sameINode(const label iNode) const
{
    return isValid_ && (status_.st_ino == ino_t(iNode));
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

Istream& operator>>(Istream& is, fileStat& fStat)
{
    // Read beginning of machine info list
    is.readBegin("fileStat");

    label 
        devMaj, devMin,
        ino, mode, uid, gid,
        rdevMaj, rdevMin,
        size, atime, mtime, ctime;

    is  >> fStat.isValid_
        >> devMaj
        >> devMin
        >> ino
        >> mode
        >> uid
        >> gid
        >> rdevMaj
        >> rdevMin
        >> size
        >> atime
        >> mtime
        >> ctime;

    dev_t st_dev = makedev(devMaj, devMin);
    fStat.status_.st_dev = st_dev;

    fStat.status_.st_ino = ino;
    fStat.status_.st_mode = mode;
    fStat.status_.st_uid = uid;
    fStat.status_.st_gid = gid;

    dev_t st_rdev = makedev(rdevMaj, rdevMin);
    fStat.status_.st_rdev = st_rdev;

    fStat.status_.st_size = size;
    fStat.status_.st_atime = atime;
    fStat.status_.st_mtime = mtime;
    fStat.status_.st_ctime = ctime;

    // Read end of machine info list
    is.readEnd("fileStat");

    // Check state of Istream
    is.check("Istream& operator>>(Istream&, fileStat&)");

    return is;
}


Ostream& operator<<(Ostream& os, const fileStat& fStat)
{
    //Set precision so 32bit unsigned int can be printed
//    int oldPrecision = os.precision();
    int oldPrecision = 0;
    os.precision(10);

    os  << token::BEGIN_LIST << fStat.isValid_
        << token::SPACE << label(major(fStat.status_.st_dev))
        << token::SPACE << label(minor(fStat.status_.st_dev))
        << token::SPACE << label(fStat.status_.st_ino)
        << token::SPACE << label(fStat.status_.st_mode)
        << token::SPACE << label(fStat.status_.st_uid)
        << token::SPACE << label(fStat.status_.st_gid)
        << token::SPACE << label(major(fStat.status_.st_rdev))
        << token::SPACE << label(minor(fStat.status_.st_rdev))
        << token::SPACE << label(fStat.status_.st_size)
        << token::SPACE << label(fStat.status_.st_atime)
        << token::SPACE << label(fStat.status_.st_mtime)
        << token::SPACE << label(fStat.status_.st_ctime)
        << token::END_LIST;

    os.precision(oldPrecision);
    return os;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
