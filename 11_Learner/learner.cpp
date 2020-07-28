
#include <algorithm>
#include <string>
#include <vector>
#include <set>

using namespace std;

class Learner {
 private:
  vector<string> dict;
  set<string> dict_ordered;
  bool was_changed;

 public:
  int Learn(const vector<string>& words) {
    int newWords = 0;
    for (const auto& word : words) {
      bool new_word = dict_ordered.insert(word).second;
      if (new_word) { newWords++; was_changed = true; }
    }
    return newWords;
  }

  vector<string> KnownWords() {
    if (was_changed) {
      dict.clear();
      was_changed = false;
      for (auto& word : dict_ordered) {
        dict.push_back(word);
      }
    }
    return dict;
  }
};
