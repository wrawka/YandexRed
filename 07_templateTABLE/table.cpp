#include "D:\\MD\\Lebedev\\projects\\YandexRed\\TestRunner\\test_runner.h"

using namespace std;

// Реализуйте здесь шаблонный класс Table
template <typename T> 
class Table {
public:
  Table(size_t rows, size_t columns) {
    Resize(rows, columns);
  }
  void Resize(size_t new_rows, size_t new_columns) {
    n_o_rows = new_rows;
    n_o_columns = new_columns;
    table.resize(new_rows);
    for (auto row : table) {
      row.resize(new_columns);
    }
  }
  vector<T>& operator[](int index) {
    return table[index];
  }
  vector<T>& operator[](int index) const {
    return table[index];
  }
  pair<size_t, size_t> Size() {
    return {n_o_rows, n_o_columns};
  }
private:
  size_t n_o_rows;
  size_t n_o_columns;
  vector< vector<T> > table;
};


void TestTable() {
  Table<int> t(1, 1);
  cerr << "Table 1x1 created\n";
  ASSERT_EQUAL(t.Size().first, 1u);
  ASSERT_EQUAL(t.Size().second, 1u);
  cerr << "Size checked\n";
  t[0][0] = 42;
  cerr << "Index accessed\n";
  ASSERT_EQUAL(t[0][0], 42);
  t.Resize(3, 4);
  ASSERT_EQUAL(t.Size().first, 3u);
  ASSERT_EQUAL(t.Size().second, 4u);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestTable);
  return 0;
}
