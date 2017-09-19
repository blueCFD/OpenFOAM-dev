/*---------------------------------------------------------------------------*\
License
    Copyright            : (C) 2011 Symscape
    Website              : www.symscape.com

    Copyright            : (C) 2011-2013 blueCAPE Lda
    Website              : www.bluecape.com.pt

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
      - Further changes were made by blueCAPE, culminating in the patches
        by blueCAPE for blueCFD 2.1, now adjusted here to OpenFOAM 2.2.


\*---------------------------------------------------------------------------*/

#include "OSspecific.H"
#include "MSwindows.H"
#include "foamVersion.H"
#include "fileName.H"
#include "fileStat.H"
#include "timer.H"
#include "IFstream.H"
#include "DynamicList.T.H"

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
        (LPTSTR) &lpMsgBuf,
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
        std::cout<< "ppid not supported under MSwindows" << std::endl;
    }

    return 0;
}


pid_t pgid()
{
    // No equivalent under windows.

    if (MSwindows::debug)
    {
        std::cout<< "pgid not supported under MSwindows" << std::endl;
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
    //FIXME: This is going to be needed as well... for building dynamic code.
    //Going to assume to be false, so this way it's possible to build dynamic code!
    //Besides, in Windows XP the usual default is to be the administrator...
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
        FatalErrorIn("cwd()")
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
            FatalErrorIn("mkDir(const fileName&, mode_t)")
              << "Couldn't create directory: " << pathName
              << " " << MSwindows::getLastError()
              << exit(FatalError);
        }
    }

    return success;
}


// Set the file mode
bool chMod(const fileName& name, const mode_t m)
{
    const int success = _chmod(name.c_str(), m);
    return success;
}


// Return the file mode
mode_t mode(const fileName& name)
{
    fileStat fileStatus(name);

    const mode_t m = fileStatus.isValid() ?
      fileStatus.status().st_mode : 0;
    return m;
}


// Return the file type: FILE or DIRECTORY
fileName::Type type(const fileName& name)
{
    fileName::Type fileType = fileName::UNDEFINED;
    const DWORD attrs = ::GetFileAttributes(name.c_str());

    if (attrs != INVALID_FILE_ATTRIBUTES) 
    {
        fileType = (attrs & FILE_ATTRIBUTE_DIRECTORY) ?
	  fileName::DIRECTORY :
	  fileName::FILE;
    }

    return fileType;
}


static
bool 
isGzFile(const fileName& name)
{
    string gzName(name);
    gzName += ".gz";
    const DWORD attrs = ::GetFileAttributes(gzName.c_str());
    const bool success = (attrs != INVALID_FILE_ATTRIBUTES);

    return success;
}


// Does the name exist in the filing system?
bool exists(const fileName& name, const bool checkGzip)
{
    const DWORD attrs = ::GetFileAttributes(name.c_str());
    const bool success = (attrs != INVALID_FILE_ATTRIBUTES) || 
                         (checkGzip && isGzFile(name));

    return success;
}


// Does the directory exist
bool isDir(const fileName& name)
{
    const DWORD attrs = ::GetFileAttributes(name.c_str());
    bool success = (attrs != INVALID_FILE_ATTRIBUTES) &&
                   (attrs & FILE_ATTRIBUTE_DIRECTORY);

    return success;
}


// Does the file exist
bool isFile(const fileName& name, const bool checkGzip)
{
    const DWORD attrs = ::GetFileAttributes(name.c_str());
    const bool success = ((attrs != INVALID_FILE_ATTRIBUTES) && 
			  !(attrs & FILE_ATTRIBUTE_DIRECTORY)) || 
                         (checkGzip && isGzFile(name));

    return success;
}


// Return size of file
off64_t fileSize(const fileName& name)
{
    fileStat fileStatus(name);

    const off64_t fileSize = fileStatus.isValid() ?
      fileStatus.status().st_size : -1;
    return fileSize;
}


// Return time of last file modification
time_t lastModified(const fileName& name)
{
    fileStat fileStatus(name);

    const time_t modifiedTime = fileStatus.isValid() ?
      fileStatus.status().st_mtime : 0;
    return modifiedTime;
}


