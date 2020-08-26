#include <iostream>
#include <vector>
#include <utility>
#include <unordered_map>
#include <algorithm>
#include <unordered_set>
#include <queue>

#include "test_runner.h"

// Failed case #15/17: time limit exceeded (Time used: 1.99/1.00)

using namespace std;

struct Booking {
  int64_t time;
  string hotel_name;
  unsigned long int client_id;
  int rooms;
};

ostream& operator<<(ostream& os, const Booking& b) {
  os << "time: " << b.time << " ";
  os << "hotel_name: " << b.hotel_name << " ";
  os << "client_id: " << b.client_id << " ";
  os << "rooms: " << b.rooms << endl;
  return os;
}

class BookingSystem {
private:
  static const int secin24 = 86400; // seconds in 24 hours
  int64_t current_time = -1'000'000'000'000'000'000;
  queue<Booking> bookings;
  unordered_map<string, int> hotel_rooms;
  unordered_map<string, queue<unsigned long int>> hotel_clients;
  // unordered_map<string, 
  void UpdateQ();

public:
  void NewBooking(const int64_t& time
                , const string& hotel_name
                , const unsigned long int& client_id
                , int room_count);
  int GetClients(const string& hotel_name);
  int GetRooms(const string& hotel_name);
};

void BookingSystem::UpdateQ() {
  auto delta = current_time - secin24;
  while(bookings.back().time <= delta) {
    hotel_rooms[bookings.back().hotel_name] -= bookings.back().rooms;
    hotel_clients[bookings.back().hotel_name].pop();
    bookings.pop();
  }
}

void BookingSystem::NewBooking(const int64_t& time
                            , const string& hotel_name
                            , const unsigned long int& client_id
                            , int room_count)
{
  current_time = time;
  bookings.push({time, hotel_name, client_id, room_count});
  hotel_rooms[hotel_name] += room_count;
  hotel_clients[hotel_name].push(client_id);
  UpdateQ();
}

int BookingSystem::GetClients(const string& hotel_name) {
  if (bookings.empty()) return 0;
  return hotel_clients.size();
}

int BookingSystem::GetRooms(const string& hotel_name) {
  if (bookings.empty()) return 0;
  else return hotel_rooms[hotel_name];
}

void TestBooking() {
  BookingSystem ts;
  ASSERT_EQUAL(ts.GetClients("Marriott"), 0);
  ASSERT_EQUAL(ts.GetRooms("Marriott"), 0);
  ts.NewBooking(10, "FourSeasons", 1, 2);
  ts.NewBooking(10, "Marriott", 1, 1);
  ts.NewBooking(86409, "FourSeasons", 2, 1);
  ASSERT_EQUAL(ts.GetClients("FourSeasons"), 2);
  ASSERT_EQUAL(ts.GetRooms("FourSeasons"), 3);
  ASSERT_EQUAL(ts.GetClients("Marriott"), 1);
  ts.NewBooking(86410, "Marriott", 2, 10);
  ASSERT_EQUAL(ts.GetRooms("FourSeasons"), 1);
  ASSERT_EQUAL(ts.GetRooms("Marriott"), 10);
}

void TestClients() {
  BookingSystem bs;
  bs.NewBooking(0, "Marriott", 0, 1);
  ASSERT_EQUAL(bs.GetClients("Marriott"), 1);
  ASSERT_EQUAL(bs.GetRooms("Marriott"), 1);
  bs.NewBooking(10, "Marriott", 0, 3);
  ASSERT_EQUAL(bs.GetClients("Marriott"), 1);
  ASSERT_EQUAL(bs.GetRooms("Marriott"), 4);
  bs.NewBooking(86411, "Marriott", 3, 1);
  ASSERT_EQUAL(bs.GetClients("Marriott"), 1);
  ASSERT_EQUAL(bs.GetRooms("Marriott"), 1);
}

void Test00() {
  BookingSystem bs;
  bs.NewBooking(1, "FourSeasons", 11, 4);
  bs.NewBooking(50000, "FourSeasons", 11, 5);
  bs.NewBooking(90000, "FourSeasons", 22, 2);
  ASSERT_EQUAL(bs.GetRooms("FourSeasons"), 7);
  ASSERT_EQUAL(bs.GetClients("FourSeasons"), 2);
}

void Test01() {
  BookingSystem bs;
  bs.NewBooking(-86400, "hotel", 2, 2);
  bs.NewBooking(0, "hotel", 1, 5);
  ASSERT_EQUAL(bs.GetRooms("hotel"), 5);
  ASSERT_EQUAL(bs.GetClients("hotel"), 1);
}


int main() {
  // Для ускорения чтения данных отключается синхронизация
  // cin и cout с stdio,
  // а также выполняется отвязка cin от cout
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  TestRunner tr;

  RUN_TEST(tr, TestBooking);
  RUN_TEST(tr, TestClients);
  RUN_TEST(tr, Test00);
  RUN_TEST(tr, Test01);

  unsigned long int query_count;
  cin >> query_count; // Q <= 10^5

  BookingSystem bs;

  for (unsigned long int query_id = 0; query_id < query_count; ++query_id) {
    string query_type;
    cin >> query_type;

    if (query_type == "BOOK") {
      int64_t time; // -10^18 < time < 10^18
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