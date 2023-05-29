#include <fstream>
#include <iostream>
#include <sstream>
#include <tuple>
#include <vector>

#include "lib_cppgeometry/Polygon.hpp"
#include "part2.hpp"

struct InputData
{
   std::vector<std::vector<std::pair<int, int>>> time_segments;
};

struct IOChar
{
   char data = '\0';
   bool success = false;
};

class CharType
{
  public:
   bool digit = false;
   bool minus = false;
   bool plus = false;
   bool dot = false;

   CharType() {}
   CharType(char data)
   {
      if (data == '.')
         dot = true;
      if (data == '-')
         minus = true;
      if (data == '+')
         plus = true;
      if (isdigit(data))
         digit = true;
   }
   bool isSignChar() const { return minus || plus; }
   bool isRecognized() const { return digit || minus || plus || dot; }
};

IOChar try_get(std::istream& in)
{
   IOChar result;
   in.get(result.data);
   if (in.fail() || in.gcount() == 0)
      result.success = false;
   else
      result.success = true;
   return result;
}

std::string extractBufferContent(const std::string& buffer)
{
   std::stringstream stream;
   for (auto&& i : buffer) {
      if (i != '\0')
         stream << i;
   }
   return stream.str();
}

double next_number(std::istream& in)
{
   const size_t BUFFER_SIZE = 255;
   std::string tmp;
   tmp.resize(BUFFER_SIZE);

   double result;
   while (true) {
      IOChar input = try_get(in);
      if (!input.success)
         throw std::runtime_error("File is empty or unknown error");
      if (input.data == '#') {
         in.getline(tmp.data(), BUFFER_SIZE, '\n');
         continue;
      }

      CharType t;
      size_t dot_count = 0, sign_char_count = 0, digit_count = 0;
      std::fill(tmp.begin(), tmp.end(), '\0');
      for (size_t i = 0; i < BUFFER_SIZE; ++i) {
         if (!input.success)
            break;

         t = { input.data };
         if (!t.isRecognized())
            break;

         if (t.isSignChar())
            ++sign_char_count;
         else if (t.digit)
            ++digit_count;
         else if (t.dot)
            ++dot_count;

         if (dot_count > 1 || sign_char_count > 1)
            break;

         tmp[i] = input.data;
         input = try_get(in);
      }
      if (digit_count == 0) {
         tmp = extractBufferContent(tmp);
         std::string msg = "'" + tmp + "',pos=" +
                           std::to_string(std::abs(in.tellg()) - tmp.size()) +
                           ". End of file reached before end of number.";
         throw std::runtime_error(msg);
      }
      result = std::stod(tmp);
      break;
   }
   return result;
}

std::string next_line(std::istream& in, const size_t line_length)
{
   std::string result;
   while (true) {
      result.resize(line_length);
      std::fill(result.begin(), result.end(), '\0');
      in.getline(result.data(), line_length, '\n');
      if (in.fail() || in.gcount() == 0)
         throw std::runtime_error(
           "Cannot read line, EOF reached or IO error happened");
      result = extractBufferContent(result);
      if (result.find_first_of('#') == result.npos && result != "\n" &&
          result != "")
         break;
   }

   return result;
};

InputData read(std::istream& in)
{
   InputData out;
   try {

   } catch (const std::exception& e) {
      std::cerr << "Invalid file format. Error: " << e.what() << '\n';
      exit(EXIT_FAILURE);
   }

   return out;
}

void task(const char* in_file)
{
   std::ifstream in(in_file);

   if (!in) {
      std::cerr << "File " << in_file << " not exist or not accessible\n";
   } else {
      
   }
   in.close();
}

void task2()
{
   const char* in_file = "provider.dat";
   task(in_file);
}
