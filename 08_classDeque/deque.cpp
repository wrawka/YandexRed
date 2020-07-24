#include "D:\\MD\\Lebedev\\projects\\YandexRed\\TestRunner\\test_runner.h"
#include "deque.h"

using namespace std;

void TestDeque() {
    Deque<int> d;
    ASSERT(d.Empty());
    d.PushBack(100);    //{100}
    ASSERT_EQUAL(d.Front(), 100);
    d.PushFront(0);     //{0, 100}
    d.PushBack(101);    //{0, 100, 101}
    ASSERT_EQUAL(d.Size(), 3u);
    ASSERT_EQUAL(d[0], 0u);
    ASSERT_EQUAL(d[1], 100u);
    ASSERT_EQUAL(d[2], 101u);
    d.PushFront(-1);    //{-1, 0, 100, 101}
    ASSERT_EQUAL(d.Front(), -1);
    d.PushBack(300);    //{-1, 0, 100, 101, 300}
    ASSERT_EQUAL(d.Back(), 300u);
    d.PushFront(-10);   //{-10, -1, 0, 100, 101, 300}
    d.Dubug(cout);
    ASSERT_EQUAL(d.Front(), -10);
    ASSERT_EQUAL(d.Back(), 300u);
    ASSERT_EQUAL(d.At(0), -10);
    ASSERT_EQUAL(d.At(1), -1);
    ASSERT_EQUAL(d.At(2), 0u);
    ASSERT_EQUAL(d.At(3), 100u);
    d[2] = -2;
    ASSERT_EQUAL(d.At(2), -2);
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