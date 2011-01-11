// File: iMemory.h
// Written by:
//    Joshua Green

#ifndef iMEMORY_H
#define iMEMORY_H

#include "iWord.h"
#include "iRegister.h"

class Word;

class iMemory {
  private:
    
  public:
    virtual Word Load(const iWord&) const = 0;

    virtual bool Store(const iWord& address, const iWord& value) = 0;
    virtual bool Store(const iWord& address, const iRegister&) = 0;

    virtual void setAddress(const iWord&) const = 0;
    virtual void setSize(const int lowerbound, const int upperbound) const = 0;
    virtual bool Initialize() const = 0;
};

#endif
