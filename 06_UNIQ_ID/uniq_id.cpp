#include <string>
#include <vector>
using namespace std;

// макрос "UNIQ_ID" передаёт __LINE__ как параметр макросу "f"
#define UNIQ_ID f(__LINE__)
// макрос "f" разворачивает __LINE__ непосредственно в номер строки и передаёт его как параметр макросу "g"
#define f(a) g(a)
// макрос "g" склеивает ID с номером строки
#define g(a) ID##a

int main() {
  int UNIQ_ID = 0;
  string UNIQ_ID = "hello";
  vector<string> UNIQ_ID = {"hello", "world"};
  vector<int> UNIQ_ID = {1, 2, 3, 4};
}