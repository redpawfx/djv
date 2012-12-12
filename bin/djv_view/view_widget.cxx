/*------------------------------------------------------------------------------
 bin/djv_view/view_widget.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "view_widget.h"

#include "input_prefs.h"
#include <djv_ui/style.h>
#include <djv_image/time.h>
#include <djv_base/math.h>
#include <FL/Fl.H>
#include <FL/gl.h>

namespace djv_bin {
namespace view {
namespace view {

//------------------------------------------------------------------------------
// Widget
//------------------------------------------------------------------------------

Widget::Widget() :
  pick_signal(this),
  mouse_wheel_signal(this),
  mouse_wheel_value_signal(this),
  file_signal(this),
  _zoom_tmp(0.0),
  _mouse_wheel(false),
  _mouse_wheel_tmp(0),
  _grid(GRID(0)),
  _hud(false),
  _hud_show(true, _HUD_SHOW_SIZE),
  _inside(false),
  _size_min(1) // XXX Fullscreen mode.
{
  text_font(djv_ui::style::global()->font_bold());
}

void Widget::del()
{
  djv_ui::image::View::del();
  
  Fl::remove_timeout(mouse_wheel_timeout_callback, this);
}

void Widget::zoom(double in)
{
  djv_ui::image::View::zoom(in);
}

void Widget::zoom(double in, const V2i & focus)
{
  djv_ui::image::View::zoom(in, focus);
}

double Widget::zoom() const
{
  return djv_ui::image::View::zoom();
}

void Widget::zoom_focus(double in)
{
  djv_ui::image::View::zoom(
    in,
    _inside ? _mouse : (geom().size / 2)
  );
}

void Widget::grid(GRID in)
{
  _grid = in;
}

void Widget::grid_color(const djv_image::Color & in)
{
  _grid_color = in;
}

void Widget::hud(bool in)
{
  _hud = in;
}

void Widget::hud_info(const Hud_Info & in)
{
  _hud_info = in;
}

void Widget::hud_show(const List<bool> & in)
{
  _hud_show = in;
}

void Widget::hud_color(const djv_image::Color & in)
{
  _hud_color = in;
}

void Widget::hud_background(HUD_BACKGROUND in)
{
  _hud_background = in;
}

void Widget::hud_background_color(const djv_image::Color & in)
{
  _hud_background_color = in;
}

int Widget::handle(int in)
{
  DJV_DEBUG("view::Widget::handle");
  DJV_DEBUG_PRINT("in = " << in);
  
  const V2i mouse = V2i(Fl::event_x(), geom().h - 1 - Fl::event_y());
  
  switch (in)
  {
    case FL_PUSH:
    {
      DJV_DEBUG("view::Widget::handle");
      DJV_DEBUG_PRINT("push");
  
      take_focus();

      _view_tmp = view();
      _zoom_tmp = zoom();
      
      _mouse = mouse;
      _mouse_td = _mouse;
      
      if (Fl::event_button2() && Fl::event_ctrl())
      {
        zoom(zoom() * 2.0, _mouse);
        redraw();
      }
      else if (Fl::event_button3() && Fl::event_ctrl())
      {
        zoom(zoom() * 0.5, _mouse);
        redraw();
      }
      else if (Fl::event_button2() || Fl::event_ctrl())
      {
        cursor(FL_CURSOR_MOVE);
      }
      else if (Fl::event_button3() || Fl::event_shift())
      {
        cursor(FL_CURSOR_WE);
        mouse_wheel_signal.emit(input::MOUSE_WHEEL_PLAYBACK_SHUTTLE);
      }
      else
      {
        cursor(FL_CURSOR_CROSS);
        pick_signal.emit(mouse);
      }
    }
    return 1;

    case FL_MOVE:
    {
      _mouse = mouse;
    }
    return 1;
    
    case FL_DRAG:
    {
      DJV_DEBUG("view::Widget::handle");
      DJV_DEBUG_PRINT("drag");
    
      _mouse = mouse;
      
      if (Fl::event_button2() || Fl::event_ctrl())
      {
        view(_view_tmp + (_mouse - _mouse_td));
        cursor(FL_CURSOR_MOVE);
        redraw();
      }
      else if (Fl::event_button3() || Fl::event_shift())
      {
        cursor(FL_CURSOR_WE);
        mouse_wheel_value_signal.emit((_mouse.x - _mouse_td.x) / 5);
      }
      else
      {
        cursor(FL_CURSOR_CROSS);
        pick_signal.emit(mouse);
      }
    }
    return 1;

    case FL_MOUSEWHEEL:
    {
      DJV_DEBUG("view::Widget::handle");
      DJV_DEBUG_PRINT("mouse wheel");
      
      input::MOUSE_WHEEL mouse_wheel = input::prefs()->mouse_wheel();
      if (Fl::event_shift())
        mouse_wheel = input::prefs()->mouse_wheel_shift();
      else if (Fl::event_ctrl())
        mouse_wheel = input::prefs()->mouse_wheel_ctrl();
      
      switch (mouse_wheel)
      {
        case input::MOUSE_WHEEL_VIEW_ZOOM:
        
          zoom_focus(zoom() * (Fl::event_dy() > 0 ? 0.5 : 2.0));
        
          redraw();
          
          break;

        case input::MOUSE_WHEEL_PLAYBACK_SHUTTLE:
        
          if (! _mouse_wheel)
          {
            cursor(FL_CURSOR_WE);
          
            mouse_wheel_signal.emit(mouse_wheel);
          
            _mouse_wheel = true;
          }
          
          _mouse_wheel_tmp += Fl::event_dy();
        
          mouse_wheel_value_signal.emit(_mouse_wheel_tmp);
          
          break;

        case input::MOUSE_WHEEL_PLAYBACK_SPEED:
        
          if (! _mouse_wheel)
          {
            mouse_wheel_signal.emit(mouse_wheel);
          
            _mouse_wheel = true;
          }
        
          _mouse_wheel_tmp += Fl::event_dy();
        
          mouse_wheel_value_signal.emit(_mouse_wheel_tmp);
          
          break;
      }

      Fl::remove_timeout(mouse_wheel_timeout_callback, this);
      Fl::add_timeout(0.3, mouse_wheel_timeout_callback, this);
    }
    return 1;

    case FL_RELEASE:
    {
      DJV_DEBUG("view::Widget::handle");
      DJV_DEBUG_PRINT("release");
    
      // Reset cursor.
      
      cursor(FL_CURSOR_DEFAULT);
    }
    return 1;
      
    case FL_ENTER: _inside = true; return 1;
    case FL_LEAVE: _inside = false; return 1;
    case FL_FOCUS:
    case FL_UNFOCUS: return 1;
    
    case FL_DND_ENTER:
    case FL_DND_DRAG:
    case FL_DND_LEAVE:
    case FL_DND_RELEASE: return 1;
    case FL_PASTE:
    {
      //DJV_DEBUG("view::Widget::handle");
      
      String tmp(Fl::event_text());
      
      // XXX Convert URL.
      
      static const String url = "file://";
      if (tmp.find(url) != String::npos)
        tmp.replace(0, url.size(), "");
      int i = static_cast<int>(tmp.size()) - 1;
      for (; i >= 0; --i)
        if (tmp[i] != '\n' && tmp[i] != '\r') break;
      tmp = String(tmp, 0, i + 1);
      
      //DJV_DEBUG_PRINT("file = " << tmp);
      file_signal.emit(tmp);

    }
    return 1;
  }
  
  return djv_ui::image::View::handle(in);
}

const V2i & Widget::mouse() const
{
  return _mouse;
}

void Widget::draw()
{
  if (! visible())
    return;
  
  //DJV_DEBUG("view::Widget::draw");
  
  djv_ui::image::View::draw();
  
  if (_grid)
    draw_grid();
  if (_hud)
    draw_hud();
}

void Widget::draw_grid()
{
  //DJV_DEBUG("view::Widget::draw_grid");
  //DJV_DEBUG_PRINT("size = " << geom().size);
  //DJV_DEBUG_PRINT("grid = " << label_grid()[_grid]);
  
  int inc = 0;
  switch (_grid)
  {
    case GRID_1x1: inc = 1; break;
    case GRID_10x10: inc = 10; break;
    case GRID_100x100: inc = 100; break;
    default: break;
  }
  
  // Bail if too small.
  
  if ((inc * zoom()) <= 2) return;
  
  // Compute view area.
  
  const Box2i area = Box2i(
    vector::floor<double, int>(
      V2f(-view()) / zoom() / static_cast<double>(inc)
    ) - 1,
    vector::ceil<double, int>(
      V2f(geom().size) / zoom() / static_cast<double>(inc)
    ) + 2
  ) * inc;
  //DJV_DEBUG_PRINT("area = " << area);
  
  // Draw.
  
  djv_image::gl::color(_grid_color);
  
  glPushMatrix();
  glTranslated(view().x, view().y, 0);
  glScaled(zoom(), zoom(), 1.0);
  
  glBegin(GL_LINES);
  for (int y = 0; y <= area.h; y += inc)
  {
    glVertex2i(area.x, area.y + y);
    glVertex2i(area.x + area.w, area.y + y);
  }
  for (int x = 0; x <= area.w; x += inc)
  {
    glVertex2i(area.x + x, area.y);
    glVertex2i(area.x + x, area.y + area.h);
  }
  glEnd();
  
  glPopMatrix();
}

void Widget::draw_hud()
{
  DJV_DEBUG("view::Widget::draw_hud");
  
  const Box2i geom = this->geom();
  const int margin = djv_ui::style::global()->margin_widget();
  
  // XXX OMGWTFBBQ? This somehow resets the OpenGL state so FLTK's text drawing
  // functions work?
  
  glGetError();

  // Setup.

  //glViewport(0, 0, geom.w, geom.h);
  //image::gl::ortho(V2i(geom.w, geom.h));

  gl_font(text_font(), djv_ui::style::global()->font_size());
  
  V2i p;
  const int h = gl_height();
  String tmp;
  
  // Draw.

  p = V2i(margin, geom.h - h - margin);
  
  if (_hud_show[HUD_FILE_NAME])
  {
    tmp = string::Format(label_hud_file_name).
      arg(_hud_info.info.file_name);
    
    draw_hud(
      tmp, p,
      _hud_color, _hud_background, _hud_background_color
    );

    p.y -= h;
  }

  if (_hud_show[HUD_LAYER])
  {
    tmp = string::Format(label_hud_layer).
      arg(_hud_info.info.layer_name);
    
    draw_hud(
      tmp, p,
      _hud_color, _hud_background, _hud_background_color
    );
    
    p.y -= h;
  }

  if (_hud_show[HUD_SIZE])
  {
    tmp = string::Format(label_hud_size).
      arg(_hud_info.info.size.x).
      arg(_hud_info.info.size.y).
      arg(vector::aspect(_hud_info.info.size), 0, -2);
    
    draw_hud(
      tmp, p,
      _hud_color, _hud_background, _hud_background_color
    );
    
    p.y -= h;
  }
  
  if (_hud_show[HUD_PROXY])
  {
    tmp = string::Format(label_hud_proxy).
      arg(_hud_info.info.proxy);
    
    draw_hud(
      tmp, p,
      _hud_color, _hud_background, _hud_background_color
    );
    
    p.y -= h;
  }
  
  if (_hud_show[HUD_PIXEL])
  {
    tmp = string::Format(label_hud_pixel).
      arg(string::label(_hud_info.info.pixel));
    
    draw_hud(
      tmp, p,
      _hud_color, _hud_background, _hud_background_color
    );
  }

  if (_hud_show[HUD_TAG] && _hud_info.tag.size())
  {
    List<String> tmp_list;
    
    tmp_list += label_hud_tag;
    
    const List<String> key_list = _hud_info.tag.key_list();
    for (list::size_type i = 0; i < key_list.size(); ++i)
      tmp_list += string::Format("%% = %%").arg(key_list[i]).
        arg(_hud_info.tag[key_list[i]]);
    
    p = V2i(margin, h * (static_cast<int>(tmp_list.size()) - 1) + margin);

    for (
      List<String>::const_iterator i = tmp_list.begin();
      i != tmp_list.end();
      ++i
    ) {
      draw_hud(
        *i, p,
        _hud_color, _hud_background, _hud_background_color
      );
      
      p.y -= h;
    }
  }

  p = V2i(geom.w - margin, geom.h - h - margin);
  
  if (_hud_show[HUD_FRAME])
  {
    tmp = string::Format(label_hud_frame).
      arg(djv_image::time::units_to_string(_hud_info.frame, _hud_info.speed));
    
    draw_hud(
      tmp, V2i(p.x - math::ceil(gl_width(tmp.c_str())), p.y),
      _hud_color, _hud_background, _hud_background_color
    );
    
    p.y -= h;
  }
  
  if (_hud_show[HUD_SPEED])
  {
    tmp = string::Format("%%").arg(_hud_info.speed_real, 0, -2);
    p.x -= math::ceil(gl_width(tmp.c_str()));
    
    draw_hud(
      tmp, p,
      _hud_info.dropped_frames ?
      djv_ui::style::global()->color_value().select :
      _hud_color,
      _hud_background,
      _hud_background_color
    );
    
    tmp = string::Format(label_hud_speed).arg(
      djv_image::time::speed_to_float(_hud_info.speed), 0, -2);
    
    p.x -= math::ceil(gl_width(tmp.c_str()));
    
    draw_hud(
      tmp, p,
      _hud_color, _hud_background, _hud_background_color
    );
  }
}

void Widget::draw_hud(
  const String & in,
  const V2i & position,
  const djv_image::Color & text_color,
  HUD_BACKGROUND background,
  const djv_image::Color & background_color
) {
  const int d = gl_descent();
  
  // Draw background.
  
  djv_image::gl::color(background_color);
    
  switch (background)
  {
    case HUD_BACKGROUND_NONE: break;

    case HUD_BACKGROUND_SOLID:
    {
      const Box2i box(
        box::border(Box2i(position, djv_ui::base::string_size(in)),
        V2i(0, 0)));
      glBegin(GL_QUADS);
      glVertex2i(box.x, box.y);
      glVertex2i(box.x + box.w, box.y);
      glVertex2i(box.x + box.w, box.y + box.h);
      glVertex2i(box.x, box.y + box.h);
      glEnd();
    }
    break;

    case HUD_BACKGROUND_SHADOW:
      gl_draw(in.c_str(), position.x + 1, position.y + d - 1);
      break;
  }

  // Draw foreground.
  
  djv_image::gl::color(text_color);

  gl_draw(in.c_str(), position.x, position.y + d);
}

const V2i & Widget::size_hint() const
{
  return _size_min;
}

const V2i & Widget::size_min() const
{
  return _size_min;
}

void Widget::mouse_wheel_timeout_callback()
{
  // Reset cursor.
  
  cursor(FL_CURSOR_DEFAULT);
  
  // Reset mouse wheel.
      
  if (_mouse_wheel)
  {
    _mouse_wheel_tmp = 0;

    _mouse_wheel = false;
  }

  Fl::remove_timeout(mouse_wheel_timeout_callback, this);
}

//------------------------------------------------------------------------------

}
}
}

