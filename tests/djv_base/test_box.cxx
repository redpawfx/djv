#define DJV_DEBUG

#include <djv_base/box.h>

using namespace djv_base;

void components()
{  
  DJV_DEBUG("components");
  
  Box2i tmp(1, 2);
  DJV_DEBUG_PRINT(tmp);
  DJV_ASSERT(1 == tmp.w);
  DJV_ASSERT(2 == tmp.h);

  tmp = Box2i(0, 10, 10, 20);
  DJV_DEBUG_PRINT(tmp);
  DJV_ASSERT(0 == tmp.x);
  DJV_ASSERT(10 == tmp.y);
  DJV_ASSERT(10 == tmp.w);
  DJV_ASSERT(20 == tmp.h);
}

void corners()
{
  DJV_DEBUG("corners");
  
  DJV_ASSERT(box::corners(V2i(0, 10), V2i(9, 29)) == Box2i(0, 10, 10, 20));
  DJV_ASSERT(
    box::corners(V2f(0.0, 10.0), V2f(10.0, 30.0)) ==
    Box2f(0.0, 10.0, 10.0, 20.0)
  );
}

void swap()
{
  DJV_DEBUG("swap");
  
  DJV_ASSERT(box::swap(Box2i(0, 10, 10, 20)) == Box2i(10, 0, 20, 10));
}

void intersect()
{
  DJV_DEBUG("intersect");

  DJV_ASSERT(
    box::intersect(Box2i(0, 5, 10, 15), Box2i(5, 10, 15, 20)) ==
    Box2i(5, 10, 5, 10)
  );
  DJV_ASSERT(box::intersect(Box2i(0, 5, 10, 15), V2i(5, 10)));
}

void bound()
{
  DJV_DEBUG("bound");

  DJV_ASSERT(
    box::bound(Box2i(0, 5, 5, 10), Box2i(5, 10, 10, 15)) ==
    Box2i(0, 5, 15, 20)
  );
}

void expand()
{
  DJV_DEBUG("expand");

  DJV_ASSERT(
    box::expand(Box2i(0, 5, 5, 10), V2i(9, 19)) ==
    Box2i(0, 5, 10, 15)
  );
}

void border()
{
  DJV_DEBUG("border");
  
  DJV_ASSERT(
    box::border(Box2i(0, 5, 5, 10), V2i(5, 5)) ==
    Box2i(-5, 0, 15, 20)
  );
}

void is_size_valid()
{
  DJV_DEBUG("is_size_valid");
  
  DJV_ASSERT(box::is_size_valid(Box2i(0, 5, 10, 15)));
}

void arithmetic()
{
  DJV_DEBUG("arithmetic");
  
  DJV_ASSERT(Box2i(0, 1, 2, 3) + 1 == Box2i(1, 2, 3, 4));
  DJV_ASSERT(Box2i(0, 1, 2, 3) - 1 == Box2i(-1, 0, 1, 2));
  DJV_ASSERT(Box2i(0, 1, 2, 3) * 2 == Box2i(0, 2, 4, 6));
  DJV_ASSERT(Box2i(0, 1, 2, 3) / 2 == Box2i(0, 0, 1, 1));
}

void comparison()
{
  DJV_DEBUG("comparison");
  
  DJV_ASSERT(Box2i(0, 1, 2, 3) == Box2i(0, 1, 2, 3));
  DJV_ASSERT(Box2i(0, 1, 2, 3) != Box2i(3, 2, 1, 0));
}

void conversion()
{
  DJV_DEBUG("conversion");
  
  const Box2i tmp = box::convert<double, int, 2>(Box2f(0.0, 1.0, 2.0, 3.0));
  DJV_ASSERT(tmp == Box2i(0, 1, 2, 3));

  DJV_ASSERT("1 2 3 4" == string::label(Box2i(1, 2, 3, 4)));
}

int main(int argc, char ** argv)
{
  components();
  corners();
  swap();
  intersect();
  bound();
  expand();
  border();
  is_size_valid();
  arithmetic();
  comparison();
  conversion();

  return 0;
}

