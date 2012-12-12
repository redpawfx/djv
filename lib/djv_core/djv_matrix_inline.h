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

//! \file djv_matrix_inline.h

namespace djv
{

//------------------------------------------------------------------------------
// M<T, D>
//------------------------------------------------------------------------------

template<typename T, int D>
const int M<T, D>::dimension = D;

template<typename T, int D>
const int M<T, D>::dimension_x2 = D * D;

template<typename T, int D>
inline M<T, D>::M()
{
    identity();
}

template<typename T, int D>
inline M<T, D>::M(const M<T, D> & in)
{
    set(in);
}

template<typename T, int D>
inline M<T, D>::M(const T * in)
{
    set(in);
}

template<typename T, int D>
inline void M<T, D>::set(const M<T, D> & in)
{
    for (int i = 0; i < dimension_x2; ++i)
    {
        e[i] = in.e[i];
    }
}

template<typename T, int D>
inline void M<T, D>::set(const T * in)
{
    for (int i = 0; i < dimension_x2; ++i)
    {
        e[i] = in[i];
    }
}

template<typename T, int D>
inline void M<T, D>::identity()
{
    for (int i = 0; i < D; ++i)
        for (int j = 0; j < D; ++j)
        {
            e[i * D + j] = (i == j) ? 1.0 : 0.0;
        }
}

template<typename T, int D>
inline void M<T, D>::zero()
{
    for (int i = 0; i < dimension_x2; ++i)
    {
        e[i] = 0.0;
    }
}

//------------------------------------------------------------------------------
// Matrix_Util
//------------------------------------------------------------------------------

template<typename T, int D>
inline M<T, D> Matrix_Util::transpose(const M<T, D> & in)
{
    M<T, D> out;

    for (int i = 0; i < D; ++i)
        for (int j = 0; j < D; ++j)
        {
            out.e[j * D + i] = in.e[i * D + j];
        }

    return out;
}

template<typename T, int D, int D2>
inline M<T, D> Matrix_Util::translate(const V<T, D2> & in)
{
    //DJV_ASSERT((D - 1) == D2);

    M<T, D> out;

    for (int i = 0; i < D2; ++i)
    {
        out.e[(D - 1) * D + i] = in.e[i];
    }

    return out;
}

inline M3f Matrix_Util::translate3f(const V2f & in)
{
    return translate<double, 3, 2>(in);
}

template<typename T, int D, int D2>
inline M<T, D> Matrix_Util::scale(const V<T, D2> & in)
{
    //DJV_ASSERT((D - 1) == D2);

    M<T, D> out;

    for (int i = 0; i < D2; ++i)
    {
        out.e[i * D + i] = in.e[i];
    }

    return out;
}

inline M3f Matrix_Util::scale3f(const V2f & in)
{
    return scale<double, 3, 2>(in);
}

inline M3f Matrix_Util::rotate3f(double in)
{
    const double cos = Math::cos(Math::degrees_to_radians(in));
    const double sin = Math::sin(Math::degrees_to_radians(in));

    return M3f(
        cos,  sin, 0,
        -sin, cos, 0,
        0,    0,   1);
}

template <typename T, typename T2, int D>
inline M<T2, D> Matrix_Util::convert(const M<T, D> & in)
{
    M<T2, D> out;

    for (int i = 0; i < out.dimension_x2; ++i)
    {
        out.e[i] = T2(in.e[i]);
    }

    return out;
}

//! \todo Probably wrong.

template <typename T>
inline M<T, 4> Matrix_Util::matrix4(const M<T, 3> & in)
{
    return M<T, 4>(
        in.e[0], in.e[1], in.e[2], 0.0,
        in.e[3], in.e[4], in.e[5], 0.0,
        0.0,     0.0,     in.e[8], 0.0,
        in.e[6], in.e[7], 0.0,     1.0);
}

//------------------------------------------------------------------------------

template<typename T, int D>
inline M<T, D> operator * (const M<T, D> & a, const M<T, D> & b)
{
    M<T, D> out;

    out.zero();

    for (int i = 0; i < D; ++i)
        for (int j = 0; j < D; ++j)
            for (int k = 0; k < D; ++k)
            {
                out.e[i * D + j] += a.e[i * D + k] * b.e[k * D + j];
            }

    return out;
}

template<typename T, int D, int D2>
inline V<T, D2> operator * (const M<T, D> & a, const V<T, D2> & b)
{
    //DJV_ASSERT((D - 1) == D2);

    V<T, D2> out;

    for (int i = 0; i < D2; ++i)
    {
        int j = 0;

        for (; j < D2; ++j)
        {
            out.e[i] += a.e[j * D + i] * b.e[j];
        }

        out.e[i] += a.e[j * D + i];
    }

    return out;
}

template<typename T>
inline Box<T, 2> operator * (const M<T, 3> & a, const Box<T, 2> & b)
{
    const V<T, 2> lower_right = b.lower_right();

    V<T, 2> pt [] =
    {
        b.position,
        b.position,
        lower_right,
        b.position
    };
    pt[1].y = lower_right.y;
    pt[3].x = lower_right.x;

    Box<T, 2> out;

    for (int i = 0; i < 4; ++i)
    {
        pt[i] = a * pt[i];

        if (0 == i)
        {
            out.position = pt[i];
        }
        else
        {
            out = Box_Util::expand(out, pt[i]);
        }
    }

    return out;
}

template<typename T, int D>
inline String & operator << (String & out, const M<T, D> & in)
{
    for (int i = 0; i < in.dimension_x2; ++i)
    {
        out << in.e[i];
    }

    return out;
}

template<typename T, int D>
inline String & operator >> (String & in, M<T, D> & out) throw (String)
{
    for (int i = 0; i < out.dimension_x2; ++i)
    {
        in >> out.e[i];
    }

    return in;
}

template<typename T, int D>
inline bool operator == (const M<T, D> & a, const M<T, D> & b)
{
    for (int i = 0; i < a.dimension_x2; ++i)
        if (a.e[i] != b.e[i])
        {
            return false;
        }

    return true;
}

template<typename T, int D>
inline bool operator != (const M<T, D> & a, const M<T, D> & b)
{
    return ! (a == b);
}

template<typename T, int D>
inline Debug & operator << (Debug & debug, const M<T, D> & in)
{
    debug << "\n";

    for (int i = 0; i < D; ++i)
    {
        for (int j = 0; j < D; ++j)
        {
            debug << in.e[i * D + j];

            if (j < D - 1)
            {
                debug << " ";
            }
        }

        if (i < D - 1)
        {
            debug << "\n";
        }
    }

    return debug;
}

} // djv

