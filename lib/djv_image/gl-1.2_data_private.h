/*------------------------------------------------------------------------------
 lib/djv_image/gl-1.2_data_private.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_IMAGE_GL_1_2_DATA_PRIVATE_H
#define DJV_IMAGE_GL_1_2_DATA_PRIVATE_H

#include <djv_image/gl_data.h>

namespace djv_image {
namespace gl_data {

//------------------------------------------------------------------------------
// Fixed
//------------------------------------------------------------------------------

class Fixed
{
public:
  
  void draw(const data::Data &, const Option &) throw (Error);
};

//------------------------------------------------------------------------------

}
}

#endif

