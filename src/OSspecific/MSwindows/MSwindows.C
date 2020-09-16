/*---------------------------------------------------------------------------*\
License
    Copyright            : (C) 2011 Symscape
    Website              : www.symscape.com

    Copyright            : (C) 2011-2017 FSD blueCAPE Lda
    Website              : www.bluecape.com.pt

    Copyright            : (C) 2011-2017 OpenFOAM Foundation
    Website              : www.openfoam.org

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

Description
    MS Windows versions of the functions declared in OSspecific.H

Details
    This file has been created by blueCAPE's unofficial mingw patches for
    OpenFOAM.
    For more information about these patches, visit:
        http://bluecfd.com/Core

    Details on how this file was created:
      - This file was originally based on Symscape's own work on patching 
        OpenFOAM for working on Windows, circa 2009.
      - Further changes were made by blueCAPE over the years.
      - Includes code from POSIX.C where applicable.


\*---------------------------------------------------------------------------*/

#include "OSspecific.H"
#include "MSwindows.H"
#include "foamVersion.H"
#include "fileName.H"
#include "fileStat.H"
#include "timer.H"
#include "IFstream.H"
#include "DynamicList.T.H"
#include "IOstreams.H"
#include "Pstream.T.H"

// Undefine DebugInfo, because we don't need it and it collides with a macro
// in windows.h
#undef DebugInfo

#include <cassert>
#include <cstdlib>
#include <fstream>
#include <map>

// Windows system header files
#include <io.h> // _close
#include <windows.h>
#include <signal.h>

// For C++11 std::thread and std::mutex
#include <thread>
#include <mutex>


// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
  defineTypeNameAndDebug(MSwindows, 0);
}

namespace Foam 
{

// Don't abort under windows, causes abort dialog to
// popup. Instead just exit with exitCode.
static
void sigAbortHandler(int exitCode)
{
  ::exit(exitCode);
}


static
bool installAbortHandler()
{
  // If it didn't succeed there's not much we can do,
  // so don't check result.
  ::signal(SIGABRT, &sigAbortHandler);
  return true;
}


static bool const abortHandlerInstalled = installAbortHandler();


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


//- Get last windows api error from GetLastError
string MSwindows::getLastError()
{
    // Based on an example at:
    // http://msdn2.microsoft.com/en-us/library/ms680582(VS.85).aspx

    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError(); 

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        LPTSTR(&lpMsgBuf),
        0, NULL );

    lpDisplayBuf = LocalAlloc(LMEM_ZEROINIT, 
        (lstrlen(static_cast<LPCTSTR>(lpMsgBuf))+40)*sizeof(TCHAR)); 
    sprintf(static_cast<LPTSTR>(lpDisplayBuf),
            "Error %d: %s", int(dw), static_cast<LPCTSTR>(lpMsgBuf));

    const string errorMessage = static_cast<LPTSTR>(lpDisplayBuf);

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);

    return errorMessage;
}


//-Declared here to avoid polluting MSwindows.H with windows.h
namespace MSwindows
{
    //- Get windows user name
    Foam::string getUserName();

    //- Remove quotes, if any, from string
    void removeQuotes(Foam::string & arg);

    //- Convert windows directory slash (back-slash) to unix (forward-slash). 
    //- Windows is fine with unix like directory slashes.
    //- Foam's file io (see src/OpenFOAM/db/IOstreams/Sstreams/OSwrite.C) 
    //- uses back-slash as escape character and continuation, 
    //- so not an option to have windows file paths with back-slashes
    void toUnixSlash(Foam::string & arg);

    //- Auto create and then delete array when this goes out of scope
    template<class T>
    class AutoArray
    {
      T* const array_;

    public:
      AutoArray(const unsigned long arrayLength);
      ~AutoArray();

      //- Access array
      T* get();
    }; // class AutoArray


    //- Directory contents iterator
    class DirectoryIterator
    {
      WIN32_FIND_DATA findData_;
      HANDLE findHandle_;
      fileName nextName_;
      bool hasMore_;
      
