/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2014-2026 OpenFOAM Foundation
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

Class
    Foam::atmosphericBoundaryLayer

Description
    This class provides functions to evaluate the velocity and turbulence
    distributions appropriate for atmospheric boundary layers (ABL).

    The profile is derived from the friction velocity, flow direction and
    "vertical" direction:

        \f[
            U = \frac{U^*}{\kappa} ln\left(\frac{z - z_g + z_0}{z_0}\right)
        \f]

        \f[
            k = \frac{(U^*)^2}{\sqrt{C_mu}}
        \f]

        \f[
            \epsilon = \frac{(U^*)^3}{\kappa(z - z_g + z_0)}
        \f]

    where
    \vartable
        U^*     | Friction velocity
        \kappa  | von Karman's constant
        C_mu    | Turbulence viscosity coefficient
        z       | Vertical coordinate
        z_0     | Surface roughness height [m]
        z_g     | Minimum z-coordinate [m]
    \endvartable
    and
        \f[
            U^* = \kappa\frac{U_{ref}}{ln\left(\frac{Z_{ref} + z_0}{z_0}\right)}
        \f]
    where
    \vartable
        U_{ref} | Reference velocity at \f$Z_{ref}\f$ [m/s]
        Z_{ref} | Reference height [m]
    \endvartable

    The specification of the atmospheric boundary layer is centralised in the
    constant/atmosphericBoundaryLayerProperties dictionary which is read once on
    construction of the atmosphericBoundaryLayer class using the
    atmosphericBoundaryLayer::New factory method and cached automatically for
    subsequent lookup so a single instance is used by all atmospheric boundary
    conditions and internal field initialisation.

    Used in the
    Foam::atmosphericBoundaryLayerVelocity,
    Foam::atmosphericBoundaryLayerTurbulentKineticEnergy,
    Foam::atmosphericBoundaryLayerTurbulentEpsilon and
    Foam::atmosphericBoundaryLayerNutWallFunction boundary conditions.

    Reference:
        D.M. Hargreaves and N.G. Wright,  "On the use of the k-epsilon model
        in commercial CFD software to model the neutral atmospheric boundary
        layer", Journal of Wind Engineering and Industrial Aerodynamics
        95(2007), pp 355-369.

Usage
    \table
        Property     | Description                      | Required  | Default
        flowDir      | Flow direction                   | yes       |
        zDir         | Vertical direction               | yes       |
        kappa        | von Karman's constant            | no        | 0.41
        Cmu          | Turbulence viscosity coefficient | no        | 0.09
        Uref         | Reference velocity [m/s]         | yes       |
        Zref         | Reference height [m]             | yes       |
        z0           | Surface roughness height [m]     | yes       |
        zGround      | Minimum z-coordinate [m]         | yes       |
        Ulower       | Velocity below the BL            | no        |
        kLower       | Turbulence k below the BL        | no        |
        epsilonLower | Turbulence epsilon below the BL  | no        |
    \endtable

    Example of the atmosphericBoundaryLayerProperties specification:
    \verbatim
        flowDir         (1 0 0);
        zDir            (0 0 1);
        Uref            10.0;
        Zref            20.0;
        z0              uniform 0.1;
        zGround         uniform 0.0;
    \endverbatim

    Note:
        D.M. Hargreaves and N.G. Wright recommend Gamma epsilon in the
        k-epsilon model should be changed from 1.3 to 1.11 for consistency.
        The roughness height (Er) is given by Er = 20 z0 following the same
        reference.

SourceFiles
    atmosphericBoundaryLayer.C

\*---------------------------------------------------------------------------*/

#ifndef atmosphericBoundaryLayer_H
#define atmosphericBoundaryLayer_H

#include "IOdictionary.H"
#include "primitiveFields.H"
#include "Function2.T.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                      Class atmosphericBoundaryLayer Declaration
\*---------------------------------------------------------------------------*/

class atmosphericBoundaryLayer
:
    public IOdictionary
{
    // Private static data

        //- Default value of the Von Karman constant
        static const scalar kappaDefault_;

        //- Default value of the turbulent viscosity coefficient
        static const scalar CmuDefault_;


    // Private Data

        //- Flow direction (x-coordinate)
        vector flowDir_;

        //- Direction of the z-coordinate
        vector zDir_;

        //- Direction of the y-coordinate
        vector yDir_;

        //- Von Karman constant
        const scalar kappa_;

        //- Turbulent viscosity coefficient
        const scalar Cmu_;

        //- Reference velocity
        const scalar Uref_;

        //- Reference height
        const scalar Zref_;

        //- Surface roughness height
        autoPtr<Function2<scalar>> z0_;

        //- Minimum coordinate value in z direction
        autoPtr<Function2<scalar>> zGround_;

        //- True if the boundary layer is offset within the domain
        bool offset_;

        //- Velocity of the flow below the boundary layer
        const scalar Ulower_;

        //- Turbulence kinetic energy of the flow below the boundary layer
        const scalar kLower_;

        //- Turbulence kinetic energy dissipation rate of the flow below the
        //  boundary layer
        const scalar epsilonLower_;


public:

    //- Runtime type information
    TypeName("atmosphericBoundaryLayer");


    // Static Data

        //- The name of the dictionary
        static const word dictName;


    // Constructors

        //- Read construct from objectRegistry
        explicit atmosphericBoundaryLayer(const objectRegistry& obr);


    // Selectors

        //- Return a reference to the atmosphericBoundaryLayer
        //  from the database, constructing and storing if it doesn't exist
        static const atmosphericBoundaryLayer& New(const objectRegistry& db);


    // Member Functions

        // Access

            //- Von Karman constant
            scalar kappa() const
            {
                return kappa_;
            }

            //- Turbulent viscosity coefficient
            scalar Cmu() const
            {
                return Cmu_;
            }

            //- Return flow direction
            const vector& flowDir() const
            {
                return flowDir_;
            }

            //- Return z-direction
            const vector& zDir() const
            {
                return zDir_;
            }

            //- Return surface roughness height
            tmp<scalarField> z0(const vectorField& C) const;

            //- Return friction velocity
            tmp<scalarField> Ustar(const scalarField& z0) const;


        // Evaluate functions

            //- Return the velocity field
            tmp<vectorField> U(const vectorField& p) const;

            //- Return the turbulent kinetic energy field
            tmp<scalarField> k(const vectorField& p) const;

            //- Return the turbulent dissipation rate field
            tmp<scalarField> epsilon(const vectorField& p) const;


        //- Inherit write from regIOobject
        using regIOobject::write;

        //- Write
        void write(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
