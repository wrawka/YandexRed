#include "test_runner.h"
// #include "logging.h"

#include <vector>

using namespace std;
// Максимальное количество токенов — 10^6
// Время выполнения одного вызова функции ограничено 1 секундой

/*
auto isEnd {
  [](auto t){return true;}
};*/

// Объявляем Sentence<Token> для произвольного типа Token
// синонимом vector<Token>.
// Благодаря этому в качестве возвращаемого значения
// функции можно указать не малопонятный вектор векторов,
// а вектор предложений — vector<Sentence<Token>>.
template <typename Token>
using Sentence = vector<Token>;

// Класс Token имеет метод bool IsEndSentencePunctuation() const
template <typename Token>
vector<Sentence<Token>> SplitIntoSentences(vector<Token> tokens) {
  // Напишите реализацию функции, не копируя объекты типа Token
  vector<Sentence<Token>> result;
  Sentence<Token> new_sentence;
  for (auto it = tokens.begin(); it != tokens.end(); it++) {
    new_sentence.push_back(move(*it));
    if  ( it -> IsEndSentencePunctuation() && 
        ( next(it) == tokens.end() || !(next(it) -> IsEndSentencePunctuation()) )
        )
    {
      // PrintRange(new_sentence.begin(), new_sentence.end());
      result.push_back(move(new_sentence));
    }
  }
  if (!new_sentence.empty()) {
    result.push_back(move(new_sentence));
  }
  return result;
}

struct TestToken {
  string data;
  bool is_end_sentence_punctuation = false;

  bool IsEndSentencePunctuation() const {
    return is_end_sentence_punctuation;
  }
  bool operator==(const TestToken& other) const {
    return data == other.data && is_end_sentence_punctuation == other.is_end_sentence_punctuation;
  }
};

// некопируемая версия Token
struct NCToken {
  string data;
  bool is_end_sentence_punctuation = false;

  NCToken(const NCToken&) = delete;
  NCToken& operator=(const NCToken&) = delete;

  NCToken(NCToken&&) = default;
  NCToken& operator=(NCToken&&) = default;

  bool IsEndSentencePunctuation() const {
    return is_end_sentence_punctuation;
  }
  bool operator==(const NCToken& other) const {
    return data == other.data && is_end_sentence_punctuation == other.is_end_sentence_punctuation;
  }
};

ostream& operator<<(ostream& stream, const TestToken& token) {
  return stream << token.data;
}

ostream& operator<<(ostream& stream, const NCToken& token) {
  return stream << token.data;
}

// Тест содержит копирования объектов класса TestToken.
// Для проверки отсутствия копирований в функции SplitIntoSentences
// необходимо написать отдельный тест.
void TestSplitting() {
  ASSERT_EQUAL(
    SplitIntoSentences(vector<TestToken>({{"Split"}, {"into"}, {"sentences"}, {"!"}})),
    vector<Sentence<TestToken>>({
        {{"Split"}, {"into"}, {"sentences"}, {"!"}}
    })
  );

  ASSERT_EQUAL(
    SplitIntoSentences(vector<TestToken>({{"Split"}, {"into"}, {"sentences"}, {"!", true}})),
    vector<Sentence<TestToken>>({
        {{"Split"}, {"into"}, {"sentences"}, {"!", true}}
    })
  );

  ASSERT_EQUAL(
    SplitIntoSentences(vector<TestToken>({{"Split"}, {"into"}, {"sentences"}, {"!", true}, {"!", true}, {"Without"}, {"copies"}, {".", true}})),
    vector<Sentence<TestToken>>({
        {{"Split"}, {"into"}, {"sentences"}, {"!", true}, {"!", true}},
        {{"Without"}, {"copies"}, {".", true}},
    })
  );
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestSplitting);
  return 0;
}
