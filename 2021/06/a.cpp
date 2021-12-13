#include <_types/_uint64_t.h>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "../AoCUtils.cpp"


bool readInput(std::string fileName, std::vector<int>* pAges)
{
   pAges->clear();

   std::ifstream file;
   file.open(fileName);

   std::string line;
   while (getline(file, line))
   {
      std::stringstream ss(line);
      for (int i; ss >> i;)
      {
         pAges->push_back(i);
         if (ss.peek() == ',')
            ss.ignore();
      }
   }
   return 1;
}


uint64_t getQuickFishTotalAtDay(int days, const std::vector<int>& ages)
{
   // Create an array where each value represents the number of fishes of that age
   // where the age is represented by the index
   const int kTimerCount = 9;
   long long fishes[kTimerCount] = { 0LL };
   for (auto age : ages)
   {
      fishes[age]++;
   }

   for (int i = 0; i < days; i++)
   {
      long long zeroCount = fishes[0];
      for (int j = 0; j < kTimerCount - 1; j++)
      {
         fishes[j] = fishes[j+1];
      }
      fishes[6] += zeroCount;
      fishes[8] = zeroCount;
   }

   uint64_t sum = 0;
   for (auto fish : fishes)
      sum += fish;

   return sum;
}

int main()
{
   std::vector<int> ages;
   if (!readInput("sample", &ages))
      return 1;

   aoc_utils::printTestResults(1, (uint64_t) 26, getQuickFishTotalAtDay(18, ages));
   aoc_utils::printTestResults(1, (uint64_t) 5934, getQuickFishTotalAtDay(80, ages));
   aoc_utils::printTestResults(2, 26984457539, getQuickFishTotalAtDay(256, ages));

   if (!readInput("input", &ages))
      return 1;

   aoc_utils::printSoltuion(1, (int) getQuickFishTotalAtDay(80, ages));
   aoc_utils::printSoltuion(2, getQuickFishTotalAtDay(256, ages));
}
