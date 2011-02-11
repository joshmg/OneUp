// File:        Printer.cpp
// Written by:  Ryan Paulson

#include "Printer.h"
#include "Word.h"
#include "Line.h"
#include "SymbolTable.h"
#include "ResultCodes.h"
using namespace std;
using namespace Codes;

// private

void Printer::_SetBits(Word &w, int value, int index);
  w = w.Or( Word(value*pow(2.0, index)) );
}

RESULT Printer::_IsReg(string reg) {
  if (reg.length() == 0 || reg[0] != 'R') {
    return RESULT(EMPTY_ARG);
  }

  if (reg[1] >= '0' && reg[1] <= '7') {
    return RESULT(SUCCESS);
  } else {
    return RESULT(INV_REG);
  }
}

int Printer::_RegNum(string reg) {
  return atoi(reg[1]);
}

RESULT Printer::_LineToWord(Line& line, SymbolTable& symbols, Word& w) {
  // do something different for each argument type
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
}

//*** public ***//

Printer::~Printer() {
  if (_infile.is_open()) {
    _inStream.close();
  }
  if (_outStream.is_open()) {
    _outStream.close();
  }
}

RESULT Printer::Open(string infile, string outfile) {
  //  Closes _fileStream if it is currently open.
  if (_inStream.is_open()) {
    _inStream.close();
  }
  if (_outStream.is_open()) {
    _outStream.close();
  }

  //  Opens the file defined by "name".
  _fileStream.open(infile);
  _fileStream.open(outfile)

  //  Determine whether the file was successfully opened and return the appropriate result code.
  if (_inStream.is_open()) {
    if (_outStream.is_open()) {
      return RESULT(SUCCESS);
    } else {
      return RESULT(outfile, FILE_NOT_OPENED);
  } else {
    return RESULT(infile, FILE_NOT_FOUND);
  }
}

RESULT Printer::Print(SymbolTable& symbols, Word& file_length) {
  
}
