#include <iostream>
#include <string>

namespace aoc_utils
{
   void printError(std::string message)
   {
      std::cout << "ERROR: " << message << std::endl;
   }

   void printParseError(std::string line, std::string description)
   {
      printError("Could not parse input. Failed on line: " + line + ". " + description);
   }

   void printParseError(std::string line)
   {
      printParseError(line, "");
   }

   void printTestResults(int part, int expected, int calculated)
   {
      std::string text ("Part " + std::to_string(part) + ") Sample test ");
      text.append(calculated == expected ?
                  "PASSED" :
                  "FAILED: Expected " + std::to_string(expected) +
                     " and calculated " + std::to_string(calculated));
      std::cout << text << std::endl;
   }

   void printTestResults(int part)
   {
      printTestResults(part, 0, 0);
   }

   void printSoltuion(int part, int solution)
   {
      std::cout << "Part " <<  part << ") Solution: " << solution << std::endl;
   }
}
