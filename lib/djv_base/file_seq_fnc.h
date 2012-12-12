/*------------------------------------------------------------------------------
 include/djv_base/file_seq_fnc.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

namespace djv_base {
namespace file_seq {

//------------------------------------------------------------------------------
//!\name Functions
//------------------------------------------------------------------------------
//@{

//! Sort frame numbers in a sequence.

_DJV_BASE void sort(Seq &);

//! Find the closest frame in a sequence.

_DJV_BASE int64_t find_closest(int64_t, const List<int64_t> &);

//@}
//------------------------------------------------------------------------------
//!\name Conversion
//------------------------------------------------------------------------------
//@{

_DJV_BASE String seq_to_string(const Seq &);

_DJV_BASE Seq string_to_seq(const String &);

_DJV_BASE String frame_to_string(int64_t, int pad = 0);

inline int64_t string_to_frame(const String &, int * pad = 0);

_DJV_BASE String & operator >> (String &, Seq &) throw (String);
_DJV_BASE String & operator >> (String &, COMPRESS &) throw (String);

_DJV_BASE String & operator << (String &, const Seq &);
_DJV_BASE String & operator << (String &, COMPRESS);

//@}
//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

_DJV_BASE const List<String> & label_compress();

//@}
//------------------------------------------------------------------------------

}
}

