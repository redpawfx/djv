/*------------------------------------------------------------------------------
 plugin/djv_tiff/data.cxx
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "base.h"

namespace djv_plugin {
namespace tiff {

//------------------------------------------------------------------------------
// palette_load()
//------------------------------------------------------------------------------

void palette_load(
  uint8_t * in,
  int size,
  int bytes,
  uint16_t * red,
  uint16_t * green,
  uint16_t * blue
) {
  switch (bytes)
  {
    case 1:
    {
      const uint8_t * in_p = in + size - 1;
      uint8_t * out_p = in + (size - 1) * 3;
      for (int x = 0; x < size; ++x, out_p -= 3)
      {
        const uint8_t index = *in_p--;
        out_p[0] = static_cast<uint8_t>(red[index]);
        out_p[1] = static_cast<uint8_t>(green[index]);
        out_p[2] = static_cast<uint8_t>(blue[index]);
      }
    }
    break;
    
    case 2:
    {
      const uint16_t * in_p = reinterpret_cast<const uint16_t *>(in) + size - 1;
      uint16_t * out_p = reinterpret_cast<uint16_t *>(in) + (size - 1) * 3;
      for (int x = 0; x < size; ++x, out_p -= 3)
      {
        const uint16_t index = *in_p--;
        out_p[0] = red[index];
        out_p[1] = green[index];
        out_p[2] = blue[index];
      }
    }
    break;
  }
}

//------------------------------------------------------------------------------

}
}

