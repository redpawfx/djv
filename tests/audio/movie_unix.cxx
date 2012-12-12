#if ! defined(WIN32) && ! defined(DJV_MACOSX)

//#define DJV_DEBUG

#include "audio.h"

#include <lqt/colormodels.h>
#include <lqt/lqt.h>

namespace audio {

class Movie::_Movie
{
public:

  _Movie();
  
  ~_Movie();
  
  void init(const String & in) throw (Error);

  const djv_image::Info & info() const { return _info; }
  const djv_image::Time & info_time() const { return _info_time; }
  const Audio_Info & info_audio() const { return _info_audio; }

  const djv_image::Data * image(int) const;  
  void audio(int16_t *, ulong) const;
  void audio_seek(int);

private:

  quicktime_t * _qt;
  djv_image::Info _info;
  djv_image::Time _info_time;
  Audio_Info _info_audio;
  djv_image::Data _tmp_image;
  memory::Buffer<int16_t> _tmp_audio;
  int _tmp_frame;
};

Movie::_Movie::_Movie() :
  _qt(0),
  _tmp_frame(-1)
{}

Movie::_Movie::~_Movie()
{
  if (_qt) quicktime_close(_qt);
}

void Movie::_Movie::init(const String & in) throw (Error)
{
  DJV_DEBUG("Movie::_Movie::init");
  DJV_DEBUG_PRINT("in = " << in);
  
  _qt = lqt_open_read(in.c_str());
  if (! _qt)
    throw Error("open", in);

  _info.size = V2i(quicktime_video_width(_qt, 0),
    quicktime_video_height(_qt, 0));
  
  //lqt_set_cmodel(_qt, 0, BC_RGBA8888);
  //_info.pixel = djv_image::pixel::RGBA_U8;
  static int cmodel_list [] =
  {
    BC_RGB888,
    BC_BGR888,
    BC_RGB161616,
    BC_RGBA8888,
    BC_RGBA16161616,
    LQT_COLORMODEL_NONE
  };
  const int cmodel = lqt_get_best_colormodel(_qt, 0, cmodel_list);
  switch (cmodel)
  {
    case BC_BGR888: _info.bgr = true;
    case BC_RGB888: _info.pixel = djv_image::pixel::RGB_U8; break;
    case BC_RGB161616: _info.pixel = djv_image::pixel::RGB_U16; break;
    case BC_RGBA8888: _info.pixel = djv_image::pixel::RGBA_U8; break;
    case BC_RGBA16161616: _info.pixel = djv_image::pixel::RGBA_U16; break;
    default: throw Error("unsupported", in);
  }
  lqt_set_cmodel(_qt, 0, cmodel);
  
  _tmp_image.set(_info);
  DJV_DEBUG_PRINT("info = " << _info);

  int time_constant = 0;
  const int time_scale = lqt_video_time_scale(_qt, 0);
  const int frame_duration = lqt_frame_duration(_qt, 0, &time_constant);
  _info_time = djv_image::Time(
    0, frame_duration ? (lqt_video_duration(_qt, 0) / frame_duration - 1) : 0,
    djv_image::time::Speed(time_scale, frame_duration)
  );
  DJV_DEBUG_PRINT("time scale = " << time_scale);
  DJV_DEBUG_PRINT("frame duration = " << frame_duration);
  DJV_DEBUG_PRINT("time constant = " << time_constant);
  DJV_DEBUG_PRINT("time = " << _info_time);
  
  _info_audio.rate = quicktime_sample_rate(_qt, 0);
  _info_audio.channel = quicktime_track_channels(_qt, 0);
  DJV_DEBUG_PRINT("audio rate = " << _info_audio.rate);
  DJV_DEBUG_PRINT("audio channel = " << _info_audio.channel);
}

const djv_image::Data * Movie::_Movie::image(int in) const
{
  DJV_DEBUG("Movie::_Movie::image");
  DJV_DEBUG_PRINT("in = " << in);
  
  _Movie * that = const_cast<_Movie *>(this);
  
  if (! _qt)
    return 0;
  
  if (in != ++that->_tmp_frame)
  {
    that->_tmp_frame = in;
    const uint64_t time = _tmp_frame * _info_time.speed.duration;
    DJV_DEBUG_PRINT("time = " << static_cast<int>(time));
    lqt_seek_video(_qt, 0, time);
  }
  
  struct Data
  {
    Data(djv_image::Data * in) : p(0)
    {
      p = new unsigned char * [in->h() + 1];
      for (int y = 0; y < in->h(); ++y)
        p[y] = reinterpret_cast<djv_image::pixel::U8_T *>(
          in->data(0, in->h() - 1 - y)
        );
    }
    
    ~Data() { if (p) delete [] p; }
    
    unsigned char ** p;
  };
  
  lqt_decode_video(_qt, Data(&that->_tmp_image).p, 0);
  
  return &_tmp_image;
}

void Movie::_Movie::audio(int16_t * in, ulong size) const
{
  DJV_DEBUG("Movie::_Movie::audio");
  DJV_DEBUG_PRINT("size = " << static_cast<int>(size));
  
  _Movie * that = const_cast<_Movie *>(this);
  
  if (! _qt) return;
  
  DJV_DEBUG_PRINT("audio = " <<
    static_cast<int>(quicktime_audio_position(_qt, 0)) << "/" <<
    static_cast<int>(quicktime_audio_length(_qt, 0)));
  
  // Setup buffer.
  
  const int channel = _info_audio.channel;
  
  that->_tmp_audio.size(size * channel);
  ulong _size = math::min(
    size,
    ulong(quicktime_audio_length(_qt, 0) - quicktime_audio_position(_qt, 0))
  );
  DJV_DEBUG_PRINT("size = " << static_cast<int>(_size));
  
  struct Data
  {
    Data(int16_t * in, ulong size, int channel) :
      p(0)
    {
      p = new int16_t * [channel + 1];
      for (int i = 0; i < channel; ++i)
        p[i] = in + size * i;
    }
    
    ~Data()
    {
      if (p) delete [] p;
    }
    
    int16_t ** p;
    
  } data(that->_tmp_audio(), _size, channel);

  // Fill buffer.
  
  if (_size) lqt_decode_audio_track(_qt, data.p, 0, _size, 0);
  
  ulong i = 0;
  for (; i < _size; ++i, in += channel)
    for (int c = 0; c < channel; ++c)
      in[c] = data.p[c][i];
  
  // Zero remainder.
  
  for (; i < size; ++i, in += channel)
    for (int c = 0; c < channel; ++c)
      in[c] = 0;
}

void Movie::_Movie::audio_seek(int in)
{
  DJV_DEBUG("Movie::_Movie::audio_seek");
  DJV_DEBUG_PRINT("in = " << in);
  
  if (! _qt) return;

  const int64_t sample = math::floor(
    (in / djv_image::time::speed_to_float(_info_time.speed)) *
    _info_audio.rate
  );
  DJV_DEBUG_PRINT("sample = " << static_cast<int>(sample));
  quicktime_set_audio_position(_qt, sample, 0);
}

Movie::Movie() : _p(new _Movie) {}
Movie::~Movie() {}

void Movie::init(const String & in) throw (Error) { _p->init(in); }

const djv_image::Info & Movie::info() const { return _p->info(); }
const djv_image::Time & Movie::info_time() const { return _p->info_time(); }
const Audio_Info & Movie::info_audio() const { return _p->info_audio(); }

const djv_image::Data * Movie::image(int in) const { return _p->image(in); }
void Movie::audio(int16_t * in, ulong size) const { _p->audio(in, size); }
void Movie::audio_seek(int in) { _p->audio_seek(in); }

}

#endif
