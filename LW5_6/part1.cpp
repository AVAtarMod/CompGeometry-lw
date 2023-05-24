#include <fstream>
#include <iostream>
#include <tuple>
#include <vector>

#include "lib_cppgeometry/Polygon.hpp"
#include "part1.hpp"

Polygon makePolygonByArea(const std::pair<double, double>& x_minmax,
                          const std::pair<double, double>& y_minmax)
{
   const Point a(x_minmax.first, y_minmax.first),
     b(x_minmax.second, y_minmax.first), c(x_minmax.second, y_minmax.second),
     d(x_minmax.first, y_minmax.second);
   return Polygon(std::vector<Point> { a, b, c, d });
}

struct InputData
{
   Polygon polygon = Polygon(std::vector<Point>());
   std::vector<Point> points;
};

struct IOChar
{
   char data = '\0';
   bool success = false;
};

IOChar try_get(std::ifstream& in)
{
   IOChar result;
   in.get(result.data);
   if (in.fail() || in.gcount() == 0)
      result.success = false;
   else
      result.success = true;
   return result;
}

double next_number(std::ifstream& in)
{
   const size_t BUFFER_SIZE = 255;
   std::string tmp;
   tmp.resize(BUFFER_SIZE);

   double result;
   while (true) {
      IOChar input = try_get(in);
      if (!input.success)
         break;
      if (input.data == '#') {
         in.getline(tmp.data(), BUFFER_SIZE, '\n');
         continue;
      }

      if (isdigit(input.data)) {
         size_t dot_count = 0;
         tmp.clear();
         tmp[0] = input.data;
         for (size_t i = 1; i < BUFFER_SIZE; ++i) {
            input = try_get(in);
            if (!input.success)
               break;
            if (isdigit(input.data) || (input.data == '.' && dot_count == 0))
               tmp[i] = input.data;
         }
      }
      result = std::stod(tmp);
   }
   return result;
}

InputData read(std::ifstream& in)
{
   std::pair<double, double> x_minmax { next_number(in), next_number(in) };
   std::pair<double, double> y_minmax { next_number(in), next_number(in) };
   const size_t areaSize = 4;
   double x, y;
   InputData out;
   out.polygon = makePolygonByArea(x_minmax, y_minmax);
   const size_t points_amount = next_number(in);
   out.points.resize(points_amount);
   for (size_t i = 0; i < points_amount; ++i) {
      out.points[i] = Point(next_number(in), next_number(in));
   }
   return out;
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
      for (auto&& i : polygon.pointsInsidePolygon(input, Polygon::SIMPLE)) {
         std::cout << "\t" << i << "\n";
      }
   }
   in.close();
}

void task1_2()
{
   const char* in_file = "task1_2in.txt";
   std::ifstream in(in_file);

   if (!in) {
      std::cerr << "File " << in_file << " not exist or not accessible\n";
   } else {
      auto data = read(in);
      Polygon& polygon = data.polygon;
      std::vector<Point>& input = data.points;
      std::cout << "Points in area: \n";
      for (auto&& i : polygon.pointsInsidePolygon(input, Polygon::GRID)) {
         std::cout << "\t" << i << "\n";
      }
   }
   in.close();
}
