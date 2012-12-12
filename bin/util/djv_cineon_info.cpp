/*------------------------------------------------------------------------------
 bin/util/djv_cineon_info.cxx

 Copyright (c) 2004-2012 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include <djv_image/io_cineon.h>
#include <djv_base/file_io.h>
#include <djv_base/system.h>

using namespace djv_base;
using namespace djv_image;

//------------------------------------------------------------------------------
// main()
//------------------------------------------------------------------------------

int main(int argc, char ** argv)
{
    int r = 0;

    try
    {

        for (int i = 1; i < argc; ++i)
        {
            File_Io io;
            io.open(argv[i], File_Io::READ);

            io::Info info;
            io_cineon::Header header;
            bool film_print = false;
            header.load(io, info, film_print);

            System::print(header.debug());
        }

        r = 1;
    }
    catch (Error error)
    {
        Error_Util::print(error);
    }

    return r;
}

//------------------------------------------------------------------------------
