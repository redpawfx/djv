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

//! \file djv_list_inline.h

namespace djv
{

//------------------------------------------------------------------------------
// List
//------------------------------------------------------------------------------

template<typename T>
inline List<T>::List()
{}

template<typename T>
inline List<T>::List(const List<T> & in) :
    std::vector<T>(in)
{}

template<typename T>
inline List<T>::List(const List<T> & in, size_t position, size_t size) :
    std::vector<T>(size)
{
    const size_t in_size = in.size();

    for (size_t i = 0, j = position; i < size && j < in_size; ++i, ++j)
    {
        (*this)[i] = in[j];
    }
}

template<typename T>
inline List<T>::List(const T & in, size_t size) :
    std::vector<T>(size, in)
{}

template<typename T>
inline List<T>::List(size_t size) :
    std::vector<T>(size)
{}

template<typename T>
inline void List<T>::push_front(const T & in)
{
    std::vector<T>::insert(List<T>::begin(), in);
}

template<typename T>
inline T List<T>::pop_front()
{
    const T out = List<T>::front();

    std::vector<T>::erase(List<T>::begin());

    return out;
}

template<typename T>
inline T List<T>::pop_back()
{
    const T out = List<T>::back();

    std::vector<T>::pop_back();

    return out;
}

template<typename T>
inline void List<T>::add(const T & in)
{
    std::vector<T>::push_back(in);
}

template<typename T>
inline void List<T>::add(const List<T> & in)
{
    for (
        typename List<T>::const_iterator i = in.begin(), end = in.end();
        i != end;
        ++i)
    {
        std::vector<T>::push_back(*i);
    }
}

template<typename T>
inline List<T> & List<T>::operator += (const T & in)
{
    add(in);

    return *this;
}

template<typename T>
inline List<T> & List<T>::operator += (const List<T> & in)
{
    add(in);

    return *this;
}

template<typename T>
inline List<T> & List<T>::operator << (const T & in)
{
    add(in);

    return *this;
}

template<typename T>
inline List<T> & List<T>::operator << (const List<T> & in)
{
    add(in);

    return *this;
}

//------------------------------------------------------------------------------
// List_Util
//------------------------------------------------------------------------------

template<typename T>
inline int List_Util::find(const T & in, const List<T> & list)
{
    int out = -1;

    const size_t size = list.size();

    for (size_t i = 0; i < size; ++i)
    {
        if (in == list[i])
        {
            out = static_cast<int>(i);
            break;
        }
    }

    return out;
}

template<typename T>
inline List<T> List_Util::unique(const List<T> & in)
{
    List<T> out;

    for (
        typename List<T>::const_iterator i = in.begin(), end = in.end();
        i != end;
        ++i)
    {
        if (-1 == find(*i, out))
        {
            out.add(*i);
        }
    }

    return out;
}

template<typename A, typename B>
inline void List_Util::convert(const List<A> & in, List<B> & out)
{
    const size_t size = in.size();

    out.resize(size);

    for (size_t i = 0; i < size; ++i)
    {
        out[i] = static_cast<B>(in[i]);
    }
}

template<typename A, typename B>
inline void List_Util::convert_append(const List<A> & in, List<B> & out)
{
    const size_t in_size = in.size();
    const size_t out_size = out.size();

    out.resize(in_size + out_size);

    for (size_t i = 0; i < in_size; ++i)
    {
        out[out_size + i] = static_cast<B>(in[i]);
    }
}

} // djv

