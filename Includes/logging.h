#pragma once

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