#pragma once

#include <string>
#include <iostream>
using namespace std;

struct Date {
  int year, month, day;
};

struct Time {
  int hours, minutes;
};

struct AirlineTicket {
  string from;
  string to;
  string airline;
  Date departure_date;
  Time departure_time;
  Date arrival_date;
  Time arrival_time;
  int price;
};

bool operator<(const Date& lhs, const Date& rhs) {
  if (lhs.year < rhs.year) return true;
  else if (lhs.month < rhs.month) return true;
  else return lhs.day < rhs.day;
}

bool operator==(const Date& lhs, const Date& rhs) {
  return lhs.year == rhs.year && lhs.month == rhs.month && lhs.day == rhs.day;
}

ostream& operator<<(ostream& os, const Date& date) {
  os << date.year << "-" << date.month << "-" << date.day;
  return os;
}