#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

#include <boost/multiprecision/cpp_int.hpp>

#include "../AoCUtils.cpp"

using namespace std;
namespace mp = boost::multiprecision;

void readTransmission(const string& fileName, string* pTrans)
{
   ifstream file;
   file.open(fileName);

   string transmission;
   for (char c; file >> c;)
   {
      switch(c)
      {
         case ('0'):
            transmission += "0000";
            break;
         case ('1'):
            transmission += "0001";
            break;
         case ('2'):
            transmission += "0010";
            break;
         case ('3'):
            transmission += "0011";
            break;
         case ('4'):
            transmission += "0100";
            break;
         case ('5'):
            transmission += "0101";
            break;
         case ('6'):
            transmission += "0110";
            break;
         case ('7'):
            transmission += "0111";
            break;
         case ('8'):
            transmission += "1000";
            break;
         case ('9'):
            transmission += "1001";
            break;
         case ('A'):
            transmission += "1010";
            break;
         case ('B'):
            transmission += "1011";
            break;
         case ('C'):
            transmission += "1100";
            break;
         case ('D'):
            transmission += "1101";
            break;
         case ('E'):
            transmission += "1110";
            break;
         default:
            transmission += "1111";
      }
   }
   *pTrans = transmission;
   cout << transmission << endl;
}

void popVersionAndType(std::string* pContents, int *pVersion, int *pType)
{
  char* pEnd;
  string versionBin(pContents->substr(0, 3));
  *pVersion = strtoull(pContents->substr(0, 3).c_str(), &pEnd, 2);
  *pType = strtoull(pContents->substr(3, 3).c_str(), &pEnd, 2);
  *pContents = pContents->substr(6);
}

void popString(const int& bits, std::string* pContents, std::string* pString)
{
   *pString = pContents->substr(0, bits);
   *pContents = pContents->substr(bits);
}

void popInt(const int& bits, std::string* pContents, int* pInt)
{
   char* pEnd;
   *pInt = strtoull(pContents->substr(0, bits).c_str(), &pEnd, 2);
   *pContents = pContents->substr(bits);
}

mp::uint1024_t getValue(const int& type, const vector<mp::uint1024_t>& values)
{
   if (type < 4)
   {
      mp::uint1024_t value = (type < 1 || type > 2) ?
                       0 : type == 1 ?
                       1 : UINT64_MAX;
      for (auto v : values)
      {
         switch(type)
         {
            case 0:
               value += v;
               break;
            case 1:
               value *= v;
               break;
            case 2:
               if (v < value)
                  value = v;
               break;
            default:
               if (v > value)
                  value = v;
         }
      }
      return value;
   }
   else if (type == 4)
   {
      aoc_utils::printError("Attempting to calculate value for type 4.");
   }
   {
      int value;
      if (values.size() != 2)
      {
         aoc_utils::printError("Could not calculate packet value of type " + to_string(type) +
               " found " + to_string(values.size()) + " values and expected 2.");
         return -1;
      }
      switch (type)
      {
         case 5:
            value = values.at(0) > values.at(1);
            break;
         case 6:
            value = values.at(0) < values.at(1);
            break;
         default:
            value = values.at(0) == values.at(1);
      }
      return value;
   }
}

mp::uint1024_t getType4Value(string* pTransmission)
{
   int lastPacket = 1; // boolean
   string value;
   while (lastPacket != 0)
   {
      if (pTransmission->size() < 5)
      {
         aoc_utils::printError("Ran out of bits while still parsing");
         return -1;
      }

      popInt(1, pTransmission, &lastPacket);

      string bits;
      popString(4, pTransmission, &bits);
      value.append(bits);
   }
   char* pEnd;
   return strtoull(value.c_str(), &pEnd, 2);
}

