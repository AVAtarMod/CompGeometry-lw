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
   std::vector<Point> test = { Point(2, 3),
                               Point(0, 0),
                               Point(2,0),
                               Point(1.5, 1.6),
                               Point(0, 2) };
   auto i = Polygon::convexHull(test, ConvexHullMethod::JARVIS);
   Point a = i[0];
   return 0;
}
