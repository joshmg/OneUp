#include <iostream>
#include "../itos.h"
using namespace std;

int main () {
  cout << itoshex(0x4) << endl;
  cout << itoshex(0xAB) << endl;
  cout << itoshex(0xFED) << endl;
  cout << itoshex(0xF025) << endl;
  cout << itoshex(0x1234) << endl;
  cout << itoshex(0xABCD) << endl;
  cout << itoshex(0xFFFF) << endl;
  cout << itoshex(0x0000) << endl;
  cout << itoshex(0xA0007) << endl;
  cout << itoshex(0x00D00F) << endl;
  cout << itoshex(0x4000004) << endl;
  cout << itoshex(0x12345678) << endl;
  cout << itoshex(0x9ABCDEF0) << endl;
  return 0;
}
