// #pragma once

#include <stdexcept>
#include <array>
using namespace std;

template <typename T, size_t N>
class StackVector {
public:
  explicit StackVector(size_t a_size = 0) {
    if (a_size > N) { 
      throw invalid_argument("Expected size over capacity");
    }
    sv_size = a_size;
    sv_cap = N;
  }

  T& operator[](size_t index) { return sv_data.at(index); }
  const T& operator[](size_t index) const { return sv_data.at(index); }

  typename array<T, N>::iterator begin()        { return sv_data.begin(); }
  typename array<T, N>::iterator end()          { return sv_data.begin() + sv_size; }
  const typename array<T, N>::const_iterator begin() const  { return sv_data.begin(); }
  const typename array<T, N>::const_iterator end() const    { return sv_data.begin() + sv_size; }

  size_t Size() const { return sv_size; }
  size_t Capacity() const { return sv_cap; }

  void PushBack(const T& value) {
    if (sv_size == sv_cap) {
      throw overflow_error("");
    }
    sv_data[sv_size++] = value;
  }
  T PopBack() {
    if (sv_size == 0) {
      throw underflow_error("");
    }
    return sv_data[--sv_size];
  }

private:
  size_t sv_size;
  size_t sv_cap;
  array<T, N> sv_data;

};
