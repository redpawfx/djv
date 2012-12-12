/*------------------------------------------------------------------------------
 lib/djv_ui/file_browser_item.cxx
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "file_browser_private.h"

#include <djv_ui/application.h>
#include <djv_ui/image.h>
#include <djv_image/image.h>

namespace djv_ui {
namespace file_browser {

//------------------------------------------------------------------------------
// Item
//------------------------------------------------------------------------------

Item::Item() :
  _prev(0),
  _next(0),
  _odd(false),
  _image(0),
  _image_is_valid(false),
  _image_init(false)
{}

Item::Item(const File & in, Item * prev, Item * next, bool odd) :
  _value(in),
  _prev(prev),
  _next(next),
  _odd(odd),
  _image(0),
  _image_is_valid(djv_image::io::global()->is_valid(_value)),
  _image_init(false)
{}

Item::~Item()
{
  delete _image;
}

namespace {

V2i thumbnail(const V2i & in, int size, IMAGE image,
  djv_image::data::PROXY * proxy)
{
  using namespace djv_image;
  
  const int image_size = math::max(in.x, in.y);
  if (image_size <= 0) return V2i();

  int _proxy = 0;
  double proxy_scale =
    static_cast<double>(data::proxy_scale(data::PROXY(_proxy)));
  if (IMAGE_LOW == image)
    while (
      (image_size / proxy_scale) > size * 2 &&
      _proxy < data::_PROXY_SIZE
    )
      proxy_scale =
        static_cast<double>(data::proxy_scale(data::PROXY(++_proxy)));

  if (proxy)
    *proxy = data::PROXY(_proxy);
  
  const double scale = size / static_cast<double>(image_size / proxy_scale);
  return vector::ceil<double, int>(V2f(in) / proxy_scale * scale);
}

}

Fl_Image * Item::image(int size, IMAGE image) const
{
  if (_image_init)
    return _image;
  
  DJV_DEBUG("Item::image");
  DJV_DEBUG_PRINT("size = " << size);
  DJV_DEBUG_PRINT("image = " << image);
  DJV_DEBUG_PRINT("value = " << _value);

  try
  {
    Item * that = const_cast<Item *>(this);
    that->_image_init = true;

    if (_image_is_valid)
    {
      base::Cursor cursor(FL_CURSOR_WAIT);

      std::auto_ptr<djv_image::io::Load> load(
        djv_image::io::global()->load(_value, &that->_image_info)
      );
      
      DJV_DEBUG_PRINT("info = " << _image_info);

      djv_image::data::PROXY proxy(djv_image::data::PROXY(0));
      const V2i _size = thumbnail(_image_info.size, size, image, &proxy);
      
      DJV_DEBUG_PRINT("size = " << _size);
      DJV_DEBUG_PRINT("proxy = " << proxy);

      const djv_image::Image * image = load->get(
        djv_image::io::Frame_Info(-1, 0, proxy)
      );
      
      DJV_DEBUG_PRINT("image = " << *image);

      that->_image = image::image_to_fltk(*image, _size, image->color_profile);
    }
  }
  catch (Error in)
  {
    application::global()->error(in);
  }
  
  return _image;
}

//------------------------------------------------------------------------------

}
}
