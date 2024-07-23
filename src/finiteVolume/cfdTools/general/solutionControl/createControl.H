#if defined(NO_CONTROL)
#elif defined(PISO_CONTROL)
    #include "createPisoControl.H"
#elif defined(PIMPLE_CONTROL)
    #include "createPimpleControl.H"
#elif defined(SIMPLE_CONTROL)
    #include "createSimpleControl.H"
#endif

// blueCFD-Core: must read the dictionary before creating the fields, so
// that the user-selected renaming mechanism works as intended
#include "readFunctionObjects.H"
