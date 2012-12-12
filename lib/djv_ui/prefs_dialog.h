/*------------------------------------------------------------------------------
 lib/djv_ui/prefs_dialog.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_UI_PREFS_DIALOG_H
#define DJV_UI_PREFS_DIALOG_H

#include <djv_ui/prefs_dialog_def.h>

#include <djv_ui/dialog.h>

namespace djv_ui {

//------------------------------------------------------------------------------
//!\namespace djv_ui::prefs_dialog
//
//! Preferences dialog.
//------------------------------------------------------------------------------

namespace prefs_dialog {

//------------------------------------------------------------------------------
//!\class Style
//
//! Style preferences widget.
//------------------------------------------------------------------------------

class _DJV_UI Style : public widget::Widget
{
public:

  Style();
  
private:

  DJV_CALLBACK(Style, color_callback, int);
  DJV_CALLBACK(Style, color_foreground_callback, const djv_image::Color &);
  DJV_CALLBACK(Style, color_background_callback, const djv_image::Color &);
  DJV_CALLBACK(Style, color_background2_callback, const djv_image::Color &);
  DJV_CALLBACK(Style, color_button_callback, const djv_image::Color &);
  DJV_CALLBACK(Style, color_select_callback, const djv_image::Color &);
  DJV_CALLBACK(Style, color_highlight_callback, const djv_image::Color &);
  DJV_CALLBACK(Style, focus_callback, bool);
  DJV_CALLBACK(Style, highlight_callback, bool);
  DJV_CALLBACK(Style, size_callback, int);
  DJV_CALLBACK(Style, size_value_callback, int);
  
  void color_update();
  void size_update();

  struct _Style;
  std::auto_ptr<_Style> _p;
};

//------------------------------------------------------------------------------
//!\class General
//
//! General preferences widget.
//------------------------------------------------------------------------------

class _DJV_UI General : public widget::Widget
{
public:

  General();
  
private:

  DJV_CALLBACK(General, time_units_callback, int);
  DJV_CALLBACK(General, speed_callback, int);
  DJV_CALLBACK(General, filter_min_callback, int);
  DJV_CALLBACK(General, filter_mag_callback, int);
  DJV_CALLBACK(General, help_tooltips_callback, bool);
};

//------------------------------------------------------------------------------
//!\class Dialog
//
//! Preferences dialog.
//------------------------------------------------------------------------------

class _DJV_UI Dialog : public dialog::Base
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{
  
  Dialog();
  
  virtual ~Dialog();

  //@}
  //----------------------------------------------------------------------------
  //!\name Tabs
  //@{
  
  void add(const String & name, Widget_Base *);
  void add_shortcut(const String & name, Widget_Base *);

  //@}
  //----------------------------------------------------------------------------
  //!\name Widget
  //@{
  
  virtual void show();

  //@}
  //----------------------------------------------------------------------------
  //!\name Layout
  //@{
  
  virtual void dirty();
  
  //@}
  
private:
  
  DJV_CALLBACK(Dialog, close_callback, bool);

  struct _Dialog;
  std::auto_ptr<_Dialog> _p;
};

//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

_DJV_UI extern const String
  dialog_title;

_DJV_UI extern const String
  prefs_style_title,
  prefs_color_group,
  prefs_color_foreground,
  prefs_color_background,
  prefs_color_background2,
  prefs_color_button,
  prefs_color_select,
  prefs_color_highlight,
  prefs_focus,
  prefs_highlight,
  prefs_size_group,
  prefs_size_value,
  prefs_general_title,
  prefs_time_group,
  prefs_time_units,
  prefs_speed,
  prefs_filter_group,
  prefs_filter_text,
  prefs_filter_min,
  prefs_filter_mag,
  prefs_help_group,
  prefs_help_tooltips;

//@}
//------------------------------------------------------------------------------
//!\name Global
//------------------------------------------------------------------------------
//@{

//! Access to global dialog.

_DJV_UI Dialog * global();

//@}
//------------------------------------------------------------------------------

}
}

#endif
