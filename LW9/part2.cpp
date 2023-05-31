#include <fstream>
#include <iostream>
#include <sstream>
#include <tuple>
#include <vector>

#include "lib_cppgeometry/Polygon.hpp"
#include "part2.hpp"

namespace p2 {
   struct event
   {
      int x, tp, id_i, id_j;

      event() {}
      event(int x, int tp, int id_i, int id_j) :
        x(x), tp(tp), id_i(id_i), id_j(id_j)
      {
      }

      bool operator<(const event& e) const
      {
         if (x != e.x)
            return x < e.x;
         return tp > e.tp;
      }
   };

   void openAndClose(const std::vector<std::vector<int>>& vec,
                     std::vector<int>& open, std::vector<int>& close)
   {
      int n = vec.size(), k, i, j;
      std::vector<event> e;
      for (i = 0; i < n; ++i) {
         k = vec[i].size();
         for (j = 0; j < k - 1; j += 2) {
            e.push_back(event(vec[i][j], +1, i, j));
            e.push_back(event(vec[i][j + 1], -1, i, j + 1));
         }
         if (j != k)
            e.push_back(event(vec[i][j], +1, i, j));
      }
      std::sort(e.begin(), e.end());

      std::set<int> s = std::set<int>();

      open.push_back(0);
      for (size_t i = 0; i < e.size(); ++i) {
         int id_i = e[i].id_i, id_j = e[i].id_j;
         if (e[i].tp == +1) {
            s.insert(s.lower_bound(vec[id_i][id_j]), vec[id_i][id_j]);
            if (s.size() == n)
               close.push_back(vec[id_i][id_j]);
            else if (s.size() == 1)
               open.push_back(vec[id_i][id_j]);
         } else {
            s.erase(vec[id_i][id_j - 1]);
            if (s.size() == n - 1)
               close.push_back(vec[id_i][id_j]);
            else if (s.size() == 0)
               open.push_back(vec[id_i][id_j]);
         }
      }
      if (s.size() == n)
         close.push_back(-1);
      else if (s.size() == 0)
         open.push_back(-1);
   }

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
            std::string msg =
              "'" + tmp +
              "',pos=" + std::to_string(std::abs(in.tellg()) - tmp.size()) +
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

   std::vector<int> read(std::istream& in)
   {
      std::vector<int> out;
      try {
         out.reserve(255);
         while (true) {
            out.push_back(next_number(in));
         }
      } catch (const std::exception& e) {
      }

      return out;
   }

   void task(const char* in_file)
   {
      std::ifstream in(in_file);

      if (!in) {
         std::cerr << "File " << in_file << " not exist or not accessible\n";
      } else {
         const size_t files_amount = static_cast<size_t>(next_number(in));
         std::vector<std::vector<int>> time_segments(files_amount);
         for (size_t i = 0; i < files_amount; ++i) {
            std::ifstream current(next_line(in, 255));
            time_segments[i] = read(current);
         }
         std::vector<int> open(0), close(0);
         openAndClose(time_segments, open, close);
         const char* out_open = "n1.solution";
         const char* out_close = "n2.solution";
         std::ofstream out_o(out_open);
         std::ofstream out_c(out_close);
         for (int i = 0; i < open.size(); i++)
            out_o << open[i] << std::endl;
         for (int i = 0; i < close.size(); i++)
            out_c << close[i] << std::endl;
         out_o.close();
         out_c.close();
      }
      in.close();
   }

   void task2()
   {
      const char* in_file = "provider.dat";
      task(in_file);
   }
} // namespace p2