/*------------------------------------------------------------------------------
 lib/djv_base/vector_fnc.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "vector.h"

namespace djv_base {
namespace vector {

//------------------------------------------------------------------------------
// sperical_to_cartesian(), cartesian_to_spherical()
//------------------------------------------------------------------------------

// XXX Left vs. right handed coordinates?

void spherical_to_cartesian(const V3f & in, V3f * out)
{
  const double tmp = math::sin(in.x);
  out->x = in.z * tmp * math::cos(in.y);
  out->y = in.z * tmp * math::sin(in.y);
  out->z = in.z * math::cos(in.x);
}

void cartesian_to_spherical(const V3f & in, V3f * out)
{
  out->z = math::sqrt(in.x * in.x + in.y * in.y + in.z * in.z);
  out->x = out->z != 0.0 ? math::arc_cos(in.z / out->z) : 0.0;
  out->y = in.x != 0.0 ? math::arc_tan(in.y / in.x) : 0.0;
}

//------------------------------------------------------------------------------

}
}

