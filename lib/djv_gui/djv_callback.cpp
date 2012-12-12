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

//! \file djv_callback.cpp

#include <djv_callback.h>

#include <algorithm>

namespace djv
{

//------------------------------------------------------------------------------
// Callback
//------------------------------------------------------------------------------

Callback::Callback() :
    _callbacks(true)
{}

Callback::~Callback()
{
    del();
}

void Callback::del()
{
    for (size_t i = 0; i < _signals.size(); ++i)
    {
        _signals[i]->del();
    }

    _signals.clear();

    for (size_t i = 0; i < _connections.size(); ++i)
    {
        _connections[i]->release(this);
    }

    _connections.clear();
}

void Callback::signal_add(Abstract_Signal * in)
{
    _signals += in;
}

void Callback::signal_del(Abstract_Signal * in)
{
    const List<Abstract_Signal *>::iterator i = std::find(
        _signals.begin(),
        _signals.end(),
        in);

    if (i != _signals.end())
    {
        _signals.erase(i);
    }
}

size_t Callback::signal_size() const
{
    return _signals.size();
}

void Callback::connect_add(Abstract_Signal * in)
{
    _connections += in;
}

void Callback::connect_del(Abstract_Signal * in)
{
    const List<Abstract_Signal *>::iterator i = std::find(
        _connections.begin(),
        _connections.end(),
        in);

    if (i != _connections.end())
    {
        _connections.erase(i);
    }
}

size_t Callback::connect_size() const
{
    return _connections.size();
}

void Callback::callbacks(bool in)
{
    _callbacks = in;
}

bool Callback::callbacks() const
{
    return _callbacks;
}

//------------------------------------------------------------------------------
// Abstract_Signal
//------------------------------------------------------------------------------

Abstract_Signal::~Abstract_Signal()
{}

} // djv

