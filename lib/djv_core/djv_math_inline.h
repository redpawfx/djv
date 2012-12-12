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

//! \file djv_math_inline.h

namespace djv
{

//------------------------------------------------------------------------------
// Math
//------------------------------------------------------------------------------

template<class T>
inline T Math::abs(T in)
{
    return in < T(0) ? -in : in;
}

template<class T>
inline T Math::mod(T in, T mod)
{
    T tmp = in;

    if (mod != T(0))
    {
        tmp = in - (static_cast<int>(in / mod) * mod);

        if (tmp < T(0))
        {
            tmp += mod;
        }
    }

    return tmp;
}

template<class T>
inline T Math::wrap(T in, T min, T max)
{
    return max > min ?
           (min + mod(in - min, max - min + static_cast<T>(1))) :
           min;
}

template<class T>
inline T Math::step(T in, T v)
{
    return in <= v;
}

template<class T>
inline T Math::pulse(T in, T min, T max)
{
    return step(min, in) - step(max, in);
}

template<class T>
inline T Math::min(const T & a, const T & b)
{
    return a < b ? a : b;
}

template<class T>
inline void Math::min(const T & a, const T & b, T * out)
{
    *out = a < b ? a : b;
}

template<class T>
inline T Math::max(const T & a, const T & b)
{
    return a > b ? a : b;
}

template<class T>
inline void Math::max(const T & a, const T & b, T * out)
{
    *out = a > b ? a : b;
}

template<class T>
inline T Math::clamp(T in, T min, T max)
{
    return in < min ? min : (in > max ? max : in);
}

template<class T, class U>
inline T Math::lerp(U in, T min, T max)
{
    return min + T(in * (max - min));
}

inline double Math::smooth_step(double in, double min, double max)
{
    return lerp(in * in * (3.0 - (2.0 * in)), min, max);
}

/*inline int Math::ceil(double in)
{
  const int tmp = static_cast<int>(in);
  return tmp + (in > 0 && in != tmp);
}

inline int Math::floor(double in)
{
  const int tmp = static_cast<int>(in);
  return tmp - (in < 0 && ! in != tmp);
}*/

inline double Math::fraction(double in)
{
    return in - static_cast<int>(in);
}

inline int Math::to_pow2(int in)
{
    int i = 0;

    for (; in > (1 << i); ++i)
        ;

    return 1 << i;
}

inline double Math::degrees_to_radians(double in)
{
    return in * 0.017453;
}

inline double Math::radians_to_degrees(double in)
{
    return in * 57.29578;
}

} // djv

