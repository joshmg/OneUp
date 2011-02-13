// File:        Printer.cpp
// Written by:  Ryan Paulson

#include "Printer.h"
#include "Word.h"
#include "Line.h"
#include "SymbolTable.h"
#include "ResultCodes.h"
#include <string>
#include <cstdlib> // for atoi()
#include <cmath> // for pow()
using namespace std;
using namespace Codes;

// private
void Printer::_SetBits(Word& w, int value, int index) {
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
  return reg[1] - '0';
}

// This function can't actually be used but the skeleton could be useful.
/*
RESULT Printer::_LineToWord(Line line, SymbolTable& symbols, Word& w) {
  string inst = line.Instruction();

  // do something different for each argument type
  if (inst == "ADD" || inst == "AND") {
    if (line.Size() != 3) {
      return RESULT(ARG_SIZE);
    }

    // OP CODE
    if (inst == "ADD") {
      _SetBits(w, 1, 12);
    } else {
      // AND
      _SetBits(w, 3, 12);
    }

    // Arg 1
    RESULT result = _IsReg(line[0]);
    if (result.msg == SUCCESS) {
      _SetBits(w, _RegNum(line[0]), 9);
    } else {
      return result;
    }

    // Arg 2
    result = _IsReg(line[1]);
    if (result.msg == SUCCESS) {
      _SetBits(w, _RegNum(line[1]), 6);
    } else {
      return result;
    }

    // Arg3
    result = _IsReg(line[2]);
    if (result.msg == SUCCESS) {
      // set register bits
      _SetBits(w, _RegNum(line[2]), 0);
    } else {
      // not register
      result = _IsConstant(line[2], 5);
      if (result.msg == SUCCESS) {
        // set immediate bits
        _SetBits(w, _ReadConstant(line[2]), 0);
      } else {
        // not register or constant
        return RESULT(INV_ARG);
      }
    }
 
  } else if (inst == "DEBUG" || inst == "RET") {
    // just op code
    if (inst == "DEBUG") {
      _SetBits(w, 8, 12);
    } else {
      // RET
      _SetBits(w, 13, 12);
    }
  } else if (inst == "JSR" || inst == "JMP") {


  } else if (inst == "JSRR" || inst == "JMPR") {


  } else if (inst == "LD" || inst == "LDI" || inst == "LEA" || inst == "ST" || inst == "STI") {


  } else if (inst == "LDR" || inst == "STR") {


  } else if (inst == "NOT") {


  } else if (inst == "TRAP") {


  } else {
    // Handle psuedo ops
    if (inst == ".ORIG") {

    } else if (inst == ".END" || inst == ".EQU" || inst == ".FILL" || inst == ".STRZ" || inst == ".BLKW") {

    } else {
      // not a real instruction
      return RESULT(INV_INST);
    }
  }
}
*/

//*** public ***//

Printer::~Printer() {
  if (_inStream.is_open()) {
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
  _inStream.open(infile.c_str());
  _outStream.open(outfile.c_str());

  //  Determine whether the file was successfully opened and return the appropriate result code.
  if (_inStream.is_open()) {
    if (_outStream.is_open()) {
      return RESULT(SUCCESS);
    } else {
      return RESULT(FILE_NOT_OPENED, outfile);
    }
  } else {
    return RESULT(FILE_NOT_FOUND, infile);
  }
}

RESULT Printer::Print(SymbolTable& symbols, Word& file_length) {
  return RESULT(SUCCESS);  
}
