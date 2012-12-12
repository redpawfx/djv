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

//! \file djv_openexr_base.cpp

#include <djv_openexr_base.h>

#include <djv_assert.h>

#include <ImfThreading.h>

namespace djv_openexr
{

//------------------------------------------------------------------------------
// djv_image_io_base
//------------------------------------------------------------------------------

extern "C" {

DJV_PLUGIN_EXPORT Plugin * djv_image_io_base()
{
    return new Base;
}

}

//------------------------------------------------------------------------------
// Base::Options
//------------------------------------------------------------------------------

Base::Options::Options() :
    threads(4)
{}

//------------------------------------------------------------------------------
// Base
//------------------------------------------------------------------------------

const List<String> & Base::label_options()
{
    static const List<String> data = List<String>() <<
        "Threads";

    DJV_ASSERT(data.size() == _OPTIONS_SIZE);

    return data;
}

Base::Base()
{
    update_threads();
}

Plugin * Base::copy() const
{
    Base * out = new Base;
    out->_options = _options;
    return out;
}

String Base::name() const
{
    return djv_openexr::name;
}

List<String> Base::extensions() const
{
    return djv_openexr::extensions;
}

bool Base::option(const String & in, String * data)
{
    const List<String> & list = options();

    try
    {
        if (String_Util::compare_no_case(in, list[THREADS]))
        {
            *data >> _options.threads;

            update_threads();
        }
    }
    catch (String)
    {
        return false;
    }

    return true;
}

String Base::option(const String & in) const
{
    const List<String> & list = options();

    String out;

    if (String_Util::compare_no_case(in, list[THREADS]))
    {
        out << _options.threads;
    }

    return out;
}

List<String> Base::options() const
{
    return label_options();
}

void Base::update_threads()
{
    //DJV_DEBUG("Base::update_threads");
    //DJV_DEBUG_PRINT("threads = " << _options.threads);

    Imf::setGlobalThreadCount(_options.threads);
}

} // djv_openexr

