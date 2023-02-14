#include <fstream>
#include <iostream>
#include <vector>

#include "lib_cppgeometry/Point.hpp"
#include "lib_cppgeometry/LineSegment.hpp"

namespace t2_1
{
   void read(std::ifstream &in, std::vector<Point> &out)
   {
      size_t testsCount = 0;
      in >> testsCount;
      const size_t testSize = 3;
      out = std::vector<Point>(testsCount * testSize);
      for (size_t i = 0; i < testsCount; ++i)
      {
         for (size_t iPoint = 0; iPoint < 3; ++iPoint)
         {
            int x, y;
            in >> x >> y;
            out[i * testSize + iPoint] = Point(x, y);
         }
      }
   }
   void solve(std::ofstream &out, std::vector<Point> &in)
   {
   }

} // namespace t2_1

void task2_1()
{
   const char *in_file = "task2_1in.txt";
   const char *out_file = "task2_1out.txt";

   std::ifstream in(in_file);
   std::ofstream out(out_file);

   if (!in)
   {
      std::cerr << "File " << in_file << " not exist or not accessible\n";
   }
   else if (!out)
   {
      std::cerr << "File " << out_file << " not exist or not accessible\n";
   }

   else
   {
      std::vector<Point> tests;
      t2_1::read(in, tests);
      t2_1::solve(out, tests);
   }
   out.close();
   in.close();
}
void task2_2()
{
   std::ifstream in("task2_2in.txt");
}
