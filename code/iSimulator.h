// File: iSimulator.h
// Written by:
//    Joshua Green

#ifndef iSIMULATOR_H
#define iSIMULATOR_H

#include "iWord.h"
#include "iRegister.h"
#include "iMemory.h"

enum REGISTER_ID { R0, R1, R2, R3, R4, R5, R6, R7, PC };

class iSimulator {
  private:
    
  public:
    virtual bool Initialize(const char*) = 0;

    virtual bool Add(const REGISTER_ID DR, const REGISTER_ID SR1, const REGISTER_ID SR2) = 0;
    virtual bool Add(const REGISTER_ID DR, const REGISTER_ID SR1, const iWord& immediate) = 0;

    virtual bool And(const REGISTER_ID DR, const REGISTER_ID SR1, const REGISTER_ID SR2) = 0;
    virtual bool And(const REGISTER_ID DR, const REGISTER_ID SR1, const iWord& immediate) = 0;

    virtual bool Branch(const iWord& address) = 0;

    virtual bool Debug() = 0;

    virtual bool JSR(const iWord&) = 0;
    virtual bool JSRR(const iWord& baseR, const iWord& address) = 0;

    virtual bool Load(const REGISTER_ID DR, const iWord& address) = 0;
    virtual bool LDI(const REGISTER_ID DR, const iWord& address) = 0;
    virtual bool LDR(const REGISTER_ID DR, const iWord& baseR, const iWord& address) = 0;

    virtual bool Not(const REGISTER_ID DR, const REGISTER_ID SR) = 0;

    virtual bool Ret() = 0;

    virtual bool Store(const REGISTER_ID DR, const iWord& address) = 0;
    virtual bool STI(const REGISTER_ID DR, const iWord& address) = 0;
    virtual bool STR(const REGISTER_ID DR, const iWord& baseR, const iWord& address) = 0;

    virtual bool Trap(const iWord& address) = 0;
};

#endif
