#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "test_runner.h"

using namespace std;

class RouteManager {
public:
  void AddRoute(int start, int finish) {
    reachable_lists_[start].push_back(finish);
    reachable_lists_[finish].push_back(start);
  }
  int FindNearestFinish(int start, int finish) const {
    int result = abs(start - finish);
    if (reachable_lists_.find(start) == reachable_lists_.end()) { // не нашли точку отправления? стоим на месте
        return result;
    }
    // нашли точку отравления - ищем точку назначения
    const vector<int>& reachable_stations = reachable_lists_.at(start);
    if (!reachable_stations.empty()) { 
      result = min(
          result,
          abs(finish - *min_element(
              begin(reachable_stations), end(reachable_stations),
              [finish](int lhs, int rhs) { return abs(lhs - finish) < abs(rhs - finish); }
          ))
      );
    }
    return result;
  }
private:
  map<int, vector<int>> reachable_lists_;
};

void TestRoutes1() {
  // Test #1
  RouteManager test;
  test.AddRoute(-2, 5);
  test.AddRoute(10, 4);
  test.AddRoute(5, 8);
  ASSERT_EQUAL(test.FindNearestFinish(4, 10), 0);
  ASSERT_EQUAL(test.FindNearestFinish(4, -2), 6);
  ASSERT_EQUAL(test.FindNearestFinish(5, 0), 2);
  ASSERT_EQUAL(test.FindNearestFinish(5, 100), 92);
}

void TestRoutes() {
  // Custom
  RouteManager test;
  test.AddRoute(-2, 5);
  test.AddRoute(10, 4);
  test.AddRoute(5, 8);
  test.AddRoute(5, 10);
  ASSERT_EQUAL(test.FindNearestFinish(10, 8), 2);
  ASSERT_EQUAL(test.FindNearestFinish(10, 3), 1);
  ASSERT_EQUAL(test.FindNearestFinish(5, 9), 1);
  ASSERT_EQUAL(test.FindNearestFinish(5, -5), 3);
}

int main0() {
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

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestRoutes1);
  RUN_TEST(tr, TestRoutes);
  return 0;
}
