// #include "test_runner.h"

#include <cstdint>
#include <iterator>
#include <numeric>
#include <vector>
#include <iostream>
#include <array>
#include <deque>

using namespace std;

static const uint32_t MAX_SIZE = 10e5;

template <typename RandomIt>
void PrintRange(RandomIt first, RandomIt last) {
  cout << "{ ";
  for (auto it = first; it != last; it++) {
    cout << *it << " ";
  }
  cout << "}\n";
}

template <typename RandomIt>
void MakeJosephusPermutation(RandomIt first, RandomIt last, uint32_t step_size) {
  vector<typename RandomIt::value_type> pool
    (make_move_iterator(first), make_move_iterator(last));
  size_t cur_pos = 0;
  while (!pool.empty()) {
    *(first++) = move(pool[cur_pos]);
    pool.erase(pool.begin() + cur_pos);
    if (pool.empty()) {
      break;
    }
    cur_pos = (cur_pos + step_size - 1) % pool.size();
  }
}

vector<int> MakeTestVector() {
  vector<int> numbers(10);
  iota(begin(numbers), end(numbers), 0);
  return numbers;
}

int main() {
  vector<int> test_vector = MakeTestVector();
	PrintRange(test_vector.begin(), test_vector.end());
  MakeJosephusPermutation(test_vector.begin(), test_vector.end(), 3);
  PrintRange(test_vector.begin(), test_vector.end());
}