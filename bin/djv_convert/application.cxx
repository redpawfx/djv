/*------------------------------------------------------------------------------
 bin/djv_convert/application.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

//#define DJV_DEBUG

#include "application.h"

#include "etc.h"
#include <djv_image/io.h>
#include <djv_base/file.h>
#include <djv_base/user.h>
#include <djv_base/system.h>

namespace djv_bin {
namespace convert {

//------------------------------------------------------------------------------
// Application
//------------------------------------------------------------------------------

Application::Application(int argc, char ** argv) :
  djv_image::application::Application_Base("djv_convert", argc, argv)
{
  DJV_DEBUG("Application::Application");
  
  // Command line.

  try
  {
    String tmp = command_line_args();
    command_line(tmp);
  }
  catch (Error in)
  {
    error(Error(string::Format(application::error_command_line).arg(in)));
    exit(application::EXIT_ERROR);
  }
  if (exit() != application::EXIT_DEFAULT)
    return;
  
  // Work.

  if (! work())
    exit(application::EXIT_ERROR);
}

bool Application::work()
{
  DJV_DEBUG("Application::work");
  DJV_DEBUG_PRINT("input = " << _input.file);
  DJV_DEBUG_PRINT("output = " << _output.file);

  djv_base::time::Timer timer;
  timer.start();

  djv_image::gl_data::Option option;
  option.xform.mirror = _image.mirror;
  option.xform.scale = _image.scale;
  option.channel = _image.channel;

  // Open input.

  std::auto_ptr<djv_image::io::Load> load;
  djv_image::io::Info load_info;

  Error error;
  while (! load.get())
  {
    try
    {
      load = std::auto_ptr<djv_image::io::Load>(
        djv_image::io::global()->load()->get(_input.file, &load_info)
      );
    }
    catch (Error in)
    {
      error = in;
    }

    if (! load.get() && _input.timeout)
    {
      print(label_timeout);
      djv_base::time::sleep(1);
    }
    else
    {
      break;
    }
  }
  if (! load.get())
  {
    this->error(error);
    return false;
  }
  
  _input.layer = math::clamp(
    _input.layer,
    0,
    static_cast<int>(load_info.layer_size()) - 1
  );

  int64_t start = 0;
  int64_t end =
    load_info.time.list.size() ?
    (static_cast<int64_t>(load_info.time.list.size()) - 1) :
    0;
  
  if (! _input.start.empty())
    start = file_seq::find_closest(
      djv_image::time::string_to_units(_input.start, load_info.time.speed),
      load_info.time.list
    );
  
  if (! _input.end.empty())
    end = file_seq::find_closest(
      djv_image::time::string_to_units(_input.end, load_info.time.speed),
      load_info.time.list
    );
  
  load_info.time.list =
    List<int64_t>(load_info.time.list, start, end - start + 1);

  print(string::Format("%% %%").
    arg(_input.file).
    arg(label_image(load_info, load_info.time)));
  
  // Open output.
  
  std::auto_ptr<djv_image::io::Save> save;
  
  djv_image::io::Info save_info(load_info[_input.layer]);
  
  if (vector::is_size_valid(_image.size))
  {
    save_info.size = _image.size;
  }
  else if (_image.size.x)
  {
    save_info.size = V2i(
      _image.size.x,
      math::ceil(_image.size.x / vector::aspect(load_info.size))
    );
  }
  else if (_image.size.y)
  {
    save_info.size = V2i(
      math::ceil(_image.size.x * vector::aspect(load_info.size)),
      _image.size.y
    );
  }
  else
  {
    save_info.size =
      vector::ceil<double, int>(V2f(load_info.size) * option.xform.scale);
  }
  
  if (_output.pixel.get())
    save_info.pixel = *_output.pixel;

  save_info.tag = _output.tag;
  
  save_info.time = djv_image::time::Time(
    _output.file.seq().start(),
    _output.file.seq().start() +
      _input.slate_frames + (
        load_info.time.list.size() ?
          (static_cast<int64_t>(load_info.time.list.size()) - 1) :
          0
      ),
    load_info.time.speed
  );
  
  if (_output.speed.get())
    save_info.time.speed =  *_output.speed;
  
  try
  {
    save = std::auto_ptr<djv_image::io::Save>(
      djv_image::io::global()->save()->get(_output.file, save_info)
    );
  }
  catch (Error error)
  {
    this->error(error);
    return false;
  }

  print(string::Format("%% %%").
    arg(_output.file).
    arg(label_image(save_info, save_info.time)));
  
  // Slate.
  
  djv_image::Image slate;
  
  if (! _input.slate.name().empty())
  {
    try
    {
      print(label_slate);

      djv_image::io::Info info;
      std::auto_ptr<djv_image::io::Load> load(
        djv_image::io::global()->load()->get(_input.slate, &info)
      );
      
      const djv_image::Image * image = load->get();
      
      slate.set(save_info);
    
      djv_image::gl_data::Option option;
      option.xform.scale = V2f(save_info.size) / V2f(info.size);
      option.color_profile = image->color_profile;
    
      djv_image::gl_data::copy(
        *image,
        &slate,
        option
      );
    }
    catch (Error error)
    {
      this->error(error);
      return false;
    }
  }

  // Work.
  
  _offscreen_buffer = std::auto_ptr<djv_image::gl_offscreen::Buffer>(
    djv_image::gl_offscreen::global()->buffer_create()
  );
  _offscreen_buffer->init(save_info);
  
  static const int progress_length = 10;
  int progress_count = 0;
  double progress_accum = 0.0;

  for (int64_t i = 0; i < _input.slate_frames; ++i)
  {
    try
    {
      save->set(
        slate,
        djv_image::io::Frame_Info(save_info.time.list.pop_front())
      );
    }
    catch (Error error)
    {
      this->error(error);
      return false;
    }
  }
  
  const int64_t length = static_cast<int64_t>(save_info.time.list.size());
  
  for (int64_t i = 0; i < length; ++i)
  {
    djv_base::time::Timer frame_timer;
    frame_timer.start();
    
    // Load.

    const djv_image::Image * image = 0;
    
    int timeout = _input.timeout;
    
    while (! image)
    {
      try
      {
        image = load->get(
          djv_image::io::Frame_Info(
            load_info.time.list.size() ? load_info.time.list[i] : -1,
            _input.layer,
            _input.proxy
          )
        );
      }
      catch (Error in)
      {
        error = in;
      }
      
      if (! image && timeout > 0)
      {
        //print(label_timeout);
        --timeout;
        djv_base::time::sleep(1);
      }
      else
      {
        break;
      }
    }
    
    if (! image)
    {
      this->error(error);
      return false;
    }
    
    DJV_DEBUG_PRINT("image = " << *image);

    // Image tags.
    
    djv_image::Tag tag = _output.tag;

    tag.add(image->tag);
    
    if (_output.tag_auto)
    {
      tag[djv_image::tag::label_tag()[djv_image::tag::CREATOR]] =
        user::current();
      
      tag[djv_image::tag::label_tag()[djv_image::tag::TIME]] =
        djv_base::time::time_to_string(djv_base::time::current());
      
      tag[djv_image::tag::label_tag()[djv_image::tag::TIMECODE]] =
        djv_image::time::timecode_to_string(
          djv_image::time::frame_to_timecode(
            save_info.time.list.size() ? save_info.time.list[i] : 0,
            save_info.time.speed
          )
        );
    }
    
    // Process.

    djv_image::Image tmp(save_info);
    tmp.tag = tag;
    
    option.xform.scale = V2f(save_info.size) / V2f(load_info.size);
    option.color_profile = image->color_profile;
    
    djv_image::gl_data::copy(
      *image,
      &tmp,
      option,
      &_data,
      _offscreen_buffer.get()
    );
    
    // Save.

    DJV_DEBUG_PRINT("output = " << tmp);
    
    try
    {
      save->set(
        tmp,
        djv_image::io::Frame_Info(
          save_info.time.list.size() ? save_info.time.list[i] : -1
        )
      );
    }
    catch (Error error)
    {
      this->error(error);
      return false;
    }

    // Statistics.

    timer.check();
    frame_timer.check();
    
    progress_accum += frame_timer.seconds();
    if (
      length > 1 &&
      i / static_cast<double>(length) * progress_length > progress_count
    ) {
      const double estimate =
        progress_accum / static_cast<double>(i + 1) * (length - (i + 1));

      print(string::Format(label_estimate).
        arg(static_cast<int>(i / static_cast<double>(length) * 100.0), 3).
        arg(djv_base::time::label_time(estimate)).
        arg(i / timer.seconds(), 0, 2));

      ++progress_count;
    }
  }
  if (length > 1)
    print(label_complete, false);
  
  try
  {
    save->close();
  }
  catch (Error error)
  {
    this->error(error);
    return false;
  }

  print(string::Format(label_elapsed).
    arg(djv_base::time::label_time(timer.seconds())));

  return true;
}

}
}

//------------------------------------------------------------------------------
// main()
//------------------------------------------------------------------------------

int main(int argc, char ** argv)
{
  int r = 1;
  
  try
  {
    r = djv_bin::convert::Application(argc, argv).exec();
  }
  catch (djv_base::Error in)
  {
    djv_base::error::print(in);
  }
  
  return r;
}

//------------------------------------------------------------------------------
