#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <unordered_set>

#include "test_runner.h"

using namespace std;

int main() {
  // Для ускорения чтения данных отключается синхронизация
  // cin и cout с stdio,
  // а также выполняется отвязка cin от cout
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int query_count;
  cin >> query_count; // Q <= 10^5

  for (int query_id = 0; query_id < query_count; ++query_id) {
    string query_type;
    cin >> query_type;

    if (query_type == "BOOK") {
      long long int time; // -10^18 < time < 10^18
      cin >> time;
      string hotel_name;  // hotel_name < 12 characters
      cin >> hotel_name;
      unsigned long int client_id; // client_id < 10^9
      cin >> client_id;
      int room_count; // room_count < 1000
      cin >> room_count;
      // [...]
    } else if (query_type == "CLIENTS") {
      string hotel_name;  // hotel_name < 12 characters
      cin >> hotel_name;
      // [...]
    } else if (query_type == "ROOMS") {
      string hotel_name;  // hotel_name < 12 characters
      cin >> hotel_name;
      // [...]
    }
  }

  return 0;
}