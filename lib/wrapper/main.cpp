#include "lib_cppgeometry/Polygon.hpp"

void test1();
void test2();
void test3();
int main(int argc, char const* argv[])
{
   test1();
   test2();
   test3();
   return 0;
}

void test1()
{
   std::vector<Point> test = { Point(3.51208, 4.03),
                               Point(5.42009, 4.03),
                               Point(5.42009, 9.15576),
                               Point(3.51208, 9.15576) };
   auto i = Polygon(test);
   auto ptr =
     i.segmentInsidePolygon(LineSegment(Point(8.84, 8.35), Point(6.47, 7.52)),
                            ClipSegmentMethod::COHEN_SUTHERLAND);
}

void test2()
{
   std::vector<Point> test = { Point(2.36, 9.36613),
                               Point(7.88037, 9.36613),
                               Point(7.88037, 3.43),
                               Point(2.36, 3.43) };
   auto i = Polygon(test);
   auto ptr =
     i.segmentInsidePolygon(LineSegment(Point(4.69, 8.62), Point(5.98, 2.38)),
                            ClipSegmentMethod::COHEN_SUTHERLAND);
}

void test3()
{
   std::vector<Point> test = { Point(2.36, 9.36613),
                               Point(7.88037, 9.36613),
                               Point(7.88037, 3.43),
                               Point(2.36, 3.43) };
   auto i = Polygon(test);
   auto ptr =
     i.segmentInsidePolygon(LineSegment(Point(4.69, 8.62), Point(5.98, 2.38)),
                            ClipSegmentMethod::SPROULE_SUTHERLAND);
   if (ptr == nullptr) {
      throw std::runtime_error("test3 error");
   }
}
