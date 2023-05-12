#include "lib_cppgeometry/Polygon.hpp"
#include "lib_cppgeometry/functions.hpp"

void test1();
void test2();
void test3();
void test4();
void test5();
int main(int argc, char const* argv[])
{
   test1();
   // test2();
   // test3();
   test4();
   // test5();
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
   std::vector<Point> test = { Point(0.945057, 6.62),
                               Point(7.88, 6.62),
                               Point(7.88, 1.31931),
                               Point(0.945057, 1.31931) };
   auto i = Polygon(test);
   auto ptr =
     i.segmentInsidePolygon(LineSegment(Point(3.91, 2.78), Point(7.17, 0.81)),
                            ClipSegmentMethod::SPROULE_SUTHERLAND);
   /** out is
      (3.91, 2.78)
      (0.948644, 4.56953)
    */
   double precision = 0.5;
   bool correctOut = ptr != nullptr &&
                     areEqual(ptr->getBegin()["x"], 3.9, precision) &&
                     areEqual(ptr->getBegin()["y"], 2.7, precision) &&
                     areEqual(ptr->getEnd()["x"], 0.9, precision) &&
                     areEqual(ptr->getEnd()["y"], 4.5, precision);
   if (!correctOut) {
      throw std::runtime_error("test 3 error");
   } else {
      std::cout << "test 3 success\n";
   }
}
void test4()
{
   std::vector<Point> test = { Point(4.17429, 4.70855),
                               Point(5.37042, 4.70855),
                               Point(5.37042, 8.18),
                               Point(4.17429, 8.18) };
   auto i = Polygon(test);
   /** out is
      nullptr (segment outside area)
    */
   auto segment = LineSegment(Point(9.23, 0.39), Point(5.13, 4.24));
   auto ptr1 =
     i.segmentInsidePolygon(segment, ClipSegmentMethod::SPROULE_SUTHERLAND);

   auto ptr2 =
     i.segmentInsidePolygon(segment, ClipSegmentMethod::COHEN_SUTHERLAND);

   auto ptr3 = i.segmentInsidePolygon(segment, ClipSegmentMethod::CYRUS_BECK);
   if (ptr1 != nullptr || ptr2 != nullptr || ptr3 != nullptr) {
      throw std::runtime_error(
        "test 4 error (" + ((ptr1 == nullptr) ? std::string("ptr1=null") : "") +
        ((ptr2 == nullptr) ? std::string("ptr2=null ") : "") +
        ((ptr3 == nullptr) ? std::string("ptr3=null ") : "") + ")");
   } else {
      std::cout << "test 4 success\n";
   }
}
void test5()
{
   std::vector<Point> test = { Point(4.21, 9.01271),
                               Point(9.78787, 9.01271),
                               Point(9.78787, 1.28),
                               Point(4.21, 1.28)

   };
   auto i = Polygon(test);
   // All segment must be inside polygon
   auto segment = LineSegment(Point(7.17, 8.79), Point(6.68, 8.71));
   auto ptr =
     i.segmentInsidePolygon(segment, ClipSegmentMethod::SPROULE_SUTHERLAND);
   double precision = 0.5;
   bool correctOut =
     ptr != nullptr &&
     areEqual(ptr->getBegin()["x"], segment.getBegin()["x"], precision) &&
     areEqual(ptr->getBegin()["y"], segment.getBegin()["y"], precision) &&
     areEqual(ptr->getEnd()["x"], segment.getEnd()["x"], precision) &&
     areEqual(ptr->getEnd()["y"], segment.getEnd()["x"], precision);
   if (!correctOut) {
      throw std::runtime_error("test 3 error");
   } else {
      std::cout << "test 3 success\n";
   }
}
