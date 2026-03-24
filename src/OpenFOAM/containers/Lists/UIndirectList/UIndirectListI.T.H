/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2011-2026 OpenFOAM Foundation
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

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class T>
inline Foam::UIndirectList<T>::UIndirectList
(
    const UList<T>& completeList,
    const UList<label>& addr
)
:
    completeList_(const_cast<UList<T>&>(completeList)),
    addressing_(addr)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class T>
inline Foam::label Foam::UIndirectList<T>::size() const
{
    return addressing_.size();
}


template<class T>
inline bool Foam::UIndirectList<T>::empty() const
{
    return addressing_.empty();
}


template<class T>
inline T& Foam::UIndirectList<T>::first()
{
    return completeList_[addressing_.first()];
}


template<class T>
inline const T& Foam::UIndirectList<T>::first() const
{
    return completeList_[addressing_.first()];
}


template<class T>
inline T& Foam::UIndirectList<T>::last()
{
    return completeList_[addressing_.last()];
}


template<class T>
inline const T& Foam::UIndirectList<T>::last() const
{
    return completeList_[addressing_.last()];
}


template<class T>
inline Foam::label Foam::UIndirectList<T>::fcIndex(const label i) const
{
    return addressing_.fcIndex(i);
}


template<class T>
inline Foam::label Foam::UIndirectList<T>::rcIndex(const label i) const
{
    return addressing_.rcIndex(i);
}


template<class T>
inline const Foam::UList<T>& Foam::UIndirectList<T>::completeList() const
{
    return completeList_;
}


