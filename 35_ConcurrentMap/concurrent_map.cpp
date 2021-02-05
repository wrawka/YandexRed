#include "test_runner.h"
#include "profile.h"
#include "logging.h"

#include <algorithm>
#include <future>
#include <map>
#include <vector>
#include <string>
#include <random>
using namespace std;

template <typename K, typename V>
class ConcurrentMap {
private:
  size_t no_buckets;
  vector<mutex> _v_of_m;
  vector<map<K, V>> _data_split;
  
public:
  static_assert(is_integral_v<K>, "ConcurrentMap supports only integer keys");

  struct Access {
    V& ref_to_value;
    lock_guard<mutex> guard;
  };

  explicit ConcurrentMap(size_t bucket_count) 
            : _v_of_m(bucket_count)
            , _data_split(bucket_count)
            , no_buckets(bucket_count) {
    /*cerr << "Entering ctor..." << endl;*/ }

  Access operator[](const K& key) {
    // cerr << "Entering operator[]..." << endl;s
    K index = key < no_buckets ? key : key % no_buckets;
    mutex& bucket_mutex = _v_of_m[index];
    V& data_chunck = _data_split[index][key];
    return {data_chunck, lock_guard(bucket_mutex)};
  }

  map<K, V> BuildOrdinaryMap() {
    // cerr << "Entering BuildOrdinaryMap..." << endl;
    map<K, V> result;
    for (auto i = 0; i < no_buckets; i++) {
      lock_guard(_v_of_m.at(i));
      result.merge(_data_split.at(i));
    }
    return result;
  }
};

void RunConcurrentUpdates(
    ConcurrentMap<int, int>& cm, size_t thread_count, int key_count
) {
  auto kernel = [&cm, key_count](int seed) {
    vector<int> updates(key_count);

    // iota( ForwardIt first, ForwardIt last, T value )
    // Fills the range [first, last) with sequentially increasing values, 
    // starting with value and repetitively evaluating ++value.
    iota(begin(updates), end(updates), -key_count / 2);

    // shuffle( RandomIt first, RandomIt last, URBG&& g )
    // Reorders the elements in the given range [first, last)
    // such that each possible permutation of those elements has equal probability of appearance.
    shuffle(begin(updates), end(updates), default_random_engine(seed));

    for (int i = 0; i < 2; ++i) {
      for (auto key : updates) {
        cm[key].ref_to_value++;
      }
    }
  };

  vector<future<void>> futures;
  for (size_t i = 0; i < thread_count; ++i) {
    futures.push_back(async(kernel, i));
  }
}

void TestConcurrentUpdate() {
  const size_t thread_count = 3;
  const size_t key_count = 50000;

  ConcurrentMap<int, int> cm(thread_count);
  RunConcurrentUpdates(cm, thread_count, key_count);

  const auto result = cm.BuildOrdinaryMap();
  ASSERT_EQUAL(result.size(), key_count);
  for (auto& [k, v] : result) {
    AssertEqual(v, 6, "Key = " + to_string(k));
  }
}

void TestReadAndWrite() {
  ConcurrentMap<size_t, string> cm(5);

  auto updater = [&cm] {
    for (size_t i = 0; i < 50000; ++i) {
      cm[i].ref_to_value += 'a';
    }
  };
  auto reader = [&cm] {
    vector<string> result(50000);
    for (size_t i = 0; i < result.size(); ++i) {
      result[i] = cm[i].ref_to_value;
    }
    return result;
  };

  auto u1 = async(updater);
  auto r1 = async(reader);
  auto u2 = async(updater);
  auto r2 = async(reader);

  u1.get();
  u2.get();

// all_of( InputIt first, InputIt last, UnaryPredicate p )
// Checks if unary predicate p returns true for all elements in the range [first, last).

  for (auto f : {&r1, &r2}) {
    auto result = f->get();
    ASSERT(all_of(result.begin(), result.end(), [](const string& s) {
      return s.empty() || s == "a" || s == "aa";
    }));
  }
}

void TestSpeedup() {
  {
    ConcurrentMap<int, int> single_lock(1);

    LOG_DURATION("Single lock");
    RunConcurrentUpdates(single_lock, 4, 50000);
  }
  {
    ConcurrentMap<int, int> many_locks(100);

    LOG_DURATION("100 locks");
    RunConcurrentUpdates(many_locks, 4, 50000);
  }
}

int main() {
  const size_t thread_count = 2;
  const size_t key_count = 50;

  ConcurrentMap<int, int> cm(thread_count);
  RunConcurrentUpdates(cm, thread_count, key_count);

  for (auto i = 0; i < key_count; i++) {
    cout << i << ": " << cm[i].ref_to_value << endl;
  }

  // TestRunner tr;
  // RUN_TEST(tr, TestConcurrentUpdate);
  // RUN_TEST(tr, TestReadAndWrite);
  // RUN_TEST(tr, TestSpeedup);
}
