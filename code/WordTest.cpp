// File: WordTest.cpp
// Written by Joshua Green

#include "Word.h"
#include <iostream>
using namespace std;


int main() {
  Word TestWord;

  for (int i=-10;i<101;i++) {
    TestWord.fromInt(i);

    TestWord.print();
    cout << endl;
  }

  return 0;
}
