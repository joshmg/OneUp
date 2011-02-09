/*! @file iExtractor.h
    @author Andrew Groot
    @brief Definition of the Extractor for the Wi-11 assembler.
*/

#ifndef iEXTRACTOR_H
#define iEXTRACTOR_H

#include "SymbolTable.h"
#include "ResultCodes.h"
#include <string>
#include <map>

class iExtractor {
  private:

  public:
    virtual bool Open(std::string filename) = 0;

    virtual Codes::RESULT GetSymbols(SymbolTable symbols) = 0;

    virtual Word GetLength() const = 0;
};

#endif
