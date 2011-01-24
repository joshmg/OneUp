/*! @file ResultCodes.h
    @author Joshua Green
    @author Andrew Groot
    @brief Definition of the Wi-11's run-time messages.
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

  enum RESULT {
    ERROR_0,              // quick descipription
    SUCCESS,              // Operation succeeded
    HALT,                 // TRAP Halt Command
    UNDEFINED,            // An error description could not be found.
    INVALID_HEADER_ENTRY, // Invalid object file header
    INVALID_DATA_ENTRY,   // there was an invalid data entry in the object file
    OUT_OF_BOUNDS,        // attempt to save memory outside of claimed area
    NOT_HEX,              // the suposedly hex string is not hex
    FILE_NOT_FOUND,       // Object file could not be opened
    INVALID_TRAP_CODE     // The provided trap code is not valid.
  };

}

/*! @brief Finds the messages associated with a given result code.
*/
class ResultDecoder {
  private:
    /*! @brief Maps a result code to, in every case but SUCCESS, an error message.

        It is static because the result code messages should be available from anyhere.
    */
    std::map<Codes::RESULT, std::string> _codes;
  public:
    //! Generates the code-to-message mappings.
    ResultDecoder() {
      _codes[Codes::ERROR_0] = "This is an example description sent to the end-user upon verbose mode.";
      _codes[Codes::SUCCESS] = "Successful.";
      _codes[Codes::HALT] = "Execution has been terminated (Halt).";
      _codes[Codes::UNDEFINED] = "An unexpected error occured.";
      _codes[Codes::INVALID_HEADER_ENTRY] = "Invalid object file header (Was the first character an 'H'?).";
      _codes[Codes::INVALID_DATA_ENTRY] = "There is an invalid text record or there was no end record found.";
      _codes[Codes::OUT_OF_BOUNDS] = "Attempt to access memory outside of the program load area.";
      _codes[Codes::NOT_HEX] = "There was an invalid hex string found.";
      _codes[Codes::FILE_NOT_FOUND] = "The specified object file could not be found.";
      _codes[Codes::INVALID_TRAP_CODE] = "The provided trap code is not valid.";
    }

    /*! @brief Looks up a result code.
        @param[in] result The result code to look up.
        @returns The messages associated with "result".
    */
    std::string Find(const Codes::RESULT& result) const;
};

#endif
