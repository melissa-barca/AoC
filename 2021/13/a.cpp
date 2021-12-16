#include <exception>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <string>
#include <utility>

#include "../AoCUtils.cpp"

using namespace std;


class Paper
{
   enum Direction
   {
      X,
      Y
   };

   typedef pair<int,int> t_point;
   const map<string,Direction> kDirectionMap = { {"x", X}, {"y", Y} };
   typedef pair<Direction,int> t_line;

   void readCoordinates(const string& fileName);

public:

   Paper(const std::string fileName)
   {
      folds_ = 0;
      maxCoord_ = { 0, 0 };
      readCoordinates(fileName);
   }

   void fold();
   void fold(const int& numOfFolds);

   void print() const;
   void printPoints() const;
   void printFold(const t_line& fold) const;

   int getPoints() const
   {
      return points_.size();
   }

   t_point maxCoord_;
   int folds_;
   set<t_point> points_;
   vector<t_line> directions_;
};


void Paper::readCoordinates(const string& fileName)
{
   points_.clear();
   directions_.clear();

   ifstream file;
   file.open(fileName);

   string line;
   while (getline(file, line))
   {
      size_t pos = line.find(',');
      if (pos != line.npos)
      {
         try
         {
            int x = stoi(line.substr(0, pos));
            int y = stoi(line.substr(pos + 1));

            points_.insert({ x, y });

            if (x > maxCoord_.first)
               maxCoord_.first = x;
            if (y > maxCoord_.second)
               maxCoord_.second = y;
         }
         catch(...)
         {
            aoc_utils::printParseError(line, "Could not parse point");
         }
      }
      else if ((pos = line.find('=')) != line.npos)
      {
         try
         {
            directions_.push_back({ kDirectionMap.at(line.substr(pos - 1, 1)),
                                    stoi(line.substr(pos + 1)) } );
         }
         catch (...)
         {
            aoc_utils::printParseError(line, "Could not parse direction");
         }
      }
      else if (!line.empty())
         aoc_utils::printParseError(line);
   }
}

void Paper::fold()
{
   assert((int) directions_.size() >= folds_);
   fold((int) directions_.size() - folds_);
}

void Paper::fold(const int& numOfFolds)
{
   if ((folds_ + numOfFolds) > (int) directions_.size())
      aoc_utils::printWarning("Paper has already been folded " +
            to_string(folds_) + " times and received request to fold " +
            to_string(numOfFolds) + " more times, but there are only " +
            to_string(directions_.size()) + " directions.");

   set<t_point> newPoints;
   int start = folds_;
   for (int i = start; i < (start + numOfFolds) && i < (int) directions_.size(); i++)
   {
      folds_++;
      t_line foldLine = directions_.at(i);

      if (foldLine.first == X)
         maxCoord_.first = foldLine.second;
      else 
         maxCoord_.second = foldLine.second;

      printFold(foldLine);
      for (t_point pt : points_)
      {
         if (foldLine.first == X)
         {
            if (pt.first <= foldLine.second)
               newPoints.insert(pt);
            else
               newPoints.insert({ foldLine.second - (pt.first - foldLine.second),
                                  pt.second });
         }
         else
         {
            if (pt.second <= foldLine.second)
               newPoints.insert(pt);
            else
               newPoints.insert({ pt.first,
                                  foldLine.second - (pt.second - foldLine.second) });
         }
      }
      points_.swap(newPoints);
   }
}

void Paper::print() const
{
   for (int row = 0; row < maxCoord_.second; row++)
   {
      for (int col = 0; col < maxCoord_.first; col++)
      {
         if (points_.find({ col, row }) != points_.end())
            cout << "█";
         else
            cout << "░";
      }
      cout << endl;
   }
}

void Paper::printPoints() const
{
   for (const t_point& pt : points_)
      cout << "(" << pt.first << ", " << pt.second << ")" << endl;
}

void Paper::printFold(const t_line& fold) const
{
   cout << "fold: " << ((fold.first == 0) ? "x" : "y") << " = " <<
      fold.second << endl;
}

int process(const string& fileName)
{
   Paper paper(fileName);
   paper.fold(1);

   int afterFirst = paper.getPoints();
   paper.fold();
   paper.print();

   return afterFirst;
}

int main()
{
   aoc_utils::printTestResults(1, 17, process("sample"));
   aoc_utils::printSoltuion(1, process("input"));
}