    public:
      DirectoryIterator(const fileName & directory);
      ~DirectoryIterator();
      
      //- Initialization succeeded
      bool isValid() const;

      //- Has more?
      bool hasNext() const;
      
      //- Next item
      const fileName & next();
    }; // class DirectoryIterator
} // namespace MSwindows


inline
void MSwindows::removeQuotes(Foam::string & arg)
{
    std::size_t pos;

    while (Foam::string::npos != (pos = arg.find('"')))
    {
        arg.erase(pos, 1);
    }
}


inline
void MSwindows::toUnixSlash(Foam::string & arg)
{
    arg.replaceAll("\\", "/");

    const string UNC("//");

    // Preserve UNC i.e., \\machine-name\...
    if (0 == arg.find(UNC)) 
    {
        arg.replace(UNC, "\\\\");
    }
}


string MSwindows::getUserName()
{
    const DWORD bufferSize = 256;
    TCHAR buffer[bufferSize];
    DWORD actualBufferSize = bufferSize;
    string nameAsString;

    bool success = ::GetUserName(buffer, &actualBufferSize);

    if (success)
    {
        nameAsString = buffer;
    }
    else 
    {
        if (ERROR_INSUFFICIENT_BUFFER == ::GetLastError() &&
            32768 > actualBufferSize) 
        {
            AutoArray<TCHAR> actualBuffer(actualBufferSize);
            ::GetUserName(actualBuffer.get(), &actualBufferSize);
            nameAsString = actualBuffer.get();
        }
    }

    return nameAsString;
}


template<class T>
inline
MSwindows::AutoArray<T>::AutoArray(const unsigned long arrayLength)
    : array_(new T[arrayLength])
{}


template<class T>
inline
MSwindows::AutoArray<T>::~AutoArray()
{
    delete [] array_;
}


template<class T>
inline
T* MSwindows::AutoArray<T>::get()
{
    return array_;
}


inline
bool MSwindows::DirectoryIterator::isValid() const
{
    const bool valid = (INVALID_HANDLE_VALUE != findHandle_);
    return valid;
}

    
MSwindows::DirectoryIterator::DirectoryIterator(const fileName & directory)
{
    const fileName directoryContents = directory/"*";
    findHandle_ = ::FindFirstFile(directoryContents.c_str(), &findData_);
    hasMore_    = isValid();
}
        

MSwindows::DirectoryIterator::~DirectoryIterator()
{
    if (isValid()) 
    {
        ::FindClose(findHandle_);
    }
}


inline
bool MSwindows::DirectoryIterator::hasNext() const
{
    assert(isValid());

    return hasMore_;
}


inline
const fileName & MSwindows::DirectoryIterator::next()
{
    assert(hasNext());

    nextName_ = findData_.cFileName;
    hasMore_  = ::FindNextFile(findHandle_, &findData_);

    return nextName_;
}


pid_t pid()
{
#ifdef WIN32
    const DWORD processId = ::GetCurrentProcessId();
#elif WIN64
    const pid_t processId = (pid_t) ::GetCurrentProcessId();
#endif
    return processId;
}


pid_t ppid()
{
    // No equivalent under windows.

    if (MSwindows::debug)
    {
        InfoInFunction
            << "ppid not supported under MSwindows" << endl;
    }

    return 0;
}


pid_t pgid()
{
    // No equivalent under windows.

    if (MSwindows::debug)
    {
        InfoInFunction
            << "pgid not supported under MSwindows" << endl;
    }

    return 0;
}


bool env(const word& envName)
{
    const DWORD actualBufferSize = 
      ::GetEnvironmentVariable(envName.c_str(), NULL, 0);

    const bool envExists = (0 < actualBufferSize);
    return envExists;
}


