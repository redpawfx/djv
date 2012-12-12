/*------------------------------------------------------------------------------
 bin/djv_view/cache.cxx

 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#undef DJV_DEBUG
//#define DJV_DEBUG

#include "cache.h"

#include <djv_image/image.h>
#include <algorithm>

namespace djv_bin {
namespace view {
namespace cache {

//------------------------------------------------------------------------------
// Ref
//------------------------------------------------------------------------------

namespace {

int ref_alive = 0;

}

Ref::Ref(djv_image::Image * in, const void * key, int64_t frame) :
  _image(in),
  _key(key),
  _frame(frame),
  _ref_count(1)
{
  ++ref_alive;
  DJV_DEBUG("Ref::Ref");
  DJV_DEBUG_PRINT("alive = " << ref_alive);
}

Ref::~Ref()
{
  --ref_alive;
  DJV_DEBUG("Ref::~Ref");
  DJV_DEBUG_PRINT("alive = " << ref_alive);
}

djv_image::Image * Ref::get()
{
  return _image.get();
}

void Ref::key(const void * in)
{
  _key = in;
}

const void * Ref::key() const
{
  return _key;
}

int64_t Ref::frame() const
{
  return _frame;
}

void Ref::ref_inc()
{
  ++_ref_count;
}
  
void Ref::ref_del()
{
  --_ref_count;
}

int Ref::ref_count() const
{
  return _ref_count;
}

//------------------------------------------------------------------------------
// Cache
//------------------------------------------------------------------------------

Cache::Cache() :
  signal(this),
  _cache_max(default_size()[4] * memory::megabyte),
  _cache_size(0),
  _type(CACHE_LRU_PLAYBACK)
{
  DJV_DEBUG("Cache::Cache");
}

Cache::~Cache()
{
  DJV_DEBUG("Cache::~Cache");

#if defined(DJV_DEBUG)
  debug();
#endif

  // Cleanup.
  
  const Ref_List::iterator end = _ref_list.end();
  for (Ref_List::iterator i = _ref_list.begin(); i != end; ++i)
    delete *i;
}

Ref * Cache::create(djv_image::Image * image, const void * key, int64_t frame)
{
  DJV_DEBUG("Cache::create");
  DJV_DEBUG_PRINT("image = " << *image);
  DJV_DEBUG_PRINT("key = " << reinterpret_cast<int64_t>(key));
  DJV_DEBUG_PRINT("frame = " << frame);

#if defined(DJV_DEBUG)
  debug();
#endif

  // Create a new reference.
  
  Ref * out = new Ref(image, key, frame);
  
  // Add the reference to the end of the list.
  
  _ref_list += out;
    
  // Update the cache size.
  
  _cache_size += out->get()->bytes_data();

  if (_cache_size > _cache_max)
    purge();
  
  signal.emit(true);

#if defined(DJV_DEBUG)
  debug();
#endif

  return out;
}

Ref * Cache::get(const void * key, int64_t frame)
{
  DJV_DEBUG("Cache::get");
  DJV_DEBUG_PRINT("key = " << reinterpret_cast<int64_t>(key));
  DJV_DEBUG_PRINT("frame = " << frame);

#if defined(DJV_DEBUG)
  //debug();
#endif

  Ref * out = 0;
  
  const Ref_List::iterator end = _ref_list.end();
  for (Ref_List::iterator i = _ref_list.begin(); i != end; ++i)
    if (key == (*i)->key() && frame == (*i)->frame())
    {
      // Found the reference.
      
      out = *i;
      
      // Increment the reference count.
      
      out->ref_inc();
      
      // Move the reference to the end of the list.
      
      _ref_list.erase(i);
      
      _ref_list += out;
      
      break;
    }

#if defined(DJV_DEBUG)
  debug();
#endif

  DJV_DEBUG_PRINT("out = " <<
    (out ? reinterpret_cast<int64_t>(out->key()) : 0));
  
  return out;
}

void Cache::del(const void * key)
{
  DJV_DEBUG("Cache::del");
  DJV_DEBUG_PRINT("key = " << reinterpret_cast<int64_t>(key));
#if defined(DJV_DEBUG)
  debug();
#endif

  // Find null references matching key.
  
  List<Ref_List::iterator> del_list;
  
  const Ref_List::iterator end = _ref_list.end();
  for (Ref_List::iterator i = _ref_list.begin(); i != end; ++i)
    if (
      ! (*i)->ref_count() &&
      key == (*i)->key()
    )
      del_list.push_front(i);
    
  // Delete the null references.
  
  DJV_DEBUG_PRINT("delete list = " << del_list.size());

  for (list::size_type i = 0; i < del_list.size(); ++i)
  {
    _cache_size -= (*del_list[i])->get()->bytes_data();

    delete *del_list[i];

    _ref_list.erase(del_list[i]);
  }
  
  // Initialize any remaining references matching key; they will be cleaned
  // up later.
  
  const Ref_List::const_iterator end2 = _ref_list.end();
  for (Ref_List::const_iterator i = _ref_list.begin(); i != end2; ++i)
    if (key == (*i)->key())
      (*i)->key(0);
  
  // Signal that the cache has changed.
  
  signal.emit(true);

#if defined(DJV_DEBUG)
  debug();
#endif
}

void Cache::del()
{
  DJV_DEBUG("Cache::del");
  
#if defined(DJV_DEBUG)
  debug();
#endif

  // Find all null references.
  
  List<Ref_List::iterator> del_list;
  
  const Ref_List::iterator end = _ref_list.end();
  for (Ref_List::iterator i = _ref_list.begin(); i != end; ++i)
    if (! (*i)->ref_count())
      del_list.push_front(i);
  
  // Delete the null references.
  
  DJV_DEBUG_PRINT("delete list = " << del_list.size());
  
  for (list::size_type i = 0; i < del_list.size(); ++i)
  {
    _cache_size -= (*del_list[i])->get()->bytes_data();

    delete *del_list[i];

    _ref_list.erase(del_list[i]);
  }
  
  // Signal that the cache has changed.

  signal.emit(true);

#if defined(DJV_DEBUG)
  debug();
#endif
}
  
List<int64_t> Cache::frame_list(const void * key)
{
  List<int64_t> out;
  
  const Ref_List::const_iterator end = _ref_list.end();
  for (Ref_List::const_iterator i = _ref_list.begin(); i != end; ++i)
    if (key == (*i)->key())
      out += (*i)->frame();
  
  std::sort(out.begin(), out.end());
  
  return out;
}

void Cache::max(int in)
{
  DJV_DEBUG("Cache::max");
  DJV_DEBUG_PRINT("in = " << in);
#if defined(DJV_DEBUG)
  debug();
#endif
  
  _cache_max = in * memory::megabyte;
  
  //if (_cache_size > _cache_max)
    purge();
  
#if defined(DJV_DEBUG)
  debug();
#endif
}

int Cache::max() const
{
  return static_cast<int>(_cache_max / memory::megabyte);
}

int Cache::size(const void * key) const
{
  uint64_t size = 0;
  
  const Ref_List::const_iterator end = _ref_list.end();
  for (Ref_List::const_iterator i = _ref_list.begin(); i != end; ++i)
    if (key == (*i)->key())
      size += (*i)->get()->bytes_data();
  
  return static_cast<int>(size / memory::megabyte);
}
 
int Cache::size() const
{
  return static_cast<int>(_cache_size / memory::megabyte);
}
 
void Cache::type(CACHE in)
{
  _type = in;
}

CACHE Cache::type() const
{
  return _type;
}

void Cache::purge()
{
  DJV_DEBUG("Cache::purge");
  DJV_DEBUG_PRINT("type = " << _type);
  
#if defined(DJV_DEBUG)
  debug();
#endif

  // Find null references we can delete to bring the cache size below the
  // maximum size.
  
  List<Ref_List::iterator> del_list;
  
  switch (_type)
  {
    case CACHE_LRU: break;
    
    case CACHE_LRU_PLAYBACK:
    {
      // Find null references that are before the current frame.

      const int64_t frame =
        _ref_list.begin() != _ref_list.end() ?
          (_ref_list.back()->frame()) :
          0;

      DJV_DEBUG_PRINT("frame = " << frame);
    
      const Ref_List::iterator end = _ref_list.end();
      for (Ref_List::iterator i = _ref_list.begin(); i != end; ++i)
        if (
          ! (*i)->ref_count() &&
          (*i)->frame() < frame &&
          _cache_size > _cache_max
        ) {
          _cache_size -= (*i)->get()->bytes_data();
      
          del_list.push_front(i);
        }
  
      // Delete the null references.

      DJV_DEBUG_PRINT("delete list = " << del_list.size());

      for (list::size_type i = 0; i < del_list.size(); ++i)
      {
        delete *del_list[i];

        _ref_list.erase(del_list[i]);
      }
    
      del_list.clear();
    }
    break;
  }
  
  const Ref_List::iterator end = _ref_list.end();
  for (Ref_List::iterator i = _ref_list.begin(); i != end; ++i)
    if (
      ! (*i)->ref_count() &&
      _cache_size > _cache_max
    ) {
      _cache_size -= (*i)->get()->bytes_data();
      
      del_list.push_front(i);
    }
  
  // Delete the null references.
  
  DJV_DEBUG_PRINT("delete list = " << del_list.size());

  for (list::size_type i = 0; i < del_list.size(); ++i)
  {
    delete *del_list[i];

    _ref_list.erase(del_list[i]);
  }
  
  // Signal that the cache has changed.
  
  signal.emit(true);

#if defined(DJV_DEBUG)
  debug();
#endif
}

void Cache::debug()
{
  DJV_DEBUG("Cache::debug");
  DJV_DEBUG_PRINT("refs = " << _ref_list.size());
  DJV_DEBUG_PRINT("cache max = " <<
    static_cast<int>(_cache_max / memory::megabyte));
  DJV_DEBUG_PRINT("cache size = " <<
    static_cast<int>(_cache_size / memory::megabyte));
  
  const Ref_List::const_iterator end = _ref_list.end();
  for (Ref_List::const_iterator i = _ref_list.begin(); i != end; ++i)
    DJV_DEBUG_PRINT(
      "item (ref = " << (*i)->ref_count() << ") = " <<
      reinterpret_cast<int64_t>((*i)->key()) << " " <<
      (*i)->frame()
    );
}

//------------------------------------------------------------------------------

}
}
}

