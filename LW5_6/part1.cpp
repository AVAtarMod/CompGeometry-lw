#include <fstream>
#include <iostream>
#include <vector>

#include "part1.hpp"
#include "lib_cppgeometry/LineSegment.hpp"
#include "lib_cppgeometry/Point.hpp"

std::vector<Point> read(std::ifstream& in)
{
    int testsCount;
    double x, y;
    in >> testsCount;
    std::vector<Point> out(testsCount);
    for (int i = 0; i < testsCount; i++)
    {
        in >> x >> y;
        out[i] = Point(x, y);
    }
    return out;
}

namespace t1_1
{
}

void task1_1()
{
    const char* in_file = "task1_1in.txt";
    std::ifstream in(in_file);

    if (!in)
    {
        std::cerr << "File " << in_file << " not exist or not accessible\n";
    }
    else
    {
        std::vector<Point> points = read(in);
      //   std::cout << t1_1::isLine(points) << std::endl;
    }
    in.close();
}

namespace t1_2
{
}

void task1_2()
{
    const char* in_file = "task1_2in.txt";
    std::ifstream in(in_file);

    if (!in)
    {
        std::cerr << "File " << in_file << " not exist or not accessible\n";
    }
    else
    {
        std::vector<Point> points = read(in);
    }
    in.close();
}
