#include "lib_cppgeometry/Curve.hpp"
#include "lib_cppgeometry/Polygon.hpp"
#include "lib_cppgeometry/functions.hpp"
#include "lib_cppgeometry/Fractals.hpp"

const double precision = 0.5;
void test1();
void test2();
void test3();
void test4();
void test5();
void test6();
void test7();
void frac_test1();
void curve_test1();

bool almost_equal(LineSegment* source, LineSegment* result, double precision);
bool almost_equal(const Point* source, const Point* result, double precision);

int main(int argc, char const* argv[])
{
   test1();
   test2();
   test3();
   test4();
   test5();
   test6();
   test7();
   curve_test1();
   frac_test1();

   return 0;
}

void test1()
{
   std::string testNum = "1";
   std::vector<Point> test = { Point(4.52659, 5.66419),
                               Point(5.62353, 5.66419),
                               Point(5.62353, 9.77856),
                               Point(4.52659, 9.77856) };
   auto i = Polygon(test);
   LineSegment segment(Point(4.79, 5.84), Point(9.85, 3.67));
   auto ptr1 = i.segmentInsidePolygon(
     segment, Polygon::ClipSegmentMethod::COHEN_SUTHERLAND);
   auto ptr2 = i.segmentInsidePolygon(
     segment, Polygon::ClipSegmentMethod::SPROULE_SUTHERLAND);

   auto ptr3 =
     i.segmentInsidePolygon(segment, Polygon::ClipSegmentMethod::CYRUS_BECK);
   /** type: out
            (4.79, 5.84)
            (5.2, 5.66417)
    */
   auto answer =
     std::make_unique<LineSegment>(Point(4.79, 5.84), Point(5.2, 5.66417));
   bool method1 = almost_equal(answer.get(), ptr1.get(), precision),
        method2 = almost_equal(answer.get(), ptr2.get(), precision),
        method3 = almost_equal(answer.get(), ptr3.get(), precision);
   std::string str = "";
   if (!method1)
      str += "COHEN_SUTHERLAND ";
   if (!method2)
      str += "SPROULE_SUTHERLAND ";
   if (!method3)
      str += "CYRUS_BECK ";
   if (!method1 || !method2 || !method3) {
      std::cerr << ("test " + testNum + " error: " + str + "failed\n");
   } else {
      std::cout << "test " + testNum + " success\n";
   }
}
void test2()
{
   std::string testNum = "2";
   std::vector<Point> test = { Point(0.945057, 6.62),
                               Point(7.88, 6.62),
                               Point(7.88, 1.31931),
                               Point(0.945057, 1.31931) };
   auto i = Polygon(test);
   auto ptr =
     i.segmentInsidePolygon(LineSegment(Point(3.91, 2.78), Point(7.17, 0.81)),
                            Polygon::ClipSegmentMethod::SPROULE_SUTHERLAND);
   /** out is
      (3.91, 2.78)
      (0.948644, 4.56953)
    */
   bool correctOut = ptr != nullptr &&
                     areEqual(ptr->getBegin()["x"], 3.9, precision) &&
                     areEqual(ptr->getBegin()["y"], 2.7, precision) &&
                     areEqual(ptr->getEnd()["x"], 0.9, precision) &&
                     areEqual(ptr->getEnd()["y"], 4.5, precision);
   if (!correctOut) {
      std::cerr << ("test " + testNum + " failed\n");
   } else {
      std::cout << "test " + testNum + " success\n";
   }
}
void test3()
{
   std::string testNum = "3";
   std::vector<Point> test = { Point(2.53954, 10.0967),
                               Point(7.75622, 10.0967),
                               Point(7.75622, 14.921),
                               Point(2.53954, 14.921) };
   auto i = Polygon(test);
   /** out is
      nullptr (segment outside area)
    */
   auto segment = LineSegment(Point(8.31, 2.96), Point(4.91, 8.77));
   auto ptr1 = i.segmentInsidePolygon(
     segment, Polygon::ClipSegmentMethod::COHEN_SUTHERLAND);

   auto ptr2 = i.segmentInsidePolygon(
     segment, Polygon::ClipSegmentMethod::SPROULE_SUTHERLAND);

   auto ptr3 =
     i.segmentInsidePolygon(segment, Polygon::ClipSegmentMethod::CYRUS_BECK);
   if (ptr1 != nullptr || ptr2 != nullptr || ptr3 != nullptr) {
      std::cerr << ("test " + testNum + " error (" +
                    ((ptr1 == nullptr) ? std::string("ptr1=null") : "") +
                    ((ptr2 == nullptr) ? std::string("ptr2=null ") : "") +
                    ((ptr3 == nullptr) ? std::string("ptr3=null ") : "") + ")");
   } else {
      std::cout << "test " + testNum + " success\n";
   }
}
void test4()
{
   std::string testNum = "4";
   std::vector<Point> test = { Point(4.21, 9.01271),
                               Point(9.78787, 9.01271),
                               Point(9.78787, 1.28),
                               Point(4.21, 1.28)

   };
   auto i = Polygon(test);
   // All segment must be inside polygon
   auto segment = LineSegment(Point(7.17, 8.79), Point(6.68, 8.71));
   auto ptr = i.segmentInsidePolygon(
     segment, Polygon::ClipSegmentMethod::SPROULE_SUTHERLAND);
   if (!almost_equal(&segment, ptr.get(), precision)) {
      std::cerr << ("test " + testNum + " failed\n");
   } else {
      std::cout << "test " + testNum + " success\n";
   }
}

