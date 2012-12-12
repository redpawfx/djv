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

//! \file djv_prefs.cpp

#include <djv_prefs.h>

#include <djv_file.h>

#include <FL/Fl_Preferences.H>

#include <stdlib.h>

namespace djv
{

//------------------------------------------------------------------------------
// Prefs
//------------------------------------------------------------------------------

Prefs::Prefs(const String & name) :
    _name(name),
    _prefs(new Fl_Preferences(
        Fl_Preferences::USER,
        file_base().c_str(),
        _name.c_str()))
{}

Prefs::Prefs(const Prefs & in, const String & group) :
    _prefs(new Fl_Preferences(in._prefs.get(), group.c_str()))
{}

Prefs::Prefs(Prefs * in, const String & group) :
    _prefs(new Fl_Preferences(in->_prefs.get(), group.c_str()))
{}

Prefs::~Prefs()
{
    _prefs->flush();
}

const String & Prefs::name() const
{
    return _name;
}

namespace
{

String pack_name(const String & in)
{
    return String_Util::replace(in, ':', '*');
}

String unpack_name(const String & in)
{
    return String_Util::replace(in, '*', ':');
}

}

void Prefs::set(const String & in, const String & value)
{
    _prefs->set(pack_name(in).c_str(), value.c_str());
}

bool Prefs::get(const String & in, String * out) const
{
    char * buf = 0;

    const bool found =
        _prefs->get(pack_name(in).c_str(), buf, "") ? true : false;

    *out = String(buf);

    if (buf)
    {
        free(buf);
    }

    return found;
}

void Prefs::del(const String & in)
{
    _prefs->deleteEntry(pack_name(in).c_str());
}

void Prefs::del_group(const String & in)
{
    _prefs->deleteGroup(pack_name(in).c_str());
}

List<String> Prefs::list() const
{
    List<String> out;

    const int size = _prefs->entries();

    for (int i = 0; i < size; ++i)
    {
        out += unpack_name(_prefs->entry(i));
    }

    return out;
}

List<String> Prefs::group() const
{
    List<String> out;

    const int size = _prefs->groups();

    for (int i = 0; i < size; ++i)
    {
        out += unpack_name(_prefs->group(i));
    }

    return out;
}

String Prefs::file_name() const
{
    //! \todo Is this still necessary?

    return file_path() + name() + ".prefs";
}

String Prefs::file_path() const
{
    //! \todo Is this still necessary?

    char tmp [cstring_size] = "";
    _prefs->getUserdataPath(tmp, cstring_size);

    return File_Util::path_fix(String(tmp) + "..");
}

String Prefs::file_base()
{
    //! \todo Hard-coded version string.

    return "djv-0.9-0";
}

} // djv

