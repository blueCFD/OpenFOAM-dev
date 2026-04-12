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

\*---------------------------------------------------------------------------*/

#include "UDimensionedFieldFunction.T.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type, class GeoMesh>
Foam::UDimensionedFieldFunction<Type, GeoMesh>::UDimensionedFieldFunction
(
    const word& name,
    const word& funcName,
    const GeoMesh& mesh,
    const dimensionSet& dimensions,
    Field<Type>& field,
    const dictionary& dict
)
:
    field_(field),
    funcName_(funcName),
    dimensionedField_
    (
        IOobject
        (
            name + '_' + funcName,
            mesh.time().name(),
            mesh.db(),
            IOobject::NO_READ,
            IOobject::NO_WRITE,
            false
        ),
        mesh,
        dimensions,
        field_
    ),
    funcPtr_
    (
        dict.isDict(funcName)
      ? DimensionedFieldFunction<DimensionedField<Type, GeoMesh>>::New
        (
            dict.subDict(funcName),
            dimensionedField_
        )
      : autoPtr<DimensionedFieldFunction<DimensionedField<Type, GeoMesh>>>
        (
            nullptr
        )
    )
{
    if (funcPtr_.valid())
    {
        if (mesh.time().completeCase())
        {
            funcPtr_->evaluate();
        }
    }
    else
    {
        field_ = Field<Type>(funcName_, dimensions, dict, mesh.size());
    }
}


template<class Type, class GeoMesh>
Foam::UDimensionedFieldFunction<Type, GeoMesh>::UDimensionedFieldFunction
(
    const word& name,
    const word& funcName,
    const GeoMesh& mesh,
    const dimensionSet& dimensions,
    Field<Type>& field,
    const dictionary& dict,
    const Type& defaultValue
)
:
    field_(field),
    funcName_(funcName),
    dimensionedField_
    (
        IOobject
        (
            name + '_' + funcName,
            mesh.time().name(),
            mesh.db(),
            IOobject::NO_READ,
            IOobject::NO_WRITE,
            false
        ),
        mesh,
        dimensions,
        field_
    ),
    funcPtr_
    (
        dict.isDict(funcName)
      ? DimensionedFieldFunction<DimensionedField<Type, GeoMesh>>::New
        (
            dict.subDict(funcName),
            dimensionedField_
        )
      : autoPtr<DimensionedFieldFunction<DimensionedField<Type, GeoMesh>>>
        (
            nullptr
        )
    )
{
    if (funcPtr_.valid())
    {
        if (mesh.time().completeCase())
        {
            funcPtr_->evaluate();
        }
    }
    else if (dict.found(funcName))
    {
        field_ = Field<Type>(funcName_, dimensions, dict, mesh.size());
        dimensionedField_.reset(field_);
    }
    else
    {
        field_ = Field<Type>(mesh.size(), defaultValue);
        dimensionedField_.reset(field_);
    }
}


template<class Type, class GeoMesh>
Foam::UDimensionedFieldFunction<Type, GeoMesh>::UDimensionedFieldFunction
(
    const UDimensionedFieldFunction<Type, GeoMesh>& udff,
    const GeoMesh& mesh,
    Field<Type>& field
)
:
    field_(field),
    funcName_(udff.funcName_),
    dimensionedField_
    (
        udff.dimensionedField_,
        mesh,
        udff.dimensionedField_.dimensions(),
        field_
    ),
    funcPtr_
    (
        udff.funcPtr_.valid()
      ? udff.funcPtr_->clone(dimensionedField_)
      : autoPtr<DimensionedFieldFunction<DimensionedField<Type, GeoMesh>>>
        (
            nullptr
        )
    )
{}


template<class Type, class GeoMesh>
Foam::UDimensionedFieldFunction<Type, GeoMesh>::UDimensionedFieldFunction
(
    const UDimensionedFieldFunction<Type, GeoMesh>& udff,
    Field<Type>& field
)
:
    field_(field),
    funcName_(udff.funcName_),
    dimensionedField_
    (
        udff.dimensionedField_,
        udff.dimensionedField_.mesh(),
        udff.dimensionedField_.dimensions(),
        field_
    ),
    funcPtr_
    (
        udff.funcPtr_.valid()
      ? udff.funcPtr_->clone(dimensionedField_)
      : autoPtr<DimensionedFieldFunction<DimensionedField<Type, GeoMesh>>>
        (
            nullptr
        )
    )
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type, class GeoMesh>
void Foam::UDimensionedFieldFunction<Type, GeoMesh>::map(const bool evaluate)
{
    dimensionedField_.reset(field_);
    if (evaluate && funcPtr_.valid())
    {
        funcPtr_->reset();
    }
}


template<class Type, class GeoMesh>
void Foam::UDimensionedFieldFunction<Type, GeoMesh>::reset()
{
    dimensionedField_.reset(field_);

    if (funcPtr_.valid())
    {
        funcPtr_->reset();
    }
}


template<class Type, class GeoMesh>
bool Foam::UDimensionedFieldFunction<Type, GeoMesh>::update()
{
    if (funcPtr_.valid())
    {
        return funcPtr_->update();
    }
    else
    {
        return false;
    }
}


template<class Type, class GeoMesh>
void Foam::UDimensionedFieldFunction<Type, GeoMesh>::write(Ostream& os) const
{
    if (funcPtr_.valid())
    {
        funcPtr_->write(os);
    }
}


template<class Type, class GeoMesh>
void Foam::writeEntry
(
    Ostream& os,
    const UDimensionedFieldFunction<Type, GeoMesh>& udff
)
{
    if (udff.funcPtr_.valid())
    {
        writeEntry(os, udff.funcName_, *udff.funcPtr_);
    }
    else
    {
        writeEntry(os, udff.funcName_, udff.field_);
    }
}


// ************************************************************************* //
