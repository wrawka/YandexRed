#pragma once

using namespace std;

template <typename Iterator>
class IteratorRange {
private :
  Iterator first , last;
  size_t sz;
public :
  IteratorRange(Iterator f, Iterator l)
                : first(f)
                , last(l)
                , sz(l - f) {}
  Iterator begin() const  { return first; }
  Iterator end() const    { return last; }
  size_t size() const     { return sz; }
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
