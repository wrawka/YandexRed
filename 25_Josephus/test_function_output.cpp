// #include "test_runner.h"

#include <cstdint>
#include <iterator>
#include <numeric>
#include <vector>
#include <iostream>

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
	RandomIt pr_r_b = first;
	RandomIt pr_r_e = last;
  vector<typename RandomIt::value_type> pool(first, last);
  size_t cur_pos = 0;
  while (!pool.empty()) {
    cout << "Pool: \n";
		PrintRange(pool.begin(), pool.end());
    cout << "Range: \n";
		PrintRange(pr_r_b, pr_r_e);
    *(first++) = pool[cur_pos];
    cout << "Erasing " << *(pool.begin() + cur_pos) << "...\n";
    pool.erase(pool.begin() + cur_pos);
    if (pool.empty()) {
      break;
    }
    cur_pos = (cur_pos + step_size - 1) % pool.size(); // што ето
		cout << "cur_pos=" << cur_pos << "\n";
		cout << "====\n";
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