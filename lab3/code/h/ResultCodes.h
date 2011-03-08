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

  enum ERROR {
    // Shared Error Codes:
    ERROR_0,        // quick descipription
    SUCCESS,        // Operation succeeded


    // Simulator/Linker Error Codes:
    HALT,                 // TRAP Halt Command
    UNDEFINED,            // An error description could not be found.
    INVALID_HEADER_ENTRY, // Invalid object file header
    INVALID_DATA_ENTRY,   // there was an invalid data entry in the object file
    OUT_OF_BOUNDS,        // attempt to save memory outside of claimed area
    NOT_HEX,              // the suposedly hex string is not hex
    INVALID_TRAP_CODE,    // The provided trap code is not valid.
    INVALID_START_PC,
    REQUESTED_MEMORY_TOO_LARGE,
    BAD_MALLOC,           // Operating system unable to provide required memory to load object file.
    RELOCATE_ENTRY_IN_ABSOLUTE, // Relocatable Text Entry found in a non-relocatable object file.
    RELOCATION_OVERFLOW,  // During relocation, the composit address overflowed.
    RELOCATION_OUTSIDE_BOUNDS,  //During relocation, the composit address attempted to access an address outside of bounds.
    UNRESOLVED_EXTERNAL,  // Unresolved Symbol found in object file.
    LINK_ABS,             // Attempt to link an absolute program
    MULTI_MAIN,           // Attempt to link multiple main files

    // Assembler Error Codes:
    INV_LBL,        // Label starts with an invalid character
    LBL_WO_INST,    // Label does not accompany an instruction
    INV_INST,       // Instruction does not exist
    STRZ_NOT_STR,   // Argument to .STRZ not a string
    END_OF_STR,     // No end quote
    STR_JUNK,       // Extra characters after end quote
    ARG_SIZE,       // Wrong number of arguments
    EMPTY_ARG,      // Argument is the empty string
    INV_REG,        // Invalid register number
    INV_CONST,      // Constant out of range
    INV_ARG,        // Invalid argument found
    INV_HEX,        // Non-hex character after x.
    INV_DEC,        // Non-digit after #.
    INV_BR,         // Invalid Branch
    NON_LD_LIT,     // Literal as an argument to something other than LD.
    ORIG,           // ORIG not first non-comment line
    ORIG2,          // Second ORIG found
    ORIG_LBL,       // ORIG label more than 6 characters
    ORIG_HEX,       // ORIG arg not hex
    REQ_LABEL,      // Instruction requires label
    LBL_NOT_FOUND,  // Could not find label
    REDEF_LBL,      // Attempt to redefine label
    MAX_S_SIZE,     // Maximum number of symbols
    MAX_L_SIZE,     // Maximum number of literals
    MAX_LENGTH,     // Maximum number of source records.
    ABS_REL,        // Absolute value in relative instruction
    INV_IMM,        // Immediate value not in range
    INV_IDX,        // Index value not in range
    PG_ERR,         // Arugment on different page
    NO_END,         // No end record
    END_OB,         // End arg out of bounds
    UNEXP_EOF,      // Unexpected EOF
    REL_PG_SIZE,    // Relocatable prog spans won't fit in one page
    MEM_FIT,        // Program won't fit in memory
    MAIN_LBL,       // .MAIN cannot have a label
    END_LBL,        // .END cannot have a label
    INV_COMMENT,    // Comment without ;
    MAIN,           // Main after .ORIG
    EXT_REDEF,      // Argument to .EXT already defined
    RELATIVE,       // EXT or ENT in an absolute program

    // ^ new codes go here ^
    FILE_NOT_FOUND, // File not found
    FILE_NOT_OPENED // File could not be opened
  };

  //! Holds error-reporting information
  struct RESULT {
    std::string info;
    ERROR msg;

    RESULT(ERROR err=ERROR_0, std::string inf = "") {
      info = inf;
      msg = err;
    }

    bool operator == (const ERROR) const;
    bool operator != (const ERROR) const;
  };
}

//! @brief Finds the messages associated with a given result code
class ResultDecoder {
  private:
    /*! @brief Maps a result code to, in every case but SUCCESS, an error message.

        It is static because the result code messages should be available from anywhere.
    */
    std::map<Codes::ERROR, std::string> _codes;