// Read a directory and return the entries as a string list
fileNameList readDir
(
    const fileName& directory,
    const fileName::Type type,
    const bool filtergz
)
{
    // Initial filename list size
    // also used as increment if initial size found to be insufficient
    const int maxNnames = 100;

    if (MSwindows::debug)
    {
        std::cout<< "readDir(const fileName&, const fileType, const bool filtergz)"
            << " : reading directory " << directory << std::endl;
    }

    // Setup empty string list MAXTVALUES long
    fileNameList dirEntries(maxNnames);

    // Temporary variables and counters
    label nEntries = 0;

    MSwindows::DirectoryIterator dirIt(directory);

    if (dirIt.isValid())
    {
        while (dirIt.hasNext())
        {
            const fileName & fName = dirIt.next();

            // ignore files begining with ., i.e. '.', '..' and '.*'
            if (fName.size() > 0 && fName[size_t(0)] != '.')
            {
                word fileNameExt = fName.ext();

                if
                (
                    (type == fileName::DIRECTORY)
                 ||
                    (
                        type == fileName::FILE
                        && fName[fName.size()-1] != '~'
                        && fileNameExt != "bak"
                        && fileNameExt != "BAK"
                        && fileNameExt != "old"
                        && fileNameExt != "save"
                    )
                )
                {
                    if ((directory/fName).type() == type)
                    {
                        if (nEntries >= dirEntries.size())
                        {
                            dirEntries.setSize(dirEntries.size() + maxNnames);
                        }

                        if (filtergz && fileNameExt == "gz")
                        {
                            dirEntries[nEntries++] = fName.lessExt();
                        }
                        else
                        {
                            dirEntries[nEntries++] = fName;
                        }
                    }
                }
            }
        }
    }
    else if (MSwindows::debug)
    {
        std::cout<< "readDir(const fileName&, const fileType, "
               "const bool filtergz) : cannot open directory "
            << directory << std::endl;
    }

    // Reset the length of the entries list
    dirEntries.setSize(nEntries);
    
    return dirEntries;
}


// Copy, recursively if necessary, the source top the destination
bool cp(const fileName& src, const fileName& dest)
{
    // Make sure source exists.
    if (!exists(src))
    {
        return false;
    }

    fileName destFile(dest);

    // Check type of source file.
    if (src.type() == fileName::FILE)
    {
        // If dest is a directory, create the destination file name.
        if (destFile.type() == fileName::DIRECTORY)
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
    else if (src.type() == fileName::DIRECTORY)
    {
        // If dest is a directory, create the destination file name.
        if (destFile.type() == fileName::DIRECTORY)
        {
            destFile = destFile/src.component(src.components().size() -1);
        }

        // Make sure the destination directory extists.
        if (!isDir(destFile) && !mkDir(destFile))
        {
            return false;
        }

        // Copy files
        fileNameList contents = readDir(src, fileName::FILE, false);
        forAll(contents, i)
        {
            if (MSwindows::debug)
            {
                std::cout<< "Copying : " << src/contents[i] 
                    << " to " << destFile/contents[i] << std::endl;
            }

            // File to file.
            cp(src/contents[i], destFile/contents[i]);
        }

        // Copy sub directories.
        fileNameList subdirs = readDir(src, fileName::DIRECTORY);
        forAll(subdirs, i)
        {
            if (MSwindows::debug)
            {
                std::cout<< "Copying : " << src/subdirs[i]
                    << " to " << destFile << std::endl;
            }

            // Dir to Dir.
            cp(src/subdirs[i], destFile);
        }
    }

    return true;
}


// Create a softlink. destFile should not exist. Returns true if successful.
bool ln(const fileName& src, const fileName& dest)
{
    // Seems that prior to Vista softlinking was poorly supported.
    // Vista does a better job, but requires adminstrator privileges.
    // Skip for now.

    if (MSwindows::debug)
    {
        std::cout<< "MSwindows does not support ln - softlinking" << std::endl;
    }

    return false;
}


// Rename srcFile destFile
bool mv(const fileName& srcFile, const fileName& destFile)
{
    if (MSwindows::debug)
    {
        std::cout<< "Move : " << srcFile << " to " << destFile << std::endl;
    }

    const fileName destName = 
      ((destFile.type() == fileName::DIRECTORY)
       && (srcFile.type() != fileName::DIRECTORY)) ?
      destFile/srcFile.name() :
      destFile;

    const bool success = 
      (0 == std::rename(srcFile.c_str(), destName.c_str()));

    return success;
}


//- Rename to a corresponding backup file
//  If the backup file already exists, attempt with "01" .. "99" index
bool mvBak(const fileName& src, const std::string& ext)
{
    if (MSwindows::debug)
    {
        std::cout<< "mvBak : " << src << " to extension " << ext << std::endl;
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
        std::cout<< "Removing : " << file << std::endl;
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
        std::cout<< "rmdir(const fileName&) : "
            << "removing directory " << directory << std::endl;
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

              if (path.type() == fileName::DIRECTORY)
              {
                  success = rmDir(path);

                  if (!success)
                  {
                      WarningIn("rmdir(const fileName&)")
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
                      WarningIn("rmdir(const fileName&)")
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
            WarningIn("rmdir(const fileName&)")
                << "failed to remove directory " << directory << endl;
        }
    }

    return success;
}


//- Sleep for the specified number of seconds
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
        FatalErrorIn
        (
            "Foam::fdClose(const int fd)"
        )   << "close error on " << fd << endl
            << abort(FatalError);    
    }
}


