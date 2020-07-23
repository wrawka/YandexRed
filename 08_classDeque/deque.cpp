#include "D:\\MD\\Lebedev\\projects\\YandexRed\\TestRunner\\test_runner.h"

#include <stdexcept>
#include <vector>

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
        size_t n = backward.size();
        if (index <= n) {
            return backward[n - index];
        }
        return forward[index - n];
    }
    const T& operator[](size_t index) const {
        size_t n = backward.size();
        if (index <= n) {
            return backward[n - index];
        }
        return forward[index - n];
    }
    // метод At(size_t index), генерирующий стандартное исключение out_of_range, если индекс больше или равен количеству элементов в деке;
    T& At(size_t index) {
        if (0 > index || index > size) throw out_of_range("Index out of bounds");
        return this->operator[](index);
    }
    const T& At(size_t index) const;
    T& Front() {
        return backward.front();
    }
    const T& Front() const {
        return backward.back();
    }
    T& Back() {
        return forward.back();
    }
    const T& Back() const {
        return forward.back();
    }
    void PushFront(const T& new_value) {
        backward.push_back(new_value);
        size++;
    }
    void PushBack(const T& new_value) {
        forward.push_back(new_value);
        size++;
    }

};

void TestDeque() {
    Deque<int> d;
    ASSERT(d.Empty());
    d.PushBack(100);    //{100}
    d.PushFront(0);     //{0, 100}
    ASSERT_EQUAL(d.Size(), 2u);
    ASSERT_EQUAL(d[0], 0u);
    d.PushBack(101);    //{0, 100, 101}
    d.PushFront(-1);    //{-1, 0, 100, 101}
    ASSERT_EQUAL(d.Front(), -1);
    ASSERT_EQUAL(d.Back(), 101u);
    ASSERT_EQUAL(d.At(2), 100u);
    try
    {
        cout << d.At(13);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    

}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestDeque);
    return 0;
}