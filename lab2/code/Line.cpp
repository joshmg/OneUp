#include "Line.h"
#include "iWord.h"
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

void Line::_SetBits(Word &w, int value, int index);
  w = w.Or( Word(value*pow(2.0, index)) );
}

RESULT _IsReg(string reg) {
  if (reg.length() == 0 || reg[0] != 'R') {
    return RESULT(EMPTY_ARG);
  }

  if (reg[1] >= '0' && reg[1] <= '7') {
    return RESULT(SUCCESS);
  } else {
    return RESULT(INV_REG);
  }
}

int _RegNum(string reg) {
  return atoi(reg[1]);
}

RESULT ReadConstant(string constant, int size) {
  if (constant.length() > 0) {
    if (constant[0] == '=') {
      _hasLiteral = true;
    }

    Word w;
    if (constant[0] == '#') {
      // take off # and convert string to integer to word
      if (w.FromInt(atoi(constant.substr(1)))) {
        // conversion successful
        if (w.ToInt() < (int)pow(2.0, size-1)) {
          // store literal, if one
          if (_hasLiteral) {
            _literal = w.ToInt();
          }
          return RESULT(SUCCESS);
        } else {
          // out of range
          return RESULT(INV_CONST);
      } else {
        // out of range
        return RESULT(INV_DEC);
      }
    } else if (constant[0] == 'x') {
      // take off x and convert hex string to word
      if (w.FromHexAbbr(constant.substr(1))) {
        // conversion successful
        if (w.ToInt() < (int)pow(2.0, size-1)) {
          // store literal, if one
          if (_hasLiteral) {
            _literal = w.ToInt();
          }
          return RESULT(SUCCESS);
        } else {
          // out of range
          return RESULT(INV_CONST);
        }
      } else {
        return RESULT(INV_HEX);
      }
    }
  }
  // empty string / not =, #, or x
  return RESULT(EXP_CONST);
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

  // Get arugument, if any
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

  // argument types
  Word w;
  if (_inst == "ADD" || _inst == "AND") {
    if (args.size() != 3) {
      return RESULT(ARG_SIZE);
    }

    // OP CODE
    if (_inst == "ADD") {
      _SetBits(w, 1, 12);
    } else {
      // AND
      _SetBits(w, 3, 12);
    }

    // Arg 1
    RESULT result = _IsReg(arg[0]);
    if (result.msg == SUCCESS) {
      _SetBits(w, _RegNum(args[0], 9));
    } else {
      return arg1;
    }

    // Arg 2
    result = _IsReg(arg[1]);
    if (result.msg == SUCCESS) {
      _SetBits(w, _RegNum(args[1], 6));
    } else {
      return result;
    }

    // Arg3
    result = _IsReg(arg[2]);
    if (result.msg == SUCCESS) {
      // set register bits
      _SetBits(w, _RegNum(arg[2]), 0);
    } else {
      // not register
      result = _IsConstant(arg[2], 5);
      if (result.msg == SUCCESS) {
        // set immediate bits
        _SetBits(w, _ReadConstant(arg[2]), 0);
      } else {
        // not register or constant
        return RESULT(INV_ARG);
      }
    }

  } else if (_inst == "DEBUG" || _inst == "RET") {
    // just op code
    if (_inst == "DEBUG") {
      _SetBits(w, 8, 12);
    } else {
      // RET
      _SetBits(w, 13, 12);
    }
  } else if (_inst == "JSR" || _inst == "JMP") {


  } else if (_inst == "JSRR" || _inst == "JMPR") {


  } else if (_inst == "LD" || _inst == "LDI" || _inst == "LEA" || _inst == "ST" || _inst == "STI") {
    

  } else if (_inst == "LDR" || _inst == "STR") {


  } else if (_inst == "NOT") {


  } else if (_inst == "TRAP") {


  } else {
    // Handle psuedo ops
    if (_inst == ".ORIG") {

    } else if (_inst == ".END" || _inst == ".EQU" || _inst == ".FILL" || _inst == ".STRZ" || _inst == ".BLKW") {
    
    } else {
      // not a real instruction
      return RESULT(INV_INST);
    }
  }

  return SUCCESS;
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

iWord& ToWord() const {
  return _word;
}

bool IsComment() {
  return _comment;
}

