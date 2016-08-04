Foam::argList args(argc, argv);
if (!args.checkRootCase())
{
    Foam::FatalError.exit();
}

//Hack by blueCAPE Lda 2009
#include "forceLoadLibraries.H"
