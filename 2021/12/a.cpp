//#include <fstream>
#include <iostream>
#include <map>
#include <string>

#include "../AoCUtils.cpp"

using namespace std;

const string kStart("start");
const string kEnd("end");

struct Cave
{
   Cave()
   {
      name_ = "";
      big_ = false;
      pretendBig_ = false;
   }
   Cave(string name):
      name_(name)
   {
      big_ = (name.at(0) >= 'A' && name.at(0) <= 'Z');
      pretendBig_ = false;
   }

   string name_;
   bool big_;
   bool pretendBig_;
};

struct Graph
{
   Graph()
   {}

   shared_ptr<Cave> addVertex(Cave& cave)
   {
      auto it = nameLookup_.find(cave.name_);
      if (it == nameLookup_.end())
      {
         shared_ptr<Cave> cavePtr = make_shared<Cave>(cave);
         nameLookup_[cave.name_] = cavePtr;
         adj_.insert({cave.name_, vector<shared_ptr<Cave>>()});
         return cavePtr;
      }
      return it->second;
   }

   void addEdge(Cave cave1, Cave cave2)
   {
      shared_ptr<Cave> cave1Ptr = addVertex(cave1);
      shared_ptr<Cave> cave2Ptr = addVertex(cave2);

      adj_[cave1.name_].push_back(cave2Ptr);
      adj_[cave2.name_].push_back(cave1Ptr);
   }

   void getPaths(Cave* pStart, Cave* pEnd, map<string,bool> visited, vector<Cave> path,
         int& total)
   {
      if (!pStart->big_)
      {
         if (!pStart->pretendBig_)
            visited[pStart->name_] = 1;
         else
         {
            int count = 0;
            for (size_t i = 0; i < path.size() && count < 2; i++)
            {
               if (path.at(i).name_ == pStart->name_)
                  ++count;
            }
            if (count >= 2)
               return;
         }
      }

      path.push_back(*pStart);

      if (pStart->name_ == pEnd->name_)
         total++;
      else
      {
         for (auto it : adj_[pStart->name_])
         {
            if (!visited[it->name_])
               getPaths(&*it, pEnd, visited, path, total);
         }
      }
   }

   int getPaths(bool allowOnePass)
   {
      map<string, bool> visited;
      for (auto it : nameLookup_)
         visited.insert({it.first, 0});

      vector<Cave> path;
      int total = 0;

      getPaths(nameLookup_[kStart].get(), nameLookup_[kEnd].get(), visited, path, total);
      if (!allowOnePass)
         return total;

      int originalTotal = total;
      for (auto it : nameLookup_)
      {
         if (it.first != kStart && it.first != kEnd && !nameLookup_.at(it.first)->big_)
         {
            int tmpTotal = 0;
            it.second->pretendBig_ = true;
            getPaths(nameLookup_[kStart].get(), nameLookup_[kEnd].get(), visited, path, tmpTotal);
            it.second->pretendBig_ = false;
            total += tmpTotal - originalTotal;
         }
      }
      return total;
   }


   map<string, shared_ptr<Cave>> nameLookup_;
   map<std::string, vector<shared_ptr<Cave>>> adj_;
};


bool readInput(std::string fileName, Graph* pGraph)
{
   std::ifstream file;
   file.open(fileName);

   std::string in;
   while (file >> in)
   {
      size_t pos = in.find('-');
      if (pos == std::string::npos)
      {
         aoc_utils::printParseError(in, "Could not find separator.");
         return false;
      }

      pGraph->addEdge(in.substr(0, pos), in.substr(pos + 1));
   }
   return 1;
}

int process(string file, bool allowOnePass = false)
{
   Graph caves;
   readInput(file, &caves);
   return caves.getPaths(allowOnePass);
}

int main()
{

   aoc_utils::printTestResults(1, 10, process("sample"));
   aoc_utils::printTestResults(1, 226, process("sample2"));
   aoc_utils::printTestResults(1, 36, process("sample", true));
   aoc_utils::printTestResults(1, 3509, process("sample2", true));
   
   aoc_utils::printSoltuion(1, process("input"));
   aoc_utils::printSoltuion(2, process("input", true));

   return 0;
}

