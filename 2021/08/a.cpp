#include <algorithm>
#include <fstream>
#include <map>
#include <string>
#include <sstream>
#include <vector>

#include "../AoCUtils.cpp"

const int kInputSize = 10;
const int kOutputSize = 4;
struct Entry
{
private:
   bool isSubset(std::string small, std::string big)
   {
      for (size_t i = 0; i < small.size(); i++)
      {
         if (big.find(small.at(i)) == std::string::npos)
            return false;
      }
      return true;
   }

public:
   void populateMap()
   {
      std::vector<std::string*> unknown5s;
      std::vector<std::string*> unknown6s;

      for (int i = 0; i < kInputSize; i++)
      {
         switch (input_[i].size())
         {
            case 2 :
               map_[1] = input_[i];
               break;

            case 4 :
               map_[4] = input_[i];
               break;

            case 3 :
               map_[7] = input_[i];
               break;

            case 7 :
               map_[8] = input_[i];
               break;

            case 5 :
               unknown5s.push_back(&input_[i]);
               break;

            case 6 :
               unknown6s.push_back(&input_[i]);
               break;

            default :
               aoc_utils::printError("Unexpected string size: " + std::to_string(input_[i].size())
                     + " found for string: " + input_[i]);
         }
      }

      // The unknown 5 digit value that is a subset of 1's characters is 3
      for (size_t i = 0; i < unknown5s.size(); i++)
      {
         std::string str = *unknown5s.at(i);
         if (isSubset(map_[1], str))
         {
            map_[3] = str;
            unknown5s.erase(unknown5s.begin() + i);
         }
      }

      // The unknown six digit value that isn't a subset of 1's characters  is 6
      for (size_t i = 0; i < unknown6s.size(); i++)
      {
         std::string str = *unknown6s.at(i);
         if (!isSubset(map_[1], str))
         {
            map_[6] = str;
            unknown6s.erase(unknown6s.begin() + i);
         }
      }

      // Out of the four remaining values (2 of length 5 and 2 of length 6), the values where
      // the unknown length 5 is a subset of the unknown length 6 are 5 and 9
      for (size_t i = 0; i < unknown5s.size(); i++)
      {
         std::string unknown5 = *unknown5s.at(i);
         for (size_t j = 0; j < unknown6s.size(); j++)
         {
            std::string unknown6 = *unknown6s.at(j);
            if (isSubset(unknown5, unknown6))
            {
               map_[5] = unknown5;
               map_[9] = unknown6;
               unknown5s.erase(unknown5s.begin() + i);
               unknown6s.erase(unknown6s.begin() + j);

               map_[2] = *unknown5s.at(0);
               map_[0] = *unknown6s.at(0);
               unknown5s.pop_back();
               unknown6s.pop_back();
            }
         }
      }
   }

   bool decodeOutput()
   {
      if (map_.empty())
         return false;

      for (int i = 0; i < kOutputSize; i++)
      {
         auto result = std::find_if(map_.begin(), map_.end(),
               [&](const auto& map) { return map.second == output_[i]; });

         if (result == map_.end())
         {
            aoc_utils::printError("Could not decode output. Found unknown pattern " + output_[i]);
            return false;
         }

         decodedOutput_.append(std::to_string(result->first));
      }
      return true;
   }

   int getValue()
   {
      if (decodedOutput_.empty())
         return -1;

      return std::stoi(decodedOutput_);
   }

   std::string input_[kInputSize];
   std::string output_[kOutputSize];

   std::map<int, std::string> map_;
   std::string decodedOutput_;
};

// 1, 4, 7, 8
const int kUniqueLengths[4] = { 2, 4, 3, 7 };

bool hasUniqueLength(std::string pattern)
{
   return std::any_of(std::begin(kUniqueLengths), std::end(kUniqueLengths), [&](int i)
          {
             return (int) pattern.size() == i;
          });
}

bool readInput(std::string fileName, int& uniqueLengths, std::vector<Entry>* pEntries)
{
   uniqueLengths = 0;
   pEntries->clear();

   std::ifstream file;
   file.open(fileName);

   std::string line;
   while (getline(file, line))
   {
      std::stringstream ss(line);
      bool output = false;
      int outputCount = 0;
      int inputCount = 0;
      Entry entry;
      for (std::string i; ss >> i;)
      {
         std::sort(i.begin(), i.end());
         if (output)
         {
            entry.output_[outputCount] = i;
            if (hasUniqueLength(i))
               ++uniqueLengths;

            if (outputCount == (kOutputSize - 1))
            {
               outputCount = 0;
               output = false;
            }
            else
               outputCount++;

         }
         else if (i == "|")
         {
            output = true;
         }
         else
         {
            entry.input_[inputCount] = i;

            if (inputCount == (kInputSize - 1))
               inputCount = 0;
            else
               inputCount++;
         }
      }
      pEntries->push_back(entry);
   }
   return 1;
}

int getTotalSum(std::vector<Entry>* pEntries)
{
   int sum = 0;
   for (auto it : *pEntries)
   {
      it.populateMap();

      if (!it.decodeOutput())
         return 1;

      sum += it.getValue();
   }
   return sum;
}

int main()
{
   std::vector<Entry> entries;
   int uniqueLengthCount = 0;

   if (!readInput("sample", uniqueLengthCount, &entries))
      return 1;

   aoc_utils::printTestResults(1, 26, uniqueLengthCount);
   aoc_utils::printTestResults(2, 61229, getTotalSum(&entries));


   if (!readInput("input", uniqueLengthCount, &entries))
      return 1;

   aoc_utils::printSoltuion(1, uniqueLengthCount);
   aoc_utils::printSoltuion(2, getTotalSum(&entries));


   return 0;
}
