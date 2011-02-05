/*! @file iSymbolTable.h
    @author Andrew Groot
    @brief Definition of the private data for the "SymbolTable" class.
*/

#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include "iSymbolTable.h"
#include "iWord.h"
#include <map>
#include <string>

class SymbolTable : public iSymbolTable {
  private:
    map<std::string, iWord&> _symbols;
    map<int, iWord&> _literals;
    map<std::string, bool> _relocatable;

  public:
    void InsertLabel(std::string label, iWord& addr, bool relocate=false);

    void InsertLiteral(int value, iWord& addr);

    iWord& GetLabelAddr(std::string symbol) const;

    iWord& GetLiteralAddr(int value) const;

    bool IsRelocatable(std::string label) const;
};

#endif
