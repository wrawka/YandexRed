#include "test_runner.h"
#include "profile.h"

#include <map>
#include <string>
using namespace std;

struct Stats {
  map<string, int> word_frequences;

  void operator += (const Stats& other) {
    for (auto [word, word_cnt] : other.word_frequences) {
      word_frequences[word] += word_cnt;
    }
  }
};

Stats ExploreLine(const set<string>& key_words, const string& line) {
  string_view line_view(line);
  line_view.remove_prefix(
    min( line_view.find_first_not_of(" "), line_view.size() )
  ); // drop leading whitespaces if any
  const size_t line_end = line_view.npos;
  size_t pos = 0;
  size_t space = line_view.find(' ', pos); // looking for the next whitespace
  string word;
  word = ( space == line_end 
        ? line_view.substr(pos) 
        : line_view.substr(pos, space - pos));
  Stats result;
  if (key_words.contains(word)) {
    result += (Stats){.word_frequences = {{word, 1}}};
  }

}

Stats ExploreKeyWordsSingleThread(const set<string>& key_words, istream& input) {
  Stats result;
  for (string line; getline(input, line); ) {
    result += ExploreLine(key_words, line);
  }
  return result;
}

Stats ExploreKeyWords(const set<string>& key_words, istream& input) {
  // Реализуйте эту функцию
}

void TestBasic() {
  const set<string> key_words = {"yangle", "rocks", "sucks", "all"};

  stringstream ss;
  ss << "this new yangle service really rocks\n";
  ss << "It sucks when yangle isn't available\n";
  ss << "10 reasons why yangle is the best IT company\n";
  ss << "yangle rocks others suck\n";
  ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

  const auto stats = ExploreKeyWords(key_words, ss);
  const map<string, int> expected = {
    {"yangle", 6},
    {"rocks", 2},
    {"sucks", 1}
  };
  ASSERT_EQUAL(stats.word_frequences, expected);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestBasic);
}
