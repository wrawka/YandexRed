#include <algorithm>
#include <numeric>
#include <iostream>
#include <vector>
#include "airline_ticket.h"

using namespace std;

#define SORT_BY(field)  \
    [](const AirlineTicket& lhs, const AirlineTicket& rhs) { \
    return lhs.field < rhs.field; \
  }

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

bool operator<(const Time& lhs, const Time& rhs) {
  return vector<int>{lhs.hours, lhs.minutes} < vector<int>{rhs.hours, rhs.minutes};
}

bool operator==(const Time& lhs, const Time& rhs) {
  return (lhs.hours == rhs.hours && lhs.minutes == rhs.minutes);
}

ostream& operator<<(ostream& os, const Time& time) {
  os << time.hours << ":" << time.minutes;
  return os;
}