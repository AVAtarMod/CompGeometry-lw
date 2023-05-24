#include <fstream>
#include <iostream>
#include <tuple>
#include <vector>

#include "lib_cppgeometry/Polygon.hpp"
#include "part1.hpp"

Polygon makePolygonByArea(const std::pair<double, double>& x_minmax, const std::pair<double, double>& y_minmax) {
    const Point a(x_minmax.first, y_minmax.first), b(x_minmax.second, y_minmax.first), c(x_minmax.second, y_minmax.second), d(x_minmax.first, y_minmax.second);
    return Polygon(std::vector<Point>{a,b,c,d});
}

struct InputData {
    Polygon polygon = Polygon(std::vector<Point>());
    std::vector<Point> points;
};

InputData read(std::ifstream& in)
{
   std::pair<double, double> x_minmax;
   std::pair<double, double> y_minmax;
   double area[4];
   const size_t areaSize = 4;
   double x, y;
   InputData out;
   int len = 255;
   std::string tmp;
   tmp.resize(len);
   for (size_t i = 0; i < areaSize; i++) {
       in.getline(tmp.data(),len);
       if (tmp.find_first_of('#') != tmp.npos) {
           i--;
           continue;
       }
       in >> area[i];
   }
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
      Polygon& polygon = data.polygon;
      std::vector<Point>& input = data.points;
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
