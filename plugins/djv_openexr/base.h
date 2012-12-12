/*------------------------------------------------------------------------------
 plugin/djv_openexr/base.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_OPENEXR_BASE_H
#define DJV_OPENEXR_BASE_H

#include <djv_image/io.h>
#include <djv_base/box.h>
#include <ImathBox.h>
#include <ImfPixelType.h>

namespace Imf {
struct Channel;
class ChannelList;
class Header;
}

namespace djv_plugin {

//------------------------------------------------------------------------------
//!\namespace djv_plugin::openexr
//
//! OpenEXR plugin - Industrial Light & Magic OpenEXR image file format.
//!
//! Requires:
//!
//! - OpenEXR - http://www.openexr.com
//!
//! Support:
//!
//! - Image data: L, LA, RGB, RGBA, 16-bit float, 32-bit float
//! - Image layers
//! - Display/data windows
//! - File compression
//------------------------------------------------------------------------------

namespace openexr {
using namespace djv_base;
using namespace djv_image;

//------------------------------------------------------------------------------
//!\name Information
//------------------------------------------------------------------------------
//@{

static const String name = "OpenEXR";

static const List<String> extension_list = List<String>() <<
  ".exr";

//@}
//------------------------------------------------------------------------------
//!\name Image Data
//------------------------------------------------------------------------------
//@{

//! Image channel.

struct Channel
{
  Channel() :
    sampling(1)
  {}

  Channel(
    const String & name,
    pixel::TYPE type,
    const V2i & sampling = V2i(1)
  );

  String name;
  pixel::TYPE type;
  V2i sampling;
};

//! Image layer.

struct Layer
{
  Layer(const List<Channel> & = List<Channel>());
  
  String name;
  List<Channel> channel;
};

//@}
//------------------------------------------------------------------------------
//!\name Options
//------------------------------------------------------------------------------
//@{

//! Color profile.

enum COLOR_PROFILE
{
  COLOR_PROFILE_NONE,
  COLOR_PROFILE_GAMMA,
  COLOR_PROFILE_EXPOSURE,
  
  _COLOR_PROFILE_SIZE
};

//! Compression.

enum COMPRESSION
{
  COMPRESSION_NONE,
  COMPRESSION_RLE,
  COMPRESSION_ZIPS,
  COMPRESSION_ZIP,
  COMPRESSION_PIZ,
  COMPRESSION_PXR24,
  COMPRESSION_B44,
  COMPRESSION_B44A,
  
  _COMPRESSION_SIZE
};

//! Channels.

enum CHANNELS
{
  CHANNELS_GROUP_NONE,
  CHANNELS_GROUP_KNOWN,
  CHANNELS_GROUP_ALL,
  
  _CHANNELS_SIZE
};

//! Image tags.

enum TAG
{
  TAG_LONGITUDE,
  TAG_LATITUDE,
  TAG_ALTITUDE,
  TAG_FOCUS,
  TAG_EXPOSURE,
  TAG_APERTURE,
  TAG_ISO_SPEED,
  TAG_CHROMATICITIES,
  TAG_WHITE_LUMINANCE,
  TAG_X_DENSITY,
  
  _TAG_SIZE
};

//! Base options.

enum BASE_OPTION
{
  BASE_THREAD_COUNT,
  
  _BASE_OPTION_SIZE
};

struct Base_Option
{
  Base_Option() :
    thread_count(4)
  {}
  
  int thread_count;
};

//@}
//------------------------------------------------------------------------------
//!\class Base
//
//! Base plugin.
//------------------------------------------------------------------------------

class Base : public djv_image::io::Base
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Base();

  //@}
  //----------------------------------------------------------------------------
  //!\name Plugin
  //@{
  
  plugin::Plugin * copy() const;
  
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
  
private:

  void update_thread_count();

  Base_Option _option;
};

//------------------------------------------------------------------------------
//!\name Functions
//------------------------------------------------------------------------------
//@{

void openexr_init() throw (Error);
void openexr_del();

//! Create a layer name from a list of channel names.
//!
//! Example:
//!
//! - R, G, B - R,G,B
//! - normal.X, normal.Y, normal.Z - normal.X,Y,Z

String layer_name(const List<String> &);

//! Find channels that aren't in any layer.

Imf::ChannelList default_layer(const Imf::ChannelList &);

//! Find a channel by name.

const Imf::Channel * find(const Imf::ChannelList &, String &);

//! Build a list of layers from Imf channels.

List<Layer> layer(const Imf::ChannelList &, CHANNELS);

//! Image tags.

void tag_load(const Imf::Header &, io::Info &);

void tag_save(const io::Info &, Imf::Header &);

//@}
//------------------------------------------------------------------------------
//!\name Conversion
//------------------------------------------------------------------------------
//@{

//! Box conversion.

Box2i imf_to_box(const Imath::Box2i &);

//! Pixel type conversion.

Imf::PixelType pixel_type_to_imf(pixel::TYPE);
pixel::TYPE imf_to_pixel_type(Imf::PixelType);

//! Channel conversion.

Channel imf_to_channel(const String & name, const Imf::Channel &);

//! Enumeration conversion.

String & operator >> (String &, COLOR_PROFILE &) throw (String);
String & operator >> (String &, COMPRESSION &) throw (String);
String & operator >> (String &, CHANNELS &) throw (String);

String & operator << (String &, COLOR_PROFILE);
String & operator << (String &, COMPRESSION);
String & operator << (String &, CHANNELS);

//@}
//------------------------------------------------------------------------------
//!\name Comparison
//------------------------------------------------------------------------------
//@{

bool compare(const List<Imf::Channel> &);

//@}
//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

const List<String> & label_tag();
const List<String> & label_color_profile();
const List<String> & label_compression();
const List<String> & label_compression_text();
const List<String> & label_channels();
const List<String> & label_channels_text();
const List<String> & label_base_option();

extern const String
  label_thread_count_value,
  label_gamma_value,
  label_exposure_value;

//@}
//------------------------------------------------------------------------------

}
}

#endif

