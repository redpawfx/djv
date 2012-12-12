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

//! \file djv_view_cache.h

#ifndef DJV_VIEW_CACHE_H
#define DJV_VIEW_CACHE_H

#include <djv_callback.h>
#include <djv_image.h>

namespace djv_view
{
using namespace djv;

//------------------------------------------------------------------------------
//! \class Cache_Ref
//!
//! This class provides a cache reference.
//------------------------------------------------------------------------------

class Cache_Ref
{
public:

    //! Constructor.

    Cache_Ref(Image *, const void * key, int64_t frame);

    //! Destructor.

    ~Cache_Ref();

    //! Get the image.

    Image * get();

    //! Set the key.

    void key(const void *);

    //! Get the key.

    const void * key() const;

    //! Get the frame.

    int64_t frame() const;

    //! Increment the reference count.

    void ref_inc();

    //! Decrement the reference count.

    void ref_del();

    //! Get the reference count.

    int ref_count() const;

private:

    std::auto_ptr<Image> _image;
    const void *         _key;
    int64_t              _frame;
    int                  _ref_count;
};

//------------------------------------------------------------------------------
//! \class Cache
//!
//! This class provides a memory cache.
//------------------------------------------------------------------------------

class Cache : public Callback
{
public:

    //! Constructor.

    Cache();

    //! Destructor.

    virtual ~Cache();

    //! Create a cache reference.

    Cache_Ref * create(Image *, const void * key, int64_t frame);

    //! Get a cache reference.

    Cache_Ref * get(const void * key, int64_t frame);

    //! Delete all null references matching key.

    void del(const void * key);

    //! Delete all the null references.

    void del();

    //! Get a list of frames for the given key.

    List<int64_t> frames(const void * key);

    //! Set the maximum cache size.

    void max(int megabytes);

    //! Get the maximim cache size.

    int max() const;

    //! Get the size for the given key.

    int size(const void * key) const;

    //! Get the cache size.

    int size() const;

    //! Cache type.

    enum CACHE
    {
        CACHE_LRU,
        CACHE_LRU_PLAYBACK,

        _CACHE_SIZE
    };

    //! Get the cache type labels.

    static const List<String> & label_cache();

    //! Set the cache type.

    void type(CACHE);

    //! Get the cache type.

    CACHE type() const;

    //! This signal is emitted when the cache changes.

    Signal<bool> signal;

    //! Get the default cache sizes.

    static const List<int> & default_size();

    //! Get the default cache size labels.

    static const List<String> & label_size();

    //! Get the global cache.

    static Cache * global();

    //! Debugging.

    void debug();

private:

    // Delete the null references only if the cache size exceeds the maximum.

    void purge();

    typedef List<Cache_Ref *> Ref_List;

    Ref_List _refs;
    uint64_t _cache_max;
    uint64_t _cache_size;
    CACHE    _type;
};

//------------------------------------------------------------------------------

String & operator >> (String &, Cache::CACHE &) throw (String);

String & operator << (String &, Cache::CACHE);

} // djv_view

#endif // DJV_VIEW_CACHE_H

