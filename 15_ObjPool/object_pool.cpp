#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <queue>
#include <stdexcept>
#include <set>
using namespace std;

template <class T>
class ObjectPool {
public:
  T* Allocate() {
    T* result;
    if (!reserved_obj.empty()) {
      result = &*(active_obj.insert(reserved_obj.front()).first);
      reserved_obj.pop();
    } else {
      result = &*(active_obj.insert(new T).first);
    }
    return result;
  }
  T* TryAllocate() {
    if() {

    } else {
      return nullptr;
    }
  }

  void Deallocate(T* object) {
    reserved_obj.push(object);
    active_obj.erase(object);
  }

  ~ObjectPool();

private:
  // Добавьте сюда поля
  set<T*> active_obj;
  queue<T*> reserved_obj;
};

void TestObjectPool() {
  ObjectPool<string> pool;

  auto p1 = pool.Allocate();
  auto p2 = pool.Allocate();
  auto p3 = pool.Allocate();

  *p1 = "first";
  *p2 = "second";
  *p3 = "third";

  pool.Deallocate(p2);
  ASSERT_EQUAL(*pool.Allocate(), "second");

  pool.Deallocate(p3);
  pool.Deallocate(p1);
  ASSERT_EQUAL(*pool.Allocate(), "third");
  ASSERT_EQUAL(*pool.Allocate(), "first");

  pool.Deallocate(p1);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestObjectPool);
  return 0;
}
