// File: Loader.cpp
// Written by Logan Coulson

#include "../h/Loader.h"
#include "../h/Word.h"
#include "../h/ResultCodes.h"
#include "../h/iMemory.h"
#include "../h/iLoader.h"
#include "../h/ObjParser.h"
#include <iostream>

using namespace Codes;
using namespace std;

RESULT Loader::_GetLoadAddress(Word& produced_address, const Word& segment_length) const {

  while (true) {
    cout << endl << "Requesting address for program length of " << segment_length.ToInt() << " (Ex: x0000) : ";

    string input;
    getline(cin, input);
    cout << endl;

    if (produced_address.FromHexAbbr(input)) break;
    else cout << "Invalid input address syntax." << endl;;
  }
  
  Word page_test;
  page_test.FromInt(produced_address.ToInt() + segment_length.ToInt());

  
  for (int i=WORD_SIZE-1;i>8;i--) {
    if (produced_address[i] != page_test[i]) return BAD_MALLOC;
  }

  return SUCCESS;
}

// establish the Memory pointer of the class
Loader::Loader(iMemory* mem) {
  _memory = mem;
}

RESULT Loader::Load(const char* filename, iWord& PC_address) const {
  ObjParser Parser; // the parser is just the iobjparser object

  Word segment_start_address;
  bool relocatable = false;

  Word Hex1,Hex2; // used to manipulate memory.  memory has the functions i need, so no hex conversion is needed, as everything is a word
  
  ObjectData Data; // the Parser return the Data to this

  RESULT returns; // meant to save the return values, and check if sucesses, or, if not, return itself.

  int min_memory, max_memory, mem_location; // check for out of bounds acess on memory
  
  returns=Parser.Initialize(filename); // initializing the Parser
  if (returns != SUCCESS) {
    return returns;
  }

  Data=Parser.GetNext(); // getting the first header

  if (Data.type != 'H' && Data.type != 'M') { // if it is not a H, problem
                                              // added M for Main designator
    return INVALID_HEADER_ENTRY;
  }

  if (Data.data[1] == RELOCATE_FLAG) {
    RESULT malloc_result = _GetLoadAddress(Hex1, Word(atoi(Data.data[2].c_str())));

    if (malloc_result != SUCCESS) return malloc_result;

    relocatable = true;
  }
  else if  (!(Hex1.FromHex(string("0x") + Data.data[1]))) { // setting up the words to use the reserve command memory has.
    return NOT_HEX;
  }

  // establishing the lower bound on memory
  min_memory=Hex1.ToInt();
  segment_start_address = Hex1;


  if  (!(Hex2.FromHex(string("0x") + Data.data[2]))) {
    return NOT_HEX;
  }
  // establishing the upper bound on memory
  max_memory=min_memory+Hex2.ToInt();

  if (Hex2.ToInt() < (unsigned short)(~0)) returns=_memory->Reserve(Hex1,Hex2);
  else return REQUESTED_MEMORY_TOO_LARGE;

  if (returns!=SUCCESS) {
    return returns;
  }
  Data=Parser.GetNext();

  // this loop goes through the Test records
  while ((Data.type == 'T') || (Data.type == 'W') || (Data.type == 'R')) {
    if (((Data.type == 'W') || (Data.type == 'R')) && !relocatable) return RELOCATE_ENTRY_IN_ABSOLUTE;

    if  (!(Hex1.FromHex(string("0x") + Data.data[0]))) {
      return NOT_HEX;
    }

    // converting the hex memory location to int
    mem_location=Hex1.ToInt();

    if (relocatable) {
      mem_location += segment_start_address.ToInt();
      Hex1.FromInt(mem_location);
    }

    // ensuring that the memory location is in bounds
    if (!(mem_location >= min_memory && mem_location < max_memory)) {
      cout << min_memory << " <= " << mem_location  << " < " << max_memory << endl;
      return OUT_OF_BOUNDS;
    }

    if  (!(Hex2.FromHex(string("0x") + Data.data[1]))) {
      return NOT_HEX;
    }

    // Relocate the last 16 bits:
    if (Data.type == 'W') {

      Word address_offset;
      address_offset.FromHex(string("0x") + Data.data[1]);

      Word relocated_address;
      int new_address_int = segment_start_address.ToInt() + address_offset.ToInt();
      if (new_address_int > Word::MAX_SIZE) return RELOCATION_OVERFLOW; // Relocation Overflow
      relocated_address.FromInt(new_address_int);

      if (new_address_int > max_memory) return RELOCATION_OUTSIDE_BOUNDS;

      for (int i=0;i<16;i++) {
        Hex2.SetBit(i, relocated_address[i]);
      }
    }
    // Relocate the last 9 bits:
    else if (Data.type == 'R') {

      Word address_offset;
      address_offset.FromHex(string("0x") + Data.data[1]);
      for (int i=WORD_SIZE-1;i>8;i--) address_offset.SetBit(i, false); // Only caring about the last 9 bits...

      Word relocated_address;
      int new_address_int = segment_start_address.ToInt() + address_offset.ToInt();
      if (new_address_int > Word::MAX_SIZE) return RELOCATION_OVERFLOW; // Relocation Overflow
      relocated_address.FromInt(new_address_int);

      if (new_address_int > max_memory) return RELOCATION_OUTSIDE_BOUNDS;

      for (int i=0;i<9;i++) {
        Hex2.SetBit(i, relocated_address[i]);
      }
    }

    // if it is in bounds, store it
    returns=_memory->Store(Hex1,Hex2);
    if (returns!=SUCCESS) {
     return returns;
    }

    Data=Parser.GetNext();
  }

  // if we ended the loop, and dectected something other that an E, we have an issue
  if (Data.type!='E') {
    return INVALID_DATA_ENTRY;
  }

  if  (!(Hex1.FromHex(string("0x") + Data.data[0]))) {
    return NOT_HEX;
  }

  // if relocatable, relocate start address (end record)
  if (relocatable) {
    if ((segment_start_address.ToInt() + Hex1.ToInt()) > Word::MAX_SIZE) return RELOCATION_OUTSIDE_BOUNDS;
    Hex1.FromInt(segment_start_address.ToInt() + Hex1.ToInt());
  }

  if (Hex1.ToInt() < min_memory || Hex1.ToInt() >= max_memory) {
    return INVALID_START_PC;
  }

  //this should be copying Hex1 onto the PC_address.  if it is not, that is what it is meant to be doing
  PC_address.Copy(Hex1);

  return SUCCESS;
}
