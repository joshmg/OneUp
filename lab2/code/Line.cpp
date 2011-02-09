#include "Line.h"
#include "Word.h"
#include "ResultCodes.h"
#include <string>
#include <cctype>
#include <cstdlib>
#include <cmath>
using namespace std;
using namespace Codes;

bool Line::IsWS(char ch) const {
  return (ch == ' ' || ch == '\t');
}

string Line::_GetNext(string& str) const {
  while ( str.length() > 0 && IsWS(str[0]) ) {
    str = str.substr(1);
  }

  string next = "";
  while ( str.length() > 0 && !IsWS(str[0]) ) {
    next += str[0];
    str = str.substr(1);
  }
  
  return next;
}

RESULT _CheckArgs() {
  if (arg.length() > 1) {
    if (arg[0] == '=') {
      if (_inst != "LD") {
        return RESULT(NON_LD_LIT);
      }
      _hasLiteral = true;
    }

    Word w;
    if (arg[0] == '#') {
      // take off # and convert string to integer to word
      if (w.FromInt(atoi(arg.substr(1)))) {
        // conversion successful
        
        // store literal, if one
        if (_hasLiteral) {
          w.FromInt(atoi(arg.substr(1)));
          _literal = w.ToInt();
        }
        return RESULT(SUCCESS);
      } else {
          // Not a decimal number
          return RESULT(INV_DEC);
      }
    } else if (arg[0] == 'x') {
      // take off x and convert hex string to word
      if (w.FromHexAbbr(arg.substr(1))) {
        // conversion successful
        
        // store literal, if one
        if (_hasLiteral) {
          _literal = w.ToInt();
        }
        return RESULT(SUCCESS);
      } else {
        // Not a hexadecimal number
        return RESULT(INV_HEX);
      }
    } else if (arg[0] == 'R') {
      // Register.  0-7?
      if (arg.length == 2 && isdigit(arg[1])) {
        int n = atoi(arg[1]);
        if (n >= 0 && n <= 7) {
          return SUCCESS;
        }
      }
      // invalid size or register number
      return RESULT(INV_REG);
    } else if (isalnum(arg[0])){
      // Label
      return RESULT(SUCCESS);
    } else {
      // Invalid
      return RESULT(INV_ARG);
    }
  } else {
    if (arg.length == 1 && isalnum(arg[0]) && arg[0] != 'R' && arg[0] != 'x') {
      // could be a label
      return RESULT(SUCCESS);
    }
  }

  // Empty or invalid
  return RESULT(INV_ARG);
}

RESULT Line::ReadLine (string line) {
  _code = line;

  if ( !IsWS(line[0]) ) {
    // begins with label
    if ( line[0] = ';' || line == "") {
      // Line contained only a comment or nothing
      _comment = true;
      return RESULT(SUCCESS);
    } else {
      // get label
      _label = _GetNext(line);
      if ( !isalnum(_label) || _label[0] == 'R' || _label[0] == 'x') {
        // Invalid label
        return RESULT(INVALID_LBL);
      }
    }
    // get instruction
    if (line.length() > 0) {
      _inst = _GetNext(line);
      if (_inst == "") {
        // whitespace after label
        return RESULT(LABEL_WO_INST);
      }
    } else {
      // label and then no instruction
      return RESULT(LABEL_WO_INST);
    }
  } else {
    // no label, get instruction
    _inst = _GetNext(line);
    if (_inst == "") {
      // Line contained only whitespace
      _comment = true;
      return RESULT(SUCCESS);
  }

  // Get aruguments, if any
  if (line.length() > 0) {
    // aruguments as one line, separated by commas
    string args = _GetNext(line);
    while (args.length() > 0 && args[0] != ';') {
      // not empty or comment
      string arg;
      while (args[0] != ',' && args[0] != ';' && args.length > 0) {
        // copy char by char
        arg += args[0];
        args.substr(1);
      }

      if (arg != "") {
        _args.push_back(arg);
      } else {
        // reached comma or semicolon immediately
        return RESULT(EMPTY_ARG);
      }
    }
  }

  // check number of arguments
  if (_inst == "DEBUG" || _inst == "RET") {
    if (args.size() != 0) {
      return RESULT(ARG_SIZE);
    }
  } else if (_inst == "JSR" || _inst == "JMP" || _inst == "TRAP" ||
              _inst == ".END" || _inst == ".EQU" || _inst == ".FILL" ||
              _inst == ".STRZ" || _inst == ".BLKW") {
    if (args.size() != 1) {
      return RESULT(ARG_SIZE);
    }
  } else if (_inst == "JSRR" || _inst == "JMPR" || _inst == "LD" ||
              _inst == "LDI" || _inst == "LEA" || _inst == "ST" ||
              _inst == "STI" || _inst == "NOT") {
    if (args.size() != 2) {
      return RESULT(ARG_SIZE);
    }
  } else if (_inst == "ADD" || _inst == "AND" || _inst == "LDR" || _inst == "STR") {
    if (args.size() != 3) {
      return RESULT(ARG_SIZE);
    }
  } else if (_inst == ".ORIG") {
    // could be zero or one
    if (args.size() > 2) {
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

string Line::ToString() const {
  return _code;
}

Word ToWord() const {
  return _word;
}

bool IsComment() {
  return _comment;
}

