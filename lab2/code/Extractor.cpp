// File:        Extractor.cpp
// Written by:  Andrew Groot

#include "Extractor.h"
#include "Word.h"
#include "SymbolTable.h"
#include "ResultCodes.h"
#include <string>
#include <ssteam> // for line numbers
#include <map>
#include <fstream>
using namespace std;
using namespace Codes;

// private


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
  _length.fromInt(0); // zero until .ORIG is found
                      // actual length starts at one
  int pos = 0;

  while (_fileStream.good()) {
    pos++; // next line

    string str;
    Line line;
    // get line, return error with line number if invalid
    _fileStream >> str;
    RESULT result = line.ReadLine(str);
    if (result.msg != SUCCESS) {
      stringstream s << "Line " << pos;
      result.info = s.str();
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
          if (! line.HasLabel()) {
            return RESULT(ORIG_LABEL);
          }

          if (line.Size() == 1) {
            if (!line[0] == 'x') {
              // not hex, error
              return RESULT(ORIG_HEX);
            }
          }
        } else if (line.Instruction() == ".END") {
          // do nothing
        } else if (line.Instruction() == ".EQU") {
          if (! line.HasLabel()) {
            return RESULT(EQU_LABEL);
          }
          if (symbols.Contains(line[0])) {
            Word w = symbols.GetLabel(line[0]);
            symbols.InsertLabel(line.Label(), w);
          } else if (line[0][0] == 'x') {
            Word w;
            w.FromHexAbbr(line[0].substr(1));
            symbols.InsertLabel(line.Label(), w);
          } else if (line[0][0] == '#') {
            Word w;
            w.FromInt(atoi(line[0].substr(1)));
            symbols.InsertLabel(line.Label(), w);
          } else {
            result.msg = LABEL_NOT_FOUND;
            stringstream s;
            s << "Line " << pos << ": " << line[0];
            result.info = s.str();
            return result;
          }
        } else if (line.Instruction() == ".FILL") {
          _length++;
        } else if (line.Instruction() == ".STRZ") {
          _length = _length + WORD(line[0].length() + 1);
        } else if (line.Instruction() == ".BLKW") {
          Word w;
          switch(line[0][0]) {
            'x': {
              w.FromHexAbbr(line[0].substr(1));
            } break;
            '#': {
              w.FromInt(atoi(line[0].substr(1)));
            } break;
            default: {
              if (symbols.Contains(line[0])) {
                w = symbols.GetLabel(line[0]);
              } else {
                result.msg = LABEL_NOT_FOUND;
                stringstream s;
                s << "Line " << pos << ": " << line[0];
                result.info = s.str();
                return result;
            } break;
          }
          _length = _length + w;
        }
      } else {
        // not a pseudo op
        _length++;
      }

      if (line.HasLabel()) {
        symbols.InsertLiteral(line.Label(), _length);
      }

      if (line.HasLiteral()) {
        symbols.InsertLiteral(line.Literal(), _length);
      }
    }
  }
}

Word Extractor::GetLength() {
  return _length;
}