bool almost_equal(LineSegment* source, LineSegment* result, double precision)
{
   bool pre = (result == nullptr && source == nullptr);
   return pre ||
          (result != nullptr && source != nullptr &&
           areEqual(
             source->getBegin()["x"], result->getBegin()["x"], precision) &&
           areEqual(
             source->getBegin()["y"], result->getBegin()["y"], precision) &&
           areEqual(source->getEnd()["x"], result->getEnd()["x"], precision) &&
           areEqual(source->getEnd()["y"], result->getEnd()["y"], precision));
}

bool almost_equal(const Point* source, const Point* result, double precision)
{
   bool pre = (result == nullptr && source == nullptr);
   return pre || (result != nullptr && source != nullptr &&
                  areEqual((*source)["x"], (*result)["x"], precision) &&
                  areEqual((*source)["y"], (*result)["y"], precision));
}

void test5()
{
   std::string testNum = "5";
   std::vector<Point> test = { Point(7.87263, 3.53123),
                               Point(9.36885, 3.53123),
                               Point(9.36885, 10.1734),
                               Point(7.87263, 10.1734) };
   auto i = Polygon(test);
   LineSegment segment(Point(8.51, 6.84), Point(2.38, 7.88));
   auto ptr1 = i.segmentInsidePolygon(
     segment, Polygon::ClipSegmentMethod::COHEN_SUTHERLAND);
   auto ptr2 = i.segmentInsidePolygon(
     segment, Polygon::ClipSegmentMethod::SPROULE_SUTHERLAND);

   auto ptr3 =
     i.segmentInsidePolygon(segment, Polygon::ClipSegmentMethod::CYRUS_BECK);
   /** type: out
            (8.51, 6.84)
            (7.87263, 6.94813)
    */
   auto answer =
     std::make_unique<LineSegment>(Point(8.51, 6.84), Point(7.87263, 6.94813));
   bool method1 = almost_equal(answer.get(), ptr1.get(), precision),
        method2 = almost_equal(answer.get(), ptr2.get(), precision),
        method3 = almost_equal(answer.get(), ptr3.get(), precision);
   std::string str = "";
   if (!method1)
      str += "COHEN_SUTHERLAND ";
   if (!method2)
      str += "SPROULE_SUTHERLAND ";
   if (!method3)
      str += "CYRUS_BECK ";
   if (!method1 || !method2 || !method3) {
      std::cerr << ("test " + testNum + " error: " + str + "failed\n");
   } else {
      std::cout << "test " + testNum + " success\n";
   }
}

