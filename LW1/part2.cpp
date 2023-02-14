#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

#include "lib_cppgeometry/LineSegment.hpp"
#include "lib_cppgeometry/Point.hpp"

namespace t2_1 {
   struct LabeledPoint
   {
      Point point;
      char label;
   };

   struct TestsInfo
   {
      const size_t testSize = 3;
      size_t testCount = 0;
      std::vector< LabeledPoint > testStorage;
   };

   struct find_label : std::unary_function< LabeledPoint, bool >
   {
      char _label;
      find_label(char label) : _label(label) {}
      bool operator()(LabeledPoint const& p) const { return p.label == _label; }
   };

   bool cmpByX(const LabeledPoint& p1, const LabeledPoint& p2)
   {
      if (p1.point["x"] > p2.point["x"])
         return false;
      return true;
   }
   bool cmpByY(const LabeledPoint& p1, const LabeledPoint& p2)
   {
      if (p1.point["y"] > p2.point["y"])
         return false;
      return true;
   }

   void read(std::ifstream& in, TestsInfo& out)
   {
      in >> out.testCount;
      const char labels[] = { 'A', 'B', 'C' };

      out.testStorage =
        std::vector< LabeledPoint >(out.testCount * out.testSize);
      for (size_t i = 0; i < out.testCount; ++i) {
         for (size_t iPoint = 0; iPoint < 3; ++iPoint) {
            LabeledPoint& current = out.testStorage[i * out.testSize + iPoint];
            double x, y;
            in >> x >> y;
            current.point = Point(x, y);
            current.label = labels[iPoint];
         }
      }
   }

   void solve(std::ofstream& out, TestsInfo& in)
   {
      for (size_t iTest = 0; iTest < in.testCount; ++iTest) {
         size_t iStart = in.testSize * iTest;
         auto start = in.testStorage.begin() + iStart,
              end = in.testStorage.begin() + iStart + in.testSize;

         std::sort(start, end, cmpByX); // sort by X
         if ((*start).point["x"] == (*(start + 1)).point["x"]) {
            // 1["x"] = 2["x"] = 3["x"] by issue definition
            std::sort(start, end, cmpByY); // sort by Y
         }

         const LabeledPoint& p1 = (*start);
         const LabeledPoint& p2 = (*(start + 1));
         const LabeledPoint& p3 = (*(start + 2));

         out << p2.label << '\n';
         std::cout << p2.label << '\n';
      }
   }
} // namespace t2_1

void task2_1()
{
   const char* in_file = "task2_1in.txt";
   const char* out_file = "task2_1out.txt";

   std::ifstream in(in_file);
   std::ofstream out(out_file);

   if (!in) {
      std::cerr << "File " << in_file << " not exist or not accessible\n";
   } else if (!out) {
      std::cerr << "File " << out_file << " not exist or not accessible\n";
   } else {
      t2_1::TestsInfo info;
      t2_1::read(in, info);
      t2_1::solve(out, info);
   }
   out.close();
   in.close();
}

namespace t2_2 {
   const size_t pointAmount = 4;
   const size_t lineAmount = pointAmount / 2;

   void read(std::ifstream& in, std::vector< Point >& out)
   {
      out = std::vector< Point >(pointAmount);
      for (size_t i = 0; i < pointAmount; ++i) {
         double x, y;
         in >> x >> y;
         out[i] = Point(x, y);
      }
   }

   void solve(std::ofstream& out, const std::vector< Point >& in)
   {
      Point intersect = Line::intersect(Line(in[0], in[1]), Line(in[2], in[3]));
      out << intersect << '\n';
      std::cout << intersect << '\n';
   }
} // namespace t2_2

void task2_2()
{

   const char* in_file = "task2_2in.txt";
   const char* out_file = "task2_2out.txt";

   std::ifstream in(in_file);
   std::ofstream out(out_file);

   if (!in) {
      std::cerr << "File " << in_file << " not exist or not accessible\n";
   } else if (!out) {
      std::cerr << "File " << out_file << " not exist or not accessible\n";
   } else {
      std::vector< Point > points;
      t2_2::read(in, points);
      t2_2::solve(out, points);
   }
   out.close();
   in.close();
}
