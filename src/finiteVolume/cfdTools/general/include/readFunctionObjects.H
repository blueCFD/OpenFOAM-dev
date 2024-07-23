if (runTime.functionObjects().readOpt() != IOobject::NO_READ)
{
    runTime.functionObjects().read();
}
