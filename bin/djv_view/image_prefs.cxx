/*------------------------------------------------------------------------------
 bin/djv_view/image_prefs.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "image_prefs.h"

#include <djv_ui/prefs.h>
#include <djv_image/image.h>

namespace djv_bin {
namespace view {
namespace image {

//------------------------------------------------------------------------------
// Prefs
//------------------------------------------------------------------------------

Prefs::Prefs() :
  mirror_signal(this),
  scale_signal(this),
  rotate_signal(this),
  color_profile_signal(this),
  display_profile_signal(this),
  display_profile_value_signal(this),
  display_profile_list_signal(this),
  channel_signal(this),
  _frame_store_file_reload(false),
  _scale(SCALE(0)),
  _rotate(ROTATE(0)),
  _color_profile(true),
  _display_profile(-1),
  _channel(CHANNEL(0))
{
  djv_ui::prefs::Prefs prefs(djv_ui::prefs::prefs(), "image");

  djv_ui::prefs::get(&prefs, "frame_store_file_reload",
    &_frame_store_file_reload);
  djv_ui::prefs::get(&prefs, "mirror", &_mirror);
  djv_ui::prefs::get(&prefs, "scale", &_scale);
  djv_ui::prefs::get(&prefs, "rotate", &_rotate);
  djv_ui::prefs::get(&prefs, "color_profile", &_color_profile);

  djv_ui::prefs::Prefs display_profile_prefs(prefs, "display_profile");
  djv_ui::prefs::get(&display_profile_prefs, "value", &_display_profile);
  int display_profile_list_size = 0;
  djv_ui::prefs::get(&display_profile_prefs, "size",
    &display_profile_list_size);
  for (int i = 0; i < display_profile_list_size; ++i)
  {
    Display_Profile value;
    djv_ui::prefs::get(&display_profile_prefs, string::Format("%%").arg(i),
      &value);
    const djv_image::Image * image = lut_load(value.lut_file);
    value.lut = image ? *image : djv_image::Image();
    _display_profile_list += value;
  }

  djv_ui::prefs::get(&prefs, "channel", &_channel);
}
  
Prefs::~Prefs()
{
  djv_ui::prefs::Prefs prefs(djv_ui::prefs::prefs(), "image");

  djv_ui::prefs::set(&prefs, "frame_store_file_reload",
    _frame_store_file_reload);
  djv_ui::prefs::set(&prefs, "mirror", _mirror);
  djv_ui::prefs::set(&prefs, "scale", _scale);
  djv_ui::prefs::set(&prefs, "rotate", _rotate);
  djv_ui::prefs::set(&prefs, "color_profile", _color_profile);

  djv_ui::prefs::Prefs display_profile_prefs(prefs, "display_profile");
  djv_ui::prefs::set(&display_profile_prefs, "value",_display_profile);
  djv_ui::prefs::set(&display_profile_prefs, "size",
    static_cast<int>(_display_profile_list.size()));
  for (list::size_type i = 0; i < _display_profile_list.size(); ++i)
    djv_ui::prefs::set(
      &display_profile_prefs,
      string::Format("%%").arg(static_cast<int>(i)),
      _display_profile_list[i]
    );

  djv_ui::prefs::set(&prefs, "channel", _channel);
}

void Prefs::frame_store_file_reload(bool in)
{
  _frame_store_file_reload = in;
}

bool Prefs::frame_store_file_reload() const
{
  return _frame_store_file_reload;
}

void Prefs::mirror(const V2b & in)
{
  if (in == _mirror)
    return;
  
  _mirror = in;
  
  mirror_signal.emit(_mirror);
}

const V2b & Prefs::mirror() const
{
  return _mirror;
}

void Prefs::scale(SCALE in)
{
  if (in == _scale)
    return;
  
  _scale = in;
  
  scale_signal.emit(_scale);
}

image::SCALE Prefs::scale() const
{
  return _scale;
}

void Prefs::rotate(ROTATE in)
{
  if (in == _rotate)
    return;
  
  _rotate = in;
  
  rotate_signal.emit(_rotate);
}

image::ROTATE Prefs::rotate() const
{
  return _rotate;
}

void Prefs::color_profile(bool in)
{
  if (in == _color_profile)
    return;
  
  _color_profile = in;
  
  color_profile_signal.emit(_color_profile);
}

bool Prefs::color_profile() const
{
  return _color_profile;
}

void Prefs::display_profile(int in)
{
  if (in == _display_profile)
    return;
  
  _display_profile = in;
  
  Display_Profile tmp;
  
  display_profile_signal.emit(_display_profile);
  display_profile_value_signal.emit(
    (_display_profile != -1) ?
    _display_profile_list[_display_profile] :
    tmp
  );
}

int Prefs::display_profile() const
{
  return _display_profile;
}

Display_Profile Prefs::display_profile_value() const
{
  Display_Profile tmp;
  
  return
    _display_profile != -1 ?
    _display_profile_list[_display_profile] :
    tmp;
}

void Prefs::display_profile_list(const List<Display_Profile> & in)
{
  _display_profile_list = in;
  _display_profile = math::min(
    _display_profile,
    static_cast<int>(_display_profile_list.size()) - 1
  );
  
  display_profile_list_signal.emit(true);
}

const List<Display_Profile> & Prefs::display_profile_list() const
{
  return _display_profile_list;
}

List<String> Prefs::display_profile_name_list() const
{
  List<String> out;
  for (list::size_type i = 0; i < _display_profile_list.size(); ++i)
    out += _display_profile_list[i].name;
  return out;
}

void Prefs::channel(CHANNEL in)
{
  if (in == _channel)
    return;
  
  _channel = in;
  
  channel_signal.emit(_channel);
}

image::CHANNEL Prefs::channel() const
{
  return _channel;
}

//------------------------------------------------------------------------------

}
}
}

