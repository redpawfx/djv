/*------------------------------------------------------------------------------
 include/djv_image/io_cineon.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_IMAGE_IO_CINEON_H
#define DJV_IMAGE_IO_CINEON_H

#include <djv_image/io.h>
#include <djv_base/file_io.h>

namespace djv_image {

//------------------------------------------------------------------------------
//!\namespace djv_image::io_cineon
//
//! Cineon plugin - Kodak Cineon image file format.
//!
//! Cineon is a specialized file format for storing images scanned from
//! motion-picture film.
//!
//! Support:
//! 
//! - Image data: RGB, 10-bit (the most common variety)
//! - Interleaved channels only
//!
//! References:
//! 
//! - Kodak, "4.5 DRAFT - Image File Format Proposal for Digital Pictures"
//------------------------------------------------------------------------------

namespace io_cineon {

//------------------------------------------------------------------------------
//!\name Information
//------------------------------------------------------------------------------
//@{

static const String name = "Cineon";

static const List<String> extension_list = List<String>() <<
  ".cin";

//@}
//------------------------------------------------------------------------------
//!\name Options
//------------------------------------------------------------------------------
//@{

//! Color profile.

enum COLOR_PROFILE
{
  COLOR_PROFILE_AUTO,
  COLOR_PROFILE_RAW,
  COLOR_PROFILE_FILM_PRINT,
  
  _COLOR_PROFILE_SIZE
};

//! Film print options.
//! 
//! - Black point = 0 - 1023, Default = 95
//! - White point = 0 - 1023, Default = 685
//! - Gamma = 0.01 - 4.0, Default = 1.7
//! - Soft clip = 0 - 50, Default = 0

struct _DJV_IMAGE Film_Print
{
  Film_Print() :
    black(95),
    white(685),
    gamma(1.7)
  {}

  int black, white;
  double gamma;
};

struct _DJV_IMAGE Film_Unprint
{
  Film_Unprint() :
    black(95),
    white(685),
    gamma(1.7),
    soft_clip(0)
  {}

  int black, white;
  double gamma;
  int soft_clip;
};

//! Convert bit-depth.

enum CONVERT
{
  CONVERT_NONE,
  CONVERT_U8,
  CONVERT_U16,
  
  _CONVERT_SIZE
};

//! Image tags.

enum TAG
{
  TAG_SOURCE_OFFSET,
  TAG_SOURCE_FILE,
  TAG_SOURCE_TIME,
  TAG_SOURCE_INPUT_DEVICE,
  TAG_SOURCE_INPUT_MODEL,
  TAG_SOURCE_INPUT_SERIAL,
  TAG_SOURCE_INPUT_PITCH,
  TAG_SOURCE_GAMMA,
  TAG_FILM_FORMAT,
  TAG_FILM_FRAME,
  TAG_FILM_FRAME_RATE,
  TAG_FILM_FRAME_ID,
  TAG_FILM_SLATE,
  
  _TAG_SIZE
};

//! Load options.

enum LOAD_OPTION
{
  LOAD_COLOR_PROFILE,
  LOAD_FILM_PRINT,
  LOAD_CONVERT,
  
  _LOAD_OPTION_SIZE
};

struct _DJV_IMAGE Load_Option
{
  Load_Option() :
    color_profile(COLOR_PROFILE_AUTO),
    convert(CONVERT_NONE)
  {}

  COLOR_PROFILE color_profile;
  
  Film_Unprint film_print;
  
  CONVERT convert;
};

//! Save options.

enum SAVE_OPTION
{
  SAVE_COLOR_PROFILE,
  SAVE_FILM_PRINT,
  
  _SAVE_OPTION_SIZE
};

struct _DJV_IMAGE Save_Option
{
  Save_Option() :
    color_profile(COLOR_PROFILE_FILM_PRINT)
  {}

  COLOR_PROFILE color_profile;
  
  Film_Print film_print;
};

//------------------------------------------------------------------------------
//!\class Header
//
//! File header.
//------------------------------------------------------------------------------

class _DJV_IMAGE Header
{
public:

  static const uint32_t magic [];

  static const float speed_min;

  //! Image orientation.
  
  enum ORIENT
  {
    ORIENT_LEFT_RIGHT_TOP_BOTTOM,
    ORIENT_LEFT_RIGHT_BOTTOM_TOP,
    ORIENT_RIGHT_LEFT_TOP_BOTTOM,
    ORIENT_RIGHT_LEFT_BOTTOM_TOP,
    ORIENT_TOP_BOTTOM_LEFT_RIGHT,
    ORIENT_TOP_BOTTOM_RIGHT_LEFT,
    ORIENT_BOTTOM_TOP_LEFT_RIGHT,
    ORIENT_BOTTOM_TOP_RIGHT_LEFT
  };

  //! Descriptor.
  
  enum DESCRIPTOR
  {
    DESCRIPTOR_L = 0,
    DESCRIPTOR_R_FILM_PRINT = 1,
    DESCRIPTOR_G_FILM_PRINT = 2,
    DESCRIPTOR_B_FILM_PRINT = 3,
    DESCRIPTOR_R_CCIR_XA11 = 4,
    DESCRIPTOR_G_CCIR_XA11 = 5,
    DESCRIPTOR_B_CCIR_XA11 = 6
  };

  //! File information.
  
  struct File
  {
    uint32_t magic;
    uint32_t image_offset;
    uint32_t header_size;
    uint32_t industry_header_size;
    uint32_t user_header_size;
    uint32_t size;
    char version [8];
    char name [100];
    char time [24];
    uint8_t _pad [36];
  
  } file;
  
  //! Image information.
  
  struct Image
  {
    uint8_t orient;
    uint8_t channels;
    uint8_t _pad [2];

    //! Channel information.
    
    struct Channel
    {
      uint8_t descriptor [2];
      uint8_t bit_depth;
      uint8_t _pad;
      uint32_t size [2];
      float low_data;
      float low_quantity;
      float high_data;
      float high_quantity;
    
    } channel [8];
    
    float white [2];
    float red [2];
    float green [2];
    float blue [2];
    char label [200];
    uint8_t _pad2 [28];
    uint8_t interleave;
    uint8_t packing;
    uint8_t data_sign;
    uint8_t data_sense;
    uint32_t line_padding;
    uint32_t channel_padding;
    uint8_t _pad3 [20];
  
  } image;
  
  //! Source information.
  
  struct Source
  {
    int32_t offset [2];
    char file [100];
    char time [24];
    char input_device [64];
    char input_model [32];
    char input_serial [32];
    float input_pitch [2];
    float gamma;
    char _pad [40];
  
  } source;
  
  //! Film information.
  
  struct Film
  {
    uint8_t id;
    uint8_t type;
    uint8_t offset;
    uint8_t _pad;
    uint32_t prefix;
    uint32_t count;
    char format [32];
    uint32_t frame;
    float frame_rate;
    char frame_id [32];
    char slate [200];
    char _pad2 [740];
  
  } film;

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{
  
  Header();
  
  //@}
  //----------------------------------------------------------------------------
  //!\name I/O Functions
  //@{
  
  void load(
    File_Io &,
    io::Info &,
    bool & film_print
  ) throw (Error);
  
  void save(
    File_Io &,
    const io::Info &,
    COLOR_PROFILE
  ) throw (Error);
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Utility Functions
  //@{
    
  static void zero(int32_t *);
  static void zero(float *);
  static void zero(char *, int size);

  static bool is_valid(const uint8_t *);
  static bool is_valid(const uint16_t *);
  static bool is_valid(const uint32_t *);
  static bool is_valid(const int32_t *);
  static bool is_valid(const float *);
  static bool is_valid(const char *, int size);

  static String to_string(const char *, int size);
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Debugging
  //@{
  
  String debug() const;

  static String debug(uint8_t);
  static String debug(uint16_t);
  static String debug(uint32_t);
  static String debug(int32_t);
  static String debug(float);
  static String debug(const char *, int size);

  //@}
  
private:

  void endian();
};

//------------------------------------------------------------------------------
//!\class Load
//
//! Load plugin.
//------------------------------------------------------------------------------

class _DJV_IMAGE Load : public io::Load
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Load();

  //@}
  //----------------------------------------------------------------------------
  //!\name Plugin
  //@{
  
  Plugin * copy() const;
  
  String name() const;
  
  List<String> extension_list() const;

  //@}
  //----------------------------------------------------------------------------
  //!\name Options
  //@{

  bool option(const String &, String *);
  
  String option(const String &) const;
  List<String> option_list() const;
  List<String> option_value(const String &) const;
  String option_default(const String &) const;

  //@}
  //----------------------------------------------------------------------------
  //!\name Load
  //@{
  
  void open(const File &, io::Info &) throw (Error);
  
  const Image * get(const io::Frame_Info &) throw (Error);

  //@}
  
private:

  void _open(const String &, io::Info &) throw (Error);

  Load_Option _option;
  bool _film_print;
  std::auto_ptr<Data> _film_print_lut;
  
  File _file;
  File_Io _io;
  
  Image _image;
  Data _tmp;
  
  Frame_Cache _frame_cache;
};

//------------------------------------------------------------------------------
//!\class Save
//
//! Save plugin.
//------------------------------------------------------------------------------

class _DJV_IMAGE Save : public io::Save
{
public:

  //----------------------------------------------------------------------------
  //!\name Plugin
  //@{
  
  Plugin * copy() const;
  
  String name() const;
  
  List<String> extension_list() const;

  //@}
  //----------------------------------------------------------------------------
  //!\name Options
  //@{
  
  bool option(const String &, String *);
  
  String option(const String &) const;
  List<String> option_list() const;
  List<String> option_value(const String &) const;
  String option_default(const String &) const;

  //@}
  //----------------------------------------------------------------------------
  //!\name Save
  //@{
  
  void open(const File &, const io::Info &) throw (Error);
  
  void set(const Image &, const io::Frame_Info &) throw (Error);

  //@}
  
private:

  void _open(const String &, const io::Info &) throw (Error);

  Save_Option _option;
  File _file;
  File_Io _io;
  Info _info;
  Image _image;
};

//------------------------------------------------------------------------------
//!\name Functions
//------------------------------------------------------------------------------
//@{

_DJV_IMAGE void info_load(File_Io &, io::Info &, bool & film_print)
  throw (Error);

_DJV_IMAGE void info_save(File_Io &, const io::Info &, COLOR_PROFILE)
  throw (Error);

_DJV_IMAGE void info_update(File_Io &) throw (Error);

_DJV_IMAGE void data_load_u8(File_Io &, Data *) throw (Error);

_DJV_IMAGE void film_print(pixel::F32_T *, int size, const Film_Print &);
_DJV_IMAGE void film_unprint(pixel::F32_T *, int size, const Film_Unprint &);

_DJV_IMAGE Data film_print_lut(const Film_Print &);
_DJV_IMAGE Data film_unprint_lut(const Film_Unprint &);

//@}
//------------------------------------------------------------------------------
//!\name Comparison
//------------------------------------------------------------------------------
//@{

_DJV_IMAGE bool operator == (const Film_Print &, const Film_Print &);
_DJV_IMAGE bool operator == (const Film_Unprint &, const Film_Unprint &);

//@}
//------------------------------------------------------------------------------
//!\name Conversion
//------------------------------------------------------------------------------
//@{

_DJV_IMAGE String & operator >> (String &, Film_Print &) throw (String);
_DJV_IMAGE String & operator >> (String &, Film_Unprint &) throw (String);
_DJV_IMAGE String & operator >> (String &, COLOR_PROFILE &) throw (String);
_DJV_IMAGE String & operator >> (String &, CONVERT &) throw (String);

_DJV_IMAGE String & operator << (String &, const Film_Print &);
_DJV_IMAGE String & operator << (String &, const Film_Unprint &);
_DJV_IMAGE String & operator << (String &, COLOR_PROFILE);
_DJV_IMAGE String & operator << (String &, CONVERT);

//@}
//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

_DJV_IMAGE const List<String> & label_tag();
_DJV_IMAGE const List<String> & label_color_profile();
_DJV_IMAGE const List<String> & label_convert();
_DJV_IMAGE const List<String> & label_load_option();
_DJV_IMAGE const List<String> & label_save_option();

_DJV_IMAGE extern const String
  label_film_print_load,
  label_film_print_save;

//@}
//------------------------------------------------------------------------------

}
}

#endif

