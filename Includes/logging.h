#pragma once

#include <iostream>
#include <vector>
#include <map>


using namespace std;

#define PRINT_VALUE(value) { std::cout << #value << ": " << value << std::endl; }

template <typename RandomIt>
void PrintRange(RandomIt first, RandomIt last) {
  cout << "{ ";
  for (auto it = first; it != last; it++) {
    cout << *it << " ";
  }
  cout << "}\n";
}

template <typename K, typename V>
void PrintMap(map<K, V> source) {
  for (auto& [key, value] : source) {
    cout << key << " : " << value << endl;
  }
}

template <typename K, typename V>
void PrintMap(map< K, vector<V> > source) {
  for (auto& [key, value] : source) {
    cout << key << " : ";
    PrintRange(value.begin(), value.end());
  }
}