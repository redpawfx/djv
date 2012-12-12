/*------------------------------------------------------------------------------
 include/djv_base/memory_inline.h

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

namespace djv_base {
namespace memory {

//------------------------------------------------------------------------------
// Buffer
//------------------------------------------------------------------------------

template<typename T>
inline Buffer<T>::Buffer() :
  _data(0),
  _size(0)
{}

template<typename T>
inline Buffer<T>::Buffer(const Buffer & in) :
  _data(0),
  _size(0)
{
  *this = in;
}

template<typename T>
inline Buffer<T>::Buffer(size_t in) :
  _data(0),
  _size(0)
{
  size(in);
}

template<typename T>
inline Buffer<T>::~Buffer()
{
  del();
}

template<typename T>
inline Buffer<T> & Buffer<T>::operator = (const Buffer<T> & in)
{
  if (&in != this)
  {
    size(in._size);
    copy(in._data, _data, _size);
  }
  return *this;
}

template<typename T>
inline void Buffer<T>::size(size_t in)
{
  if (in == _size) return;
  
  del();

  _size = in;
  _data = reinterpret_cast<T *>(memory::get(_size * sizeof(T) + 1));

  //zero(); // XXX
}
  
template<typename T>
inline size_t Buffer<T>::size() const
{
  return _size;
}
  
template<typename T>
inline T * Buffer<T>::data()
{
  return _data;
}

template<typename T>
inline const T * Buffer<T>::data() const
{
  return _data;
}

template<typename T>
inline void Buffer<T>::zero()
{
  memory::zero(_data, _size * sizeof(T));
}

template<typename T>
inline T * Buffer<T>::operator () ()
{
  return _data;
}
  
template<typename T>
inline const T * Buffer<T>::operator () () const
{
  return _data;
}

template<typename T>
inline void Buffer<T>::del()
{
  if (_data)
  {
    memory::del(_data);
    _data = 0;
    _size = 0;
  }
}

//------------------------------------------------------------------------------
// set()
//------------------------------------------------------------------------------

template<typename T>
inline void set(T in, void * out, size_t size)
{
  T * p = reinterpret_cast<T *>(out);
  for (size_t i = 0; i < size; ++i)
    p [i] = in;
}

//------------------------------------------------------------------------------
// endian(), endian_opposite()
//------------------------------------------------------------------------------

inline ENDIAN endian()
{
  static const int tmp = 1;
  static const uint8_t * const p = reinterpret_cast<const uint8_t *>(&tmp);
  return *p ? LSB : MSB;
}

inline ENDIAN endian_opposite(ENDIAN in)
{
  return MSB == in ? LSB : MSB;
}

inline void endian(
  void * in,
  size_t size,
  size_t word_size
) {
  uint8_t * p = reinterpret_cast<uint8_t *>(in);
  uint8_t tmp;
  
  switch (word_size)
  {
    case 2:
      while (size--)
      {
        tmp = p[0];
        p[0] = p[1];
        p[1] = tmp;

        p += 2;
      }
      break;

    case 4:
      while (size--)
      {
        tmp = p[0];
        p[0] = p[3];
        p[3] = tmp;
        
        tmp = p[1];
        p[1] = p[2];
        p[2] = tmp;
        
        p += 4;
      }
      break;

    case 8:
      while (size--)
      {
        tmp = p[0];
        p[0] = p[7];
        p[7] = tmp;
        
        tmp = p[1];
        p[1] = p[6];
        p[6] = tmp;
        
        tmp = p[2];
        p[2] = p[5];
        p[5] = tmp;
        
        tmp = p[3];
        p[3] = p[4];
        p[4] = tmp;
        
        tmp = p[4];
        p[4] = p[3];
        p[3] = tmp;
        
        tmp = p[5];
        p[5] = p[2];
        p[2] = tmp;
        
        tmp = p[6];
        p[6] = p[1];
        p[1] = tmp;
        
        tmp = p[7];
        p[7] = p[0];
        p[0] = tmp;
        
        p += 8;
      }
      break;
  }
}

inline void endian(
  const void * in,
  void * out,
  size_t size,
  size_t word_size
) {
  const uint8_t * inP = reinterpret_cast<const uint8_t *>(in);
  uint8_t * outP = reinterpret_cast<uint8_t *>(out);
  
  switch (word_size)
  {
    case 2:
      while (size--)
      {
        outP[0] = inP[1];
        outP[1] = inP[0];
        
        inP += 2;
        outP += 2;
      }
      break;
      
    case 4:
      while (size--)
      {
        outP[0] = inP[3];
        outP[1] = inP[2];
        outP[2] = inP[1];
        outP[3] = inP[0];
        
        inP += 4;
        outP += 4;
      }
      break;

    case 8:
      while (size--)
      {
        outP[0] = inP[7];
        outP[1] = inP[6];
        outP[2] = inP[5];
        outP[3] = inP[4];
        outP[4] = inP[3];
        outP[5] = inP[2];
        outP[6] = inP[1];
        outP[7] = inP[0];
        
        inP += 8;
        outP += 8;
      }
      break;
      
    default:
      copy(in, out, size * word_size);
      break;
  }
}

//------------------------------------------------------------------------------

}
}

