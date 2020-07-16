#include <algorithm>
#include <numeric>
#include <iostream>
#include "airline_ticket.h"

using namespace std;

#define SORT_BY(field)  \
    [](const AirlineTicket& lhs, const AirlineTicket& rhs) { \
    return lhs.field < rhs.field; \
  }

bool operator<(const Date& lhs, const Date& rhs) {
  if (lhs.year < rhs.year) return true;
  else if (lhs.month < rhs.month) return true;
  else if (lhs.day < rhs.day) return true;
  return false;
}

bool operator==(const Date& lhs, const Date& rhs) {
  return lhs.year == rhs.year && lhs.month == rhs.month && lhs.day == rhs.day;
}

ostream& operator<<(ostream& os, const Date& date) {
  os << date.year << "-" << date.month << "-" << date.day;
  return os;
}

bool operator<(const Time& lhs, const Time& rhs) {
  if (lhs.hours < rhs.hours) return true;
  else return lhs.minutes < rhs.minutes;
}

bool operator==(const Time& lhs, const Time& rhs) {
  return lhs.hours == rhs.hours && lhs.minutes == rhs.minutes;
}

ostream& operator<<(ostream& os, const Time& time) {
  os << time.hours << ":" << time.minutes;
  return os;
}