/*------------------------------------------------------------------------------
 include/djv_image/io.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_IMAGE_IO_H
#define DJV_IMAGE_IO_H

#include <djv_image/io_def.h>

#include <djv_image/image.h>
#include <djv_image/time.h>
#include <djv_base/file.h>
#include <djv_base/plugin.h>

namespace djv_image {

//------------------------------------------------------------------------------
//!\namespace djv_image::io
//
//! Image I/O.
//------------------------------------------------------------------------------

namespace io {

//------------------------------------------------------------------------------
//!\class Info
//
//! Image I/O information.
//------------------------------------------------------------------------------

class _DJV_IMAGE Info : public data::Info
{
public:
  
  //----------------------------------------------------------------------------
  //!\name Construction
  //@{
  
  Info();
  
  Info(const data::Info &);
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Layers
  //!
  //! Note there is always a layer 0 that points to "this".
  //@{
  
  void layer_add(const data::Info &);
  void layer_size(list::size_type);
  void layer_clear();
  
  list::size_type layer_size() const;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Operators
  //@{
  
  data::Info & operator [] (list::size_type);
  const data::Info & operator [] (list::size_type) const;

  //@}
  
  tag::Tag tag;
  time::Time time;

private:

  List<data::Info> _info;
};

//! Frame information.

struct _DJV_IMAGE Frame_Info
{
  Frame_Info(
    int64_t frame = -1,
    int layer = 0,
    data::PROXY proxy = data::PROXY_NONE
  );
  
  int64_t frame;
  int layer;
  data::PROXY proxy;
};

//------------------------------------------------------------------------------
//!\class Base
//
//! A base class for image I/O plugins.
//------------------------------------------------------------------------------

class _DJV_IMAGE Base : public plugin::Plugin
{
public:
  
  //----------------------------------------------------------------------------
  //!\name Functions
  //@{
  
  //! List of supported file extensions.
  
  virtual List<String> extension_list() const;
  
  //! Does the plugin use file sequences?
  
  virtual bool is_seq() const;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Options
  //@{
  
  virtual bool option(const String &, String *);

  virtual String option(const String &) const;
  virtual List<String> option_list() const;
  virtual List<String> option_value(const String &) const;
  virtual String option_default(const String &) const;
  
  //@}
};

//------------------------------------------------------------------------------
//!\class Load
//
//! A base class for image load plugins.
//------------------------------------------------------------------------------

class _DJV_IMAGE Load : public Base
{
public:
  
  //! Open an image.
  
  virtual void open(
    const File &,
    Info &
  ) throw (Error) = 0;
  
  //! Load the image.
  
  virtual const Image * get(
    const Frame_Info & = Frame_Info()
  ) throw (Error) = 0;

protected:

  //! Utility class for caching a single frame.

  struct _DJV_IMAGE Frame_Cache
  {
    Frame_Cache();
    
    Frame_Cache(
      const String & file_name,
      const Frame_Info &
    );

    File file;
    
    Frame_Info info;
    
    bool operator == (const Frame_Cache &) const;
  };
};

//------------------------------------------------------------------------------
//!\class Save
//
//! A base class for image save plugins.
//------------------------------------------------------------------------------

class _DJV_IMAGE Save : public Base
{
public:

  virtual ~Save();

  //! Open an image.
  
  virtual void open(
    const File &,
    const Info &
  ) throw (Error) = 0;
  
  //! Close the image.
  
  virtual void close() throw (Error);
  
  //! Save the image.
  
  virtual void set(
    const Image &,
    const Frame_Info & = Frame_Info()
  ) throw (Error) = 0;
};

//------------------------------------------------------------------------------
//!\class Base_Factory
//
//! Image I/O plugin factory.
//------------------------------------------------------------------------------

class _DJV_IMAGE Base_Factory : public plugin::Factory
{
public:

  Base_Factory();
  
  ~Base_Factory();

  //! Plugin options.

  bool option(const String & plugin, const String &, String *);
  
  //! Plugin list.
  
  List<Base *> list() const;

private:

  String plugin_entry() const { return "djv_image_io"; }
};

//------------------------------------------------------------------------------
//!\class Load_Factory
//
//! Image load plugin factory.
//------------------------------------------------------------------------------

class _DJV_IMAGE Load_Factory : public plugin::Factory
{
public:

  Load_Factory();
  
  ~Load_Factory();
  
  //! Test if a file is an image.
  
  bool is_valid(
    const File &,
    const String & plugin = String()
  ) const;

  //! Create a new plugin.
  
  Load * get(
    const File &,
    Info * = 0,
    const String & plugin = String()
  ) const throw (Error);

  //! Plugin options.

  bool option(const String & plugin, const String &, String *);
  
  //! Plugin list.
  
  List<Load *> list() const;

private:

  String plugin_entry() const { return "djv_image_load"; }
};

//------------------------------------------------------------------------------
//!\class Save_Factory
//
//! Image save plugin factory.
//------------------------------------------------------------------------------

class _DJV_IMAGE Save_Factory : public plugin::Factory
{
public:

  Save_Factory();
  
  ~Save_Factory();

  //! Create a new plugin.
  
  Save * get(
    const File &,
    const Info &,
    const String & plugin = String()
  ) const throw (Error);

  //! Plugin options.

  bool option(const String & plugin, const String &, String *);
  
  //! Plugin list.
  
  List<Save *> list() const;

private:

  String plugin_entry() const { return "djv_image_save"; }
};

//------------------------------------------------------------------------------
//!\name Functions
//------------------------------------------------------------------------------
//@{

//! Errors.

_DJV_IMAGE void throw_error(
  const String & plugin,
  const String & error
) throw (Error);

_DJV_IMAGE void throw_error_unrecognized(
  const String & plugin,
  const String & file_name
) throw (Error);

_DJV_IMAGE void throw_error_unsupported(
  const String & plugin,
  const String & file_name
) throw (Error);

_DJV_IMAGE void throw_error_open(
  const String & plugin,
  const String & file_name
) throw (Error);

_DJV_IMAGE void throw_error_read(
  const String & plugin,
  const String & file_name
) throw (Error);

_DJV_IMAGE void throw_error_write(
  const String & plugin,
  const String & file_name
) throw (Error);

//@}
//------------------------------------------------------------------------------
//!\name Comparison
//------------------------------------------------------------------------------
//@{

_DJV_IMAGE bool operator == (const Info &, const Info &);
_DJV_IMAGE bool operator == (const Frame_Info &, const Frame_Info &);

_DJV_IMAGE bool operator != (const Info &, const Info &);
_DJV_IMAGE bool operator != (const Frame_Info &, const Frame_Info &);

//@}
//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

_DJV_IMAGE extern const String
  error,
  error_plugin,
  error_unrecognized,
  error_unsupported,
  error_open,
  error_read,
  error_write;

//@}
//------------------------------------------------------------------------------
//!\name Debugging
//------------------------------------------------------------------------------
//@{

_DJV_IMAGE Debug & operator << (Debug &, const Info &);
_DJV_IMAGE Debug & operator << (Debug &, const Frame_Info &);

//@}
//------------------------------------------------------------------------------
//!\name Global
//------------------------------------------------------------------------------
//@{

//! Global options.

class _DJV_IMAGE Global
{
public:

  Global();
  
  ~Global();
  
  bool is_valid(
    const File &,
    const String & plugin = String()
  ) const;

  Load * load(
    const File &,
    Info * = 0,
    const String & plugin = String()
  ) const throw (Error);

  Save * save(
    const File &,
    const Info &,
    const String & plugin = String()
  ) const throw (Error);

  Base_Factory * base() const;
  Load_Factory * load() const;
  Save_Factory * save() const;
  
  List<Base *> list() const;
  List<Base *> base_list() const;
  List<Base *> load_list() const;
  List<Base *> save_list() const;
  
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

_DJV_IMAGE Global * global();

_DJV_IMAGE void group_name(
  const List<plugin::Plugin *> &,
  List<String> &
);

_DJV_IMAGE plugin::Plugin * group_find(
  const String & name,
  const List<plugin::Plugin *> &
);

//@}
//------------------------------------------------------------------------------

}
}

#endif

