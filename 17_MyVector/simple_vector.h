#pragma once

#include <cstdlib>
#include <iostream>

using namespace std;

// Реализуйте шаблон SimpleVector
template <typename T>
class SimpleVector {
public:
  SimpleVector() {
    data = nullptr;
    end_ = nullptr;
    v_size = 0;
    v_cap = 0;
  }
  explicit SimpleVector(size_t size) {
    data = new T[size];
    end_ = data + size;
    v_size = size;
    v_cap = size;
    cout << "data: " << data << ", end_: " << end_ << endl;
  }
  ~SimpleVector() {
    delete[] data;
  }

  T& operator[](size_t index) {
    return data[index];
  }

  T* begin()  { return data; }
  T* end()    { return end_; }

  size_t Size() const     { return v_size; }
  size_t Capacity() const { return v_cap; }

  void PushBack(const T& value) {
    if (data == nullptr) {
      data = new T[1];
      end_ = data + 1;
      v_size = 1;
      v_cap = 1;
    } else if (v_cap == v_size) {
      *end_ = value;
      data = new T[v_size * 2];
      end_++;
      v_size++;
      v_cap = v_cap * 2;
    } else {
      *end_ = value;
      end_++;
      v_size++; 
    }  
  }

private:
  // Добавьте поля для хранения данных вектора
  T* data;
  T* end_;
  size_t v_size;
  size_t v_cap;
};
