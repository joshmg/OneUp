/*! @file iDecoder.h
    @author Joshua Green
    @author Andrew Groot
    @brief Definition of the Wi-11 instruction decoder.
*/

#ifndef iDECODER_H
#define iDECODER_H

#include "iWord.h"
#include <vector>

class Word;

/*! @brief Defines the register id's and instruction types.

    With these definitions, the process of executing instructions
    is made easier as REGISTER_ID's and INSTRUCTION_TYPE's can
    be used instead of strings.
*/
namespace Decoder {
  enum REGISTER_ID { R0, R1, R2, R3, R4, R5, R6, R7, PC };
  enum INSTRUCTION_TYPE { ADD, AND, BRx, DBUG, JSR, JSRR, LD, LDI, LDR, LEA, NOT, RET, ST, STI, STR, TRAP, ERROR };
}

/*! @brief Container to simplify interactions with Wi-11 instructions.
*/
struct Instruction {
  //! The type of instruction.
  INSTRUCTION_TYPE type;
  /*! @brief The arguemnts to the operation (including unecessary bits).

      @par Example:
      The add instruction comes in two forms:
      @par
      @arg dest_reg = source_reg_1 + source_reg_2
      For this form, the encoding (as ordered) is as follows:
      <ul>
      <li>dest_reg</li>
      <li>source_reg_1</li>
      <li>a 0</li>
      <li>2 unused bits</li>
      <li>source_reg_2</li>
      These segments are each an element of the data vector.
      </ul>
      @par
      @arg dest_reg = source_reg + immediate_value
      For this form, the encoding (as ordered) is as follows:
      <ul>
      <li>op code</li>
      <li>dest_reg</li>
      <li>source_reg_1</li>
      <li>a 1</li>
      <li>a 5-bit immediate value</li>
      These segments are also each an element of the data vector.
      </ul>
      In short, any division specified in \ref instructions will
      be an element of the data vector.

      @note
      Both of the overloaded instructions (ADD and AND)
      can be differentiated by the number of divisions:
      @arg ADD with two registers has 5
      @arg ADD with a register and immediate has 4
      and
      @arg AND with two registers has 5
      @arg AND with a register and immediate has 4
      Thus the fifth bit (either a 1 or 0) is not
      needed to determine the variation of the instruction
      (HOWEVER: the 1 or 0 is still included).
  */
  std::vector<Word> data;
};

/*! @brief Decodes Wi-11 instructions.
    
    This could be a struct or even a fucntion.
    It is declared as an object for consistency purposes.
*/
class iDecoder {
  private:

  public:
    /*! @brief Translates the binary instruction into more usable objects.
        @param[in] inst The instruction to be translated.
        @return An Instruction object as specificied in \ref Instruction "its documentation".
    */
    virtual Instruction DecodeInstruction(const iWord& inst) const = 0;

};

#endif
