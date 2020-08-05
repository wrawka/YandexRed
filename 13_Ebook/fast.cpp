#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <set>

// #include "test_runner.h"

using namespace std;

// template <typename T>
// void PrintSomething(T source, string hint) {
//   cout << "Printing " << hint << ": ";
//   for (auto out : source) {
//     cout << out << " ";
//   }
//   cout << endl;
// }


struct Reader
{
  int user_id;
  int pages;
};

using myIt = set<Reader>::iterator;

// auto SortByPages = [](Reader lhs, Reader rhs) { return lhs.pages < rhs.pages; };

// bool SortByPages(Reader lhs, Reader rhs) {
//   return lhs.pages < rhs.pages;
// }

bool operator<(Reader lhs, Reader rhs) {
  return (lhs.pages <= rhs.pages) && (lhs.user_id != rhs.user_id);
}

ostream& operator<<(ostream& os, Reader& r) {
  return os << "ID:" << r.user_id << " finished " << r.pages << " pages\n";
}

class ReadingManager {
public:
  ReadingManager()
      : registered_users(MAX_USER_COUNT_ + 1, false)
      , reader_access(MAX_USER_COUNT_ + 1) {}

  void Read(int user_id, int page_count) {
    if (!registered_users[user_id]) {
      AddUser(user_id, page_count);
    } else {
      UpdateUser(user_id, page_count);
    } 
  }

  double Cheer(int user_id) const {
    if (!registered_users[user_id]) {
      return 0;
    }
    const int user_count = GetUserCount();
    if (user_count == 1) {
      return 1;
    }
    auto losers = distance(readers.begin(), reader_access[user_id]);
    // cout << "'distance' for UID:" << user_id << " is " << losers << endl;
    // По умолчанию деление целочисленное, поэтому
    // нужно привести числитель к типу double.
    // Простой способ сделать это — умножить его на 1.0.
    return (losers) * 1.0 / (user_count - 1);
  }

  // void Dump() const {
  //   PrintSomething(readers, "readers details");
  //   PrintSomething(registered_users, "registered readers");
  // }

private:
  // Статическое поле не принадлежит какому-то конкретному
  // объекту класса. По сути это глобальная переменная,
  // в данном случае константная.
  // Будь она публичной, к ней можно было бы обратиться снаружи
  // следующим образом: ReadingManager::MAX_USER_COUNT.
  static const int MAX_USER_COUNT_ = 100'000;

  // vector<int> user_page_counts_;  // индекс вектора [user_id] -> кол-во страниц
  // vector<int> sorted_users_;   // user_id внутри; отсортированы по убыванию количества страниц
  // vector<int> user_positions_; // позиции в векторе sorted_users_

  int GetUserCount() const {    // O(1)
    return readers.size(); 
  }

  set<Reader> readers;
  // set<Reader> readers;
  vector<bool> registered_users;
  vector<myIt> reader_access;
  
  void AddUser(int user_id, int pages = 0) {
    // cerr << "Adding user " << user_id << "..." << endl;
    try
    {
      pair<myIt, bool> add = readers.insert({user_id, pages});
      // cerr << "User added" << endl;
      registered_users[user_id] = true;
      // cerr << "User registered" << endl;
      reader_access[user_id] = add.first;
      // cerr << "Iterator stored" << endl;
    }
    catch(const std::exception& e)
    {
      std::cerr << e.what() << '\n';
    }
  }

  void UpdateUser(int user_id, int pages) {
    // cerr << "Updating user " << user_id << "..." << endl;
    auto& target = reader_access[user_id];
    // cerr << "target caught" << endl;
    readers.erase(target);
    // cerr << "target erased" << endl;
    target = readers.insert({user_id, pages}).first;
    // cerr << "target updated" << endl;
  }

  // void SwapUsers(int lhs_position, int rhs_position) {
  //   const int lhs_id = sorted_users_[lhs_position];
  //   const int rhs_id = sorted_users_[rhs_position];
  //   swap(sorted_users_[lhs_position], sorted_users_[rhs_position]); // O(1)
  //   swap(user_positions_[lhs_id], user_positions_[rhs_id]);         // O(1)
  // }
};


// void TestReadingManager() {
//   // Coursera Test #1
//   ReadingManager rm;
//   ASSERT_EQUAL(rm.Cheer(5), 0);
//   cerr << __LINE__ << " done" << endl;
//   rm.Read(1, 10);
//   cerr << __LINE__ << " done" << endl;
//   ASSERT_EQUAL(rm.Cheer(1), 1);
//   cerr << __LINE__ << " done" << endl;
//   rm.Read(2, 5);
//   cerr << __LINE__ << " done" << endl;
//   rm.Read(3, 7);
//   cerr << __LINE__ << " done" << endl;
//   rm.Dump();
//   ASSERT_EQUAL(rm.Cheer(2), 0);
//   cerr << __LINE__ << " done" << endl;
//   rm.Dump();
//   ASSERT_EQUAL(rm.Cheer(3), 0.5);
//   cerr << __LINE__ << " done" << endl;
//   rm.Read(3, 10);
//   cerr << __LINE__ << " done" << endl;
//   rm.Dump();
//   ASSERT_EQUAL(rm.Cheer(3), 0.5);
//   cerr << __LINE__ << " done" << endl;
//   rm.Read(3, 11);
//   cerr << __LINE__ << " done" << endl;
//   ASSERT_EQUAL(rm.Cheer(3), 1);
//   cerr << __LINE__ << " done" << endl;
//   ASSERT_EQUAL(rm.Cheer(1), 0.5);
//   cerr << __LINE__ << " done" << endl;
// }

int main() {
  // Для ускорения чтения данных отключается синхронизация
  // cin и cout с stdio,
  // а также выполняется отвязка cin от cout
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  // TODO: ^^^ проверь, что тут происходит ^^^

  // TestRunner tr;

  // RUN_TEST(tr, TestReadingManager);

  ReadingManager manager;

  int query_count;
  cin >> query_count;

  for (int query_id = 0; query_id < query_count; ++query_id) {
    string query_type;
    cin >> query_type;
    int user_id;
    cin >> user_id;

    if (query_type == "READ") {
      int page_count;
      cin >> page_count;
      manager.Read(user_id, page_count);
    } else if (query_type == "CHEER") {
      cout << setprecision(6) << manager.Cheer(user_id) << "\n";
    }
  }

  return 0;
}