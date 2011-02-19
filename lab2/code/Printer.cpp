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

void Printer::_SetBits(std::string reg, Word& initial_mem, int& bit_offset) {
  switch (reg[1]) {
    case "0": for (int i = 0; i < 3; i++) {
                initial_mem.setBit(bit_offset, false);
                bit_offset--;
              }
              break;
    case "1": for (int i = 0; i < 3; i++) {
                if (i != 2) {
                  initial_mem.setBit(bit_offset, false);
                } else {
                  initial_mem.setBit(bit_offset, true);
                }
                bit_offset--;
              }
              break;
    case "2": for (int i = 0; i < 3; i++) {
                if (i != 1) {
                  initial_mem.setBit(bit_offset, false);
                } else {
                  initial_mem.setBit(bit_offset, true);
                }
                bit_offset--;
              }
              break;
    case "3": for (int i = 0; i < 3; i++) {
                if (i == 0) {
                  initial_mem.setBit(bit_offset, false);
                } else {
                  initial_mem.setBit(bit_offset, true);
                }
                bit_offset--;
              }
              break;
    case "4": for (int i = 0; i < 3; i++) {
                if (i != 0) {
                  initial_mem.setBit(bit_offset, false);
                } else {
                  initial_mem.setBit(bit_offset, true);
                }
                bit_offset--;
              }
              break;
    case "5": for (int i = 0; i < 3; i++) {
                if (i == 1) {
                  initial_mem.setBit(bit_offset, false);
                } else {
                  initial_mem.setBit(bit_offset, true);
                }
                bit_offset--;
              }
              break;
    case "6": for (int i = 0; i < 3; i++) {
                if (i == 2) {
                  initial_mem.setBit(bit_offset, false);
                } else {
                  initial_mem.setBit(bit_offset, true);
                }
                bit_offset--;
              }
              break;
    case "7": for (int i = 0; i < 3; i++) {
                initial_mem.setBit(bit_offset, true);
                bit_offset--;
              }
              break;
  }
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

RESULT Printer::Print(SymbolTable& symbols, Word& file_length, Word& initial_load) {
  string input_line;
  Word current_address = initial_load;

  while (_inStream.good()) {
    getline(_inStream, input_line);

    Line current_line = Line::ReadLine(input_line);
    int bit_offset = 11;

    if (! current_line.IsComment()) {
      string inst = current_line.Instruction();


      if (strcmp(inst, ".ORIG") == 0) {
        // Header record
        _outStream << "H" << current_line.Label();

        string str = initial_load.ToHex();
        // Strip the 0x and print the initial load address
        _outStream << str.substr(2,4) << "\n";

        str = file_length.ToHex();
        // Strip the 0x and print the segment length
        _outStream << str.substr(2,4) << "\n";
      } else if (strcmp(inst, ".BLKW") == 0) {
        // Block pseudo-op
        string op = current_line[0];

        int value = 0;

        if (op[0] == "#") {
          // Decimal operand
          bool neg = false;

          if(op[1] == "-") {
            neg = true;
          }
          if (neg) {
            for (int i = 2; i < op.length(); i++) {
              value *= 10;
              value += op[i] - '0';
            }
            value *= -1;
          }
          else {
            for (int i = 1; i < op.length(); i++) {
              value *= 10;
              value += op[i] - '0';
            }
          }
        }
        else if (op[0] == "x") {
          // Hex operand
          Word temp = new Word();
          temp.FromHexAbbr(op.substr(1));

          value = temp.ToInt();
        }

        while (value > 0) {
          // Text Records
          _outStream << "T";

          // Print address to be reserved
          _outStream << current_address.ToHex().substr(2,4);
          current_address++;

          // Print a non-initialization
          _outStream << "0000";

          _outStream << "\n";

          value--;
        }
      } else if (strcmp(inst, ".FILL") == 0) {
        // Fill pseudo-op
        string op = current_line[0];

        Word value = new Word();

        if (op[0] == "#") {
          // Decimal operand
          int temp = 0;
          bool neg = false;

          if(op[1] == "-") {
            neg = true;
          }

          if(neg) {
            for (int i = 2; i < op.length(); i++) {
              temp *= 10;
              temp += op[i] - '0';
            }
            temp *= -1;
          }
          else {
            for (int i = 1; i < op.length(); i++) {
              temp *= 10;
              temp += op[i] - '0';
            }
          }

          value = new Word(temp);
        } else if (op[0] == "x") {
          // Hex operand
          value.FromHexAbbr(op.substr(1));
        }

        // Text Record
        _outStream << "T";

        // Print address to be initialized
        _outStream << current_address.ToHex().substr(2,4);
        current_address++;

        // Print initial memory value
        _outStream << value.ToHex().substr(2,4);

        _outStream << "\n";
      } else if (strcmp(inst, ".STRZ") == 0) {
        // String pseudo-op
        string op = current_line[0];

        for (int i = 0; i < op.length(); i++) {
          Word character = new Word(atoi(op[i]);

          // Text Record
          _outStream << "T";

          // Print the address of the current character
          _outStream << current_address.ToHex().substr(2,4);
          current_address++;

          // Print the hex representing the current character
          _outStream << character.ToHex().substr(2,4);

          _outStream << "\n";
        }

        // Text record for null character at string termination
        _outStream << "T" << current_address.ToHex().substr(2,4); << "0000";
        current_address++;

        _outStream << "\n";
      } else if (strcmp(inst, "ADD") == 0) {
        // ADD instruction
        // Text Record
        _outStream << "T";

        // Print address to be initialized
        _outStream << current_address.ToHex().substr(2,4);
        current_address++;

        // **Begin parsing instruction**
        Word initial_mem = new Word();

        // Set bits for opcode
        initial_mem.setBit(12, true);

        // Set bits for destination register
        _SetBits(current_line[0], initial_mem, bit_offset);

        // Set bits for source register 1
        _SetBits(current_line[1], initial_mem, bit_offset);

        string op3 = current_line[2];

        if (symbols.Contains(op3)) {
          // Set appropriate bits in initial_mem to match appropriate symbol value
          Word value = symbols.getLabelAddr(op3);

          for (int i = 4; i >= 0; i--;) {
            initial_mem.setBit(i, value[i]);
          }
        }else if (op3[0] == "R") {
          // Set next 3 bits to 0
          _SetBits("R0", initial_mem, bit_offset);

          // Set bits for source register 2
          _SetBits(op3, initial_mem, bit_offset);
        } else {
          initial_mem.setBit(bit_offset, true);
          bit_offset--;

          if (op3[0] == "#") {
            // Decimal operand
            int value = 0;
            bool neg = false;

            if(op3[1] == '-') {
              neg = true;
            }

            if(neg) {
              for (int i = 2; i < op3.length(); i++) {
                value *= 10;
                value += op3[i] - '0';
              }
              value *= -1;
            }
            else {
              for (int i = 1; i < op3.length(); i++) {
                value *= 10;
                value += op3[i] - '0';
              }
            }

            Word temp = new Word(value);

            for (int i = 4; i >= 0; i--;) {
              initial_mem.setBit(i, temp[i]);
            }
          } else if (op3[0] == "x") {
            // Hex operand
            Word temp = new Word();
            temp.FromHexAbbr(op3.substr(1));

            for( int i = 4; i >= 0; i--;) {
              initial_mem.setBit(i, temp[i]);
            }
          }
        }
        // **End parsing instruction**

        // Print the instruction in hex
        _outStream << initial_mem.ToHex().substr(2,4);

        _outStream << "\n";
      } else if (strcmp(inst, "AND") == 0) {
        // AND instruction
        // Text Record
        _outStream << "T";

        // Print address to be initialized
        _outStream << current_address.ToHex().substr(2,4);
        current_address++;

        // **Begin parsing instruction**
        Word initial_mem = new Word();

        // Set bits for opcode
        initial_mem.setBit(14, true);
        initial_mem.setBit(12, true);

        // Set bits for destination register
        _SetBits(current_line[0], initial_mem, bit_offset);

        // Set bits for source register 1
        _SetBits(current_line[1], initial_mem, bit_offset);

        string op3 = current_line[2];

        if (symbols.Contains(op3)) {
          // Set appropriate bits in initial_mem to match appropriate symbol value
          Word value = symbols.getLabelAddr(op3);

          for (int i = 4; i >= 0; i--;) {
            initial_mem.setBit(i, value[i]);
          }
        }else if (op3[0] == "R") {
          // Set next 3 bits to 0
          _SetBits("R0", initial_mem, bit_offset);

          // Set bits for source register 2
          _SetBits(op3, initial_mem, bit_offset);
        } else {
          initial_mem.setBit(bit_offset, true);
          bit_offset--;

          if (op3[0] == "#") {
            // Decimal operand
            int value = 0;
            bool neg = false;

            if(op3[1] == '-') {
              neg = true;
            }

            if(neg) {
              for (int i = 2; i < op3.length(); i++) {
                value *= 10;
                value += op3[i] - '0';
              }
              value *= -1;
            }
            else {
              for (int i = 1; i < op3.length(); i++) {
                value *= 10;
                value += op3[i] - '0';
              }
            }

            Word temp = new Word(value);

            for (int i = 4; i >= 0; i--;) {
              initial_mem.setBit(i, temp[i]);
            }
          } else if (op3[0] == "x") {
            // Hex operand
            Word temp = new Word();
            temp.FromHexAbbr(op3.substr(1));

            for (int i = 4; i >= 0; i--;) {
              initial_mem.setBit(i, temp[i]);
            }
          }
        }
        // **End parsing instruction**

        // Printthe instruction in hex
        _outStream << initial_mem.ToHex().substr(2,4);

        _outStream << "\n";
      } else if (strcmp(inst, "JSR") == 0) {
        // JSR Instruction
        // Text Record
        _outStream << "T";

        // Print the address to be initialized
        _outStream << current_address.ToHex().substr(2,4);
        current_address++;

        // **Begin parsing instruction**
        Word initial_mem = new Word();

        // Set bits for opcode
        initial_mem.setBit(14, true);

        // Set link bit
        // **To be changed, currently sets to 0**
        _SetBits("R0", initial_mem, bit_offset);

        string op = current_line[0];

        if (symbols.Contains(op)) {
          Word temp = symbols.GetLabelAddr(op);

          while (bit_offset >= 0) {
            initial_mem.setBit(bit_offset, temp[bit_offset]);
            bit_offset--;
          }
        } else {
          if (op3[0] == "#") {
            // Decimal operand
            int value = 0;
            bool neg = false;

            if(op3[1] == '-') {
              neg = true;
            }

            if(neg) {
              for (int i = 2; i < op3.length(); i++) {
                value *= 10;
                value += op3[i] - '0';
              }
              value *= -1;
            }
            else {
              for (int i = 1; i < op3.length(); i++) {
                value *= 10;
                value += op3[i] - '0';
              }
            }

            Word temp = new Word(value);

            while (bit_offset >= 0) {
              initial_mem.setBit(bit_offset, temp[bit_offset]);
              bit_offset--;
            }
          } else if (op3[0] == "x") {
            // Hex operand
            Word temp = new Word();
            temp.FromHexAbbr(op3.substr(1));

            while (bit_offset >= 0) {
              initial_mem.setBit(bit_offset, temp[bit_offset]);
              bit_offset--;
            }
          }
        }
        // **End parsing instruction**

        _outStream << initial_mem.ToHex().substr(2,4);

        _outStream << "\n";
      } else if (strcmp(inst, "JSRR") == 0) {
        // JSRR Instruction
        // Text Record
        _outStream << "T";

        _outStream << current_address.ToHex.substr(2,4);
        current_address++;

        // **Begin parsing instruction**
        Word initial_mem = new Word();

        // Set bits for opcode
        initial_mem.setBit(15, true);
        initial_mem.setBit(14, true);

        // Set link bit
        // **To be changed, currently sets to 0**
        _SetBits("R0", initial_mem, bit_offset);

        // Set bits for base register
        _SetBits(current_line[0], initial_mem, bit_offset);

        string op2 = current_line[1];

        if (symbols.Contains(op2)) {
          Word temp = symbols.GetLabelAddr(op2);

          while (bit_offset >= 0) {
            initial_mem.setBit(bit_offset, temp[bit_offset]);
            bit_offset--;
          }
        } else if (op2[0] == "#") {
          // Decimal operand
          int value = 0;
          bool neg = false;

          if(op2[1] == '-') {
            neg = true;
          }

          if(neg) {
            for (int i = 2; i < op2.length(); i++) {
              value *= 10;
              value += op2[i] - '0';
            }
            value *= -1;
          }
          else {
            for (int i = 1; i < op2.length(); i++) {
              value *= 10;
              value += op2[i] - '0';
            }
          }

          Word temp = new Word(value);

          while (bit_offset >= 0) {
            initial_mem.setBit(bit_offset, temp[bit_offset]);
            bit_offset--;
          }
        } else if (op2[0] == "x") {
          // Hex operand
          Word temp = new Word();
          temp.FromHexAbbr(op2.substr(1));

          while (bit_offset >= 0) {
            initial_mem.setBit(bit_offset, temp[bit_offset]);
            bit_offset--;
          }
        }
        // **End parsing instruction**

        _outStream << initial_mem.ToHex().substr(2,4);

        _outStream << "\n";
      } else if (strcmp(inst, "LD")) {
        // LD Instruction
        // Text Record
        _outStream << "T";

        // Print the address to be initialized
        _outStream << current_address.ToHex().substr(2,4);
        current_address++;

        // **Begin parsing instruction**
        Word initial_mem = new Word();

        // Set bits for opcode
        initial_mem.setBit(13, true);

        // Set bits for destination register
        _SetBits(current_line[0], initial_mem, bit_offset);

        string op2 = current_line[1];

        if (current_line.HasLiteral()) {
          Word temp = symbols.GetLiteralAddr(current_line.Literal());

          while (bit_offset >= 0) {
            initial_mem.setBit(bit_offset, temp[bit_offset]);
          }
        } else if (symbols.Contains(op2)) {
          Word temp = symbols.GetLabelAddr(op2);

          while (bit_offset >= 0) {
            initial_mem.setBit(bit_offset, temp[bit_offset]);
          }
        } else if (op2[0] == "#") {
          // Decimal operand
          int value = 0;
          bool neg = false;

          if(op2[1] == '-') {
            neg = true;
          }

          if(neg) {
            for (int i = 2; i < op2.length(); i++) {
              value *= 10;
              value += op2[i] - '0';
            }
            value *= -1;
          }
          else {
            for (int i = 1; i < op2.length(); i++) {
              value *= 10;
              value += op2[i] - '0';
            }
          }

          Word temp = new Word(value);

          while (bit_offset >= 0) {
            initial_mem.setBit(bit_offset, temp[bit_offset]);
            bit_offset--;
          }
        } else if (op2[0] == "x") {
          // Hex operand
          Word temp = new Word();
          temp.FromHexAbbr(op2.substr(1));

          while (bit_offset >= 0) {
            initial_mem.setBit(bit_offset, temp[bit_offset]);
            bit_offset--;
          }
        }
        // **End parsing instruction**

        _outStream << initial_mem.ToHex().substr(2,4);

        _outStream << "\n";
      } else if (strcmp(inst, "LDI") == 0) {
        // LDI Instruction
        // Text Record
        _outStream << "T";

        // Print the address to be initialized
        _outStream << current_address.ToHex().substr(2,4);
        current_address++;

        // **Begin parsing instruction**
        Word initial_mem = new Word();

        // Set bits for opcode
        initial_mem.setBit(15, true);
        initial_mem.setBit(13, true);

        // Set bits for destination register
        _SetBits(current_line[0], initial_mem, bit_offset);

        string op2 = current_line[1];

        if (current_line.HasLiteral()) {
          Word temp = symbols.GetLiteralAddr(current_line.Literal());

          while (bit_offset >= 0) {
            initial_mem.setBit(bit_offset, temp[bit_offset]);
          }
        } else if (symbols.Contains(op2)) {
          Word temp = symbols.GetLabelAddr(op2);

          while (bit_offset >= 0) {
            initial_mem.setBit(bit_offset, temp[bit_offset]);
          }
        } else if (op2[0] == "#") {
          // Decimal operand
          int value = 0;
          bool neg = false;

          if(op2[1] == '-') {
            neg = true;
          }

          if(neg) {
            for (int i = 2; i < op2.length(); i++) {
              value *= 10;
              value += op2[i] - '0';
            }
            value *= -1;
          }
          else {
            for (int i = 1; i < op2.length(); i++) {
              value *= 10;
              value += op2[i] - '0';
            }
          }

          Word temp = new Word(value);

          while (bit_offset >= 0) {
            initial_mem.setBit(bit_offset, temp[bit_offset]);
            bit_offset--;
          }
        } else if (op2[0] == "x") {
          // Hex operand
          Word temp = new Word();
          temp.FromHexAbbr(op2.substr(1));

          while (bit_offset >= 0) {
            initial_mem.setBit(bit_offset, temp[bit_offset]);
            bit_offset--;
          }
        }
        // **End parsing instruction**

        _outStream << initial_mem.ToHex().substr(2,4);

        _outStream << "\n";
      } else if (strcmp(inst, "LDR") == 0) {
        // LDR Instruction
        // Text Record
        _outStream << "T";

        // Print the address to be initialized
        _outStream << current_address.ToHex().substr(2,4);
        current_address++;

        // **Begin parsing instruction**
        Word initial_mem = new Word();

        // Set bits for opcode
        initial_mem.setBit(14, true);
        initial_mem.setBit(13, true);

        // Set bits for destination register
        _SetBits(current_line[0], initial_mem, bit_offset);

        // Set bits for base register
        _SetBits(current_line[1], initial_mem, bit_offset);

        string op3 = current_line[2];

        if (symbols.Contains(op3)) {
          Word temp = symbols.GetLabelAddr(op3);

          while (bit_offset >= 0) {
            initial_mem.setBit(bit_offset, temp[bit_offset]);
          }
        } else if (op3[0] == "#") {
          // Decimal operand
          int value = 0;
          bool neg = false;

          if(op3[1] == '-') {
            neg = true;
          }

          if(neg) {
            for (int i = 2; i < op3.length(); i++) {
              value *= 10;
              value += op3[i] - '0';
            }
            value *= -1;
          }
          else {
            for (int i = 1; i < op3.length(); i++) {
              value *= 10;
              value += op3[i] - '0';
            }
          }

          Word temp = new Word(value);

          while (bit_offset >= 0) {
            initial_mem.setBit(bit_offset, temp[bit_offset]);
            bit_offset--;
          }
        } else if (op3[0] == "x") {
          // Hex operand
          Word temp = new Word();
          temp.FromHexAbbr(op3.substr(1));

          while (bit_offset >= 0) {
            initial_mem.setBit(bit_offset, temp[bit_offset]);
            bit_offset--;
          }
        }
        // **End parsing instruction**

        _outStream << initial_mem.ToHex().substr(2,4);

        _outStream << "\n";
      } else if (strcmp(inst, "LEA") == 0) {
        // LEA Instruction
        // Text Record
        _outStream << "T";

        // Print the address to be initialized
        _outStream << current_address.ToHex().substr(2,4);

        // **Begin parsing instruction**
        Word initial_mem = new Word();

        // Set bits for opcode
        initial_mem.setBit(15, true);
        initial_mem.setBit(14, true);
        initial_mem.setBit(13, true);

        // Set bits for destination register
        _SetBits(current_line[0], initial_mem, bit_offset);

        string op2 = current_line[1];

        if (current_line.HasLiteral()) {
          Word temp = symbols.GetLiteralAddr(current_line.Literal());

          while (bit_offset >= 0) {
            initial_mem.setBit(bit_offset, temp[bit_offset]);
            bit_offset--;
          }
        } else if (symbols.Contains(op2)) {
          Word temp = symbols.GetLabelAddr(op2);

          while (bit_offset >= 0) {
            initial_mem.setBit(bit_offset, temp[bit_offset]);
            bit_offset--;
          }
        } else if (op2[0] == "#") {
          // Decimal operand
          int value = 0;
          bool neg = false;

          if(op2[1] == '-') {
            neg = true;
          }

          if(neg) {
            for (int i = 2; i < op2.length(); i++) {
              value *= 10;
              value += op2[i] - '0';
            }
            value *= -1;
          }
          else {
            for (int i = 1; i < op2.length(); i++) {
              value *= 10;
              value += op2[i] - '0';
            }
          }

          Word temp = new Word(value);

          while (bit_offset >= 0) {
            initial_mem.setBit(bit_offset, temp[bit_offset]);
            bit_offset--;
          }
        } else if (op3[0] == "x") {
          // Hex operand
          Word temp = new Word();
          temp.FromHexAbbr(op2.substr(1));

          while (bit_offset >= 0) {
            initial_mem.setBit(bit_offset, temp[bit_offset]);
            bit_offset--;
          }
        }
        // **End parsing instruction**

        _outStream << initial_mem.ToHex().substr(2,4);

        _outStream << "\n";
      } else if (strcmp(inst, "NOT") == 0) {
        // NOT Instruction
        // Text Record
        _outStream << "T";

        // Print the address to be initialized
        _outStream << current_address.ToHex().substr(2,4);
        current_address++;

        // **Begin parsing instruction**
        Word initial_mem = new Word();

        // Set bits for opcode
        initial_mem.setBit(15, true);
        initial_mem.setBit(12, true);

        // Set bits for destination register
        _SetBits(current_line[0], initial_mem, bit_offset);

        // Set bits for source register
        _SetBits(current_line[1], initial_mem, bit_offset);

        // **End parsing instruction**

        _outStream << initial_mem.ToHex().substr(2,4);

        _outStream << "\n";
      } else if (strcmp(inst, "RET") == 0) {
        // RET Instruction
        // Text Record
        _outStream << "T";

        // Print the address to be initialized
        _outStream << current_address.ToHex().substr(2,4);
        current_address++;

        // **Begin parsing instruction**
        Word initial_mem = new Word();

        // Set bits for opcode
        initial_mem.setBit(15, true);
        initial_mem.setBit(14, true);
        initial_mem.setBit(12, true);

        // **End parsing instruction**

        _outStream << initial_mem.ToHex().substr(2,4);

        _outStream << "\n";
      } else if (strcmp(inst, "ST") == 0) {
        // ST Instruction
        // Text Record
        _outStream << "T";

        // Print the address to be initialized
        _outStream << current_address.ToHex().substr(2,4);
        current_address++;

        // **Begin parsing instruction**
        Word initial_mem = new Word();

        // Set bits for opcode
        initial_mem.setBit(13, true);
        initial_mem.setBit(12, true);

        // Set bits for source register
        _SetBits(current_line[0], initial_mem, bit_offset);

        string op2 = current_line[1];

        if (symbols.Contains(op2)) {
          Word temp = symbols.GetLabelAddr(op2);

          while (bit_offset >= 0) {
            initial_mem.setBit(bit_offset, temp[bit_offset]);
            bit_offset--;
          }
        } else if (op2[0] == "#") {
          // Decimal operand
          int value = 0;
          bool neg = false;

          if(op2[1] == '-') {
            neg = true;
          }

          if(neg) {
            for (int i = 2; i < op2.length(); i++) {
              value *= 10;
              value += op2[i] - '0';
            }
            value *= -1;
          }
          else {
            for (int i = 1; i < op2.length(); i++) {
              value *= 10;
              value += op2[i] - '0';
            }
          }

          Word temp = new Word(value);

          while (bit_offset >= 0) {
            initial_mem.setBit(bit_offset, temp[bit_offset]);
            bit_offset--;
          }
        } else if (op2[0] == "x") {
          // Hex operand
          Word temp = new Word();
          temp.FromHexAbbr(op2.substr(1));

          while (bit_offset >= 0) {
            initial_mem.setBit(bit_offset, temp[bit_offset]);
            bit_offset--;
          }
        }
        // **End parsing instruction**

        _outStream << initial_mem.ToHex().substr(2,4);

        _outStream << "\n";
      } else if (strcmp(inst, "STI") == 0) {
        // STI Instruction
        // Text Record
        _outStream << "T";

        // Print the address to be initialized
        _outStream << current_address.ToHex().substr(2,4);
        current_address++;

        // **Begin parsing instruction**
        Word initial_mem = new Word();

        // Set bits for opcode
        initial_mem.setBit(15, true);
        initial_mem.setBit(13, true);
        initial_mem.setBit(12, true);

        // Set bits for source register
        _SetBits(current_line[0], initial_mem, bit_offset);

        string op2 = current_line[1];

        if (symbols.Contains(op2)) {
          Word temp = symbols.GetLabelAddr(op2);

          while (bit_offset >= 0) {
            initial_mem.setBit(bit_offset, temp[bit_offset]);
            bit_offset--;
          }
        } else if (op2[0] == "#") {
          // Decimal operand
          int value = 0;
          bool neg = false;

          if(op2[1] == '-') {
            neg = true;
          }

          if(neg) {
            for (int i = 2; i < op2.length(); i++) {
              value *= 10;
              value += op2[i] - '0';
            }
            value *= -1;
          }
          else {
            for (int i = 1; i < op2.length(); i++) {
              value *= 10;
              value += op2[i] - '0';
            }
          }

          Word temp = new Word(value);

          while (bit_offset >= 0) {
            initial_mem.setBit(bit_offset, temp[bit_offset]);
            bit_offset--;
          }
        } else if (op2[0] == "x") {
          // Hex operand
          Word temp = new Word();
          temp.FromHexAbbr(op2.substr(1));

          while (bit_offset >= 0) {
            initial_mem.setBit(bit_offset, temp[bit_offset]);
            bit_offset--;
          }
        }
        // **End parsing instruction**

        _outStream << initial_mem.ToHex().substr(2,4);

        _outStream << "\n";
      } else if (strcmp(inst, "STR") == 0) {
        // STR Instruction
        // Text Record
        _outStream << "T";

        // Print the address to be initialized
        _outStream << current_address.ToHex().substr(2,4);
        current_address++;

        // **Begin parsing instruction**
        Word initial_mem = new Word();

        // Set bits for opcode
        initial_mem.setBit(14, true);
        initial_mem.setBit(13, true);
        initial_mem.setBit(12, true);

        // Set bits for source register
        _SetBits(current_line[0], initial_mem, bit_offset);

        // Set bits for base register
        _SetBits(current_line[1], initial_mem, bit_offset);

        string op3 = current_line[1];

        if (symbols.Contains(op3)) {
          Word temp = symbols.GetLabelAddr(op3);

          while (bit_offset >= 0) {
            initial_mem.setBit(bit_offset, temp[bit_offset]);
            bit_offset--;
          }
        } else if (op3[0] == "#") {
          // Decimal operand
          int value = 0;
          bool neg = false;

          if(op3[1] == '-') {
            neg = true;
          }

          if(neg) {
            for (int i = 2; i < op3.length(); i++) {
              value *= 10;
              value += op3[i] - '0';
            }
            value *= -1;
          }
          else {
            for (int i = 1; i < op3.length(); i++) {
              value *= 10;
              value += op3[i] - '0';
            }
          }

          Word temp = new Word(value);

          while (bit_offset >= 0) {
            initial_mem.setBit(bit_offset, temp[bit_offset]);
            bit_offset--;
          }
        } else if (op3[0] == "x") {
          // Hex operand
          Word temp = new Word();
          temp.FromHexAbbr(op3.substr(1));

          while (bit_offset >= 0) {
            initial_mem.setBit(bit_offset, temp[bit_offset]);
            bit_offset--;
          }
        }
        // **End parsing instruction**

        _outStream << initial_mem.ToHex().substr(2,4);

        _outStream << "\n";
      } else if (strcmp(inst, "TRAP") == 0) {
        // TRAP Instruction
        // Text Record
        _outStream << "T";

        // Print the address to be initialized
        _outStream << current_address.ToHex().substr(2,4);
        current_address++;

        // **Begin parsing instruction**
        Word initial_mem = new Word();

        // Set bits for opcode
        initial_mem.setBit(15, true);
        initial_mem.setBit(14, true);
        initial_mem.setBit(13, true);
        initial_mem.setBit(12, true);

        bit_offset -= 4;

        string op = current_line[0];

        if (symbols.Contains(op)) {
          Word temp = symbols.GetLabelAddr(op);

          while (bit_offset >= 0) {
            initial_mem.setBit(bit_offset, temp[bit_offset]);
            bit_offset--;
          }
        } else if (op[0] == "#") {
          // Decimal operand
          int value = 0;
          bool neg = false;

          if(op[1] == '-') {
            neg = true;
          }

          if(neg) {
            for (int i = 2; i < op.length(); i++) {
              value *= 10;
              value += op[i] - '0';
            }
            value *= -1;
          }
          else {
            for (int i = 1; i < op.length(); i++) {
              value *= 10;
              value += op[i] - '0';
            }
          }

          Word temp = new Word(value);

          while (bit_offset >= 0) {
            initial_mem.setBit(bit_offset, temp[bit_offset]);
            bit_offset--;
          }
        } else if (op[0] == "x") {
          // Hex operand
          Word temp = new Word();
          temp.FromHexAbbr(op.substr(1));

          while (bit_offset >= 0) {
            initial_mem.setBit(bit_offset, temp[bit_offset]);
            bit_offset--;
          }
        }
        // **End parsing instruction**

        _outStream << initial_mem.ToHex().substr(2,4);

        _outStream << "\n";
      } else if (strcmp(inst.substr(0,2), "BR") {
        // BRx Instruction
        // Text Record
        _outStream << "T";

        // Print the address to be initialized
        _outStream << current_address.ToHex().substr(2,4);
        current_address++;

        // **Begin parsing instruction**
        Word initial_mem = new Word();

        // Set bits for opcode

        // Check CCR bits
        bool n, z, p;
        if (inst.find("N") != string::npos) {
          initial_mem.setBit(11, true);
        }
        bit_offset--;

        if (inst.find("Z") != string::npos) {
          initial_mem.setBit(10, true);
        }
        bit_offset--;

        if (inst.find("P") != string::npos) {
          initial_mem.setBit(9, true);
        }
        bit_offset--;

        string op = current_line[0];

        if (symbols.Contains(op)) {
          Word temp = symbols.GetLabelAddr(op);

          while (bit_offset >= 0) {
            initial_mem.setBit(bit_offset, temp[bit_offset]);
            bit_offset--;
          }
        } else if (op[0] == "#") {
          // Decimal operand
          int value = 0;
          bool neg = false;

          if(op[1] == '-') {
            neg = true;
          }

          if(neg) {
            for (int i = 2; i < op.length(); i++) {
              value *= 10;
              value += op[i] - '0';
            }
            value *= -1;
          }
          else {
            for (int i = 1; i < op.length(); i++) {
              value *= 10;
              value += op[i] - '0';
            }
          }

          Word temp = new Word(value);

          while (bit_offset >= 0) {
            initial_mem.setBit(bit_offset, temp[bit_offset]);
            bit_offset--;
          }
        } else if (op[0] == "x") {
          // Hex operand
          Word temp = new Word();
          temp.FromHexAbbr(op.substr(1));

          while (bit_offset >= 0) {
            initial_mem.setBit(bit_offset, temp[bit_offset]);
            bit_offset--;
          }
        }
        // **End parsing instruction**

        _outStream << initial_mem.ToHex().substr(2,4);

        _outStream << "\n";
      } else if (strcmp(inst, ".END") == 0) {
        //End Record
        _outStream << "E" << initial_load << "\n";
      }
    }
  }

  return RESULT(SUCCESS);
}
