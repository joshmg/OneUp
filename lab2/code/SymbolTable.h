/*! @file SymbolTable.h
    @author Andrew Groot
    @brief Definition of the private data for the "SymbolTable" class.
*/

#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include "iSymbolTable.h"
#include "Word.h"
#include <map>
#include <string>

class SymbolTable : public iSymbolTable {
  private:
    map<std::string, Word> _symbols;
    map<int, Word> _literals;
    map<std::string, bool> _relocatable;

  public:
    void InsertLabel(std::string label, Word addr, bool relocate=false);

    void InsertLiteral(int value, Word addr);

    Word GetLabelAddr(std::string symbol) const;

    Word GetLiteralAddr(int value) const;

    bool IsRelocatable(std::string label) const;
};

#endif
