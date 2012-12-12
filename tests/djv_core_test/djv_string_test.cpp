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

//! \file djv_string_test.cpp

#include <djv_assert.h>
#include <djv_debug.h>
#include <djv_string.h>

using namespace djv;

void format()
{
    const struct Data
    {
        String a, b;
    } data [] =
    {
        { "%%%%%%", "abc" },
        { "%%%%", "ab" },
        { "%%%%%%%%", "abc%%" }
    };
    const int data_size = sizeof(data) / sizeof(Data);

    for (int i = 0; i < data_size; ++i)
    {
        const String tmp = String_Format(data[i].a).arg("a").arg("b").arg("c");
        DJV_ASSERT(tmp == data[i].b);
    }
}

void format2()
{
    const String tmp =
        " a b c\n"
        " d e f\n"
        "\n"
        " g h i";

    DJV_ASSERT(String_Util::split(tmp, List<char>() << ' ' << '\n', false) ==
        List<String>() << "a" << "b" << "c" << "d" << "e" << "f" << "g" <<
            "h" << "i");
}

void char_case()
{
    DJV_ASSERT("abc" == String_Util::lower("ABC"));
    DJV_ASSERT("ABC" == String_Util::upper("abc"));
}

void join()
{
    const String tmp =
        String_Util::join(List<String>() << "a" << "b" << "c", " ");
    DJV_ASSERT("a b c" == tmp);
}

void split()
{
    const struct Data
    {
        String a;
        String split;
        bool empty;
        List<String> b;
    } data [] =
    {
        { "", "_", false, List<String>() << "" },
        { "_", "_", false, List<String>() },
        { "_", "_", true, List<String>() << "" },
        { "__", "_", false, List<String>() },
        { "__", "_", true, List<String>() << "" << "" },
        { "a", "_", false, List<String>() << "a" },
        { "abc", "_", false, List<String>() << "abc" },
        { "a_b", "_", false, List<String>() << "a" << "b" },
        { "a_b_c", "_", false, List<String>() << "a" << "b" << "c" },
        { "_a_b_c", "_", false, List<String>() << "a" << "b" << "c" },
        { "a_b_c_", "_", false, List<String>() << "a" << "b" << "c" },
        { "__a__b__c", "__", false, List<String>() << "a" << "b" << "c" },
        { "a__b__c__", "__", false, List<String>() << "a" << "b" << "c" },
        { "a_bc_def", "_", false, List<String>() << "a" << "bc" << "def" },
        { "a__b__c", "_", true, List<String>() << "a" << "" << "b" << "" <<
            "c" }
    };
    const int data_size = sizeof(data) / sizeof(Data);

    for (int i = 0; i < data_size; ++i)
    {
        const List<String> tmp =
            String_Util::split(data[i].a, data[i].split, data[i].empty);
        DJV_ASSERT(data[i].b == tmp);
    }
}

void replace()
{
    const String data = "a b c";
    const String tmp = String_Util::replace(data, ' ', '_');
    DJV_ASSERT("a_b_c" == tmp);
}

void convert()
{
    const struct Int_Data
    {
        String a;
        int b;
        String c;
    } int_data [] =
    {
        { "-100", -100, "-100" },
        { "-10", -10, "-10" },
        { "-1", -1, "-1" },
        { "0", 0, "0" },
        { ".1", 0, "0" },
        { "1", 1, "1" },
        { "1.1", 1, "1" },
        { "10", 10, "10" },
        { "100", 100, "100" },
        { "1000", 1000, "1000" }
    };
    const int int_size = sizeof(int_data) / sizeof(Int_Data);

    for (int i = 0; i < int_size; ++i)
    {
        const int tmp = String_Util::string_to_int(int_data[i].a);
        DJV_ASSERT(int_data[i].b == tmp);
        DJV_ASSERT(int_data[i].c == String_Util::int_to_string(tmp));
    }

    const double float_data [] = { -1.0, 0.0, 1.0, 10.0, 100.0, 1000.0 };
    const int float_size = sizeof(float_data) / sizeof(double);

    for (int i = 0; i < float_size; ++i)
    {
        const double tmp = String_Util::string_to_float(
                               String_Util::float_to_string(float_data[i]));
        DJV_ASSERT(float_data[i] == tmp);
    }

    const String string_data [] = { "abc", "a b c" };
    const int string_size = sizeof(string_data) / sizeof(String);

    for (int i = 0; i < string_size; ++i)
    {
        String tmp = serialize(string_data[i]);
        String tmp2;
        tmp >> tmp2;
        DJV_ASSERT(string_data[i] == tmp2);
    }
}

int main(int argc, char ** argv)
{
    format();
    format2();
    char_case();
    join();
    split();
    replace();
    convert();

    return 0;
}

