#include <fstream>
#include <string>
#include <vector>

#include "../AoCUtils.cpp"

using namespace std;

typedef aoc_utils::row_t row_t;
typedef aoc_utils::vector_t vector_t;

const int kFlashLevel = 10;
void flash(size_t row, size_t column, int& sum, vector_t* grid, vector_t* pFlashed)
{
   if (grid->at(row).at(column) < kFlashLevel || pFlashed->at(row).at(column))
      return;

   sum++;
   pFlashed->at(row).at(column) = 1;

   // left
   if (column != 0 && !pFlashed->at(row).at(column - 1))
   {
      grid->at(row).at(column - 1)++;
      flash(row, column - 1, sum, grid, pFlashed);
   }

   // right
   if (column != grid->at(row).size() - 1 && !pFlashed->at(row).at(column + 1))
   {
      grid->at(row).at(column + 1)++;
      flash(row, column + 1, sum, grid, pFlashed);
   }

   // top
   if (row != 0 && !pFlashed->at(row - 1).at(column))
   {
      grid->at(row - 1).at(column)++;
      flash(row - 1, column, sum, grid, pFlashed);
   }

   // bottom
   if (row != grid->size() - 1 && !pFlashed->at(row+1).at(column))
   {
      grid->at(row + 1).at(column)++;
      flash(row + 1, column, sum, grid, pFlashed);
   }

   // top left diagonal
   if (column != 0 && row != 0 &&
       !pFlashed->at(row-1).at(column - 1))
   {
      grid->at(row - 1).at(column - 1)++;
      flash(row - 1, column - 1, sum, grid, pFlashed);
   }

   // top right diagonal
   if (column != grid->at(row).size() -1 &&
       row != 0 &&
       !pFlashed->at(row - 1).at(column + 1))
   {
      grid->at(row-1).at(column + 1)++;
      flash(row - 1, column + 1, sum, grid, pFlashed);
   }

   // bottom left diagonal
   if (column != 0 &&
       row != grid->size() -1 &&
       !pFlashed->at(row + 1).at(column - 1))
   {
      grid->at(row + 1).at(column - 1)++;
      flash(row + 1, column - 1, sum, grid, pFlashed);
   }

   // bottom right diagonal
   if (column != grid->at(row).size() - 1 &&
       row !=  grid->at(row).size() - 1 &&
       !pFlashed->at(row + 1).at(column + 1))
   {
      grid->at(row + 1).at(column + 1)++;
      flash(row + 1, column + 1, sum, grid, pFlashed);
   }

   grid->at(row).at(column) = 0;
}

vector_t getEmptyGrid(const vector_t& grid)
{
   vector_t flashed;
   for (size_t i = 0; i < grid.size(); i++)
   {
      row_t row;
      for (size_t j = 0; j < grid.at(i).size(); j++)
      {
         row.push_back(0);
      }
      flashed.push_back(row);
   }
   return flashed;
}

void increaseEachEnergyLevel(vector_t* pGrid)
{
   for (row_t& row : *pGrid)
      for (int& i : row)
         i++;
}

void getFlashes(int* pFlashes, vector_t* pGrid, vector_t* pFlashed)
{
   for (size_t i = 0; i < pGrid->size(); i++)
   {
      for (size_t j = 0; j < pGrid->at(i).size(); j++)
      {
         if (!pFlashed->at(i).at(j) && pGrid->at(i).at(j) > kFlashLevel - 1)
         {
            flash(i, j, *pFlashes, pGrid, pFlashed);
         }
      }
   }
}

void getFlashesPerSteps(const int& steps, int* pFlashes, const vector_t& grid)
{
   *pFlashes = 0;
   vector_t gridCopy(grid);

   for (int step = 0; step < steps; step++)
   {
      vector_t flashed = getEmptyGrid(gridCopy);
      increaseEachEnergyLevel(&gridCopy);
      getFlashes(pFlashes, &gridCopy, &flashed);
   }
}

int getFirstCompleteFlash(const vector_t& grid)
{
   int complete = grid.size() * grid.at(0).size();
   int count = 0;
   int steps = 0;

   vector_t gridCopy(grid);
   while (count != complete)
   {
      steps++;
      count = 0;
      vector_t flashed = getEmptyGrid(gridCopy);
      increaseEachEnergyLevel(&gridCopy);
      getFlashes(&count, &gridCopy, &flashed);
   }

   return steps;
}

int main()
{
   vector_t grid;
   if (!aoc_utils::readGridInput("sample", &grid))
      return 1;

   int flashes = 0;
   getFlashesPerSteps(100, &flashes, grid);
   aoc_utils::printTestResults(1, 1656, flashes);
   aoc_utils::printTestResults(2, 195, getFirstCompleteFlash(grid));

   if (!aoc_utils::readGridInput("input", &grid))
      return 1;

   getFlashesPerSteps(100, &flashes, grid);
   aoc_utils::printSoltuion(1, flashes);
   aoc_utils::printSoltuion(2, getFirstCompleteFlash(grid));
}
