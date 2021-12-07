#include <boost/numeric/conversion/cast.hpp>

#include <fstream>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <vector>
#include <bitset>

#include "../AoCUtils.cpp"

const int kGammaRateLength = 5;
//const int kGammaRateLength = 12;
typedef std::vector< std::bitset<kGammaRateLength> > Matrix;

bool readMatrix(int gammaRateLength, std::string fileName, Matrix* pMatrix)
{
   if (pMatrix != nullptr)
      pMatrix->clear();

   std::ifstream file;
   file.open(fileName);
   
   std::string line;
   while (std::getline(file, line))
   {
      if (line.size() != gammaRateLength)
      {
         aoc_utils::printParseError(line, "Invalid length: " + std::to_string(line.size()));
         return 0;
      }
      std::bitset<kGammaRateLength> set(line);
      pMatrix->push_back(set);
   }

   return 1;
}

int getMostCommonBitInColumn(int column, int tieBreaker, const Matrix* matrix)
{
   int one = 0;
   int zero = 0;

   for (auto v : *matrix)
   {
      if (v[column] == 1)
      {
         one++;
      }
      else
      {
         zero++;
      }
   }
 
   if (one == zero)
      return tieBreaker;
   
   return one > zero;
}

void removeSetsWithBit(int column, int bit, Matrix* pMatrix)
{
   Matrix newMatrix;

   for (auto v : *pMatrix)
   {
      if (v[column] == bit)
      {
         newMatrix.push_back(v);
      }
   }

   *pMatrix = newMatrix;
}

int getProduct(u_long gammaRate, u_long epsilonRate)
{
   return boost::numeric_cast<int>(gammaRate * epsilonRate);
}

std::bitset<kGammaRateLength> getOxygenGeneratorRating(const Matrix matrix)
{
   Matrix candidateMatrix = matrix;

   for (int i = kGammaRateLength - 1; i >= 0; i--)
   {
      int bit = getMostCommonBitInColumn(i, 1, &candidateMatrix);
      removeSetsWithBit(i, bit, &candidateMatrix);

      if (candidateMatrix.size() == 1)
         return candidateMatrix[0];
   }

   return candidateMatrix[0];
}

std::bitset<kGammaRateLength> getC02ScrubberRating(const Matrix matrix)
{
   Matrix candidateMatrix = matrix;

   for (int i = kGammaRateLength - 1; i >= 0; i--)
   {
      int bit = !getMostCommonBitInColumn(i, 1, &candidateMatrix);
      removeSetsWithBit(i, bit, &candidateMatrix);

      if (candidateMatrix.size() == 1)
         return candidateMatrix[0];
   }

   return candidateMatrix[0];
}

int main()
{
   Matrix matrix;
   // sample file ----
   if (!readMatrix(kGammaRateLength, "sample", &matrix))
      return 1;

   {
      std::bitset<kGammaRateLength> gammaRate;
      std::bitset<kGammaRateLength> epsilonRate;

      for (int i = 0; i < kGammaRateLength; i++)
      {
         gammaRate[i] = getMostCommonBitInColumn(i, 0, &matrix);
         epsilonRate[i] = !gammaRate[i];
      }
      aoc_utils::printTestResults(1, 
                                  198,
                                  getProduct(gammaRate.to_ulong(),
                                             epsilonRate.to_ulong()));

      aoc_utils::printTestResults(2,
                                  230,
                                  getProduct(getOxygenGeneratorRating(matrix).to_ulong(),
                                             getC02ScrubberRating(matrix).to_ulong()));

   }


   // input file ----
   /*
   if (!readMatrix(kGammaRateLength, "input", &matrix))
      return 1;

   {

      std::bitset<kGammaRateLength> gammaRate;
      std::bitset<kGammaRateLength> epsilonRate;
      for (int i = 0; i < kGammaRateLength; i++)
      {
         gammaRate[i] = getMostCommonBitInColumn(i, 0, &matrix);
         epsilonRate[i] = !gammaRate[i];
      }
      aoc_utils::printSoltuion(1,
                               getProduct(gammaRate.to_ulong(),
                                          epsilonRate.to_ulong()));

      aoc_utils::printSoltuion(2,
                               getProduct(getOxygenGeneratorRating(matrix).to_ulong(),
                                          getC02ScrubberRating(matrix).to_ulong()));
   }
   */
}
