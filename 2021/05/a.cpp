#include <boost/geometry/geometries/segment.hpp>
#include <boost/geometry/algorithms/intersection.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/geometry/index/rtree.hpp>

#include <boost/geometry/geometries/register/point.hpp>

#include <fstream>
#include <sstream>
#include <string>
#include <sys/unistd.h>
#include <utility>
#include <vector>


#include "../AoCUtils.cpp"

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

typedef bg::model::point<int, 2, bg::cs::cartesian> Point;
typedef bg::model::segment<Point> Segment;
typedef bgi::rtree<Segment, bgi::linear<16>> Segments;

struct PointCount
{
   PointCount(Point point):
      point_(point)
   {
      count_ = 1;
   }
   
   PointCount(const PointCount& p)
   {
      this->point_ = p.point_;
      this->count_ = p.count_;
   }

   PointCount operator++()
   {
      this->count_++;
      return *this;
   }

   bool operator>(const int& i) const
   {
      return this->count_ > i;
   }

   Point point_;
   int count_;
};

bool readInput(std::string fileName, Segments* pSegments)
{
   pSegments->clear();

   std::ifstream file;
   file.open(fileName);

   std::string line;
   while (getline(file, line))
   {
      std::stringstream ss(line);

      std::vector<int> coords;
      for (int i; ss >> i;)
      {
         coords.push_back(i);
         if (ss.peek() == ',')
            ss.ignore();
         else if (ss.peek() == ' ')
         {
            ss.ignore(); // ignore space
            ss.ignore(); // ignore -
            ss.ignore(); // ignore >
            ss.ignore(); // ignore space
         }
      }
      if (coords.size() != 4)
         aoc_utils::printParseError(line, "Invalid number of coordinates");
      pSegments->insert(
         Segment(
            Point(coords.at(0), coords.at(1)),
            Point(coords.at(2), coords.at(3))));
   }

   return 1;
}

void getInbetweenPoints(const Point a, const Point b, std::vector<Point>* pPoints)
{
   int x1 = a.get<0>(); // 0
   int y1 = a.get<1>(); // 9
   int x2 = b.get<0>(); // 2
   int y2 = b.get<1>(); // 9

   //std::cout << "Get inbetween points between (" << x1 << "," << y1 << ") and " <<
   //   "(" << x2 << "," << y2 << ")" << std::endl;

   if (x1 != x2)
   {
      int diff = std::abs(x1 - x2); // 2
      //std::cout << "diff: " << diff << std::endl;
      if (x1 > x2)
      {
         for (int i = 1; i < diff; i++)
            pPoints->push_back(Point(++x2, y1));
      }
      else
      {
         for (int i = 1; i < diff; i++)
            pPoints->push_back(Point(++x1, y1));
      }
   }
   else if (y1 != y2)
   {
      int diff = std::abs(y1 - y2);
      if (y1 > y2)
      {
         for (int i = 1; i < diff; i++)
            pPoints->push_back(Point(x1, ++y2));
      }
      else
      {
         for (int i = 1; i < diff; i++)
            pPoints->push_back(Point(x1, ++y1));
      }
   }
   else
      std::cout << "Diagnonal inbetween" << std::endl;
}

bool isHorizontalOrVertical(const Segment& seg)
{
   if (seg.first.get<0>() == seg.second.get<0>() ||
       seg.first.get<1>() == seg.second.get<1>())
      return true;
   return false;
}

void getIntersects(const bool onlyHorizontalAndVertical, const Segments* segments,
  std::vector<PointCount>* pIntersects)
{
   pIntersects->clear();

   Segments::const_iterator first = segments->begin();
   Segments::const_iterator second = segments->begin();
   second++;

   while (first != segments->end())
   {
      if (!onlyHorizontalAndVertical ||
          (onlyHorizontalAndVertical && isHorizontalOrVertical(*first)))
      {
         while (second != segments->end())
         {
            if (!onlyHorizontalAndVertical ||
                (onlyHorizontalAndVertical && isHorizontalOrVertical(*second)))
            {
               std::vector<Point> intersects;
               boost::geometry::intersection(*first, *second, intersects);
      
               if (intersects.size() == 2)
                  getInbetweenPoints(intersects.at(0), intersects.at(1), &intersects);
      
               for (auto const& point : intersects)
               {
                  auto it = std::find_if(pIntersects->begin(), pIntersects->end(),
                                        [point](const PointCount& p)
                                        {
                                           return point.get<0>() == p.point_.get<0>() &&
                                                  point.get<1>() == p.point_.get<1>();
                                        });
      
                  if (it == pIntersects->end())
                  {
                     pIntersects->push_back(PointCount(point));
                  }
                  else
                  {
                     ++*it;
                  }
               }
            }
            second++;
         }
      }
      first++;
      second = first;
      second++;
   }
}

int main()
{
   Segments segments;
   if (!readInput("sample", &segments))
      return 1;

   std::vector<PointCount> intersects;

   getIntersects(true, &segments, &intersects);
   aoc_utils::printTestResults(1, 5, intersects.size());

   getIntersects(false, &segments, &intersects);
   aoc_utils::printTestResults(2, 12, intersects.size());

   if (!readInput("input", &segments))
      return 1;

   getIntersects(true, &segments, &intersects);
   aoc_utils::printSoltuion(1, intersects.size());

   getIntersects(false, &segments, &intersects);
   aoc_utils::printSoltuion(2, intersects.size());
   if (intersects.size() >= 23542)
      aoc_utils::printError("Solution is too high");
}

