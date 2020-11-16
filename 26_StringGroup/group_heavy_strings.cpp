#include "test_runner.h"

#include <algorithm>
#include <string>
#include <vector>
#include <set>
#include <map>

using namespace std;

// Объявляем Group<String> для произвольного типа String
// синонимом vector<String>.
// Благодаря этому в качестве возвращаемого значения
// функции можно указать не малопонятный вектор векторов,
// а вектор групп — vector<Group<String>>.
template <typename String>
using Group = vector<String>;

// Ещё один шаблонный синоним типа
// позволяет вместо громоздкого typename String::value_type
// использовать Char<String>
template <typename String>
using Char = typename String::value_type;

template <typename String>
set<Char<String>> MakeUnique(const String& input) {
  set<Char<String>> out{input.begin(), input.end()};
  return move(out);
}

template <typename RandomIt>
void PrintRange(RandomIt first, RandomIt last) {
  cout << "{ ";
  for (auto it = first; it != last; it++) {
    cout << *it << " ";
  }
  cout << "}\n";
}

template <typename String>
vector<Group<String>> GroupHeavyStrings(vector<String> strings) {
  // Напишите реализацию функции,
  // использовав не более 1 копирования каждого символа
  vector<Group<String>> results;
  map< set<Char<String>>, Group<String> > collection;
  for (auto& str : strings) {
    collection[MakeUnique(str)].push_back(move(str));
  }
  /*
  for (auto& out : collection) {
    cout << "Key: " << out.first << endl;
    PrintRange(out.second.begin(), out.second.end());
  }*/
  for (auto& out : collection) {
    results.push_back(move(out.second));
  }
  /*
  cout << "Results:\n";
  for (auto & out : results) {
    PrintRange(out.begin(), out.end());
  }*/
  return results;
}


void TestGroupingABC() {
  vector<string> strings = {"caab", "abc", "cccc", "bacc", "c"};
  auto groups = GroupHeavyStrings(strings);
  ASSERT_EQUAL(groups.size(), 2);
  sort(begin(groups), end(groups));  // Порядок групп не имеет значения
  ASSERT_EQUAL(groups[0], vector<string>({"caab", "abc", "bacc"}));
  ASSERT_EQUAL(groups[1], vector<string>({"cccc", "c"}));
}

void TestGroupingReal() {
  vector<string> strings = {"law", "port", "top", "laptop", "pot", "paloalto", "wall", "awl"};
  auto groups = GroupHeavyStrings(strings);
  ASSERT_EQUAL(groups.size(), 4);
  sort(begin(groups), end(groups));  // Порядок групп не имеет значения
  ASSERT_EQUAL(groups[0], vector<string>({"laptop", "paloalto"}));
  ASSERT_EQUAL(groups[1], vector<string>({"law", "wall", "awl"}));
  ASSERT_EQUAL(groups[2], vector<string>({"port"}));
  ASSERT_EQUAL(groups[3], vector<string>({"top", "pot"}));
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestGroupingABC);
  RUN_TEST(tr, TestGroupingReal);
  return 0;
}
