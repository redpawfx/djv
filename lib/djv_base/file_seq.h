/*------------------------------------------------------------------------------
 include/djv_base/file_seq.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_FILE_SEQ_H
#define DJV_FILE_SEQ_H

#include <djv_base/file_seq_def.h>

#include <djv_base/base.h>

namespace djv_base {

//------------------------------------------------------------------------------
//!\namespace djv_base::file_seq
//
//! File sequences.
//------------------------------------------------------------------------------

namespace file_seq {

//------------------------------------------------------------------------------
//!\name Options
//------------------------------------------------------------------------------
//@{

//! Sequence compression.

// XXX Forward declared.

enum COMPRESS
{
  COMPRESS_OFF,     //!< No sequence compression
  COMPRESS_SPARSE,  //!< Sparse sequence compression, for example: 1-3,5
  COMPRESS_RANGE,   //!< Range sequence compression, for example: 1-5
  
  _COMPRESS_SIZE
};

// XXX Sizes larger than seq_max are probably an error...

static const int64_t seq_max = 3 * 60 * 60 * 24;

//@}
//------------------------------------------------------------------------------
//!\class Seq
//
//! A file sequence.
//------------------------------------------------------------------------------

struct _DJV_BASE Seq
{
  //----------------------------------------------------------------------------
  //!\name Construction
  //@{
  
  Seq();
  
  Seq(const List<int64_t> &, int pad = 0);
  
  Seq(int64_t start, int64_t end, int pad = 0);
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Components
  //@{
  
  //! List of frame numbers.
  
  List<int64_t> list;
  
  //! Frame number padding; e.g., 0010.
  
  int pad;

  //@}
  //----------------------------------------------------------------------------
  //!\name Functions
  //@{
  
  //! Set the list of frame numbers.
  
  void set(int64_t start, int64_t end);
  
  inline int64_t start() const;
  inline int64_t end() const;
  
  //@}
};

//------------------------------------------------------------------------------
//!\name Comparison
//------------------------------------------------------------------------------
//@{

inline bool operator == (const Seq &, const Seq &);

inline bool operator != (const Seq &, const Seq &);

//@}
//------------------------------------------------------------------------------
//!\name Debugging
//------------------------------------------------------------------------------
//@{

_DJV_BASE Debug & operator << (Debug &, const Seq &);

//@}
//------------------------------------------------------------------------------

}
}

#include <djv_base/file_seq_inline.h>
#include <djv_base/file_seq_fnc.h>
#include <djv_base/file_seq_fnc_inline.h>
#endif

