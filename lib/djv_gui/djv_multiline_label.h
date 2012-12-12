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

//! \file djv_multiline_label.h

#ifndef DJV_MULITLINE_LABEL_H
#define DJV_MULITLINE_LABEL_H

#include <djv_widget.h>

namespace djv
{

//------------------------------------------------------------------------------
//! \class Multiline_Label
//!
//! This class provides a multi-line label widget.
//------------------------------------------------------------------------------

class DJV_GUI_EXPORT Multiline_Label : public Widget
{
public:

    //! Constructor.

    Multiline_Label(const String & = String());

    //! Destructor.

    virtual ~Multiline_Label();

    //! Set the text.

    void set(const String &);

    //! Get the text.

    const String & get() const;

    //! This signal is emitted when the label is clicked.

    Signal<bool> action_signal;

    virtual int handle(int);

    virtual void draw();

    virtual void dirty();

    virtual void geom(const Box2i &);

    virtual const Box2i & geom() const;

    virtual const V2i & size_min() const;

    virtual Layout_Item * group_align();

    virtual Layout_Item * label_align();

private:

    void text_measure();

    V2i text_hint(const V2i &) const;

    String       _value;
    List<String> _words;
    List<int>    _word_sizes;
    int          _word_size_max;
    int          _space_size;
    int          _text_height;
    V2i          _size_min;
    V2i          _text_hint;
    int          _font_size;
};

} // djv

#endif // DJV_MULITLINE_LABEL_H

