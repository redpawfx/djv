/*------------------------------------------------------------------------------
 lib/djv_ui/prefs.cxx
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "prefs.h"

#include <djv_base/file.h>
#include <FL/Fl_Preferences.H>
#include <stdlib.h>

namespace djv_ui {
namespace prefs {

//------------------------------------------------------------------------------
// Prefs
//------------------------------------------------------------------------------

Prefs::Prefs(const String & name) :
  _name(name),
  _prefs(
    new Fl_Preferences(
      Fl_Preferences::USER,
      file_base().c_str(),
      _name.c_str()
    )
  )
{}

Prefs::Prefs(const Prefs & in, const String & group) :
  _prefs(new Fl_Preferences(in._prefs.get(), group.c_str()))
{}

Prefs::Prefs(Prefs * in, const String & group) :
  _prefs(new Fl_Preferences(in->_prefs.get(), group.c_str()))
{}

Prefs::~Prefs()
{
  _prefs->flush();
}

const String & Prefs::name() const
{
  return _name;
}

namespace {

String pack_name(const String & in)
{
  return string::replace(in, ':', '*');
}

String unpack_name(const String & in)
{
  return string::replace(in, '*', ':');
}

}

void Prefs::set(const String & in, const String & value)
{
  _prefs->set(pack_name(in).c_str(), value.c_str());
}

bool Prefs::get(const String & in, String * out) const
{
  char * buf = 0;
  
  const bool found =
    _prefs->get(pack_name(in).c_str(), buf, "") ? true : false;
  
  *out = String(buf);
  
  if (buf)
    free(buf);
  
  return found;
}
  
void Prefs::del(const String & in)
{
  _prefs->deleteEntry(pack_name(in).c_str());
}

void Prefs::del_group(const String & in)
{
  _prefs->deleteGroup(pack_name(in).c_str());
}

List<String> Prefs::list() const
{
  List<String> out;
  
  const int size = _prefs->entries();
  for (int i = 0; i < size; ++i)
    out += unpack_name(_prefs->entry(i));
  
  return out;
}

List<String> Prefs::group() const
{
  List<String> out;
  
  const int size = _prefs->groups();
  for (int i = 0; i < size; ++i)
    out += unpack_name(_prefs->group(i));
  
  return out;
}

String Prefs::file_name() const
{
  // XXX
  
  return file_path() + name() + ".prefs";
}

String Prefs::file_path() const
{
  // XXX
  
  char tmp [string::cstring_size] = "";
  _prefs->getUserdataPath(tmp, string::cstring_size);
  
  return file::path_fix(String(tmp) + "..");
}

String Prefs::file_base()
{
  // XXX
  
  return "djv-0.8-3";
}

//------------------------------------------------------------------------------

}
}

