#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "../AoCUtils.cpp"

bool readInput(std::string fileName, std::vector<int>* pPositions)
{
   pPositions->clear();

   std::ifstream file;
   file.open(fileName);
   

   std::string line;
   while (getline(file, line))
   {
      std::stringstream ss(line);
      for (int i; ss >> i;)
      {
         pPositions->push_back(i);
         if (ss.peek() == ',')
            ss.ignore();
      }
   }
   std::sort(pPositions->begin(), pPositions->end());
   return 1;
}

int getSummation(int num)
{
   return (num * (num + 1)) / 2;
}

int getSmallestAdjustedFuelAmount(const std::vector<int>& positions)
{
   int startPos = positions.at(0);
   int endPos = positions.at(positions.size() - 1);

   int minFuelCost = -1;
   for (int i = startPos; i < endPos; i++)
   {
      int fuelCost = 0;
      for (int j = 0; j < (int) positions.size(); j++)
      {
         fuelCost += getSummation(std::abs(positions.at(j) - i));
      }
      if (minFuelCost < 0 || fuelCost < minFuelCost)
         minFuelCost = fuelCost;
   }

   return minFuelCost;
}

int getSmallestFuelAmount(const std::vector<int>& positions)
{
   int startPos = positions.at(0);
   int endPos = positions.at(positions.size() - 1);
   int minFuelCost = -1;
   for (int i = startPos; i < endPos; i++)
   {
      int fuelCost = 0;
      for (int j = 0; j < (int) positions.size(); j++)
      {
         fuelCost += std::abs(positions.at(j) - i);
      }
      if (minFuelCost < 0 || fuelCost < minFuelCost)
         minFuelCost = fuelCost;
   }

   return minFuelCost;
}

int main()
{
   std::vector<int> positions;
   if (!readInput("sample", &positions))
      return 1;
   aoc_utils::printTestResults(1, 37, getSmallestFuelAmount(positions));
   aoc_utils::printTestResults(1, 168, getSmallestAdjustedFuelAmount(positions));

   if (!readInput("input", &positions))
      return 1;
   aoc_utils::printSoltuion(1, getSmallestFuelAmount(positions));
   aoc_utils::printSoltuion(2, getSmallestAdjustedFuelAmount(positions));
}

