#include "test_runner.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <list>
#include <set>
#include <utility>
#include <vector>

using namespace std;

// Контейнер должен обрабатывать 10^6 запросов не более чем за 1 секунду. 
// Гарантируется, что суммарная длина диапазонов, 
// с которыми вызывается вторая версия метода Add, также не превосходит 10^6.

const uint32_t MAX_CAP = 1e6;


template <typename T>
class PriorityCollection {
public:
  struct RankedObject {
    T object;
    int priority = 0;
    RankedObject(T&& o) : object(move(o)) {}
    /*
    RankedObject& operator=(RankedObject&& other) {
      object = move(other.object);
      priority = move(other.priority);
      return *this;
    }*/
    bool operator<(const RankedObject& other) const {
      return priority < other.priority;
    }
  };

  using Id = list<RankedObject>::iterator; /* тип, используемый для идентификаторов */

  // Добавить объект с нулевым приоритетом
  // с помощью перемещения и вернуть его идентификатор
  Id Add(T object) {
    Id out = priority_collection.insert(end(priority_collection),
                                        move(RankedObject(move(object))));
    collection.push_back(out);
    return out;
  }

  // Добавить все элементы диапазона [range_begin, range_end)
  // с помощью перемещения, записав выданные им идентификаторы
  // в диапазон [ids_begin, ...)
  template <typename ObjInputIt, typename IdOutputIt>
  void Add(ObjInputIt range_begin, ObjInputIt range_end, IdOutputIt ids_begin) {
    for (auto it = range_begin; it != range_end; it++) {
      *(ids_begin++) = Add(*it);
    }
  }

  // Определить, принадлежит ли идентификатор какому-либо
  // хранящемуся в контейнере объекту
  bool IsValid(Id id) const {
    return find(begin(collection), end(collection), id) != end(collection);
  }

  // Получить объект по идентификатору
  const T& Get(Id id) const {
    return id->object; 
  }

  // Увеличить приоритет объекта на 1
  void Promote(Id id) {
    (id->priority)++;
    
  }

  // Получить объект с максимальным приоритетом и его приоритет
  pair<const T&, int> GetMax() const {
    return make_pair(priority_collection.front().object, priority_collection.front().priority);
  }

  // Аналогично GetMax, но удаляет элемент из контейнера
  pair<T, int> PopMax() {
    // auto out = GetMax();
    priority_collection.pop_front();
    // collection.erase
    return make_pair(move(priority_collection.front().object), move(priority_collection.front().priority));
  }

private:
  list<RankedObject> priority_collection;
  vector<Id> collection;
};


class StringNonCopyable : public string {
public:
  using string::string;  // Позволяет использовать конструкторы строки
  StringNonCopyable(const StringNonCopyable&) = delete;
  StringNonCopyable(StringNonCopyable&&) = default;
  StringNonCopyable& operator=(const StringNonCopyable&) = delete;
  StringNonCopyable& operator=(StringNonCopyable&&) = default;
};


void TestIsValid() {
  PriorityCollection<StringNonCopyable> strings;
  const auto white_id = strings.Add("white");
  ASSERT(strings.IsValid(white_id));
  ASSERT_EQUAL(strings.Get(white_id), "white");
  // PriorityCollection<StringNonCopyable>::RankedObject o(StringNonCopyable("white"));
  // ASSERT_EQUAL(strings.GetMax(), make_pair(o.priority, o.priority));
  // strings.PopMax();
  // ASSERT(!strings.IsValid(white_id));
}

void TestNoCopy() {
  PriorityCollection<StringNonCopyable> strings;
  const auto white_id = strings.Add("white");
  const auto yellow_id = strings.Add("yellow");
  const auto red_id = strings.Add("red");

  strings.Promote(yellow_id);
  for (int i = 0; i < 2; ++i) {
    strings.Promote(red_id);
  }
  strings.Promote(yellow_id);
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "red");
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "yellow");
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "white");
    ASSERT_EQUAL(item.second, 0);
  }
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestIsValid);
  // RUN_TEST(tr, TestNoCopy);
  return 0;
}
