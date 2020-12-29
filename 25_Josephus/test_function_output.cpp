// #include "test_runner.h"

#include <cstdint>
#include <iterator>
#include <numeric>
#include <vector>
#include <iostream>
#include <array>

using namespace std;

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
  uint32_t size = last - first;
  array<typename RandomIt::value_type, size> pool;
  while(size--) {
    
  }
}

vector<int> MakeTestVector() {
  vector<int> numbers(10);
  iota(begin(numbers), end(numbers), 0);
  return numbers;
}

int main() {
	vector<int> test_vector = MakeTestVector();
	MakeJosephusPermutation(test_vector.begin(), test_vector.end(), 3);
}