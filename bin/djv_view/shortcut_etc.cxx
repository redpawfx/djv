/*------------------------------------------------------------------------------
 bin/djv_view/shortcut_etc.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "shortcut.h"

namespace djv_bin {
namespace view {
namespace shortcut {

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

const List<String> & label_shortcut()
{
  static const List<String> data = List<String>() <<
    "Exit" <<
    
    "File open" <<
    "File reload" <<
    "File save" <<
    "File save frame" <<
    "File close" <<
    "File layer default" <<
    "File layer 1" <<
    "File layer 2" <<
    "File layer 3" <<
    "File layer 4" <<
    "File layer 5" <<
    "File layer 6" <<
    "File layer 7" <<
    "File layer 8" <<
    "File layer 9" <<
    "File layer 10" <<
    "File layer previous" <<
    "File layer next" <<
    "File proxy none" <<
    "File proxy 1/2" <<
    "File proxy 1/4" <<
    "File proxy 1/8" <<
    
    "Window new" <<
    "Window copy" <<
    "Window close" <<
    "Window fit" <<
    "Window fullscreen" <<
    "Window toolbar" <<
    "Window playbar" <<
    "Window infobar" <<
    
    "View left" <<
    "View right" <<
    "View up" <<
    "View down" <<
    "View center" <<
    "View zoom in" <<
    "View zoom out" <<
    "View zoom reset" <<
    "View reset" <<
    "View fit" <<
    "View HUD" <<
    
    "Image frame store" <<
    "Image frame store load" <<
    "Image mirror horizontal" <<
    "Image mirror vertical" <<
    "Image scale none" <<
    "Image scale 16:9" <<
    "Image scale 1.85" <<
    "Image scale 2.35" <<
    "Image rotate 0" <<
    "Image rotate 90" <<
    "Image rotate 180" <<
    "Image rotate 270" <<
    "Image color profile" <<
    "Image channel red" <<
    "Image channel green" <<
    "Image channel blue" <<
    "Image channel alpha" <<
    "Image display profile" <<
    "Image display profile reset" <<
    "Image display profile 1" <<
    "Image display profile 2" <<
    "Image display profile 3" <<
    "Image display profile 4" <<
    "Image display profile 5" <<
    "Image display profile 6" <<
    "Image display profile 7" <<
    "Image display profile 8" <<
    "Image display profile 9" <<
    "Image display profile 10" <<
    
    "Playback stop" <<
    "Playback forward" <<
    "Playback reverse" <<
    "Playback loop" <<
    "Playback start" <<
    "Playback start absolute" <<
    "Playback end" <<
    "Playback end absolute" <<
    "Playback previous" <<
    "Playback previous X10" <<
    "Playback previous X100" <<
    "Playback next" <<
    "Playback next X10" <<
    "Playback next X100" <<
    "Playback in/out points" <<
    "Playback in point mark" <<
    "Playback in point reset" <<
    "Playback out point mark" <<
    "Playback out point reset" <<
    
    "Tool magnify" <<
    "Tool color picker" <<
    "Tool histogram" <<
    "Tool information";
  
  DJV_ASSERT(data.size() == _SHORTCUT_SIZE);
  
  return data;
}

extern const String
  prefs_title = "Application";

//------------------------------------------------------------------------------

}
}
}

