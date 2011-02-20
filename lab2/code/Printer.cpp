// File:        Printer.cpp
// Written by:  Ryan Paulson
//              Andrew Groot

#include "Printer.h"
#include "Word.h"
#include "Line.h"
#include "SymbolTable.h"
#include "ResultCodes.h"
#include "itos.h"
#include <iostream>
#include <string>
#include <vector>
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
    case '0': for (int i = 0; i < 3; i++) {
                initial_mem.SetBit(bit_offset, false);
                bit_offset--;
              }
              break;
    case '1': for (int i = 0; i < 3; i++) {
                if (i != 2) {
                  initial_mem.SetBit(bit_offset, false);
                } else {
                  initial_mem.SetBit(bit_offset, true);
                }
                bit_offset--;
              }
              break;
    case '2': for (int i = 0; i < 3; i++) {
                if (i != 1) {
                  initial_mem.SetBit(bit_offset, false);
                } else {
                  initial_mem.SetBit(bit_offset, true);
                }
                bit_offset--;
              }
              break;
    case '3': for (int i = 0; i < 3; i++) {
                if (i == 0) {
                  initial_mem.SetBit(bit_offset, false);
                } else {
                  initial_mem.SetBit(bit_offset, true);
                }
                bit_offset--;
              }
              break;
    case '4': for (int i = 0; i < 3; i++) {
                if (i != 0) {
                  initial_mem.SetBit(bit_offset, false);
                } else {
                  initial_mem.SetBit(bit_offset, true);
                }
                bit_offset--;
              }
              break;
    case '5': for (int i = 0; i < 3; i++) {
                if (i == 1) {
                  initial_mem.SetBit(bit_offset, false);
                } else {
                  initial_mem.SetBit(bit_offset, true);
                }
                bit_offset--;
              }
              break;
    case '6': for (int i = 0; i < 3; i++) {
                if (i == 2) {
                  initial_mem.SetBit(bit_offset, false);
                } else {
                  initial_mem.SetBit(bit_offset, true);
                }
                bit_offset--;
              }
              break;
    case '7': for (int i = 0; i < 3; i++) {
                initial_mem.SetBit(bit_offset, true);
                bit_offset--;
              }
              break;
  }
}

Word Printer::_ParseWord(const string& op, const SymbolTable& symbols) {
  Word value;

  if (op[0] == '#') {
    // decimal operand
    value.FromInt(atoi(op.substr(1).c_str()));
  }
  else if (op[0] == 'x') {
    // Hex operand
    value.FromHexAbbr(op);
  } else {
    // label operand, perviously defined, already checked
    value = symbols.GetLabelAddr(op);
  }

  return value;
}

bool Printer::_Check9(Word value, Word PC) {
  // test first 7 bits
  Word temp1 = PC.And(Word(0xFE00));
  Word temp2 = PC.And(Word(0xFE00));
  return (temp1.ToInt() == temp2.ToInt());
}

bool Printer::_Check6(Word value) {
  return (value.ToInt() < 64);
}

bool Printer::_Check5(Word value) {
  return (value.ToInt2Complement() < 16 && value.ToInt2Complement() >= -16);
}

void Printer::_PreError(const string& line) {
  cout << "\n--- In line: " << line << "\n--- ";
}

void Printer::_LineListing(const Word& current_address, const Word& value, const Line& current_line, const int& pos) {
  cout << '(' << current_address.ToHex().substr(2) << ')'
              << ' ' << value.ToHex().substr(2) << "  " << value.ToStr() << ' '
              << _InFileData(pos, current_line);
}

