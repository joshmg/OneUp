/*! @file iSymbolTable.h
    @author Andrew Groot
    @brief Definition of the symbol table for the Wi-11 assembler.
*/

#ifndef iSYMBOLTABLE_H
#define iSYMBOLTABLE_H

#include "Word.h"
#include <string>

class iSymbolTable {
  private:

  public:
    virtual void InsertLabel(std::string label, Word addr, bool relocate=false) = 0;

    virtual void InsertLiteral(int value, Word addr) = 0;

    virtual Word GetLabelAddr(std::string symbol) const = 0;

    virtual Word GetLiteralAddr(int value) const = 0;

    virtual bool IsRelocatable(std::string label) const = 0;
};

#endif
