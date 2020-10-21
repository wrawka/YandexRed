#include "test_runner.h"
#include "profile.h"

#include <string>
#include <list>
#include <random>

using namespace std;
using list_it = list<char>::iterator;

class Editor {
public:
  // Реализуйте конструктор по умолчанию и объявленные методы
  Editor() = default;
  void Left();
  void Right();
  void Insert(char token);
  void Cut(size_t tokens = 1);
  void Copy(size_t tokens = 1);
  void Paste();
  string GetText() const;

  void FlushBuffer();
  // string text;
  list<char> text;
  // string buffer;
  list<char> buffer;
  list_it coursor_position = text.end();
private:
};

void Editor::Left() {
  if (coursor_position != text.begin()) {
    coursor_position--;
  }
}

void Editor::Right() {
  if (coursor_position != text.end()) {
    coursor_position++;
  }
}

void Editor::Insert(char token) {
  text.insert(coursor_position, token);
}

void Editor::Cut(size_t tokens) {
  FlushBuffer();
  auto cut_end = coursor_position;
  while (cut_end != text.end() && tokens--) {
    advance(cut_end, 1);
  }
  buffer.splice(buffer.end(), text, coursor_position--, cut_end);
  Right();
}

void Editor::Copy(size_t tokens) {
  FlushBuffer();
  auto cut_end = coursor_position;
  while (cut_end != text.end() && tokens--) {
    advance(cut_end, 1);
  }
  buffer = list<char>(coursor_position, cut_end);
}

void Editor::Paste() {
  text.insert(coursor_position, buffer.begin(), buffer.end());
}

void Editor::FlushBuffer() {
  buffer.clear();
}

string Editor::GetText() const {
  return string(text.begin(), text.end());
}

void TypeText(Editor& editor, const string& text) {
  for(char c : text) {
    editor.Insert(c);
  }
}

void TestTypeText() {
  Editor editor;
  TypeText(editor, "The text");
  ASSERT_EQUAL(editor.GetText(), "The text");
  for (auto i = 0; i < editor.text.size(); i++) {
    editor.Left();
  }
  editor.Insert('!');
  ASSERT_EQUAL(editor.GetText(), "!The text");
  editor.coursor_position = editor.text.begin();
  editor.Insert('?');
  ASSERT_EQUAL(editor.GetText(), "?!The text");
  editor.coursor_position = editor.text.end();
  editor.Insert('?');
  ASSERT_EQUAL(editor.GetText(), "?!The text?");
}

void TestLeftRight() {
  Editor editor;
  TypeText(editor, "The text");
  int count = 0;
  while (*editor.coursor_position != ' ')  {
    editor.Left();
    count++;
  }
  ASSERT_EQUAL(count, (int)5);
  for (int i = 0; i < 50; i++) {
    editor.Left();
  }
  for (int i = 0; i < 50; i++) {
    editor.Right();
  }
}

void TestCutCopyPaste() {
  Editor editor;
  TypeText(editor, "AssTits");
  for (int i = 0; i < 4; i++) {
    editor.Left();
  }
  editor.Cut(4);
  for (auto out : editor.buffer) {
    cout << out;
  }
  cout << endl;
  ASSERT_EQUAL(editor.GetText(), "Ass");
  // editor.Left();
  editor.Paste();
  // editor.Insert('-');
  ASSERT_EQUAL(editor.GetText(), "AssTits");
  for (auto out : editor.text) {
    cout << out;
  }
  cout << endl;
}

void TestEditing() {
  {
    Editor editor;

    const size_t text_len = 12;
    const size_t first_part_len = 7;
    TypeText(editor, "hello, world");
    for(size_t i = 0; i < text_len; ++i) {
      editor.Left();
    }
    editor.Cut(first_part_len);
    for(size_t i = 0; i < text_len - first_part_len; ++i) {
      editor.Right();
    }
    TypeText(editor, ", ");
    editor.Paste();
    editor.Left();
    editor.Left();
    editor.Cut(3);
    
    ASSERT_EQUAL(editor.GetText(), "world, hello");
  }
  {
    Editor editor;
    
    TypeText(editor, "misprnit");
    editor.Left();
    editor.Left();
    editor.Left();
    editor.Cut(1);
    editor.Right();
    editor.Paste();
    
    ASSERT_EQUAL(editor.GetText(), "misprint");
  }
}

void TestReverse() {
  Editor editor;

  const string text = "esreveR";
  for(char c : text) {
    editor.Insert(c);
    editor.Left();
  }
  
  ASSERT_EQUAL(editor.GetText(), "Reverse");
}

void TestNoText() {
  Editor editor;
  ASSERT_EQUAL(editor.GetText(), "");
  
  editor.Left();
  editor.Left();
  editor.Right();
  editor.Right();
  editor.Copy(0);
  editor.Cut(0);
  editor.Paste();
  
  ASSERT_EQUAL(editor.GetText(), "");
}

void TestEmptyBuffer() {
  Editor editor;

  editor.Paste();
  TypeText(editor, "example");
  editor.Left();
  editor.Left();
  editor.Paste();
  editor.Right();
  editor.Paste();
  editor.Copy(0);
  editor.Paste();
  editor.Left();
  editor.Cut(0);
  editor.Paste();
  
  ASSERT_EQUAL(editor.GetText(), "example");
}

void TestMillion() {
  const int i10e6 = 1'000'000;
  enum Commands {
    Left,
    Right,
    Insert,
    Cut,
    Copy,
    Paste
  };

  default_random_engine rnd(20191019);
  uniform_int_distribution<size_t> gen_command( 0, 5 );

  Editor editor;
  vector<int> comms_done(6);

  for (int i = 0; i < i10e6; i++) {
    auto command = static_cast<Commands>(gen_command(rnd));
    switch (command)
    {
    case Left:
      editor.Left();
      comms_done[0]++;
      break;
    
    case Right:
      editor.Right();
      comms_done[1]++;
      break;
    
    case Insert:
      editor.Insert('0');
      comms_done[2]++;
      break;
    
    case Cut:
      editor.Cut(i);
      comms_done[3]++;
      break;
    
    case Copy:
      editor.Copy(i);
      comms_done[4]++;
      break;
    
    case Paste:
      editor.Paste();
      comms_done[5]++;
      break;
    
    default:
      break;
    }
  }
  for (auto i = 0; i < 5; i++) {
    std::cout << "Command " << i << " was done " << comms_done.at(i) << " times\n";
  }
  std::cout << "Итоговый размер строки: " << editor.GetText().size() << endl;
}

int main() {
  TestRunner tr;
  // RUN_TEST(tr, TestTypeText);
  // RUN_TEST(tr, TestLeftRight);
  // RUN_TEST(tr, TestCutCopyPaste);
  RUN_TEST(tr, TestEditing);
  RUN_TEST(tr, TestReverse);
  RUN_TEST(tr, TestNoText);
  RUN_TEST(tr, TestEmptyBuffer);
  // { LOG_DURATION("TestMillion duration");
  //   RUN_TEST(tr, TestMillion);
  // }
  return 0;
}