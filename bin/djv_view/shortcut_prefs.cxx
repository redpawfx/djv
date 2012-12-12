/*------------------------------------------------------------------------------
 bin/djv_view/shortcut_prefs.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "shortcut.h"

#include "application.h"
#include <djv_ui/global.h>
#include <djv_ui/prefs.h>

namespace djv_bin {
namespace view {
namespace shortcut {

//------------------------------------------------------------------------------
// Prefs
//------------------------------------------------------------------------------

Prefs::Prefs()
{
  using djv_ui::shortcut::Shortcut;
  
  String tmp;
  
  const List<String> & label = shortcut::label_shortcut();
  
  _shortcut += Shortcut(label[EXIT], FL_COMMAND + 'q');
  
  _shortcut += Shortcut(label[FILE_OPEN], FL_COMMAND + 'o');
  _shortcut += Shortcut(label[FILE_RELOAD], FL_COMMAND + 'r');
  _shortcut += Shortcut(label[FILE_SAVE], FL_COMMAND + 's');
  _shortcut += Shortcut(label[FILE_SAVE_FRAME], FL_SHIFT + FL_COMMAND + 's');
  _shortcut += Shortcut(label[FILE_CLOSE], FL_COMMAND + 'w');
  _shortcut += Shortcut(label[FILE_LAYER_DEFAULT], FL_COMMAND + '`');
  _shortcut += Shortcut(label[FILE_LAYER_1], FL_COMMAND + '1');
  _shortcut += Shortcut(label[FILE_LAYER_2], FL_COMMAND + '2');
  _shortcut += Shortcut(label[FILE_LAYER_3], FL_COMMAND + '3');
  _shortcut += Shortcut(label[FILE_LAYER_4], FL_COMMAND + '4');
  _shortcut += Shortcut(label[FILE_LAYER_5], FL_COMMAND + '5');
  _shortcut += Shortcut(label[FILE_LAYER_6], FL_COMMAND + '6');
  _shortcut += Shortcut(label[FILE_LAYER_7], FL_COMMAND + '7');
  _shortcut += Shortcut(label[FILE_LAYER_8], FL_COMMAND + '8');
  _shortcut += Shortcut(label[FILE_LAYER_9], FL_COMMAND + '9');
  _shortcut += Shortcut(label[FILE_LAYER_10], FL_COMMAND + '0');
  _shortcut += Shortcut(label[FILE_LAYER_PREV], FL_COMMAND + '-');
  _shortcut += Shortcut(label[FILE_LAYER_NEXT], FL_COMMAND + '=');
  _shortcut += Shortcut(label[FILE_PROXY_NONE], 0);
  _shortcut += Shortcut(label[FILE_PROXY_1_2], 0);
  _shortcut += Shortcut(label[FILE_PROXY_1_4], 0);
  _shortcut += Shortcut(label[FILE_PROXY_1_8], 0);
  
  _shortcut += Shortcut(label[WINDOW_NEW], FL_COMMAND + 'n');
  _shortcut += Shortcut(label[WINDOW_COPY], FL_COMMAND + 'c');
  _shortcut += Shortcut(label[WINDOW_CLOSE], FL_COMMAND + 'e');
  _shortcut += Shortcut(label[WINDOW_FIT], 'f');
  _shortcut += Shortcut(label[WINDOW_FULLSCREEN], 'u');
  _shortcut += Shortcut(label[WINDOW_TOOLBAR_BUTTONS], 0);
  _shortcut += Shortcut(label[WINDOW_TOOLBAR_PLAYBACK], 0);
  _shortcut += Shortcut(label[WINDOW_TOOLBAR_INFO], 0);
  
  _shortcut += Shortcut(label[VIEW_LEFT], 0);
  _shortcut += Shortcut(label[VIEW_RIGHT], 0);
  _shortcut += Shortcut(label[VIEW_UP], 0);
  _shortcut += Shortcut(label[VIEW_DOWN], 0);
  _shortcut += Shortcut(label[VIEW_CENTER], FL_Insert);
  _shortcut += Shortcut(label[VIEW_ZOOM_IN], '=');
  _shortcut += Shortcut(label[VIEW_ZOOM_OUT], '-');
  _shortcut += Shortcut(label[VIEW_ZOOM_RESET], '0');
  _shortcut += Shortcut(label[VIEW_RESET], FL_Delete);
  _shortcut += Shortcut(label[VIEW_FIT], FL_BackSpace);
  _shortcut += Shortcut(label[VIEW_HUD], 'h');
  
  _shortcut += Shortcut(label[IMAGE_FRAME_STORE], 'e');
  _shortcut += Shortcut(label[IMAGE_FRAME_STORE_LOAD], FL_SHIFT + 'e');
  _shortcut += Shortcut(label[IMAGE_MIRROR_HORIZONTAL], FL_F + 1);
  _shortcut += Shortcut(label[IMAGE_MIRROR_VERTICAL], FL_F + 2);
  _shortcut += Shortcut(label[IMAGE_SCALE_NONE], FL_F + 5);
  _shortcut += Shortcut(label[IMAGE_SCALE_16_9], FL_F + 6);
  _shortcut += Shortcut(label[IMAGE_SCALE_1_85], FL_F + 7);
  _shortcut += Shortcut(label[IMAGE_SCALE_2_35], FL_F + 8);
  _shortcut += Shortcut(label[IMAGE_ROTATE_0], FL_F + 9);
  _shortcut += Shortcut(label[IMAGE_ROTATE_90], FL_F + 10);
  _shortcut += Shortcut(label[IMAGE_ROTATE_180], FL_F + 11);
  _shortcut += Shortcut(label[IMAGE_ROTATE_270], FL_F + 12);
  _shortcut += Shortcut(label[IMAGE_COLOR_PROFILE], 0);
  _shortcut += Shortcut(label[IMAGE_CHANNEL_RED], 'r');
  _shortcut += Shortcut(label[IMAGE_CHANNEL_GREEN], 'g');
  _shortcut += Shortcut(label[IMAGE_CHANNEL_BLUE], 'b');
  _shortcut += Shortcut(label[IMAGE_CHANNEL_ALPHA], 'a');
  _shortcut += Shortcut(label[IMAGE_DISPLAY_PROFILE], 0);
  _shortcut += Shortcut(label[IMAGE_DISPLAY_PROFILE_RESET], FL_SHIFT + '`');
  _shortcut += Shortcut(label[IMAGE_DISPLAY_PROFILE_1], FL_SHIFT + '1');
  _shortcut += Shortcut(label[IMAGE_DISPLAY_PROFILE_2], FL_SHIFT + '2');
  _shortcut += Shortcut(label[IMAGE_DISPLAY_PROFILE_3], FL_SHIFT + '3');
  _shortcut += Shortcut(label[IMAGE_DISPLAY_PROFILE_4], FL_SHIFT + '4');
  _shortcut += Shortcut(label[IMAGE_DISPLAY_PROFILE_5], FL_SHIFT + '5');
  _shortcut += Shortcut(label[IMAGE_DISPLAY_PROFILE_6], FL_SHIFT + '6');
  _shortcut += Shortcut(label[IMAGE_DISPLAY_PROFILE_7], FL_SHIFT + '7');
  _shortcut += Shortcut(label[IMAGE_DISPLAY_PROFILE_8], FL_SHIFT + '8');
  _shortcut += Shortcut(label[IMAGE_DISPLAY_PROFILE_9], FL_SHIFT + '9');
  _shortcut += Shortcut(label[IMAGE_DISPLAY_PROFILE_10], FL_SHIFT + '0');
  
  _shortcut += Shortcut(label[PLAYBACK_STOP], 'k');
  _shortcut += Shortcut(label[PLAYBACK_FORWARD], 'l');
  _shortcut += Shortcut(label[PLAYBACK_REVERSE], 'j');
  _shortcut += Shortcut(label[PLAYBACK_LOOP], ';');
  _shortcut += Shortcut(label[PLAYBACK_START], FL_Up);
  _shortcut += Shortcut(label[PLAYBACK_START_ABS], FL_SHIFT + FL_Up);
  _shortcut += Shortcut(label[PLAYBACK_END], FL_Down);
  _shortcut += Shortcut(label[PLAYBACK_END_ABS], FL_SHIFT + FL_Down);
  _shortcut += Shortcut(label[PLAYBACK_PREV], FL_Left);
  _shortcut += Shortcut(label[PLAYBACK_PREV_X10], FL_SHIFT + FL_Left);
  _shortcut += Shortcut(label[PLAYBACK_PREV_X100], FL_ALT + FL_Left);
  _shortcut += Shortcut(label[PLAYBACK_NEXT], FL_Right);
  _shortcut += Shortcut(label[PLAYBACK_NEXT_X10], FL_SHIFT + FL_Right);
  _shortcut += Shortcut(label[PLAYBACK_NEXT_X100], FL_ALT + FL_Right);
  _shortcut += Shortcut(label[PLAYBACK_IN_OUT], 'p');
  _shortcut += Shortcut(label[PLAYBACK_IN_MARK], 'i');
  _shortcut += Shortcut(label[PLAYBACK_IN_RESET], FL_SHIFT + 'i');
  _shortcut += Shortcut(label[PLAYBACK_OUT_MARK], 'o');
  _shortcut += Shortcut(label[PLAYBACK_OUT_RESET], FL_SHIFT + 'o');
  
  _shortcut += Shortcut(label[TOOL_MAGNIFY], '1');
  _shortcut += Shortcut(label[TOOL_COLOR_PICKER], '2');
  _shortcut += Shortcut(label[TOOL_HISTOGRAM], '3');
  _shortcut += Shortcut(label[TOOL_INFO], '4');
  
  djv_ui::prefs::Prefs prefs(djv_ui::prefs::prefs(), "shortcut");
  for (list::size_type i = 0; i < _shortcut.size(); ++i)
    if (prefs.get(_shortcut[i].name, &tmp))
      djv_ui::shortcut::serialize(&tmp, &_shortcut[i].value);
}

Prefs::~Prefs()
{
  djv_ui::prefs::Prefs prefs(djv_ui::prefs::prefs(), "shortcut");
  for (list::size_type i = 0; i < _shortcut.size(); ++i)
    prefs.set(
      _shortcut[i].name,
      djv_ui::shortcut::serialize(_shortcut[i].value)
    );
}

void Prefs::shortcut(const List<djv_ui::shortcut::Shortcut> & in)
{
  if (in == _shortcut)
    return;
  
  _shortcut = in;
  
  djv_ui::global()->window_update();
}

const List<djv_ui::shortcut::Shortcut> & Prefs::shortcut() const
{
  return _shortcut;
}

//------------------------------------------------------------------------------

}
}
}

