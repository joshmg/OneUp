#include <iostream>
#include "../Word.cpp"
using namespace std;

int main() {
  // test from int and print
  Word w, x;
  for (int i=-10; i<10; i++) {
    w.FromInt(i);
    cout << w.ToInt() << " = ";

    // test hex
    if (x.FromStr(w.ToStr())) {
      cout << x.ToInt() << endl;
      w = x.Not();
      cout << "!(" << w.ToInt2Complement() << ")\n";
      cout << x.ToHex() << endl;
      x++;
      ++x;
      cout << "+ 2 = " << x.ToStr() << endl;
      cout << "--------------------\n";
    }
  }
}
