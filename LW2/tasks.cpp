#include <fstream>
#include <iostream>
#include <vector>

#include "lib_cppgeometry/Point.hpp"
#include "lib_cppgeometry/Polygon.hpp"
#include "tasks.hpp"

std::vector<Point> read(std::ifstream& in)
{
   int testsCount;
   double x, y;
   in >> testsCount;
   std::vector<Point> out(testsCount);
   for (int i = 0; i < testsCount; i++) {
      in >> x >> y;
      out[i] = Point(x, y);
   }
   return out;
}

void task1()
{
   const char* in_file = "task1in.txt";
   std::ifstream in(in_file);

   if (!in) {
      std::cerr << "File " << in_file << " not exist or not accessible\n";
   } else {
      std::vector<Point> points = read(in);
      if (Point::isInsideAngle(points[1], points[2], points[3], points[0]))
         std::cout << "The point lies inside the angle." << std::endl;
      else
         std::cout << "The point does not lie inside the angle." << std::endl;
   }
   in.close();
}

void task2_1()
{
   const char* in_file = "task2_1in.txt";
   std::ifstream in(in_file);

   if (!in) {
      std::cerr << "File " << in_file << " not exist or not accessible\n";
   } else {
      std::vector<Point> points = read(in);
      Polygon pol(points);
      double x, y;
      std::cout << "Point: ";
      std::cin >> x >> y;
      Point p(x, y);
      bool result;
      if (points.size() == 3)
         result = pol.isInsideTriangle(points[0], points[1], points[2], p);
      else
         result = pol.isInside(p);

      if (result)
         std::cout << "The point lies inside the polygon." << std::endl;
      else
         std::cout << "The point does not lie inside the polygon." << std::endl;
   }
   in.close();
}

void task2_2()
{
   const char* in_file = "task2_2in.txt";
   std::ifstream in(in_file);

   if (!in) {
      std::cerr << "File " << in_file << " not exist or not accessible\n";
   } else {
      std::vector<Point> points = read(in);
      Polygon pol(points);
      double x, y;
      std::cout << "Point: ";
      std::cin >> x >> y;
      Point p(x, y);
      if (pol.isInsideConvexPolygon(p, pol.anglesForConvexPolygon()))
         std::cout << "The point lies inside the polygon." << std::endl;
      else
         std::cout << "The point does not lie inside the polygon." << std::endl;
   }
   in.close();
}

void task3()
{
   const char* in_file = "task3in.txt";
   std::ifstream in(in_file);

   if (!in) {
      std::cerr << "File " << in_file << " not exist or not accessible\n";
   } else {
      std::vector<Point> points = read(in);
      Polygon pol(points);
      if (pol.isSimple())
         std::cout << "The polygon is simple." << std::endl;
      else
         std::cout << "The polygon is not simple." << std::endl;
   }
   in.close();
}
