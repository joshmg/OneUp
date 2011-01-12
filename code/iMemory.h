// File: iMemory.h
// Written by:
//    Joshua Green

#ifndef iMEMORY_H
#define iMEMORY_H

#include "iWord.h"

class Word;

class iMemory {
    /*  iMemory is a basic component to mimic the functionality of memory
        found within the Wi-11 Machine. Its size can be variable per object
        but can be only set once, at which point the values of Address
        Offset, and Memory Size become immutable. iMemory is capable of
        storing, overwriting, and retrieving values. iMemory utilizes iWord
        as its data's type. setAddress(), setSize(), and Initialize() must
        be called before any calls to Store() or Load() are performed.
    */
  private:
    
  public:
    virtual void setAddress(const iWord&) const = 0;
    /*  setAddress() (set Address Offset) must be invoked prior to
        any calls to Initialize(), Load(), and Store(). setAddress() sets
        the scope of address to be used during Load() and Store()
        operations. Attempting to access a memory space less than the value
        of the Address Offset will produce an iWord of value 0. A call to
        setAddress() after a call to Initialize() will produce no result.
    */

    virtual void setSize(const int lowerbound, const int upperbound) const = 0;
    /*  setSize() (set Memory Size) must be invoked prior to any calls to
        Initialize(), Load(), and Store(). setSize() sets capacity of iWord
        values to be stored within iMemory. Attempting to access a memory
        space less greater than the Address Offset + the Memory Size will
        produce an iWord of value 0. A call to setSize() after a call to
        Initialize() will produce no result.
    */

    virtual bool Initialize() const = 0;
    /*  Initialize() must be invoked prior to any calls to Load() and
        Store(). Initialize() finalizes the capacity of iMemory as well as
        finalizes the Address Offset value within iMemory. Initialize()
        will return a false value if setAddress() and setSize() have not
        been called successfully.
    */

    virtual Word Load(const iWord&) const = 0;
    /*  Load(iWord) interprets its parameter as an address within memory.
        Before a call to Load() is executed, setAddress(), setSize(), and
        Initialize() but be invoked. The value of Load's parameter is
        expected to be between the values supplied during the call to
        setAddress() ("Address Offset") + the value supplied during the
        call to setSize() ("Memory Size") - 1. That is:
          (Address Offset) <= (Address) <= (Address Offset + Memory Size - 1)
    */

    virtual bool Store(const iWord& address, const iWord& value) = 0;
    /*  Store(iWord, iWord) interprets its first parameter as an address
        within iMemory and its second parameter as an immediate value to be
        stored within iMemory. Before a call to Store() is executed,
        setAddress(), setSize(), and Initialize() but be invoked. The value
        of Load's address parameter is expected to be between the values
        supplied during the call to setAddress() ("Address Offset") + the
        value supplied during the call to setSize() ("Memory Size") - 1.
        That is:
          (Address Offset) <= (Address) <= (Address Offset + Memory Size - 1)
    */
};

#endif
