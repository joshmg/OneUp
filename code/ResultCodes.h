// File: ResultCodes.h
// Written by Joshua Green

#ifndef RESULTCODES_H
#define RESULTCODES_H

#include <string>
#include <map>

/*  Purpose:
      RESULT codes will be returned by most of Wi11's functions providing
      useful debug information relating the success or failure of the
      execution. The RESULT codes should be enumerations for efficiency--
      directly returning a string explaining the error is resource costly
      and not always necessary (for instance, the code can be thrown away
      if the Wi11 is not being run in verbose mode). If the code thrown
      requires a desciption, ResultDecoder can be used to do a look-up of
      the error code.
*/

namespace Codes {

  enum RESULT {
    ERROR_0,          // quick descipription
    SUCCESS,          // Operation succeeded
    HALT,             // TRAP Halt Command
    UNDEFINED,         // An error description could not be found.
    INVALID_OBJECT_FILE,  // the first letter in the object file was not a H
    INVALID_DATA_ENTRY,   // there was an invalid data entry in the object file
    OUT_OF_BOUNDS,        // attempt to save memory outside of claimed area
    NOT_HEX           // the suposedly hex string is not hex
  };

}

class ResultDecoder {
  private:
    static std::map<Codes::RESULT, std::string> _codes;
  public:
    ResultDecoder() {
      _codes[Codes::ERROR_0] = "This is an example description sent to the end-user upon verbose mode.";
      _codes[Codes::SUCCESS] = "The operation executed successfully.";
      _codes[Codes::HALT] = "Execution has been terminated (Halt).";
      _codes[Codes::UNDEFINED] = "An unexpected error occured.";
      _codes[Codes::INVALID_OBJECT_FILE] = "The First character in the object file must be a H.";
      _codes[Codes::INVALID_DATA_ENTRY] = "There is an invalid text record or there was no end record found.";
      _codes[Codes::OUT_OF_BOUNDS] = "Attempt to access memory outside of the program load area.";
      _codes[Codes::NOT_HEX] = "There was an invalid hex string found.";
    }

    std::string Find(const Codes::RESULT&) const;
};

#endif
