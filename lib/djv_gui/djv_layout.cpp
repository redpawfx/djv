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

//! \file djv_layout.cpp

#include <djv_layout.h>

#include <djv_style.h>
#include <djv_widget.h>

#include <FL/Fl.H>
#include <FL/Fl_Group.H>

//#include <algorithm>

#include <stdio.h>

namespace djv
{

//------------------------------------------------------------------------------
// Layout
//------------------------------------------------------------------------------

Layout::Layout(Layout_Item * parent) :
    Layout_Item(Layout_Item::LAYOUT),
    _spacing(-1),
    _margin(-1)
{
    debug_path("Layout");

    //DJV_DEBUG("Layout::Layout");

    align(Layout_Item::CENTER, Layout_Item::CENTER);

    //! \todo Parent.

    if (parent)
    {
        if (Layout_Item::WIDGET == parent->item())
        {
            static_cast<Abstract_Widget *>(parent)->layout(this);
        }
        else if (Layout_Item::LAYOUT == parent->item())
        {
            static_cast<Layout *>(parent)->add(this);
        }

        this->parent(parent);
    }
}

Layout::~Layout()
{
    //DJV_DEBUG("Layout::~Layout");
    //DJV_DEBUG_PRINT("item = " << item());

    del();
}

void Layout::add(Layout_Item * in)
{
    if (! in)
    {
        return;
    }

    insert(in, static_cast<int>(_list.size()));
}

void Layout::insert(Layout_Item * in, int index)
{
    if (! in)
    {
        return;
    }

    //DJV_DEBUG("Layout::insert");
    //DJV_DEBUG_PRINT(Layout_Item::debug_path(this) << "/" <<
    //  Layout_Item::debug_path(in));
    //DJV_DEBUG_PRINT("index = " << index);
    //DJV_DEBUG_PRINT("item = " << in->item());

    List<Layout_Item *>::iterator i = _list.begin();
    int j = 0;

    for (; j < index; ++j, ++i)
        ;

    _list.insert(i, in);

    //! \todo Reparent item.

    if (in->parent())
    {
        if (Layout_Item::WIDGET == in->parent()->item())
        {
            if (Layout_Item::LAYOUT == in->item())
            {
                static_cast<Abstract_Widget *>(in->parent())->layout(0);
            }
        }
        else if (Layout_Item::LAYOUT == in->parent()->item())
        {
            static_cast<Layout *>(in->parent())->remove(in);
        }
    }

    if (Fl_Widget * widget = dynamic_cast<Fl_Widget *>(in))
    {
        if (widget->parent())
        {
            //DJV_DEBUG_PRINT("fltk remove");

            widget->parent()->remove(widget);
            widget->parent(0);
        }

        Layout_Item * parent = this;

        while (Layout_Item::WIDGET != parent->item())
        {
            parent = parent->parent();

            if (! parent)
            {
                break;
            }
        }

        if (Fl_Group * group = dynamic_cast<Fl_Group *>(parent))
        {
            //DJV_DEBUG_PRINT("fltk add = " << Layout_Item::debug_path(parent));

            //! \todo Is this still necessary?

            if (0 == j)
            {
                group->insert(*widget, j);
            }
            else
            {
                group->add(*widget);
            }
        }
    }

    in->parent(this);

#if defined(DJV_DEBUG)
    //debug();
#endif
}

Layout_Item * Layout::remove(int in)
{
    //DJV_DEBUG("Layout::remove");
    //DJV_DEBUG_PRINT("in = " << in);

    List<Layout_Item *>::iterator i = _list.begin();

    for (int j = 0; j < in; ++j, ++i)
        ;

    Layout_Item * item = *i;
    _list.erase(i);

    //DJV_DEBUG_PRINT("item = " << Layout_Item::debug_path(item));

    //! \todo Is this still necessary?

    if (Fl_Widget * widget = dynamic_cast<Fl_Widget *>(item))
        if (widget->parent())
        {
            //DJV_DEBUG_PRINT("fltk remove");

            widget->parent()->remove(widget);
            widget->parent(0);
        }

    item->parent(0);

    return item;
}

Layout_Item * Layout::remove(Layout_Item * in)
{
    Layout_Item * out = 0;

    List<Layout_Item *>::iterator i = _list.begin();
    int j = 0;

    for (
        const List<Layout_Item *>::const_iterator end = _list.end();
        i != end;
        ++i, ++j)
    {
        if (in == *i)
        {
            out = remove(j);
            break;
        }
    }

    return out;
}

Layout_Item * Layout::remove_child(Layout_Item * in)
{
    Layout_Item * out = remove(in);

    if (! out)
    {
        List<Layout_Item *>::iterator i = _list.begin();

        for (
            List<Layout_Item *>::const_iterator end = _list.end();
            i != end;
            ++i)
        {
            if (Layout * layout = dynamic_cast<Layout *>(*i))
            {
                out = layout->remove_child(in);

                if (out)
                {
                    break;
                }
            }
        }
    }

    return out;
}

void Layout::del(int in)
{
    //DJV_DEBUG("Layout::del");
    //DJV_DEBUG_PRINT("in = " << in);

    Layout_Item * item = remove(in);

    //! \todo Is this still necessary?

    if (Layout_Item::WIDGET == item->item())
    {
        static_cast<Abstract_Widget *>(item)->del();
    }
    else
    {
        delete item;
    }
}

void Layout::del(Layout_Item * in)
{
    for (size_t i = 0; i < _list.size(); ++i)
        if (in == _list[i])
        {
            del(static_cast<int>(i));
            break;
        }
}

void Layout::del()
{
    while (_list.size())
    {
        del(0);
    }
}

void Layout::spacing(int in)
{
    _spacing = in;
}

int Layout::spacing() const
{
    return _spacing != -1 ? _spacing : Style::global()->spacing();
}

void Layout::margin(int in)
{
    _margin = in;
}

int Layout::margin() const
{
    return _margin != -1 ? _margin : Style::global()->margin();
}

void Layout::dirty()
{
    List<Layout_Item *>::iterator i = _list.begin();

    for (
        List<Layout_Item *>::const_iterator end = _list.end();
        i != end;
        ++i)
    {
        (*i)->dirty();
    }
}

int Layout::visible() const
{
    for (
        List<Layout_Item *>::const_iterator i = _list.begin(),
        end = _list.end();
        i != end;
        ++i)
    {
        if (Layout_Item::SPACER != (*i)->item())
        {
            if ((*i)->visible())
            {
                return 1;
            }
        }
    }

    return 0;
}

int Layout::window_handle_size()
{
    int out = 0;

#if defined(DJV_OSX)

    //! \todo Hard coded value.

    out = 14;

#endif

    return out;
}

namespace
{

void _debug(const Layout_Item * in, int indent)
{
    const Box2i & geom = in->geom();

    const String tmp = String_Format("%%%% %%,%%,%%x%%\n").
                       arg(String(indent, ' ')).
                       arg(Layout_Item::debug_path(in)).
                       arg(geom.x).arg(geom.y).arg(geom.w).arg(geom.h);

    printf(tmp.c_str());
}

} // namespace

void Layout::debug(int indent)
{
//#if defined(DJV_DEBUG)
    _debug(this, indent);

    for (size_t i = 0; i < _list.size(); ++i)
    {
        if (Abstract_Widget * widget =
            dynamic_cast<Abstract_Widget *>(_list[i]))
        {
            _debug(widget, indent + 2);

            if (widget->layout())
            {
                widget->layout()->debug(indent + 2);
            }
        }

        if (Layout * layout = dynamic_cast<Layout *>(_list[i]))
        {
            layout->debug(indent + 2);
        }
    }

//#endif
}

} // djv

