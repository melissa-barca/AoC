#include <_types/_uint64_t.h>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace aoc_utils
{
   typedef std::vector<int> row_t;
   typedef std::vector<row_t> vector_t;
   bool readGridInput(std::string fileName, vector_t* pGrid)
   {
      pGrid->resize(0);

      std::ifstream file;
      file.open(fileName);

      std::string line;
      for (int i = 0; getline(file, line); i++)
      {
         std::vector<int> row;
         for (size_t j = 0; j < line.size(); j++)
            row.push_back(line.at(j) - 48);
         pGrid->push_back(row);
      }

      return 1;
   }

   void printWarning(std::string message)
   {
      std::cout << "WARNING: " << message << std::endl;
   }

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

   void printTestResults(int part, std::uint64_t expected, std::uint64_t calculated)
   {
      std::string text ("Part " + std::to_string(part) + ") Sample test ");
      text.append(calculated == expected ?
                  "PASSED" :
                  "FAILED: Expected " + std::to_string(expected) +
                     " and calculated " + std::to_string(calculated));
      std::cout << text << std::endl;
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

   void printSoltuion(int part, uint64_t solution)
   {
      std::cout << "Part " <<  part << ") Solution: " << solution << std::endl;
   }

   void printSoltuion(int part, int solution)
   {
      std::cout << "Part " <<  part << ") Solution: " << solution << std::endl;
   }
}
