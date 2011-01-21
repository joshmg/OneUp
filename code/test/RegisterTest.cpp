#include <iostream>
#include "../Register.cpp"
#include "../Word.cpp"
using namespace std;

int main() {
  // test from int and print
  Word w;
  Register r;
  for (int i=-10; i<10; i++) {
    w.fromInt(i);
    r.Store(w);
    cout << r.GetValue().toStr() << endl;
  }
}
