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

//! \file djv_view_cache.cpp

#include <djv_view_cache.h>

#include <djv_assert.h>

#include <algorithm>

namespace djv_view
{

//------------------------------------------------------------------------------
// Cache_Ref
//------------------------------------------------------------------------------

namespace
{

int ref_alive = 0;

} // namespace

Cache_Ref::Cache_Ref(Image * in, const void * key, int64_t frame) :
    _image(in),
    _key(key),
    _frame(frame),
    _ref_count(1)
{
    ++ref_alive;

    //DJV_DEBUG("Cache_Ref::Cache_Ref");
    //DJV_DEBUG_PRINT("alive = " << ref_alive);
}

Cache_Ref::~Cache_Ref()
{
    --ref_alive;

    //DJV_DEBUG("Cache_Ref::~Cache_Ref");
    //DJV_DEBUG_PRINT("alive = " << ref_alive);
}

Image * Cache_Ref::get()
{
    return _image.get();
}

void Cache_Ref::key(const void * in)
{
    _key = in;
}

const void * Cache_Ref::key() const
{
    return _key;
}

int64_t Cache_Ref::frame() const
{
    return _frame;
}

void Cache_Ref::ref_inc()
{
    ++_ref_count;
}

void Cache_Ref::ref_del()
{
    --_ref_count;
}

int Cache_Ref::ref_count() const
{
    return _ref_count;
}

//------------------------------------------------------------------------------
// Cache
//------------------------------------------------------------------------------

Cache::Cache() :
    signal(this),
    _cache_max(default_size()[4] * Memory::megabyte),
    _cache_size(0),
    _type(CACHE_LRU_PLAYBACK)
{
    //DJV_DEBUG("Cache::Cache");
}

Cache::~Cache()
{
    //DJV_DEBUG("Cache::~Cache");

#if defined(DJV_DEBUG)
    debug();
#endif

    // Cleanup.

    const Ref_List::iterator end = _refs.end();

    for (Ref_List::iterator i = _refs.begin(); i != end; ++i)
    {
        delete *i;
    }
}

Cache_Ref * Cache::create(Image * image, const void * key, int64_t frame)
{
    //DJV_DEBUG("Cache::create");
    //DJV_DEBUG_PRINT("image = " << *image);
    //DJV_DEBUG_PRINT("key = " << reinterpret_cast<int64_t>(key));
    //DJV_DEBUG_PRINT("frame = " << frame);

#if defined(DJV_DEBUG)
    debug();
#endif

    // Create a new reference.

    Cache_Ref * out = new Cache_Ref(image, key, frame);

    // Add the reference to the end of the list.

    _refs += out;

    // Update the cache size.

    _cache_size += out->get()->bytes_data();

    if (_cache_size > _cache_max)
    {
        purge();
    }

    signal.emit(true);

#if defined(DJV_DEBUG)
    debug();
#endif

    return out;
}

Cache_Ref * Cache::get(const void * key, int64_t frame)
{
    //DJV_DEBUG("Cache::get");
    //DJV_DEBUG_PRINT("key = " << reinterpret_cast<int64_t>(key));
    //DJV_DEBUG_PRINT("frame = " << frame);

#if defined(DJV_DEBUG)
    //debug();
#endif

    Cache_Ref * out = 0;

    const Ref_List::iterator end = _refs.end();

    for (Ref_List::iterator i = _refs.begin(); i != end; ++i)
        if (key == (*i)->key() && frame == (*i)->frame())
        {
            // Found the reference.

            out = *i;

            // Increment the reference count.

            out->ref_inc();

            // Move the reference to the end of the list.

            _refs.erase(i);

            _refs += out;

            break;
        }

#if defined(DJV_DEBUG)
    debug();
#endif

    //DJV_DEBUG_PRINT("out = " <<
    //  (out ? reinterpret_cast<int64_t>(out->key()) : 0));

    return out;
}

void Cache::del(const void * key)
{
    //DJV_DEBUG("Cache::del");
    //DJV_DEBUG_PRINT("key = " << reinterpret_cast<int64_t>(key));
#if defined(DJV_DEBUG)
    debug();
#endif

    // Find null references matching key.

    List<Ref_List::iterator> dels;

    const Ref_List::iterator end = _refs.end();

    for (Ref_List::iterator i = _refs.begin(); i != end; ++i)
        if (
            ! (*i)->ref_count() &&
            key == (*i)->key())
        {
            dels.push_front(i);
        }

    // Delete the null references.

    //DJV_DEBUG_PRINT("delete list = " << dels.size());

    for (size_t i = 0; i < dels.size(); ++i)
    {
        _cache_size -= (*dels[i])->get()->bytes_data();

        delete *dels[i];

        _refs.erase(dels[i]);
    }

    // Initialize any remaining references matching key; they will be cleaned
    // up later.

    const Ref_List::const_iterator end2 = _refs.end();

    for (Ref_List::const_iterator i = _refs.begin(); i != end2; ++i)
        if (key == (*i)->key())
        {
            (*i)->key(0);
        }

    // Signal that the cache has changed.

    signal.emit(true);

#if defined(DJV_DEBUG)
    debug();
#endif
}

void Cache::del()
{
    //DJV_DEBUG("Cache::del");

#if defined(DJV_DEBUG)
    debug();
#endif

    // Find all null references.

    List<Ref_List::iterator> dels;

    const Ref_List::iterator end = _refs.end();

    for (Ref_List::iterator i = _refs.begin(); i != end; ++i)
        if (! (*i)->ref_count())
        {
            dels.push_front(i);
        }

    // Delete the null references.

    //DJV_DEBUG_PRINT("delete list = " << dels.size());

    for (size_t i = 0; i < dels.size(); ++i)
    {
        _cache_size -= (*dels[i])->get()->bytes_data();

        delete *dels[i];

        _refs.erase(dels[i]);
    }

    // Signal that the cache has changed.

    signal.emit(true);

#if defined(DJV_DEBUG)
    debug();
#endif
}

List<int64_t> Cache::frames(const void * key)
{
    List<int64_t> out;

    const Ref_List::const_iterator end = _refs.end();

    for (Ref_List::const_iterator i = _refs.begin(); i != end; ++i)
        if (key == (*i)->key())
        {
            out += (*i)->frame();
        }

    std::sort(out.begin(), out.end());

    return out;
}

void Cache::max(int in)
{
    //DJV_DEBUG("Cache::max");
    //DJV_DEBUG_PRINT("in = " << in);
#if defined(DJV_DEBUG)
    debug();
#endif

    _cache_max = in * Memory::megabyte;

    //if (_cache_size > _cache_max)
    purge();

#if defined(DJV_DEBUG)
    debug();
#endif
}

int Cache::max() const
{
    return static_cast<int>(_cache_max / Memory::megabyte);
}

int Cache::size(const void * key) const
{
    uint64_t size = 0;

    const Ref_List::const_iterator end = _refs.end();

    for (Ref_List::const_iterator i = _refs.begin(); i != end; ++i)
        if (key == (*i)->key())
        {
            size += (*i)->get()->bytes_data();
        }

    return static_cast<int>(size / Memory::megabyte);
}

int Cache::size() const
{
    return static_cast<int>(_cache_size / Memory::megabyte);
}

void Cache::type(CACHE in)
{
    _type = in;
}

Cache::CACHE Cache::type() const
{
    return _type;
}

const List<int> & Cache::default_size()
{
    static const List<int> data = List<int>() <<
                                  200 <<
                                  400 <<
                                  600 <<
                                  800 <<
                                  1000 <<
                                  2000 <<
                                  3000 <<
                                  4000 <<
                                  5000 <<
                                  6000 <<
                                  7000 <<
                                  8000;

    return data;
}

const List<String> & Cache::label_size()
{
    static const List<String> data = List<String>() <<
        "200" <<
        "400" <<
        "600" <<
        "800" <<
        "1000" <<
        "2000" <<
        "3000" <<
        "4000" <<
        "5000" <<
        "6000" <<
        "7000" <<
        "8000";

    return data;
}

const List<String> & Cache::label_cache()
{
    static const List<String> data = List<String>() <<
        "LRU" <<
        "LRU + Playback";

    DJV_ASSERT(data.size() == _CACHE_SIZE);

    return data;
}

void Cache::purge()
{
    //DJV_DEBUG("Cache::purge");
    //DJV_DEBUG_PRINT("type = " << _type);

#if defined(DJV_DEBUG)
    debug();
#endif

    // Find null references we can delete to bring the cache size below the
    // maximum size.

    List<Ref_List::iterator> dels;

    switch (_type)
    {
        case CACHE_LRU:
            break;

        case CACHE_LRU_PLAYBACK:
        {
            // Find null references that are before the current frame.

            const int64_t frame =
                _refs.begin() != _refs.end() ?
                (_refs.back()->frame()) :
                0;

            //DJV_DEBUG_PRINT("frame = " << frame);

            const Ref_List::iterator end = _refs.end();

            for (Ref_List::iterator i = _refs.begin(); i != end; ++i)
            {
                if (
                    ! (*i)->ref_count() &&
                    (*i)->frame() < frame &&
                    _cache_size > _cache_max)
                {
                    _cache_size -= (*i)->get()->bytes_data();

                    dels.push_front(i);
                }
            }

            // Delete the null references.

            //DJV_DEBUG_PRINT("delete list = " << dels.size());

            for (size_t i = 0; i < dels.size(); ++i)
            {
                delete *dels[i];

                _refs.erase(dels[i]);
            }

            dels.clear();
        }
        break;

        default:
            break;
    }

    const Ref_List::iterator end = _refs.end();

    for (Ref_List::iterator i = _refs.begin(); i != end; ++i)
    {
        if (
            ! (*i)->ref_count() &&
            _cache_size > _cache_max)
        {
            _cache_size -= (*i)->get()->bytes_data();

            dels.push_front(i);
        }
    }

    // Delete the null references.

    //DJV_DEBUG_PRINT("delete list = " << dels.size());

    for (size_t i = 0; i < dels.size(); ++i)
    {
        delete *dels[i];

        _refs.erase(dels[i]);
    }

    // Signal that the cache has changed.

    signal.emit(true);

#if defined(DJV_DEBUG)
    debug();
#endif
}

Cache * Cache::global()
{
    static Cache * global = 0;

    if (! global)
    {
        global = new Cache;
    }

    return global;
}

void Cache::debug()
{
    //DJV_DEBUG("Cache::debug");
    //DJV_DEBUG_PRINT("refs = " << _refs.size());
    //DJV_DEBUG_PRINT("cache max = " <<
    //    static_cast<int>(_cache_max / Memory::megabyte));
    //DJV_DEBUG_PRINT("cache size = " <<
    //    static_cast<int>(_cache_size / Memory::megabyte));

    //const Ref_List::const_iterator end = _refs.end();
    //for (Ref_List::const_iterator i = _refs.begin(); i != end; ++i)
    //    DJV_DEBUG_PRINT(
    //        "item (ref = " << (*i)->ref_count() << ") = " <<
    //        reinterpret_cast<int64_t>((*i)->key()) << " " <<
    //        (*i)->frame());
}

//------------------------------------------------------------------------------

_DJV_STRING_OPERATOR_LABEL(Cache::CACHE, Cache::label_cache())

} // djv_view