void readPackets(string* pTransmission, int* pVersionSum, vector<mp::uint1024_t>* pValues)
{
   while (pTransmission->find("1") != pTransmission->npos)
   {
      int version;
      int type;
      popVersionAndType(pTransmission, &version, &type);
      *pVersionSum += version;
   
      if (type == 4)
      {
         string tmp = *pTransmission;
         mp::uint1024_t value = getType4Value(&tmp);
         *pTransmission = tmp;
         pValues->push_back(value);
         return;
      }
      else
      {
         int lengthTypeId;
         popInt(1, pTransmission, &lengthTypeId);
         vector<mp::uint1024_t> myValues;
   
         if (lengthTypeId == 1) //  next 11 bits are the number of subpackets contained
         {
            int numberOfSubpackets;
            popInt(11, pTransmission, &numberOfSubpackets);
            vector<mp::uint1024_t> subpacketValues;
            for (int i = 0; i < numberOfSubpackets; i++)
            {
               readPackets(pTransmission, pVersionSum, &subpacketValues);
               myValues.push_back(subpacketValues.at(subpacketValues.size() - 1));
            }
            mp::uint1024_t value = getValue(type, myValues);
            pValues->push_back(value);
         }
         else // next 15 bits are the total length in bits of the sub packets
         {
            int lengthOfSubpackets;
            popInt(15, pTransmission, &lengthOfSubpackets);
   
            string subpacket;
            popString(lengthOfSubpackets, pTransmission, &subpacket);
            while (!subpacket.empty())
               readPackets(&subpacket, pVersionSum, &myValues);
            mp::uint1024_t value = getValue(type, myValues);
            pValues->push_back(value);
         }
      }
   }
}

int main()
{
   string trans;

   vector<mp::uint1024_t> values;
   int versionSum = 0;
   readTransmission("sample", &trans);
   readPackets(&trans, &versionSum, &values);
   aoc_utils::printTestResults(1, 16, versionSum);
   cout << endl;

   versionSum = 0;
   readTransmission("sample1", &trans);
   readPackets(&trans, &versionSum, &values);
   aoc_utils::printTestResults(1, 23, versionSum);
   cout << endl;

   versionSum = 0;
   readTransmission("sample2", &trans);
   readPackets(&trans, &versionSum, &values);
   aoc_utils::printTestResults(1, 31, versionSum);
   cout << endl;
   versionSum = 0;
   values.clear();
   readTransmission("sample3", &trans);
   readPackets(&trans, &versionSum, &values);
   aoc_utils::printTestResults(2, 3, (int) values.at(values.size() - 1));
   cout << endl;

   versionSum = 0;
   values.clear();
   readTransmission("sample4", &trans);
   readPackets(&trans, &versionSum, &values);
   aoc_utils::printTestResults(2, 54, (int) values.at(values.size() - 1));
   cout << endl;

   versionSum = 0;
   values.clear();
   readTransmission("sample5", &trans);
   readPackets(&trans, &versionSum, &values);
   aoc_utils::printTestResults(2, 7, (int) values.at(values.size() - 1));
   cout << endl;

   versionSum = 0;
   values.clear();
   readTransmission("sample6", &trans);
   readPackets(&trans, &versionSum, &values);
   aoc_utils::printTestResults(2, 9, (int) values.at(values.size() - 1));
   cout << endl;

   versionSum = 0;
   values.clear();
   readTransmission("sample7", &trans);
   readPackets(&trans, &versionSum, &values);
   aoc_utils::printTestResults(2, 1, (int) values.at(values.size() - 1));
   cout << endl;

   versionSum = 0;
   values.clear();
   readTransmission("sample8", &trans);
   readPackets(&trans, &versionSum, &values);
   aoc_utils::printTestResults(2, 0, (int) values.at(values.size() - 1));
   cout << endl;

   versionSum = 0;
   values.clear();
   readTransmission("sample9", &trans);
   readPackets(&trans, &versionSum, &values);
   aoc_utils::printTestResults(2, 0, (int) values.at(values.size() - 1));
   cout << endl;

   versionSum = 0;
   values.clear();
   readTransmission("sample10", &trans);
   readPackets(&trans, &versionSum, &values);
   aoc_utils::printTestResults(2, 1, (int) values.at(values.size() - 1));
   cout << endl;

   cout << "input----" << endl;
   versionSum = 0;
   values.clear();
   readTransmission("input", &trans);
   readPackets(&trans, &versionSum, &values);
   aoc_utils::printSoltuion(1, versionSum);
   cout << "Part 2) Solution: " << values.at(values.size() - 1);
}
