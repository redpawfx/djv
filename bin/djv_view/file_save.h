/*------------------------------------------------------------------------------
 bin/djv_view/file_save.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_VIEW_FILE_SAVE_H
#define DJV_VIEW_FILE_SAVE_H

#include "base.h"
#include <djv_ui/callback.h>
#include <djv_image/gl_data.h>
#include <djv_image/io.h>
#include <djv_base/file.h>

namespace djv_bin {
namespace view {
namespace file {

//------------------------------------------------------------------------------
//!\class Save_Info
//
//! File save information.
//------------------------------------------------------------------------------

struct Save_Info
{
  Save_Info(
    const File & input_file = File(),
    const File & output_file = File(),
    const djv_image::Info & = djv_image::Info(),
    const djv_image::Time & = djv_image::Time(),
    int layer = 0,
    const djv_image::data::PROXY proxy = djv_image::data::PROXY_NONE,
    const djv_image::gl_data::Option & = djv_image::gl_data::Option()
  );
  
  File input_file;
  File output_file;
  djv_image::Info info;
  djv_image::Time time;
  int layer;
  djv_image::data::PROXY proxy;
  djv_image::io::Frame_Info frame_info;
  djv_image::gl_data::Option option;
};

//------------------------------------------------------------------------------
//!\class Save
//
//! File save.
//------------------------------------------------------------------------------

class Save : public djv_ui::Callback_Base
{
public:

  ~Save();
  
  void set(const Save_Info &);

private:

  struct _Save;
  
  void del(_Save *);
  
  List<_Save *> _list;
};

//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

extern const String
  dialog_save;

//@}
//------------------------------------------------------------------------------
//!\name Global
//------------------------------------------------------------------------------
//@{

//! Access to global file save.

Save * save();

//@}
//------------------------------------------------------------------------------

}
}
}

#endif

