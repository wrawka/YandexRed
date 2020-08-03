#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>
#include <set>
#include <algorithm>

#include "test_runner.h"

using namespace std;

struct User {
  // User(int f, int s) : user_id(f), progress(s) {}
  int user_id;
  int progress;
};

using myIt = set<User>::iterator;

// auto id_ordering = [](User A, User B) { return A.user_id < B.user_id; };
// bool id_ordering (User a, User b) { return a.user_id < b.user_id; }
bool p_ordering (User a, User b) { return a.progress < b.progress; };

class ReadingManager {
public:
  ReadingManager()
      : access(MAX_USER_COUNT_ + 1),
        users() {}

/* READ user page — сохранить факт того, что пользователь под номером user дочитал книгу до страницы page.
Если ранее такой пользователь не встречался, необходимо его добавить. 
Гарантируется, что в рамках одного пользователя номера страниц в соответствующих ему событиях возрастают. */
  void Read(int user_id, int page_count) {
    if (AddUser(user_id, page_count)) return;
    UpdateUser(user_id, page_count);
  }

/* CHEER user — сообщить пользователю user, какая доля существующих пользователей (не считая его самого)
прочитала меньшую часть книги, чем он. Если этот пользователь на данный момент единственный, 
доля считается равной 1. Если для данного пользователя пока не было ни одного события READ, 
доля считается равной 0, а сам пользователь не учитывается при вычислении долей 
для других пользователей до тех пор, пока для него не случится событие READ. */
  double Cheer(int user_id) const {
    const User& user = *access[user_id];
    if (user.progress == 0) return 0;
    if (users.size() ==1 ) return 1;
    User zero{0, 0};
    auto l_position = users.upper_bound(zero);
    auto h_position = users.lower_bound(user);
    double diff = distance(l_position, h_position) / distance(l_position, users.end());
    // По умолчанию деление целочисленное, поэтому
    // нужно привести числитель к типу double.
    // Простой способ сделать это — умножить его на 1.0.
    return diff;
  }

private:
  // Статическое поле не принадлежит какому-то конкретному
  // объекту класса. По сути это глобальная переменная,
  // в данном случае константная.
  // Будь она публичной, к ней можно было бы обратиться снаружи
  // следующим образом: ReadingManager::MAX_USER_COUNT.
  static const int MAX_USER_COUNT_ = 100'000;

  vector<int> user_page_counts_;
  vector<int> sorted_users_;   // отсортированы по убыванию количества страниц
  vector<int> user_positions_; // позиции в векторе sorted_users_

// TODO: not working
  set<User, decltype(p_ordering)*> users; // список пользователей, упорядоченный по прогрессу
  vector<myIt> access;  // доступ на список выше


  int GetUserCount() const {
    return users.size();
  }

  bool AddUser(int new_id, int p_count = 0) {
    auto add = users.insert({new_id, p_count});
    if (add.second) { access[new_id] = add.first; };
    return add.second;
  }

  void UpdateUser(int user_id, int p_count) {
    auto user = access[user_id];
    users.erase(user);
    AddUser(user_id, p_count);
  }


  void SwapUsers(int lhs_position, int rhs_position) {
    const int lhs_id = sorted_users_[lhs_position];
    const int rhs_id = sorted_users_[rhs_position];
    swap(sorted_users_[lhs_position], sorted_users_[rhs_position]);
    swap(user_positions_[lhs_id], user_positions_[rhs_id]);
  }
};

void TestReadingManager() {
  // Coursera Test #1
  ReadingManager rm;
  ASSERT_EQUAL(rm.Cheer(5), 0);
  rm.Read(1, 10);
  ASSERT_EQUAL(rm.Cheer(1), 1);
  rm.Read(2, 5);
  rm.Read(3, 7);
  ASSERT_EQUAL(rm.Cheer(2), 0);
  ASSERT_EQUAL(rm.Cheer(3), 0.5);
  rm.Read(3, 10);
  ASSERT_EQUAL(rm.Cheer(3), 0.5);
  rm.Read(3, 11);
  ASSERT_EQUAL(rm.Cheer(3), 1);
  ASSERT_EQUAL(rm.Cheer(1), 0.5);

}


int main() {
  // Для ускорения чтения данных отключается синхронизация
  // cin и cout с stdio,
  // а также выполняется отвязка cin от cout
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  TestRunner tr;
  RUN_TEST(tr, TestReadingManager);

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