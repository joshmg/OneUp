// File: Wi11.h
// Written by:
//    Joshua Green

#ifndef WI11_H
#define WI11_H

#include "Word.h"
#include "Register.h"
#include "Memory.h"
#include "Decoder.h"
#include "ResultCodes.h"
#include "Loader.h"
#include "ResultCodes.h"

class Wi11 : public iWi11 {
  private:

    Memory _memory;
    Register _R0, _R1, _R2, _R3, _R4, _R5, _R6, _R7, _PC;

    struct CCR {
      bool n, z, p;
    } _CCR;

    ObjLoader _loader;
    Decoder _decoder;
    ResultDecoder _result_decoder;

    REGISTER_ID Wi11::_Word2RegisterID(const Word&) const;

    iRegister& _GetRegister(const Decoder::REGISTER_ID&);

    void _UpdateCCR(int);

    virtual Codes::RESULT _Add(const Decoder::REGISTER_ID& DR, const Decoder::REGISTER_ID& SR1, const Decoder::REGISTER_ID& SR2);

    virtual Codes::RESULT _Add(const Decoder::REGISTER_ID& DR, const Decoder::REGISTER_ID& SR1, const iWord& immediate);

    virtual Codes::RESULT _And(const Decoder::REGISTER_ID& DR, const Decoder::REGISTER_ID& SR1, const Decoder::REGISTER_ID& SR2);

    virtual Codes::RESULT _And(const Decoder::REGISTER_ID& DR, const Decoder::REGISTER_ID& SR1, const iWord& immediate);

    virtual Codes::RESULT _Branch(const iWord& address);

    virtual Codes::RESULT _Debug();

    virtual Codes::RESULT _JSR(const iWord&, bool);

    virtual Codes::RESULT _JSRR(const Decoder::REGISTER_ID& baseR, const iWord& address, bool link);

    virtual Codes::RESULT _Load(const Decoder::REGISTER_ID& DR, const iWord& address);

    virtual Codes::RESULT _LoadI(const Decoder::REGISTER_ID& DR, const iWord& address);

    virtual Codes::RESULT _LoadR(const Decoder::REGISTER_ID& DR, const Decoder::REGISTER_ID& baseR, const iWord& address);

    virtual Codes::RESULT _LoadEA(const Decoder::REGISTER_ID& DR, const iWord& address);

    virtual Codes::RESULT _Not(const Decoder::REGISTER_ID& DR, const Decoder::REGISTER_ID& SR);

    virtual Codes::RESULT _Ret();

    virtual Codes::RESULT _Store(const Decoder::REGISTER_ID& SR, const iWord& address);

    virtual Codes::RESULT _STI(const Decoder::REGISTER_ID& SR, const iWord& address);

    virtual Codes::RESULT _STR(const Decoder::REGISTER_ID& SR, const Decoder::REGISTER_ID& baseR, const iWord& address);

    virtual Codes::RESULT _Trap(const iWord& code);
    
  public:
    virtual iWi11();

    virtual bool LoadObj(const char*);

    virtual void DisplayMemory() const;

    virtual void DisplayRegisters() const;

    virtual bool ExecuteNext(bool verbose=false);

};

#endif
