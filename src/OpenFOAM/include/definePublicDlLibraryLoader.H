// July 2024, FS Dynamics Portugal: Created this file for blueCFD-Core
// Changes are tracked at: https://github.com/blueCFD/OpenFOAM-dev
//
// LIBS_TO_LOAD - introduced by FS Dynamics Portugal (blueCAPE) on February 2010
// This allows to forcefully load libraries crucial to the application to run
// on Windows

// Must always define on Windows, just in case there is a module that does not
// need extra libraries
#if defined(WIN32) || defined(WIN64)
    public virtual dlLibraryLoader,
#endif
