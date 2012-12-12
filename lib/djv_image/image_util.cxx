/*------------------------------------------------------------------------------
 include/djv_image/image_util.cxx
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#include "image_util.h"

namespace djv_image {
namespace util {

//------------------------------------------------------------------------------
// Double_Buffer
//------------------------------------------------------------------------------

Double_Buffer::Double_Buffer() :
  _p0(&_in),
  _p1(&_out)
{}

const Image & Double_Buffer::in() const
{
  return *_p0;
}

Image * Double_Buffer::out()
{
  return _p1;
}

void Double_Buffer::swap()
{
  if (_p0 == &_in)
  {
    _p0 = &_out;
    _p1 = &_in;
  }
  else
  {
    _p0 = &_in;
    _p1 = &_out;
  }
}

void Double_Buffer::tag(const Tag & in)
{
  _p0->tag = in;
  _p1->tag = in;
}

void Double_Buffer::color_profile(const Color_Profile & in)
{
  _p0->color_profile = in;
  _p1->color_profile = in;
}

Image * Double_Buffer::operator ()  ()
{
  return _p0;
}

const Image * Double_Buffer::operator () () const
{
  return _p0;
}

//------------------------------------------------------------------------------

}
}

