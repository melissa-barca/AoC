#include <_types/_uint64_t.h>
#include <array>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <utility>

#include "../AoCUtils.cpp"

using namespace std;

class PolymerManual
{
public:
   typedef pair<array<char,2>, char> t_rule;
   typedef map<string, uint64_t> t_pairMap;

   PolymerManual(const string& fileName)
   {
      readInput(fileName);
      commonCount_ = 0;
      leastCommonCount_ = 0;
   }

   void applyStepsPt2(const int& count);
   void analyzePolymerPt2();

   uint64_t getDifference() const;

   char lastChar_;
   map<string, char> rules_;
   string polymer_;

   t_pairMap pairCounts_;
   uint64_t commonCount_, leastCommonCount_;
   char mostCommonChar_, leastCommonChar_;

private:
   void readInput(const string& fileName);
   void initializePolymer();
};

void PolymerManual::readInput(const string& fileName)
{
   rules_.clear();
   polymer_.clear();

   ifstream file;
   file.open(fileName);

   string line;
   while (getline(file, line))
   {
      if (line.empty())
         continue;

      size_t pos = line.find(' ');
      if (pos == line.npos)
         polymer_ = line;
      else
      {
         rules_[line.substr(0, 2)] = line.at(6);
      }
   }

   lastChar_ = polymer_.at(polymer_.size() - 1);
}

void PolymerManual::applyStepsPt2(const int& count)
{
   if (polymer_.empty())
      return;

   for (size_t i = 0; i < polymer_.size() + 1; i++)
   {
      auto rule = rules_.find(polymer_.substr(i, 2));
      if (rule != rules_.end())
      {
         if (pairCounts_.find(rule->first) == pairCounts_.end())
            pairCounts_[rule->first] = 1;
         else
            pairCounts_[rule->first]++;
      }
   }

   for (int steps = 0; steps < count; steps++)
   {
      t_pairMap newCounts;
      for (auto it : pairCounts_)
      {
         string pattern = it.first;
         auto rule = rules_.find(pattern);
         string newPattern1(1, pattern.at(0));
         newPattern1.push_back(rule->second);
         if (newCounts.find(newPattern1) == newCounts.end())
            newCounts[newPattern1] = it.second;
         else
            newCounts[newPattern1] += it.second;

         string newPattern2(1, rule->second);
         newPattern2.push_back(pattern.at(1));
         if (newCounts.find(newPattern2) == newCounts.end())
            newCounts[newPattern2] = it.second;
         else
            newCounts[newPattern2] += it.second;
      }
      pairCounts_.swap(newCounts);
   }
}

void PolymerManual::analyzePolymerPt2()
{
   std::map<char,uint64_t> counts;
 
   // last letter gets one extra
   counts[lastChar_] = 1;

   for (auto it : pairCounts_)
   {
      auto c = counts.find(it.first.at(0));
      if (c == counts.end())
         counts[it.first.at(0)] = it.second;
      else
         counts[it.first.at(0)] += it.second;
   }

   bool first = true;
   for (auto it : counts)
   {
      if (first)
      {
         commonCount_ = counts[it.first];
         leastCommonCount_ = counts[it.first];

         mostCommonChar_ = it.first;
         leastCommonChar_ = it.first;

         first = false;
         continue;
      }
         
      if (counts[it.first] > commonCount_)
      {
         mostCommonChar_ = it.first;
         commonCount_ = counts[it.first]; 
      }
      else if (counts[it.first] < leastCommonCount_)
      {
         leastCommonChar_ = it.first;
         leastCommonCount_ = counts[it.first];
      }
   }

}

uint64_t PolymerManual::getDifference() const
{
   return commonCount_ - leastCommonCount_;
}

void process(const string& fileName, const int& steps, uint64_t* pValue)
{
   PolymerManual pm(fileName);
   pm.applyStepsPt2(steps);
   pm.analyzePolymerPt2();

   *pValue = pm.getDifference();
}

int main()
{
   uint64_t value;
   process("sample", 10, &value);
   aoc_utils::printTestResults(1, 1588, (int) value);

   process("sample", 40, &value);
   aoc_utils::printTestResults(2, 2188189693529, value);

   process("input", 10, &value);
   aoc_utils::printSoltuion(2, value);

   process("input", 40, &value);
   aoc_utils::printSoltuion(2, value);

   return 0;
}
