#include <iostream>
#include <vector>
#include <utility>
#include <map>
#include <algorithm>
#include <unordered_set>

#include "test_runner.h"

using namespace std;

class BookingSystem {
private:
  long long int current_time;
  map<long long int, map<string, int> > rooms;
  map<long long int, map<string, unordered_set<int>> > clients;

  pair<long long int, long long int> Get24H() const {
    return {current_time - 86400, current_time};
  }

public:
  void NewBooking(const long long int& time
                , const string& hotel_name
                , const unsigned long int& client_id
                , int room_count);
  int GetClients(const string& hotel_name) const;
  int GetRooms(const string& hotel_name) const;
};

void BookingSystem::NewBooking(const long long int& time
                            , const string& hotel_name
                            , const unsigned long int& client_id
                            , int room_count)
{
  current_time = time;
  rooms[time][hotel_name] += room_count;
  clients[time][hotel_name].insert(client_id);
}

int BookingSystem::GetClients(const string& hotel_name) const {
  int result = 0;
  auto range = Get24H();
  cout << "Range is: " << range.first << " | " << range.second << endl;

  return result;
}

int BookingSystem::GetRooms(const string& hotel_name) const {
  int result = 0;

  return result;
}

void TestBooking() {
  BookingSystem ts;
  ASSERT_EQUAL(ts.GetClients("Marriott"), 0);
  ASSERT_EQUAL(ts.GetRooms("Marriott"), 0);
  ts.NewBooking(10, "FourSeasons", 1, 2);
  ts.NewBooking(10, "Marriott", 1, 1);
  ts.NewBooking(86409, "FourSeasons", 2, 1);
  //
  ASSERT_EQUAL(ts.GetClients("FourSeasons"), 2);
  ASSERT_EQUAL(ts.GetRooms("FourSeasons"), 3);
  ASSERT_EQUAL(ts.GetClients("Marriott"), 1);
  ts.NewBooking(86410, "Marriott", 2, 10);
  ASSERT_EQUAL(ts.GetRooms("FourSeasons"), 1);
  ASSERT_EQUAL(ts.GetRooms("Marriott"), 10);
}


int main() {
  // Для ускорения чтения данных отключается синхронизация
  // cin и cout с stdio,
  // а также выполняется отвязка cin от cout
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  TestRunner tr;

  RUN_TEST(tr, TestBooking);

  // int query_count;
  // cin >> query_count; // Q <= 10^5

  // BookingSystem bs;

  // for (int query_id = 0; query_id < query_count; ++query_id) {
  //   string query_type;
  //   cin >> query_type;

  //   if (query_type == "BOOK") {
  //     long long int time; // -10^18 < time < 10^18
  //     cin >> time;
  //     string hotel_name;  // hotel_name < 12 characters
  //     cin >> hotel_name;
  //     unsigned long int client_id; // client_id < 10^9
  //     cin >> client_id;
  //     int room_count; // room_count < 1000
  //     cin >> room_count;
  //     bs.NewBooking(time, hotel_name, client_id, room_count);
  //     // [...]
  //   } else if (query_type == "CLIENTS") {
  //     string hotel_name;  // hotel_name < 12 characters
  //     cin >> hotel_name;
  //     cout << bs.GetClients(hotel_name) << endl;
  //     // [...]
  //   } else if (query_type == "ROOMS") {
  //     string hotel_name;  // hotel_name < 12 characters
  //     cin >> hotel_name;
  //     cout << bs.GetRooms(hotel_name) << endl;
  //     // [...]
  //   }
  // }

  return 0;
}