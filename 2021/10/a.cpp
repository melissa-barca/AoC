#include <cstdint>
#include <fstream>
#include <map>
#include <math.h>
#include <string>
#include <utility>
#include <vector>

#include "../AoCUtils.cpp"

typedef std::vector<std::pair<std::string, int>> lines_t;

bool readInput(std::string fileName, lines_t* pLines)
{
   pLines->clear();

   std::ifstream file;
   file.open(fileName);

   std::string line;
   while (std::getline(file, line))
   {
      pLines->push_back(std::make_pair(line, 0));
   }

   return 1;
}

const std::map<char, int> kErrorPoints = { {')', 3}, {']', 57}, {'}', 1197}, {'>', 25137} };
const std::map<char, int> kAutoCompletePoints = { {'(', 1}, {'[', 2}, {'{', 3}, {'<', 4} };

int getAutoCompleteWinner(lines_t* pLines)
{
   std::sort(pLines->begin(), pLines->end(), 
         [](std::pair<std::string,int> i, std::pair<std::string,int> j)
         {
            return i.second > j.second;
         });
   return pLines->at(ceil(pLines->size() / 2)).second;
}

void getScores(const lines_t& pLines, int* pErrorScore, int* pAutoCompleteScore)
{

   int errorScore = 0;
   lines_t incomplete;
   for (auto it : pLines)
   {
      std::stack<char> syntax;
      bool valid = true;
      for (const char& c : it.first)
      {
         if (std::string("([{<").find(c) != std::string::npos)
         {
            syntax.push(c);
         }
         else
         {
            if (syntax.empty())
               errorScore += kErrorPoints.at(c);
            else
            {
               char open = syntax.top();
               syntax.pop();
               if ((open == '(' && c != ')') ||
                   (open == '[' && c != ']') ||
                   (open == '{' && c != '}') ||
                   (open == '<' && c != '>'))
               {
                  errorScore += kErrorPoints.at(c);
                  valid = false;
                  break;
               }
            }
         }
      }
      if (valid && !syntax.empty())
      {
         while (!syntax.empty())
         {
            char c = syntax.top();
            syntax.pop();
            it.second *= 5;
            it.second += kAutoCompletePoints.at(c);
         }
         incomplete.push_back(std::make_pair(it.first, it.second));
      }
   }
   *pErrorScore = errorScore;
   *pAutoCompleteScore = getAutoCompleteWinner(&incomplete);
}

int main()
{
   lines_t lines;
   if (!readInput("sample", &lines))
      return 1;

   int error, autocomplete;
   getScores(lines, &error, &autocomplete);

   aoc_utils::printTestResults(1, 26397, error);
   aoc_utils::printTestResults(2, 288957, autocomplete);

   if (!readInput("input", &lines))
      return 1;
   getScores(lines, &error, &autocomplete);
   aoc_utils::printSoltuion(1, error);
   aoc_utils::printSoltuion(2, autocomplete);

}