string Printer::_InFileData(const int line_number, const Line& current_line) {
  return "(" + string(4 - itos(line_number).length(), ' ') + itos(line_number) + ") " + current_line.ToString() + '\n';
}

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
  string input_line;
  Word current_address(0);
  int initial_load = 0;
  int pos = 0;
  bool relocatable = false;

  while (_inStream.good()) {
    getline(_inStream, input_line);
    pos++; // line number

    Line current_line;
    current_line.ReadLine(input_line);

    int bit_offset = 11;
    const int listing_offset = 30;

    if (! current_line.IsComment()) {
      string inst = current_line.Instruction();

      if (inst == ".ORIG") {
        // Header record
        _outStream << "H" << current_line.Label() + string(6-current_line.Label().length(), ' ');

        if (current_line.Size() == 1) {
          Word addr;
          addr.FromHexAbbr(current_line[0]);

          // Initial load address
          _outStream << addr.ToHex().substr(2,4);
          current_address = current_address + addr;
          initial_load += addr.ToInt();
        } else {
          relocatable = true;
        }

        // Strip the 0x and print the segment length
        _outStream << file_length.ToHex().substr(2,4) << "\n";

        //*** listing output
        cout << string(listing_offset, ' ') << _InFileData(pos, current_line);

      } else if (inst == ".BLKW") {
        // Block pseudo-op
        string op = current_line[0];

        Word value = _ParseWord(op, symbols);

        //*** listing output
        cout << '(' << current_address.ToHex().substr(2) << ')'
              << string(listing_offset - 6, ' ')  // already printed 6 chars
              << _InFileData(pos, current_line);
        current_address = current_address + value;

      } else if (inst == ".FILL") {
        // Fill pseudo-op
        string op = current_line[0];
        Word value = _ParseWord(op, symbols);

        // Text Record
        if (symbols.IsRelocatable(op) && relocatable) {
          _outStream << 'W';
        } else {
          _outStream << 'T';
        }

        // Print address to be initialized
        _outStream << current_address.ToHex().substr(2,4);
        current_address++;

        // Print initial memory value
        _outStream << value.ToHex().substr(2,4) << '\n';

        //*** listing output
        _LineListing(current_address, value, current_line, pos);
        current_address++;

      } else if (inst == ".STRZ") {
        // String pseudo-op
        string op = current_line[0];

        for (int i = 1; i < op.length()-1; i++) {
          Word character((int) op[i]);

          // Text Record
          _outStream << 'T';

          // Print the address of the current character
          _outStream << current_address.ToHex().substr(2,4);

          // Print the hex representing the current character
          _outStream << character.ToHex().substr(2,4) << "\n";

          //*** listing output
          if (i == 1) {
            _LineListing(current_address, character, current_line, pos);
          } else {
            _LineListing(current_address, character, Line(), pos);
          }
        }

        // Text record for null character at string termination
        _outStream << 'T' << current_address.ToHex().substr(2,4) << "0000\n";

        //*** listing output 2 -- null character
        cout << '(' << current_address.ToHex().substr(2) << ')'
              << ' ' << string(4, '0') << "  " << string(16, '0') << ' '
              << _InFileData(pos, Line());
        current_address++;

      } else if (inst == "ADD" || inst == "AND") {
        // ADD-like instructions
        // Text Record
        _outStream << 'T';

        // Print address to be initialized
        _outStream << current_address.ToHex().substr(2,4);

        // **Begin parsing instruction**
        Word initial_mem;

        // Set bits for opcode
        initial_mem.SetBit(12, true);
        if (inst == "AND") {
          initial_mem.SetBit(14, true);
        }

        // Set bits for destination register
        if (current_line[0][0] == 'R') {
          _SetBits(current_line[0], initial_mem, bit_offset);
        } else if (symbols.Contains(current_line[0])) {
          // check label addr as register number
          int reg_num = symbols.GetLabelAddr(current_line[0]).ToInt();
          if (reg_num <= 7 && reg_num >= 0) {
            string reg = "R" + itos(reg_num);
            _SetBits(reg, initial_mem, bit_offset);
          } else {
            _PreError(current_line.ToString());
            return RESULT(INV_REG);
          }
        } else {
          _PreError(current_line.ToString());
          RESULT result(LBL_NOT_FOUND);
          result.info = current_line[0];
          return result;
        }

        // Set bits for source register 1
        if (current_line[1][0] == 'R') {
          _SetBits(current_line[1], initial_mem, bit_offset);
        } else if (symbols.Contains(current_line[1])) {
          // check label addr as register number
          int reg_num = symbols.GetLabelAddr(current_line[1]).ToInt();
          if (reg_num <= 7 && reg_num >= 0) {
            string reg = "R" + itos(reg_num);
            _SetBits(reg, initial_mem, bit_offset);
          } else {
            _PreError(current_line.ToString());
            return RESULT(INV_REG);
          }
        } else {
          _PreError(current_line.ToString());
          RESULT result(LBL_NOT_FOUND);
          result.info = current_line[1];
          return result;
        }

        string op3 = current_line[2];

        if (op3[0] == 'R') {
          // Set next 3 bits to 0
          _SetBits("R0", initial_mem, bit_offset);

          // Set bits for source register 2
          _SetBits(op3, initial_mem, bit_offset);

        } else {
          initial_mem.SetBit(bit_offset, true);
          bit_offset--;

          string op = current_line[0];
          Word value = _ParseWord(op, symbols);
          if (! _Check5(value)) {
            // invalid immediate
            _PreError(current_line.ToString());
            return RESULT(INV_IMM);
          }

          for( int i = 4; i >= 0; i--) {
            initial_mem.SetBit(i, value[i]);
          }
        }
        // **End parsing instruction**

        // Print the instruction in hex
        _outStream << initial_mem.ToHex().substr(2,4) << '\n';

        //*** listing output
        _LineListing(current_address, initial_mem, current_line, pos);
        current_address++;

      } else if (inst == "JSR" || inst == "JMP") {
        // JSR Instructions
        // Text Record
        if (relocatable) {
          _outStream << 'R';
        } else {
          _outStream << 'T';
        }

        // Print the address to be initialized
        _outStream << current_address.ToHex().substr(2,4);

        // **Begin parsing instruction**
        Word initial_mem;

        // Set bits for opcode
        initial_mem.SetBit(14, true);
        if (inst == "JSR") {
          // set link bit
          initial_mem.SetBit(11, true);
        }

        bit_offset -= 3;

        string op = current_line[0];
        Word value = _ParseWord(op, symbols);

        if (relocatable) {
          if (symbols.Contains(op) && !symbols.IsRelocatable(op)) {
            _PreError(current_line.ToString());
            RESULT result(ABS_REL);
            result.info = op;
            return result;
          }
        }

        while (bit_offset >= 0) {
          initial_mem.SetBit(bit_offset, value[bit_offset]);
          bit_offset--;
        }
        // **End parsing instruction**

        _outStream << initial_mem.ToHex().substr(2,4) << '\n';

        //*** listing output
        _LineListing(current_address, initial_mem, current_line, pos);
        current_address++;

      } else if (inst == "JSRR" || inst == "JMPR") {
        // JSRR Instructions
        // Text Record
        _outStream << 'T';

        _outStream << current_address.ToHex().substr(2,4);

        // **Begin parsing instruction**
        Word initial_mem;

        // Set bits for opcode
        initial_mem.SetBit(15, true);
        initial_mem.SetBit(14, true);
        if (inst == "JSRR") {
          // set link bit
          initial_mem.SetBit(11, true);
        }

        bit_offset -= 3;

        // Set bits for base register
        if (current_line[0][0] == 'R') {
          _SetBits(current_line[0], initial_mem, bit_offset);
        } else if (symbols.Contains(current_line[0])) {
          // check label addr as register number
          int reg_num = symbols.GetLabelAddr(current_line[0]).ToInt();
          if (reg_num <= 7 && reg_num >= 0) {
            string reg = "R" + itos(reg_num);
            _SetBits(reg, initial_mem, bit_offset);
          } else {
            _PreError(current_line.ToString());
            return RESULT(INV_REG);
          }
        } else {
          _PreError(current_line.ToString());
          RESULT result(LBL_NOT_FOUND);
          result.info = current_line[1];
          return result;
        }

        string op = current_line[1];

        Word value = _ParseWord(op, symbols);
        if (! _Check6(value)) {
          _PreError(current_line.ToString());
          // invalid index
          return RESULT(INV_IDX);
        }

        while (bit_offset >= 0) {
          initial_mem.SetBit(bit_offset, value[bit_offset]);
          bit_offset--;
        }
        // **End parsing instruction**

        _outStream << initial_mem.ToHex().substr(2,4) << '\n';

        //*** listing output
        _LineListing(current_address, initial_mem, current_line, pos);
        current_address++;

      } else if (inst == "LD" || inst == "LDI" || inst == "LEA" || inst == "ST" || inst == "STI") {
        // LD-like Instructions
        // Text Record
        if (relocatable) {
          _outStream << 'R';
        } else {
          _outStream << 'T';
        }

        // Print the address to be initialized
        _outStream << current_address.ToHex().substr(2,4);

        // **Begin parsing instruction**
        Word initial_mem;

        // Set bits for opcode
        initial_mem.SetBit(13, true);
        if (inst == "LDI") {
          initial_mem.SetBit(15, true);

        } else if (inst == "LEA") {
          initial_mem.SetBit(15, true);
          initial_mem.SetBit(14, true);

        } else if (inst == "ST") {
          initial_mem.SetBit(12, true);

        } else if (inst == "STI") {
          initial_mem.SetBit(15, true);
          initial_mem.SetBit(12, true);
        }

        // Set bits for destination register
        if (current_line[0][0] == 'R') {
          _SetBits(current_line[0], initial_mem, bit_offset);
        } else if (symbols.Contains(current_line[0])) {
          // check label addr as register number
          int reg_num = symbols.GetLabelAddr(current_line[0]).ToInt();
          if (reg_num <= 7 && reg_num >= 0) {
            string reg = "R" + itos(reg_num);
            _SetBits(reg, initial_mem, bit_offset);
          } else {
            _PreError(current_line.ToString());
            return RESULT(INV_REG);
          }
        } else {
          _PreError(current_line.ToString());
          RESULT result(LBL_NOT_FOUND);
          result.info = current_line[0];
          return result;
        }

        bit_offset -= 3;

        string op2 = current_line[1];
        Word value;
        if (current_line.HasLiteral()) {
          // LD only -- already checked
          value = symbols.GetLiteralAddr(current_line.Literal());

        } else if (symbols.Contains(op2)) {
          value = symbols.GetLabelAddr(op2);

          if (relocatable) {
            if (!symbols.IsRelocatable(op2)) {
              _PreError(current_line.ToString());
              RESULT result(ABS_REL);
              result.info = op2;
              return result;
            }
          }

        } else {
          if (relocatable) {
            _PreError(current_line.ToString());
            return RESULT(ABS_REL);
          }

          value = _ParseWord(op2, symbols);
        }
        if (! _Check9(value, current_address)) {
          _PreError(current_line.ToString());
          return RESULT(PG_ERR);
        }

        while (bit_offset >= 0) {
            initial_mem.SetBit(bit_offset, value[bit_offset]);
            bit_offset--;
        }
        // **End parsing instruction**

        _outStream << initial_mem.ToHex().substr(2,4) << '\n';

        //*** listing output
        _LineListing(current_address, initial_mem, current_line, pos);
        current_address++;

      } else if (inst == "LDR" || inst == "STR") {
        // LDR-like Instructions
        // Text Record
        _outStream << 'T';

        // Print the address to be initialized
        _outStream << current_address.ToHex().substr(2,4);

        // **Begin parsing instruction**
        Word initial_mem;

        // Set bits for opcode
        initial_mem.SetBit(13, true);
        initial_mem.SetBit(14, true);
        if (inst == "STR") {
          initial_mem.SetBit(12, true);
        }

        // Set bits for destination register
        if (current_line[0][0] == 'R') {
          _SetBits(current_line[0], initial_mem, bit_offset);
        } else if (symbols.Contains(current_line[0])) {
          // check label addr as register number
          int reg_num = symbols.GetLabelAddr(current_line[0]).ToInt();
          if (reg_num <= 7 && reg_num >= 0) {
            string reg = "R" + itos(reg_num);
            _SetBits(reg, initial_mem, bit_offset);
          } else {
            _PreError(current_line.ToString());
            return RESULT(INV_REG);
          }
        } else {
          _PreError(current_line.ToString());
          RESULT result(LBL_NOT_FOUND);
          result.info = current_line[0];
          return result;
        }

        // Set bits for base register
        if (current_line[1][0] == 'R') {
          _SetBits(current_line[1], initial_mem, bit_offset);
        } else if (symbols.Contains(current_line[1])) {
          // check label addr as register number
          int reg_num = symbols.GetLabelAddr(current_line[1]).ToInt();
          if (reg_num <= 7 && reg_num >= 0) {
            string reg = "R" + itos(reg_num);
            _SetBits(reg, initial_mem, bit_offset);
          } else {
            _PreError(current_line.ToString());
            return RESULT(INV_REG);
          }
        } else {
          _PreError(current_line.ToString());
          RESULT result(LBL_NOT_FOUND);
          result.info = current_line[1];
          return result;
        }

        bit_offset -= 6;

        string op3 = current_line[2];

        Word value = _ParseWord(op3, symbols);
        if (! _Check6(value)) {
          _PreError(current_line.ToString());
          return RESULT(INV_IDX);
        }

        while (bit_offset >= 0) {
          initial_mem.SetBit(bit_offset, value[bit_offset]);
          bit_offset--;
        }
        // **End parsing instruction**

        _outStream << initial_mem.ToHex().substr(2,4) << '\n';

        //*** listing output
        _LineListing(current_address, initial_mem, current_line, pos);
        current_address++;

      } else if (inst == "NOT") {
        // NOT Instruction
        // Text Record
        _outStream << 'T';

        // Print the address to be initialized
        _outStream << current_address.ToHex().substr(2,4);

        // **Begin parsing instruction**
        Word initial_mem;

        // Set bits for opcode
        initial_mem.SetBit(15, true);
        initial_mem.SetBit(12, true);

        // Set bits for destination register
        if (current_line[0][0] == 'R') {
          _SetBits(current_line[0], initial_mem, bit_offset);
        } else if (symbols.Contains(current_line[0])) {
          // check label addr as register number
          int reg_num = symbols.GetLabelAddr(current_line[0]).ToInt();
          if (reg_num <= 7 && reg_num >= 0) {
            string reg = "R" + itos(reg_num);
            _SetBits(reg, initial_mem, bit_offset);
          } else {
            _PreError(current_line.ToString());
            return RESULT(INV_REG);
          }
        } else {
          _PreError(current_line.ToString());
          RESULT result(LBL_NOT_FOUND);
          result.info = current_line[0];
          return result;
        }

        // Set bits for source register
        if (current_line[1][0] == 'R') {
          _SetBits(current_line[1], initial_mem, bit_offset);
        } else if (symbols.Contains(current_line[1])) {
          // check label addr as register number
          int reg_num = symbols.GetLabelAddr(current_line[1]).ToInt();
          if (reg_num <= 7 && reg_num >= 0) {
            string reg = "R" + itos(reg_num);
            _SetBits(reg, initial_mem, bit_offset);
          } else {
            _PreError(current_line.ToString());
            return RESULT(INV_REG);
          }
        } else {
          _PreError(current_line.ToString());
          RESULT result(LBL_NOT_FOUND);
          result.info = current_line[1];
          return result;
        }

        // **End parsing instruction**

        _outStream << initial_mem.ToHex().substr(2,4) << '\n';

        //*** listing output
        _LineListing(current_address, initial_mem, current_line, pos);
        current_address++;

      } else if (inst == "DBUG" || inst == "RET") {
        // DEBUG/RET Instructions
        // Text Record
        _outStream << 'T';

        // Print the address to be initialized
        _outStream << current_address.ToHex().substr(2,4);

        // **Begin parsing instruction**
        Word initial_mem;

        // Set bits for opcode
        initial_mem.SetBit(15, true);
        if (inst == "RET") {
          initial_mem.SetBit(14, true);
          initial_mem.SetBit(12, true);
        }

        // **End parsing instruction**

        _outStream << initial_mem.ToHex().substr(2,4) << _outStream << '\n';

        //*** listing output
        _LineListing(current_address, initial_mem, current_line, pos);
        current_address++;

      } else if (inst == "TRAP") {
        // TRAP Instruction
        // Text Record
        _outStream << 'T';

        // Print the address to be initialized
        _outStream << current_address.ToHex().substr(2,4);

        // **Begin parsing instruction**
        Word initial_mem;

        // Set bits for opcode
        initial_mem.SetBit(15, true);
        initial_mem.SetBit(14, true);
        initial_mem.SetBit(13, true);
        initial_mem.SetBit(12, true);

        bit_offset -= 4;

        string op = current_line[0];

        Word value = _ParseWord(op, symbols);

        while (bit_offset >= 0) {
          initial_mem.SetBit(bit_offset, value[bit_offset]);
          bit_offset--;
        }
        // **End parsing instruction**

        _outStream << initial_mem.ToHex().substr(2,4) << '\n';

        //*** listing output
        _LineListing(current_address, initial_mem, current_line, pos);
        current_address++;

      } else if (inst.substr(0,2) == "BR") {
        // BRx Instruction
        // Text Record
        if (relocatable) {
          _outStream << 'R';
        } else {
          _outStream << 'T';
        }

        // Print the address to be initialized
        _outStream << current_address.ToHex().substr(2,4);

        // **Begin parsing instruction**
        Word initial_mem;

        // Set bits for opcode

        // Check CCR bits
        bool n, z, p;
        if (inst.find('N') != string::npos) {
          initial_mem.SetBit(11, true);
        }
        bit_offset--;

        if (inst.find('Z') != string::npos) {
          initial_mem.SetBit(10, true);
        }
        bit_offset--;

        if (inst.find('P') != string::npos) {
          initial_mem.SetBit(9, true);
        }
        bit_offset--;

        string op = current_line[0];

        Word value = _ParseWord(op, symbols);
        if (! _Check9(value, current_address)) {
          _PreError(current_line.ToString());
          return RESULT(PG_ERR);
        }

        while (bit_offset >= 0) {
          initial_mem.SetBit(bit_offset, value[bit_offset]);
          bit_offset--;
        }
        // **End parsing instruction**

        _outStream << initial_mem.ToHex().substr(2,4) << '\n';

        //*** listing output
        _LineListing(current_address, initial_mem, current_line, pos);
        current_address++;

      } else if (inst == ".END") {
        //End Record

        // done with file, output literals
        const map<int, Word>* literals = symbols.GetLiterals();
        map<int, Word>::const_iterator it = literals->begin();

        while (it != literals->end()) {
          Word value(it->first);
          // object file output
          _outStream << 'T' << current_address.ToHex().substr(2) << value.ToHex().substr(2) << '\n';
          //*** listing output 1
          cout << '(' << current_address.ToHex().substr(2) << ')'
                << ' ' << value.ToHex().substr(2) << "  " << value.ToStr() << " ( lit)\n";
          current_address++;
          it++;
        }

        Word load(initial_load);
        _outStream << 'E' << load.ToHex().substr(2) << '\n';

        //*** listing output 2
        cout << string(listing_offset, ' ') << _InFileData(pos, current_line);

        return RESULT(SUCCESS);
      }
    }
  }

  return RESULT(NO_END);
}
