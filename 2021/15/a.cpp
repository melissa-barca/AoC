#include <fstream>
#include <iostream>
#include <queue>
#include <utility>
#include <vector>

#include "../AoCUtils.cpp"

using namespace std;

typedef aoc_utils::vector_t vector_t;
typedef pair<int, int> coords_t;

const int kMultiplier = 5;

struct Point
{
   Point (int x, coords_t y)
   {
      riskLevel_ = x;
      coords_ = y;
   }

   int riskLevel_;
   coords_t coords_;
};

struct Comparator
{
   bool operator()(Point const& p1, Point const& p2)
   {
      return p1.riskLevel_ > p2.riskLevel_;
   }
};

void process(vector_t* pGrid, int* pSmallMapRisk, int* pBigMapRisk)
{
   int height = pGrid->size();
   int width = pGrid[0].size();

   vector_t totalRisk(height * kMultiplier,
                      vector<int>(width * kMultiplier, INT_MAX));
   vector<coords_t> neighbors{{0, 1}, {1, 0}, {-1, 0}, {0, -1}};
   priority_queue<Point, vector<Point>, Comparator> pq; 
   
   pq.push(Point( 0, { 0, 0 } ));
   
   while(!pq.empty())
   {
      Point top = pq.top();
      pq.pop();

      coords_t point = top.coords_;
      int riskLevel_ = top.riskLevel_;
      int x = point.first;
      int y = point.second;
      
      if (x < 0 || y < 0 || x >= kMultiplier * height || y >= kMultiplier * width)
        continue;
      
      int currRisk = x / height + y / width + (*pGrid)[x % height][y % width];
      
      currRisk = currRisk % 9 == 0 ? 9 :
                             (currRisk - (currRisk/9) * 9);
      
      if (totalRisk[x][y] == INT_MAX ||
         (currRisk + riskLevel_ < totalRisk[x][y]))
        totalRisk[x][y] = currRisk + riskLevel_;
      else
        continue;
         
      if (x == 5 * height-1 && y == 5 * width - 1)
        break;
         
     for(size_t i = 0; i < neighbors.size(); i++)
     {
       pq.push(Point(totalRisk[x][y],
               { x + neighbors[i].first,
                 y + neighbors[i].second }));
      }
   }
   
   *pSmallMapRisk =  totalRisk[height - 1][width - 1] - (*pGrid)[0][0];
   *pBigMapRisk = totalRisk[5 * height - 1][5 * width - 1] - (*pGrid)[0][0];
}
   
int main()
{
   vector_t grid;
   int smallMapRisk, bigMapRisk;

   aoc_utils::readGridInput("sample", &grid);
   process(&grid, &smallMapRisk, &bigMapRisk);
   aoc_utils::printTestResults(1, 40, smallMapRisk);
   aoc_utils::printTestResults(2, 315, bigMapRisk);

   aoc_utils::readGridInput("input", &grid);
   process(&grid, &smallMapRisk, &bigMapRisk);
   aoc_utils::printSoltuion(1, smallMapRisk);
   aoc_utils::printSoltuion(2, bigMapRisk);
}
