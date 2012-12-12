/*------------------------------------------------------------------------------
 include/djv_image/io_dpx.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_IMAGE_IO_DPX_H
#define DJV_IMAGE_IO_DPX_H

#include <djv_image/io_cineon.h>

namespace djv_image {

//------------------------------------------------------------------------------
//!\namespace djv_image::io_dpx
//
//! DPX plugin - SMPTE Digital Picture Exchange image file format.
//!
//! DPX is the successor to the Cineon file format with support for additional
//! image and meta data.
//!
//! Support:
//! 
//! - Image data:
//!   - RGB, 10-bit, type "A" packing (the most common variety)
//!   - L, RGB, RGBA, 8-bit, 16-bit
//! - Interleaved channels only
//!
//! References:
//! 
//! - SMPTE, "SMPTE 268M-2003"
//!   http://www.smpte.org
//! - Cinesite, "Conversion of 10-bit Log Film Data To 8-bit Linear or Video
//!   Data"
//------------------------------------------------------------------------------

namespace io_dpx {

//------------------------------------------------------------------------------
//!\name Information
//------------------------------------------------------------------------------
//@{

static const String name = "DPX";

static const List<String> extension_list = List<String>() <<
  ".dpx";

//@}
//------------------------------------------------------------------------------
//!\name Options
//------------------------------------------------------------------------------
//@{

//! File format version.

enum VERSION
{
  VERSION_1_0,
  VERSION_2_0,
  
  _VERSION_SIZE
};

//! File type.

enum TYPE
{
  TYPE_AUTO,
  TYPE_U10,
  
  _TYPE_SIZE
};

//! File endian.

enum ENDIAN
{
  ENDIAN_AUTO,
  ENDIAN_MSB,
  ENDIAN_LSB,
  
  _ENDIAN_SIZE
};

//! Image tags.

enum TAG
{
  TAG_SOURCE_OFFSET,
  TAG_SOURCE_CENTER,
  TAG_SOURCE_SIZE,
  TAG_SOURCE_FILE,
  TAG_SOURCE_TIME,
  TAG_SOURCE_INPUT_DEVICE,
  TAG_SOURCE_INPUT_SERIAL,
  TAG_SOURCE_BORDER,
  TAG_SOURCE_PIXEL_ASPECT,
  TAG_SOURCE_SCAN_SIZE,
  TAG_FILM_FORMAT,
  TAG_FILM_FRAME,
  TAG_FILM_SEQUENCE,
  TAG_FILM_HOLD,
  TAG_FILM_FRAME_RATE,
  TAG_FILM_SHUTTER,
  TAG_FILM_FRAME_ID,
  TAG_FILM_SLATE,
  TAG_TV_INTERLACE,
  TAG_TV_FIELD,
  TAG_TV_VIDEO_SIGNAL,
  TAG_TV_SAMPLE_RATE,
  TAG_TV_FRAME_RATE,
  TAG_TV_TIME_OFFSET,
  TAG_TV_GAMMA,
  TAG_TV_BLACK_LEVEL,
  TAG_TV_BLACK_GAIN,
  TAG_TV_BREAK_POINT,
  TAG_TV_WHITE_LEVEL,
  TAG_TV_INTEGRATION_TIMES,
  
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
  Load_Option();

  io_cineon::COLOR_PROFILE color_profile;
  
  io_cineon::Film_Unprint film_print;
  
  io_cineon::CONVERT convert;
};

//! Save options.

enum SAVE_OPTION
{
  SAVE_COLOR_PROFILE,
  SAVE_FILM_PRINT,
  SAVE_VERSION,
  SAVE_TYPE,
  SAVE_ENDIAN,
  
  _SAVE_OPTION_SIZE
};

struct _DJV_IMAGE Save_Option
{
  Save_Option();

  io_cineon::COLOR_PROFILE color_profile;
  
  io_cineon::Film_Print film_print;
  
  VERSION version;
  
  TYPE type;
  
  ENDIAN endian;
};

//------------------------------------------------------------------------------
//!\class Header
//
//! File header.
//------------------------------------------------------------------------------

class _DJV_IMAGE Header
{
public:

  static const char magic [][5];

  //! Image orientation.
  
  enum ORIENT
  {
    ORIENT_LEFT_RIGHT_TOP_BOTTOM,
    ORIENT_RIGHT_LEFT_TOP_BOTTOM,
    ORIENT_LEFT_RIGHT_BOTTOM_TOP,
    ORIENT_RIGHT_LEFT_BOTTOM_TOP,
    ORIENT_TOP_BOTTOM_LEFT_RIGHT,
    ORIENT_TOP_BOTTOM_RIGHT_LEFT,
    ORIENT_BOTTOM_TOP_LEFT_RIGHT,
    ORIENT_BOTTOM_TOP_RIGHT_LEFT
  };

  //! Descriptor.
  
  enum DESCRIPTOR
  {
    DESCRIPTOR_USER = 0,
    DESCRIPTOR_R = 1,
    DESCRIPTOR_G = 2,
    DESCRIPTOR_B = 3,
    DESCRIPTOR_A = 4,
    DESCRIPTOR_L = 6,
    DESCRIPTOR_CHROMA = 7,
    DESCRIPTOR_Z = 8,
    DESCRIPTOR_COMPOSITE_VIDEO = 9,
    DESCRIPTOR_RGB = 50,
    DESCRIPTOR_RGBA = 51,
    DESCRIPTOR_ABGR = 52,
    DESCRIPTOR_CBYCRY = 100,
    DESCRIPTOR_CBYACRYA = 101,
    DESCRIPTOR_CBYCR = 102,
    DESCRIPTOR_CBYCRA = 103,
    DESCRIPTOR_USER_2 = 150,
    DESCRIPTOR_USER_3 = 151,
    DESCRIPTOR_USER_4 = 152,
    DESCRIPTOR_USER_5 = 153,
    DESCRIPTOR_USER_6 = 154,
    DESCRIPTOR_USER_7 = 155,
    DESCRIPTOR_USER_8 = 156
  };
  
  //! Transfer.

  enum TRANSFER
  {
    TRANSFER_USER,
    TRANSFER_FILM_PRINT,
    TRANSFER_LINEAR,
    TRANSFER_LOG,
    TRANSFER_VIDEO,
    TRANSFER_SMPTE_274M,
    TRANSFER_ITU_R_709_4,
    TRANSFER_ITU_R_601_5_B_OR_G,
    TRANSFER_ITU_R_601_5_M,
    TRANSFER_NTSC,
    TRANSFER_PAL,
    TRANSFER_Z,
    TRANSFER_Z_HOMOGENEOUS
  };
  
  //! Colorimetric vesion 1.0.

  enum COLORIMETRIC_1_0
  {
    COLORIMETRIC_1_0_USER = 0,
    COLORIMETRIC_1_0_FILM_PRINT = 1,
    COLORIMETRIC_1_0_VIDEO = 4,
    COLORIMETRIC_1_0_SMPTE_240M = 5,
    COLORIMETRIC_1_0_ITU_R_709_1 = 6,
    COLORIMETRIC_1_0_ITU_R_601_2_B_OR_G = 7,
    COLORIMETRIC_1_0_ITU_R_601_2_M = 8,
    COLORIMETRIC_1_0_NTSC = 9,
    COLORIMETRIC_1_0_PAL = 10
  };

  //! Colorimetric vesion 2.0.

  enum COLORIMETRIC_2_0
  {
    COLORIMETRIC_2_0_USER = 0,
    COLORIMETRIC_2_0_FILM_PRINT = 1,
    COLORIMETRIC_2_0_VIDEO = 4,
    COLORIMETRIC_2_0_SMPTE_274M = 5,
    COLORIMETRIC_2_0_ITU_R_709_4 = 6,
    COLORIMETRIC_2_0_ITU_R_601_5_B_OR_G = 7,
    COLORIMETRIC_2_0_ITU_R_601_5_M = 8,
    COLORIMETRIC_2_0_NTSC = 9,
    COLORIMETRIC_2_0_PAL = 10
  };
  
  //! Components.

  enum COMPONENTS
  {
    PACK,
    TYPE_A,
    TYPE_B
  };
  
  //! File information.
  
  struct File
  {
    uint32_t magic;
    uint32_t image_offset;
    char version [8];
    uint32_t size;
    uint32_t ditto_key;
    uint32_t header_size;
    uint32_t industry_header_size;
    uint32_t user_header_size;
    char name [100];
    char time [24];
    char creator [100];
    char project [200];
    char copyright [200];
    uint32_t encryption_key;
    
    uint8_t _pad [104];
  
  } file;
  
  //! Image information.
  
  struct Image
  {
    uint16_t orient;
    uint16_t elem_size;
    uint32_t size [2];
  
    //! Image element information.
    
    struct Elem
    {
      uint32_t data_sign;
      uint32_t low_data;
      float low_quantity;
      uint32_t high_data;
      float high_quantity;
      uint8_t descriptor;
      uint8_t transfer;
      uint8_t colorimetric;
      uint8_t bit_depth;
      uint16_t packing;
      uint16_t encoding;
      uint32_t data_offset;
      uint32_t line_padding;
      uint32_t elem_padding;
      char description [32];
    
    } elem [8];

    uint8_t _pad [52];
  
  } image;
  
  //! Source information.

  struct Source
  {
    uint32_t offset [2];
    float center [2];
    uint32_t size [2];
    char file [100];
    char time [24];
    char input_device [32];
    char input_serial [32];
    uint16_t border [4];
    uint32_t pixel_aspect [2];
    
    float scan_size [2]; // V2.0
    
    uint8_t _pad [20];
    
  } source;
  
  //! Film information.

  struct Film
  {
    char id [2];
    char type [2];
    char offset [2];
    char prefix [6];
    char count [4];
    char format [32];
    uint32_t frame;
    uint32_t sequence;
    uint32_t hold;
    float frame_rate;
    float shutter;
    char frame_id [32];
    char slate [100];
    
    uint8_t _pad [56];
    
  } film;
  
  //! TV information.

  struct Tv
  {
    uint32_t timecode;
    uint32_t user_bits;
    uint8_t interlace;
    uint8_t field;
    uint8_t video_signal;
    
    uint8_t _pad;
    
    float sample_rate [2];
    float frame_rate;
    float time_offset;
    float gamma;
    float black_level;
    float black_gain;
    float breakpoint;
    float white_level;
    float integration_times;
    
    uint8_t _pad2 [76];
    
  } tv;

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
    ENDIAN,
    io_cineon::COLOR_PROFILE,
    VERSION
  ) throw (Error);
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Utility Functions
  //@{
  
  static void zero(char *, int size);

  static bool is_valid(const uint8_t *);
  static bool is_valid(const uint16_t *);
  static bool is_valid(const uint32_t *);
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

_DJV_IMAGE void info_load(
  File_Io &,
  io::Info &,
  bool & film_print
) throw (Error);

_DJV_IMAGE void info_save(
  File_Io &,
  const io::Info &,
  ENDIAN,
  io_cineon::COLOR_PROFILE,
  VERSION
) throw (Error);

_DJV_IMAGE void info_update(File_Io &) throw (Error);

//@}
//------------------------------------------------------------------------------
//!\name Conversion
//------------------------------------------------------------------------------
//@{

_DJV_IMAGE String & operator >> (String &, VERSION &) throw (String);
_DJV_IMAGE String & operator >> (String &, TYPE &) throw (String);
_DJV_IMAGE String & operator >> (String &, ENDIAN &) throw (String);

_DJV_IMAGE String & operator << (String &, VERSION);
_DJV_IMAGE String & operator << (String &, TYPE);
_DJV_IMAGE String & operator << (String &, ENDIAN);

//@}
//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

_DJV_IMAGE const List<String> & label_tag();
_DJV_IMAGE const List<String> & label_version();
_DJV_IMAGE const List<String> & label_type();
_DJV_IMAGE const List<String> & label_type_text();
_DJV_IMAGE const List<String> & label_endian();
_DJV_IMAGE const List<String> & label_endian_text();
_DJV_IMAGE const List<String> & label_load_option();
_DJV_IMAGE const List<String> & label_save_option();

//@}
//------------------------------------------------------------------------------

}
}

#endif

