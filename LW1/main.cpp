#include <iostream>

#include "part2.hpp"

void task1(){}; //define in own files

int main(int argc, char const *argv[])
{
   bool isArgsEmpty = false;
   if (argc <= 1)
   {
      isArgsEmpty = true;
      std::cout << "Enter program number to start: ";
      argc++;
   }
   for (int i = 1; i < argc; i++)
   {
      int choice;
      if (isArgsEmpty)
      {
         std::cin >> choice;
         std::cin.ignore(32767, '\n');
      }
      else
      {
         choice = std::stoi(argv[i]);
      }

      switch (choice)
      {
      case 1:
         std::cout << "Task #1\n";
         task1();
         std::cout << "-----\n";
         break;
      case 5:
         std::cout << "Task #5\n";
         task2_1();
         std::cout << "-----\n";
         break;
      case 6:
         std::cout << "Task #6\n";
         task2_2();
         std::cout << "-----\n";
         break;
      default:
         std::cerr << "Error: task number is unsupported or cannot be recognized.\n";
         break;
      }
   }
}
