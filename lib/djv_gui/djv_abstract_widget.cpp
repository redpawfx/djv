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

//! \file djv_abstract_widget.cpp

#include <djv_abstract_widget.h>

#include <djv_application.h>
#include <djv_style.h>

#include <FL/Fl.H>
#include <FL/fl_draw.H>

namespace djv
{

//------------------------------------------------------------------------------
// Abstract_Widget
//------------------------------------------------------------------------------

Abstract_Widget::Abstract_Widget() :
    Layout_Item(WIDGET),
    _text_font(Style::global()->font()),
    _text_color(FL_FOREGROUND_COLOR),
    _inside(false),
    _arrow_navkeys(true),
    _del(false)
{
    debug_path("Abstract_Widget");

    //DJV_DEBUG("Abstract_Widget::Abstract_Widget");

    DJV_APP->widget_add(this);
}

Abstract_Widget::~Abstract_Widget()
{
    //DJV_DEBUG("Abstract_Widget::~Abstract_Widget");
}

void Abstract_Widget::del()
{
    //if (_del)
    //  return;
    //_del = true;

    //DJV_DEBUG(String_Format("Abstract_Widget::del(%%)").
    //  arg(layout_item::debug_path(this)));

    Callback::del();

    _layout.reset();

    Fl_Widget * widget = dynamic_cast<Fl_Widget *>(this);

    DJV_ASSERT(widget);

    if (widget->parent())
    {
        //DJV_DEBUG_PRINT("fltk remove");

        widget->parent()->remove(widget);
        widget->parent(0);
    }

    Fl::delete_widget(widget);

    DJV_APP->widget_del(this);
}

void Abstract_Widget::text_font(Fl_Font in)
{
    _text_font = in;
}

void Abstract_Widget::text_color(Fl_Color in)
{
    _text_color = in;
}

void Abstract_Widget::tooltip(const String & in)
{
    _tooltip = in;
}

int Abstract_Widget::handle(int in)
{
    switch (in)
    {
        case FL_KEYBOARD:
            if (! _arrow_navkeys)
                switch (Fl::event_key())
                {
                    case FL_Left:
                    case FL_Right:
                    case FL_Up:
                    case FL_Down:
                        return -1;
                }

            break;

        case FL_ENTER:
            _inside = true;
            break;

        case FL_LEAVE:
        case FL_HIDE:
            _inside = false;
            break;
    }

    return 0;
}

void Abstract_Widget::show()
{}

namespace
{

/*void draw_item(Layout_Item * in)
{
  const Box2i & geom = in->geom();

  const int x = geom.x, y = geom.y;
  const int w = geom.w, h = geom.h;

  switch (in->item())
  {
    case layout_item::WIDGET:
      fl_color(FL_RED);
      fl_rect(x + 0, y + 0, w - 0, h - 0);
      break;

    case layout_item::LAYOUT:
      fl_color(FL_BLUE);
      fl_rect(x + 0, y + 0, w - 0, h - 0);
      break;

    case layout_item::SPACER:
      fl_color(FL_CYAN);
      fl_rectf(x + 0, y + 0, w - 0, h - 0);
      break;
  }
}

void draw_layout(layout::Layout * in)
{
  const List<Layout_Item *> & list = in->list();

  for (size_t i = 0; i < list.size(); ++i)
  {
    if (layout_item::LAYOUT == list[i]->item())
      draw_layout(static_cast<layout::Layout *>(list[i]));
    else
      draw_item(list[i]);
  }

  draw_item(in);
}*/

} // namespace

void Abstract_Widget::draw()
{
    //if (_layout.get())
    //  draw_layout(_layout.get());

    //draw_item(this);
}

void Abstract_Widget::arrow_navkeys(bool in)
{
    _arrow_navkeys = in;
}

bool Abstract_Widget::arrow_navkeys() const
{
    return _arrow_navkeys;
}

void Abstract_Widget::layout(Layout * in)
{
    _layout.reset();
    _layout = std::auto_ptr<Layout>(in);

    if (_layout.get())
    {
        _layout->parent(this);
    }
}

void Abstract_Widget::dirty()
{
    Layout_Item::dirty();

    if (_layout.get())
    {
        _layout->dirty();

        size_hint(_layout->size_hint());
    }
}

void Abstract_Widget::geom(const Box2i & in)
{
    //DJV_DEBUG(String_Format("Abstract_Widget::geom(%%)").
    //  arg(layout_item::debug_path(this)));
    //DJV_DEBUG_PRINT("in = " << in);

    Layout_Item::geom(in);

    if (_layout.get())
    {
        _layout->geom(in);
    }
}

} // djv

