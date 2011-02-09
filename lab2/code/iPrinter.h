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

class iPrinter {
  private:

  public:
    virtual bool Open(std::string infile, std::string outname) = 0;

    virtual Codes::RESULT Print(SymbolTable& symbols, Word file_length) const = 0;
};

#endif
