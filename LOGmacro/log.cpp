#include "D:\\MD\\Lebedev\\projects\\TestRunner\\test_runner.h"
#include <sstream>
#include <string>
using namespace std;

enum State {
  sFULL,
  sFILE,
  sLINE,
  sERROR
};

class Logger {
public:
  explicit Logger(ostream& output_stream) : os(output_stream) {
  }

  void SetLogLine(bool value) { log_line = value; }
  void SetLogFile(bool value) { log_file = value; }
  
  State State();
  ostream& Out() { return os;}
  void Log(const string& message);

private:
  ostream& os;
  bool log_line = false;
  bool log_file = false;
};

State Logger::State()
 {
    if (log_file && log_line) {
      return State::sFULL;
    } else if (log_line) {
      return State::sLINE;
    } else if (log_file) {
      return State::sFILE;
    } 
    return State::sERROR;
  }

void Logger::Log(const string& message) {
  os << message << "\n";
}

#define LOG(logger, message)                  \
  switch (logger.State())                     \
  {                                           \
  case State::sFULL:                                \
    logger.Out() << __FILE__ << ":" << __LINE__ << " "; \
    break;                                    \
  case State::sLINE:                                \
    logger.Out() << "Line " << __LINE__ << " ";\
    break;         \
  case State::sFILE:                                \
    logger.Out() << __FILE__ << " "; \
    break;                   \
  default:                                    \
    break;                                    \
  }                                           \
  logger.Log(message);


void TestLog() {
/* Для написания юнит-тестов в этой задаче нам нужно фиксировать конкретные
 * номера строк в ожидаемом значении (см. переменную expected ниже). Если
 * мы добавляем какой-то код выше функции TestLog, то эти номера строк меняются,
 * и наш тест начинает падать. Это неудобно.
 *
 * Чтобы этого избежать, мы используем специальный макрос #line
 * (http://en.cppreference.com/w/cpp/preprocessor/line), который позволяет
 * переопределить номер строки, а также имя файла. Благодаря ему, номера
 * строк внутри функции TestLog будут фиксированы независимо от того, какой
 * код мы добавляем перед ней*/
#line 1 "logger.cpp"

  ostringstream logs;
  Logger l(logs);
  LOG(l, "hello");

  l.SetLogFile(true);
  LOG(l, "hello");

  l.SetLogLine(true);
  LOG(l, "hello");

  l.SetLogFile(false);
  LOG(l, "hello");

  string expected = "hello\n";
  expected += "logger.cpp hello\n";
  expected += "logger.cpp:10 hello\n";
  expected += "Line 13 hello\n";
  ASSERT_EQUAL(logs.str(), expected);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestLog);
}