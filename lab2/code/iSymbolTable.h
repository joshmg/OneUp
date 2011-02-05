/*! @file iSymbolTable.h
    @author Andrew Groot
    @brief Definition of the symbol table for the Wi-11 assembler.
*/

#ifndef iSYMBOLTABLE_H
#define iSYMBOLTABLE_H

#include "iWord.h"
#include <string>

class iSymbolTable {
  private:

  public:
    virtual void InsertLabel(std::string label, iWord& addr, bool relocate=false) = 0;

    virtual void InsertLiteral(int value, iWord& addr) = 0;

    virtual iWord& GetLabelAddr(std::string symbol) const = 0;

    virtual iWord& GetLiteralAddr(int value) const = 0;

    virtual bool IsRelocatable(std::string label) const = 0;
};

#endif
