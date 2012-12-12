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

//! \file djv_memory_buffer_inline.h

#include <djv_memory.h>

namespace djv
{

//------------------------------------------------------------------------------
// Memory_Buffer
//------------------------------------------------------------------------------

template<typename T>
inline Memory_Buffer<T>::Memory_Buffer() :
    _data(0),
    _size(0)
{}

template<typename T>
inline Memory_Buffer<T>::Memory_Buffer(const Memory_Buffer & in) :
    _data(0),
    _size(0)
{
    *this = in;
}

template<typename T>
inline Memory_Buffer<T>::Memory_Buffer(size_t in) :
    _data(0),
    _size(0)
{
    size(in);
}

template<typename T>
inline Memory_Buffer<T>::~Memory_Buffer()
{
    del();
}

template<typename T>
inline Memory_Buffer<T> & Memory_Buffer<T>::operator = (
    const Memory_Buffer<T> & in)
{
    if (&in != this)
    {
        size(in._size);
        Memory::copy(in._data, _data, _size);
    }

    return *this;
}

template<typename T>
inline void Memory_Buffer<T>::size(size_t in)
{
    if (in == _size)
    {
        return;
    }

    del();

    _size = in;
    _data = reinterpret_cast<T *>(Memory::get(_size * sizeof(T) + 1));

    //zero(); //! \todo Is this still necessary?
}

template<typename T>
inline size_t Memory_Buffer<T>::size() const
{
    return _size;
}

template<typename T>
inline T * Memory_Buffer<T>::data()
{
    return _data;
}

template<typename T>
inline const T * Memory_Buffer<T>::data() const
{
    return _data;
}

template<typename T>
inline void Memory_Buffer<T>::zero()
{
    Memory::zero(_data, _size * sizeof(T));
}

template<typename T>
inline T * Memory_Buffer<T>::operator () ()
{
    return _data;
}

template<typename T>
inline const T * Memory_Buffer<T>::operator () () const
{
    return _data;
}

template<typename T>
inline void Memory_Buffer<T>::del()
{
    if (_data)
    {
        Memory::del(_data);
        _data = 0;
        _size = 0;
    }
}

} // djv

