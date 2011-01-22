// File: MemoryTest.cpp
// Written by Joshua Green

#include "Memory.h"
#include "Word.h"
#include <iostream>

using namespace std;

int main() {

  Memory MyMemory;

  Word MyAddress, MyWord;
  MyAddress.fromInt(10);
  MyWord.fromInt(10);

  MyMemory.Reserve(MyAddress, MyWord);

  MyAddress.fromInt(1);
  MyWord.fromInt(2);
  MyMemory.Store(MyAddress, MyWord);

  MyAddress.fromInt(10);
  MyWord.fromInt(9);
  MyMemory.Store(MyAddress, MyWord);
  
  MyAddress.fromInt(19);
  MyWord.fromInt(999);
  MyMemory.Store(MyAddress, MyWord);

  MyAddress.fromInt(24);
  MyWord.fromInt(222);
  MyMemory.Store(MyAddress, MyWord);


  for (int i=0;i<25;i++) {
    MyAddress.fromInt(i);
    cout << "Memory[" << MyAddress.toInt() << "] = " << MyMemory.Load(MyAddress).toInt() << " (" << MyMemory.Load(MyAddress).toHex() << ")" << endl;
  }

  return 0;
}
