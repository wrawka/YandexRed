#include "test_runner.h"

#include <algorithm>
#include <string>
#include <vector>
#include <set>

using namespace std;

template <typename RandomIt>
void PrintRange(RandomIt first, RandomIt last) {
  cout << "{ ";
  for (auto it = first; it != last; it++) {
    cout << *it << " ";
  }
  cout << "}\n";
}

template <typename String>
String MakeUnique(String& input) {
  input.erase(unique(input.begin(), input.end()), input.end());
  return input;
}

int main() {
    vector<string> strings = {"caab", "abc", "cccc", "bacc", "c"};
    // auto item = strings.front();
    for (auto& item : strings) {
      MakeUnique(item);
    }
    PrintRange(strings.begin(), strings.end());
    return 0;
}