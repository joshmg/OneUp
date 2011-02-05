/*! @file iExtractor.h
    @author Andrew Groot
    @brief Definition of the Extractor for the Wi-11 assembler.
*/

#ifndef iEXTRACTOR_H
#define iEXTRACTOR_H

#include "iSymbolTable.h"
#include "iWord.h"
#include "iLine.h"
#include "ResultCodes.h"
#include <string>
#include <map>

class iExtractor {
  private:

  public:
    virtual bool Open(std::string filename) = 0;

    virtual Codes::RESULT Read() = 0;

    virtual iSymbolTable* GetSymbols() const = 0;

    virtual iWord& GetLength() const = 0;
};

#endif
