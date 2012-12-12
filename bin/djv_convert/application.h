/*------------------------------------------------------------------------------
 bin/djv_convert/application.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_CONVERT_APPLICATION_H
#define DJV_CONVERT_APPLICATION_H

#include <djv_image/application.h>
#include <djv_image/gl_data.h>
#include <djv_image/gl_offscreen.h>
#include <djv_image/image.h>
#include <djv_image/time.h>
#include <djv_base/file.h>

namespace djv_bin {

//------------------------------------------------------------------------------
//!\namespace djv_bin::convert
//
//! Utility for converting and processing images.
//------------------------------------------------------------------------------

namespace convert {
using namespace djv_base;

//------------------------------------------------------------------------------
//!\name Options
//------------------------------------------------------------------------------
//@{

//! Image options.

struct Image
{
  Image() :
    scale(1.0),
    channel(djv_image::gl_data::CHANNEL(0))
  {}
  
  V2b mirror;
  V2f scale;
  djv_image::gl_data::CHANNEL channel;
  V2i size;
};

//! Input options.

struct Input
{
  Input() :
    layer(0),
    proxy(djv_image::data::PROXY(0)),
    slate_frames(0),
    timeout(0)
  {}

  File file;
  int layer;
  djv_image::data::PROXY proxy;
  String start, end;
  File slate;
  int slate_frames;
  int timeout;
};

//! Output options.

struct Output
{
  Output() :
    tag_auto(true)
  {}

  File file;
  std::auto_ptr<djv_image::PIXEL> pixel;
  djv_image::Tag tag;
  bool tag_auto;
  std::auto_ptr<djv_image::time::FPS> speed;
};

//@}
//------------------------------------------------------------------------------
//!\class Application
//
//! The djv_convert application.
//------------------------------------------------------------------------------

class Application : public djv_image::application::Application_Base
{
public:

  Application(int, char **);

private:

  bool work();
  
  void command_line(String &) throw (Error);
  
  String command_line_help() const;

  Image _image;
  Input _input;
  Output _output;
  
  std::auto_ptr<djv_image::gl_offscreen::Buffer> _offscreen_buffer;
  djv_image::gl_data::Data _data;
};

//------------------------------------------------------------------------------

}
}

#endif

