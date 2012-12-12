//------------------------------------------------------------------------------
// Copyright (c) 2004-2012 Darby Johnston
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions, and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions, and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the names of the copyright holders nor the names of any
//   contributors may be used to endorse or promote products derived from this
//   software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------

//! \file djv_set_inline.h

namespace djv
{

//------------------------------------------------------------------------------
// Set
//------------------------------------------------------------------------------

template<typename T>
inline Set<T>::Set()
{}

template<typename T>
inline Set<T>::Set(const Set<T> & in) :
    std::set<T>(in)
{}

template<typename T>
inline void Set<T>::add(const T & in)
{
    Set<T>::insert(in);
}

template<typename T>
inline void Set<T>::add(const Set<T> & in)
{
    for (
        typename Set<T>::const_iterator i = in.begin(), end = in.end();
        i != end;
        ++i)
    {
        Set<T>::insert(*i);
    }
}

template<typename T>
inline void Set<T>::add(const List<T> & in)
{
    for (
        typename List<T>::const_iterator i = in.begin(), end = in.end();
        i != end;
        ++i)
    {
        Set<T>::insert(*i);
    }
}

template<typename T>
inline Set<T> & Set<T>::operator += (const T & in)
{
    add(in);

    return *this;
}

template<typename T>
inline Set<T> & Set<T>::operator += (const Set<T> & in)
{
    add(in);

    return *this;
}

template<typename T>
inline Set<T> & Set<T>::operator += (const List<T> & in)
{
    add(in);

    return *this;
}

template<typename T>
inline Set<T> & Set<T>::operator << (const T & in)
{
    add(in);

    return *this;
}

template<typename T>
inline Set<T> & Set<T>::operator << (const Set<T> & in)
{
    add(in);

    return *this;
}

template<typename T>
inline Set<T>::operator List<T> () const
{
    List<T> out;

    for (
        typename Set<T>::const_iterator i = Set<T>::begin(),
            end = Set<T>::end();
        i != end;
        ++i)
    {
        out.add(*i);
    }

    return out;
}

} // djv

