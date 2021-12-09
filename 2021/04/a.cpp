#include <fstream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "../AoCUtils.cpp"

struct Number
{
   Number(int number):
      number_(number)
   {
      called_ = false;
   }

   void mark()
   {
      called_ = true;
   }

   int number_;
   bool called_;
};

struct Row : std::vector<Number>
{
   bool isWinner()
   {
      for (Number num : *this)
      {
         if (!num.called_)
            return false;
      }
      return true;
   }

   bool has(int num)
   {
      for (int i = 0; i < (int) this->size(); i++)
      {
         if (this->at(i).number_ == num && this->at(i).called_)
            return true;
      }
      return false;
   }

   void call(int n)
   {
      for (int i = 0; i < (int)this->size(); i++)
      {
         if (this->at(i).number_ == n)
         {
            this->at(i).mark();
         }
      }
   }

   void print()
   {
      for (Number num : *this)
         std::cout << num.number_ << " ";
      std::cout << std::endl;
   }
   
   int score()
   {
      int sum = 0;
      for (Number n : *this)
      {
         if (!n.called_)
            sum += n.number_;
      }
      return sum;
   }
};

struct Board : std::vector<Row>
{
   int rowSize()
   {
      return this->begin()->size();
   }

   bool isMarked(int n)
   {
      for (Row row : *this)
      {
         if (row.has(n))
            return true;
      }
      return false;
   }

   void call(int n)
   {
      int count = 0;
      for (int i = 0; i < (int) this->size(); i++)
      {
         this->at(i).call(n);
         count++;
      }
   }

   bool isWinner()
   {
      if (winner_)
         return true;

      for (Row row : *this)
      {
         if (row.isWinner())
         {
            winner_ = true;
            return true;
         }
      }

      for (int i = 0; i < rowSize(); i++)
      {
         bool columnWon = true;
         for (Row row : *this)
         {
            if (!row.at(i).called_)
            {
               columnWon = false;
               break;
            }
         }
         if (columnWon)
         {
            winner_ = true;
            return true;
         }
      }

      return false;
   }

   int score(int winningNumber)
   {
      int sum = 0;
      for (Row row : *this)
      {
         sum += row.score();
      }

      return sum * winningNumber;
   }

   bool winner_ = false;
};

bool readInput(const std::string fileName,
               std::vector<int>* pCalledNumbers,
               std::vector<Board>* pBoards)
{
   pCalledNumbers->clear();
   pBoards->clear();

   std::ifstream file;
   file.open(fileName);

   std::string line;
   getline(file, line);

   try
   {
      std::stringstream ss(line);

      for (int i; ss >> i;)
      {
         pCalledNumbers->push_back(i);
         if (ss.peek() == ',')
         {
            ss.ignore();
         }
      }
   }
   catch (...)
   {
      aoc_utils::printParseError(line, "Could not parse called number input");
      return 0;
   }

   while (std::getline(file, line))
   {
      if (!line.empty())
      {
         Board board;
         do
         {
            std::stringstream ss(line);
      
            Row row;
            for (int i; ss >> i;)
            {
               row.push_back(Number(i));
               if (ss.peek() == ' ')
               {
                  ss.ignore();
               }
            }
            board.push_back(row);
         }
         while (std::getline(file, line) && !line.empty());
         if (!board.empty())
            pBoards->push_back(board);
      }
   }

   file.close();
   return 1;
}

void printBoard(Board* pBoard)
{
   for (Row row : *pBoard)
   {
      for (Number num : row)
      {
         std::cout << num.number_ << " " ;
      }
      std::cout << std::endl;
   }
}

void printWinningBoard(Board* pBoard)
{
   for (Row row : *pBoard)
   {
      for (Number num : row)
      {
         std::cout << num.called_ << " ";
      }
      std::cout << std::endl;
   }
}


void callNumbers(const std::vector<int>* pCalledNumbers,
                 std::vector<Board>* pBoards,
                 std::vector<std::pair<Board*,int>>* pWinners)
{
   pWinners->clear();
   for (int i : *pCalledNumbers)
   {
      for (int b = 0; b < (int) pBoards->size(); b++)
      {
         pBoards->at(b).call(i);
         if (pBoards->at(b).isWinner())
         {
            pWinners->push_back(std::make_pair(&pBoards->at(b), i));
         }
      }
      if (!pWinners->empty())
         return;
   }
}

int main()
{
   std::vector<int> calledNumbers;
   std::vector<Board> boards;

   if (!readInput("sample", &calledNumbers, &boards))
      return 1;

   std::vector<std::pair<Board*,int>> winningBoards;
   callNumbers(&calledNumbers, &boards, &winningBoards);

   aoc_utils::printTestResults(1, 4512,
                               winningBoards.at(0).first->score(winningBoards.at(0).second));

   if (!readInput("input", &calledNumbers, &boards))
      return 1;

   callNumbers(&calledNumbers, &boards, &winningBoards);
   aoc_utils::printSoltuion(1, winningBoards.at(0).first->score(winningBoards.at(0).second));

   return 0;
}
