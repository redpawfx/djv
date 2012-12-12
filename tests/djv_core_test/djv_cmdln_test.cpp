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

//! \file djv_cmdln_test.cpp

#include <djv_debug.h>
#include <djv_error.h>
#include <djv_string.h>

using namespace djv;

void parse_cmdln(String & in) throw (Error)
{
    DJV_DEBUG("parse_cmdln");
    DJV_DEBUG_PRINT("in = " << in);

    List<String> args;
    String arg;

    try
    {
        while (! in.empty())
        {
            in >> arg;

            if ("-bool" == arg)
            {
                bool value = false;
                in >> value;
                DJV_DEBUG_PRINT("bool = " << value);
            }

            else
            {
                args += arg;
            }
        }
    }
    catch (String)
    {
        throw Error(arg);
    }

    if (! args.size())
    {
        throw Error("input 1");
    }

    DJV_DEBUG_PRINT("input 1 = " << args.pop_front());

    if (! args.size())
    {
        throw Error("input 2");
    }

    DJV_DEBUG_PRINT("input 2 = " << args.pop_front());

    if (! args.empty())
    {
        throw Error(args[0]);
    }
}

int main(int argc, char ** argv)
{
    List<String> cmdln;

    for (int i = 1; i < argc; ++i)
    {
        cmdln += serialize(String(argv[i]));
    }

    String cmdln_arg = String_Util::join(cmdln, ' ');

    try
    {
        parse_cmdln(cmdln_arg);
    }
    catch (Error in)
    {
        Error_Util::print(in);
        return 1;
    }

    return 0;
}

