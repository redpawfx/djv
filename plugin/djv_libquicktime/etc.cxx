/*------------------------------------------------------------------------------
 plugin/djv_libquicktime/etc.cxx
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "base.h"

#include "load.h"
#include "save.h"
#include <djv_base/application.h>
#include <djv_base/system.h>

namespace djv_plugin {
namespace libquicktime {

//------------------------------------------------------------------------------
// quicktime_init()
//------------------------------------------------------------------------------

namespace {

extern "C" {

void _log_callback(lqt_log_level_t, const char *, const char *, void *)
{}

}

}

void quicktime_init()
{
  DJV_DEBUG("quicktime_init");

#if defined(DJV_THIRD_PARTY_LIBQUICKTIME)

  const String plugin_path =
    application::global()->path() + "../lib/libquicktime";
  
  DJV_DEBUG_PRINT("plugin path = " << plugin_path);
  
  djv_base::system::env("LIBQUICKTIME_PLUGIN_DIR", plugin_path);

#endif // DJV_THIRD_PARTY_LIBQUICKTIME
  
  lqt_set_log_callback(_log_callback, 0);
  
  DJV_DEBUG_PRINT("codec = " << label_codec());
}

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

List<String> label_codec()
{
  List<String> out;
  lqt_codec_info_t ** info = lqt_query_registry(0, 1, 1, 0);
  if (info)
  {
    for (int i = 0; info[i]; ++i)
      out += info[i]->name;
    lqt_destroy_codec_info(info);
  }
  return out;
}

List<String> label_codec_text()
{
  List<String> out;
  lqt_codec_info_t ** info = lqt_query_registry(0, 1, 1, 0);
  if (info)
  {
    for (int i = 0; info[i]; ++i)
      out += info[i]->long_name;
    lqt_destroy_codec_info(info);
  }
  return out;
}

const List<String> & label_load_option()
{
  static const List<String> data = List<String>() <<
    "Start frame";
  
  DJV_ASSERT(data.size() == _LOAD_OPTION_SIZE);
  
  return data;
}

const List<String> & label_save_option()
{
  static const List<String> data = List<String>() <<
    "Codec";
  
  DJV_ASSERT(data.size() == _SAVE_OPTION_SIZE);
  
  return data;
}

extern const String
  error_video = "No video tracks in: %%",
  error_codec = "Cannot set codec for: %%",
  error_colormodel = "Cannot set color model for: %%";

extern const String
  label_start_frame_group = "Start Frame";

extern const String
  label_codec_group = "Codec";

//------------------------------------------------------------------------------

}
}

