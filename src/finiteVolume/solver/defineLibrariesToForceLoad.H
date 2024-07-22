// July 2024 FS Dynamics Portugal: Created this file for blueCFD-Core
// Changes are tracked at: https://github.com/blueCFD/OpenFOAM-dev
//
// LIBS_TO_LOAD - introduced by FS Dynamics Portugal (blueCAPE) on February 2010
// This allows to forcefully load libraries crucial to the application to run
// on Windows

#if defined(WIN32) || defined(WIN64)

#ifdef LFL_HEADER_MODE
#undef LFL_HEADER_MODE
protected:
    virtual fileName librariesToForceLoad();

public:

#else //LFL_HEADER_MODE

#ifdef PARENT_CLASS
    Foam::fileName LFL_CLASS_PREFIX::librariesToForceLoad()
    {
        return PARENT_CLASS::librariesToForceLoad() + LIBS_TO_LOAD;
    }
#else
    Foam::fileName LFL_CLASS_PREFIX::librariesToForceLoad()
    {
        return LIBS_TO_LOAD;
    }
#endif //PARENT_CLASS

#undef PARENT_CLASS
#undef LFL_CLASS_PREFIX

#endif //LFL_HEADER_MODE

#endif //WIN*
