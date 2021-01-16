#include "test_runner.h"
#include "logging.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <list>
#include <set>
#include <utility>
#include <vector>
#include <string>
#include <ostream>


using namespace std;

template<typename T>
struct RankedObject {
  RankedObject() = default;
  RankedObject(const T& o, uint32_t r = 0) : object(o), rank(r) {}
  RankedObject(T&& o, uint32_t r = 0) : object(move(o)), rank(r) {}
  bool operator<(const RankedObject& other) const {
    return rank < other.rank;
  }
	ostream& operator<<(ostream& os) {
		os << rank << ":" << object;
		return os;
	}
  T object;
  uint32_t rank;
};

int main() {
	set<RankedObject<string>> string_collection;
	string ass("ass", 3);
	string butt("butt");
	string_collection.insert(begin(string_collection), RankedObject(ass));
	string_collection.insert(begin(string_collection), RankedObject(butt));
	PrintRange(string_collection.begin(), string_collection.end());
  return 0;
}