string getEnv(const word& envName)
{
    string envAsString;

    const DWORD actualBufferSize = 
      ::GetEnvironmentVariable(envName.c_str(), NULL, 0);

    if (0 < actualBufferSize) 
    {
        MSwindows::AutoArray<TCHAR> actualBuffer(actualBufferSize);
        ::GetEnvironmentVariable(envName.c_str(),
                                 actualBuffer.get(),
                                 actualBufferSize);
        envAsString = actualBuffer.get();
        toUnixPath(envAsString);
    }

    return envAsString;
}


bool setEnv
(
    const word& envName,
    const std::string& value,
    const bool /*overwrite*/
)
{
    const bool success = 
      ::SetEnvironmentVariable(envName.c_str(), value.c_str());
    return success;
}


string hostName(bool full)
{
    const DWORD bufferSize = MAX_COMPUTERNAME_LENGTH + 1;
    TCHAR buffer[bufferSize];
    DWORD actualBufferSize = bufferSize;

    const bool success = 
      ::GetComputerName(buffer, &actualBufferSize);
    const string computerName = success ? buffer : string::null;
    return computerName;
}


string domainName()
{
    // FIXME: this should be implemented for completion.
    // Could use ::gethostname and ::gethostbyname like POSIX.C, but would
    // then need to link against ws_32. Prefer to minimize dependencies.
    // See task https://github.com/blueCFD/Core/issues/61

    return string::null;
}


string userName()
{
    string nameAsString = getEnv("USERNAME");

    if (nameAsString.empty()) 
    {
        nameAsString = MSwindows::getUserName();
    }

    return nameAsString;
}


bool isAdministrator()
{
    // FIXME: This is going to be needed as well... for building dynamic code.
    // Going to assume to be false, so this way it's possible to build dynamic
    // code!
    // Besides, in Windows XP the usual default is to be the administrator...
    // See task https://github.com/blueCFD/Core/issues/62

    return false;
}


fileName home()
{
    string homeDir = getEnv("HOME");

    if (homeDir.empty()) 
    {
        homeDir = getEnv("USERPROFILE");
    }

    return homeDir;
}


fileName home(const string& userName)
{
    return home();
}


fileName cwd()
{
    string currentDirectory;

    const DWORD actualBufferSize = 
      ::GetCurrentDirectory(0, NULL);

    if (0 < actualBufferSize) 
    {
        MSwindows::AutoArray<TCHAR> actualBuffer(actualBufferSize);
        ::GetCurrentDirectory(actualBufferSize,
                              actualBuffer.get());   
        currentDirectory = actualBuffer.get();
        MSwindows::toUnixSlash(currentDirectory);
    }
    else 
    {
        FatalErrorInFunction
            << "Couldn't get the current working directory"
            << exit(FatalError);
    }

    return currentDirectory;
}


bool chDir(const fileName& dir)
{
    const bool success = ::SetCurrentDirectory(dir.c_str());
    return success; 
}


bool mkDir(const fileName& pathName, const mode_t mode)
{
    if (MSwindows::debug)
    {
        Pout<< FUNCTION_NAME << " : pathName:" << pathName << " mode:" << mode
            << endl;
        if ((MSwindows::debug & 2) && !Pstream::master())
        {
            error::printStack(Pout);
        }
    }

    if (pathName.empty())
    {
        return false;
    }

    bool success = ::CreateDirectory(pathName.c_str(), NULL);
    if (success)
    {
        chMod(pathName, mode);
    }
    else 
    {
        const DWORD error = ::GetLastError();

        switch (error)
        {
            case ERROR_ALREADY_EXISTS:
            {
                success = true;
                break;
            }
            case ERROR_PATH_NOT_FOUND:
            {
                // Part of the path does not exist so try to create it
                const fileName& parentName = pathName.path();

                if (parentName.size() && mkDir(parentName, mode))
                {
                    success = mkDir(pathName, mode);
                }
                
                break;
            }  
        }

        if (!success) 
        {
            FatalErrorInFunction
              << "Couldn't create directory: " << pathName
              << " " << MSwindows::getLastError()
              << exit(FatalError);
        }
    }

    return success;
}


