/*------------------------------------------------------------------------------
 include/djv_image/tag.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_IMAGE_TAG_H
#define DJV_IMAGE_TAG_H

#include <djv_image/tag_def.h>

#include <djv_image/base.h>

namespace djv_image {

//------------------------------------------------------------------------------
//!\namespace djv_image::tag
//
//! Image tags.
//------------------------------------------------------------------------------

namespace tag {

//------------------------------------------------------------------------------
//!\name Options
//------------------------------------------------------------------------------
//@{

//! Standard image tags.

enum TAG
{
  PROJECT,
  CREATOR,
  DESCRIPTION,
  COPYRIGHT,
  TIME,
  UTC_OFFSET,
  KEYCODE,
  TIMECODE,
  
  _TAG_SIZE
};

//@}
//------------------------------------------------------------------------------
//!\class Tag
//
//! A collection of image tags.
//------------------------------------------------------------------------------

class _DJV_IMAGE Tag
{
public:
  
  //----------------------------------------------------------------------------
  //!\name Functions
  //@{
  
  void add(const Tag &);
  void add(const String & key, const String & value);
  
  String get(const String & key) const;

  List<String> key_list() const;  
  List<String> value_list() const;
  
  int size() const;
  
  bool is_valid(const String & key);
  
  void clear();

  //@}
  //----------------------------------------------------------------------------
  //!\name Operators
  //@{
  
  String & operator [] (const String & key);

  String operator [] (const String & key) const;
  
  //@}
  
private:

  typedef std::pair<String, String> Pair;
  
  List<Pair> _list;
};

//------------------------------------------------------------------------------
//!\name Comparison
//------------------------------------------------------------------------------
//@{

_DJV_IMAGE bool operator == (const Tag &, const Tag &);

_DJV_IMAGE bool operator != (const Tag &, const Tag &);

//@}
//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

_DJV_IMAGE const List<String> & label_tag();

//@}
//------------------------------------------------------------------------------

}
}

#endif

