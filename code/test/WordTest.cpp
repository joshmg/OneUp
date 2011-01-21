#include <iostream>
#include "../Word.cpp"
using namespace std;

int main() {
  // test from int and print
  Word w, x;
  for (int i=-10; i<10; i++) {
    w.fromInt(i);
    cout << w.toInt() << " = ";

    // test hex
    if (x.fromHex(w.toHex())) {
      cout << x.toInt() << endl;
      w = x.Not();
      cout << "!(" << w.toInt2Complement() << ")\n";
      cout << x.toHex() << endl;
      x++;
      ++x;
      cout << "+ 2 = " << x.toStr() << endl;
      cout << "--------------------\n";
    }
  }
}
