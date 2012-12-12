/*------------------------------------------------------------------------------
 lib/djv_image/io_cineon_color.cxx
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "io_cineon.h"

namespace djv_image {
namespace io_cineon {

//------------------------------------------------------------------------------
// film_print()
//------------------------------------------------------------------------------

// XXX Preserve black and white points?

void film_print(pixel::F32_T * in, int size, const Film_Print & value)
{
  DJV_DEBUG("film_print");
  DJV_DEBUG_PRINT("size = " << size);
  DJV_DEBUG_PRINT("black = " << value.black);
  DJV_DEBUG_PRINT("white = " << value.white);
  DJV_DEBUG_PRINT("gamma = " << value.gamma);
  
  const double gain =
    1.0 / (
      1.0 - math::pow(
        math::pow(10.0, (value.black - value.white) * 0.002 / 0.6),
        value.gamma / 1.7
      )
    );
  
  const double offset = gain - 1.0;
  
  DJV_DEBUG_PRINT("gain = " << gain * 255);
  DJV_DEBUG_PRINT("offset = " << offset * 255);
  
  for (int i = 0; i < size; ++i)
    in[i] = i / pixel::F32_T(size - 1);
  
  for (int i = 0; i < size; ++i)
  {
    in[i] = pixel::F32_T(
      value.white / 1023.0 +
      math::log_10(math::pow((in[i] + offset) / gain, 1.7 / value.gamma)) /
      (2.048 / 0.6)
    );

    //DJV_DEBUG_PRINT("lut[" << i << "] = " <<
    //  in[i] << " " << static_cast<int>(in[i] * 1024));
  }
}

//------------------------------------------------------------------------------
// film_print_lut()
//------------------------------------------------------------------------------

Data film_print_lut(const Film_Print & in)
{
  Data out(data::Info(V2i(1024, 1), pixel::L_F32));
  
  film_print(reinterpret_cast<pixel::F32_T *>(out.data()), out.w(), in);
  
  return out;
}

//------------------------------------------------------------------------------
// film_unprint()
//------------------------------------------------------------------------------

void film_unprint(pixel::F32_T * in, int size, const Film_Unprint & value)
{
  DJV_DEBUG("film_unprint");
  DJV_DEBUG_PRINT("black = " << value.black);
  DJV_DEBUG_PRINT("white = " << value.white);
  DJV_DEBUG_PRINT("gamma = " << value.gamma);
  DJV_DEBUG_PRINT("soft clip = " << value.soft_clip);
  
  const double gain =
    1.0 / (
      1.0 - math::pow(
        math::pow(10.0, (value.black - value.white) * 0.002 / 0.6),
        value.gamma / 1.7
      )
    );
  
  const double offset = gain - 1.0;
  
  DJV_DEBUG_PRINT("gain = " << gain * 255);
  DJV_DEBUG_PRINT("offset = " << offset * 255);

  const int break_point = value.white - value.soft_clip;
  
  const double knee_offset =
    math::pow(
      math::pow(10.0, (break_point - value.white) * 0.002 / 0.6),
      value.gamma / 1.7
    ) *
    gain - offset;
  
  const double knee_gain =
    (
      (255 - (knee_offset * 255)) /
      math::pow(5.0 * value.soft_clip, value.soft_clip / 100.0)
    ) / 255.0;
  
  DJV_DEBUG_PRINT("break point = " << break_point);
  DJV_DEBUG_PRINT("knee offset = " << knee_offset * 255);
  DJV_DEBUG_PRINT("knee gain = " << knee_gain * 255);
  
  for (int i = 0; i < size; ++i)
    in[i] = i / pixel::F32_T(size - 1);
  
  for (int i = 0; i < size; ++i)
  {
    const int tmp = static_cast<int>(in[i] * 1023.0);
    
    if (tmp < value.black)
    {
      in[i] = 0.0;
    }
    else if (tmp > break_point)
    {
      in[i] = pixel::F32_T(
        (
          math::pow(
            static_cast<double>(tmp - break_point),
            value.soft_clip / 100.0
          ) *
          knee_gain * 255 + knee_offset * 255
        ) / 255.0
      );
    }
    else
    {
      in[i] = pixel::F32_T(
        math::pow(
          math::pow(10.0, (tmp - value.white) * 0.002 / 0.6),
          value.gamma / 1.7
        ) *
        gain - offset
      );
    }

    //DJV_DEBUG_PRINT("lut[" << i << "] = " <<
    //  in[i] << " " << static_cast<int>(in[i] * 255.0));
  }
}

//------------------------------------------------------------------------------
// film_unprint_lut()
//------------------------------------------------------------------------------

Data film_unprint_lut(const Film_Unprint & in)
{
  Data out(data::Info(V2i(1024, 1), pixel::L_F32));
  
  film_unprint(reinterpret_cast<pixel::F32_T *>(out.data()), out.w(), in);
  
  return out;
}

//------------------------------------------------------------------------------

}
}
