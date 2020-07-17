#pragma once

#include <string>
#include <iostream>
#include <vector>
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

// Date
bool operator<(const Date& lhs, const Date& rhs) {
  return vector<int>{lhs.year, lhs.month, lhs.day} < vector<int>{rhs.year, rhs.month, rhs.day};
}

bool operator==(const Date& lhs, const Date& rhs) {
  return (lhs.year == rhs.year && lhs.month == rhs.month && lhs.day == rhs.day);
}

ostream& operator<<(ostream& os, const Date& date) {
  os << date.year << "-" << date.month << "-" << date.day;
  return os;
}
// /Date

// Time
bool operator<(const Time& lhs, const Time& rhs) {
  return vector<int>{lhs.hours, lhs.minutes} < vector<int>{rhs.hours, rhs.minutes};
}

bool operator==(const Time& lhs, const Time& rhs) {
  return lhs.hours == rhs.hours && lhs.minutes == rhs.minutes;
}

ostream& operator<<(ostream& os, const Time& time) {
  os << time.hours << ":" << time.minutes;
  return os;
}
// /Time