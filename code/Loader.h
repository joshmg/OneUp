// File: Loader.h
// Written by Logan Coulson
//            Joshua Green

#ifndef LOADER_H
#define LOADER_H

#include "iLoader.h"
#include "iMemory.h"
#include "ResultCodes.h"

class Loader : public iLoader {
  private:
    iMemory* _memory;
    
  public:
    virtual iLoader(iMemory* mem) = 0;

    virtual Codes::RESULT Load(const char* filename, iWord& PC_address) const = 0;
};

#endif