//- Check if machine is up by pinging given port
bool ping
(
    const word& destName,
    const label destPort,
    const label timeOut
)
{
    // Appears that socket calls require adminstrator privileges.
    // Skip for now.

    if (MSwindows::debug)
    {
        std::cout<< "MSwindows does not support ping" << std::endl;
    }

    return false;
}


//- Check if machine is up by ping port 22 = ssh and 222 = rsh
bool ping(const word& hostname, const label timeOut)
{
    return ping(hostname, 222, timeOut) || ping(hostname, 22, timeOut);
}


int system(const std::string& command)
{
    return std::system(command.c_str());
}


// Explicitly track loaded libraries, rather than use
// EnumerateLoadedModules64 and have to link against 
// Dbghelp.dll
// Details at http://msdn.microsoft.com/en-us/library/ms679316(v=vs.85).aspx
typedef std::map<void*, std::string> OfLoadedLibs;

static
OfLoadedLibs &
getLoadedLibs()
{
  static OfLoadedLibs loadedLibs;
  return loadedLibs;
}


//- Open shared library
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
          std::cout<< "Libraries to be loaded: " <<  libsToLoad << std::endl;
      }

      //generate the word list
      size_t stposstr=0, found=libsToLoad.find_first_of(',');
      while (found!=string::npos)
      {
          string libToLoad = libsToLoad.substr(stposstr,found-stposstr);
          moduleh = dlOpen(libToLoad); //FIX: module handle is ignored and maybe it shouldn't
          stposstr=found+1; found=libsToLoad.find_first_of(',',stposstr);
      }

      return moduleh;
    }
    else
    {
      if (MSwindows::debug)
      {
          std::cout<< "dlOpen(const fileName&)"
              << " : LoadLibrary of " << libName << std::endl;
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
          WarningIn("dlOpen(const fileName&)")
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
          std::cout<< "Library " <<  libName << " loaded "
              << (libHandle != NULL ? "with success!" : "without success.")
              << std::endl;
      }

      return libHandle;
    }
}


//- Close shared library
bool dlClose(void* libHandle)
{
    if (MSwindows::debug)
    {
        std::cout<< "dlClose(void*)"
            << " : FreeLibrary of handle " << libHandle << std::endl;
    }

    const bool success = 
      ::FreeLibrary(static_cast<HMODULE>(libHandle));
  
    if (!success) 
    {
        WarningIn("closeLibrary(void* const libHandle)")
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


//- Lookup a symbol in a dlopened library using handle to library
void* dlSym(void* handle, const std::string& symbol)
{
    if (MSwindows::debug)
    {
        std::cout<< "dlSym(void*, const std::string&)"
            << " : dlsym of " << symbol << std::endl;
    }

    // get address of symbol
    void* fun = (void *)(::GetProcAddress(static_cast<HMODULE>(handle), symbol.c_str()));

    if(fun == NULL)
    {
        WarningIn("dlSym(void*, const std::string&)")
            << "Cannot lookup symbol " << symbol << " : " << MSwindows::getLastError()
            << endl;
    }

    return fun;
}


//- Report if symbol in a dlopened library could be found
bool dlSymFound(void* handle, const std::string& symbol)
{
    if (handle && !symbol.empty())
    {
        if (MSwindows::debug)
        {
            std::cout<< "dlSymFound(void*, const std::string&)"
                << " : dlSym of " << symbol << std::endl;
        }

        // symbol can be found if there was no error
        return dlSym(handle, symbol.c_str()) != NULL;
    }
    else
    {
        return false;
    }
}


//- Return all loaded libraries
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
        std::cout<< "dlLoaded()"
            << " : determined loaded libraries :" << libs.size() << std::endl;
    }
    return libs;
}