bool chMod(const fileName& name, const mode_t m)
{
    if (MSwindows::debug)
    {
        Pout<< FUNCTION_NAME << " : name:" << name << endl;
        if ((MSwindows::debug & 2) && !Pstream::master())
        {
            error::printStack(Pout);
        }
    }

    const int success = _chmod(name.c_str(), m);
    return success;
}


mode_t mode(const fileName& name, const bool followLink)
{
    if (MSwindows::debug)
    {
        Pout<< FUNCTION_NAME << " : name:" << name << endl;
    }

    fileStat fileStatus(name, followLink);
    if (fileStatus.isValid())
    {
        return fileStatus.status().st_mode;
    }
    else
    {
        return 0;
    }
}


fileType type
(
    const fileName& name,
    const bool checkVariants,
    const bool followLink
)
{
    //FIXME: 'followLink' can't be used with 'GetFileAttributes'.
    //Task for fixing this detail: https://github.com/blueCFD/Core/issues/60

    fileType vfileType = fileType::undefined;
    const DWORD attrs = ::GetFileAttributes(name.c_str());

    if (attrs != INVALID_FILE_ATTRIBUTES) 
    {
        vfileType = (attrs & FILE_ATTRIBUTE_DIRECTORY) ?
            fileType::directory :
            fileType::file;
    }

    return fileType;
}


static bool isGzFile(const fileName& name)
{
    string gzName(name);
    gzName += ".gz";
    const DWORD attrs = ::GetFileAttributes(gzName.c_str());
    const bool success = (attrs != INVALID_FILE_ATTRIBUTES);

    return success;
}


bool exists
(
    const fileName& name,
    const bool checkGzip,
    const bool followLink
)
{
    //FIXME: 'followLink' can't be used with 'GetFileAttributes'.
    //Task for fixing this detail: https://github.com/blueCFD/Core/issues/60

    if (MSwindows::debug)
    {
        Pout<< FUNCTION_NAME << " : name:" << name
            << " checkGzip:" << checkGzip
            << endl;
        if ((MSwindows::debug & 2) && !Pstream::master())
        {
            error::printStack(Pout);
        }
    }

    const DWORD attrs = ::GetFileAttributes(name.c_str());
    const bool success = (attrs != INVALID_FILE_ATTRIBUTES) || 
                         (checkGzip && isGzFile(name));

    return success;
}


bool isDir(const fileName& name, const bool followLink)
{
    //FIXME: 'followLink' can't be used with 'GetFileAttributes'.
    //Task for fixing this detail: https://github.com/blueCFD/Core/issues/60

    if (MSwindows::debug)
    {
        Pout<< FUNCTION_NAME << " : name:" << name << endl;
        if ((MSwindows::debug & 2) && !Pstream::master())
        {
            error::printStack(Pout);
        }
    }

    const DWORD attrs = ::GetFileAttributes(name.c_str());
    bool success = (attrs != INVALID_FILE_ATTRIBUTES) &&
                   (attrs & FILE_ATTRIBUTE_DIRECTORY);

    return success;
}


bool isFile
(
    const fileName& name,
    const bool checkGzip,
    const bool followLink
)
{
    //FIXME: 'followLink' can't be used with 'GetFileAttributes'.
    //Task for fixing this detail: https://github.com/blueCFD/Core/issues/60

   if (MSwindows::debug)
    {
        Pout<< FUNCTION_NAME << " : name:" << name
            << " checkGzip:" << checkGzip
            << endl;
        if ((MSwindows::debug & 2) && !Pstream::master())
        {
            error::printStack(Pout);
        }
    }

    const DWORD attrs = ::GetFileAttributes(name.c_str());
    const bool success =
    (
        (attrs != INVALID_FILE_ATTRIBUTES) && 
        !(attrs & FILE_ATTRIBUTE_DIRECTORY)
    )
     ||
    (
        checkGzip && isGzFile(name)
    );

    return success;
}


