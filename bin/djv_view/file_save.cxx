/*------------------------------------------------------------------------------
 bin/djv_view/file_save.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "file_save.h"

#include "application.h"
#include <djv_ui/dialog.h>

namespace djv_bin {
namespace view {
namespace file {

//------------------------------------------------------------------------------
// Save_Info
//------------------------------------------------------------------------------

Save_Info::Save_Info(
  const File & input_file,
  const File & output_file,
  const djv_image::Info & info,
  const djv_image::Time & time,
  int layer,
  djv_image::data::PROXY proxy,
  const djv_image::gl_data::Option & option
) :
  input_file(input_file),
  output_file(output_file),
  info(info),
  time(time),
  layer(layer),
  proxy(proxy),
  option(option)
{}

//------------------------------------------------------------------------------
// Save::_Save
//------------------------------------------------------------------------------

struct Save::_Save : public djv_ui::Callback_Base
{
  _Save(const Save_Info &, Save * parent);

  ~_Save();
  
private:

  DJV_CALLBACK(_Save, callback, int);
  DJV_CALLBACK(_Save, del_callback, bool);

  Save_Info _info;
  std::auto_ptr<djv_image::io::Load> _load;
  std::auto_ptr<djv_image::io::Save> _save;
  djv_image::Time _load_time, _save_time;
  djv_ui::dialog::Progress * _dialog;
  Save * _parent;
};

Save::_Save::_Save(const Save_Info & in, Save * parent) :
  _info(in),
  _load_time(in.time),
  _save_time(
    in.output_file.seq().start(),
    in.output_file.seq().start() + static_cast<int>(in.time.list.size()) - 1,
    in.time.speed
  ),
  _dialog(0),
  _parent(parent)
{
  using namespace djv_image;
  
  DJV_DEBUG("Save::_Save::_Save");
  DJV_DEBUG_PRINT("input = " << _info.input_file);
  DJV_DEBUG_PRINT("output = " << _info.output_file);
  DJV_DEBUG_PRINT("load time = " << _load_time);
  DJV_DEBUG_PRINT("save time = " << _save_time);

  _info.option.xform.rotate = -_info.option.xform.rotate; // XXX WTF?
  
  const Box2f bbox =
    gl_data::xform_matrix(_info.option.xform) *
    Box2f(_info.info.size * data::proxy_scale(_info.info.proxy)
  );
  
  DJV_DEBUG_PRINT("bbox = " << bbox);
  
  _info.option.xform.position = -bbox.position;
  _info.info.size = bbox.size;

  // XXX
    
  const pixel::TYPE type = pixel::type(_info.info.pixel);
  if (
#if defined(DJV_OPENEXR)
    type != pixel::F16 &&
#endif
    type != pixel::F32
  )
    _info.option.clamp = true;

  // Open input.
  
  djv_image::io::Info load_info;
  
  try
  {
    _load = std::auto_ptr<djv_image::io::Load>(
      djv_image::io::global()->load(_info.input_file, &load_info)
    );
  }
  catch (Error error)
  {
    application::global()->error(error);
    return;
  }

  // Open output.
  
  djv_image::io::Info save_info(_info.info);
  save_info.tag = load_info.tag;
  save_info.time = _save_time;
  
  try
  {
    _save = std::auto_ptr<djv_image::io::Save>(
      djv_image::io::global()->save(_info.output_file, save_info)
    );
  }
  catch (Error error)
  {
    application::global()->error(error);
    return;
  }
  
  // Start...
  
  _dialog = new djv_ui::dialog::Progress;
  _dialog->signal.set(this, callback);
  _dialog->del_signal.set(this, del_callback);
  _dialog->set(static_cast<int>(_save_time.list.size()));
  _dialog->label(string::Format(dialog_save).arg(_info.output_file));
  _dialog->show();
}

Save::_Save::~_Save()
{
  DJV_DEBUG("Save::_Save::~_Save");
  
  if (_save.get())
  {
    try
    {
      DJV_DEBUG_PRINT("close");

      _save->close();
    }
    catch (Error error)
    {
      application::global()->error(error);
    }
    
    _save.reset();
  }

  if (_dialog)
  {
    _dialog->signal.del(this);
    _dialog->del_signal.del(this);
    _dialog->del();
  }
}

void Save::_Save::callback(int in)
{
  DJV_DEBUG("Save::_Save::callback");
  DJV_DEBUG_PRINT("in = " << in);

  // Load frame.

  const djv_image::Image * image = 0;
  
  try
  {
    DJV_DEBUG_PRINT("load");

    image = _load->get(
      djv_image::io::Frame_Info(
        _load_time.list[in],
        _info.layer,
        _info.proxy
      )
    );
    
    DJV_DEBUG_PRINT("image = " << *image);
  }
  catch (Error error)
  {
    application::global()->error(error);
    del_callback(false);
    return;
  }
  
  // Process frame.

  djv_image::Image tmp(_info.info);
  tmp.tag = image->tag;
  
  try
  {
    DJV_DEBUG_PRINT("process");
    
    djv_image::gl_data::copy(
      *image,
      &tmp,
      _info.option
    );
  }
  catch (Error error)
  {
    application::global()->error(error);
    del_callback(false);
    return;
  }

  // Save frame.
  
  try
  {
    DJV_DEBUG_PRINT("save");
    
    _save->set(
      tmp,
      _save_time.list[in]
    );
  }
  catch (Error error)
  {
    application::global()->error(error);
    del_callback(false);
    return;
  }
}

void Save::_Save::del_callback(bool)
{
  DJV_DEBUG("Save::_Save::del_callback");
  
  _parent->del(this);
}

//------------------------------------------------------------------------------
// Save
//------------------------------------------------------------------------------

Save::~Save()
{
  DJV_DEBUG("Save::~Save");
  
  for (list::size_type i = 0; i < _list.size(); ++i)
    delete _list[i];
}

void Save::set(const Save_Info & in)
{
  DJV_DEBUG("Save::Save::set");
  
  _list += new _Save(in, this);
}

void Save::del(_Save * in)
{
  DJV_DEBUG("Save::Save::del");
  
  for (
    List<_Save *>::iterator i = _list.begin(), end = _list.end();
    i != end;
    ++i
  )
    if (in == *i)
    {
      delete *i;
      _list.erase(i);
      break;
    }
}

//------------------------------------------------------------------------------

}
}
}

