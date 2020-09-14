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
    v_size = 0;
    v_cap = 0;
  }
  explicit SimpleVector(size_t size) {
    data = new T[size];
    v_size = size;
    v_cap = size;
  }
  ~SimpleVector() {
    delete[] data;
  }

  T& operator[](size_t index) {
    return data[index];
  }

  T* begin()  { return data; }
  T* end()    { return data + v_size; }

  size_t Size() const     { return v_size; }
  size_t Capacity() const { return v_cap; }

  void PushBack(const T& value) {
    if (data == nullptr) {
      // инициализация
      data = new T[1];
      *data = value;
      v_size = 1;
      v_cap = 1;
    } else if (v_cap == v_size) {
      // удвоение ёмкости
      T* new_data = new T[v_size * 2];
      v_cap = v_cap * 2;
      for (auto i = 0; i < v_size; i++) {
        *(new_data + i) = *(data + i);
      }
      delete[] data;
      data = new_data;
      new_data = nullptr;
      *(data + v_size) = value;
      v_size++;
    } else {
      // обычная вставка
      *(data + v_size) = value;
      v_size++; 
    }  
  }

private:
  // Добавьте поля для хранения данных вектора
  T* data;
  size_t v_size;
  size_t v_cap;
};
