#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <map>
#include <vector>

#include <boost/assign/list_of.hpp>


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
         std::cout << "ERROR: Could not parse line: " << line << std::endl;
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
            std::cout << "ERROR: Could not parse direction: " << directionValue  << " in line "
               << line << "." << std::endl;
            std::cout << e.what() << std::endl;
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

   // sample file ------
   if (!readCommands("sample", &commands))
      return 1;

   int horizontal, depth;
   calculateDistances(false, horizontal, depth, &commands);

   int product = horizontal * depth;
   if (product != 150)
      std:: cout << "Part 1) Sample test FAILED: Expected 150 and caclulated " << product << std::endl; 
   else
      std::cout << "Part 1) Sample test PASSED" << std::endl;

   calculateDistances(true, horizontal, depth, &commands);
   product = horizontal * depth;

   if (product != 900)
      std:: cout << "Part 2) Sample test FAILED: Expected 900 and caclulated " << product << std::endl; 
   else
      std::cout << "Part 2) Sample test PASSED" << std::endl;

   // input file ----
   if (!readCommands("input", &commands))
      return 1;

   calculateDistances(false, horizontal, depth, &commands);
   std::cout << "Part 1) Product of horizontal distance and depth: " << horizontal * depth << std::endl;

   calculateDistances(true, horizontal, depth, &commands);
   std::cout << "Part 2) Product of horizontal distance and depth: " << horizontal * depth << std::endl;

   return 0;
}

