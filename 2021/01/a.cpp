#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int part1(string file)
{
   int total = 0;
   int prev = -1;

   ifstream myfile;
   myfile.open(file);

   int num;
   while (myfile >> num)
   {
      if (prev > -1 && num > prev)
         total++;
      prev = num;
   }
   myfile.close();

   return total;
}

int part2(string file)
{
   int total = 0;
   int prevSum = INT_MAX;
   int one = INT_MIN;
   int two = INT_MIN;
   int three = INT_MIN;

   ifstream myfile;
   myfile.open(file);

   while (myfile >> three)
   {
      if (one == INT_MIN)
         one = three;
      else if (two == INT_MIN)
         two = three;
      else 
      {
         int sum = one + two + three;
         if (sum > prevSum)
            total++;
         prevSum = sum;
         one = two;
         two = three;
      }
   }

   myfile.close();
   return total;
}

int main()
{
   if (part1("sample") == 7)
      cout << "Part 1 sample test PASSED." << endl;
   else
      cout << "Part 1 sample test FAILED." << endl;

   if (part2("sample") == 5)
      cout << "Part 2 sample test PASSED." << endl;
   else
      cout << "Part 2 sample test FAILED." << endl;

   cout << "Part 1 total: " << part1("input") << endl;
   cout << "Part 2 total: " << part2("input") << endl;

   return 0;
}

