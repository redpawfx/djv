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

//! \file djv_label.h

#ifndef DJV_LABEL_H
#define DJV_LABEL_H

#include <djv_widget.h>

namespace djv
{

//------------------------------------------------------------------------------
//! \class Label
//!
//! This class provides a label widget.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT Label : public Widget
{
public:

    //! Constructor.

    Label(const String & = String());

    //! Destructor.

    virtual ~Label();

    //! Set the text.

    void set(const String &);

    //! Get the text.

    const String & get() const;

    //! This signal is emitted when the label is clicked.

    Signal<bool> action_signal;

    virtual int handle(int);

    virtual void draw();

    virtual void size_string(const String &);

    virtual const String & size_string() const;

    virtual void dirty();

    virtual Layout_Item * group_align();

    virtual Layout_Item * label_align();

private:

    String       _value;
    List<String> _lines;
    int          _lines_max_width;
    String       _size_string;
};

} // djv

#endif // DJV_LABEL_H

