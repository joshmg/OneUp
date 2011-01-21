// File: iWi11.h
// Written by:
//    Joshua Green

#ifndef iWI11_H
#define iWI11_H

#include "iWord.h"
#include "iRegister.h"
#include "iMemory.h"

enum REGISTER_ID { R0, R1, R2, R3, R4, R5, R6, R7, PC };

class iWi11 {
  private:
    virtual bool _Add(const REGISTER_ID DR, const REGISTER_ID SR1, const REGISTER_ID SR2) = 0;
    virtual bool _Add(const REGISTER_ID DR, const REGISTER_ID SR1, const iWord& immediate) = 0;

    virtual bool _And(const REGISTER_ID DR, const REGISTER_ID SR1, const REGISTER_ID SR2) = 0;
    virtual bool _And(const REGISTER_ID DR, const REGISTER_ID SR1, const iWord& immediate) = 0;

    virtual bool _Branch(const iWord& address) = 0;

    virtual bool _Debug() = 0;

    virtual bool _JSR(const iWord&) = 0;
    virtual bool _JSRR(const iWord& baseR, const iWord& address) = 0;

    virtual bool _Load(const REGISTER_ID DR, const iWord& address) = 0;
    virtual bool _LDI(const REGISTER_ID DR, const iWord& address) = 0;
    virtual bool _LDR(const REGISTER_ID DR, const iWord& baseR, const iWord& address) = 0;

    virtual bool _Not(const REGISTER_ID DR, const REGISTER_ID SR) = 0;

    virtual bool _Ret() = 0;

    virtual bool _Store(const REGISTER_ID DR, const iWord& address) = 0;
    virtual bool _STI(const REGISTER_ID DR, const iWord& address) = 0;
    virtual bool _STR(const REGISTER_ID DR, const iWord& baseR, const iWord& address) = 0;

    virtual bool _Trap(const iWord& address) = 0;
    
  public:
    virtual bool Initialize(const char*) = 0;

};

#endif
