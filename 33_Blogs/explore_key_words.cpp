#include "test_runner.h"
#include "profile.h"
#include "logging.h"

#include <map>
#include <string>
#include <future>
#include <functional>
#include <iterator>
using namespace std;

// Paginator
template <typename Iterator>
class IteratorRange {
private :
  Iterator first , last;
	size_t range_size;
public :
  IteratorRange(Iterator f, Iterator l)
                : first(f)
                , last(l) 
								, range_size(size_t(distance(f, l))) {}
  Iterator begin() const  { return first; }
  Iterator end() const    { return last; }
  size_t size() const 		{ return range_size; }
};

template <typename Iterator>
class Paginator {
private:
  vector< IteratorRange<Iterator> > pages;

public:
  Paginator (Iterator begin, Iterator end, size_t page_size) {
    for (Iterator it = begin; it != end; ) {
      Iterator nend = next(it, min(page_size, size_t(distance(it, end))));
      IteratorRange ir(it, nend);
      pages.push_back(ir);
      it = nend;
    }
  }
  auto begin()        { return pages.begin(); }
  auto end()          { return pages.end(); }
  size_t size() const { return pages.size(); }
};

template <typename C>
auto Paginate(C& c, size_t page_size) {
  return Paginator(c.begin(), c.end(), page_size);
}
// end of Paginator

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
    min( line_view.find_first_not_of(' '), line_view.size() )
  ); // drop leading whitespaces if any
  Stats result;
  while(true) {
    size_t space = line_view.find(' '); // looking for the next whitespace
    string word(line_view.substr(0, space));
    if (key_words.find(word) != key_words.end()) {
      result += (Stats){.word_frequences = {{word, 1}}};
    }
    if (space == line_view.npos) {
      break;
    } else {
      line_view.remove_prefix(space + 1);
    }
  }
  PrintMap(result.word_frequences);
  return result;
}

Stats ExploreKeyWordsSingleThread(const set<string>& key_words, istream& input) {
  Stats result;
  for (string line; getline(input, line); ) {
    result += ExploreLine(key_words, line);
  }
  return result;
}

Stats ExploreKeyWords(const set<string>& key_words, istream& input) {
  //TODO: split the input stream, NOT the dictionary
  const size_t CHUNK_SIZE = key_words.size() / 2;
  vector<future<Stats>> futures;
  Stats result;
  for (auto chunk : Paginate(key_words, CHUNK_SIZE)) {
    set<string> c_set{begin(chunk), end(chunk)};
    // PrintRange(c_set.begin(), c_set.end());
    futures.push_back(async(ExploreKeyWordsSingleThread, c_set, ref(input)));
  }
  for (auto& f : futures) {
    result += f.get();
  }
  return result;
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