void test6()
{
   std::string testNum = "6";
   std::vector<Point> test = { Point(0.22, 5.82167),
                               Point(4.81551, 5.82167),
                               Point(4.81551, 0.13),
                               Point(0.22, 0.13) };
   auto i = Polygon(test);
   LineSegment segment(Point(4.47, 9.39), Point(0.2, 0.65));
   auto ptr1 = i.segmentInsidePolygon(
     segment, Polygon::ClipSegmentMethod::COHEN_SUTHERLAND);
   auto ptr2 = i.segmentInsidePolygon(
     segment, Polygon::ClipSegmentMethod::SPROULE_SUTHERLAND);
   auto ptr3 =
     i.segmentInsidePolygon(segment, Polygon::ClipSegmentMethod::CYRUS_BECK);
   /** type: out
               (2.64279, 5.65)
               (0.22, 0.690937)
    */
   auto answer =
     std::make_unique<LineSegment>(Point(2.64279, 5.65), Point(0.22, 0.690937));
   bool method1 = almost_equal(answer.get(), ptr1.get(), precision),
        method2 = almost_equal(answer.get(), ptr2.get(), precision),
        method3 = almost_equal(answer.get(), ptr3.get(), precision);
   std::string str = "";
   if (!method1)
      str += "COHEN_SUTHERLAND ";
   if (!method2)
      str += "SPROULE_SUTHERLAND ";
   if (!method3)
      str += "CYRUS_BECK ";
   if (!method1 || !method2 || !method3) {
      std::cerr << ("test " + testNum + " error: " + str + "failed\n");
   } else {
      std::cout << "test " + testNum + " success\n";
   }
}

void test7()
{
   std::string testNum = "7";
   std::vector<Point> test = { Point(0.607989, 3.84617),
                               Point(3.30436, 3.84617),
                               Point(3.30436, 8.92549),
                               Point(0.607989, 8.92549) };
   auto i = Polygon(test);
   LineSegment segment(Point(-0.740197, 9.51157), Point(8.36, 3.82));
   auto ptr1 = i.segmentInsidePolygon(
     segment, Polygon::ClipSegmentMethod::COHEN_SUTHERLAND);
   auto ptr2 = i.segmentInsidePolygon(
     segment, Polygon::ClipSegmentMethod::SPROULE_SUTHERLAND);
   auto ptr3 =
     i.segmentInsidePolygon(segment, Polygon::ClipSegmentMethod::CYRUS_BECK);
   /** type: out
         (8.46659, 8.8699)
         (3.48143, 6.12654)
    */
   auto answer = std::make_unique<LineSegment>(Point(0.607989, 8.66837),
                                               Point(3.30436, 6.98197));
   bool method1 = almost_equal(answer.get(), ptr1.get(), precision),
        method2 = almost_equal(answer.get(), ptr2.get(), precision),
        method3 = almost_equal(answer.get(), ptr3.get(), precision);
   std::string str = "";
   if (!method1)
      str += "COHEN_SUTHERLAND ";
   if (!method2)
      str += "SPROULE_SUTHERLAND ";
   if (!method3)
      str += "CYRUS_BECK ";
   if (!method1 || !method2 || !method3) {
      std::cerr << ("test " + testNum + " error: " + str + "failed\n");
   } else {
      std::cout << "test " + testNum + " success\n";
   }
}
void curve_test1()
{
   std::string testNum = "curve::1";
   Point a(0, 0), b(0, 3), c(3, 3), d(3, 0);
   Curve out3 = Curve::makeBezierCurve(a, b, c);
   Curve out4 = Curve::makeBezierCurve(a, b, c, d);
   bool method3 = almost_equal(&out3[out4.size() - 1], &c, precision),
        method4 = almost_equal(&out4[out4.size() - 1], &d, precision);
   std::string str = "";
   if (!method3)
      str += "QUADRATIC ";
   if (!method4)
      str += "CUBIC ";
   if (!method3 || !method4) {
      std::cerr << ("test " + testNum + " error: " + str + "failed\n");
   } else {
      std::cout << "test " + testNum + " success\n";
   }
}

void frac_test1()
{
   auto vector = Fractals::geometricFractal(
     Point(0, 0),
     Fractals::Area(1920, 1080, { -100, 100 }, { -100, 100 }),
     Fractals::GeometricFractalType::KOCH_SNOWFLAKE);
}
