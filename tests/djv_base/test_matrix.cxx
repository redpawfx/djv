#define DJV_DEBUG

#include <djv_base/matrix.h>

using namespace djv_base;

void components()
{  
  DJV_DEBUG("components");

  M3f mtx3f;
  DJV_DEBUG_PRINT("mtx3f = " << mtx3f);
}

void transpose()
{  
  DJV_DEBUG("transpose");

  DJV_ASSERT(
    matrix::transpose(M3f(0, 1, 2, 3, 4, 5, 6, 7, 8)) ==
    M3f(0, 3, 6,
        1, 4, 7,
        2, 5, 8)
  );
}

void translate()
{
  DJV_DEBUG("translate");

  DJV_ASSERT(
    matrix::translate3f(V2f(1, 2)) ==
    M3f(1, 0, 0,
        0, 1, 0,
        1, 2, 1)
  );
}

void scale()
{
  DJV_DEBUG("scale");

  DJV_ASSERT(
    matrix::scale3f(V2f(2.0, 4.0)) ==
    M3f(2, 0, 0,
        0, 4, 0,
        0, 0, 1)
  );
}

void rotate()
{
  /*DJV_DEBUG("rotate");

  DJV_ASSERT(
    matrix::rotate3f(90.0) ==
    M3f( 0, 1, 0,
        -1, 0, 0,
         0, 0, 1)
  );*/
}

void comparison()
{  
  DJV_DEBUG("comparison");

  DJV_ASSERT(
    M3f(0, 1, 2, 3, 4, 5, 6, 7, 8) ==
    M3f(0, 1, 2, 3, 4, 5, 6, 7, 8)
  );
  DJV_ASSERT(
    M3f(0, 1, 2, 3, 4, 5, 6, 7, 8) !=
    M3f(8, 7, 6, 5, 4, 3, 2, 1, 0)
  );
}

int main(int argc, char ** argv)
{
  components();
  transpose();
  translate();
  scale();
  rotate();
  comparison();

  return 0;
}

