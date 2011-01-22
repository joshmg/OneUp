
#include "iWord.h"
#include "ResultCodes.h"
#include "iMemory.h"
#include "iLoader.h"
#include "iObjParser.h"

//i think that this is meant to be the constructor.  if not, all it needs to do is establish
//the Memory pointer i added to the class
iLoader::iLoader(iMemory *Memory)
{
  *Memory=*Memory;
}
Codes::RESULT iLoader::Load(const char* filename, iWord& PC_address)
{
  //the parser is just the iobjparser object
  iObjParser Parser;
  //used to manipulate memory.  memory has the functions i need, so no hex conversion is needed, as everything is a word
  iWord Hex1,Hex2;
  //the Parser return the Data to this
  ObjectData Data;
  //meant to save the return values, and check if sucesses, or, if not, return itself.
  Codes::RESULTS returns;
  //check for out of bounds acess on memory
  int min_memory, max_memory, mem_location;
  //initializing the Parser
  returns=Parser.Initialize(filename);
    if (returns!=Codes::SUCCESS)
  {
    return returns;
  }
  //getting my first header
  Data=Parser.Get_Next();
  //if it is not a H, problem
  if (Data.type!='H')
  {
    return Codes::INVALID_OBJECT_FILE;
  }
  //setting up the words to use the reserve command memory has.  seemed to me to be an time improver.
  if  (!(Hex1.FromHex(Data.data[1])))
  {
    return Codes::NOT_HEX;
  }
  //establishing the lower bound on memory
  min_memory=Hex1.ToInt();
  if  (!(Hex2.FromHex(Data.data[2])))
  {
    return Codes::NOT_HEX;
  }
  //establishing the upper bound on memory
  max_memory=init_memory+Hex2.ToInt();
  returns=Memory->Reserve(Hex1,Hex2);
  if (returns!=Codes::SUCCESS)
  {
    return returns;
  }
  Data=Parser.Get_Next();
  //this loop goes through the Test records
  while (Data.type=='T')
  {
    if  (!(Hex1.FromHex(Data.data[0])))
    {
      return Codes::NOT_HEX;
    }
    //converting the hex memory location to int
    mem_location=Hex1.ToInt();
    //ensureing that the memory location is in bounds
    if (!(mem_location>min_memory && mem_location<max_memory))
    {
      return Codes::OUT_OF_BOUNDS;
    }
    if  (!(Hex2.FromHex(Data.data[1])))
    {
      return Codes::NOT_HEX;
    }
    //if it is in bounds, store it
    returns=Memory->Store(Hex1,Hex2);
    if (returns!=Codes::SUCCESS)
    {
     return returns;
    }
    Data=Parser.Get_Next();
  }
  //if we ended the loop, and dectected something other that an E, we have an issue
  if (Data.type!='E')
  {
    return Codes::INVALID_DATA_ENTRY;
  }
  if  (!(Hex1.FromHex(Data.data[0])))
    {
      return Codes::NOT_HEX;
    }
  //this should be copying Hex1 onto the PC_address.  if it is not, that is what it is meant to be doing
  PC_address.Copy(Hex1);
  return Codes::SUCCESS;
}

    
    




