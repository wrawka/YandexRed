#include "test_runner.h"
#include <string>
#include <vector>
#include <string_view>

using namespace std;

class Translator {
public:
  Translator() = default;
  void Add(string_view source, string_view target) {
    auto word_1 = lang_1.insert(static_cast<string>(source));
    auto word_2 = lang_2.insert(static_cast<string>(target));
    dictionary_f[*(word_1.first)] = *(word_2.first);
    dictionary_b[*(word_2.first)] = *(word_1.first);
  }
  string_view TranslateForward(string_view source) const {
    if (dictionary_f.find(source) == dictionary_f.end()) {
      return "";
    } else {
      return dictionary_f.at(source);
    }
  }
  string_view TranslateBackward(string_view target) const {
    if (dictionary_b.find(target) == dictionary_b.end()) {
      return "";
    } else {
      return dictionary_b.at(target);
    }
  }

private:
  map<string_view, string_view> dictionary_f;
  map<string_view, string_view> dictionary_b;
  set<string> lang_1;
  set<string> lang_2;
};

void TestSimple() {
  Translator translator;
  translator.Add(string("okno"), string("window"));
  translator.Add(string("stol"), string("table"));

  ASSERT_EQUAL(translator.TranslateForward("okno"), "window");
  ASSERT_EQUAL(translator.TranslateBackward("table"), "stol");
  ASSERT_EQUAL(translator.TranslateBackward("stol"), "");
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestSimple);
  return 0;
}
