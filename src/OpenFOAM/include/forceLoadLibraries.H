// February 2010 blueCAPE Lda: Created this file for blueCFD
// 2014-02-21 blueCAPE Lda: Adaptation for Modifications for blueCFD-Core 2.3

#if defined(WIN32) || defined(WIN64)
//LIBS_TO_LOAD - introduced by blueCAPE (bluecape.com.pt) on February 2010
//This allows to forcefully load libraries crucial to the application to run on Windows
#ifdef LIBS_TO_LOAD
  //example of LIBS_TO_LOAD: "OpenFOAM,incompressibleRASModels,Pstream"
  Foam::string libs_to_load(LIBS_TO_LOAD);
  Foam::dlOpen(libs_to_load);
#endif
#endif