off_t fileSize(const fileName& name, const bool followLink)
{
    if (MSwindows::debug)
    {
        Pout<< FUNCTION_NAME << " : name:" << name << endl;
        if ((MSwindows::debug & 2) && !Pstream::master())
        {
            error::printStack(Pout);
        }
    }

    fileStat fileStatus(name, followLink);

    if (fileStatus.isValid())
    {
        return fileStatus.status().st_size;
    }
    else
    {
        return off_t(-1);
    }
}


time_t lastModified(const fileName& name, const bool followLink)
{
    if (MSwindows::debug)
    {
        Pout<< FUNCTION_NAME << " : name:" << name << endl;
        if ((MSwindows::debug & 2) && !Pstream::master())
        {
            error::printStack(Pout);
        }
    }

    fileStat fileStatus(name, followLink);
    if (fileStatus.isValid())
    {
        return fileStatus.status().st_mtime;
    }
    else
    {
        return 0;
    }
}


double highResLastModified(const fileName& name, const bool followLink)
{
    if (MSwindows::debug)
    {
        Pout<< FUNCTION_NAME << " : name:" << name << endl;
        if ((MSwindows::debug & 2) && !Pstream::master())
        {
            error::printStack(Pout);
        }
    }
    fileStat fileStatus(name, followLink);
    if (fileStatus.isValid())
    {
        return
            fileStatus.status().st_mtime
          + 1e-9*fileStatus.status().st_atim.tv_nsec;
    }
    else
    {
        return 0;
    }
}


fileNameList readDir
(
    const fileName& directory,
    const fileType type,
    const bool filterVariants,
    const bool followLink
)
{
    // Initial filename list size
    // also used as increment if initial size found to be insufficient
    const int maxNnames = 100;

    if (MSwindows::debug)
    {
        Pout<< FUNCTION_NAME << " : reading directory " << directory << endl;
        if ((MSwindows::debug & 2) && !Pstream::master())
        {
            error::printStack(Pout);
        }
    }

    // Setup empty string list MAXTVALUES long
    HashSet<fileName> dirEntries(maxNnames);

    MSwindows::DirectoryIterator dirIt(directory);

    if (dirIt.isValid())
    {
        while (dirIt.hasNext())
        {
            const fileName & fName = dirIt.next();

            // Ignore files begining with ., i.e. '.', '..' and '.*'
            if (fName.size() > 0 && fName[size_t(0)] != '.')
            {
                word fileNameExt = fName.ext();

                if
                (
                    (type == fileType::directory)
                 ||
                    (
                        type == fileType::file
                        && fName[fName.size()-1] != '~'
                        && fileNameExt != "bak"
                        && fileNameExt != "BAK"
                        && fileNameExt != "old"
                        && fileNameExt != "save"
                    )
                )
                {
                    if ((directory/fName).type(followLink) == type)
                    {
                        bool filtered = false;

                        if (filterVariants)
                        {
                            for (label i = 0; i < fileStat::nVariants_; ++ i)
                            {
                                if (fExt == fileStat::variantExts_[i])
                                {
                                    dirEntries.insert(fName.lessExt());
                                    filtered = true;
                                    break;
                                }
                            }
                        }

                        if (!filtered)
                        {
                            dirEntries.insert(fName);
                        }
                    }
                }
            }
        }
    }
    else if (MSwindows::debug)
    {
        InfoInFunction
            << "cannot open directory " << directory << endl;
    }

    return dirEntries.toc();
}


