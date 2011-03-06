// File:        Extractor.cpp
// Written by:  Andrew Groot

#include "../h/Extractor.h"
#include "../h/Word.h"
#include "../h/Line.h"
#include "../h/SymbolTable.h"
#include "../h/ResultCodes.h"
#include <cstdlib>
#include <string>
#include <map>
#include <set>
#include <fstream>
#include "../h/itos.h"
#include <iostream> 
using namespace std;
using namespace Codes;

// private
string Extractor::_LineNumber(int pos) {
  return ("Line " + itos(pos));
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
  set<int> literals;
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
      if (_length == -1) {
        if (line.Instruction() != ".ORIG" && line.Instruction() != ".MAIN") {
          return RESULT(ORIG);
        }
      }

      if (line.Instruction() == ".ORIG") {
        if (_length >= 0) {
          return RESULT(ORIG2);
        }
      }

      if (line.Instruction() == ".MAIN") {
        if (_length >= 0) {
          return RESULT(MAIN, _LineNumber(pos));
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
            return result;
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
          
          symbols.InsertLabel(line.Label(), begin, relocatable);
          _length++;

        } else if (line.Instruction() == ".MAIN") {
          // .MAIN
          // Do nothing, only affects output file

        } else if (line.Instruction() == ".END") {
          // .END
          if (line.HasLabel()) {
            // END cannot have label
            return RESULT(END_LBL);
          }

          Word w;
          if (line.Size() == 1) {
            // has argument
            if (symbols.IsSymbol(line[0])) {
              w = symbols.GetLabelAddr(line[0]);
            } else if (line[0][0] == 'x') {
              Word w;
              w.FromHexAbbr(line[0]);
            } else if (line[0][0] == '#') {
              Word w;
              w.FromInt(atoi(line[0].substr(1).c_str()));
            } else {
              result.msg = LBL_NOT_FOUND;
              result.info = _LineNumber(pos) + ": " + line[0];
              return result;
            }

            int ulimit = begin.ToInt() + _length;
            int llimit = begin.ToInt();
            if (w.ToInt() >= llimit && (w.ToInt() < ulimit)) {
              // Everything is good, done with first pass
              break;
            }
          } else {
            // No arugments, nothing to check or do
            // Everything is good, done with first pass
            break;
          }

          // out of bounds
          result.msg = END_OB;
          result.info = _LineNumber(pos);
          return result;

        } else if (line.Instruction() == ".EQU") {
          // .EQU
          if (! line.HasLabel()) {
            result.msg = REQ_LABEL;
            result.info = _LineNumber(pos) + ": .EQU: ";
            return result;
          }
          if (symbols.IsSymbol(line[0])) {
            Word w = symbols.GetLabelAddr(line[0]);
            if (symbols.LabelCount() <= _max_size) {
              symbols.InsertLabel(line.Label(), w, symbols.IsRelocatable(line[0]));
            } else {
              return RESULT(MAX_S_SIZE, _LineNumber(pos) + ": (MaxSymbolCount: " + itos(_max_size) + ")");
            }
          } else if (line[0][0] == 'x') {
            Word w;
            w.FromHexAbbr(line[0]);
            if (symbols.LabelCount() <= _max_size) {
              symbols.InsertLabel(line.Label(), w);
            } else {
              return RESULT(MAX_S_SIZE, _LineNumber(pos) + ": (MaxSymbolCount: " + itos(_max_size) + ")");
            }
          } else if (line[0][0] == '#') {
            Word w;
            w.FromInt(atoi(line[0].substr(1).c_str()));
            if (symbols.LabelCount() <= _max_size) {
              symbols.InsertLabel(line.Label(), w);
            } else {
              return RESULT(MAX_S_SIZE, _LineNumber(pos) + ": (MaxSymbolCount: " + itos(_max_size) + ")");
            }
          } else {
            result.msg = LBL_NOT_FOUND;
            result.info = _LineNumber(pos) + ": " + line[0];
            return result;
          }
          
        } else if (line.Instruction() == ".FILL") {
          // .FILL
          if (line.HasLabel()) {
            if (symbols.IsSymbol(line.Label())) {
              result.msg = REDEF_LBL;
              result.info = _LineNumber(pos) + ": " + line.Label();
              return result;
            }
            if (symbols.LabelCount() <= _max_size) {
              symbols.InsertLabel(line.Label(), begin + Word(_length), relocatable);
            } else {
              return RESULT(MAX_S_SIZE, _LineNumber(pos) + ": (MaxSymbolCount: " + itos(_max_size) + ")");
            }
          }
          _length++;

        } else if (line.Instruction() == ".STRZ") {
          // .STRZ
          if (line.HasLabel()) {
            if (symbols.IsSymbol(line.Label())) {
              result.msg = REDEF_LBL;
              result.info = _LineNumber(pos) + ": " + line.Label();
              return result;
            }
            if (symbols.LabelCount() <= _max_size) {
              symbols.InsertLabel(line.Label(), begin + Word(_length), relocatable);
            } else {
              return RESULT(MAX_S_SIZE, _LineNumber(pos) + ": (MaxSymbolCount: " + itos(_max_size) + ")");
            }
          }
          // adding on length of string
          // +1 for null termination
          _length += line[0].length() + 1;

        } else if (line.Instruction() == ".BLKW") {
          // .BLKW
          if (line.HasLabel()) {
            if (symbols.IsSymbol(line.Label())) {
              result.msg = REDEF_LBL;
              result.info = _LineNumber(pos) + ": " + line.Label();
              return result;
            }
            if (symbols.LabelCount() <= _max_size) {
              symbols.InsertLabel(line.Label(), begin + Word(_length), relocatable);
            } else {
              return RESULT(MAX_S_SIZE, _LineNumber(pos) + ": (MaxSymbolCount: " + itos(_max_size) + ")");
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
              if (symbols.IsSymbol(line[0])) {
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

      } else if (line.Instruction() == ".ENT") {
        // .ENT
        // Shouldn't occur in an absolute program
        if (!relocatable) {
          result.msg = RELATIVE;
          result.info = _LineNumber(pos);
          return result;
        }

      } else if (line.Instruction() == ".EXT") {
        // .EXT
        // Declare external labels for each argument
        for (int i = 0; i<line.Size(); i++) {
          symbols.AddExternal(line[i]);
        }
        // Shouldn't occur in an absolute program
        if (!relocatable) {
          result.msg = RELATIVE;
          result.info = _LineNumber(pos);
          return result;
        }

      } else {
        // not a pseduo-op
        if (line.HasLabel()) {
          if (symbols.IsSymbol(line.Label())) {
            result.msg = REDEF_LBL;
            result.info = _LineNumber(pos) + ": " + line.Label();
            return result;
          }
          if (symbols.LabelCount() <= _max_size) {
            symbols.InsertLabel(line.Label(), begin + Word(_length), relocatable);
          } else {
            return RESULT(MAX_S_SIZE, _LineNumber(pos) + ": (MaxSymbolCount: " + itos(_max_size) + ")");
          }
        }
        _length++;
      }

      if (line.HasLiteral()) {
        if (literals.size() <= (_max_size / 2)) {
          literals.insert(line.Literal());
        } else {
          return RESULT(MAX_L_SIZE, _LineNumber(pos) + ": (MaxLiteralCount: " + itos(_max_size / 2) + ")");
        }
      }
    }

    //--- various tests of program length
    if (relocatable) {
      int page_size = 0x1FF;
      if (_length > page_size ) {
        // must fit in one page
        return RESULT(REL_PG_SIZE, _LineNumber(pos) + ": Stopping at length " +
                                    itos(_length) + '(' + itoshex(_length) + ')');
      }
    }

    if ( _length >= (_max_size * 2) ) {
      // maximum size of footprint in memory
      return RESULT(MAX_LENGTH, _LineNumber(pos) + ": (MaxObjectFileSize: " +
                                  itos(_max_size * 2) + "(" + itoshex(_max_size * 2) + "))");
    }

    if ( begin.ToInt() + _length > Word::MAX_SIZE) {
      // begin is zero in relocatable program and
      // could be anything from 0 to MAX_SIZE otherwise

      // program will not fit in memory
      return RESULT(MEM_FIT, _LineNumber(pos) + ": Stopping at length " + itoshex(_length));
    }      
      

    //--- end of length related testing
  }
  
  if (_length < 1) {
    // shouldn't happen
    return RESULT(UNEXP_EOF);
  }
  
  set<int>::iterator it = literals.begin();
  while (it != literals.end()) {
    symbols.InsertLiteral(*it, begin + Word(_length++));
    it++;
  }

  return RESULT(SUCCESS);
}

Word Extractor::GetLength() const {
  return Word(_length);
}

