#include <fstream>
#include <iostream>
#include <tuple>
#include <vector>

#include "lib_cppgeometry/Polygon.hpp"
#include "part1.hpp"

std::tuple<std::vector<Point>, std::vector<Point>> read(std::ifstream& in)
{
   int testsCount;
   double x, y;
   in >> testsCount;
   std::tuple<std::vector<Point>, std::vector<Point>> out;
   // for (int i = 0; i < testsCount; i++) {
   //    in >> x >> y;
   //    out[i] = Point(x, y);
   // }
   return out;
}

namespace t1_1 {
}

void task1_1()
{
   const char* in_file = "task1_1in.txt";
   std::ifstream in(in_file);

   if (!in) {
      std::cerr << "File " << in_file << " not exist or not accessible\n";
   } else {
      auto data = read(in);
      std::vector<Point>& polygon = std::get<0>(data);
      std::vector<Point>& input = std::get<1>(data);
      std::cout << "Points in area: \n";
      for (auto&& i :
           Polygon(polygon).pointsInsidePolygon(input, Polygon::SIMPLE)) {
         std::cout << "\t" << i << "\n";
      }
   }
   in.close();
}

namespace t1_2 {
}

void task1_2()
{
   const char* in_file = "task1_2in.txt";
   std::ifstream in(in_file);

   if (!in) {
      std::cerr << "File " << in_file << " not exist or not accessible\n";
   } else {
      auto data = read(in);
      std::vector<Point>& polygon = std::get<0>(data);
      std::vector<Point>& input = std::get<1>(data);
      std::cout << "Points in area: \n";
      for (auto&& i :
           Polygon(polygon).pointsInsidePolygon(input, Polygon::SIMPLE)) {
         std::cout << "\t" << i << "\n";
      }
   }
   in.close();
}
