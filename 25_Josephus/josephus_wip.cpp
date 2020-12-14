#include "test_runner.h"
// #include "logging.h"
// #include "profile.h"

#include <cstdint>
#include <iterator>
#include <numeric>
#include <vector>
#include <list>
#include <random>

using namespace std;

static const uint32_t MAX_SIZE = 1e5;

// макс. размер диапазона (n) : 10^5
// макс. размер шага      (m) : 10^2

template <typename RandomIt>
void MakeJosephusPermutation(RandomIt first, RandomIt last, uint32_t step_size) {
  list<typename RandomIt::value_type> pool
    (make_move_iterator(first), make_move_iterator(last));
  // size_t cur_pos = 0;
  auto point = pool.begin();
  while (!pool.empty()) {
    // advance(point, cur_pos);
    for (auto i = step_size; i != 0; i--) {
      if (point == pool.end()) { 
        cout << "Making a loop..." << endl;
        point = pool.begin(); 
      }
      point++;
    }
    *(first++) = move(*point);
    point = pool.erase(point);
    if (pool.empty()) {
      break;
    }
    // cur_pos = (cur_pos + step_size - 1) % pool.size();
  }
}

vector<int> MakeTestVector() {
  vector<int> numbers(10);
  iota(begin(numbers), end(numbers), 0);
  return numbers;
}

void TestIntVector() {
  const vector<int> numbers = MakeTestVector();
  {
    vector<int> numbers_copy = numbers;
    MakeJosephusPermutation(begin(numbers_copy), end(numbers_copy), 1);
    ASSERT_EQUAL(numbers_copy, numbers);
  }
  {
    vector<int> numbers_copy = numbers;
    MakeJosephusPermutation(begin(numbers_copy), end(numbers_copy), 3);
    ASSERT_EQUAL(numbers_copy, vector<int>({0, 3, 6, 9, 4, 8, 5, 2, 7, 1}));
  }
}

/*
void TestExecTime() {
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> distrib(1, 100);
  size_t seed = size_t(distrib(gen));
  PRINT_VALUE(seed);
  vector<int> big_vector(MAX_SIZE);
  PRINT_VALUE(big_vector.size());
  cout << "Filling vector..." << endl;
  {
  LOG_DURATION("iota")
  iota(begin(big_vector), end(big_vector), 0);
  }
  {
  LOG_DURATION("rand")
  for (auto& element : big_vector) {
    element = seed;
  }
  PRINT_VALUE(big_vector.at(3));
  }
  cout << "Jo list started..." << endl;
  {
  LOG_DURATION("jo_list")
  MakeJosephusPermutationList(begin(big_vector), end(big_vector), seed);
  }
  cout << "Jo vector started..." << endl;
  {
  LOG_DURATION("jo_list")
  MakeJosephusPermutationVector(begin(big_vector), end(big_vector), seed);
  }
}
*/

// Это специальный тип, который поможет вам убедиться, что ваша реализация
// функции MakeJosephusPermutation не выполняет копирование объектов.
// Сейчас вы, возможно, не понимаете как он устроен, однако мы расскажем,
// почему он устроен именно так, далее в блоке про move-семантику —
// в видео «Некопируемые типы»

struct NoncopyableInt {
  int value;

  NoncopyableInt(const NoncopyableInt&) = delete;
  NoncopyableInt& operator=(const NoncopyableInt&) = delete;

  NoncopyableInt(NoncopyableInt&&) = default;
  NoncopyableInt& operator=(NoncopyableInt&&) = default;
};

bool operator == (const NoncopyableInt& lhs, const NoncopyableInt& rhs) {
  return lhs.value == rhs.value;
}

ostream& operator << (ostream& os, const NoncopyableInt& v) {
  return os << v.value;
}

void TestAvoidsCopying() {
  vector<NoncopyableInt> numbers;
  numbers.push_back({1});
  numbers.push_back({2});
  numbers.push_back({3});
  numbers.push_back({4});
  numbers.push_back({5});

  MakeJosephusPermutation(begin(numbers), end(numbers), 2);

  vector<NoncopyableInt> expected;
  expected.push_back({1});
  expected.push_back({3});
  expected.push_back({5});
  expected.push_back({4});
  expected.push_back({2});

  ASSERT_EQUAL(numbers, expected);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestIntVector);
  RUN_TEST(tr, TestAvoidsCopying);
  // TestExecTime();
  return 0;
}
