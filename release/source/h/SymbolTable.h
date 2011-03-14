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

/*! @brief Implements the iSymbolTable interface.

    This implementation keeps mappings of the different
    symbols and literal values to their address, as well as
    a mapping of symbol to its "relocatable" status.
    The purpose of this class is primarily to isolate the
    use of some of the more fragile code and provide the client
    with a tool rather than a list of components to pass into
    every function. 
*/
class SymbolTable : public iSymbolTable {
  private:
    //! Store symbol to address mappings.
    std::map<std::string, Word> _symbols;
    //! Store literal value to address mappings.
    std::map<int, Word> _literals;
    //! Store relocation information by string.
    std::map<std::string, bool> _relocatable;
    //! Store exteranls -- map merely for ease of use.
    std::map<std::string, bool> _externals;

  public:
    void InsertLabel(std::string label, Word addr, bool relocate=false);
    void InsertLiteral(int value, Word addr);
    void AddExternal(std::string label);

    bool IsSymbol(std::string symbol) const;
    bool IsExternal(std::string label) const;

    Word GetLabelAddr(std::string symbol) const;
    Word GetLiteralAddr(int value) const;

    bool IsRelocatable(std::string label) const;

    int LabelCount() const;
    int LiteralCount() const;

    const std::map<int, Word>* GetLiterals() const;
};

#endif
