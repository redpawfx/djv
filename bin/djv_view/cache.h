/*------------------------------------------------------------------------------
 bin/djv_view/cache.h
 
 Copyright (c) 2004-2009 Darby Johnston
 All rights reserved.
 Use is subject to license terms.
------------------------------------------------------------------------------*/

#ifndef DJV_VIEW_CACHE_H
#define DJV_VIEW_CACHE_H

#include "base.h"
#include <djv_ui/callback.h>
#include <djv_image/image_def.h>

namespace djv_bin {
namespace view {

//------------------------------------------------------------------------------
//!\namespace djv_bin::view::cache
//
//! Memory cache.
//------------------------------------------------------------------------------

namespace cache {

//------------------------------------------------------------------------------
//!\name Options
//------------------------------------------------------------------------------
//@{

enum CACHE
{
  CACHE_LRU,
  CACHE_LRU_PLAYBACK,
  
  _CACHE_SIZE
};

//@}
//------------------------------------------------------------------------------
//!\class Ref
//
//! Cache reference.
//------------------------------------------------------------------------------

class Ref
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Ref(djv_image::Image *, const void * key, int64_t frame);
  
  ~Ref();
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Value
  //@{
  
  djv_image::Image * get();
    
  void key(const void *);
  
  const void * key() const;
  
  int64_t frame() const;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Reference Count
  //@{

  void ref_inc();
  void ref_del();
  
  int ref_count() const;
  
  //@}
  
private:

  std::auto_ptr<djv_image::Image> _image;
  const void * _key;
  int64_t _frame;
  int _ref_count;
};

//------------------------------------------------------------------------------
//!\class Cache
//
//! Memory cache.
//------------------------------------------------------------------------------

class Cache : public djv_ui::Callback_Base
{
public:

  //----------------------------------------------------------------------------
  //!\name Construction
  //@{

  Cache();
  
  ~Cache();
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Cache References
  //@{

  //! Create a cache reference.
  
  Ref * create(djv_image::Image *, const void * key, int64_t frame);
  
  //! Get a cache reference.
  
  Ref * get(const void * key, int64_t frame);
  
  //! Delete all null references matching key.
  
  void del(const void * key);
  
  //! Delete all null references.
  
  void del();
  
  //! Return a list of frames for the given key.
  
  List<int64_t> frame_list(const void * key);
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Options
  //@{
  
  //! Maximum cache size.
  
  void max(int megabytes);
  
  int max() const;
  
  //! Current cache size.
  
  int size(const void * key) const;
  int size() const;
  
  //! Cache type.
  
  void type(CACHE);
  
  CACHE type() const;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Signals
  //@{
  
  djv_ui::callback::Signal<bool> signal;
  
  //@}
  //----------------------------------------------------------------------------
  //!\name Debugging
  //@{
  
  void debug();

  //@}
  
private:

  // Delete null references only if the cache size exceeds the maximum.
  
  void purge();
    
  typedef List<Ref *> Ref_List;
  
  Ref_List _ref_list;
  
  uint64_t _cache_max;
  uint64_t _cache_size;
  
  CACHE _type;
};

//------------------------------------------------------------------------------
//!\name Conversion
//------------------------------------------------------------------------------
//@{

String & operator >> (String &, CACHE &) throw (String);

String & operator << (String &, CACHE);

//@}
//------------------------------------------------------------------------------
//!\name Defaults
//------------------------------------------------------------------------------
//@{

const List<int> & default_size();

//@}
//------------------------------------------------------------------------------
//!\name Labels
//------------------------------------------------------------------------------
//@{

const List<String> & label_size();
const List<String> & label_cache();
const List<String> & label_cache_text();

//@}
//------------------------------------------------------------------------------
//!\name Global
//------------------------------------------------------------------------------
//@{

//! Access to global image cache.

Cache * global();

//@}
//------------------------------------------------------------------------------

}
}
}

#endif

