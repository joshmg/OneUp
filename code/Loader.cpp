// File: Loader.cpp
// Written by Logal Coulson

#include "Loader.h"
#include "Word.h"
#include "ResultCodes.h"
#include "iMemory.h"
#include "iLoader.h"
#include "iObjParser.h"

using namespace Codes;


// establish the Memory pointer of the class
iLoader::iLoader(iMemory* mem) {
  _memory = mem;
}

RESULT Loader::Load(const char* filename, iWord& PC_address) {
  ObjParser Parser; // the parser is just the iobjparser object

  Word Hex1,Hex2; // used to manipulate memory.  memory has the functions i need, so no hex conversion is needed, as everything is a word
  
  ObjectData Data; // the Parser return the Data to this

  RESULTS returns; // meant to save the return values, and check if sucesses, or, if not, return itself.

  int min_memory, max_memory, mem_location; // check for out of bounds acess on memory
  
  returns=Parser.Initialize(filename); // initializing the Parser
  if (returns!=SUCCESS) {
    return returns;
  }
  
  Data=Parser.Get_Next(); // getting my first header
  
  if (Data.type!='H') { //if it is not a H, problem
    return INVALID_OBJECT_FILE;
  }

  if  (!(Hex1.FromHex(Data.data[1]))) { // setting up the words to use the reserve command memory has.  seemed to me to be an time improver.
    return NOT_HEX;
  }

  // establishing the lower bound on memory
  min_memory=Hex1.ToInt();
  if  (!(Hex2.FromHex(Data.data[2])))
  {
    return NOT_HEX;
  }

  // establishing the upper bound on memory
  max_memory=init_memory+Hex2.ToInt();
  returns=Memory->Reserve(Hex1,Hex2);
  if (returns!=SUCCESS) {
    return returns;
  }
  Data=Parser.Get_Next();

  // this loop goes through the Test records
  while (Data.type=='T') {
    if  (!(Hex1.FromHex(Data.data[0]))) {
      return NOT_HEX;
    }

    // converting the hex memory location to int
    mem_location=Hex1.ToInt();
    // ensureing that the memory location is in bounds
    if (!(mem_location>min_memory && mem_location<max_memory)) {
      return OUT_OF_BOUNDS;
    }
    if  (!(Hex2.FromHex(Data.data[1]))) {
      return NOT_HEX;
    }

    // if it is in bounds, store it
    returns=Memory->Store(Hex1,Hex2);
    if (returns!=SUCCESS) {
     return returns;
    }
    Data=Parser.Get_Next();
  }

  // if we ended the loop, and dectected something other that an E, we have an issue
  if (Data.type!='E') {
    return INVALID_DATA_ENTRY;
  }
  if  (!(Hex1.FromHex(Data.data[0]))) {
    return NOT_HEX;
  }

  //this should be copying Hex1 onto the PC_address.  if it is not, that is what it is meant to be doing
  PC_address.Copy(Hex1);

  return SUCCESS;
}
