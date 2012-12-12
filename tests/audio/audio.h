#ifndef AUDIO_H
#define AUDIO_H

#include <djv_ui/application.h>
#include <djv_ui/image_view.h>
#include <djv_ui/slider.h>
#include <djv_ui/tool_button.h>
#include <djv_ui/window.h>
#include <djv_image/time.h>
#include <portaudio.h>

namespace audio {
using namespace djv_base;

struct Audio_Info
{
  Audio_Info(int rate = 0, int channel = 0) : rate(rate), channel(channel) {}
  
  int rate;
  int channel;
};

class Audio_Buffer
{
public:

  Audio_Buffer(int chunk = 1024, int min = 100, int max = 1000);
  
  int16_t * push();
  const int16_t * pop() const;
  void clear();
  
  int chunk() const { return _chunk; }
  int chunk_bytes() const { return _chunk * sizeof(int16_t); }
  int min() const { return _min; }
  int max() const { return _max; }
  int size() const { return _size; }
  bool fill() const { return _size < _min; }
  
private:

  List<memory::Buffer<int16_t> > _tmp;
  int _chunk, _min, _max;
  int _size, _in, _out;
};

class Audio
{
public:

  Audio();
  
  ~Audio();
  
  void init(const Audio_Info &, Audio_Buffer *) throw (Error);
  void start();
  void stop();

  double time() const;
  
private:

  static int _callback(
    const void *,
    void *,
    ulong,
    const PaStreamCallbackTimeInfo *,
    PaStreamCallbackFlags,
    void *
  );
  
  int callback(
    const void *,
    void *, ulong,
    const PaStreamCallbackTimeInfo *,
    PaStreamCallbackFlags
  );
  
  PaStream * _h;
  Audio_Info _info;
  Audio_Buffer * _buffer;
  double _time_start;
  static int _instance;
};

class Audio_Factory
{
public:

  Audio_Factory();
  
  ~Audio_Factory();
  
  void init() throw (Error);
};

class Movie
{
public:

  Movie();
  
  ~Movie();
  
  void init(const String & in) throw (Error);

  const djv_image::Info & info() const;
  const djv_image::Time & info_time() const;
  const Audio_Info & info_audio() const;

  const djv_image::Data * image(int) const;  
  void audio(int16_t *, ulong) const;
  void audio_seek(int);

private:

  class _Movie;
  std::auto_ptr<_Movie> _p;
};

class Window : public djv_ui::Window
{
public:

  Window(const String &);

  void idle();
    
private:

  void play(bool);

  DJV_CALLBACK(Window, play_callback, bool);
  DJV_CALLBACK(Window, frame_callback, int);
  DJV_CALLBACK(Window, close_callback, bool);

  void play_update();
  void frame_update();

  Movie _movie;
  Audio_Buffer _audio_buffer;
  std::auto_ptr<Audio> _audio;
  bool _play;
  int _frame;
  bool _idle_init;
  djv_base::time::Timer _idle_timer;
  int _idle_frame;
  djv_ui::image::View * _widget;
  djv_ui::Tool_Button * _play_widget;
  djv_ui::Int_Slider * _slider;
};

class Application : public djv_ui::application::Application_Base
{
public:

  Application(int argc, char ** argv);
  
  ~Application();
  
  void window_add(const String &);
  void window_del(Window *);
  
  void idle_add(Window *);
  void idle_del(Window *);
  
private:

  DJV_FL_CALLBACK(Application, idle_callback);
  
  Audio_Factory _audio_factory;
  List<Window *> _window;
  List<Window *> _idle;
};

inline Application * application()
{
  return static_cast<Application *>(djv_ui::application::global());
}

}

#endif