template<class T>
inline const Foam::List<Foam::label>& Foam::UIndirectList<T>::addressing() const
{
    return addressing_;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class T>
inline Foam::List<T> Foam::UIndirectList<T>::operator()() const
{
    List<T> result(size());

    forAll(*this, i)
    {
        result[i] = operator[](i);
    }

    return result;
}


template<class T>
inline T& Foam::UIndirectList<T>::operator[](const label i)
{
    return completeList_[addressing_[i]];
}


template<class T>
inline const T& Foam::UIndirectList<T>::operator[](const label i) const
{
    return completeList_[addressing_[i]];
}


template<class T>
inline void Foam::UIndirectList<T>::operator=(const UList<T>& ae)
{
    if (addressing_.size() != ae.size())
    {
        FatalErrorInFunction
            << "Addressing and list of addressed elements "
               "have different sizes: "
            << addressing_.size() << " " << ae.size()
            << abort(FatalError);
    }

    forAll(addressing_, i)
    {
        completeList_[addressing_[i]] = ae[i];
    }
}


template<class T>
inline void Foam::UIndirectList<T>::operator=(const UIndirectList<T>& ae)
{
    if (addressing_.size() != ae.size())
    {
        FatalErrorInFunction
            << "Addressing and list of addressed elements "
               "have different sizes: "
            << addressing_.size() << " " << ae.size()
            << abort(FatalError);
    }

    forAll(addressing_, i)
    {
        completeList_[addressing_[i]] = ae[i];
    }
}


template<class T>
inline void Foam::UIndirectList<T>::operator=(const T& t)
{
    forAll(addressing_, i)
    {
        completeList_[addressing_[i]] = t;
    }
}


template<class T>
inline bool Foam::UIndirectList<T>::operator==(const UIndirectList<T>& ae) const
{
    if (addressing_.size() != ae.size())
    {
        return false;
    }

    forAll(addressing_, i)
    {
        if (completeList_[addressing_[i]] != ae[i])
        {
            return false;
        }
    }

    return true;
}


template<class T>
inline bool Foam::UIndirectList<T>::operator!=(const UIndirectList<T>& ae) const
{
    return !(*this == ae);
}


// * * * * * * * * * * * * * * * * STL iterator  * * * * * * * * * * * * * * //

template<class T>
inline Foam::UIndirectList<T>::iterator::iterator
(
    UList<T>& l,
    const UList<label>::const_iterator& i
)
:
    l_(l),
    i_(i)
{}


template<class T>
inline bool Foam::UIndirectList<T>::iterator::operator==
(
    const iterator& iter
) const
{
    return i_ == iter.i_;
}


template<class T>
inline bool Foam::UIndirectList<T>::iterator::operator!=
(
    const iterator& iter
) const
{
    return i_ != iter.i_;
}


template<class T>
inline T& Foam::UIndirectList<T>::iterator::operator*() const
{
    return l_[*i_];
}


template<class T>
inline T& Foam::UIndirectList<T>::iterator::operator()() const
{
    return operator*();
}


template<class T>
inline typename Foam::UIndirectList<T>::iterator
Foam::UIndirectList<T>::iterator::operator++()
{
    ++i_;
    return *this;
}


template<class T>
inline typename Foam::UIndirectList<T>::iterator
Foam::UIndirectList<T>::iterator::operator++(const int)
{
    iterator tmp = *this;
    ++i_;
    return tmp;
}


template<class T>
inline typename Foam::UIndirectList<T>::iterator
Foam::UIndirectList<T>::iterator::operator--()
{
    --i_;
    return *this;
}


template<class T>
inline typename Foam::UIndirectList<T>::iterator
Foam::UIndirectList<T>::iterator::operator--(const int)
{
    iterator tmp = *this;
    --i_;
    return tmp;
}


template<class T>
inline typename Foam::UIndirectList<T>::iterator
Foam::UIndirectList<T>::iterator::operator+=(const label n)
{
    i_ += n;
    return *this;
}


template<class T>
inline typename Foam::UIndirectList<T>::iterator
Foam::UIndirectList<T>::iterator::operator-=(const label n)
{
    i_ -= n;
    return *this;
}


template<class T>
inline typename Foam::UIndirectList<T>::iterator
Foam::UIndirectList<T>::iterator::operator+(const label n) const
{
    typename UIndirectList<T>::iterator tmp = *this;
    return tmp += n;
}


template<class T>
inline typename Foam::UIndirectList<T>::iterator
Foam::UIndirectList<T>::iterator::operator-(const label n) const
{
    typename UIndirectList<T>::iterator tmp = *this;
    return tmp -= n;
}


template<class T>
inline Foam::label Foam::UIndirectList<T>::iterator::operator-
(
    const typename UIndirectList<T>::iterator& iter
) const
{
    return i_ - iter.i_;
}


template<class T>
inline T& Foam::UIndirectList<T>::iterator::operator[](const label n)
{
    return *(*this + n);
}


template<class T>
inline bool Foam::UIndirectList<T>::iterator::operator<
(
    const iterator& iter
) const
{
    return i_ < iter.i_;
}


template<class T>
inline bool Foam::UIndirectList<T>::iterator::operator>
(
    const iterator& iter
) const
{
    return i_ > iter.i_;
}


template<class T>
inline bool Foam::UIndirectList<T>::iterator::operator<=
(
    const iterator& iter
) const
{
    return i_ <= iter.i_;
}


template<class T>
inline bool Foam::UIndirectList<T>::iterator::operator>=
(
    const iterator& iter
) const
{
    return i_ >= iter.i_;
}


template<class T>
inline typename Foam::UIndirectList<T>::iterator
Foam::UIndirectList<T>::begin()
{
    return iterator(completeList_, addressing_.begin());
}


template<class T>
inline typename Foam::UIndirectList<T>::iterator
Foam::UIndirectList<T>::end()
{
    return iterator(completeList_, addressing_.end());
}


// * * * * * * * * * * * * * * * STL const_iterator  * * * * * * * * * * * * //

template<class T>
inline Foam::UIndirectList<T>::const_iterator::const_iterator
(
    const UList<T>& l,
    const UList<label>::const_iterator& i
)
:
    l_(l),
    i_(i)
{}


template<class T>
inline bool Foam::UIndirectList<T>::const_iterator::operator==
(
    const const_iterator& iter
) const
{
    return i_ == iter.i_;
}


template<class T>
inline bool Foam::UIndirectList<T>::const_iterator::operator!=
(
    const const_iterator& iter
) const
{
    return i_ != iter.i_;
}


template<class T>
inline const T& Foam::UIndirectList<T>::const_iterator::operator*() const
{
    return l_[*i_];
}


template<class T>
inline const T& Foam::UIndirectList<T>::const_iterator::operator()() const
{
    return operator*();
}


template<class T>
inline typename Foam::UIndirectList<T>::const_iterator
Foam::UIndirectList<T>::const_iterator::operator++()
{
    ++i_;
    return *this;
}


template<class T>
inline typename Foam::UIndirectList<T>::const_iterator
Foam::UIndirectList<T>::const_iterator::operator++(const int)
{
    const_iterator tmp = *this;
    ++i_;
    return tmp;
}


template<class T>
inline typename Foam::UIndirectList<T>::const_iterator
Foam::UIndirectList<T>::const_iterator::operator--()
{
    --i_;
    return *this;
}


template<class T>
inline typename Foam::UIndirectList<T>::const_iterator
Foam::UIndirectList<T>::const_iterator::operator--(const int)
{
    const_iterator tmp = *this;
    --i_;
    return tmp;
}


template<class T>
inline typename Foam::UIndirectList<T>::const_iterator
Foam::UIndirectList<T>::const_iterator::operator+=(const label n)
{
    i_ += n;
    return *this;
}


template<class T>
inline typename Foam::UIndirectList<T>::const_iterator
Foam::UIndirectList<T>::const_iterator::operator-=(const label n)
{
    i_ -= n;
    return *this;
}


template<class T>
inline typename Foam::UIndirectList<T>::const_iterator
Foam::UIndirectList<T>::const_iterator::operator+(const label n) const
{
    typename UIndirectList<T>::const_iterator tmp = *this;
    return tmp += n;
}


template<class T>
inline typename Foam::UIndirectList<T>::const_iterator
Foam::UIndirectList<T>::const_iterator::operator-(const label n) const
{
    typename UIndirectList<T>::const_iterator tmp = *this;
    return tmp -= n;
}


template<class T>
inline Foam::label Foam::UIndirectList<T>::const_iterator::operator-
(
    const typename UIndirectList<T>::const_iterator& iter
) const
{
    return i_ - iter.i_;
}


template<class T>
inline const T& Foam::UIndirectList<T>::const_iterator::operator[]
(
    const label n
)
{
    return *(*this + n);
}


template<class T>
inline bool Foam::UIndirectList<T>::const_iterator::operator<
(
    const const_iterator& iter
) const
{
    return i_ < iter.i_;
}


template<class T>
inline bool Foam::UIndirectList<T>::const_iterator::operator>
(
    const const_iterator& iter
) const
{
    return i_ > iter.i_;
}


template<class T>
inline bool Foam::UIndirectList<T>::const_iterator::operator<=
(
    const const_iterator& iter
) const
{
    return i_ <= iter.i_;
}


template<class T>
inline bool Foam::UIndirectList<T>::const_iterator::operator>=
(
    const const_iterator& iter
) const
{
    return i_ >= iter.i_;
}


template<class T>
inline typename Foam::UIndirectList<T>::const_iterator
Foam::UIndirectList<T>::begin() const
{
    return const_iterator(completeList_, addressing_.begin());
}


template<class T>
inline typename Foam::UIndirectList<T>::const_iterator
Foam::UIndirectList<T>::end() const
{
    return const_iterator(completeList_, addressing_.end());
}


template<class T>
inline typename Foam::UIndirectList<T>::const_iterator
Foam::UIndirectList<T>::cbegin() const
{
    return const_iterator(completeList_, addressing_.begin());
}


template<class T>
inline typename Foam::UIndirectList<T>::const_iterator
Foam::UIndirectList<T>::cend() const
{
    return const_iterator(completeList_, addressing_.end());
}


// ************************************************************************* //
