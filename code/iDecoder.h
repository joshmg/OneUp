// File: iDecoder.h
// Written by Joshua Green

#ifndef iDECODER_H
#define iDECODER_H

#include "iWord.h"
#include <vector>

class Word;

namespace Decoder {
  enum REGISTER_ID { R0, R1, R2, R3, R4, R5, R6, R7, PC };
  enum INSTRUCTION_TYPE { ADD, AND, BRx, DBUG, JSR, JSRR, LD, LDI, LDR, LEA, NOT, RET, ST, STI, STR, TRAP, ERROR };
}

struct Instruction {
  INSTRUCTION_TYPE type;
  std::vector<Word> data;
};

class iDecoder {
  private:

  public:
    virtual Instruction DecodeInstruction(const iWord&) const = 0;
    /*  DecodeInstruction() parses the provided iWord and returns an
        Instruction. Instruction.type is determined by the first four bits
        of the iWord, and Instruction.data is a vector of structs for each
        number of fields the respective Instruction.type should have.
        Ensures the instruction is valid (ie. the first 4 bits represent
        a valid instruction). [In the final spec, this needs to be
        elaborated on, but for now, refer to page 3 of "The Wi11 Machine
        Characteristics" handout.]
    */

};

#endif
