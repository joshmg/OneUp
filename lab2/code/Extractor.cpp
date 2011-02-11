// File:        Extractor.cpp
// Written by:  Andrew Groot

#include "Extractor.h"
#include "Word.h"
#include "SymbolTable.h"
#include "ResultCodes.h"
#include <string>
#include <map>
#include <fstream>
using namespace std;
using namespace Codes;

// public
Extractor::~Extractor() {
  if (_fileStream.is_open()) {
    _fileStream.close();
  }
}

bool Extractor::Open(std::string filename) {
  if (_fileStream.is_open()) {
    _fileStream.close();
  }

  _fileStream.open(filename);

  return _fileStream.is_open();
}

RESULT Extractor::GetSymbolTable(SymbolTable& symbols) {
  
}

Word Extractor::GetLength(Word length) {
  return _length;
}

