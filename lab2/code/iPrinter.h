/*! @file iPrinter.h
    @author Andrew Groot
    @brief Definition of the output of the Wi-11 assembler.
*/

#ifndef iPRINTER_H
#define iPRINTER_H

#include "SymbolTable.h"
#include "Word.h"
#include "ResultCodes.h"
#include <string>

/*! @brief Writes an object file and prints a listing to standard out.

    This class defines a very simple interface by which the second pass
    of the two-pass algorithm is completed and the user is presented with
    useful information about the encoding process as well as an object file
    if the code is entirely valid.
*/
class iPrinter {
  private:

  public:
    /*! @brief Opens the input and output files for reading and writing.
        @param[in] infile The name of the input file to be opened.
        @param[in] outfile The name of the output file to be opened.
        @return 
    */
    virtual Codes::RESULT Open(std::string infile, std::string outname) = 0;

    /*! @brief Reads from the source file, writes an object file, and prints a listing.
        @param[in] symbols A SymbolTable produced from a previous read of the input file.
        @param[in] file_length The size the program should occupy in memory.
        @return SUCCESS iff the object file could be created; otherwise an appropriate error message.
    */
    virtual Codes::RESULT Print(SymbolTable& symbols, Word file_length) const = 0;
};

#endif
