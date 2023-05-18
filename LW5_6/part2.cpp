#include "part2.hpp"

namespace t2 {
   std::vector<Point> read(std::ifstream& in)
   {
      int testsCount;
      double x, y;
      in >> testsCount;
      std::vector<Point> p(testsCount);
      for (int i = 0; i < testsCount; i++) {
         in >> x >> y;
         p[i] = Point(x, y);
      }
      return p;
   }

   matrix_t readMat(std::ifstream& in)
   {
      int n, i, j;
      double x, y;
      in >> n;
      matrix_t mat = matrix_t(n);
      for (i = 0; i < n; i++) {
         mat[i] = std::vector<int>(n);
         for (j = 0; j < n; j++)
            in >> mat[i][j];
      }
      return mat;
   }
} // namespace t2

void task2()
{
   const char* in_file1 = "task2_1in.txt";
   std::ifstream in(in_file1);
   const char* in_file2 = "task2_2in.txt";
   std::ifstream in_mat(in_file2);

   if (!in) {
      std::cerr << "File " << in_file1 << " not exist or not accessible\n";
   } else {
      if (!in_mat) {
         std::cerr << "File " << in_file2 << " not exist or not accessible\n";
      } else {
         std::vector<Point> points = t2::read(in);
         matrix_t mat = t2::readMat(in_mat);

         double x, y;
         cout << "Enter point coordinates: ";
         cin >> x >> y;

         Point p(x, y);
         Graph g(mat, points);
         std::unique_ptr<Polygon> pol = g.localizationOfAPoint(p);
         if (pol)
            for (int i = 0; i < pol->size(); i++)
               std::cout << (*pol)[i] << std::endl;
         else
            std::cout << "The point is outside of the graph!" << std::endl;
      }
      in_mat.close();
   }
   in.close();
}
