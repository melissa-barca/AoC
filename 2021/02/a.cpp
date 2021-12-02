#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <map>
#include <vector>

#include <boost/assign/list_of.hpp>

#include "../AoCUtils.cpp"

struct SubmarineCommand
{
   enum Direction { Forward, Down, Up };
   const std::map<std::string, Direction> directionMap =
      boost::assign::map_list_of("forward", Forward)("down", Down)("up", Up);

   SubmarineCommand(std::string line)
   {
      size_t pos = line.find(" ");
      if (pos == std::string::npos)
      {
         distance_ = -1;
         aoc_utils::printParseError(line);
      }
      else
      {
         std::string directionValue = line.substr(0, pos);
         std::string distanceValue = line.substr(pos);
         try
         {
            direction_ = directionMap.at(directionValue);
            distance_ = std::atoi(distanceValue.c_str());
         }
         catch (std::out_of_range e)
         {
            distance_ = -1;
            aoc_utils::printParseError(line, 
               "Could not parse direction " + directionValue + " " + e.what());
         }
      }
   }

   Direction direction_;
   int distance_;
};

// Read the submarine commands from file
// Returns false when the file could not be parsed
bool readCommands(std::string fileName, std::vector<SubmarineCommand>* pCommands)
{
   pCommands->clear();

   std::ifstream file;
   file.open(fileName);

   std::string line;
   while (std::getline(file, line))
   {
      SubmarineCommand command(line);

      if (command.distance_ == -1)
      {
         file.close();
         return false;
      }

      pCommands->push_back(command);
   }

   file.close();
   return true;
}

void calculateDistances(bool useAim, int& horizontal, int& depth,
   const std::vector<SubmarineCommand>* pCommands)
{
   horizontal = 0;
   depth = 0;

   int aim = 0;

   for (SubmarineCommand command : *pCommands)
   {
      switch(command.direction_)
      {
         case SubmarineCommand::Forward :
            horizontal += command.distance_;
            if (useAim)
               depth += (aim * command.distance_);
            break;

         case SubmarineCommand::Up :
            aim -= command.distance_;
            break;

         default: // SubmarineCommand::Down
            aim += command.distance_;
            break;
      }
   }

   if (!useAim)
      depth = aim;
}

int main()
{
   std::vector<SubmarineCommand> commands;
   int horizontal, depth;

   // sample file ------
   if (!readCommands("sample", &commands))
      return 1;

   calculateDistances(false, horizontal, depth, &commands);
   aoc_utils::printTestResults(1, 150, horizontal * depth);

   calculateDistances(true, horizontal, depth, &commands);
   aoc_utils::printTestResults(2, 900, horizontal * depth);

   std::cout << "---" << std::endl;

   // input file ----
   if (!readCommands("input", &commands))
      return 1;

   calculateDistances(false, horizontal, depth, &commands);
   aoc_utils::printSoltuion(1, horizontal * depth);

   calculateDistances(true, horizontal, depth, &commands);
   aoc_utils::printSoltuion(2, horizontal * depth);

   return 0;
}

