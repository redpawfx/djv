//#define DJV_DEBUG

#include "audio.h"

namespace audio {

Audio_Buffer::Audio_Buffer(int chunk, int min, int max) :
  _chunk(chunk),
  _min(min),
  _max(max),
  _size(0),
  _in(0),
  _out(0)
{
  _tmp.resize(_max);
  for (int i = 0; i < _max; ++i)
    _tmp[i].size(_chunk);
}

int16_t * Audio_Buffer::push()
{
  if (_size >= _max)
    return 0;
  int16_t * out = _tmp[_in]();
  _in = (_in + 1) % _max;
  ++_size;
  return out;
}

const int16_t * Audio_Buffer::pop() const
{
  Audio_Buffer * that = const_cast<Audio_Buffer *>(this);
  if (! _size)
    return 0;
  const int16_t * out = _tmp[_out]();
  that->_out = (_out + 1) % _max;
  --that->_size;
  return out;
}

void Audio_Buffer::clear()
{
  _size = _in = _out = 0;
}

Audio::Audio() :
  _h(0),
  _time_start(0.0)
{
  DJV_DEBUG("Audio::Audio");
}

Audio::~Audio()
{
  DJV_DEBUG("Audio::~Audio");

  if (_h) Pa_CloseStream(_h);
}

void Audio::init(const Audio_Info & info, Audio_Buffer * buffer) throw (Error)
{
  DJV_DEBUG("Audio::init");

  _info = info;
  _buffer = buffer;
  
  // Open.
  
  PaError error = Pa_OpenDefaultStream(
    &_h,
    0,
    _info.channel,
    paInt16,
    _info.rate,
    //paFramesPerBufferUnspecified,
    _info.channel ? (_buffer->chunk() / _info.channel) : 0,
    _callback,
    this
  );
  if (error != paNoError)
    throw Error(Pa_GetErrorText(error));
}

void Audio::start()
{
  DJV_DEBUG("Audio::start");

  Pa_StartStream(_h);
  _time_start = Pa_GetStreamTime(_h);
}

void Audio::stop()
{
  DJV_DEBUG("Audio::stop");
  
  //Pa_StopStream(_h);
  Pa_AbortStream(_h);
}

double Audio::time() const
{
  return
    Pa_GetStreamTime(_h) - _time_start -
    Pa_GetStreamInfo(_h)->outputLatency;
}

int Audio::_callback(
  const void * in ,
  void * out,
  ulong size,
  const PaStreamCallbackTimeInfo * time_info,
  PaStreamCallbackFlags flag,
  void * data
) {
  return static_cast<Audio *>(data)->callback(in, out, size, time_info, flag);
}

int Audio::callback(
  const void * in,
  void * out,
  ulong size,
  const PaStreamCallbackTimeInfo * time_info,
  PaStreamCallbackFlags flag)
{
  DJV_DEBUG("Audio::callback");
  DJV_DEBUG_PRINT("size = " << static_cast<int>(size));
  
  const int16_t * p = _buffer->pop();
  if (! p)
  {
    DJV_DEBUG_PRINT("xrun");
    memory::zero(out, _buffer->chunk_bytes());
    //abort();
  }
  else
  {
    memory::copy(p, out, _buffer->chunk_bytes());
  }

  return 0;
}

Audio_Factory::Audio_Factory()
{
  DJV_DEBUG("Audio_Factory::Audio_Factory");
}

Audio_Factory::~Audio_Factory()
{
  DJV_DEBUG("Audio_Factory::~Audio_Factory");

  Pa_Terminate();
}

void Audio_Factory::init() throw (Error)
{
  DJV_DEBUG("Audio_Factory::init");
  
  PaError error = Pa_Initialize();
  if (error != paNoError)
    throw Error(Pa_GetErrorText(error));
}

}