//It's easier to include it here...
#include "random.c"

//- Random functions
void osRandomSeed(const label seed)
{
    srandom(unsigned int(seed));
}

label osRandomInteger()
{
    return random();
}

scalar osRandomDouble()
{
    return scalar(random()) / scalar(INT_MAX);
}

string toUnixPath(const string & path)
{
    string unixPath(path);
    MSwindows::toUnixSlash(unixPath);
    MSwindows::removeQuotes(unixPath);

    return unixPath;
}


// Thread handling: Using std::thread and std::mutex

#include <thread>
#include <mutex>

static DynamicList<autoPtr<std::thread>> threads_;
static DynamicList<autoPtr<std::mutex>> mutexes_;


label allocateThread()
{
    forAll(threads_, i)
    {
        if (!threads_[i].valid())
        {
            if (MSWindows::debug)
            {
                Pout<< "allocateThread : reusing index:" << i << endl;
            }
            // Reuse entry
            threads_[i].reset(new std::thread());
            return i;
        }
    }

    label index = threads_.size();
    if (MSWindows::debug)
    {
        Pout<< "allocateThread : new index:" << index << endl;
    }
    threads_.append(autoPtr<std::thread>(new std::thread()));

    return index;
}


void createThread
(
    const label index,
    void *(*start_routine) (void *),
    void *arg
)
{
    if (MSWindows::debug)
    {
        Pout<< "createThread : index:" << index << endl;
    }

    try
    {
        threads_[index].reset(std::thread(start_routine, arg));
    }
    catch(...)
    {
        FatalErrorInFunction
            << "Failed starting thread " << index << exit(FatalError);
    }
}


void joinThread(const label index)
{
    if (MSWindows::debug)
    {
        Pout<< "joinThread : join:" << index << endl;
    }

    if (!threads_[index]().joinable())
    {
        FatalErrorInFunction << "Failed freeing thread " << index
            << exit(FatalError);
    }
    else
    {
        try
        {
            threads_[index]().join();
        }
        catch(...)
        {
            FatalErrorInFunction << "Failed freeing thread " << index
                << exit(FatalError);
        }
    }
}


void freeThread(const label index)
{
    if (MSWindows::debug)
    {
        Pout<< "freeThread : index:" << index << endl;
    }
    threads_[index].clear();
}


label allocateMutex()
{
    forAll(mutexes_, i)
    {
        if (!mutexes_[i].valid())
        {
            if (MSWindows::debug)
            {
                Pout<< "allocateMutex : reusing index:" << i << endl;
            }
            // Reuse entry
            mutexes_[i].reset(new std::mutex());
            return i;
        }
    }

    label index = mutexes_.size();

    if (MSWindows::debug)
    {
        Pout<< "allocateMutex : new index:" << index << endl;
    }
    mutexes_.append(autoPtr<std::mutex>(new std::mutex()));
    return index;
}


void lockMutex(const label index)
{
    if (MSWindows::debug)
    {
        Pout<< "lockMutex : index:" << index << endl;
    }

    try
    {
        mutexes_[index]().lock();
    }
    catch(...)
    {
        FatalErrorInFunction << "Failed locking mutex " << index
            << exit(FatalError);
    }
}


void unlockMutex(const label index)
{
    if (MSWindows::debug)
    {
        Pout<< "unlockMutex : index:" << index << endl;
    }

    try
    {
        mutexes_[index]().unlock();
    }
    catch(...)
    {
        FatalErrorInFunction << "Failed unlocking mutex " << index
            << exit(FatalError);
    }
}


void freeMutex(const label index)
{
    if (MSWindows::debug)
    {
        Pout<< "freeMutex : index:" << index << endl;
    }
    mutexes_[index].clear();
}


} // namespace Foam
// ************************************************************************* //
