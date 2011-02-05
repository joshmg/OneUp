/*! @file ResultCodes.h
    @author Joshua Green
    @author Andrew Groot
    @brief Definition of the Wi-11 assemblers error messages.
*/

#ifndef RESULTCODES_H
#define RESULTCODES_H

#include <string>
#include <map>

/*! @brief Values corresponding to the results of Wi-11 function calls.

    An enum is used for efficiency.  The code can be returned up the
    collaboration hierarchy quickly so that, if necessary, the program
    can print an appropriate error message

    @note
    ResultDecoder can be used to do a look-up of the error message.
*/
namespace Codes {

  enum ERRORS {
    ERROR_0,              // quick descipription
    SUCCESS,              // Operation succeeded
    INV_LBL,          // Label starts with an invalid character
    LBL_WO_INST,          // Label does not accompany an instruction
    INV_INST,         // Instruction does not exist
    ARG_SIZE,             // Wrong number of arguments
    EMPTY_ARG,            // Argument is the empty string
    EXP_REG,              // Expected a register as an arugment
    INV_REG,              // Invalid register number
    EXP_CONST,            // Expected a constant as an argument
    INV_CONST,            // Constant out of range
    INV_ARG,              // Invalid argument found
  };

  struct RESULT {
    std::string info;
    ERROR msg;

    RESULT(std::string inf = "", RESULT err) {
      info = inf;
      msg = err;
    }
  };
}

/*! @brief Finds the messages associated with a given result code.
*/
class ResultDecoder {
  private:
    /*! @brief Maps a result code to, in every case but SUCCESS, an error message.

        It is static because the result code messages should be available from anywhere.
    */
    std::map<Codes::RESULT, std::string> _codes;
  public:
    //! Generates the code-to-message mappings.
    ResultDecoder() {
      _codes[Codes::ERROR_0] = "This is an example description sent to the end-user upon verbose mode.";
      _codes[Codes::SUCCESS] = "Successful.";
      _codes[Codes::INV_LBL] = "Label starting with 'R' or 'x'.";
      _codes[Codes::LBL_WO_INST] = "Label is not followed by an instruction.";
      _codes[Codes::INV_INST] = "Instruction not recognized.";
      _codes[Codes::ARG_SIZE] = "Invalid number of arguments.";
      _codes[Codes::EMPTY_ARG] = "Argugment is the empty string (misplaced comma?).";
      _codes[Codes::EXP_REG] = "Expected a register as an argument.";
      _codes[Codes::INV_REG] = "Non-existent register as arugment.";
      _codes[Codes::EXP_CONST] = "Expected a constant as an argument.";
      _codes[Codes::INV_CONST] = "Constant value out of range.";
      _codes[Codes::INV_ARG] = "Invalid arugment.";
    }

    /*! @brief Looks up a result code.
        @param[in] result The result code to look up.
        @returns The messages associated with "result".
    */
    std::string Find(const Codes::RESULT& result) const;
};

#endif