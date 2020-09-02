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
    if (reserved_obj.empty()) {
      T* objP = new T;
      auto it = active_obj.insert(objP).first;
      return *it;
    } else {
      auto it = active_obj.insert(reserved_obj.front()).first;
      reserved_obj.pop();
      return *it;
    }
  }
  T* TryAllocate() {
     if (reserved_obj.empty()) {
      return nullptr;
    } else {
      auto it = active_obj.insert(reserved_obj.front()).first;
      reserved_obj.pop();
      return *it;
    }
  }

  void Deallocate(T* object) {
    if (active_obj.find(object) != active_obj.end()) {
      reserved_obj.push(object);
      active_obj.erase(object);
    } else {
      throw invalid_argument("no object");
    }
  }

  ~ObjectPool() {
    for (auto d : active_obj) {
      delete d;
    }
    while (!reserved_obj.empty()) {
      delete reserved_obj.front();
      reserved_obj.pop();
    }
    
  }

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
