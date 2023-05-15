#include "part2.hpp"
#include <fstream>
#include <iostream>
#include <vector>

namespace t2 {
} // namespace t2

void task2()
{
   const char* in_file = "task2in.txt";
   std::ifstream in(in_file);

   if (!in) {
      std::cerr << "File " << in_file << " not exist or not accessible\n";
   } else {
      // code here
   }
   in.close();
}
