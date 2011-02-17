#include "Line.h"
#include "Word.h"
#include "ResultCodes.h"
#include <string>
#include <cctype>
#include <cstdlib>
#include <cmath>
using namespace std;
using namespace Codes;

#include <iostream>

bool Line::_IsWS(char ch) const {
  return (ch == ' ' || ch == '\t');
}

string Line::_GetNext(string& str) const {
  while ( str.length() > 0 && _IsWS(str[0]) ) {
    str = str.substr(1);
  }

  string next = "";
  while ( str.length() > 0 && !_IsWS(str[0]) ) {
    next += str[0];
    str = str.substr(1);
  }
  
  return next;
}

RESULT Line::_CheckArgs() {
  for (int i=0; i<_args.size(); i++) {
    if (_args[i][0] == '=') {
      if (_inst != "LD") {
        return RESULT(NON_LD_LIT);
      }
      _hasLiteral = true;
    }

    Word w;
    if (_args[i][(_args[i][0] == '=')] == '#') {
      // take off # and convert string to integer to word
      if (w.FromInt(atoi(_args[i].substr(1 + (_args[i][0] == '=')).c_str()))) {
        // conversion successful
        // store literal, if one
        if (_hasLiteral) {
          _literal = w.ToInt2Complement();
        }
        // valid decimal
        continue;
      } else {
        // Not a decimal number
        return RESULT(INV_DEC);
      }
    } else if (_args[i][(_args[i][0] == '=')] == 'x') {
      // take off x and convert hex string to word
      if (w.FromHexAbbr(_args[i].substr(_args[i][0] == '=').c_str())) {
        // conversion successful
        
        // store literal, if one
        if (_hasLiteral) {
          _literal = w.ToInt2Complement();
        }
        // valid hex
        continue;
      } else {
        // Not a hexadecimal number
        return RESULT(INV_HEX);
      }
    } else if (_args[i][0] == 'R') {
      // Register.  0-7?
      if (_args[i].length() == 2 && isdigit(_args[i][1])) {
        int n = _args[i][1] - '0';
        if (n >= 0 && n <= 7) {
          // good arg
          continue;
        }
      } else {
        // invalid size or number
        return RESULT(INV_REG);
      }
    } else if (isalnum(_args[i][0])){
      // Label, good arg
      continue;
    } else {
      // Invalid
      return RESULT(INV_ARG);
    }
  }

  return RESULT(SUCCESS);
}

RESULT Line::ReadLine (string line) {
  // get code
  _code = line;

  // reset important values
  _hasLabel = false;
  _hasLiteral = false;
  _comment = false;
  _args.clear();

  if (line.length() == 0) {
    _comment = true;
    return RESULT(SUCCESS);
  }

  if (! _IsWS(line[0]) ) {
    // begins with label
    if ( line[0] == ';') {
      // Line contained only a comment or nothing
      _comment = true;
      return RESULT(SUCCESS);
    } else {
      // get label
      _label = _GetNext(line);
      if ( !isalnum(_label[0]) || _label[0] == 'R' || _label[0] == 'x') {
        // Invalid label
        return RESULT(INV_LBL);
      }
      _hasLabel = true;
    }
    // get instruction
    if (line.length() > 0) {
      _inst = _GetNext(line);
      if (_inst.length() == 0) {
        // whitespace after label
        return RESULT(LBL_WO_INST);
      }
    } else {
      // label and then no instruction
      return RESULT(LBL_WO_INST);
    }
  } else {
    // no label, get instruction
    _inst = _GetNext(line);
    if (_inst.length() == 0 || _inst[0] == ';') {
      // Line contained only whitespace or comment
      _comment = true;
      return RESULT(SUCCESS);
    }
  }
  
  // Get aruguments
  if (_inst == ".STRZ") {
    // special case as .STRZ args may contain whitespace
    // remove whitespace
    while (_IsWS(line[0])) {
      line = line.substr(1);
    }
    // get quote
    if (line[0] != '\"') {
      return RESULT(STRZ_NOT_STR);
    } else {
      // move " to arg
      string arg;
      arg += line[0];
      line = line.substr(1);

      while (line.length() > 0) {
        arg += line[0];
        if (line[0] == '\"') {
          // end of string
          break;
        } else {
          // keep going
          line = line.substr(1);
        }
      }
      
      // store arg
      _args.push_back(arg);

      if (line.length() == 0) {
        // no end quote
        return RESULT(END_OF_STR);
      }
    }
  } else if (line.length() > 0) {
    // _inst != .STRZ
    // aruguments as one line, separated by commas
    string args = _GetNext(line);
    while (args.length() > 0 && args[0] != ';') {
      // not empty or comment
      string arg;
      while (args.length() > 0 && args[0] != ',' && args[0] != ';') {
        // copy char by char
        arg += args[0];
        args = args.substr(1);
      }
      
      if (arg.length() > 0) {
        _args.push_back(arg);

        if (args.length() > 0 && args[0] == ',') {
          // get rid of comma
          args = args.substr(1);
        }
      } else {
        // reached comma or semicolon immediately
        return RESULT(EMPTY_ARG);
      }
    }
  }

  // check number of arguments
  if (_inst.length() > 1 && _inst[0] == 'B' && _inst[1] == 'R') {
    // make sure valid branch
    for (int i = 2; i < _inst.length(); i++) {
      if (_inst[i] != 'P' && _inst[i] != 'N' && _inst[i] != 'Z') {
        return RESULT(INV_BR);
      }
    }
    // check for 1 arg
    if (_args.size() != 1) {
      return RESULT(ARG_SIZE);
    }
  } else if (_inst == "DEBUG" || _inst == "RET") {
    if (_args.size() != 0) {
      return RESULT(ARG_SIZE);
    }
  } else if (_inst == "JSR" || _inst == "JMP" || _inst == "TRAP" ||
              _inst == ".END" || _inst == ".EQU" || _inst == ".FILL" ||
              _inst == ".STRZ" || _inst == ".BLKW") {
    if (_args.size() != 1) {
      return RESULT(ARG_SIZE);
    }
  } else if (_inst == "JSRR" || _inst == "JMPR" || _inst == "LD" ||
              _inst == "LDI" || _inst == "LEA" || _inst == "ST" ||
              _inst == "STI" || _inst == "NOT") {
    if (_args.size() != 2) {
      return RESULT(ARG_SIZE);
    }
  } else if (_inst == "ADD" || _inst == "AND" || _inst == "LDR" || _inst == "STR") {
    if (_args.size() != 3) {
      return RESULT(ARG_SIZE);
    }
  } else if (_inst == ".ORIG") {
    // could be zero or one
    if (_args.size() > 1) {
      return RESULT(ARG_SIZE);
    }
  } else {
    // not a real instruction
    return RESULT(INV_INST);
  }

  // check arguments for validity
  return _CheckArgs();
}

string Line::Label() const {
  return _label;
}

string Line::Instruction() const {
  return _inst;
}

string Line::operator[] (int index) const {
  return _args[index];
}

int Line::Size() const {
  return _args.size();
}

int Line::Literal() const {
  return _literal;
}


string Line::ToString() const {
  return _code;
}


bool Line::HasLabel() const {
  return _hasLabel;
}

bool Line::IsPseudoOp() const {
  return _inst[0] == '.';
}

bool Line::HasLiteral() const {
  return _hasLiteral;
}

bool Line::IsComment() const {
  return _comment;
}

