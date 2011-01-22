/*! @file Wi11.h
    @author Andrew Groot
    @brief Definition of the private data for the "Wi11" class.
*/

#ifndef WI11_H
#define WI11_H

#include "Word.h"
#include "Register.h"
#include "Memory.h"
#include "Decoder.h"
#include "ResultCodes.h"
#include "iWi11.h"

class Wi11 : public iWi11 {
  private:
    //! Wi-11's memory.
    Memory _mem;
    //! 8 general purpose registers.
    Register _reg[8];
    //! CCR, true iff positive.
    bool _pos;
    //! CCR, true iff zero.
    bool _zero;

    iRegister& _GetRegister(const Decoder::REGISTER_ID& id);
    Codes::RESULT _Add(const Decoder::REGISTER_ID DR, const Decoder::REGISTER_ID SR1, const Decoder::REGISTER_ID SR2);
    Codes::RESULT _Add(const Decoder::REGISTER_ID DR, const Decoder::REGISTER_ID SR1, const iWord& immediate);
    Codes::RESULT _And(const Decoder::REGISTER_ID DR, const Decoder::REGISTER_ID SR1, const Decoder::REGISTER_ID SR2);
    Codes::RESULT _And(const Decoder::REGISTER_ID DR, const Decoder::REGISTER_ID SR1, const iWord& immediate);
    Codes::RESULT _Branch(const iWord& address);
    Codes::RESULT _Debug();
    Codes::RESULT _JSR(const iWord& w);
    Codes::RESULT _JSRR(const iWord& baseR, const iWord& address);
    Codes::RESULT _Load(const Decoder::REGISTER_ID DR, const iWord& address);
    Codes::RESULT _LoadI(const Decoder::REGISTER_ID DR, const iWord& address);
    Codes::RESULT _LoadR(const Decoder::REGISTER_ID DR, Decoder::REGISTER_ID baseR, const iWord& address);
    Codes::RESULT _Not(const Decoder::REGISTER_ID DR, const Decoder::REGISTER_ID SR);
    Codes::RESULT _Ret();
    Codes::RESULT _Store(const Decoder::REGISTER_ID SR1, const iWord& address);
    Codes::RESULT _STI(const Decoder::REGISTER_ID SR1, const iWord& address);
    Codes::RESULT _STR(const Decoder::REGISTER_ID SR1, const Decoder::REGISTER_ID baseR, const iWord& address);
    Codes::RESULT _Trap(const iWord& code);
   
  public:
    Wi11();

    // This will prove to be a useful command, but is not necessary at this time.
    // bool Reset();

    bool LoadObj(const char* filename);

    void DisplayMemory() const;
    void DisplayRegisters() const;

    bool ExecuteNext(bool verbose=false);
    /*  Data is loaded at the address contained within the PC from Memory,
        then the PC is incremented by one. The loaded data is sent to the
        iDecoder component which returns an Instruction struct. The type
        field within the struct is used to determine the appropriate
        private function to execute (in the case of branching, ExecuteStep()
        determines if the branch should be executed. The combination of the
        type field coupled with the size of the data field determines the
        appropriate parameters. If the end of the program has been reached,
        ExecuteStep() returns true. [In the official spec, the list of the
        commands and how they are determined must be included in this 
        section. For now, reference page 3 of "The Wi11 Machine
        Characteristics" handout.]
    */
};

#endif
