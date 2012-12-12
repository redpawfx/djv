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

//! \file djv_form_widget.h

#ifndef DJV_FORM_WIDGET_H
#define DJV_FORM_WIDGET_H

#include <djv_frame.h>

namespace djv
{

class Vertical_Layout;

//------------------------------------------------------------------------------
//! \class Form_Widget
//!
//! This class provides a form widget.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT Form_Widget : public Widget
{
public:

    //! Constructor.

    Form_Widget();

    //! Destructor.

    virtual ~Form_Widget();

    //! Add a row.
    
    void add_row(const String &, Widget *);
    
    //! Remove a row.
    
    void remove_row(int);
    
    //! Get the number of rows.
    
    int row_count() const;
    
    //! Get the label layout items.
    
    const List<Layout_Item *> & labels() const;
    
private:

    List<Layout_Item *> _labels;
    Vertical_Layout *   _layout;
};

} // djv

#endif // DJV_FORM_WIDGET_H

