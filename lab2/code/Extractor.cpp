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
#include "itos.h"
using namespace std;
using namespace Codes;

// private
string Extractor::_LineNumber(int pos) {
  stringstream s;
  s << "Line " << pos;
  return s.str();
}

// public
Extractor::Extractor(int size) {
  _max_size = size;
}

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
  _length = -1; // -1 until .ORIG is found
                       // actual length starts at zero
  int pos = 0;
  // store literals
  vector<int> literals;
  // relocation status
  bool relocatable = false;
  // beginning location
  Word begin(0);

  while (_fileStream.good()) {
    pos++;  // next line
            // start at 1

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
      if (line.Instruction() == ".ORIG") {
        if (_length >= 0) {
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

          if (line.Label().length() > 6) {
            result.msg = ORIG_LBL;
            result.info = _LineNumber(pos);
          }
          
          if (line.Size() == 1) {
            if ( !(line[0][0] == 'x') ) {
              // not hex, error
              return RESULT(ORIG_HEX);
            } else {
              begin.FromHexAbbr(line[0]);
            }
          } else {
            relocatable = true;
          }

          _length++; 
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
            if (symbols.LabelCount() < _max_size) {
              symbols.InsertLabel(line.Label(), w, symbols.IsRelocatable(line[0]));
            } else {
              return RESULT(MAX_S_SIZE);
            }
          } else if (line[0][0] == 'x') {
            Word w;
            w.FromHexAbbr(line[0]);
            if (symbols.LabelCount() < _max_size) {
              symbols.InsertLabel(line.Label(), w);
            } else {
              return RESULT(MAX_S_SIZE);
            }
          } else if (line[0][0] == '#') {
            Word w;
            w.FromInt(atoi(line[0].substr(1).c_str()));
            if (symbols.LabelCount() < _max_size) {
              symbols.InsertLabel(line.Label(), w);
            } else {
              return RESULT(MAX_S_SIZE);
            }
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
            if (symbols.LabelCount() < _max_size) {
              symbols.InsertLabel(line.Label(), begin + Word(_length));
            } else {
              return RESULT(MAX_S_SIZE);
            }
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
            if (symbols.LabelCount() < _max_size) {
              symbols.InsertLabel(line.Label(), begin + Word(_length));
            } else {
              return RESULT(MAX_S_SIZE);
            }
          }
          // adding on length of string (+1 for null termination)
          _length += line[0].length() + 1;
        } else if (line.Instruction() == ".BLKW") {
          // .BLKW
          if (line.HasLabel()) {
            if (symbols.Contains(line.Label())) {
              result.msg = REDEF_LBL;
              result.info = _LineNumber(pos) + ": " + line.Label();
              return result;
            }
            if (symbols.LabelCount() < _max_size) {
              symbols.InsertLabel(line.Label(), begin + Word(_length), relocatable);
            } else {
              return RESULT(MAX_S_SIZE);
            }
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
          _length += w.ToInt();
        }
      } else {
        // not a pseduo-op
        if (line.HasLabel()) {
          if (symbols.Contains(line.Label())) {
            result.msg = REDEF_LBL;
            result.info = _LineNumber(pos) + ": " + line.Label();
            return result;
          }
          if (symbols.LabelCount() < _max_size) {
            symbols.InsertLabel(line.Label(), begin + Word(_length), relocatable);
          } else {
            return RESULT(MAX_S_SIZE);
          }
        }
        _length++;
      }

      if (line.HasLiteral()) {
        if (literals.size() < (_max_size / 2)) {
          literals.push_back(line.Literal());
        }
      }
    }
    if ( _length >= (_max_size * 2) || _length >= Word::MAX_SIZE ) {
      int max = _length & Word::MAX_SIZE;
      return RESULT(MAX_LENGTH, "(Max Length: " + itos(max) + ")");
    }
  }
  
  for (int i=0; i<literals.size(); i++) {
    symbols.InsertLiteral(literals[i], Word(_length));
    _length++;
  }

  return RESULT(SUCCESS);
}

Word Extractor::GetLength() const {
  return Word(_length);
}

