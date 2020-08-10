#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <unordered_set>

using namespace std;

class ReadingManager {
public:
  ReadingManager()
      : user_is_registered(MAX_USER_COUNT_ + 1, false)
      , user_page_count_(MAX_USER_COUNT_ + 1, -1)
      , user_positions_(MAX_PAGE_COUNT_ + 1) {}

  void Read(int user_id, int page_count) {
    if (!user_is_registered[user_id]) { AddUser(user_id); }
    UpdateUser(user_id, page_count);
  }

  double Cheer(int user_id) const {
    if (!user_is_registered[user_id]) {
      return 0;
    }
    if (user_count == 1) {
      return 1;
    }
    auto page_count = user_page_count_[user_id];
    int losers = 0;
    for (int i = 1; i != page_count; i++) {
      losers += user_positions_[i].size();
    }
    // По умолчанию деление целочисленное, поэтому
    // нужно привести числитель к типу double.
    // Простой способ сделать это — умножить его на 1.0.
    return (losers) * 1.0 / (user_count - 1);
  }


private:
  // Статическое поле не принадлежит какому-то конкретному
  // объекту класса. По сути это глобальная переменная,
  // в данном случае константная.
  // Будь она публичной, к ней можно было бы обратиться снаружи
  // следующим образом: ReadingManager::MAX_USER_COUNT.
  // TODO: поправь в grader
  static const int MAX_USER_COUNT_ = 100'000;
  static const int MAX_PAGE_COUNT_ = 1000;

  int user_count = 0;

  vector<int> user_page_count_; // size()==MAX_USER_COUNT_
  vector<bool> user_is_registered;
  /* user_positions_.size()==1000 - max page count
   * user_positions_[page_count] => user_id(s) unordered set */
  vector<unordered_set<int>> user_positions_;
  
  void AddUser(int user_id) {
      ++user_count;
      user_is_registered[user_id] = true;
      user_page_count_[user_id] = 0;
  }

  void UpdateUser(int user_id, int page_count) {
    int current_progress = user_page_count_[user_id];
    // если пользователь что-то читал раньше - забываем об этом
    if (current_progress > 0) {
      user_positions_[current_progress].erase(user_id);
    }
    // обновляем прогресс пользователя
    user_page_count_[user_id] = page_count;
    user_positions_[page_count].insert(user_id);
  }
};


int main() {
  // Для ускорения чтения данных отключается синхронизация
  // cin и cout с stdio,
  // а также выполняется отвязка cin от cout
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

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