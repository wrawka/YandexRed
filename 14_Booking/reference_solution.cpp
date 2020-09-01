#include <cstdint>
#include <iostream>
#include <map>
#include <queue>

using namespace std;


// Действительно, сложность решения складывается из трёх частей:

// Поиск названия отеля по словарю hotels_ на каждый запрос: O(QL logQ).
// Суммарное время работы всех вызовов метода Book: O(Q logQ).
// Суммарное время работы всех вызовов метода RemoveOldBookings. 
// Поскольку каждое добавленное событие может быть удалено не более 1 раза, 
// получаем не более Q вызовов метода PopBooking, то есть O(Q logQ).
// Итого, путём поглощения получаем итоговую оценку O(QL logQ). 
// Величины T, C, R и W в оценке сложности не участвуют: 
// T, R и C определяют лишь необходимую размерность типов, 
// а W влияет лишь на количество удалений: 
// чем больше W, тем меньше в конечном счёте будет вызовов PopBooking.

class HotelManager {
public:
  void Book(int64_t time, const string& hotel_name,
            int client_id, int room_count) {
    current_time_ = time;
    hotels_[hotel_name].Book({time, client_id, room_count}); // O(logQ)
  }
  int ComputeClientCount(const string& hotel_name) {
    return hotels_[hotel_name].ComputeClientCount(current_time_); // O(logW? + W*logQ)
  }
  int ComputeRoomCount(const string& hotel_name) {
    return hotels_[hotel_name].ComputeRoomCount(current_time_); // O(logW? + W*logQ)
  }

private:
  struct Booking {
    int64_t time;
    int client_id;
    int room_count;
  };

  class HotelInfo {
  public:
    void Book(const Booking& booking) { // O(1)
      last_bookings_.push(booking); 
      room_count_ += booking.room_count;
      ++client_booking_counts_[booking.client_id]; // O(logQ)
    }
    int ComputeClientCount(int64_t current_time) {
      RemoveOldBookings(current_time); // O(W*logQ)
      return client_booking_counts_.size(); // O(1)
    }
    int ComputeRoomCount(int64_t current_time) {
      RemoveOldBookings(current_time); // O(W*logQ)
      return room_count_;
    }
  private:
    static const int TIME_WINDOW_SIZE = 86400;
    queue<Booking> last_bookings_;
    int room_count_ = 0;
    map<int, int> client_booking_counts_;

    void PopBooking() {
      const Booking& booking = last_bookings_.front(); // O(1)
      room_count_ -= booking.room_count; // O(1)
      const auto client_stat_it =
          client_booking_counts_.find(booking.client_id); // O(logQ)
      if (--client_stat_it->second == 0) {
        client_booking_counts_.erase(client_stat_it); // O(a1)
      }
      last_bookings_.pop(); // O(1)
    }
    void RemoveOldBookings(int64_t current_time) {
      while ( // total "W" polls
          !last_bookings_.empty()
          && last_bookings_.front().time <= current_time - TIME_WINDOW_SIZE
      ) {
        PopBooking(); // O(logQ)
      }
    }
  };

  int64_t current_time_ = 0;
  map<string, HotelInfo> hotels_;
};


int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  
  HotelManager manager;

  int query_count;
  cin >> query_count;

  for (int query_id = 0; query_id < query_count; ++query_id) {
    string query_type;
    cin >> query_type;

    if (query_type == "BOOK") {
      int64_t time;
      cin >> time;
      string hotel_name;
      cin >> hotel_name;
      int client_id, room_count;
      cin >> client_id >> room_count;
      manager.Book(time, hotel_name, client_id, room_count);
    } else {
      string hotel_name;
      cin >> hotel_name;
      if (query_type == "CLIENTS") {
        cout << manager.ComputeClientCount(hotel_name) << "\n";
      } else if (query_type == "ROOMS") {
        cout << manager.ComputeRoomCount(hotel_name) << "\n";
      }
    }
  }

  return 0;
}
