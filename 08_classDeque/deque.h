#pragma once

#include <stdexcept>
#include <vector>
#include <iterator>

using namespace std;

template <typename T>
class Deque
{
private:
    vector<T> forward;
    vector<T> backward;
    size_t size;

public:
    // конструктор по умолчанию;
    Deque() {
        forward.clear();
        backward.clear();
        size = 0;
    }

    // константный метод Empty, возвращающий true, если дек не содержит ни одного элемента;
    bool Empty() const {
        return size == 0;
    }

    // константный метод Size, возвращающий количество элементов в деке;
    size_t Size() const {
        return size;
    }

    T& operator[](size_t index) {
        if (index == 0) return this->Front();
        auto n = backward.size();
        if (index < n) {
            auto it = backward.rbegin();
            advance(it, index);
            return *it;
        } else {
            auto it = forward.begin();
            advance(it, index - n);
            return *it;
        }
    }

    const T& operator[](size_t index) const {
        if (index == 0) return this->Front();
        auto n = backward.size();
        if (index < n) {
            auto it = backward.rbegin();
            advance(it, index);
            return *it;
        } else {
            auto it = forward.begin();
            advance(it, index - n);
            return *it;
        }
    }

    // метод At(size_t index), генерирующий стандартное исключение out_of_range, если индекс больше или равен количеству элементов в деке;
    T& At(size_t index) {
        if (0 > index || index > size) throw out_of_range("Index out of bounds");
        return this->operator[](index);
    }
    const T& At(size_t index) const {
        if (0 > index || index > size) throw out_of_range("Index out of bounds");
        return this->operator[](index);
    }
    T& Front() {
        if (backward.empty()) {
            return forward.front();
        }
        else {
            return backward.back();
        }
    }
    const T& Front() const {
        if (backward.empty()) {
            return forward.front();
        }
        else {
            return backward.back();
        }
    }
    T& Back() {
        if (forward.empty()) {
            return backward.front();
        }
        else {
            return forward.back();
        }
    }
    const T& Back() const {
        if (forward.empty()) {
            return backward.front();
        }
        else {
            return forward.back();
        }
    }
    void PushFront(const T& new_value) {
        backward.push_back(new_value);
        size++;
    }
    void PushBack(const T& new_value) {
        forward.push_back(new_value);
        size++;
    }
    ostream& Dubug(ostream& os) {
        os << "Forward vector: ";
        for (auto i : forward) {
            os << i << " ";
        }
        os << endl;
        os << "Backward vector: ";
        for (auto i : backward) {
            os << i << " ";
        }
        return os << endl;
    }
};