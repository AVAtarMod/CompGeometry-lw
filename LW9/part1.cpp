#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "part1.hpp"

namespace p1 {
   std::vector<LineSegment> read(std::ifstream& in)
   {
      double x, y;
      int n;
      in >> n;
      Point p;
      LineSegment ls(Point(0, 0), Point(0, 0));
      std::vector<LineSegment> ans(0);
      for (int i = 0; i < n; i++) {
         in >> x >> y;
         p = Point(x, y);
         in >> x >> y;
         ls = LineSegment(p, Point(x, y));
         ans.push_back(ls);
      }
      return ans;
   }

   void task1()
   {
      const char* in_file = "in_file1.txt";
      std::ifstream in(in_file);

      if (!in) {
         std::cerr << "File " << in_file << " not exist or not accessible\n";
      } else {
         std::vector<LineSegment> vec = read(in);
         if (LineSegment::isIntersection(vec))
            std::cout << "Segments are intersect." << std::endl;
         else
            std::cout << "Segments are not intersect." << std::endl;
      }
      in.close();
   }
} // namespace p1