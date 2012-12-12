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

//! \file djv_form_widget.cpp

#include <djv_form_widget.h>

#include <djv_label.h>
#include <djv_row_layout.h>
#include <djv_style.h>

namespace djv
{

//------------------------------------------------------------------------------
// Form_Widget
//------------------------------------------------------------------------------

Form_Widget::Form_Widget() :
    _layout(0)
{
    debug_path("Form_Widget");
    
    _layout = new Vertical_Layout(this);
    _layout->margin(0);
}

Form_Widget::~Form_Widget()
{}
    
void Form_Widget::add_row(const String & string, Widget * widget)
{
    Label * label = new Label(string);
    label->align(LEFT, LEFT);    
    _labels += label;    

    Layout_Item::create_group(_labels);
    
    Horizontal_Layout * layout = new Horizontal_Layout(_layout);
    layout->margin(0);
    layout->add(label);
    widget->align(LEFT, LEFT);
    layout->add(widget);
}

void Form_Widget::remove_row(int row)
{
    _layout->del(row);
    
    int j = 0;
    
    for (List<Layout_Item *>::iterator i = _labels.begin();
        i != _labels.end();
        ++i)
    {
        if (j == row)
        {
            _labels.erase(i);
            
            break;
        }
        
        ++j;
    }
}

int Form_Widget::row_count() const
{
    return _layout->list().size();
}
    
const List<Layout_Item *> & Form_Widget::labels() const
{
    return _labels;
}

} // djv