  public:
    //! Generates the code-to-message mappings.
    ResultDecoder() {
      _codes[Codes::ERROR_0] = "Undefined Error.";
      _codes[Codes::SUCCESS] = "Successful.";


      // Simulator/Linker Error Codes:
      _codes[Codes::HALT] = "Execution has been terminated (Halt).";
      _codes[Codes::UNDEFINED] = "An unexpected error occured.";
      _codes[Codes::INVALID_HEADER_ENTRY] = "Invalid object file header (Was the first character an 'H' or 'M'?).";
      _codes[Codes::INVALID_DATA_ENTRY] = "There is an invalid text record or there was no end record found.";
      _codes[Codes::OUT_OF_BOUNDS] = "Attempt to access memory outside of the program load area.";
      _codes[Codes::NOT_HEX] = "There was an invalid hex string found.";
      _codes[Codes::INVALID_TRAP_CODE] = "The provided trap code is not valid.";
      _codes[Codes::INVALID_START_PC] = "The initial value of the PC is out of bounds.";
      _codes[Codes::REQUESTED_MEMORY_TOO_LARGE] = "Requested memory too large.";
      _codes[Codes::BAD_MALLOC] = "Bad Malloc from OS.";
      _codes[Codes::RELOCATE_ENTRY_IN_ABSOLUTE] = "Relocatable Text Entry found in a non-relocatable object file.";
      _codes[Codes::RELOCATION_OVERFLOW] = "During relocation, the composite address overflowed.";
      _codes[Codes::RELOCATION_OUTSIDE_BOUNDS] = "During relocation, the composite address attempted to access an address outside of bounds.";
      _codes[Codes::UNRESOLVED_EXTERNAL] = "Unresolved Symbol in object file.";
      _codes[Codes::LINK_ABS] = "Attempt to link an absolute program.";
      _codes[Codes::MULTI_MAIN] = "Attempt to link multiple main files.";

      // Assembler Error Codes:
      _codes[Codes::INV_LBL] = "Label starting with 'R' or 'x'.";
      _codes[Codes::LBL_WO_INST] = "Label is not followed by an instruction.";
      _codes[Codes::INV_INST] = "Instruction not recognized.";
      _codes[Codes::STRZ_NOT_STR] = "Argument to \".STRZ\" is not a string.";
      _codes[Codes::END_OF_STR] = "End of string not found.";
      _codes[Codes::STR_JUNK] = "Extra characters found after end of string.";
      _codes[Codes::ARG_SIZE] = "Invalid number of arguments.";
      _codes[Codes::EMPTY_ARG] = "Argugment is the empty string (misplaced comma?).";
      _codes[Codes::INV_REG] = "Non-existent register as arugment.";
      _codes[Codes::INV_CONST] = "Constant value out of range.";
      _codes[Codes::INV_ARG] = "Invalid argument.";
      _codes[Codes::INV_HEX] = "Invalid hex following 'x'.";
      _codes[Codes::INV_DEC] = "Invalid decimal following '#'.";
      _codes[Codes::INV_BR] = "Invalid CCR mask for branch instruction.";
      _codes[Codes::NON_LD_LIT] = "Literals may only be used with the LD instruction.";
      _codes[Codes::ORIG] = "First non-comment line should contain \".ORIG\" instruction.";
      _codes[Codes::ORIG2] = "Second \".ORIG\" instruction found.";
      _codes[Codes::ORIG_HEX] = "Argument to \".ORIG\" not hex.";
      _codes[Codes::ORIG_LBL] = "\".ORIG\" label longer than six characters.";
      _codes[Codes::REQ_LABEL] = "Instruction requires label.";
      _codes[Codes::LBL_NOT_FOUND] = "Label not found.\n((Forward reference to .EQU label?  Case-sensitvity issue?))";
      _codes[Codes::REDEF_LBL] = "Attempt to redefine label.";
      _codes[Codes::MAX_S_SIZE] = "Maximum number of symbols reached.\n((Alter with '-s'?))";
      _codes[Codes::MAX_L_SIZE] = "Maximum number of literals reached.\n((Alter with '-s'?))";
      _codes[Codes::MAX_LENGTH] = "Maximum object file size reached.\n((Alter with '-s'?))";
      _codes[Codes::ABS_REL] = "Absolute value in instruction that requires a relative.";
      _codes[Codes::INV_IMM] = "Immediate value not expressible in 5 bits.";
      _codes[Codes::INV_IDX] = "Index value not expressible in 6 bits.";
      _codes[Codes::PG_ERR] = "Page Error: Address references a different page.";
      _codes[Codes::NO_END] = "File has no end record.";
      _codes[Codes::END_OB] = "Arugment to \".END\" instruction is outside declared memory.";
      _codes[Codes::UNEXP_EOF] = "Unexpected end of file.";
      _codes[Codes::REL_PG_SIZE] = "Relocatable programs cannot occupy more than one page in memory.";
      _codes[Codes::MEM_FIT] = "This program will not fit in memory.";
      _codes[Codes::MAIN_LBL] = "\".MAIN.\" instruction cannot have a label.";
      _codes[Codes::END_LBL] = "\".END\" instruction cannot have a label.";
      _codes[Codes::INV_COMMENT] = "Comment not preceeded by ';'.\n((Whitespace separating arguments?))";
      _codes[Codes::MAIN] = "\".MAIN\" should preceed \".ORIG\".";
      _codes[Codes::EXT_REDEF] = "Argument to \"EXT\" is defined is this file.";
      _codes[Codes::RELATIVE] = "The \".EXT\" and \".ENT\" psuedo-ops are not valid in absolute program.";

      _codes[Codes::FILE_NOT_FOUND] = "File not found.";
      _codes[Codes::FILE_NOT_OPENED] = "File could not be opened.";
    }

    /*! @brief Looks up a result code.
        @param[in] result The result code to look up.
        @returns The messages associated with "result".
    */
    std::string Find(const Codes::RESULT& result) const;
};

#endif
