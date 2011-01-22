// File: iMemory.h
// Written by:
//    Joshua Green

#ifndef iMEMORY_H
#define iMEMORY_H

#include "iWord.h"
#include "ResultCodes.h"
#include <vector>

class Word;

class iMemory {
    /*  iMemory is a basic component to mimic the functionality of memory
        found within the Wi11 Machine. Its size is semi-infinite, providing
        constant-time access to addresses reserved at initialization and
        nlogn time for memory outside those bounds. iMemory is capable of
        storing, overwriting, and retrieving values. iMemory utilizes iWord
        as its data's type.
    */
  private:
    
  public:
    virtual Codes::RESULT Reserve(const iWord& initial_address, const iWord& length) = 0;
    /*  Reserve() reserves a dynamic piece of memory that, when accessing
        addresses between initial_address (inclusive) and
        initial_address+length (exclusive), the value is retrieved in
        constant time.
    */

    virtual Word Load(const iWord&) const = 0;
    /*  Load() interprets its parameter as an address within memory.
        The value of Load's parameter is expected to be between the values
        supplied during the call to Reserve() if the data is to be retrieved
        in constant time. Otherwise, maximum retrieval time is nlogn, where
        n is the size of Memory outside of the Reserved space.

    */

    virtual Codes::RESULT Store(const iWord& address, const Word& value) = 0;
    /*  Store() interprets its parameter as an address within memory.
        The value of Store's parameter is expected to be between the values
        supplied during the call to Reserve() if the data is to be stored
        in constant time. Otherwise, maximum store time is nlogn, where
        n is the size of Memory outside of the Reserved space.
    */

    std::vector<Word[2]> GetUsedMemory() const = 0;
};

#endif
