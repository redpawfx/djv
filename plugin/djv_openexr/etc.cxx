/*------------------------------------------------------------------------------
 plugin/djv_openexr/etc.cxx
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "base.h"

#include "load.h"
#include "save.h"
#include <djv_image/image_util.h>
#include <ImfChannelList.h>
#include <ImfFramesPerSecond.h>
#include <ImfStandardAttributes.h>
#include <ImfThreading.h>

namespace djv_plugin {
namespace openexr {

//------------------------------------------------------------------------------
// Channel
//------------------------------------------------------------------------------

Channel::Channel(
  const String & name,
  pixel::TYPE type,
  const V2i & sampling
) :
  name(name),
  type(type),
  sampling(sampling)
{}

//------------------------------------------------------------------------------
// Layer
//------------------------------------------------------------------------------

Layer::Layer(const List<Channel> & channel) :
  channel(channel)
{
  List<String> name_list;
  for (list::size_type i = 0; i < channel.size(); ++i)
    name_list += channel[i].name;
  
  name = layer_name(name_list);
}

//------------------------------------------------------------------------------
// openexr_init(), openexr_del()
//------------------------------------------------------------------------------

namespace {

int _ref = 0;

}

void openexr_init() throw (Error)
{
  ++_ref;
  if (_ref > 1)
    return;

  DJV_DEBUG("openexr_init");
}

void openexr_del()
{
  --_ref;
  if (_ref)
    return;
  
  DJV_DEBUG("openexr_del");

#if defined(DJV_WINDOWS)

  // XXX

  Imf::setGlobalThreadCount(0);

#endif // DJV_WINDOWS
}
    
//------------------------------------------------------------------------------
// layer_name()
//------------------------------------------------------------------------------

String layer_name(const List<String> & in)
{  
  String out;
  
  // Split into a prefix and suffix.
  
  Set<String> prefix_set;
  List<String> suffix_list;
  
  for (list::size_type i = 0; i < in.size(); ++i)
  {
    const String & name = in[i];
    
    const String::size_type index = name.rfind('.');
    if (index != String::npos && index != 0 && index != name.size() - 1)
    {
      prefix_set += (String(name, 0, index));
      suffix_list += String(name, index + 1, name.size() - index - 1);
    }
    else
    {
      prefix_set += name;
    }
  }
  
  // Join pieces.
  
  out = string::join(prefix_set, ",");
  
  if (suffix_list.size())
    out += "." + string::join(suffix_list, ",");

  return out;
}

//------------------------------------------------------------------------------
// default_layer()
//------------------------------------------------------------------------------

Imf::ChannelList default_layer(const Imf::ChannelList & in)
{
  Imf::ChannelList out;
  
  for (
    Imf::ChannelList::ConstIterator i = in.begin();
    i != in.end();
    ++i
  ) {
    const String tmp = i.name();
    
    const String::size_type index = tmp.find('.');
    
    if (index != String::npos)
      if (index != 0 || index != tmp.size() - 1)
        continue;

    out.insert(i.name(), i.channel());
  }
  
  return out;
}

//------------------------------------------------------------------------------
// find()
//------------------------------------------------------------------------------

const Imf::Channel * find(const Imf::ChannelList & in, String & channel)
{
  //DJV_DEBUG("find");
  //DJV_DEBUG_PRINT("channel = " << channel);
  
  const String channel_lower = string::lower(channel);
  for (
    Imf::ChannelList::ConstIterator i = in.begin();
    i != in.end();
    ++i
  ) {
    const String & in_name = i.name();
    
    const String::size_type index = in_name.rfind('.');
    
    const String tmp =
      (index != String::npos) ?
      String(in_name, index + 1, in_name.size() - index - 1) :
      in_name;
    
    if (channel_lower == string::lower(tmp))
    {
      //DJV_DEBUG_PRINT("found = " << in_name);
      channel = in_name;
      return &i.channel();
    }
  }
  
  return 0;
}

//------------------------------------------------------------------------------
// layer()
//------------------------------------------------------------------------------

namespace {

List<Layer> _layer(const Imf::ChannelList & in, CHANNELS channels)
{
  DJV_DEBUG("_layer");
  
  List<Layer> out;
  
  List<const Imf::Channel *> reserved_list;
  
  if (channels != CHANNELS_GROUP_NONE)
  {
    // Look for known channel configurations then convert the remainder.

    // RGB / RGBA.

    String r_name = "r";
    String g_name = "g";
    String b_name = "b";
    String a_name = "a";
    
    const Imf::Channel * r = find (in, r_name);
    const Imf::Channel * g = find (in, g_name);
    const Imf::Channel * b = find (in, b_name);
    const Imf::Channel * a = find (in, a_name);
  
    if (
      r && g && b && a &&
      compare(List<Imf::Channel>() << *r << *g << *b << *a)
    ) {
      out += Layer(List<Channel>() <<
        imf_to_channel(r_name, *r) <<
        imf_to_channel(g_name, *g) <<
        imf_to_channel(b_name, *b) <<
        imf_to_channel(a_name, *a));
      
      DJV_DEBUG_PRINT("rgba = " << out[out.size() - 1].name);

      reserved_list += List<const Imf::Channel *>() <<
        r << g << b << a;
    }
    else if (
      r && g && b &&
      compare(List<Imf::Channel>() << *r << *g << *b)
    ) {
      out += Layer(List<Channel>() <<
        imf_to_channel(r_name, *r) <<
        imf_to_channel(g_name, *g) <<
        imf_to_channel(b_name, *b));
      
      DJV_DEBUG_PRINT("rgb = " << out[out.size() - 1].name);

      reserved_list += List<const Imf::Channel *>() <<
        r << g << b;
    }
  
    // Luminance, XYZ.
  
    String y_name = "y";
    String ry_name = "ry";
    String by_name = "by";
    String x_name = "x";
    String z_name = "z";
    
    const Imf::Channel * y = find (in, y_name);
    const Imf::Channel * ry = find (in, ry_name);
    const Imf::Channel * by = find (in, by_name);
    const Imf::Channel * x = find (in, x_name);
    const Imf::Channel * z = find (in, z_name);
  
    if (
      y && ry && by &&
      compare(List<Imf::Channel>() << *y << *ry << *by)
    ) {
      out += Layer(List<Channel>() <<
        imf_to_channel(y_name, *y) <<
        imf_to_channel(ry_name, *ry) <<
        imf_to_channel(by_name, *by));
      
      DJV_DEBUG_PRINT("yc = " << out[out.size() - 1].name);

      reserved_list += List<const Imf::Channel *>() <<
        y << ry << by;
    }
    else if (
      x && y && z &&
      compare(List<Imf::Channel>() << *x << *y << *z)
    ) {
      out += Layer(List<Channel>() <<
        imf_to_channel(x_name, *x) <<
        imf_to_channel(y_name, *y) <<
        imf_to_channel(z_name, *z));
      
      DJV_DEBUG_PRINT("xyz = " << out[out.size() - 1].name);

      reserved_list += List<const Imf::Channel *>() <<
        x << y << z;
    }
    else if (
      x && y &&
      compare(List<Imf::Channel>() << *x << *y)
    ) {
      out += Layer(List<Channel>() <<
        imf_to_channel(x_name, *x) <<
        imf_to_channel(y_name, *y));
      
      DJV_DEBUG_PRINT("xy = " << out[out.size() - 1].name);

      reserved_list += List<const Imf::Channel *>() <<
        x << y;
    }
    else if (x)
    {
      out += Layer(List<Channel>() <<
        imf_to_channel(x_name, *x));
      
      DJV_DEBUG_PRINT("x = " << out[out.size() - 1].name);

      reserved_list += List<const Imf::Channel *>() <<
        x;
    }
    else if (y)
    {
      out += Layer(List<Channel>() <<
        imf_to_channel(y_name, *y));
      
      DJV_DEBUG_PRINT("y = " << out[out.size() - 1].name);

      reserved_list += List<const Imf::Channel *>() <<
        y;
    }
    else if (z)
    {
      out += Layer(List<Channel>() <<
        imf_to_channel(z_name, *z));
      
      DJV_DEBUG_PRINT("z = " << out[out.size() - 1].name);

      reserved_list += List<const Imf::Channel *>() <<
        z;
    }
  
    // Colored mattes.
  
    String ar_name = "ar";
    String ag_name = "ag";
    String ab_name = "ab";
    
    const Imf::Channel * ar = find (in, ar_name);
    const Imf::Channel * ag = find (in, ag_name);
    const Imf::Channel * ab = find (in, ab_name);
  
    if (
      ar && ag && ab &&
      compare(List<Imf::Channel>() << *ar << *ag << *ab)
    ) {
      out += Layer(List<Channel>() <<
        imf_to_channel(ar_name, *ar) <<
        imf_to_channel(ag_name, *ag) <<
        imf_to_channel(ab_name, *ab));
      
      DJV_DEBUG_PRINT("matte = " << out[out.size() - 1].name);

      reserved_list += List<const Imf::Channel *>() <<
        ar << ag << ab;
    }
  }
  
  // Convert the remainder.

  //DJV_DEBUG_PRINT("reserved list = " << reserved_list.size());
  
  for (
    Imf::ChannelList::ConstIterator i = in.begin();
    i != in.end();
  ) {
    List<Channel> list;

    // Add the first channel.
    
    const String & name = i.name();
    const Imf::Channel & channel = i.channel();
    ++i;
    if (list::find(&channel, reserved_list) != -1)
      continue;
    list += imf_to_channel(name, channel);
    
    if (CHANNELS_GROUP_ALL == channels)
    {
      // Group as many additional channels as possible.

      for (
        ;
        i != in.end() &&
        i.channel() == channel &&
        list::find(&i.channel(), reserved_list) == -1;
        ++i
      )
        list += imf_to_channel(i.name(), i.channel());
    }
    
    // Add the layer.
    
    out += Layer(list);
    
    DJV_DEBUG_PRINT("layer = " << out[out.size() - 1].name);
  }

  return out;
}

}

List<Layer> layer(const Imf::ChannelList & in, CHANNELS channels)
{
  DJV_DEBUG("layer");
  
  List<Layer> out;
  
  // Default layer.
  
  out += _layer(default_layer(in), channels);
  
  // Additional layers.
  
  std::set<std::string> layer_list;
  in.layers(layer_list);
  
  for (
    std::set<std::string>::const_iterator i = layer_list.begin();
    i != layer_list.end();
    ++i
  ) {
    Imf::ChannelList list;
    Imf::ChannelList::ConstIterator f, l;
    in.channelsInLayer(*i, f, l);
    for (Imf::ChannelList::ConstIterator j = f; j != l; ++j)
      list.insert(j.name(), j.channel());
    out += _layer(list, channels);
  }
  
  for (list::size_type i = 0; i < out.size(); ++i)
    DJV_DEBUG_PRINT("layer[" << static_cast<int>(i) << "] = " << out[i].name);
   
  return out;
}

//------------------------------------------------------------------------------
// tag_load()
//------------------------------------------------------------------------------

void tag_load(const Imf::Header & in, io::Info & out)
{
  using namespace djv_image::tag;
  using string::float_to_string;
  
  const List<String> & openexr = openexr::label_tag();
  const List<String> & image = djv_image::tag::label_tag();
  
  if (hasOwner(in))
    out.tag[image[CREATOR]] = ownerAttribute(in).value();

  if (hasComments(in))
    out.tag[image[DESCRIPTION]] = commentsAttribute(in).value();

  if (hasCapDate(in))
    out.tag[image[TIME]] = capDateAttribute(in).value();

  if (hasUtcOffset(in))
    out.tag[image[UTC_OFFSET]] =
      float_to_string(utcOffsetAttribute(in).value());
  
  if (hasLongitude(in))
    out.tag[openexr[TAG_LONGITUDE]] =
      float_to_string(longitudeAttribute(in).value());

  if (hasLatitude(in))
    out.tag[openexr[TAG_LATITUDE]] =
      float_to_string(latitudeAttribute(in).value());

  if (hasAltitude(in))
    out.tag[openexr[TAG_ALTITUDE]] =
      float_to_string(altitudeAttribute(in).value());
  
  if (hasFocus(in))
    out.tag[openexr[TAG_FOCUS]] =
      float_to_string(focusAttribute(in).value());

  if (hasExpTime(in))
    out.tag[openexr[TAG_EXPOSURE]] =
      float_to_string(expTimeAttribute(in).value());

  if (hasAperture(in))
    out.tag[openexr[TAG_APERTURE]] =
      float_to_string(apertureAttribute(in).value());

  if (hasIsoSpeed(in))
    out.tag[openexr[TAG_ISO_SPEED]] =
      float_to_string(isoSpeedAttribute(in).value());

  if (hasChromaticities(in))
  {
    const Imf::Chromaticities data = chromaticitiesAttribute(in).value();
    
    out.tag[openexr[TAG_CHROMATICITIES]] = string::join(List<String>() <<
      float_to_string(data.red.x) << float_to_string(data.red.y) <<
      float_to_string(data.green.x) << float_to_string(data.green.y) <<
      float_to_string(data.blue.x) << float_to_string(data.blue.y) <<
      float_to_string(data.white.x) << float_to_string(data.white.y),
      ' ');
  }

  if (hasWhiteLuminance(in))
    out.tag[openexr[TAG_WHITE_LUMINANCE]] =
      float_to_string(whiteLuminanceAttribute(in).value());

  if (hasXDensity(in))
    out.tag[openexr[TAG_X_DENSITY]] =
      float_to_string(xDensityAttribute(in).value());
  
  if (hasKeyCode(in))
  {
    const Imf::KeyCode data = keyCodeAttribute(in).value();
    
    out.tag[image[KEYCODE]] =
      djv_image::time::keycode_to_string(
        data.filmMfcCode(), data.filmType(), data.prefix(), data.count(),
        data.perfOffset());
  }

  if (hasTimeCode(in))
    out.tag[image[TIMECODE]] = djv_image::time::timecode_to_string(
      timeCodeAttribute(in).value().timeAndFlags());
  
  if (hasFramesPerSecond(in))
  {
    const Imf::Rational data = framesPerSecondAttribute(in).value();
    
    out.time.speed = djv_image::time::Speed(data.n, data.d);
  }
}

//------------------------------------------------------------------------------
// tag_save()
//------------------------------------------------------------------------------

void tag_save(const io::Info & in, Imf::Header & out)
{
  using namespace djv_image::tag;
  using string::string_to_float;
  
  const List<String> & openexr = openexr::label_tag();
  const List<String> & image = djv_image::tag::label_tag();
  
  String tag = in.tag[image[CREATOR]];
  if (tag.size())
    addOwner(out, tag);

  tag = in.tag[image[DESCRIPTION]];
  if (tag.size())
    addComments(out, tag);

  tag = in.tag[image[TIME]];
  if (tag.size())
    addCapDate(out, tag);

  tag = in.tag[image[UTC_OFFSET]];
  if (tag.size())
    addUtcOffset(out, static_cast<float>(string_to_float(tag)));
    
  tag = in.tag[openexr[TAG_LONGITUDE]];
  if (tag.size())
    addLongitude(out, static_cast<float>(string_to_float(tag)));
  
  tag = in.tag[openexr[TAG_LATITUDE]];
  if (tag.size())
    addLatitude(out, static_cast<float>(string_to_float(tag)));
  
  tag = in.tag[openexr[TAG_ALTITUDE]];
  if (tag.size())
    addAltitude(out, static_cast<float>(string_to_float(tag)));

  tag = in.tag[openexr[TAG_FOCUS]];
  if (tag.size())
    addFocus(out, static_cast<float>(string_to_float(tag)));
  
  tag = in.tag[openexr[TAG_EXPOSURE]];
  if (tag.size())
    addExpTime(out, static_cast<float>(string_to_float(tag)));
  
  tag = in.tag[openexr[TAG_APERTURE]];
  if (tag.size())
    addAperture(out, static_cast<float>(string_to_float(tag)));
  
  tag = in.tag[openexr[TAG_ISO_SPEED]];
  if (tag.size())
    addIsoSpeed(out, static_cast<float>(string_to_float(tag)));

  tag = in.tag[openexr[TAG_CHROMATICITIES]];
  if (tag.size())
  {
    const List<String> tmp = string::split(tag, ' ');
    
    if (8 == tmp.size())
      addChromaticities(
        out,
        Imf::Chromaticities(
          Imath::V2f(
            static_cast<float>(string_to_float(tmp[0])),
            static_cast<float>(string_to_float(tmp[1]))
          ),
          Imath::V2f(
            static_cast<float>(string_to_float(tmp[2])),
            static_cast<float>(string_to_float(tmp[3]))
          ),
          Imath::V2f(
            static_cast<float>(string_to_float(tmp[4])),
            static_cast<float>(string_to_float(tmp[5]))
          ),
          Imath::V2f(
            static_cast<float>(string_to_float(tmp[6])),
            static_cast<float>(string_to_float(tmp[7]))
          )
        )
      );
  }
  
  tag = in.tag[openexr[TAG_WHITE_LUMINANCE]];
  if (tag.size())
    addWhiteLuminance(out, static_cast<float>(string_to_float(tag)));
  
  tag = in.tag[openexr[TAG_X_DENSITY]];
  if (tag.size())
    addXDensity(out, static_cast<float>(string_to_float(tag)));

  tag = in.tag[image[KEYCODE]];
  if (tag.size())
  {
    int id = 0, type = 0, prefix = 0, count = 0, offset = 0;
    djv_image::time::string_to_keycode(tag, id, type, prefix, count, offset);
    
    addKeyCode(out, Imf::KeyCode(id, type, prefix, count, offset));
  }
  
  tag = in.tag[image[TIMECODE]];
  if (tag.size())
    addTimeCode(out, djv_image::time::string_to_timecode(tag));
  
  addFramesPerSecond(
    out,
    Imf::Rational(in.time.speed.scale, in.time.speed.duration)
  );
}

//------------------------------------------------------------------------------
// imf_to_box()
//------------------------------------------------------------------------------

Box2i imf_to_box(const Imath::Box2i & in)
{
  return Box2i(
    V2i(in.min.x, in.min.y),
    V2i(in.max.x, in.max.y) - V2i(in.min.x, in.min.y) + 1
  );
}

//------------------------------------------------------------------------------
// pixel_type_to_imf(), imf_to_pixel_type()
//------------------------------------------------------------------------------

Imf::PixelType pixel_type_to_imf(pixel::TYPE in)
{
  switch (in)
  {
    case pixel::U8:
    case pixel::U10:
    case pixel::U16: 
    case pixel::F16: return Imf::HALF;
    
    default: break;
  }
  
  return Imf::FLOAT;
}

pixel::TYPE imf_to_pixel_type(Imf::PixelType in)
{
  switch (in)
  {
    case Imf::HALF: return pixel::F16;
    
    default: break;
  }
  
  return pixel::F32;
}

//------------------------------------------------------------------------------
// imf_to_channel()
//------------------------------------------------------------------------------

Channel imf_to_channel(const String & name, const Imf::Channel & channel)
{
  return Channel(
    name,
    imf_to_pixel_type(channel.type),
    V2i(channel.xSampling, channel.ySampling)
  );
}

//------------------------------------------------------------------------------
// Conversion
//------------------------------------------------------------------------------

_DJV_STRING_OPERATOR_LABEL(COLOR_PROFILE, label_color_profile())
_DJV_STRING_OPERATOR_LABEL(COMPRESSION, label_compression())
_DJV_STRING_OPERATOR_LABEL(CHANNELS, label_channels())

//------------------------------------------------------------------------------
// Comparison
//------------------------------------------------------------------------------

bool compare(const List<Imf::Channel> & in)
{
  for (list::size_type i = 1; i < in.size(); ++i)
    if (! (in[0] == in[i]))
      return false;
  
  return true;
}

//------------------------------------------------------------------------------
// Labels
//------------------------------------------------------------------------------

const List<String> & label_tag()
{
  static const List<String> data = List<String>() <<
    "Longitude" <<
    "Latitude" <<
    "Altitude" <<
    "Focus" <<
    "Exposure" <<
    "Aperture" <<
    "ISO speed" <<
    "Chromaticities" <<
    "White luminance" <<
    "X density";
  
  DJV_ASSERT(data.size() == _TAG_SIZE);
  
  return data;
}

const List<String> & label_color_profile()
{
  static const List<String> data = List<String>() <<
    "None" <<
    "Gamma" <<
    "Exposure";
  
  DJV_ASSERT(data.size() == _COLOR_PROFILE_SIZE);
  
  return data;
}

const List<String> & label_compression()
{
  static const List<String> data = List<String>() <<
    "None" <<
    "RLE" <<
    "ZIPS" <<
    "ZIP" <<
    "PIZ" <<
    "PXR24" <<
    "B44" <<
    "B44A";
  
  DJV_ASSERT(data.size() == _COMPRESSION_SIZE);
  
  return data;
}

const List<String> & label_compression_text()
{
  static const List<String> data = List<String>() <<
    "None" <<
    "RLE (Run Length Encoded)" <<
    "ZIPS" <<
    "ZIP" <<
    "PIZ" <<
    "PXR24" <<
    "B44" <<
    "B44A";
  
  DJV_ASSERT(data.size() == _COMPRESSION_SIZE);
  
  return data;
}

const List<String> & label_channels()
{
  static const List<String> data = List<String>() <<
    "Group none" <<
    "Group known" <<
    "Group all";
  
  DJV_ASSERT(data.size() == _CHANNELS_SIZE);
  
  return data;
}

const List<String> & label_channels_text()
{
  static const List<String> data = List<String>() <<
    "Give each channel it's own layer" <<
    "Group known channels into layers (e.g., RGB)" <<
    "Group all channels into a single layer";
  
  DJV_ASSERT(data.size() == _CHANNELS_SIZE);
  
  return data;
}

const List<String> & label_base_option()
{
  static const List<String> data = List<String>() <<
    "Thread count";
  
  DJV_ASSERT(data.size() == _BASE_OPTION_SIZE);
  
  return data;
}

extern const String
  label_thread_count_value =
  "0 (disable threading) - maximum (thread count)",
  label_gamma_value = "gamma value",
  label_exposure_value = "exposure, defog, knee low, knee high";

const List<String> & label_load_option()
{
  static const List<String> data = List<String>() <<
    "Color profile" <<
    "Gamma" <<
    "Exposure" <<
    "Channels";
  
  DJV_ASSERT(data.size() == _LOAD_OPTION_SIZE);
  
  return data;
}

const List<String> & label_save_option()
{
  static const List<String> data = List<String>() <<
    "Compression";
  
  DJV_ASSERT(data.size() == _SAVE_OPTION_SIZE);
  
  return data;
}

extern const String
  label_thread_count_group = "Thread Count",
  label_thread_count_text =
  "Set the maximum number of threads to use. A value of zero disables\n"
  "multi-threading.";

extern const String
  label_color_profile_group = "Color Profile",
  label_gamma_group = "Gamma",
  label_gamma = "Value:",
  label_exposure_group = "Exposure",
  label_exposure = "Value:",
  label_exposure_defog = "Defog:",
  label_exposure_knee_low = "Knee low:",
  label_exposure_knee_high = "Knee high:",
  label_channels_group = "Channels";

extern const String
  label_compression_group = "Compression";

//------------------------------------------------------------------------------

}
}

