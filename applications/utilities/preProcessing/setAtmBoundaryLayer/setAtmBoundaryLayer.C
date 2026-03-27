/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2022-2026 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

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

Application
    setAtmBoundaryLayer

Description
    Applies atmospheric boundary layer models to the entire domain for case
    initialisation.

\*---------------------------------------------------------------------------*/

#include "argList.H"
#include "timeSelector.H"
#include "volFields.H"
#include "wallFvPatch.H"
#include "atmosphericBoundaryLayer.H"

using namespace Foam;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    timeSelector::addOptions(false, false);

    #include "addDictOption.H"
    #include "addRegionOption.H"
    #include "setRootCase.H"
    #include "createTime.H"

    const instantList timeDirs = timeSelector::selectIfPresent(runTime, args);

    #include "createRegionMeshNoChangers.H"

    const atmosphericBoundaryLayer& atm =
        atmosphericBoundaryLayer::New(runTime);

    // Set field names if specified
    const word UName = atm.lookupOrDefault<word>("U", "U");
    const word kName = atm.lookupOrDefault<word>("k", "k");
    const word epsilonName = atm.lookupOrDefault<word>("epsilon", "epsilon");

    forAll(timeDirs, timeI)
    {
        runTime.setTime(timeDirs[timeI], timeI);

        Info<< "Time = " << runTime.userTimeName() << nl << endl;

        mesh.readUpdate();

        // Read the fields
        volVectorField U
        (
            IOobject
            (
                UName,
                runTime.name(),
                mesh,
                IOobject::MUST_READ
            ),
            mesh
        );
        IOobject kIO
        (
            kName,
            runTime.name(),
            mesh,
            IOobject::MUST_READ
        );
        tmp<volScalarField> k
        (
            kIO.headerOk() ? new volScalarField(kIO, mesh) : nullptr
        );
        IOobject epsilonIO
        (
            epsilonName,
            runTime.name(),
            mesh,
            IOobject::MUST_READ
        );
        tmp<volScalarField> epsilon
        (
            epsilonIO.headerOk() ? new volScalarField(epsilonIO, mesh) : nullptr
        );

        // Set the internal fields

        U.primitiveFieldRef() = atm.U(mesh.C());
        if (k.valid())
        {
            k.ref().primitiveFieldRef() = atm.k(mesh.C());
        }
        if (epsilon.valid())
        {
            epsilon.ref().primitiveFieldRef() = atm.epsilon(mesh.C());
        }

        // Set all non-wall patch fields
        forAll(mesh.boundary(), patchi)
        {
            const fvPatch& patch = mesh.boundary()[patchi];

            if (!isA<wallFvPatch>(patch))
            {
                U.boundaryFieldRef()[patchi] == atm.U(patch.Cf());
                if (k.valid())
                {
                    k.ref().boundaryFieldRef()[patchi] ==
                        atm.k(patch.Cf());
                }
                if (epsilon.valid())
                {
                    epsilon.ref().boundaryFieldRef()[patchi] ==
                        atm.epsilon(patch.Cf());
                }
            }
        }

        // Output
        Info<< "Writing " << U.name() << nl << endl;
        U.write();
        if (k.valid())
        {
            Info<< "Writing " << k->name() << nl << endl;
            k->write();
        }
        if (epsilon.valid())
        {
            Info<< "Writing " << epsilon->name() << nl << endl;
            epsilon->write();
        }
    }

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
