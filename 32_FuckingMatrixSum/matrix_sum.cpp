#include "test_runner.h"
// #include "profile.h"

#include <vector>
#include <future>
#include <numeric>
using namespace std;

template <typename Iterator>
class IteratorRange {
private :
  Iterator first , last;
	size_t range_size;
public :
  IteratorRange(Iterator f, Iterator l)
                : first(f)
                , last(l) 
								, range_size(l - f) {}
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
      Iterator nend = next(it, min(page_size, size_t(end - it)));
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

template <typename C>
int64_t CalculateMatrixSumSingle(const C& matrix) {
  int64_t result = 0;
  for (const auto& line : matrix) {
    // result += reduce(begin(line), end(line)); // падает в грейдере с ошибкой компиляции
    /*
    Compile error: b"/tmp/tmpflo2d_4_.cpp: In instantiation of 'int64_t CalculateMatrixSumSingle(const C&) 
    [with C = IteratorRange<__gnu_cxx::__normal_iterator<const std::vector<int>*, std::vector<std::vector<int> > > >; int64_t = long int]':\n/tmp/tmpflo2d_4_.cpp:61:70:   
    required from here\n/tmp/tmpflo2d_4_.cpp:52:21: error: 'reduce' was not declared in this scope\n     result += reduce(begin(line), end(line));\n               
    ~~~~~~^~~~~~~~~~~~~~~~~~~~~~~~\ncompilation terminated due to -Wfatal-errors.\n"
    */
    for (auto item : line) { result += item; }; // OK
  }
  return result;
}

int64_t CalculateMatrixSum(const vector<vector<int>>& matrix) {
  int64_t result = 0;
  vector<future<int64_t>> futures;
  for (auto page : Paginate(matrix, 2000)) { // why 2000? fuck you, that's why
    futures.push_back(async([=]{ return CalculateMatrixSumSingle(page); }));
  }
  for (auto& f : futures) {
    result += f.get();
  }
  return result;
}

void TestCalculateMatrixSum() {
  const vector<vector<int>> matrix = {
    {1, 2, 3, 4},
    {5, 6, 7, 8},
    {9, 10, 11, 12},
    {13, 14, 15, 16}
  };
  ASSERT_EQUAL(CalculateMatrixSum(matrix), 136);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestCalculateMatrixSum);
}
