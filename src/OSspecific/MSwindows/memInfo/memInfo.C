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

    This file has been created by blueCAPE's unofficial mingw patches for
    OpenFOAM.
    For more information about these patches, visit:
        http://bluecfd.com/Core

    Modifications made:
      - Derived from the patches for blueCFD 2.1 and 2.2.
      - Source for modifications: http://stackoverflow.com/a/633112

\*---------------------------------------------------------------------------*/

#include "memInfo.H"

// Undefine DebugInfo, because we don't use it here and it collides with a
// macro in windows.h
#undef DebugInfo

#include <windows.h>
#include <stdio.h>
#include <psapi.h>

// Define type for using the function GetProcessMemoryInfo
typedef BOOL (*GetProcessMemoryInfoType)
(
    HANDLE,
    PPROCESS_MEMORY_COUNTERS,
    DWORD
);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::memInfo::memInfo()
:
    peak_(-1),
    size_(-1),
    rss_(-1),
    GetProcessMemoryInfo_(NULL)
{
  GetProcessMemoryInfo_ =
        (void *)(
        GetProcAddress(LoadLibrary("Psapi.dll"), "GetProcessMemoryInfo")
        );

  update();
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::memInfo::~memInfo()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

const Foam::memInfo& Foam::memInfo::update()
{
    HANDLE hProcess = NULL;
    PROCESS_MEMORY_COUNTERS pmc;

    // reset to invalid values first
    peak_ = size_ = rss_ = -1;

    hProcess = OpenProcess(  PROCESS_QUERY_INFORMATION |
                             PROCESS_VM_READ,
                             FALSE, 
                             pid() );
    if (NULL != hProcess)
    {
        if(GetProcessMemoryInfo_!=NULL)
        {
            if
            (
                (GetProcessMemoryInfoType(GetProcessMemoryInfo_))
                (
                    hProcess,
                    &pmc,
                    sizeof(pmc)
                )
            )
            {
                peak_ = pmc.PeakWorkingSetSize;
                size_ = pmc.WorkingSetSize;
                rss_ = pmc.QuotaPagedPoolUsage;
            }
            else
            {
                WarningIn("memInfo::update()")
                  << "GetProcessMemoryInfo function was unable to "
                  << "retrieve information." << endl;
            }
        }
        else
        {
            WarningIn("memInfo::update()")
              << "Unable to access the GetProcessMemoryInfo function." << endl;
        }

        CloseHandle( hProcess );
    }

    return *this;
}


bool Foam::memInfo::valid() const
{
    return peak_ != -1;
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

Foam::Istream& Foam::operator>>(Istream& is, memInfo& m)
{
    is.readBegin("memInfo");

    is  >> m.peak_ >> m.size_ >> m.rss_;

    is.readEnd("memInfo");

    // Check state of Istream
    is.check
    (
        "Foam::Istream& Foam::operator>>(Foam::Istream&, Foam::memInfo&)"
    );

    return is;
}


Foam::Ostream& Foam::operator<<(Ostream& os, const memInfo& m)
{
    os  << token::BEGIN_LIST
        << m.peak_ << token::SPACE << m.size_ << token::SPACE << m.rss_
        << token::END_LIST;

    // Check state of Ostream
    os.check
    (
        "Foam::Ostream& Foam::operator<<(Foam::Ostream&, "
        "const Foam::memInfo&)"
    );

    return os;
}


// ************************************************************************* //
