#include <iostream>
#include "../Line.h"
using namespace std;

int main() {
  Line line;
  while (cin.good()) {
    string str;
    getline(cin, str);
    line.ReadLine(str);
    if (line.IsComment()) {
      cout << line.ToString();
    } else {
      if (line.HasLabel()) {
        cout << line.Label() << '\t';
      } else {
        cout << '\t';
      }
      cout << line.Instruction();
      for (int i=0; i<line.Size(); i++) {
        cout << line[i];
      }
      if (line.HasLiteral()) {
        cout << "  " << line.Literal();
      }
    }
    cout << endl;
  }

  return 0;
}
