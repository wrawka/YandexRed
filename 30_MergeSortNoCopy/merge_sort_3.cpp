// #include "logging.h"
#include "test_runner.h"

#include <algorithm>
#include <memory>
#include <vector>

using namespace std;

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
  // Напишите реализацию функции,
  // не копируя сортируемые элементы

  size_t range = range_end - range_begin;

  // Если диапазон содержит меньше 2 элементов, выйти из функции
	if (range < 2) {
		return;
	}

  // Создать вектор, содержащий все элементы текущего диапазона
	vector<typename RandomIt::value_type> v
    (
    make_move_iterator(range_begin),
    make_move_iterator(range_end)
    );
    
  // Разбить вектор на три равные части
  range /= 3;
 
  auto one3rd = begin(v) + range;
	auto two3rd = begin(v) + (range * 2);

  // Вызвать функцию MergeSort от каждой части вектора
  MergeSort(begin(v), one3rd);
	MergeSort(one3rd, two3rd);
	MergeSort(two3rd, end(v));

  // Слить первые две трети вектора с помощью алгоритма merge,
  // сохранив результат во временный вектор с помощью back_inserter
	vector<typename RandomIt::value_type> v_temp;
  // cout << "Merging...\n";
	merge(
    make_move_iterator(begin(v)), make_move_iterator(one3rd),
    make_move_iterator(one3rd), make_move_iterator(two3rd), 
    back_inserter(v_temp));

  // PrintRange(begin(v_temp), end(v_temp));

  // Слить временный вектор из предыдущего пункта с последней третью вектора из п. 2,
  // записав полученный отсортированный диапазон вместо исходного.
  merge(
    make_move_iterator(begin(v_temp)), make_move_iterator(end(v_temp)),
    make_move_iterator(two3rd), make_move_iterator(end(v)),
    range_begin);
}
/*
class StrNonCopyable : public string {
public:
  using string::string;
  StrNonCopyable(string&& other) : string(move(other)) {}
  StrNonCopyable(const StrNonCopyable&) = delete;
  StrNonCopyable(StrNonCopyable&&) = default;
  StrNonCopyable& operator=(const StrNonCopyable&) = delete;
  StrNonCopyable& operator=(StrNonCopyable&&) = default;
};*/

void TestIntVector() {
  vector<int> numbers = {6, 1, 3, 9, 1, 9, 8, 12, 1};
  MergeSort(begin(numbers), end(numbers));
  ASSERT(is_sorted(begin(numbers), end(numbers)));
}
/*
void TestNCStrVector() {
  vector<StrNonCopyable> numbers = {"6", "1", "3", "9", "1", "9", "8", "12", "1"};
  MergeSort(begin(numbers), end(numbers));
  ASSERT(is_sorted(begin(numbers), end(numbers)));
}*/

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestIntVector);
  // RUN_TEST(tr, TestNCStrVector);
  return 0;
}