bool cp(const fileName& src, const fileName& dest, const bool followLink)
{
    if (MSwindows::debug)
    {
        Pout<< FUNCTION_NAME << " : src:" << src << " dest:" << dest << endl;
        if ((MSwindows::debug & 2) && !Pstream::master())
        {
            error::printStack(Pout);
        }
    }

    // Make sure source exists.
    if (!exists(src))
    {
        return false;
    }

    const fileType srcType = src.type(false, followLink);

    fileName destFile(dest);

    // Check type of source file.
    if (srcType == fileType::file)
    {
        // If dest is a directory, create the destination file name.
        if (destFile.type() == fileType::directory)
        {
            destFile = destFile/src.name();
        }

        // Make sure the destination directory exists.
        if (!isDir(destFile.path()) && !mkDir(destFile.path()))
        {
            return false;
        }

        // Open and check streams.
        // Use binary mode in case we read binary.
        // Causes windows reading to fail if we don't.
        std::ifstream srcStream(src.c_str(), 
                                ios_base::in|ios_base::binary);
        if (!srcStream) 
        {
            return false;
        }

        // Use binary mode in case we write binary.
        // Causes windows reading to fail if we don't.
        std::ofstream destStream(destFile.c_str(), 
                                 ios_base::out|ios_base::binary);
        if (!destStream)
        {
            return false;
        }

        // Copy character data.
        char ch;
        while (srcStream.get(ch))
        {
            destStream.put(ch);
        }

        // Final check.
        if (!srcStream.eof() || !destStream)
        {
            return false;
        }
    }
    else if (srcType == fileType::link)
    {
        // If dest is a directory, create the destination file name.
        if (destFile.type() == fileType::directory)
        {
            destFile = destFile/src.name();
        }

        // Make sure the destination directory exists.
        if (!isDir(destFile.path()) && !mkDir(destFile.path()))
        {
            return false;
        }

        ln(src, destFile);
    }
    else if (srcType == fileType::directory)
    {
        // If dest is a directory, create the destination file name.
        if (destFile.type() == fileType::directory)
        {
            destFile = destFile/src.component(src.components().size() -1);
        }

        // Make sure the destination directory extists.
        if (!isDir(destFile) && !mkDir(destFile))
        {
            return false;
        }

        // Copy files
        fileNameList contents = readDir(src, fileType::file, false);
        forAll(contents, i)
        {
            if (MSwindows::debug)
            {
                InfoInFunction
                    << "Copying : " << src/contents[i] 
                    << " to " << destFile/contents[i] << endl;
            }

            // File to file.
            cp(src/contents[i], destFile/contents[i]);
        }

        // Copy sub directories.
        fileNameList subdirs = readDir(src, fileType::directory);
        forAll(subdirs, i)
        {
            if (MSwindows::debug)
            {
                InfoInFunction
                    << "Copying : " << src/subdirs[i]
                    << " to " << destFile << endl;
            }

            // Dir to Dir.
            cp(src/subdirs[i], destFile);
        }
    }

    return true;
}


bool ln(const fileName& src, const fileName& dest)
{
    // FIXME: Seems that prior to Vista softlinking was poorly supported.
    // Vista does a better job, but requires administrator privileges.
    // Task for this feature: https://github.com/blueCFD/Core/issues/63

    if (MSwindows::debug)
    {
        //InfoInFunction
        Pout<< FUNCTION_NAME
            << " : Create softlink from : " << src << " to " << dest << endl;
        if ((MSwindows::debug & 2) && !Pstream::master())
        {
            error::printStack(Pout);
        }
    }

    if (MSwindows::debug)
    {
        InfoInFunction
            << "MSwindows does not support ln - softlinking" << endl;
    }

    return false;
}


bool mv(const fileName& src, const fileName& dst, const bool followLink)
{
    if (MSwindows::debug)
    {
        //InfoInFunction
        Pout<< FUNCTION_NAME << " : Move : " << src << " to " << dst << endl;
        if ((MSwindows::debug & 2) && !Pstream::master())
        {
            error::printStack(Pout);
        }
    }

    if
    (
        dst.type() == fileType::directory
     && src.type(false, followLink) != fileType::directory
    )
    {
        const fileName dstName(dst/src.name());

        return std::rename(src.c_str(), dstName.c_str()) == 0;
    }
    else
    {
        return std::rename(src.c_str(), dst.c_str()) == 0;
    }
}


