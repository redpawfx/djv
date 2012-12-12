//------------------------------------------------------------------------------
// Copyright (c) 2004-2012 Darby Johnston
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions, and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions, and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the names of the copyright holders nor the names of any
//   contributors may be used to endorse or promote products derived from this
//   software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------

//! \file djv_view_view_widget.cpp

#include <djv_view_view_widget.h>

#include <djv_view_input_prefs.h>

#include <djv_font.h>
#include <djv_style.h>

#include <djv_gl.h>
#include <djv_file.h>
#include <djv_math.h>
#include <djv_time.h>

#include <FL/Fl.H>

namespace djv_view
{
using djv::Time;

//------------------------------------------------------------------------------
// View_Widget
//------------------------------------------------------------------------------

View_Widget::View_Widget() :
    pick_signal(this),
    mouse_wheel_signal(this),
    mouse_wheel_value_signal(this),
    file_signal(this),
    _zoom_tmp(0.0),
    _mouse_wheel(false),
    _mouse_wheel_tmp(0),
    _grid(View::GRID(0)),
    _hud(false),
    _hud_show(true, View::_HUD_SHOW_SIZE),
    _inside(false),
    _size_min(1) //! \todo Fullscreen mode.
{
    text_font(Style::global()->font_bold());
}

void View_Widget::del()
{
    Image_View::del();

    Fl::remove_timeout(mouse_wheel_timeout_callback, this);
}

void View_Widget::zoom(double in)
{
    Image_View::zoom(in);
}

void View_Widget::zoom(double in, const V2i & focus)
{
    Image_View::zoom(in, focus);
}

double View_Widget::zoom() const
{
    return Image_View::zoom();
}

void View_Widget::zoom_focus(double in)
{
    Image_View::zoom(
        in,
        _inside ? _mouse : (geom().size / 2));
}

void View_Widget::grid(View::GRID in)
{
    _grid = in;
}

void View_Widget::grid_color(const Color & in)
{
    _grid_color = in;
}

void View_Widget::hud(bool in)
{
    _hud = in;
}

void View_Widget::hud_info(const Hud_Info & in)
{
    _hud_info = in;
}

void View_Widget::hud_show(const List<bool> & in)
{
    _hud_show = in;
}

void View_Widget::hud_color(const Color & in)
{
    _hud_color = in;
}

void View_Widget::hud_background(View::HUD_BACKGROUND in)
{
    _hud_background = in;
}

void View_Widget::hud_background_color(const Color & in)
{
    _hud_background_color = in;
}

int View_Widget::handle(int in)
{
    //DJV_DEBUG("View_Widget::handle");
    //DJV_DEBUG_PRINT("in = " << in);

    const V2i mouse = V2i(Fl::event_x(), geom().h - 1 - Fl::event_y());

    switch (in)
    {
        case FL_PUSH:
        {
            //DJV_DEBUG("View_Widget::handle");
            //DJV_DEBUG_PRINT("push");

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
                mouse_wheel_signal.emit(Input::MOUSE_WHEEL_PLAYBACK_SHUTTLE);
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
            //DJV_DEBUG("View_Widget::handle");
            //DJV_DEBUG_PRINT("drag");

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
            //DJV_DEBUG("View_Widget::handle");
            //DJV_DEBUG_PRINT("mouse wheel");

            Input::MOUSE_WHEEL mouse_wheel =
                Input_Prefs::global()->mouse_wheel();

            if (Fl::event_shift())
            {
                mouse_wheel = Input_Prefs::global()->mouse_wheel_shift();
            }
            else if (Fl::event_ctrl())
            {
                mouse_wheel = Input_Prefs::global()->mouse_wheel_ctrl();
            }

            switch (mouse_wheel)
            {
                case Input::MOUSE_WHEEL_VIEW_ZOOM:

                    zoom_focus(zoom() * (Fl::event_dy() > 0 ? 0.5 : 2.0));

                    redraw();

                    break;

                case Input::MOUSE_WHEEL_PLAYBACK_SHUTTLE:

                    if (! _mouse_wheel)
                    {
                        cursor(FL_CURSOR_WE);

                        mouse_wheel_signal.emit(mouse_wheel);

                        _mouse_wheel = true;
                    }

                    _mouse_wheel_tmp += Fl::event_dy();

                    mouse_wheel_value_signal.emit(_mouse_wheel_tmp);

                    break;

                case Input::MOUSE_WHEEL_PLAYBACK_SPEED:

                    if (! _mouse_wheel)
                    {
                        mouse_wheel_signal.emit(mouse_wheel);

                        _mouse_wheel = true;
                    }

                    _mouse_wheel_tmp += Fl::event_dy();

                    mouse_wheel_value_signal.emit(_mouse_wheel_tmp);

                    break;

                default:
                    break;
            }

            Fl::remove_timeout(mouse_wheel_timeout_callback, this);
            Fl::add_timeout(0.3, mouse_wheel_timeout_callback, this);
        }

        return 1;

        case FL_RELEASE:
        {
            //DJV_DEBUG("View_Widget::handle");
            //DJV_DEBUG_PRINT("release");

            // Reset cursor.

            cursor(FL_CURSOR_DEFAULT);
        }

        return 1;

        case FL_ENTER:
            _inside = true;
            return 1;

        case FL_LEAVE:
            _inside = false;
            return 1;

        case FL_FOCUS:
        case FL_UNFOCUS:
            return 1;

        case FL_DND_ENTER:
        case FL_DND_DRAG:
        case FL_DND_LEAVE:
        case FL_DND_RELEASE:
            return 1;

        case FL_PASTE:
        {
            //DJV_DEBUG("View_Widget::handle");

            String tmp(Fl::event_text());

            //! \todo Convert URL.

            static const String url = "file://";

            if (tmp.find(url) != String::npos)
            {
                tmp.replace(0, url.size(), "");
            }

            int i = static_cast<int>(tmp.size()) - 1;

            for (; i >= 0; --i)
            {
                if (tmp[i] != '\n' && tmp[i] != '\r')
                {
                    break;
                }
            }

            tmp = String(tmp, 0, i + 1);

            //DJV_DEBUG_PRINT("file = " << tmp);
            file_signal.emit(tmp);

        }

        return 1;
    }

    return Image_View::handle(in);
}

const V2i & View_Widget::mouse() const
{
    return _mouse;
}

void View_Widget::draw()
{
    if (! visible())
    {
        return;
    }

    //DJV_DEBUG("View_Widget::draw");

    Image_View::draw();

    if (_grid)
    {
        draw_grid();
    }

    if (_hud)
    {
        draw_hud();
    }
}

void View_Widget::draw_grid()
{
    //DJV_DEBUG("View_Widget::draw_grid");
    //DJV_DEBUG_PRINT("size = " << geom().size);
    //DJV_DEBUG_PRINT("grid = " << label_grid()[_grid]);

    int inc = 0;

    switch (_grid)
    {
        case View::GRID_1x1:
            inc = 1;
            break;

        case View::GRID_10x10:
            inc = 10;
            break;

        case View::GRID_100x100:
            inc = 100;
            break;

        default:
            break;
    }

    // Bail if too small.

    if ((inc * zoom()) <= 2)
    {
        return;
    }

    // Compute view area.

    const Box2i area = Box2i(
        Vector_Util::floor<double, int>(
            V2f(-view()) / zoom() / static_cast<double>(inc)) - 1,
        Vector_Util::ceil<double, int>(
            V2f(geom().size) / zoom() / static_cast<double>(inc)) + 2) * inc;
    //DJV_DEBUG_PRINT("area = " << area);

    // Draw.

    Gl_Util::color(_grid_color);

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

namespace
{

const String
    label_hud_file_name = "File name = %%",
    label_hud_layer = "Layer = %%",
    label_hud_size = "Size = %%x%%:%%",
    label_hud_proxy = "Proxy = %%",
    label_hud_pixel = "Pixel = %%",
    label_hud_tag = "Tags:",
    label_hud_frame = "Frame = %%",
    label_hud_speed = "Speed = %%/";

} // namespace

void View_Widget::draw_hud()
{
    //DJV_DEBUG("View_Widget::draw_hud");

    const Box2i geom = this->geom();
    const int margin = Style::global()->margin_widget();

    //! \todo OMGWTFBBQ? This somehow resets the OpenGL state so FLTK's text
    //! drawing functions work?

    glGetError();

    // Setup.

    //glViewport(0, 0, geom.w, geom.h);
    //image::gl::ortho(V2i(geom.w, geom.h));

    gl_font(text_font(), Style::global()->font_size());

    V2i p;
    const int h = gl_height();
    String tmp;

    // Draw.

    p = V2i(margin, geom.h - h - margin);

    if (_hud_show[View::HUD_FILE_NAME])
    {
        tmp = String_Format(label_hud_file_name).
              arg(_hud_info.info.file_name);

        draw_hud(
            tmp, p,
            _hud_color, _hud_background, _hud_background_color);

        p.y -= h;
    }

    if (_hud_show[View::HUD_LAYER])
    {
        tmp = String_Format(label_hud_layer).
              arg(_hud_info.info.layer_name);

        draw_hud(
            tmp, p,
            _hud_color, _hud_background, _hud_background_color);

        p.y -= h;
    }

    if (_hud_show[View::HUD_SIZE])
    {
        tmp = String_Format(label_hud_size).
              arg(_hud_info.info.size.x).
              arg(_hud_info.info.size.y).
              arg(Vector_Util::aspect(_hud_info.info.size), 0, -2);

        draw_hud(
            tmp, p,
            _hud_color, _hud_background, _hud_background_color);

        p.y -= h;
    }

    if (_hud_show[View::HUD_PROXY])
    {
        tmp = String_Format(label_hud_proxy).
              arg(_hud_info.info.proxy);

        draw_hud(
            tmp, p,
            _hud_color, _hud_background, _hud_background_color);

        p.y -= h;
    }

    if (_hud_show[View::HUD_PIXEL])
    {
        tmp = String_Format(label_hud_pixel).
              arg(String_Util::label(_hud_info.info.pixel));

        draw_hud(
            tmp, p,
            _hud_color, _hud_background, _hud_background_color);
    }

    if (_hud_show[View::HUD_TAG] && _hud_info.tag.size())
    {
        List<String> tmp_list;

        tmp_list += label_hud_tag;

        const List<String> keys = _hud_info.tag.keys();

        for (size_t i = 0; i < keys.size(); ++i)
            tmp_list += String_Format("%% = %%").arg(keys[i]).
                arg(_hud_info.tag[keys[i]]);

        p = V2i(margin, h * (static_cast<int>(tmp_list.size()) - 1) + margin);

        for (
            List<String>::const_iterator i = tmp_list.begin();
            i != tmp_list.end();
            ++i)
        {
            draw_hud(
                *i, p,
                _hud_color, _hud_background, _hud_background_color);

            p.y -= h;
        }
    }

    p = V2i(geom.w - margin, geom.h - h - margin);

    if (_hud_show[View::HUD_FRAME])
    {
        tmp = String_Format(label_hud_frame).
              arg(Time::units_to_string(_hud_info.frame, _hud_info.speed));

        draw_hud(
            tmp, V2i(p.x - Math::ceil(gl_width(tmp.c_str())), p.y),
            _hud_color, _hud_background, _hud_background_color);

        p.y -= h;
    }

    if (_hud_show[View::HUD_SPEED])
    {
        tmp = String_Format("%%").arg(_hud_info.speed_real, 0, -2);
        p.x -= Math::ceil(gl_width(tmp.c_str()));

        draw_hud(
            tmp,
            p,
            _hud_info.dropped_frames ?
            Style::global()->color_value().select :
            _hud_color,
            _hud_background,
            _hud_background_color);

        tmp = String_Format(label_hud_speed).arg(
            Speed::speed_to_float(_hud_info.speed), 0, -2);

        p.x -= Math::ceil(gl_width(tmp.c_str()));

        draw_hud(
            tmp, p,
            _hud_color, _hud_background, _hud_background_color);
    }
}

void View_Widget::draw_hud(
    const String & in,
    const V2i & position,
    const Color & text_color,
    View::HUD_BACKGROUND background,
    const Color & background_color)
{
    const int d = gl_descent();

    // Draw background.

    Gl_Util::color(background_color);

    switch (background)
    {
        case View::HUD_BACKGROUND_NONE:
            break;

        case View::HUD_BACKGROUND_SOLID:
        {
            const Box2i box(
                Box_Util::border(Box2i(position, djv::Font::string_size(in)),
                                 V2i(0, 0)));
            glBegin(GL_QUADS);
            glVertex2i(box.x, box.y);
            glVertex2i(box.x + box.w, box.y);
            glVertex2i(box.x + box.w, box.y + box.h);
            glVertex2i(box.x, box.y + box.h);
            glEnd();
        }
        break;

        case View::HUD_BACKGROUND_SHADOW:
            gl_draw(in.c_str(), position.x + 1, position.y + d - 1);
            break;

        default:
            break;
    }

    // Draw foreground.

    Gl_Util::color(text_color);

    gl_draw(in.c_str(), position.x, position.y + d);
}

const V2i & View_Widget::size_hint() const
{
    return _size_min;
}

const V2i & View_Widget::size_min() const
{
    return _size_min;
}

void View_Widget::mouse_wheel_timeout_callback()
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

} // djv_view

