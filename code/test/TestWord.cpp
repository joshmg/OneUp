#include <iostream>
#include "../Word.cpp"
using namespace std;

int main() {
  // test from int and print
  Word w, x;
  for (int i=-10; i<100; i++) {
    w.fromInt(i);
    x.fromInt(-i);
    w.And(x).print();
    cout << endl;
  }
}
