//------------------------------------------------------------------------------
// Copyright (c) 2004-2012 Darby Johnston
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions, and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions, and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the names of the copyright holders nor the names of any
//   contributors may be used to endorse or promote products derived from this
//   software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//------------------------------------------------------------------------------

//! \file djv_openexr.h

#ifndef DJV_OPENEXR_H
#define DJV_OPENEXR_H

#include <djv_box.h>
#include <djv_image_io.h>

#include <ImathBox.h>
#include <ImfPixelType.h>

namespace Imf
{

struct Channel;
class ChannelList;
class Header;

} // Imf

//! \namespace djv_openexr
//
//! This plugin provides support for the Industrial Light and Magic OpenEXR
//! image file format.
//!
//! Requires:
//!
//! - OpenEXR - http://www.openexr.com
//!
//! Supports:
//!
//! - Images: 16-bit float, 32-bit float, Luminance, Luminance Alpha, RGB, RGBA
//! - Image layers
//! - Display and data windows
//! - File compression

namespace djv_openexr
{
using namespace djv;

static const String name = "OpenEXR";

static const List<String> extensions = List<String>() <<
    ".exr";

//! This struct provides an image channel.

struct Channel
{
    //! Constructor.

    Channel() :
        sampling(1)
    {}

    //! Constructor.

    Channel(
        const String & name,
        Pixel::TYPE    type,
        const V2i &    sampling = V2i(1));

    String      name;
    Pixel::TYPE type;
    V2i         sampling;
};

//! This struct provides an image layer.

struct Layer
{
    //! Constructor.

    Layer(const List<Channel> & = List<Channel>());

    String        name;
    List<Channel> channel;
};

//! Color profile.

enum COLOR_PROFILE
{
    COLOR_PROFILE_NONE,
    COLOR_PROFILE_GAMMA,
    COLOR_PROFILE_EXPOSURE,

    _COLOR_PROFILE_SIZE
};

//! Get the color profile labels.

const List<String> & label_color_profile();

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

//! Get the compression labels.

const List<String> & label_compression();

//! Channels.

enum CHANNELS
{
    CHANNELS_GROUP_NONE,
    CHANNELS_GROUP_KNOWN,
    CHANNELS_GROUP_ALL,

    _CHANNELS_SIZE
};

//! Get the channel labels.

const List<String> & label_channels();

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

//! Get the image tag labels.

const List<String> & label_tag();

//! Initialize OpenEXR.

void openexr_init() throw (Error);

//! De-initialize OpenEXR.

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

//! Load image tags.

void tag_load(const Imf::Header &, Image_Io_Info &);

//! Save image tags.

void tag_save(const Image_Io_Info &, Imf::Header &);

//! Convert from an OpenEXR box type.

Box2i imf_to_box(const Imath::Box2i &);

//! Convert to a OpenEXR pixel type.

Imf::PixelType pixel_type_to_imf(Pixel::TYPE);

//! Convert from an OpenEXR pixel type.

Pixel::TYPE imf_to_pixel_type(Imf::PixelType);

//! Convert from an OpenEXR channel.

Channel imf_to_channel(const String & name, const Imf::Channel &);

String & operator >> (String &, COLOR_PROFILE &) throw (String);
String & operator >> (String &, COMPRESSION &) throw (String);
String & operator >> (String &, CHANNELS &) throw (String);

String & operator << (String &, COLOR_PROFILE);
String & operator << (String &, COMPRESSION);
String & operator << (String &, CHANNELS);

bool compare(const List<Imf::Channel> &);

} // djv_openexr

#endif // DJV_OPENEXR_H

