/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2026 OpenFOAM Foundation
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
    Foam::DimensionedFieldFunctions::Zonal

Description
    Zonal internal and patch field initialisation function

    The field is initialised to the given \c defaultValue and then updated to
    the \c value provided for each zone (cellZones for the internalField of
    volFields or faceZones for patchFields).

Usage
    To set the water column in the \c alpha.water field for the damBreak cases
    specifying a default value of 0 and a value of 1 in a \c box \c cellZone:
    \verbatim
    internalField
    {
        type        zonal;

        defaultValue    0;

        zones
        {
            waterColumn
            {
                type            box;

                box             (0 0 -1) (0.1461 0.292 1);

                value           1;
            }
        }
    }
    \endverbatim

    The value of inlet and other fixed value patch fields can be specified using
    DimensionedFieldFunctions::Zonal with the Foam::functionalFixedValue patch
    field.  It is also possible to use the same zone specification as used for
    the internalField initialisation and filter it using the patch faceZone
    for efficiency, e.g.
    \verbatim
    internalField
    {
        type        zonal;

        defaultValue    0;

        zones
        {
            water
            {
                type            box;
                box             (-999 -999 -999) (999 999 0.244);
                value           1;
            }
        }
    }

    boundaryField
    {
        inlet
        {
            type            functionalFixedValue;

            value
            {
                $internalField;

                zones
                {
                    water
                    {
                        zone
                        {
                            type        patch;
                            patch       inlet;
                        }
                    }
                }
            }
        }
        .
        .
        .
    \endverbatim

SourceFiles
    Zonal_DimensionedFieldFunction.C

\*---------------------------------------------------------------------------*/

#ifndef Zonal_DimensionedFieldFunction_H
#define Zonal_DimensionedFieldFunction_H

#include "DimensionedFieldFunction.T.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace DimensionedFieldFunctions
{

/*---------------------------------------------------------------------------*\
                            Class Zonal Declaration
\*---------------------------------------------------------------------------*/

template<class DimensionedFieldType>
class Zonal
:
    public DimensionedFieldFunction<DimensionedFieldType>
{
    typedef typename DimensionedFieldType::Type_ Type;
    typedef typename DimensionedFieldType::GeoMesh_::Zone Zone;

    //- Default value
    Type value_;

    //- Cached zone and value specification dictionary
    dictionary zonesDict_;


public:

    //- Runtime type information
    TypeName("zonal");


    // Constructors

        //- Construct with dictionary to initialise given field
        Zonal
        (
            const dictionary& dict,
            DimensionedFieldType& field
        );

        //- Construct a copy for the given field
        Zonal
        (
            const Zonal& dff,
            DimensionedFieldType& field
        );

        //- Construct and return a clone for the specified field
        virtual autoPtr<DimensionedFieldFunction<DimensionedFieldType>>
        clone(DimensionedFieldType& field) const;


    //- Destructor
    virtual ~Zonal()
    {}


    // Member Functions

        //- Evaluate the function and set the field
        virtual void evaluate();

        //- Write data to dictionary stream
        virtual void write(Ostream& os) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace DimensionedFieldFunctions
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#include "Zonal_DimensionedFieldFunction.T.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