bool mvBak(const fileName& src, const std::string& ext)
{
    if (MSwindows::debug)
    {
        //InfoInFunction
        Pout<< FUNCTION_NAME
            << " : moving : " << src << " to extension " << ext << endl;
        if ((MSwindows::debug & 2) && !Pstream::master())
        {
            error::printStack(Pout);
        }
    }

    if (exists(src, false))
    {
        const int maxIndex = 99;
        char index[3];

        for (int n = 0; n <= maxIndex; n++)
        {
            fileName dstName(src + "." + ext);
            if (n)
            {
                sprintf(index, "%02d", n);
                dstName += index;
            }

            // avoid overwriting existing files, except for the last
            // possible index where we have no choice
            if (!exists(dstName, false) || n == maxIndex)
            {
                return (0 == std::rename(src.c_str(), dstName.c_str()));
            }

        }
    }

    // fall-through: nothing to do
    return false;
}


// Remove a file returning true if successful otherwise false
bool rm(const fileName& file)
{
    if (MSwindows::debug)
    {
        //InfoInFunction
        Pout<< FUNCTION_NAME << " : Removing : " << file << endl;
        if ((MSwindows::debug & 2) && !Pstream::master())
        {
            error::printStack(Pout);
        }
    }

    bool success = (0 == std::remove(file.c_str()));

    // If deleting plain file name failed try with .gz
    if (!success) 
    {
        const string fileGz = file + ".gz";
        success = (0 == std::remove(fileGz.c_str()));
    }

    return success;
}


// Remove a dirctory and it's contents
bool rmDir(const fileName& directory)
{
    if (MSwindows::debug)
    {
        //InfoInFunction
        Pout<< FUNCTION_NAME << " : removing directory " << directory << endl;
        if ((MSwindows::debug & 2) && !Pstream::master())
        {
            error::printStack(Pout);
        }
    }

    bool success = true;

    // Need to destroy DirectorIterator prior to
    // removing directory otherwise fails on Windows XP
    {
      MSwindows::DirectoryIterator dirIt(directory);

      while (success && dirIt.hasNext())
      {
          const fileName & fName = dirIt.next(); 

          if (fName != "." && fName != "..")
          {
              fileName path = directory/fName;

              if (path.type(false, false) == fileType::directory)
              {
                  success = rmDir(path);

                  if (!success)
                  {
                      WarningInFunction
                        << "failed to remove directory " << fName
                        << " while removing directory " << directory
                        << endl;
                  }
              }
              else
              {
                  success = rm(path);

                  if (!success)
                  {
                      WarningInFunction
                        << "failed to remove file " << fName
                        << " while removing directory " << directory
                        << endl;
                  }
              }
          }
      }
    }
        
    if (success) 
    {
        success = ::RemoveDirectory(directory.c_str());

        if (!success) 
        {
            WarningInFunction
                << "failed to remove directory " << directory << endl;
        }
    }

    return success;
}


unsigned int sleep(const unsigned int s)
{
    const DWORD milliseconds = s * 1000;

    ::Sleep(milliseconds);

    return 0;
}


void fdClose(const int fd)
{
    const int result = ::_close(fd);

    if (0 != result)
    {
        FatalErrorInFunction
            << "close error on " << fd << endl
            << abort(FatalError);
    }
}


bool ping
(
    const word& destName,
    const label destPort,
    const label timeOut
)
{
    // FIXME: Task for this: https://github.com/blueCFD/Core/issues/64

    if (MSwindows::debug)
    {
        InfoInFunction
            << "MSwindows does not support ping" << endl;
    }

    return false;
}


bool ping(const word& hostname, const label timeOut)
{
    return ping(hostname, 222, timeOut) || ping(hostname, 22, timeOut);
}


int system(const std::string& command)
{
    return std::system(command.c_str());
}


// Explicitly track loaded libraries, rather than use
// EnumerateLoadedModules64 and have to link against Dbghelp.dll
// Details at http://msdn.microsoft.com/en-us/library/ms679316(v=vs.85).aspx
typedef std::map<void*, std::string> OfLoadedLibs;

static
OfLoadedLibs &
getLoadedLibs()
{
  static OfLoadedLibs loadedLibs;
  return loadedLibs;
}


