/*------------------------------------------------------------------------------
 lib/djv_ui/image_io.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_UI_IMAGE_IO_H
#define DJV_UI_IMAGE_IO_H

#include <djv_ui/image_io_def.h>

#include <djv_base/plugin.h>
#include <djv_image/io.h>
#include <djv_ui/widget.h>

namespace djv_ui {

//------------------------------------------------------------------------------
//!\namespace djv_ui::image_io
//
//! Image I/O widgets.
//------------------------------------------------------------------------------

namespace image_io {

//------------------------------------------------------------------------------
//!\class Base
//
//! A base class for image I/O widgets.
//------------------------------------------------------------------------------

class _DJV_UI Base : public widget::Widget, public plugin::Plugin
{
public:
  
  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Base();
  
  virtual ~Base();
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{
  
  virtual void set(djv_image::io::Base *) = 0;
  
  callback::Signal<djv_image::io::Base *> signal;
  
  //@}
};

//------------------------------------------------------------------------------
//!\class Load
//
//! A base class for image load widgets.
//------------------------------------------------------------------------------

class _DJV_UI Load : public Base {};

//------------------------------------------------------------------------------
//!\class Save
//
//! A base class for image save widgets.
//------------------------------------------------------------------------------

class _DJV_UI Save : public Base
{};

//------------------------------------------------------------------------------
//!\class Base_Factory
//
//! Image I/O widget plugin factory.
//------------------------------------------------------------------------------

class _DJV_UI Base_Factory : public Callback_Base, public plugin::Factory
{
public:

  Base_Factory();
  
  ~Base_Factory();
  
  Base * get(djv_image::io::Base *) const;
  
  List<Base *> list() const;
  
private:

  String plugin_entry() const { return "djv_ui_image_io"; }
};

//------------------------------------------------------------------------------
//!\class Load_Factory
//
//! Image load widget plugin factory.
//------------------------------------------------------------------------------

class _DJV_UI Load_Factory : public Callback_Base, public plugin::Factory
{
public:

  Load_Factory();
  
  ~Load_Factory();
  
  Load * get(djv_image::io::Load *) const;
  
  List<Load *> list() const;
  
private:

  String plugin_entry() const { return "djv_ui_image_load"; }
};

//------------------------------------------------------------------------------
//!\class Save_Factory
//
//! Image save widget plugin factory.
//------------------------------------------------------------------------------

class _DJV_UI Save_Factory : public Callback_Base, public plugin::Factory
{
public:

  Save_Factory();
  
  ~Save_Factory();
  
  Save * get(djv_image::io::Save *) const;
  
  List<Save *> list() const;
  
private:

  String plugin_entry() const { return "djv_ui_image_save"; }
};

//------------------------------------------------------------------------------
//!\class Prefs_Widget
//
//! Image I/O preferences widget.
//------------------------------------------------------------------------------

class _DJV_UI Prefs_Widget : public widget::Widget
{
public:

  Prefs_Widget();

  callback::Signal<djv_image::io::Base *> signal;
  
private:

  DJV_CALLBACK(Prefs_Widget, widget_callback, djv_image::io::Base *);
};

//@}
//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

_DJV_UI extern const String
  prefs_title,
  prefs_base_title,
  prefs_load_title,
  prefs_save_title;

//@}
//------------------------------------------------------------------------------
//!\name Global
//------------------------------------------------------------------------------
//@{

//! Global options.

class _DJV_UI Global : public Callback_Base
{
public:

  Global();
  
  ~Global();
  
  Base_Factory * base() const;
  Load_Factory * load() const;
  Save_Factory * save() const;
  
  callback::Signal<djv_image::io::Base *> signal;
  
  List<Base *> plugin_list() const;
  
  struct Group
  {
    Group(
      const String & name = String(),
      Base * base = 0,
      Load * load = 0,
      Save * save = 0
    );
    
    String name;
    Base * base;
    Load * load;
    Save * save;
  };

  List<Group> group_list() const;

private:

  std::auto_ptr<Base_Factory> _base;
  std::auto_ptr<Load_Factory> _load;
  std::auto_ptr<Save_Factory> _save;
};

//! Access to global options.

_DJV_UI Global * global();

//@}
//------------------------------------------------------------------------------

}
}

#endif

