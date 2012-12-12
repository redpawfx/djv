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

//! \file djv_callback_inline.h

namespace djv
{

//------------------------------------------------------------------------------
// Signal
//------------------------------------------------------------------------------

template <class T>
inline Signal<T>::Signal(Callback * parent) :
    _parent(parent)
{
    _parent->signal_add(this);
}

template <class T>
inline Signal<T>::~Signal()
{
    del();

    _parent->signal_del(this);
}

template <class T>
inline void Signal<T>::set(Callback * in, Callback_Fnc * callback)
{
    in->connect_add(this);

    _callbacks.push_back(Pair(in, (void *)callback));
}

template <class T>
inline void Signal<T>::del(Callback * in)
{
    List<Pair>::iterator poo;

    for (
        List<Pair>::iterator i = _callbacks.begin();
        i != _callbacks.end();
        ++i)
    {
        if (in == i->first)
        {
            in->connect_del(this);

            _callbacks.erase(i);

            break;
        }
    }
}

template <class T>
inline void Signal<T>::del()
{
    for (
        List<Pair>::iterator i = _callbacks.begin();
        i != _callbacks.end();
        ++i)
    {
        i->first->connect_del(this);
    }

    _callbacks.clear();
}

template <class T>
inline void Signal<T>::emit(T in)
{
    //! \todo Is this still necessary?

    for (size_t i = 0; i < _callbacks.size(); ++i)
    {
        ((Callback_Fnc *)_callbacks[i].second)(_callbacks[i].first, in);
    }
}

template <class T>
inline void Signal<T>::release(Callback * in)
{
    for (
        List<Pair>::iterator i = _callbacks.begin();
        i != _callbacks.end();
        ++i)
    {
        if (in == i->first)
        {
            _callbacks.erase(i);

            break;
        }
    }
}

template <class T>
inline size_t Signal<T>::size() const
{
    return _callbacks.size();
}

} // djv
