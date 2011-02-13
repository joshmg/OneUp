// File:        Extractor.cpp
// Written by:  Andrew Groot

#include "Extractor.h"
#include "Word.h"
#include "Line.h"
#include "SymbolTable.h"
#include "ResultCodes.h"
#include <cstdlib>
#include <string>
#include <sstream> // for line numbers
#include <map>
#include <vector>
#include <fstream>
using namespace std;
using namespace Codes;

// private
string Extractor::_LineNumber(int pos) {
  stringstream s;
  s << "Line " << pos;
  return s.str();
}

// public
Extractor::~Extractor() {
  if (_fileStream.is_open()) {
    _fileStream.close();
  }
}

bool Extractor::Open(string filename) {
  if (_fileStream.is_open()) {
    _fileStream.close();
  }

  _fileStream.open(filename.c_str());

  return _fileStream.is_open();
}

RESULT Extractor::GetSymbols(SymbolTable& symbols) {
  _length.FromInt(0); // zero until .ORIG is found
                      // actual length starts at one
  int pos = 0;
  // store literals
  vector<int> literals;
  // relocation status
  bool relocatable = false;
  // beginning location
  Word begin(0);

  while (_fileStream.good()) {
    pos++; // next line

    string str;
    Line line;
    // get line, return error with line number if invalid
    getline(_fileStream, str);
    RESULT result = line.ReadLine(str);
    if (result.msg != SUCCESS) {
      result.info = _LineNumber(pos);
      return result;
    }

    if (! line.IsComment() ) {
      if (_length.ToInt() == 0) {
        if (line.Instruction() != ".ORIG") {
          return RESULT(ORIG);
        }
      }

      if (line.IsPseudoOp()) {
        if (line.Instruction() == ".ORIG") {
          // .ORIG
          if (! line.HasLabel()) {
            result.msg = REQ_LABEL;
            result.info = _LineNumber(pos) + ": .ORIG: ";
            return result;
          }
          
          if (line.Size() == 1) {
            if (! line[0][0] == 'x' ) {
              // not hex, error
              return RESULT(ORIG_HEX);
            } else {
              begin.FromHexAbbr(line[0]);
            }
          } else {
            relocatable = true;
          }
          
        } else if (line.Instruction() == ".END") {
          // .END, do nothing
        } else if (line.Instruction() == ".EQU") {
          // .EQU
          if (! line.HasLabel()) {
            result.msg = REQ_LABEL;
            result.info = _LineNumber(pos) + ": .EQU: ";
            return result;
          }
          if (symbols.Contains(line[0])) {
            Word w = symbols.GetLabelAddr(line[0]);
            symbols.InsertLabel(line.Label(), w, symbols.IsRelocatable(line[0]));
          } else if (line[0][0] == 'x') {
            Word w;
            w.FromHexAbbr(line[0]);
            symbols.InsertLabel(line.Label(), w);
          } else if (line[0][0] == '#') {
            Word w;
            w.FromInt(atoi(line[0].substr(1).c_str()));
            symbols.InsertLabel(line.Label(), w);
          } else {
            result.msg = LBL_NOT_FOUND;
            result.info = _LineNumber(pos) + ": " + line[0];
            return result;
          }
          
        } else if (line.Instruction() == ".FILL") {
          // .FILL
          Word w;
          bool hasvalue = true;
          switch(line[0][0]) {
            case 'x': {
              w.FromHexAbbr(line[0]);
            } break;

            case '#': {
              w.FromInt(atoi(line[0].substr(1).c_str()));
            } break;

            default: {
              if (symbols.Contains(line[0])) {
                w = symbols.GetLabelAddr(line[0]);
              } else {
                // don't know this value yet
                hasvalue = false;
              }
            } break;
          }
                    
          if (hasvalue && line.HasLabel()) {
            if (symbols.Contains(line.Label())) {
              result.msg = REDEF_LBL;
              result.info = _LineNumber(pos) + ": " + line.Label();
              return result;
            }
            symbols.InsertLabel(line.Label(), begin + _length);
          }
          _length++;
        } else if (line.Instruction() == ".STRZ") {
          // .STRZ
          if (line.HasLabel()) {
            if (symbols.Contains(line.Label())) {
              result.msg = REDEF_LBL;
              result.info = _LineNumber(pos) + ": " + line.Label();
              return result;
            }
            symbols.InsertLabel(line.Label(), begin + _length);
          }
          // adding on length of string (+1 for null termination)
          _length = _length + Word(line[0].length() + 1);
        } else if (line.Instruction() == ".BLKW") {
          // .BLKW
          if (line.HasLabel()) {
            if (symbols.Contains(line.Label())) {
              result.msg = REDEF_LBL;
              result.info = _LineNumber(pos) + ": " + line.Label();
              return result;
            }
            symbols.InsertLabel(line.Label(), begin + _length);
          }
          // get arugment
          Word w;
          switch(line[0][0]) {
            case 'x': {
              w.FromHexAbbr(line[0]);
            } break;
            case '#': {
              w.FromInt(atoi(line[0].substr(1).c_str()));
            } break;
            default: {
              if (symbols.Contains(line[0])) {
                w = symbols.GetLabelAddr(line[0]);
              } else {
                result.msg = LBL_NOT_FOUND;
                result.info = _LineNumber(pos) + ": " + line[0];
                return result;
              }
            } break;
          }
          _length = _length + w;
        }
      } else {
        // not a pseduo-op
        if (line.HasLabel()) {
          if (symbols.Contains(line.Label())) {
            result.msg = REDEF_LBL;
            result.info = _LineNumber(pos) + ": " + line.Label();
            return result;
          }
          symbols.InsertLabel(line.Label(), begin + _length, relocatable);
        }
        _length++;
      }
         

      if (line.HasLiteral()) {
        literals.push_back(line.Literal());
      }
    }
  }
  
  for (int i=0; i<literals.size(); i++) {
    symbols.InsertLiteral(literals[i], _length);
    _length++;
  }
}

Word Extractor::GetLength() const {
  return _length;
}

