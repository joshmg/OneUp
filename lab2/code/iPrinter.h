/*! @file iPrinter.h
    @author Andrew Groot
    @brief Definition of the output of the Wi-11 assembler.
*/

#ifndef iPRINTER_H
#define iPRINTER_H

#include "iListing.h"
#include "iWord.h"
#include "SymbolTable.h"
#include "ResultCodes.h"
#include <string>

class iPrinter {
  private:

  public:
    virtual bool Open(std::string filename) = 0;

    virtual Codes::RESULT Print(SymbolTable& symbols, iWord& file_length) const = 0;
};

#endif
