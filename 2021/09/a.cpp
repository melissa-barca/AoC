#include <fstream>
#include <string>
#include <vector>

#include "../AoCUtils.cpp"


typedef std::vector<std::vector<int>> vector_t;
bool readInput(std::string fileName, vector_t* grid)
{
   grid->resize(0);

   std::ifstream file;
   file.open(fileName);

   std::string line;
   for (int i = 0; getline(file, line); i++)
   {
      std::vector<int> row;
      for (size_t j = 0; j < line.size(); j++)
         row.push_back(line.at(j) - 48);
      grid->push_back(row);
   }

   return true;
}

void basin(size_t row, size_t column, int& sum, vector_t& seen, const vector_t& grid)
{
   if (grid.at(row).at(column) == 9 || seen.at(row).at(column))
      return;
   else
   {
      sum++;
      seen.at(row).at(column) = 1;
      if (column != 0 && !seen.at(row).at(column - 1))
         basin(row, column - 1, sum, seen, grid);
      if (column != grid.at(row).size() - 1 && !seen.at(row).at(column + 1))
         basin(row, column + 1, sum, seen, grid);
      if (row != 0 && !seen.at(row - 1).at(column))
         basin(row - 1, column, sum, seen, grid);
      if (row != grid.size() - 1 && !seen.at(row+1).at(column))
         basin(row + 1, column, sum, seen, grid);
   }

}

int basin(size_t row, size_t column, const vector_t& grid)
{
   std::vector<std::vector<int>> seen;
   for (size_t i = 0; i < grid.size(); i++)
   {
      std::vector<int> row;
      for (size_t j = 0; j < grid.at(i).size(); j++)
      {
         row.push_back(0);
      }
      seen.push_back(row);
   }

   seen.at(row).at(column) = 1;
   int sum = 1;
   if (column != 0)
      basin(row, column - 1, sum, seen, grid);
   if (column != grid.at(row).size() - 1)
      basin(row, column + 1, sum, seen, grid);
   if (row !=  0)
      basin(row - 1, column, sum, seen, grid);
   if (row != grid.size() - 1)
      basin(row + 1, column, sum, seen, grid);
   return sum;
}

int findSumOfRiskLevels(vector_t* grid, int* pLargest, int* pSecondLargest, int* pThirdLargest)
{
   int riskLevelSum = 0;
   *pLargest = -1;
   *pSecondLargest = -1;
   *pThirdLargest = -1;
   for (size_t i = 0; i < grid->size(); i++)
   {
      std::vector<int> row = grid->at(i);
      for (size_t j = 0; j < row.size(); j++)
      {
         int digit = row.at(j);

         if (j == 0 || digit < row.at(j-1))
            if (j == (row.size() - 1) || digit < row.at(j+1))
               if (i == 0 || digit < grid->at(i-1).at(j))
                  if (i == (grid->size() - 1) || digit < grid->at(i+1).at(j))
                  {
                     riskLevelSum += 1 + digit;
                     int basinSize = basin(i, j, *grid);
                     if (basinSize > *pLargest)
                     {
                        *pThirdLargest = *pSecondLargest;
                        *pSecondLargest = *pLargest;
                        *pLargest = basinSize;
                     }
                     else if (basinSize > *pSecondLargest)
                     {
                        *pThirdLargest = *pSecondLargest;
                        *pSecondLargest = basinSize;
                     }
                     else if (basinSize > *pThirdLargest)
                     {
                        *pThirdLargest = basinSize;
                     }
                  }

      }
   }
   return riskLevelSum;
}

int main()
{
   vector_t grid;

   if (!readInput("sample", &grid))
      return 1;

   int largest = -1;
   int secondLargest = -1;
   int thirdLargest = -1;

   aoc_utils::printTestResults(1, 15, findSumOfRiskLevels(&grid, &largest, &secondLargest, &thirdLargest));
   aoc_utils::printTestResults(2, 1134, largest * secondLargest * thirdLargest);

   if (!readInput("input", &grid))
      return 1;
   aoc_utils::printSoltuion(1, findSumOfRiskLevels(&grid, &largest, &secondLargest, &thirdLargest));
   aoc_utils::printSoltuion(2, largest * secondLargest * thirdLargest);

   return 0;
}
