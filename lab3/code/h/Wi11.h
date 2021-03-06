/*! @file Wi11.h
    @author Joshua Green
    @author Andrew Groot
    @brief Definition of the private data for the "Wi11" class.
*/

#ifndef WI11_H
#define WI11_H

#include "iWi11.h"
#include "Word.h"
#include "Register.h"
#include "Memory.h"
#include "Decoder.h"
#include "ResultCodes.h"
#include "Loader.h"
#include "ResultCodes.h"

//! Implements iWi11.
class Wi11 : public iWi11 {
  private:
    //! Acts as the Wi-11's memory.
    Memory _memory;
    //! The 8 general purpose registers and PC.
    Register _R0, _R1, _R2, _R3, _R4, _R5, _R6, _R7, _PC;
    //! Condition code registers: negative, zero, positive.
    struct CCR {
      bool n, z, p;
    } _CCR;
    //! For loading the object file.
    Loader _loader;
    //! For decoding instructions fetch from memory.
    Decoder _decoder;
    //! For error messages.
    ResultDecoder _result_decoder;

    //! Translates an internal Register ID identifier into human readable string format.
    std::string _RegisterID2String(const Decoder_Directory::REGISTER_ID&) const;

    Decoder_Directory::REGISTER_ID _Word2RegisterID(const Word&) const;
    iRegister& _GetRegister(const Decoder_Directory::REGISTER_ID&);
    void _UpdateCCR(int);
    virtual Codes::RESULT _Add(const Decoder_Directory::REGISTER_ID& DR, const Decoder_Directory::REGISTER_ID& SR1, const Decoder_Directory::REGISTER_ID& SR2);
    virtual Codes::RESULT _Add(const Decoder_Directory::REGISTER_ID& DR, const Decoder_Directory::REGISTER_ID& SR1, const iWord& immediate);
    virtual Codes::RESULT _And(const Decoder_Directory::REGISTER_ID& DR, const Decoder_Directory::REGISTER_ID& SR1, const Decoder_Directory::REGISTER_ID& SR2);
    virtual Codes::RESULT _And(const Decoder_Directory::REGISTER_ID& DR, const Decoder_Directory::REGISTER_ID& SR1, const iWord& immediate);
    virtual Codes::RESULT _Branch(const iWord& address);
    virtual Codes::RESULT _Debug();
    virtual Codes::RESULT _JSR(const iWord&, bool);
    virtual Codes::RESULT _JSRR(const Decoder_Directory::REGISTER_ID& baseR, const iWord& address, bool link);
    virtual Codes::RESULT _Load(const Decoder_Directory::REGISTER_ID& DR, const iWord& address);
    virtual Codes::RESULT _LoadI(const Decoder_Directory::REGISTER_ID& DR, const iWord& address);
    virtual Codes::RESULT _LoadR(const Decoder_Directory::REGISTER_ID& DR, const Decoder_Directory::REGISTER_ID& baseR, const iWord& address);
    virtual Codes::RESULT _LoadEA(const Decoder_Directory::REGISTER_ID& DR, const iWord& address);
    virtual Codes::RESULT _Not(const Decoder_Directory::REGISTER_ID& DR, const Decoder_Directory::REGISTER_ID& SR);
    virtual Codes::RESULT _Ret();
    virtual Codes::RESULT _Store(const Decoder_Directory::REGISTER_ID& SR, const iWord& address);
    virtual Codes::RESULT _STI(const Decoder_Directory::REGISTER_ID& SR, const iWord& address);
    virtual Codes::RESULT _STR(const Decoder_Directory::REGISTER_ID& SR, const Decoder_Directory::REGISTER_ID& baseR, const iWord& address);
    virtual Codes::RESULT _Trap(const iWord& code);

  public:
    /*! @brief Creates and organizes the components of the Wi11 machine.

        Initializes the general purpose registers, CCR, and memory.
    */
    Wi11();

    virtual bool LoadObj(const char*);

    virtual void DisplayMemory() const;
    virtual void DisplayRegisters() const;

    virtual bool ExecuteNext(bool verbose=false);

};

#endif
