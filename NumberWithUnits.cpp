#include "NumberWithUnits.hpp"
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
using namespace std;
using namespace ariel;

namespace ariel {

NumberWithUnits::NumberWithUnits(double num, const string &UnitType) {
  if (unitValidity(UnitType)) {
    this->num = num;
    this->UnitType = UnitType;
  } else {
    throw invalid_argument("illegal unit!\n");
  }
}

double convert(const string &UnitOne, const string &UnitTwo, double amount) {
  if (UnitOne == UnitTwo) {
    return amount;
  }

  map<string, double> innermap = UnitConvertMap[UnitOne];
  map<string, double>::iterator it = innermap.find(UnitTwo);

  if (it == innermap.end()) {
    throw invalid_argument("can't convert, units doesn't Linked!\n");
  }

  return UnitConvertMap[UnitTwo][UnitOne] * amount;
}

void addUnits(string &Unit, string &UnitConvert) {
  for (auto &x : UnitConvertMap[UnitConvert]) {
    double new_val = UnitConvertMap[Unit][UnitConvert] * x.second;
    UnitConvertMap[Unit][x.first] = new_val;
    UnitConvertMap[x.first][Unit] = 1 / new_val;
  }
}

void NumberWithUnits::read_units(ifstream &units_file) {
  string Unit, UnitConvert, equal;
  double AmountUnit = 0, AmountConvert = 0;
  while (units_file >> AmountUnit >> Unit >> equal >> AmountConvert >>
         UnitConvert) {
    if ((AmountUnit == 1) && (equal == "=") && (AmountConvert > 0)) {
      UnitConvertMap[Unit][UnitConvert] = AmountConvert;
      UnitConvertMap[UnitConvert][Unit] = 1 / AmountConvert;

      addUnits(Unit, UnitConvert);
      addUnits(UnitConvert, Unit);
    } else {
      throw invalid_argument("format error!\n");
    }
  }
}

NumberWithUnits NumberWithUnits::operator+(const NumberWithUnits &other) {
  double tmp = convert(this->UnitType, other.UnitType, other.num);
  return NumberWithUnits(this->num + tmp, this->UnitType);
}

NumberWithUnits &NumberWithUnits::operator+=(const NumberWithUnits &other) {
  this->num += convert(this->UnitType, other.UnitType, other.num);
  return *this;
}

NumberWithUnits NumberWithUnits::operator-(const NumberWithUnits &other) {
  double tmp = convert(this->UnitType, other.UnitType, other.num);
  return NumberWithUnits(this->num - tmp, this->UnitType);
}

NumberWithUnits &NumberWithUnits::operator-=(const NumberWithUnits &other) {
  this->num -= convert(this->UnitType, other.UnitType, other.num);
  return *this;
}

NumberWithUnits NumberWithUnits::operator+() {
  return NumberWithUnits(this->num, this->UnitType);
}

NumberWithUnits NumberWithUnits::operator-() {
  return NumberWithUnits(-this->num, this->UnitType);
}

bool operator>(const NumberWithUnits &num1, const NumberWithUnits &num2) {
  double tmp = convert(num1.UnitType, num2.UnitType, num2.num);
  return (num1.num - tmp) > epsilon;
}

bool operator>=(const NumberWithUnits &num1, const NumberWithUnits &num2) {
  double tmp = convert(num1.UnitType, num2.UnitType, num2.num);
  return (num1.num > tmp) || (num1.num == tmp);
}

bool operator<(const NumberWithUnits &num1, const NumberWithUnits &num2) {
  double tmp = convert(num1.UnitType, num2.UnitType, num2.num);
  return (tmp - num1.num) > epsilon;
}

bool operator<=(const NumberWithUnits &num1, const NumberWithUnits &num2) {
  double tmp = convert(num1.UnitType, num2.UnitType, num2.num);
  return (num1.num < tmp) || (num1.num == tmp);
}

bool operator==(const NumberWithUnits &num1, const NumberWithUnits &num2) {
  double tmp = convert(num1.UnitType, num2.UnitType, num2.num);
  return (abs(num1.num - tmp) <= epsilon);
}

bool operator!=(const NumberWithUnits &num1, const NumberWithUnits &num2) {
  return (!(num1 == num2));
}

// Overloading ++/-- operators
NumberWithUnits &NumberWithUnits::operator++() {
  this->num++;
  return *this;
}

NumberWithUnits &NumberWithUnits::operator--() {
  --(this->num);
  return *this;
}

NumberWithUnits NumberWithUnits::operator++(int bulshit) {
  NumberWithUnits copy = *this;
  this->num++;
  return copy;
}

NumberWithUnits NumberWithUnits::operator--(int bulshit) {
  NumberWithUnits copy = *this;
  (this->num)--;
  return copy;
}

NumberWithUnits NumberWithUnits::operator*(double f) {
  return NumberWithUnits(this->num * f, this->UnitType);
}

NumberWithUnits operator*(double f, const NumberWithUnits &num) {
  return NumberWithUnits(num.num * f, num.UnitType);
}

ostream &operator<<(ostream &os, const NumberWithUnits &num) {
  os << num.num << "[" << num.UnitType << "]";
  return os;
}

istream &operator>>(istream &is, NumberWithUnits &num) {
  string str;
  string n1;
  string unit;

  getline(is, str, ']');

  for (size_t i = 0; i < str.length(); i++) {
    if (str[i] == ' ' || str[i] == '[') {
      continue;
    }
    if (('0' <= str[i] && str[i] <= '9') || '+' == str[i] || '-' == str[i] ||
        '.' == str[i]) {
      n1 += str[i];
    }
    if ('A' <= str[i] && str[i] <= 'z') {
      unit += str[i];
    }
  }

  if (unitValidity(unit) != 0) {
    num = NumberWithUnits(stod(n1), unit);
    return is;
  }

  throw invalid_argument(unit + " is invalid unit!\n");
}
} // namespace ariel