/*------------------------------------------------------------------------------
 lib/djv_image/data_gradient.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "image.h"

namespace djv_image {
namespace data {

//------------------------------------------------------------------------------
// gradient()
//------------------------------------------------------------------------------

void gradient(data::Data * out)
{
  DJV_ASSERT(out);

  DJV_DEBUG("gradient");
  
  const Info info(out->size(), pixel::L_F32);
  out->set(info);
  DJV_DEBUG_PRINT("out = " << *out);
  
  for (int y = 0; y < info.size.y; ++y)
  {
    pixel::F32_T * p = reinterpret_cast<pixel::F32_T *>(out->data(0, y));
    
    for (int x = 0; x < info.size.x; ++x, ++p)
      *p = static_cast<pixel::F32_T>(
        x / static_cast<double>(info.size.x - 1)
      );
  }
}

//------------------------------------------------------------------------------

}
}

