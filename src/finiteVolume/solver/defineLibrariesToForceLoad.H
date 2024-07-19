// July 2024 FS Dynamics Portugal: Created this file for blueCFD-Core
// Changes are tracked at: https://github.com/blueCFD/OpenFOAM-dev
//
// LIBS_TO_LOAD - introduced by FS Dynamics Portugal (blueCAPE) on February 2010
// This allows to forcefully load libraries crucial to the application to run
// on Windows

protected:
#if defined(WIN32) || defined(WIN64)
#ifdef PARENT_CLASS
    virtual fileName librariesToForceLoad()
    {
        return PARENT_CLASS::librariesToForceLoad() + LIBS_TO_LOAD;
    }
#else
    virtual fileName librariesToForceLoad()
    {
        return LIBS_TO_LOAD;
    }
#endif
#endif
public:
