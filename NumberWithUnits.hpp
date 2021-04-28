#pragma once
#include <iostream>
#include <string>
#include <map>
using namespace std;
namespace ariel
{
    static map<string, map<string, double>> UnitConvertMap;
    static const float epsilon = 0.001;
    static bool unitValidity(const string &UnitType)
    {
      if(UnitConvertMap.count(UnitType)>0)
      {
        return true;
      }
      else
      {
        return false;
      }
    }
    class NumberWithUnits
    {
    private:
        double num;
        std::string UnitType;
      
    public:
    NumberWithUnits(double num, const string &UnitType);
        ~NumberWithUnits() {}

        static void read_units(std::ifstream &units_file);

        NumberWithUnits operator+(const NumberWithUnits &other);
        NumberWithUnits &operator+=(const NumberWithUnits &other);
        NumberWithUnits operator-(const NumberWithUnits &other);
        NumberWithUnits &operator-=(const NumberWithUnits &other);
        NumberWithUnits operator+(); 
        NumberWithUnits operator-();

        NumberWithUnits &operator++();
        NumberWithUnits &operator--();
        NumberWithUnits operator++(int one);
        NumberWithUnits operator--(int one);

        NumberWithUnits operator*(double f);



        friend bool operator==(const NumberWithUnits &num1, const NumberWithUnits &num2);
        friend bool operator!=(const NumberWithUnits &num1, const NumberWithUnits &num2);
        friend bool operator>(const NumberWithUnits &num1, const NumberWithUnits &num2);
        friend bool operator>=(const NumberWithUnits &num1, const NumberWithUnits &num2);
        friend bool operator<(const NumberWithUnits &num1, const NumberWithUnits &num2);
        friend bool operator<=(const NumberWithUnits &num1, const NumberWithUnits &num2);

        friend NumberWithUnits operator*(double f, const NumberWithUnits &num);

        friend std::ostream &operator<<(std::ostream &os, const NumberWithUnits &num);
        friend std::istream &operator>>(std::istream &is, NumberWithUnits &num);
    };
}