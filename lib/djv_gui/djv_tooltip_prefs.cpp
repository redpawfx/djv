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

//! \file djv_tooltip_prefs.cpp

#include <djv_tooltip_prefs.h>

#include <djv_prefs.h>

#include <FL/Fl_Tooltip.H>

namespace djv
{

//------------------------------------------------------------------------------
// Tooltip_Prefs
//------------------------------------------------------------------------------

Tooltip_Prefs::Tooltip_Prefs() :
    _tooltips(true)
{
    Prefs prefs(Prefs::prefs_global(), "base");
    Prefs::get_(&prefs, "tooltips", &_tooltips);

    tooltips_update();
}

Tooltip_Prefs::~Tooltip_Prefs()
{
    Prefs prefs(Prefs::prefs_global(), "base");
    Prefs::set_(&prefs, "tooltips", _tooltips);
}

void Tooltip_Prefs::tooltips(bool in)
{
    if (in == _tooltips)
    {
        return;
    }

    _tooltips = in;

    tooltips_update();
}

bool Tooltip_Prefs::tooltips() const
{
    return _tooltips;
}

Tooltip_Prefs * Tooltip_Prefs::global()
{
    static Tooltip_Prefs * prefs = 0;

    if (! prefs)
    {
        prefs = new Tooltip_Prefs;
    }

    return prefs;
}

void Tooltip_Prefs::tooltips_update()
{
    if (_tooltips)
    {
        Fl_Tooltip::enable();
    }
    else
    {
        Fl_Tooltip::disable();
    }
}

} // djv
