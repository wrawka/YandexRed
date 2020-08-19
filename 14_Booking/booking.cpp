#include <iostream>
#include <vector>
#include <utility>
#include <map>
#include <algorithm>
#include <unordered_set>

// #include "test_runner.h"

using namespace std;

struct Booking {
  long long int time = 0;
  unsigned long int client_id = 0;
  int rooms = 0;
};

ostream& operator<<(ostream& os, Booking b) {
  os << "time: " << b.time << " ";
  os << "client_id: " << b.client_id << " ";
  os << "rooms: " << b.rooms << endl;
  return os;
}

class BookingSystem {
private:
  static const int secin24 = 86400; // seconds in 24 hours
  long long int current_time; // = -1'000'000'000'000'000'000;
  map<string, vector<Booking>> bookings;

public:
  void NewBooking(const long long int& time
                , const string& hotel_name
                , const unsigned long int& client_id
                , int room_count);
  int GetClients(const string& hotel_name);
  int GetRooms(const string& hotel_name);
};

void BookingSystem::NewBooking(const long long int& time
                            , const string& hotel_name
                            , const unsigned long int& client_id
                            , int room_count)
{
  current_time = time;
  bookings[hotel_name].push_back(Booking{time, client_id, room_count});
  // cout << Booking{time, client_id, room_count};
}

int BookingSystem::GetClients(const string& hotel_name) {
  unordered_set<unsigned long> result;
  auto hotel = bookings[hotel_name];
  if (hotel.empty()) return 0;
  // cout << "delta_time is: " << (current_time - secin24) << endl;
  for (auto it = hotel.rbegin(); ((*it).time > (current_time - secin24) && it != hotel.rend()); it++) {
    // cout << *it;
    result.insert((*it).client_id);
  }
  // cout << endl;
  return result.size();
}

int BookingSystem::GetRooms(const string& hotel_name) {
  int result = 0;
  auto hotel = bookings[hotel_name];
  if (hotel.empty()) return 0;
  for (auto it = hotel.rbegin(); ((*it).time > (current_time - secin24) && it != hotel.rend()); it++) {
    result += (*it).rooms;
  }
  return result;
}

// void TestBooking() {
//   BookingSystem ts;
//   ASSERT_EQUAL(ts.GetClients("Marriott"), 0);
//   ASSERT_EQUAL(ts.GetRooms("Marriott"), 0);
//   ts.NewBooking(10, "FourSeasons", 1, 2);
//   ts.NewBooking(10, "Marriott", 1, 1);
//   ts.NewBooking(86409, "FourSeasons", 2, 1);
//   //
//   ASSERT_EQUAL(ts.GetClients("FourSeasons"), 2);
//   ASSERT_EQUAL(ts.GetRooms("FourSeasons"), 3);
//   ASSERT_EQUAL(ts.GetClients("Marriott"), 1);
//   ts.NewBooking(86410, "Marriott", 2, 10);
//   ASSERT_EQUAL(ts.GetRooms("FourSeasons"), 1);
//   ASSERT_EQUAL(ts.GetRooms("Marriott"), 10);
// }


int main() {
  // Для ускорения чтения данных отключается синхронизация
  // cin и cout с stdio,
  // а также выполняется отвязка cin от cout
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  // TestRunner tr;

  // RUN_TEST(tr, TestBooking);

  int query_count;
  cin >> query_count; // Q <= 10^5

  BookingSystem bs;

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
      bs.NewBooking(time, hotel_name, client_id, room_count);
      // [...]
    } else if (query_type == "CLIENTS") {
      string hotel_name;  // hotel_name < 12 characters
      cin >> hotel_name;
      cout << bs.GetClients(hotel_name) << endl;
      // [...]
    } else if (query_type == "ROOMS") {
      string hotel_name;  // hotel_name < 12 characters
      cin >> hotel_name;
      cout << bs.GetRooms(hotel_name) << endl;
      // [...]
    }
  }

  return 0;
}