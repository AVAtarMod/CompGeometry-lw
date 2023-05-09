/**
 * @file main.cpp Test file
 * @author Grigory Stupnikov
 * @brief
 * @version 0.1
 * @date 2023-04-13
 */

#include "lib_cppgeometry/Polygon.hpp"

int main(int argc, char const* argv[])
{
   std::vector<Point> test = { Point(9.60817, 11.1311),
                               Point(15.66, 10.08),
                               Point(21.22, 18.11),
                               Point(18.77, 15.82),
                               Point(1.88, 21.39) };
   auto i = Polygon::convexHull(test, ConvexHullMethod::JARVIS);
   Polygon hull(i);
   auto ptr = hull.segmentInsidePolygon(LineSegment({ 0, 0 }, { 2, 0 }),
                                        ClipSegmentMethod::COHEN_SUTHERLAND);
   int a = 0;
   return 0;
}