void* dlOpen(const fileName& libName, const bool check)
{
    //Lets check if this is a list of libraries to be loaded
    //NOTE: should only be used for "force loading libraries"
    if (libName.find_first_of(',')!=Foam::string::npos)
    {
        void *moduleh=NULL;
        string libsToLoad=libName;
        libsToLoad.removeTrailing(' '); //removes spaces from both ends
        libsToLoad.removeRepeated(',');
        libsToLoad += ',';

        if (MSwindows::debug)
        {
            InfoInFunction
                << "Libraries to be loaded: " <<  libsToLoad << endl;
        }

        //generate the word list
        size_t stposstr=0, found=libsToLoad.find_first_of(',');
        while (found!=string::npos)
        {
            string libToLoad = libsToLoad.substr(stposstr,found-stposstr);
            moduleh = dlOpen(libToLoad);
            stposstr=found+1; found=libsToLoad.find_first_of(',',stposstr);
        }

        return moduleh;
    }
    else
    {
        if (MSwindows::debug)
        {
            InfoInFunction
                << " : LoadLibrary of " << libName << endl;
        }

        const char* dllExt = ".dll";

        // Assume libName is of the form, lib<name>.so
        Foam::string winLibName(libName);
        winLibName.replace(".so", dllExt);
        void* libHandle = ::LoadLibrary(winLibName.c_str());

        if (NULL == libHandle)
        {
            // Assumes libName = name
            winLibName = "lib";
            winLibName += libName;
            winLibName += dllExt;

            libHandle = ::LoadLibrary(winLibName.c_str());
        }

        if (NULL == libHandle && check)
        {
            WarningInFunction
                << "LoadLibrary failed. "
                << MSwindows::getLastError()
                << endl;
        }
        else
        {
            getLoadedLibs()[libHandle] = libName;
        }

        if (MSwindows::debug)
        {
            InfoInFunction
                << "Library " <<  libName << " loaded "
                << (libHandle != NULL ? "with success!" : "without success.")
                << endl;
        }

        return libHandle;
    }
}


bool dlClose(void* libHandle)
{
    if (MSwindows::debug)
    {
        InfoInFunction
            << "FreeLibrary of handle " << libHandle << endl;
    }

    const bool success = 
      ::FreeLibrary(static_cast<HMODULE>(libHandle));
  
    if (!success) 
    {
        WarningInFunction
            << "FreeLibrary failed. " 
            << MSwindows::getLastError()
            << endl;
    }
    else
    {
        getLoadedLibs().erase(libHandle);
    }
    
    return success;
}


void* dlSym(void* handle, const std::string& symbol)
{
    if (MSwindows::debug)
    {
        InfoInFunction
            << "dlsym of " << symbol << endl;
    }

    // get address of symbol
    void* fun = (void *)
    (
        ::GetProcAddress(static_cast<HMODULE>(handle), symbol.c_str())
    );

    if(fun == NULL)
    {
        WarningInFunction
            << "Cannot lookup symbol " << symbol << " : "
            << MSwindows::getLastError()
            << endl;
    }

    return fun;
}


bool dlSymFound(void* handle, const std::string& symbol)
{
    if (handle && !symbol.empty())
    {
        if (MSwindows::debug)
        {
            InfoInFunction
                << "dlSym of " << symbol << endl;
        }

        // symbol can be found if there was no error
        return dlSym(handle, symbol.c_str()) != NULL;
    }
    else
    {
        return false;
    }
}


fileNameList dlLoaded()
{
    fileNameList libs;
    OfLoadedLibs & loadedLibs = getLoadedLibs();

    for (OfLoadedLibs::const_iterator it = loadedLibs.begin();
         it != loadedLibs.end();
         ++it)
    {
        libs.append(it->second);
    }

    if (MSwindows::debug)
    {
        InfoInFunction
            << "determined loaded libraries :" << libs.size() << endl;
    }
    return libs;
}

string toUnixPath(const string & path)
{
    string unixPath(path);
    MSwindows::toUnixSlash(unixPath);
    MSwindows::removeQuotes(unixPath);

    return unixPath;
}

} // namespace Foam
// ************************************************************************* //
