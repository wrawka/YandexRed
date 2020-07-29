#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <set>
#include <sstream>

#include "test_runner.h"

using namespace std;

class RouteManager {
public:
  void AddRoute(int start, int finish) {
    reachable_lists_[start].insert(finish);
    reachable_lists_[finish].insert(start);
  }
  int FindNearestFinish(int start, int finish) const {
    int result = abs(start - finish);
    if (reachable_lists_.find(start) == reachable_lists_.end()) {
        return result;
    }
    const set<int>& reachable_stations = reachable_lists_.at(start);
    if (!reachable_stations.empty()) {
      auto guess = *(--reachable_stations.upper_bound(finish));
      result = min(
        result,
        abs(finish - guess)
      );
    }
    return result;
  }
private:
  map<int, set<int>> reachable_lists_;
};

void TestRoutes() {

}


int main() {

  RouteManager routes;

  int query_count;
  cin >> query_count;

  for (int query_id = 0; query_id < query_count; ++query_id) {
    string query_type;
    cin >> query_type;
    int start, finish;
    cin >> start >> finish;
    if (query_type == "ADD") {
      routes.AddRoute(start, finish);
    } else if (query_type == "GO") {
      cout << routes.FindNearestFinish(start, finish) << "\n";
    }
  }

  return 0;
}
