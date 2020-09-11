/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2011-2020 OpenFOAM Foundation
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

\*---------------------------------------------------------------------------*/

#include "fieldAverageItem.H"
#include "fieldAverage.H"
#include "wordAndDictionary.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::functionObjects::fieldAverageItem::fieldAverageItem
(
    const fieldAverage& fa,
    Istream& is
)
:
    fieldName_("unknown"),
    mean_(false),
    meanFieldName_("unknown"),
    prime2Mean_(false),
    prime2MeanFieldName_("unknown")
{
    is.check
    (
        "Foam::Istream& Foam::operator>>"
        "(Foam::Istream&, Foam::functionObjects::fieldAverageItem&)"
    );

    wordAndDictionary wd(is);

    fieldName_ = wd.first();

    mean_ = wd.second().lookupOrDefault<Switch>("mean", fa.mean_);
    prime2Mean_ =
        wd.second().lookupOrDefault<Switch>("prime2Mean", fa.prime2Mean_);

    meanFieldName_ = IOobject::groupName
    (
        IOobject::member(fieldName_) + fieldAverageItem::meanExt,
        IOobject::group(fieldName_)
    );

    prime2MeanFieldName_ = IOobject::groupName
    (
        IOobject::member(fieldName_) + fieldAverageItem::prime2MeanExt,
        IOobject::group(fieldName_)
    );

    if ((fa.window_ > 0) && (fa.windowName_ != ""))
    {
        meanFieldName_ =
            meanFieldName_ + "_" + fa.windowName_;

        prime2MeanFieldName_ =
            prime2MeanFieldName_ + "_" + fa.windowName_;
    }
}


// ************************************************************************* //
