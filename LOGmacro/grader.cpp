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