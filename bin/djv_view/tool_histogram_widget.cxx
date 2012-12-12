/*------------------------------------------------------------------------------
 bin/djv_view/tool_histogram_widget.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "tool_histogram.h"

#include <djv_ui/style.h>
#include <FL/fl_draw.H>

namespace djv_bin {
namespace view {
namespace tool_histogram {

//------------------------------------------------------------------------------
// Widget
//------------------------------------------------------------------------------

Widget::Widget() :
  _value(0),
  _value_max(0)
{
  style(djv_ui::frame::STYLE_BORDER);
  color(FL_BLACK);
}
  
void Widget::set(
  const djv_image::Data * in,
  const djv_image::Color & min,
  const djv_image::Color & max
) {
  _value = in;
  
  if (_value)
  {
    _value_max = 0;
    
    // XXX
    
    const djv_image::pixel::U16_T * p =
      reinterpret_cast<const djv_image::pixel::U16_T *>(_value->data());

    const int w = in->w();
    const int channels = in->channels();
    for (int i = 0; i < w; ++i, p += channels)
      for (int c = 0; c < channels; ++c)
        _value_max = math::max(static_cast<int>(p[c]), _value_max);
  }
  
  _min = min;
  _max = max;
  
  redraw();
}

void Widget::draw()
{
  const Box2i geom = frame_geom();
  
  fl_color(FL_BACKGROUND2_COLOR);
  fl_rectf(geom.x, geom.y, geom.w, geom.h);
  
  if (_value)
  {
    fl_push_clip(geom.x, geom.y, geom.w, geom.h);
    
    //fl_color(fl_darker(FL_BACKGROUND2_COLOR));
    //for (int x = geom.x; x < geom.x + geom.w -1; x += 10)
    //  fl_line(geom.x + x, geom.y, geom.x + x, geom.y + geom.h - 1);
    
    // XXX
    
    const djv_image::pixel::U16_T * p =
      reinterpret_cast<const djv_image::pixel::U16_T *>(_value->data());

    const uchar color_list [][3] =
    {
      { 255, 0,   0 },
      { 0,   255, 0 },
      { 0,   0,   255 },
      { 255, 255, 255 }
    };

    const int w = _value->w();
    const int channels = djv_image::pixel::channels(_value->pixel());
    for (int i = 0; i < w; ++i, p += channels)
      for (int c = channels - 1; c >= 0; --c)
      {
        fl_color(color_list[c][0], color_list[c][1], color_list[c][2]);
        fl_line(
          geom.x + i,
          geom.y + geom.h - 1,
          geom.x + i,
          geom.y + geom.h - 1 - static_cast<int>(
            p[c] / static_cast<double>(_value_max) * (geom.h - 1))
        );
      }
    
    fl_pop_clip();
  }

  Frame::draw();
}

void Widget::dirty()
{
  Frame::dirty();
  
  size_hint(V2i(_value ? _value->w() : 256, 200) + frame_size() * 2);
}

//------------------------------------------------------------------------------

}
}
}

