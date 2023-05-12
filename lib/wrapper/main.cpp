#include "lib_cppgeometry/Polygon.hpp"
#include "lib_cppgeometry/functions.hpp"

void test1();
void test2();
void test3();
void test4();
void test5();

bool almost_equal(std::unique_ptr<LineSegment> source,
                  std::unique_ptr<LineSegment> result, double precision);

int main(int argc, char const* argv[])
{
   test1();
   test2();
   test3();
   test4();
   test5();
   return 0;
}

void test1()
{
   std::vector<Point> test = { Point(4.52659, 5.66419),
                               Point(5.62353, 5.66419),
                               Point(5.62353, 9.77856),
                               Point(4.52659, 9.77856) };
   auto i = Polygon(test);
   LineSegment segment(Point(4.79, 5.84), Point(9.85, 3.67));
   auto ptr1 =
     i.segmentInsidePolygon(segment, ClipSegmentMethod::COHEN_SUTHERLAND);
   auto ptr2 =
     i.segmentInsidePolygon(segment, ClipSegmentMethod::SPROULE_SUTHERLAND);

   auto ptr3 = i.segmentInsidePolygon(segment, ClipSegmentMethod::CYRUS_BECK);
   /** type: out
            (4.79, 5.84)
            (5.2, 5.66417)
    */
   double precision = 0.5;
   auto input = std::make_unique<LineSegment>(segment);
   bool method1 = almost_equal(std::move(input), std::move(ptr1), precision),
        method2 = almost_equal(std::move(input), std::move(ptr2), precision),
        method3 = almost_equal(std::move(input), std::move(ptr3), precision);
   std::string str = "";
   if (!method1)
      str += "COHEN_SUTHERLAND ";
   if (!method2)
      str += "SPROULE_SUTHERLAND ";
   if (!method3)
      str += "CYRUS_BECK ";
   if (!method1 || !method2 || !method3) {
      throw std::runtime_error("test 3 error: " + str + "failed.");
   } else {
      std::cout << "test 3 success\n";
   }
}

void test2()
{
   std::vector<Point> test = { Point(8.28809, 2.9038),
                               Point(7.05812, 2.9038),
                               Point(7.05812, 9.74),
                               Point(8.28809, 9.74) };
   auto i = Polygon(test);
   auto ptr =
     i.segmentInsidePolygon(LineSegment(Point(8.64, 8.92), Point(6, 3.5)),
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
   std::vector<Point> test = { Point(2.53954, 10.0967),
                               Point(7.75622, 10.0967),
                               Point(7.75622, 14.921),
                               Point(2.53954, 14.921) };
   auto i = Polygon(test);
   /** out is
      nullptr (segment outside area)
    */
   auto segment = LineSegment(Point(8.31, 2.96), Point(4.91, 8.77));
   auto ptr1 =
     i.segmentInsidePolygon(segment, ClipSegmentMethod::COHEN_SUTHERLAND);

   auto ptr2 =
     i.segmentInsidePolygon(segment, ClipSegmentMethod::SPROULE_SUTHERLAND);

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
   if (!almost_equal(
         std::make_unique<LineSegment>(segment), std::move(ptr), precision)) {
      throw std::runtime_error("test 3 error");
   } else {
      std::cout << "test 3 success\n";
   }
}

bool almost_equal(std::unique_ptr<LineSegment> source,
                  std::unique_ptr<LineSegment> result, double precision)
{
   bool pre = (result == nullptr && source == nullptr);
   return pre ||
          (result != nullptr && source != nullptr &&
           areEqual(
             source->getBegin()["x"], result->getBegin()["x"], precision) &&
           areEqual(
             source->getBegin()["y"], result->getBegin()["y"], precision) &&
           areEqual(source->getEnd()["x"], result->getEnd()["x"], precision) &&
           areEqual(source->getEnd()["y"], result->getEnd()["x"], precision));
}
