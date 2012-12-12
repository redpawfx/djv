//#define DJV_DEBUG

#include "audio.h"

#include <djv_ui/layout_row.h>
#include <algorithm>

namespace audio {

Application::Application(int argc, char ** argv) :
  djv_ui::application::Application_Base("audio", argc, argv)
{
  DJV_DEBUG("Application::Application");
  
  // Command line.
  
  if (argc < 2)
  {
    print("usage: test_audio (input)...");
    djv_image::application::Application_Base::exit(
      djv_base::application::EXIT_ERROR
    );
    return;
  }

  // Initialize.
  
  try { _audio_factory.init(); }
  catch (Error in) { error(in); }
  
  is_valid(true);
  
  for (int i = 1; i < argc; ++i)
    window_add(argv[i]);
}

Application::~Application()
{
  DJV_DEBUG("Application::~Application");

  Fl::remove_timeout(idle_callback, this);
  
  DJV_DEBUG_PRINT("window = " << uint(_window.size()));
  for (list::size_type i = 0; i < _window.size(); ++i)
    _window[i]->del();
}

void Application::window_add(const String & in)
{
  DJV_DEBUG("Application::window_add");
  DJV_DEBUG_PRINT("in = " << in);
  
  _window += new Window(in);
}

void Application::window_del(Window * in)
{
  DJV_DEBUG("Application::window_del");

  idle_del(in);
  in->del();
  _window.erase(std::find(_window.begin(), _window.end(), in));
}

const double idle_timeout = 0.001;
  
void Application::idle_add(Window * in)
{
  DJV_DEBUG("Application::idle_add");
  
  _idle += in;
  if (1 == _idle.size())
    Fl::add_timeout(idle_timeout, idle_callback, this);
}

void Application::idle_del(Window * in)
{
  DJV_DEBUG("Application::idle_del");

  List<Window *>::iterator i = std::find(_idle.begin(), _idle.end(), in);
  if (i != _idle.end())
    _idle.erase(i);
  if (! _idle.size())
    Fl::remove_timeout(idle_callback, this);
}

void Application::idle_callback()
{
  DJV_DEBUG("Application::idle_callback");
  
  for (list::size_type i = 0; i < _idle.size(); ++i)
    _idle[i]->idle();
  Fl::add_timeout(idle_timeout, idle_callback, this);
}

}

int main(int argc, char ** argv)
{
  return audio::Application(argc, argv).exec();
}
