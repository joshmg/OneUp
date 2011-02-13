// File:        SymbolTable.h
// Written by:  Joshua Green

#include "SymbolTable.h"
#include "Word.h"
#include <map>
#include <string>
using namespace std;

//    map<std::string, Word> _symbols;
//    map<int, Word> _literals;
//    map<std::string, bool> _relocatable;

void SymbolTable::InsertLabel(string label, Word addr, bool relocate) {
  
}

void SymbolTable::InsertLiteral(int value, Word addr) {
  
}

bool SymbolTable::Contains(string symbol) const {

}

Word SymbolTable::GetLabelAddr(string symbol) const {
  
}

Word SymbolTable::GetLiteralAddr(int value) const {
  
}

bool SymbolTable::IsRelocatable(string label) const {
  
}

