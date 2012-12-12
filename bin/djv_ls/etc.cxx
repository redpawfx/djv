/*------------------------------------------------------------------------------
 bin/djv_ls/etc.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "etc.h"

namespace djv_bin {
namespace ls {

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

const String
#if defined(DJV_WINDOWS)
  label_info_format = "%% %% %% %%",
#else
  label_info_format = "%% %% %% %% %%",
#endif
  label_directory = "%%:";

const String
  error_open = "Cannot open: %%";

//------------------------------------------------------------------------------

}
}
