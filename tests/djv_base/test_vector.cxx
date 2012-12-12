#define DJV_DEBUG

#include <djv_base/vector.h>

using namespace djv_base;

void components()
{
  DJV_DEBUG("components");

  V2i vec2i(1, 2);
  V2f vec2f(1.0, 2.0);
  
  DJV_DEBUG_PRINT("vec2i = " << vec2i);
  DJV_DEBUG_PRINT("vec2f = " << vec2f);  
  DJV_ASSERT(1 == vec2i.x);
  DJV_ASSERT(2 == vec2i.y);
  DJV_ASSERT(1.0 == vec2f.x);
  DJV_ASSERT(2.0 == vec2f.y);

  DJV_ASSERT(V2i(V2f(1.0, 2.0)) == V2i(1, 2));
  DJV_ASSERT(V2f(V2i(1, 2)) == V2f(1.0, 2.0));
}

void abs()
{  
  DJV_DEBUG("abs");

  DJV_ASSERT(vector::abs(V2i(-1, -2)) == V2i(1, 2));
}

void swap()
{  
  DJV_DEBUG("swap");

  DJV_ASSERT(vector::swap(V2i(1, 2)) == V2i(2, 1));
}

void range()
{  
  DJV_DEBUG("min, max, clamp");

  DJV_ASSERT(vector::min(V2i(1, 2), V2i(3, 4)) == V2i(1, 2));
  DJV_ASSERT(vector::max(V2i(1, 2), V2i(3, 4)) == V2i(3, 4));
  DJV_ASSERT(
    vector::clamp(V2i(-10, 10), V2i(1, 2), V2i(3, 4)) == V2i(1, 4));
}

void is_size_valid()
{    
  DJV_DEBUG("is_size_valid");
  
  DJV_ASSERT(
    vector::is_size_valid(V2i(1, 2)) &&
    ! vector::is_size_valid(V2i(-1, -2))
  );
}

void length()
{
  DJV_DEBUG("length");
  
  DJV_ASSERT(vector::length(V2i(0, 2)) == 2);
}

void normalize()
{
  DJV_DEBUG("normalize");
  
  DJV_ASSERT(vector::normalize(V2f(0.0, 2.0)) == V2f(0.0, 1.0));
}

void dot()
{
  DJV_DEBUG("dot");
  
  DJV_ASSERT(vector::dot(V2f(1.0, 0.0), V2f(0.0, 1.0)) == 0.0);
}

void aspect()
{
  DJV_DEBUG("aspect");
  
  DJV_ASSERT(vector::aspect(V2i(2, 1)) == 2.0);
}

void arithmetic()
{
  DJV_DEBUG("arithemtic");
  
  V2i vec2i = V2i(1, 2);
  vec2i += V2i(1, 2);
  DJV_ASSERT(V2i(2, 4) == vec2i);
  vec2i -= V2i(1, 2);
  DJV_ASSERT(V2i(1, 2) == vec2i);
  vec2i *= V2i(1, 2);
  DJV_ASSERT(V2i(1, 4) == vec2i);
  vec2i /= V2i(1, 2);
  DJV_ASSERT(V2i(1, 2) == vec2i);

  DJV_ASSERT(V2i(1, 2) + V2i(1, 2) == V2i(2, 4));
  DJV_ASSERT(V2i(1, 2) - V2i(1, 2) == V2i(0, 0));
  DJV_ASSERT(V2i(1, 2) * V2i(1, 2) == V2i(1, 4));
  DJV_ASSERT(V2i(1, 2) / V2i(1, 2) == V2i(1, 1));
  
  DJV_ASSERT(-V2i(1, 2) == V2i(-1, -2));
}

void comparison()
{
  DJV_DEBUG("comparison");
  
  DJV_ASSERT(V2i(1, 2) == V2i(1, 2));
  DJV_ASSERT(V2i(1, 2) != V2i(0, 0));
  DJV_ASSERT(V2i(1, 2) <  V2i(2, 3));
  DJV_ASSERT(V2i(1, 2) <= V2i(1, 2));
  DJV_ASSERT(V2i(2, 3) >  V2i(1, 2));
  DJV_ASSERT(V2i(1, 2) >= V2i(1, 2));
}

void conversion()
{
  DJV_DEBUG("conversion");
  
  V2i vec2i = vector::convert<double, int, 2>(V2f(1.0, 2.0));
  DJV_ASSERT(vec2i == V2i(1, 2));
  vec2i = vector::ceil<double, int, 2>(V2f(0.5, 0.5));
  DJV_ASSERT(vec2i == V2i(1, 1));
  vec2i = vector::floor<double, int, 2>(V2f(0.5, 0.5));
  DJV_ASSERT(vec2i == V2i(0, 0));

  DJV_ASSERT("1 2" == string::label(V2i(1, 2)));
}

int main(int argc, char ** argv)
{
  components();
  abs();
  swap();
  range();
  is_size_valid();
  length();
  normalize();
  dot();
  aspect();
  arithmetic();
  comparison();
  conversion();
  
  return 0;
}

