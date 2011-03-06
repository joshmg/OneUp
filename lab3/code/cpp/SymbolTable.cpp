// File:        SymbolTable.h
// Written by:  Joshua Green

#include "../h/SymbolTable.h"
#include "../h/Word.h"
#include <map>
#include <string>
using namespace std;

//    map<std::string, Word> _symbols;
//    map<int, Word> _literals;
//    map<std::string, bool> _relocatable;
//    map<std::string, bool> _externals;

void SymbolTable::InsertLabel(string label, Word addr, bool relocate) {
  _symbols[label] = addr;
  _relocatable[label] = relocate;
}

void SymbolTable::InsertLiteral(int value, Word addr) {
  _literals[value] = addr;
}

void SymbolTable::AddExternal(string label) {
  // The accessor ensures that the entry in in map
  // Here we don't care what the mapped value is
  _externals[label] = true;
}

bool SymbolTable::IsSymbol(string symbol) const {
  return (_symbols.count(symbol) != 0);
}

bool SymbolTable::IsExternal(string symbol) const {
  return (_externals.count(symbol) != 0);
}

Word SymbolTable::GetLabelAddr(string symbol) const {
  // GetLabelAddr is designed by contract to require symbol be within Symbol Table;
  // However, if the contract is violated and symbol is not in the table, an unitialized Word object is returned.

  map<string, Word>::const_iterator it = _symbols.find(symbol);
  if (it == _symbols.end()) {
    return Word();
  }
  else {
    return it->second;
  }
}

Word SymbolTable::GetLiteralAddr(int value) const {
  // GetLiteralAddr is designed by contract to require symbol be within Symbol Table;
  // However, if the contract is violated and symbol is not in the table, an unitialized Word object is returned.

  map<int, Word>::const_iterator it = _literals.find(value);
  if (it == _literals.end()) {
    return Word();
  }
  else {
    return it->second;
  }
}

bool SymbolTable::IsRelocatable(string label) const {
  // IsRelocatable is designed by contract to require symbol be within Symbol Table;
  // However, if the contract is violated and symbol is not in the table, false is returned.

  map<string, bool>::const_iterator it = _relocatable.find(label);
  if (it == _relocatable.end()) {
    return false;
  }
  else {
    return it->second;
  }
}

int SymbolTable::LabelCount() const {
  return _symbols.size();
}

int SymbolTable::LiteralCount() const {
  return _literals.size();
}

const map<int, Word>* SymbolTable::GetLiterals() const {
  return &_literals;
}

