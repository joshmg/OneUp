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

  enum RESULT {
    ERROR_0,              // quick descipription
    SUCCESS              // Operation succeeded
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
    }

    /*! @brief Looks up a result code.
        @param[in] result The result code to look up.
        @returns The messages associated with "result".
    */
    std::string Find(const Codes::RESULT& result) const;
};

#endif
