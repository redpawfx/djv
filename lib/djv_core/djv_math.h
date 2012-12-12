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

//! \file djv_math.h

#ifndef DJV_MATH_H
#define DJV_MATH_H

#include <djv_string.h>

namespace djv
{

//------------------------------------------------------------------------------
//! \struct Math
//!
//! This struct provides math utilities.
//!
//! References:
//!
//! - Andrew Glassner, "Graphics Gems"
//! - David S. Ebert, F. Kenton Musgrave, Darwyn Peachey, Ken Perlin,
//!   Steven Worley, "Texturing & Modeling, A Procedural Approach"
//------------------------------------------------------------------------------

struct DJV_CORE_EXPORT Math
{
    static const double pi;    //!< Pi
    static const double pi_x2; //!< Pi times two
    static const double pi_2;  //!< Pi divided by two
    static const double pi_4;  //!< Pi divided by four

    //! Get the absolute value.

    template<typename T>
    static inline T abs(T);

    //! Raise a value to a power.

    static int pow(int in, int value);

    //! Raise a value to a power.

    static double pow(double in, double value);

    //! Get the square root of a value.

    static double sqrt(double);

    //! Exponential.

    static double exp(double);

    //! Logarithm.

    static double log(double);

    //! Logarithm.

    static double log_10(double);

    //! Logarithm.

    static double anti_log_10(double);

    //! Modulus.

    template<typename T>
    static inline T mod(T, T mod);

    //! Wrap a number between two values.

    template<typename T>
    static inline T wrap(T in, T min, T max);

    //! Step function.

    template<typename T>
    static inline T step(T in, T value);

    //! Pulse function.

    template<typename T>
    static inline T pulse(T in, T min, T max);

    //! Bias function.

    static double bias(double in, double value);

    //! Gain function.

    static double gain(double in, double value);

    //! Return the minimum of two values.

    template<typename T>
    static inline T min(const T &, const T &);

    //! Return the minimum of two values.

    template<typename T>
    static inline void min(const T &, const T &, T *);

    //! Return the maximum of two values.

    template<typename T>
    static inline T max(const T &, const T &);

    //! Return the maximum of two values.

    template<typename T>
    static inline void max(const T &, const T &, T *);

    //! Clamp a number between two values.

    template<typename T>
    static inline T clamp(T, T min, T max);

    //! Linear interpolation.

    template<typename T, typename U>
    static inline T lerp(U in, T min, T max);

    //! Smooth step function.

    static inline double smooth_step(double in, double min, double max);

    //! Soft clip function.
    //!
    //! References:
    //!
    //! - Ken McGaugh, "Re: [Openexr-devel] Cineon -> half conversion?"
    //!   OpenEXR Mailing List 3/2/05

    static double soft_clip(double in, double value);

    //! Sine function.

    static double sin(double);

    //! Cosine function.

    static double cos(double);

    //! Secant function.

    static double sec(double);

    //! Tangent function.

    static double tan(double);

    //! Inverse sine function.

    static double arc_sin(double);

    //! Inverse cosine function.

    static double arc_cos(double);

    //! Inverse tangent function.

    static double arc_tan(double);

    //! Inverse tangent function.

    static double arc_tan(double, double);

    //! Cosecant function.

    static double co_sec(double);

    //! Cotangent function.

    static double co_tan(double);

    //! Get a random number between zero and one.

    static double rand();

    //! Get a random number between zero and the given value.

    static double rand(double);

    //! Get a random number between two values.

    static double rand(double min, double max);

    //! Seed the random value generator. A value of zero uses a random seed.

    static void rand_seed(uint = 0);

    //! Round to the smallest integer value.

    //static inline int ceil(double);
    static int ceil(double);

    //! Round to the largest integer value.

    //static inline int floor(double);
    static int floor(double);

    //! Round to an integer value.

    static int round(double);

    //! Get the value after the decimal.

    static inline double fraction(double);

    //! Find the closest power of two.

    static inline int to_pow2(int);

    //! Convert degrees to radians.

    static inline double degrees_to_radians(double);

    //! Convert radians to degrees.

    static inline double radians_to_degrees(double);
};

} // djv

#include <djv_math_inline.h>

#endif // DJV_MATH_H

